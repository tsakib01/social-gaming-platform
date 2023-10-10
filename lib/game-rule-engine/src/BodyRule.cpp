#include "Rule.h"

BodyRule::BodyRule(ts::Node node, const std::string& source) : IRule(node, source) {
    std::cout << "Body Rule created" << std::endl;
}

std::optional<ts::Node> BodyRule::execute() {
    std::cout << "Executing BodyRule..." << std::endl;

    if (index == 0 || index < node.getNumNamedChildren()) {
        ts::Node nextNode = node.getNamedChild(index);
        index++;
        return nextNode;
    } 

    return std::nullopt;
}