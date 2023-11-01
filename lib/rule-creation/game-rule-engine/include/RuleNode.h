#ifndef RULE__NODE__H
#define RULE__NODE__H

#include "Rule.h"

class RuleNode {
public:
    RuleNode(IRule* rule) : rule(rule) {}

    ~RuleNode() {
        this->deleteTree();
    }

    void addChildNode(RuleNode* node) {
        this->children.push_back(node);
    }

    void deleteTree() {
        for (RuleNode* child : this->children) {
            child->deleteTree();
            delete child->rule;
            delete child;
        }
        children.clear();
    }

    IRule* getRule() { return this->rule; }

    std::vector<RuleNode*> getChildren() { return this->children; }

private:
    IRule* rule;
    std::vector<RuleNode*> children;
};

#endif