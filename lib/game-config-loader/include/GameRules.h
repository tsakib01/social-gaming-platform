#ifndef RULES__H
#define RULES__H

#include <cpp-tree-sitter.h>
#include <string>
#include <memory>
#include "RuleNode.h"

class GameRules {
public:
    GameRules(std::string_view source);

    [[nodiscard]] std::shared_ptr<RuleNode> getRules() const { return m_treeRoot; }

    [[nodiscard]] std::string_view getSource() const { return m_source; }

private:
    std::string m_source{};
    ts::Tree m_exprTree{nullptr}; // Dummy placeholder replaced on constructor call
    std::shared_ptr<RuleNode> m_treeRoot;
};

#endif