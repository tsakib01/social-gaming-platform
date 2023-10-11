#ifndef RULE__H
#define RULE__H

#include <vector>
#include <cpp-tree-sitter.h>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <string_view>
#include <optional>

class Rule {
public:
    Rule(ts::Node node, const std::string_view source) 
        : node(node), source(source)  {}

    virtual std::optional<ts::Node> execute() = 0;
    virtual ~Rule() {}

protected:
    const ts::Node node;
    const std::string_view source;
};


class BodyRule : public Rule {
public:
    BodyRule(ts::Node node, const std::string_view source);
    std::optional<ts::Node> execute() override;
private:
    uint32_t index = 0; 
};


class BaseRule : public Rule {
public:
    BaseRule(ts::Node node, const std::string_view source);
    std::optional<ts::Node> execute() override;
private:
    bool executed = false;
};


class MessageRule : public Rule {
public:
    MessageRule(ts::Node node, const std::string_view source);
    std::optional<ts::Node> execute() override;
};


class MatchRule : public Rule {
public:
    MatchRule(const ts::Node node, std::string_view source) 
        : Rule(node, source)
    {
        std::cout << "Match Rule created" << std::endl;
    }

    std::optional<ts::Node> execute() override;
};

#endif