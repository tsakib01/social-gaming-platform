#include "RuleInterpreter.h"
#include <iostream>
#include <string.h>

IRule* RuleInterpreter::createRule(std::optional<ts::Node> node, const std::string& source) {
    // TO-DO: handle other types
    // if (node.getType() == "for") {
    //     return new ForLoopRule(node);
    // }
    // if (node.getType() == "match") {
	//     return new MatchRule(node);
    // }

    // TO-DO : replace with a switch case
    // std::cout << "Creating rule of type ";
    // std::cout << node.getType() << std:: endl;
    
    if (node.has_value()) {
        ts::Node actualNode = node.value();
        std::cout << "Node ID " << actualNode.getID() << std::endl;

        if (actualNode.getType() == "body") {
            return new BodyRule(actualNode, source);
        }
        if (actualNode.getType() == "rule") {
            return new BaseRule(actualNode, source);
        }
        if (actualNode.getType() == "message") {
            return new MessageRule(actualNode, source);
        }
    }

    return nullptr;
}