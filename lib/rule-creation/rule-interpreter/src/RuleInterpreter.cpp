#include "RuleInterpreter.h"
#include <iostream>
#include <string>
#include <algorithm>


std::shared_ptr<RuleNode> 
RuleInterpreter::createRuleNode(std::optional<ts::Node> node, std::string_view source) {
    if (node.has_value()) {
        ts::Node actualNode = node.value();
        std::string_view type = actualNode.getType();
        std::shared_ptr<RuleNode> ruleNode = std::make_shared<RuleNode>();
        if (type == "body") {
            std::unique_ptr<Rule> rule = std::make_unique<BodyRule>(ruleNode, source);
            ruleNode->setRule(std::move(rule), "bodyRule");
            return ruleNode;
        }
        if (type == "rule") {
            std::unique_ptr<Rule> rule = std::make_unique<BaseRule>(ruleNode, source);
            ruleNode->setRule(std::move(rule), "baseRule");
            return ruleNode;
        }
        if (type == "message") {
            std::unique_ptr<Rule> rule = std::make_unique<MessageRule>(ruleNode, source);
            ruleNode->setRule(std::move(rule), "message");
            return ruleNode;
        }
        if (type == "for") {
            std::unique_ptr<Rule> rule = std::make_unique<ForRule>(ruleNode, source);
            ruleNode->setRule(std::move(rule), "for");
            return ruleNode;
        }
        if (type == "match") {
            std::unique_ptr<Rule> rule = std::make_unique<MatchRule>(ruleNode, source);
            ruleNode->setRule(std::move(rule), "match");
            return ruleNode;
        }
        if (type == "match_entry") {
            std::unique_ptr<Rule> rule = std::make_unique<MatchEntryRule>(ruleNode, source);
            ruleNode->setRule(std::move(rule), "match_entry");
            return ruleNode;
        }
        if (type == "discard") {
            std::unique_ptr<Rule> rule = std::make_unique<DiscardRule>(ruleNode, source);
            ruleNode->setRule(std::move(rule), "discard");
            return ruleNode;
        }
        if (type == "assignment") {
            std::unique_ptr<Rule> rule = std::make_unique<AssignmentRule>(ruleNode, source);
            ruleNode->setRule(std::move(rule), "assignment");
            return ruleNode;
        }
        if (type == "extend") {
            std::unique_ptr<Rule> rule = std::make_unique<ExtendRule>(ruleNode, source);
            ruleNode->setRule(std::move(rule), "extend");
            return ruleNode;
        }
        if (type == "reverse") {
            std::unique_ptr<Rule> rule = std::make_unique<ReverseRule>(ruleNode, source);
            ruleNode->setRule(std::move(rule), "reverse");
            return ruleNode;
        }
        if (type == "parallel_for") {
            std::unique_ptr<Rule> rule = std::make_unique<ParallelForRule>(ruleNode, source);
            ruleNode->setRule(std::move(rule), "parallel_for");
            return ruleNode;
        }
        if (type == "input_choice") {
            std::unique_ptr<Rule> rule = std::make_unique<InputRule>(ruleNode, source);
            ruleNode->setRule(std::move(rule), "input");
            return ruleNode;
        }
        if (type == "scores") {
            std::unique_ptr<Rule> rule = std::make_unique<ScoresRule>(ruleNode, source);
            ruleNode->setRule(std::move(rule), "scores");
            return ruleNode;
        }
    }
    return nullptr;
}

std::shared_ptr<RuleNode> 
RuleInterpreter::convertNodeTreeToRuleTree(const ts::Node& root, std::string_view source) {
    if (root.isNull()) {
        return nullptr;
    }
    std::shared_ptr<RuleNode> ruleNode = createRuleNode(root, source);
    if (!ruleNode) {
        return nullptr;
    }
    for (uint32_t idx = 0; idx < root.getNumNamedChildren(); ++idx) {
        ts::Node child = root.getNamedChild(idx);
        std::shared_ptr<RuleNode> childRuleNode = convertNodeTreeToRuleTree(child, source);
        if (childRuleNode) {
            ruleNode->addChildNode(childRuleNode);
        }
    }
    return ruleNode;
}