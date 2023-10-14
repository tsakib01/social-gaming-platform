#include "RuleInterpreter.h"
#include <iostream>
#include <string>
#include <algorithm>


std::shared_ptr<Rule> 
RuleInterpreter::createRule(std::optional<ts::Node> node, std::string_view source) {
    if (node.has_value()) {
        ts::Node actualNode = node.value();
        std::string_view type = actualNode.getType();

        if (type == "body") {
            return std::make_shared<BodyRule>(actualNode, source);
        }
        if (type == "rule") {
            return std::make_shared<BaseRule>(actualNode, source);
        }
        if (type == "message") {
            return std::make_shared<MessageRule>(actualNode, source);
        }
        if (type == "match") {
            return std::make_shared<MatchRule>(actualNode, source);
        }

        std::string errorMsg = std::string("Rule hasn't been created yet: ") 
                             + std::string(actualNode.getType());

        throw std::runtime_error(errorMsg);
    }

    return nullptr;
}


std::unique_ptr<RuleNode> 
RuleInterpreter::convertNodeTreeToRuleTree(const ts::Node& root, std::string_view source) {
    if (root.isNull()) {
        return nullptr;
    }

    auto ruleNode = std::make_unique<RuleNode>(createRule(root, source));
    // Recursively add children to rule tree
    for (uint32_t idx = 0; idx < root.getNumNamedChildren(); ++idx) {
        ts::Node child = root.getNamedChild(idx);
        auto childRuleNode = convertNodeTreeToRuleTree(child, source);
        ruleNode->addChildNode(std::move(childRuleNode));
    }
    return ruleNode;
}