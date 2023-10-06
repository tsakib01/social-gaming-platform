#ifndef RULE__H
#define RULE__H

#include <vector>
#include <cpp-tree-sitter.h>
#include <iostream>

class IRule {
public:
    virtual void execute() = 0;
    virtual ~IRule() {}
};

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

class ForLoopRule : public IRule {
public:
    ForLoopRule() {
        std::cout << "For Loop Rule created" << std::endl;
    }

    void execute() override {};
};
//TO-DO: create other types of rule expressions
#endif