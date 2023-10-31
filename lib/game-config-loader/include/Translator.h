// Translates a ts::Tree into a tree of our own Rule and Expression business objects
#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <cpp-tree-sitter.h>
#include "Rule.h"


// A RuleTree is just the root BodyRule (a list of rules)
class RuleTree final {
public:
    RuleTree(std::unique_ptr<BodyRule> root)
        : root{std::move(root)}
    {}

private:
    std::unique_ptr<BodyRule> root;
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
private:
    std::unique_ptr<BodyRule> 
    createImpl(const ts::Node& node, std::string_view source) {
        // Create the body rule here
        return std::make_unique<BodyRule>();
    }
};


class ForFactory final : public RuleFactory {
private:
    std::unique_ptr<ForRule>
    createImpl(const ts::Node& node, std::string_view source) {
        // Create the for rule here
        return std::make_unique<ForRule>();
    }
}


class MatchFactory final : public RuleFactory {
private:
    std::unique_ptr<ForRule>
    createImpl(const ts::Node& node, std::string_view source) {
        // Create the match rule here
        return std::make_unique<MatchRule>();
    }
}


class DiscardFactory final : public RuleFactory {
private:
    std::unique_ptr<ForRule>
    createImpl(const ts::Node& node, std::string_view source) {
        // Create the discard rule here
        return std::make_unique<DiscardRule>();
    }
}


class MessageFactory final : public RuleFactory {
private:
    std::unique_ptr<ForRule>
    createImpl(const ts::Node& node, std::string_view source) {
        auto messageRule = std::make_unique<MessageRule>();
        messageRule->message = translator->createExpression(node.getNamedChild(0));
        messageRule->toList = translator->createExpression(node.getNamedChild(1));
    }
}


////////////////////////////////////////////////////////////////////////////////
// Expressions
////////////////////////////////////////////////////////////////////////////////


// Base class for expression factories
class ExpressionFactory {
public:
    explicit ExpressionFactory(const Translator* translator) 
        : translator{translator}
    {}
    
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
private:
    std::unique_ptr<Expression>
    createImpl(const ts::Node& node, std::string_view source) {
        return translator->createExpression(node.getNamedChild(0)); 
    } 
}


class IdentifierFactory final : public ExpressionFactory {
private:
    std::unique_ptr<Expression>
    createImpl(const ts::Node& node, std::string_view source) {
        return std::make_unique<IdentifierExpression>(node.getSourceRange(source));
    }
}


////////////////////////////////////////////////////////////////////////////////
// Translator
////////////////////////////////////////////////////////////////////////////////


class Translator final {
public:
    using RuleFactoryPointer = std::unique_ptr<RuleFactory>;
    using ExpressionFactoryPointer = std::unique_ptr<ExpressionFactory>;

    Translator(std::string_view source) : source{source} {}
    
    std::unique_ptr<Rule> createRule(const ts::Node& node);
    std::unique_ptr<Expression> createExpression(const ts::Node& node);
    
    void 
    registerRuleFactory(std::string key, FactoryPointer factory) {
        ruleFactories[key] = factory;
    };

    void
    registerExpressionFactory(std::string key, ExpressionFactoryPointer factory) {
        expressionFactories[key] = factory;
    }

private:
    std::unordered_map<std::string, RuleFactoryPointer> ruleFactories;
    std::unordered_map<std::string, ExpressionFactoryPointer> expressionFactories;
    std::string_view source;
};


Translator 
createTranslator(std::string_view source) {
    Translator translator{source};

    // Rules
    translator.registerRuleFactory("body",      std::make_unique<BodyFactory>(translator));
    translator.registerRuleFactory("for",       std::make_unique<ForFactory>(translator));
    translator.registerRuleFactory("match",     std::make_unique<MatchFactory>(translator));
    translator.registerRuleFactory("discard",   std::make_unique<DiscardFactory>(translator));
    translator.registerRuleFactory("message",   std::make_unique<MessageFactory>(translator));

    // Expressions
    translator.registerExpressionFactory("expression",  std::make_unique<BaseExpressionFactory>(translator));
    translator.registerExpressionFactory("identifier",  std::make_unique<IdentifierFactory>(translator));

    return translator;
}