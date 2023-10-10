#ifndef RULE__H
#define RULE__H

#include <vector>
#include <cpp-tree-sitter.h>
#include <iostream>

#include <optional>

class IRule {
public:
    IRule(ts::Node node, const std::string& source) : node(node), source(source)  {}   
    virtual std::optional<ts::Node> execute() = 0;
    virtual ~IRule() {}
protected:
    const ts::Node node;
    const std::string& source;
};

class BodyRule : public IRule {
public:
    BodyRule(ts::Node node, const std::string& source) : IRule(node, source) {
        std::cout << "Body Rule created" << std::endl;
        std::cout  << "node type : " << node.getType() << std::endl;
    }

    std::optional<ts::Node> execute() override {
        std::cout << "Executing Body..." << std::endl;

        std::cout << "node type: " << node.getType() << std::endl;

        if (index == 0 || index < node.getNumNamedChildren() - 1) {
            // TO-DO: don't use index based loop, change this later

            std::cout << "type: " << std::endl;
            std::cout << node.getType() << std::endl;

            while (index < node.getNumNamedChildren()) { 
                index++;
                ts::Node nextNode = node.getNamedChild(index);
                return nextNode;
            }
        } 

        return std::nullopt;
    };
private:
    uint32_t index = 0; 
};

class BaseRule : public IRule {
public:
    BaseRule(ts::Node node, const std::string& source) : IRule(node, source) {
        std::cout << "Base Rule created" << std::endl;
    }

    std::optional<ts::Node> execute() override {
        std::cout << "Executing Rule..." << std::endl;

        // get the first child (always)
        // ts::Node ruleToCreate = 
        // return

        if (!executed) {
            executed = true;
            ts::Node nextNode = node.getNamedChild(0);
            return nextNode;
        }

        return std::nullopt;
        // return nullptr;
    };
private:
    bool executed = false;
};

class MessageRule : public IRule {
public:
    MessageRule(ts::Node node, const std::string& source) : IRule(node, source) {
        std::cout << "MessageRule created" << std::endl;
    }

    std::optional<ts::Node> execute() override {
        // cout the quoted string
        std::cout << "Executing MessageRule..." << std::endl;

        // TODO - find player set 
        // find expression for quoted string
        ts::Node expression = node.getNamedChild(1);
        std::cout << expression.getSourceRange(source) << std::endl;

        // return nullptr;
        return std::nullopt;
    }
};

//TO-DO: create other types of rule expressions
//TO-DO: instead of having all classes defined here, separate into Rule .cpp files
#endif