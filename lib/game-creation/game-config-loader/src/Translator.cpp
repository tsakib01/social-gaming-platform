#include <stdexcept>
#include "Translator.h"


////////////////////////////////////////////////////////////////////////////////
// Rules
////////////////////////////////////////////////////////////////////////////////


std::unique_ptr<Rule>
DummyRuleFactory::createImpl(const ts::Node& node, std::string_view source) {
    return translator->createRule(node.getNamedChild(0)); 
} 


std::unique_ptr<Rule> 
BodyFactory::createImpl(const ts::Node& node, std::string_view source) {
    std::cout << "Body Rule Created\n";
    auto bodyRule = std::make_unique<BodyRule>();

    for (size_t i = 0; i < node.getNumNamedChildren(); ++i) {
        if (node.getNamedChild(i).getType() == "rule") {
            bodyRule->rules.push_back(translator->createRule(node.getNamedChild(i)));
        }
    }

    return bodyRule;
}

BodyRule
BodyFactory::createBodyRule(const ts::Node& node, const Translator* translator) {
    std::cout << "Body Rule Created\n";
    BodyRule bodyRule;

    for (size_t i = 0; i < node.getNumNamedChildren(); ++i) {
        if (node.getNamedChild(i).getType() == "rule") {
            bodyRule.rules.push_back(translator->createRule(node.getNamedChild(i)));
        }
    }

    return bodyRule;
}


std::unique_ptr<Rule>
ForFactory::createImpl(const ts::Node& node, std::string_view source) {
    // Create the for rule here
    std::cout << "For Rule Created\n";
    std::unique_ptr<ForRule> rule = std::make_unique<ForRule>();
    rule->currentItem = IdentifierExpression(node.getNamedChild(0).getSourceRange(source));
    rule->list = translator->createExpression(node.getNamedChild(1));
    rule->body = BodyFactory::createBodyRule(node.getNamedChild(2), translator);
    return rule;
}

std::unique_ptr<Rule>
ParallelForFactory::createImpl(const ts::Node& node, std::string_view source) {
    // Create the for rule here
    std::cout << "Parallel For Rule Created\n";
    std::unique_ptr<ParallelForRule> rule = std::make_unique<ParallelForRule>();
    rule->currentItem = IdentifierExpression(node.getNamedChild(0).getSourceRange(source));
    rule->list = translator->createExpression(node.getNamedChild(1));
    rule->body = BodyFactory::createBodyRule(node.getNamedChild(2), translator);
    return rule;
}

std::unique_ptr<Rule>
MatchFactory::createImpl(const ts::Node& node, std::string_view source) {
    // Create the match rule here
    return std::make_unique<MatchRule>();
}


std::unique_ptr<Rule>
DiscardFactory::createImpl(const ts::Node& node, std::string_view source) {
    // Create the discard rule here
    return std::make_unique<DiscardRule>();
}


std::unique_ptr<Rule>
MessageFactory::createImpl(const ts::Node& node, std::string_view source) {
    std::cout << "Message Rule Created\n";
    return std::make_unique<MessageRule>();
    // auto messageRule = std::make_unique<MessageRule>();
    // messageRule->message = translator->createExpression(node.getNamedChild(0));
    // messageRule->toList = translator->createExpression(node.getNamedChild(1));
}

std::unique_ptr<Rule>
InputChoiceFactory::createImpl(const ts::Node& node, std::string_view source) {
    std::cout << "Input Rule Created\n";
    return std::make_unique<InputChoiceRule>();
}

std::unique_ptr<Rule>
ScoresFactory::createImpl(const ts::Node& node, std::string_view source) {
    std::cout << "Scores Rule Created\n";
    return std::make_unique<ScoresRule>();
}


////////////////////////////////////////////////////////////////////////////////
// Expressions
////////////////////////////////////////////////////////////////////////////////


std::unique_ptr<Expression>
DummyExpressionFactory::createImpl(const ts::Node& node, std::string_view source) {
    return translator->createExpression(node.getNamedChild(0)); 
} 


std::unique_ptr<Expression>
IdentifierFactory::createImpl(const ts::Node& node, std::string_view source) {
    return std::make_unique<IdentifierExpression>(node.getSourceRange(source));
}


////////////////////////////////////////////////////////////////////////////////
// Translator
////////////////////////////////////////////////////////////////////////////////


Translator 
createTranslator(std::string_view source) {
    Translator translator{source};

    // Rules
    translator.registerRuleFactory("rule",          std::make_unique<DummyRuleFactory>(&translator));
    translator.registerRuleFactory("body",          std::make_unique<BodyFactory>(&translator));
    translator.registerRuleFactory("for",           std::make_unique<ForFactory>(&translator));
    translator.registerRuleFactory("parallel_for",  std::make_unique<ParallelForFactory>(&translator));
    translator.registerRuleFactory("match",         std::make_unique<MatchFactory>(&translator));
    translator.registerRuleFactory("discard",       std::make_unique<DiscardFactory>(&translator));
    translator.registerRuleFactory("message",       std::make_unique<MessageFactory>(&translator));
    translator.registerRuleFactory("input_choice",  std::make_unique<InputChoiceFactory>(&translator));
    translator.registerRuleFactory("scores",        std::make_unique<ScoresFactory>(&translator));

    // Expressions
    translator.registerExpressionFactory("expression",  std::make_unique<DummyExpressionFactory>(&translator));
    translator.registerExpressionFactory("identifier",  std::make_unique<IdentifierFactory>(&translator));

    return translator;
};


std::unique_ptr<RuleTree> 
Translator::translate(const ts::Node& root) const {
    return std::make_unique<RuleTree>(createRule(root));
}


std::unique_ptr<Rule>
Translator::createRule(const ts::Node& node) const {
    std::string_view type = node.getType();
    auto factory = ruleFactories.find(std::string{type});

    if (factory == ruleFactories.end()) {
        std::string errorMessage = "Unable to create rule type: " + std::string{type};
        throw std::runtime_error(errorMessage);
    }

    return factory->second->create(node, source);
}


std::unique_ptr<Expression>
Translator::createExpression(const ts::Node& node) const {
    std::string_view type = node.getType();
    auto factory = expressionFactories.find(std::string{type});

    if (factory == expressionFactories.end()) {
        throw std::runtime_error("Unable to create expression of type: "+ std::string{type});
    }

    return factory->second->create(node, source);
}