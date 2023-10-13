#include "GameRules.h"
#include <iostream>

extern "C" {
    TSLanguage* tree_sitter_socialgaming();
}

// NOTE: We want to create the ts::Tree here because it must stay in scope for the
// duration of the game or else the ts::Nodes become invalid.
// Additionally, ts::Tree, ts::Node, ts::Parser, etc. cause memory issues if used 
// with smart pointers since they handle their own memory allocation internally.
GameRules::GameRules(std::string_view source)
    : m_source(source) 
{
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};

    m_exprTree = parser.parseString(m_source);

    if(m_exprTree.getRootNode().getNumChildren() == 0) {
        throw std::runtime_error("Error: Empty config.");
    }

    m_rules = m_exprTree.getRootNode().getChildByFieldName("rules");
}