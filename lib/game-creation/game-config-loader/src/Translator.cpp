#include <stdexcept>
#include "Translator.h"
#include "GameState.h"
#include <variant>
#include <iostream>
#include <set>
#include <unordered_map>


////////////////////////////////////////////////////////////////////////////////
// Rules
////////////////////////////////////////////////////////////////////////////////


std::unique_ptr<Rule>
DummyRuleFactory::createImpl(const ts::Node& node) {
    return translator->createRule(node.getNamedChild(0)); 
} 


std::unique_ptr<Rule> 
BodyFactory::createImpl(const ts::Node& node) {
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
ForFactory::createImpl(const ts::Node& node) {
    std::cout << "For Rule Created\n";
    auto rule = std::make_unique<ForRule>();
    rule->currentItem = IdentifierExpression(node.getNamedChild(0).getSourceRange(translator->source));
    rule->list = translator->createExpression(node.getNamedChild(1));
    rule->body = BodyFactory::createBodyRule(node.getNamedChild(2), translator);
    return rule;
}


std::unique_ptr<Rule>
ParallelForFactory::createImpl(const ts::Node& node) {
    std::cout << "Parallel For Rule Created\n";
    auto rule = std::make_unique<ParallelForRule>();
    rule->currentItem = IdentifierExpression(node.getNamedChild(0).getSourceRange(translator->source));
    rule->list = translator->createExpression(node.getNamedChild(1));
    rule->body = BodyFactory::createBodyRule(node.getNamedChild(2), translator);
    return rule;
}


std::unique_ptr<Rule>
MatchFactory::createImpl(const ts::Node& node) {
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
DiscardFactory::createImpl(const ts::Node& node) {
    std::cout << "Discard Rule Created\n";
    auto rule = std::make_unique<DiscardRule>();

    rule->count = translator->createExpression(node.getChildByFieldName("count"));
    rule->source = QualifiedIdentifier{node.getChildByFieldName("source").getSourceRange(translator->source)};

    return rule;
}


std::unique_ptr<Rule>
MessageFactory::createImpl(const ts::Node& node) {
    std::cout << "Message Rule Created\n";
    auto rule = std::make_unique<MessageRule>();

    rule->content = translator->createExpression(node.getChildByFieldName("content"));
    ts::Node players = node.getChildByFieldName("players");

    if (players.getSourceRange(translator->source) == "all") {
        rule->players = std::make_unique<LiteralExpression>(
                std::make_unique<GameEnvironment::Value>("all"));
    } else {

    }
    
    return rule;
}


std::unique_ptr<Rule>
InputChoiceFactory::createImpl(const ts::Node& node) {
    std::cout << "Input Rule Created\n";
    auto rule = std::make_unique<InputChoiceRule>();

    rule->target = QualifiedIdentifier{
        node.getChildByFieldName("player").getSourceRange(translator->source)
    };

    rule->prompt = translator->createExpression(node.getChildByFieldName("prompt"));
    rule->choices = translator->createExpression(node.getChildByFieldName("choices"));

    ts::Node timeoutNode = node.getChildByFieldName("timeout");
    if (!timeoutNode.isNull()) {
        rule->timeout = translator->createExpression(timeoutNode);
    }

    return rule;
}


std::unique_ptr<Rule>
ScoresFactory::createImpl(const ts::Node& node) {
    auto rule = std::make_unique<ScoresRule>(); 
    rule->keys = translator->createExpression(node.getChildByFieldName("keys"));
    return rule;
}


std::unique_ptr<Rule>
ExtendFactory::createImpl(const ts::Node& node) {
    std::cout << "Extend Rule Created\n";
    auto rule = std::make_unique<ExtendRule>(); 
    rule->target = QualifiedIdentifier{node.getChildByFieldName("target").getSourceRange(translator->source)};
    rule->value = translator->createExpression(node.getChildByFieldName("value"));
    return rule;
}


std::unique_ptr<Rule>
AssignmentFactory::createImpl(const ts::Node& node) {
    std::cout << "Assignment Rule Created\n";
    auto rule = std::make_unique<AssignmentRule>(); 
    rule->target = QualifiedIdentifier{node.getChildByFieldName("target").getSourceRange(translator->source)};
    rule->value = translator->createExpression(node.getChildByFieldName("value"));
    return rule;
}

std::unique_ptr<Rule>
ReverseFactory::createImpl(const ts::Node& node) {
    std::cout << "Reverse Rule Created\n";
    auto rule = std::make_unique<ReverseRule>(); 
    rule->target = QualifiedIdentifier{node.getChildByFieldName("target").getSourceRange(translator->source)};
    return rule;
}


////////////////////////////////////////////////////////////////////////////////
// Expressions
////////////////////////////////////////////////////////////////////////////////

const std::set<std::string_view> literalExpressionTypes {
    "boolean",
    "number",
    "quoted_string",
    "list_literal",
    "identifier",
    "value_map"
};

const std::unordered_map<std::string_view, Operator> symbolsToOperators {
    {"||",  Operator::OR},
    {"&&",  Operator::AND},
    {"=",   Operator::EQUALS},
    {"!=",  Operator::NOT},
    {"<",   Operator::LT},
    {"<=",  Operator::LTE},
    {">",   Operator::GT},
    {">=",  Operator::GTE},
    {"+",   Operator::PLUS},
    {"-",   Operator::SUBTRACT},
    {"*",   Operator::MULTIPLY},
    {"/",   Operator::DIVISION},
    {"%",   Operator::MOD}
};

const std::unordered_map<std::string_view, Builtin> spellingsToBuiltins {
    {"upfrom",      Builtin::UPFROM},
    {"size",        Builtin::SIZE},
    {"contains",    Builtin::CONTAINS},
    {"collect",     Builtin::COLLECT},
};

bool isLiteralExpression(const ts::Node& node) {
    auto type = node.getNamedChild(0).getType();
    return node.getNumNamedChildren() == 1 &&
            literalExpressionTypes.find(type) != literalExpressionTypes.end();
}

bool isBinaryExpression(const ts::Node& node) {
    return !node.getChildByFieldName("lhs").isNull();
}

// PRE: node is not a literal (quoted_string) expression containing a . 
bool isDotExpression(const ts::Node& node, std::string_view source) {
    auto view = node.getSourceRange(source);
    return view.find(std::string_view{"."}) != std::string_view::npos;
}

bool isNegatedExpression(const ts::Node& node, std::string_view source) {
    return !node.getChildByFieldName("operand").isNull() &&
            node.getSourceRange(source)[0] == '!';
}


// Helper to construct a builtin expression from a builtin dot expression node
// PRE: node is an expression node that contains a builtin child
//      e.g. "players.elements.weapon.contains(weapon.name)"
std::unique_ptr<BuiltinExpression> 
createBuiltinExpression(ts::Node node, const Translator* translator) {
    std::string_view spelling = node.getChildByFieldName("builtin").getSourceRange(translator->source);
    auto builtinExpression = std::make_unique<BuiltinExpression>();
    builtinExpression->builtin = spellingsToBuiltins.at(spelling);
    ts::Node expressionList = node.getChild(3).getNamedChild(0);

    if (!expressionList.isNull()) {
        ts::Cursor cursor = expressionList.getCursor();
        if (cursor.gotoFirstChild()) {
            do {
                ts::Node node = cursor.getCurrentNode();
                if (node.getType() == "expression") {
                    builtinExpression->arguments.push_back(translator->createExpression(node));    
                }
            } while (cursor.gotoNextSibling());
        }
    }

    return builtinExpression;
}


std::unique_ptr<Expression>
DummyExpressionFactory::createImpl(const ts::Node& node) {
    if (isLiteralExpression(node)) {
        return translator->createExpression(node.getNamedChild(0));
    }

    if (isNegatedExpression(node, translator->source)) {
        return std::make_unique<UnaryExpression>(
            translator->createExpression(node.getChild(1)), 
            Operator::NOT
        );
    }

    if (isBinaryExpression(node)) {
        return std::make_unique<BinaryExpression>(
            translator->createExpression(node.getChild(0)),
            translator->createExpression(node.getChild(2)),
            symbolsToOperators.at(node.getChild(1).getSourceRange(translator->source))
        );
    }

    if (isDotExpression(node, translator->source)) {
        // Identifier dot expression
        if (!node.getChildByFieldName("identifier").isNull()) {
            return std::make_unique<BinaryExpression>(
                translator->createExpression(node.getChild(0)),
                translator->createExpression(node.getChild(2)),
                Operator::DOT
            );
        }

        // Builtin dot expression
        if (!node.getChildByFieldName("builtin").isNull()) {
            auto builtinExpression = createBuiltinExpression(node, translator); 
            return std::make_unique<BinaryExpression>(
                translator->createExpression(node.getChild(0)),
                std::move(builtinExpression),
                Operator::DOT
            );
        }
    }
    
    throw std::runtime_error("Count not deduce type of expression");
} 


// Literal Expression Factories

std::unique_ptr<Expression>
IdentifierFactory::createImpl(const ts::Node& node) {
    return std::make_unique<IdentifierExpression>(node.getSourceRange(translator->source));
}


std::unique_ptr<Expression>
BooleanFactory::createImpl(const ts::Node& node) {
    bool value = node.getSourceRange(translator->source) == "true" ? true : false;
    return std::make_unique<LiteralExpression>(
        std::make_unique<GameEnvironment::Value>(value));
}


std::unique_ptr<Expression>
QuotedStringFactory::createImpl(const ts::Node& node) {
    auto value = std::string{node.getSourceRange(translator->source)};
    return std::make_unique<LiteralExpression>(
        std::make_unique<GameEnvironment::Value>(value));
}


std::unique_ptr<Expression>
ListLiteralFactory::createImpl(const ts::Node& node) {
    std::unique_ptr<GameEnvironment::Value> value = translator->gsl->convertNode(node);
    return std::make_unique<LiteralExpression>(std::move(value));
}


std::unique_ptr<Expression>
NumberFactory::createImpl(const ts::Node& node) {
    auto value = std::stoi(std::string{node.getSourceRange(translator->source)});
    return std::make_unique<LiteralExpression>(
        std::make_unique<GameEnvironment::Value>(value));
}

// TODO: value_map literal expressions


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
    translator.registerRuleFactory("reverse",           std::make_unique<ReverseFactory>(&translator));

    // Expressions
    translator.registerExpressionFactory("expression",        std::make_unique<DummyExpressionFactory>(&translator));
    translator.registerExpressionFactory("identifier",        std::make_unique<IdentifierFactory>(&translator));
    translator.registerExpressionFactory("boolean",           std::make_unique<BooleanFactory>(&translator));
    translator.registerExpressionFactory("quoted_string",     std::make_unique<QuotedStringFactory>(&translator));
    translator.registerExpressionFactory("list_literal",      std::make_unique<ListLiteralFactory>(&translator));
    translator.registerExpressionFactory("number",            std::make_unique<NumberFactory>(&translator));

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

    return factory->second->create(node);
}


std::unique_ptr<Expression>
Translator::createExpression(const ts::Node& node) const {
    std::string_view type = node.getType();
    auto factory = expressionFactories.find(std::string{type});

    if (factory == expressionFactories.end()) {
        throw std::runtime_error("Unable to create expression of type: "+ std::string{type});
    }

    return factory->second->create(node);
}