#include "GameInstance.h"

#include <fstream>
#include <sstream>

extern "C" {
    TSLanguage* tree_sitter_socialgaming();
}

GameInstance::GameInstance(std::string_view gameFilePath) {
    // create empty stack, push rules body

    std::cout << "Game Instance created. Game: " << gameFilePath << std::endl;

    // This implementation should probably go to GameConfigLoader
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};

    std::ifstream ifs(gameFilePath.data());
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    std::string source = buffer.str();
    ifs.close();

    ts::Tree tree = parser.parseString(source);

    this->source = source;

    ts::Node root = tree.getRootNode();
    ts::Node rules = root.getChildByFieldName("rules");
    ts::Node body = rules.getNamedChild(0);

    std::cout << "body children : " << body.getNumNamedChildren() << std::endl;

    // convert this node to a IRule (body rule)
    
    std::shared_ptr<IRule> firstInstruction(RuleInterpreter::createRule(body, source));
    instructionStack.push(firstInstruction);
}

void GameInstance::startGame() {

    // This game shouldn't exist - it's just a test to see if it will work
    // Should probably be in GameInstanceManager
    while (!instructionStack.empty()) {
        executeNextInstruction();
    }
}

void GameInstance::executeNextInstruction() {
    std::shared_ptr<IRule> instruction = instructionStack.top();
    std::optional<ts::Node> nextInstructionNode = instruction->execute();

    if (nextInstructionNode.has_value()) {
        std::shared_ptr<IRule> nextInstruction(RuleInterpreter::createRule(nextInstructionNode, source));
        instructionStack.push(nextInstruction);
    } else {
        instructionStack.pop();
    }
}