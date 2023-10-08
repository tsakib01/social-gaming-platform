#include "Rule.h"
#include <iostream>

IRule* RuleMap::getRule(std::string_view ruleName, const ts::Node& node) {
    if (m_map.find(ruleName) != m_map.end()) {
        return m_map[ruleName](node);
    }
    return nullptr;
};

template <typename RuleType>
IRule* RuleMap::createRule(const ts::Node& node) {
    return new RuleType(node);
}

// New rules can easily be added here
RuleMap::MapType RuleMap::generate() {
    MapType map;
    map["for"] = createRule<ForLoopRule>;
    map["match"] = createRule<MatchRule>;
    return map;
}

/////////////////////////////////////////////////////////////////////////
// Rules
/////////////////////////////////////////////////////////////////////////

void ForLoopRule::execute() {
    std::cout << "Executing for loop\n";
    std::cout << m_node.getSExpr() << std::endl;
}

void MatchRule::execute() {
    std::cout << "Executing match\n";
    std::cout << m_node.getSExpr() << std::endl;
}