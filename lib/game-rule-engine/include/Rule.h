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
    BodyRule(ts::Node node, const std::string& source);
    std::optional<ts::Node> execute() override;
private:
    uint32_t index = 0; 
};


class BaseRule : public IRule {
public:
    BaseRule(ts::Node node, const std::string& source);
    std::optional<ts::Node> execute() override;
private:
    bool executed = false;
};


class MessageRule : public IRule {
public:
    MessageRule(ts::Node node, const std::string& source);
    std::optional<ts::Node> execute() override;
};

#endif