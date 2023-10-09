#include "Rule.h"
#include <iostream>

/////////////////////////////////////////////////////////////////////////
// Rules
/////////////////////////////////////////////////////////////////////////

void print(const ts::Node& node) {
    if (node.isNull()) {
        return;
    }
    std::cout << node.getType() << std::endl;
    for (auto i{0}; i < node.getNumNamedChildren(); ++i) {
        print(node.getNamedChild(i));
    }
}

void ForLoopRule::execute() {
    std::cout << "Executing for loop\n";
    std::cout << m_node.getSExpr() << std::endl;
}

void MatchRule::execute() {
    std::cout << "Executing match\n";
    print(m_node);
}