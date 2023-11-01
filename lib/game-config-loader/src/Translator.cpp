#include <stdexcept>
#include "Translator.h"


std::unique_ptr<RuleTree> 
Translator::translate(const ts::Node& root) const {
    ts::Node rootBodyNode = root.getChildByFieldName("rules").getNamedChild(0);
    return std::make_unique<RuleTree>(createRule(rootBodyNode));
}


std::unique_ptr<Rule>
Translator::createRule(const ts::Node& node) const {
    std::string_view type = node.getType();
    auto factory = ruleFactories.find(std::string{type});

    if (factory == ruleFactories.end()) {
        throw std::runtime_error("Unable to create rule");
    }

    return factory->second->create(node, source);
}


std::unique_ptr<Expression>
Translator::createExpression(const ts::Node& node) const {
    std::string_view type = node.getType();
    auto factory = expressionFactories.find(std::string{type});

    if (factory == expressionFactories.end()) {
        throw std::runtime_error("Unable to create expression");
    }

    return factory->second->create(node, source);
}


Translator 
createTranslator(std::string_view source) {
    Translator translator{source};

    // Rules
    translator.registerRuleFactory("body",      std::make_unique<BodyFactory>(&translator));
    translator.registerRuleFactory("for",       std::make_unique<ForFactory>(&translator));
    translator.registerRuleFactory("match",     std::make_unique<MatchFactory>(&translator));
    translator.registerRuleFactory("discard",   std::make_unique<DiscardFactory>(&translator));
    translator.registerRuleFactory("message",   std::make_unique<MessageFactory>(&translator));

    // Expressions
    translator.registerExpressionFactory("expression",  std::make_unique<BaseExpressionFactory>(&translator));
    translator.registerExpressionFactory("identifier",  std::make_unique<IdentifierFactory>(&translator));

    return translator;
};