#include "GameInstance.h"
#include "GameConfigLoader.h"

#include <fstream>
#include <sstream>
#include <cpp-tree-sitter.h>

// This probably also shouldn't be here
extern "C" {
    TSLanguage* tree_sitter_socialgaming();
}


GameInstance::GameInstance(std::unique_ptr<GameRules> gameRules, 
std::unique_ptr<GameState> gameState)
    : gameRules(std::move(gameRules)), gameState(std::move(gameState))
{
    // TODO: FIX these 2 lines - this should replace the implementation below, but I
    // ran into some issues with resource management where ts::Tree is involved
    // - you can't make copies of ts::Tree, and you can't make smart pointers to
    // ts::Tree, so it is hard to pass around. See GameRules.cpp for more info.

    // const ts::Node rules = gameRules->getRules();
    // ts::Node body = rules.getNamedChild(0);

    // This implementation should probably go to GameConfigLoader
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};

    std::cout << gameRules.get() << '\n';

    // ts::Tree tree = parser.parseString(gameRules->getSource());
    // std::cout << gameRules->getSource();

    // ts::Node root = tree.getRootNode();
    // ts::Node rules = root.getChildByFieldName("rules");
    // ts::Node body = rules.getNamedChild(0);


    // std::shared_ptr<Rule> firstInstruction(RuleInterpreter::createRule(body, source));
    // instructionStack.push(firstInstruction);

    // startGame();
}

void GameInstance::startGame() {
    // This function shouldn't exist - it's just a test to see if it will work
    // Should probably be in GameInstanceManager

    std::cout << "\nGame started." << std::endl;
    while (!instructionStack.empty()) {
        executeNextInstruction();
    }
    std::cout << "Game ended." << std::endl;
}

void GameInstance::executeNextInstruction() {
    std::shared_ptr<Rule> instruction = instructionStack.top();
    std::optional<ts::Node> nextInstructionNode = instruction->execute();

    if (nextInstructionNode.has_value()) {
        std::shared_ptr<Rule> nextInstruction(RuleInterpreter::createRule(nextInstructionNode, source));
        instructionStack.push(nextInstruction);
    } else {
        instructionStack.pop();
    }
}