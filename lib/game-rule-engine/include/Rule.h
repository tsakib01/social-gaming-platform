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

class ForLoopRule : public IRule {
public:
    ForLoopRule() {
        std::cout << "For Loop Rule created" << std::endl;
    }

    void execute() override {};
};
//TO-DO: create other types of rule expressions
#endif