#include "GameInstance.h"
#include "GameConfigLoader.h"

#include <fstream>
#include <sstream>
#include <cpp-tree-sitter.h>

// This probably also shouldn't be here
extern "C" {
    TSLanguage* tree_sitter_socialgaming();
}


GameInstance::GameInstance(std::shared_ptr<GameRules> gameRules, 
std::shared_ptr<GameState> gameState)
    : m_gameRules(gameRules), m_gameState(std::move(gameState))
{
    const ts::Node rules = gameRules->getRules();
    ts::Node body = rules.getNamedChild(0);

    // This implementation should probably go to GameConfigLoader
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};

    std::shared_ptr<Rule> firstInstruction(RuleInterpreter::createRule(body, gameRules->getSource()));
    instructionStack.push(firstInstruction);

    startGame();
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
        std::shared_ptr<Rule> nextInstruction(
                RuleInterpreter::createRule(nextInstructionNode, m_gameRules->getSource()));

        instructionStack.push(nextInstruction);
    } else {
        instructionStack.pop();
    }
}