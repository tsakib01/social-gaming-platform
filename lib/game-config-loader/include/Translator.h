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


class Translator;

// A RuleTree is just the root BodyRule (a list of rules)
class RuleTree final {
public:
    RuleTree(std::unique_ptr<Rule> root)
        : root{std::move(root)}
    {}

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
    create(const ts::Node& node, std::string_view source) {
        return createImpl(node, source);
    }

protected:
    // Some Rules need to be created recursively, so they need access to translator
    const Translator* translator;

private:
    virtual std::unique_ptr<Rule> createImpl(const ts::Node&, std::string_view) = 0;
};


class BodyFactory final : public RuleFactory {
public:
    BodyFactory(const Translator* translator) : RuleFactory(translator) {}
private:
    std::unique_ptr<Rule> 
    createImpl(const ts::Node& node, std::string_view source) {
        // Create the body rule here
        std::cout << "Body Rule Created\n";
        return std::make_unique<BodyRule>();
    }
};


class ForFactory final : public RuleFactory {
public:
    ForFactory(const Translator* translator) : RuleFactory(translator) {}
private:
    std::unique_ptr<Rule>
    createImpl(const ts::Node& node, std::string_view source) {
        // Create the for rule here
        std::cout << "For Rule Created\n";
        return std::make_unique<ForRule>();
    }
};


class MatchFactory final : public RuleFactory {
public:
    MatchFactory(const Translator* translator) : RuleFactory(translator) {}
private:
    std::unique_ptr<Rule>
    createImpl(const ts::Node& node, std::string_view source) {
        // Create the match rule here
        return std::make_unique<MatchRule>();
    }
};


class DiscardFactory final : public RuleFactory {
public:
    DiscardFactory(const Translator* translator) : RuleFactory(translator) {}
private:
    std::unique_ptr<Rule>
    createImpl(const ts::Node& node, std::string_view source) {
        // Create the discard rule here
        return std::make_unique<DiscardRule>();
    }
};


class MessageFactory final : public RuleFactory {
public:
    MessageFactory(const Translator* translator) : RuleFactory(translator) {}
private:
    std::unique_ptr<Rule>
    createImpl(const ts::Node& node, std::string_view source) {
        // auto messageRule = std::make_unique<MessageRule>();
        // messageRule->message = translator->createExpression(node.getNamedChild(0));
        // messageRule->toList = translator->createExpression(node.getNamedChild(1));
    }
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
    create(const ts::Node& node, std::string_view source) {
        return createImpl(node, source);
    }

protected:
    // Expressions need to be created recursively, so they need access to translator
    const Translator* translator;

private:
    virtual std::unique_ptr<Expression> createImpl(const ts::Node&, std::string_view) = 0;
};


// Just creates the child expression of this node for nodes that look like
// (expression
//      (some child expression)
// )
class BaseExpressionFactory final : public ExpressionFactory {
public:
    BaseExpressionFactory(const Translator* translator) : ExpressionFactory(translator) {}
private:
    std::unique_ptr<Expression>
    createImpl(const ts::Node& node, std::string_view source) {
        // return translator->createExpression(node.getNamedChild(0)); 
        return {};
    } 
};


class IdentifierFactory final : public ExpressionFactory {
public:
    IdentifierFactory(const Translator* translator) : ExpressionFactory(translator) {}
private:
    std::unique_ptr<Expression>
    createImpl(const ts::Node& node, std::string_view source) {
        return std::make_unique<IdentifierExpression>(node.getSourceRange(source));
    }
};


////////////////////////////////////////////////////////////////////////////////
// Translator
////////////////////////////////////////////////////////////////////////////////


class Translator final {
public:
    using RuleFactoryPointer = std::unique_ptr<RuleFactory>;
    using ExpressionFactoryPointer = std::unique_ptr<ExpressionFactory>;

    Translator(std::string_view source) : source{source} {}
    
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

private:
    std::unordered_map<std::string, RuleFactoryPointer> ruleFactories;
    std::unordered_map<std::string, ExpressionFactoryPointer> expressionFactories;
    std::string_view source;
};


Translator createTranslator(std::string_view source);

#endif