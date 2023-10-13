#ifndef RULES__H
#define RULES__H

#include <cpp-tree-sitter.h>
#include <string>
#include <memory>

class GameRules {
public:
    GameRules(std::string_view source);

    // [[nodiscard]] const ts::Node& getRules() const { return m_rules; }
    [[nodiscard]] std::string_view getSource() { return m_source; }

private:
    std::string m_source{};
    // ts::Tree m_exprTree{nullptr}; // Dummy placeholder replaced on constructor call
    // ts::Node m_rules{TSNode{}}; // Dummy placeholder
};

#endif