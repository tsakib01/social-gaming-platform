#include <stdexcept>
#include "Translator.h"
#include <iostream>


////////////////////////////////////////////////////////////////////////////////
// Rules
////////////////////////////////////////////////////////////////////////////////


std::unique_ptr<Rule>
DummyRuleFactory::createImpl(const ts::Node& node, [[maybe_unused]] std::string_view source) {
    return translator->createRule(node.getNamedChild(0)); 
} 


std::unique_ptr<Rule> 
BodyFactory::createImpl(const ts::Node& node, [[maybe_unused]] std::string_view source) {
    std::cout << "Body Rule Created\n";
    auto bodyRule = std::make_unique<BodyRule>();
    ts::Cursor cursor = node.getCursor();

    if (!cursor.gotoFirstChild()) {
        throw std::runtime_error("Error traversing tree in BodyFactory");
    }

    do {
        ts::Node current = cursor.getCurrentNode();
        if (current.getType() == "rule") {
            bodyRule->rules.push_back(translator->createRule(current));
        }
    } while (cursor.gotoNextSibling());

    return bodyRule;
}

BodyRule
BodyFactory::createBodyRule(const ts::Node& node, const Translator* translator) {
    std::cout << "Body Rule Created\n";
    BodyRule bodyRule;
    ts::Cursor cursor = node.getCursor();

    if (!cursor.gotoFirstChild()) {
        throw std::runtime_error("Error traversing tree in BodyFactory");
    }

    do {
        ts::Node current = cursor.getCurrentNode();
        if (current.getType() == "rule") {
            bodyRule.rules.push_back(translator->createRule(current));
        }
    } while (cursor.gotoNextSibling());

    return bodyRule;
}


std::unique_ptr<Rule>
ForFactory::createImpl(const ts::Node& node, std::string_view source) {
    std::cout << "For Rule Created\n";
    auto rule = std::make_unique<ForRule>();
    rule->currentItem = IdentifierExpression(node.getNamedChild(0).getSourceRange(source));
    rule->list = translator->createExpression(node.getNamedChild(1));
    rule->body = BodyFactory::createBodyRule(node.getNamedChild(2), translator);
    return rule;
}


std::unique_ptr<Rule>
ParallelForFactory::createImpl(const ts::Node& node, std::string_view source) {
    std::cout << "Parallel For Rule Created\n";
    auto rule = std::make_unique<ParallelForRule>();
    rule->currentItem = IdentifierExpression(node.getNamedChild(0).getSourceRange(source));
    rule->list = translator->createExpression(node.getNamedChild(1));
    rule->body = BodyFactory::createBodyRule(node.getNamedChild(2), translator);
    return rule;
}


std::unique_ptr<Rule>
MatchFactory::createImpl(const ts::Node& node, std::string_view source) {
    std::cout << "Match Rule Created\n";
    auto rule = std::make_unique<MatchRule>();
    rule->target = translator->createExpression(node.getChildByFieldName("target"));

    ts::Cursor cursor = node.getCursor();

    if (!cursor.gotoFirstChild()) {
        throw std::runtime_error("Error traversing tree in MatchFactory");
    }

    do {
        ts::Node current = cursor.getCurrentNode();
        if (current.getType() == "match_entry") {
            auto guard = translator->createExpression(current.getChildByFieldName("guard"));
            auto body = BodyFactory::createBodyRule(current.getChildByFieldName("body"), translator);
            rule->cases[std::move(guard)] = std::move(body);
        }
    } while (cursor.gotoNextSibling());

    return rule;
}


std::unique_ptr<Rule>
DiscardFactory::createImpl(const ts::Node& node, std::string_view source) {
    std::cout << "Discard Rule Created\n";
    auto rule = std::make_unique<DiscardRule>();

    rule->count = translator->createExpression(node.getChildByFieldName("count"));
    rule->source = QualifiedIdentifier{node.getChildByFieldName("source").getSourceRange(source)};

    return rule;
}


std::unique_ptr<Rule>
MessageFactory::createImpl(const ts::Node& node, std::string_view source) {
    std::cout << "Message Rule Created\n";
    auto rule = std::make_unique<MessageRule>();

    ts::Node players = node.getChildByFieldName("players");
    if (players.getSourceRange(source) == "all") {
        rule->players = std::make_unique<LiteralExpression<std::string_view>>("all");
    } else {
        rule->players = translator->createExpression(players);
    }
    
    rule->content = LiteralExpression<std::string_view> {
        node.getChildByFieldName("content")
            .getNamedChild(0)
            .getSourceRange(source)
    };

    return rule;

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


std::unique_ptr<Rule>
ExtendFactory::createImpl(const ts::Node& node, std::string_view source) {
    std::cout << "Extend Rule Created\n";
    auto rule = std::make_unique<ExtendRule>(); 
    rule->target = QualifiedIdentifier{node.getChildByFieldName("target").getSourceRange(source)};
    rule->value = translator->createExpression(node.getChildByFieldName("value"));
    return rule;
}


std::unique_ptr<Rule>
AssignmentFactory::createImpl(const ts::Node& node, std::string_view source) {
    std::cout << "Assignment Rule Created\n";
    auto rule = std::make_unique<AssignmentRule>(); 
    rule->target = QualifiedIdentifier{node.getChildByFieldName("target").getSourceRange(source)};
    rule->value = translator->createExpression(node.getChildByFieldName("value"));
    return rule;
}


////////////////////////////////////////////////////////////////////////////////
// Expressions
////////////////////////////////////////////////////////////////////////////////


std::unique_ptr<Expression>
DummyExpressionFactory::createImpl(const ts::Node& node, [[maybe_unused]] std::string_view source) {
    return translator->createExpression(node.getNamedChild(0)); 
} 


std::unique_ptr<Expression>
IdentifierFactory::createImpl(const ts::Node& node, std::string_view source) {
    return std::make_unique<IdentifierExpression>(node.getSourceRange(source));
}


std::unique_ptr<Expression>
BooleanFactory::createImpl(const ts::Node& node, std::string_view source) {
    bool value = node.getSourceRange(source) == "true" ? true : false;
    return std::make_unique<LiteralExpression<bool>>(value);
}

////////////////////////////////////////////////////////////////////////////////
// Translator
////////////////////////////////////////////////////////////////////////////////


Translator 
createTranslator(std::string_view source) {
    Translator translator{source};

    // Rules
    translator.registerRuleFactory("rule",              std::make_unique<DummyRuleFactory>(&translator));
    translator.registerRuleFactory("body",              std::make_unique<BodyFactory>(&translator));
    translator.registerRuleFactory("for",               std::make_unique<ForFactory>(&translator));
    translator.registerRuleFactory("parallel_for",      std::make_unique<ParallelForFactory>(&translator));
    translator.registerRuleFactory("match",             std::make_unique<MatchFactory>(&translator));
    translator.registerRuleFactory("discard",           std::make_unique<DiscardFactory>(&translator));
    translator.registerRuleFactory("message",           std::make_unique<MessageFactory>(&translator));
    translator.registerRuleFactory("input_choice",      std::make_unique<InputChoiceFactory>(&translator));
    translator.registerRuleFactory("scores",            std::make_unique<ScoresFactory>(&translator));
    translator.registerRuleFactory("extend",            std::make_unique<ExtendFactory>(&translator));
    translator.registerRuleFactory("assignment",        std::make_unique<AssignmentFactory>(&translator));

    // Expressions
    translator.registerExpressionFactory("expression",  std::make_unique<DummyExpressionFactory>(&translator));
    translator.registerExpressionFactory("identifier",  std::make_unique<IdentifierFactory>(&translator));
    translator.registerExpressionFactory("boolean",     std::make_unique<BooleanFactory>(&translator));

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