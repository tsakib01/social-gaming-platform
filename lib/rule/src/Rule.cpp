#include "Rule.h"
#include <iostream>
#include <string_view>

/////////////////////////////////////////////////////////////////////////
// Rules
/////////////////////////////////////////////////////////////////////////

void 
print(const ts::Node& node, std::string_view source) {
    if (node.isNull()) {
        return;
    }
    std::cout << node.getType() << std::endl;
    for (uint32_t i{0}; i < node.getNumNamedChildren(); ++i) {
        print(node.getNamedChild(i), source);
        if (node.getType() == "expression") { 
            std::cout << node.getSourceRange(source) << std::endl;
        }
    }
}

std::optional<ts::Node> 
MatchRule::execute() {
    std::cout << "Executing match\n";
    ts::Node target = node.getChildByFieldName("target");
    std::cout << target.getSourceRange(source) << std::endl;
    std::vector<ts::Node> matchEntries{};

    ts::Node matchEntry = target.getNextSibling();
    while (!matchEntry.isNull()) {
        if (matchEntry.isNamed())
            matchEntries.push_back(matchEntry);
        matchEntry = matchEntry.getNextSibling();
    }

    for (const auto& m : matchEntries) {
        std::cout << "MATCHENTRY\n";
        std::cout << m.getSourceRange(source) << '\n';
    }

    return std::nullopt;

    // print(m_node, m_source);
    // std::cout << m_node.getSExpr() << std::endl;
}