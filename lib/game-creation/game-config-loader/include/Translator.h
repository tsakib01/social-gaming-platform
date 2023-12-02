// Translates a ts::Tree into a tree of our own Rule and Expression business objects
#ifndef TRANSLATOR
#define TRANSLATOR

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <cpp-tree-sitter.h>
#include <iostream>
#include "Rule.h"
#include "Expression.h"
#include "GameStateLoader.h"


class Translator;

// A RuleTree is a thin wrapper around the root BodyRule (a list of rules)
class RuleTree final {
public:
    RuleTree(std::unique_ptr<Rule> root)
        : root{std::move(root)}
    {}

    Rule* getRoot() {
        return root.get();
    }

private:
    std::unique_ptr<Rule> root;
};


////////////////////////////////////////////////////////////////////////////////
// Rules
////////////////////////////////////////////////////////////////////////////////


// Base class for rule factories
class RuleFactory {
public:
    explicit RuleFactory(const Translator* translator) 
        : translator{translator}
    {}

    virtual ~RuleFactory() = default;
    
    std::unique_ptr<Rule> 
    create(const ts::Node& node) {
        return createImpl(node);
    }

protected:
    // Some Rules need to be created recursively, so the factory needs access to the 
    // translator that it is part of
    const Translator* translator;

private:
    virtual std::unique_ptr<Rule> createImpl(const ts::Node&) = 0;
};

// Creates the child rule of this node for nodes that look like
//      (rule
//         (some child rule)
//      )
class DummyRuleFactory final : public RuleFactory {
public:
    DummyRuleFactory(const Translator* translator) : RuleFactory(translator) {}
private:
    std::unique_ptr<Rule> createImpl(const ts::Node& node);
};


class BodyFactory final : public RuleFactory {
public:
    BodyFactory(const Translator* translator) : RuleFactory(translator) {}
    static BodyRule createBodyRule(const ts::Node& node, const Translator* translator);
private:
    std::unique_ptr<Rule> createImpl(const ts::Node& node);
};

class ForFactory final : public RuleFactory {
public:
    ForFactory(const Translator* translator) : RuleFactory(translator) {}

private:
    std::unique_ptr<Rule> createImpl(const ts::Node& node);
};

class ParallelForFactory final : public RuleFactory {
public:
    ParallelForFactory(const Translator* translator) : RuleFactory(translator) {}
private:
    std::unique_ptr<Rule> createImpl(const ts::Node& node);
};


class MatchFactory final : public RuleFactory {
public:
    MatchFactory(const Translator* translator) : RuleFactory(translator) {}
private:
    std::unique_ptr<Rule> createImpl(const ts::Node& node); 
};


class DiscardFactory final : public RuleFactory {
public:
    DiscardFactory(const Translator* translator) : RuleFactory(translator) {}
private:
    std::unique_ptr<Rule> createImpl(const ts::Node& node);
};


class MessageFactory final : public RuleFactory {
public:
    MessageFactory(const Translator* translator) : RuleFactory(translator) {}
private:
    std::unique_ptr<Rule> createImpl(const ts::Node& node);
};


class InputChoiceFactory final : public RuleFactory {
public:
    InputChoiceFactory(const Translator* translator) : RuleFactory(translator) {}
private:
    std::unique_ptr<Rule> createImpl(const ts::Node& node);
};


class ScoresFactory final : public RuleFactory {
public:
    ScoresFactory(const Translator* translator) : RuleFactory(translator) {}
private:
    std::unique_ptr<Rule> createImpl(const ts::Node& node);
};


class ExtendFactory final : public RuleFactory {
public:
    ExtendFactory(const Translator* translator) : RuleFactory(translator) {}
private:
    std::unique_ptr<Rule> createImpl(const ts::Node& node);
};


class AssignmentFactory final : public RuleFactory {
public:
    AssignmentFactory(const Translator* translator) : RuleFactory(translator) {}
private:
    std::unique_ptr<Rule> createImpl(const ts::Node& node);
};

////////////////////////////////////////////////////////////////////////////////
// Expressions
////////////////////////////////////////////////////////////////////////////////


// Base class for expression factories
class ExpressionFactory {
public:
    explicit ExpressionFactory(const Translator* translator) 
        : translator{translator}
    {}
    
    virtual ~ExpressionFactory() = default;

    std::unique_ptr<Expression> 
    create(const ts::Node& node) {
        return createImpl(node);
    }

protected:
    // Expressions need to be created recursively, so factories need access to 
    // the translator they are part of
    const Translator* translator;

private:
    virtual std::unique_ptr<Expression> createImpl(const ts::Node&) = 0;
};


// Creates the expression for general expression nodes
//      (expression
//          ... 
//      )
// Which can be either          For example,
// 1) a literal expression      quoted_string, number, etc.
// 2) a unary expression        !players.elements...
// 3) a binary expression       players.size() || winners.size()
// 4) a dot expression          configuration.rounds.upfrom(1)
class DummyExpressionFactory final : public ExpressionFactory {
public:
    DummyExpressionFactory(const Translator* translator) : ExpressionFactory(translator) {}
private:
    std::unique_ptr<Expression> createImpl(const ts::Node& node); 
};


// Literal Expression Factories

class IdentifierFactory final : public ExpressionFactory {
public:
    IdentifierFactory(const Translator* translator) : ExpressionFactory(translator) {}
private:
    std::unique_ptr<Expression> createImpl(const ts::Node& node); 
};


class BooleanFactory final : public ExpressionFactory {
public:
    BooleanFactory(const Translator* translator) : ExpressionFactory(translator) {}
private:
    std::unique_ptr<Expression> createImpl(const ts::Node& node); 
};


class QuotedStringFactory final : public ExpressionFactory {
public:
    QuotedStringFactory(const Translator* translator) : ExpressionFactory(translator) {}
private:
    std::unique_ptr<Expression> createImpl(const ts::Node& node); 
};


class ListLiteralFactory final : public ExpressionFactory {
public:
    ListLiteralFactory(const Translator* translator) : ExpressionFactory(translator) {}
private:
    std::unique_ptr<Expression> createImpl(const ts::Node& node); 
};

class NumberFactory final : public ExpressionFactory {
public:
    NumberFactory(const Translator* translator) : ExpressionFactory(translator) {}
private:
    std::unique_ptr<Expression> createImpl(const ts::Node& node); 
};

// TODO: value_map literal expression


////////////////////////////////////////////////////////////////////////////////
// Translator
////////////////////////////////////////////////////////////////////////////////


class Translator {
public:
    using RuleFactoryPointer = std::unique_ptr<RuleFactory>;
    using ExpressionFactoryPointer = std::unique_ptr<ExpressionFactory>;

    Translator(std::string_view source) : source{source} {
        gsl = GameStateLoader::createDefaultGameStateLoader(source);
    }
    
    std::unique_ptr<RuleTree> translate(const ts::Node& root) const;
    std::unique_ptr<Rule> createRule(const ts::Node& node) const;
    std::unique_ptr<Expression> createExpression(const ts::Node& node) const;
    
    void 
    registerRuleFactory(std::string key, RuleFactoryPointer factory) {
        ruleFactories[key] = std::move(factory);
    };

    void
    registerExpressionFactory(std::string key, ExpressionFactoryPointer factory) {
        expressionFactories[key] = std::move(factory);
    }

    std::string_view source = {};

    // Borrowing from gameStateLoader for it's methods to convert from node -> GameEnvironment::Value
    std::unique_ptr<GameStateLoader> gsl = nullptr;

private:
    std::unordered_map<std::string, RuleFactoryPointer> ruleFactories;
    std::unordered_map<std::string, ExpressionFactoryPointer> expressionFactories;
};


Translator createTranslator(std::string_view source);

#endif