#include <stdexcept>
#include "Translator.h"

std::unique_ptr<Rule>
Translator::createRule(const ts::Node& node) {
    std::string_view type = node.getType();
    auto factory = ruleFactories.find(type);

    if (factory == ruleFactories.end()) {
        throw std::runtime_error("Unable to create rule");
    }

    return factory->create(node, source);
}


std::unique_ptr<Expression>
Translator::createExpression(const ts::Node& node) {
    std::string_view type = node.getType();
    auto factory = expressionFactories.find(type);

    if (factory == expressionFactories.end()) {
        throw std::runtime_error("Unable to create expression");
    }

    return factory->create(node, source);
}