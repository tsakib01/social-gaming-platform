#ifndef RULE__H
#define RULE__H

#include "RuleNode.h"
#include <vector>
#include <cpp-tree-sitter.h>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <string_view>
#include <optional>

class RuleNode;

class Rule {
public:
    Rule(std::shared_ptr<RuleNode> node, std::string_view source) 
        : node(node), source(source)  {}

    virtual std::optional<std::shared_ptr<RuleNode>> execute() = 0;
    virtual void reset() = 0;
    virtual ~Rule() {}

protected:
    std::shared_ptr<RuleNode> node;
    std::string_view source;
};


class BodyRule : public Rule {
public:
    BodyRule(std::shared_ptr<RuleNode> node, std::string_view source);
    std::optional<std::shared_ptr<RuleNode>> execute() override;
    void reset() override;

private:
    size_t index = 0; 
};


class BaseRule : public Rule {
public:
    BaseRule(std::shared_ptr<RuleNode> node, std::string_view source);
    std::optional<std::shared_ptr<RuleNode>> execute() override;
    void reset() override;

private:
    bool executed = false;
};


class MessageRule : public Rule {
public:
    MessageRule(std::shared_ptr<RuleNode> node, std::string_view source);
    std::optional<std::shared_ptr<RuleNode>> execute() override;
    void reset() override {}
};

class ForRule : public Rule {
public:
    ForRule(std::shared_ptr<RuleNode> node, std::string_view source) 
        : Rule(node, source)
    {
        std::cout << "For Rule created" << std::endl;
    }

    std::optional<std::shared_ptr<RuleNode>> execute() override;
    void reset() override { iteration = 1; }

private:
    size_t index = 0;
    size_t iteration = 1;
    size_t maxLength = 4;
};

class MatchRule : public Rule {
public:
    MatchRule(const std::shared_ptr<RuleNode> node, std::string_view source) 
        : Rule(node, source)
    {
        std::cout << "Match Rule created" << std::endl;
    }

    std::optional<std::shared_ptr<RuleNode>> execute() override;
    void reset() override {}
};

class MatchEntryRule : public Rule {
public:
    MatchEntryRule(const std::shared_ptr<RuleNode> node, std::string_view source) 
        : Rule(node, source)
    {
        std::cout << "Match Entry created" << std::endl;
    }

    std::optional<std::shared_ptr<RuleNode>> execute() override;
    void reset() override {}
};

class DiscardRule : public Rule {
public:
    DiscardRule(const std::shared_ptr<RuleNode> node, std::string_view source) 
        : Rule(node, source)
    {
        std::cout << "Dicard Rule created" << std::endl;
    }

    std::optional<std::shared_ptr<RuleNode>> execute() override;
    void reset() override {}
};

class AssignmentRule : public Rule {
public:
    AssignmentRule(const std::shared_ptr<RuleNode> node, std::string_view source) 
        : Rule(node, source)
    {
        std::cout << "Assignment Rule created" << std::endl;
    }

    std::optional<std::shared_ptr<RuleNode>> execute() override;
    void reset() override {}
};

class ExtendRule : public Rule {
public:
    ExtendRule(const std::shared_ptr<RuleNode> node, std::string_view source) 
        : Rule(node, source)
    {
        std::cout << "Extend Rule created" << std::endl;
    }

    std::optional<std::shared_ptr<RuleNode>> execute() override;
    void reset() override {}
};

class ParallelForRule : public Rule {
public:
    ParallelForRule(const std::shared_ptr<RuleNode> node, std::string_view source) 
        : Rule(node, source)
    {
        std::cout << "Parralel For Rule created" << std::endl;
    }

    std::optional<std::shared_ptr<RuleNode>> execute() override;
    void reset() override {}
};

class InputRule : public Rule {
public:
    InputRule(const std::shared_ptr<RuleNode> node, std::string_view source) 
        : Rule(node, source)
    {
        std::cout << "Input Rule created" << std::endl;
    }

    std::optional<std::shared_ptr<RuleNode>> execute() override;
    void reset() override {}
};

class ScoresRule : public Rule {
public:
    ScoresRule(const std::shared_ptr<RuleNode> node, std::string_view source) 
        : Rule(node, source)
    {
        std::cout << "Score Rule created" << std::endl;
    }

    std::optional<std::shared_ptr<RuleNode>> execute() override;
    void reset() override {}
};

#endif