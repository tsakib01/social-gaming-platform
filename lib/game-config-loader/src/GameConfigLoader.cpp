#include "GameConfigLoader.h"
#include "RuleInterpreter.h"
#include "ConstantManager.h"
#include <iostream>
#include <fstream>
#include <sstream>


std::shared_ptr<GameRules> 
GameConfigLoader::createGameRules(std::string_view path) { 
    setSource(path);
    return std::make_unique<GameRules>(m_source);
}


// TODO: Move handling of loading constants into createGameState

// void GameConfigLoader::loadConstants(const ts::Node& root){
//     ts::Node constants = root.getChildByFieldName("constants");
//     ConstantManager constantManager(constants, source);
//     constantManager.print();
// }

/// @brief temp overload to print contents of vector (since printing ValidTypes that is a vector doesn't work)
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    if(vec.empty()) {
        os << "[]";
        return os;
    }

    os << "[";
    std::for_each(vec.begin(), vec.end() - 1,
        [&os](const T& value) { os << value << ","; });
    os << vec.back() << "]";
    return os;
}

/// @brief temp overload to print a ValidTypes variant.
std::ostream& operator<<(std::ostream& os, const ValidTypes& value) {
    std::visit([&os](auto&& arg) { os << arg; }, value);
    return os;
}

// TODO: Change implementation of handling game state to use std::variant
std::shared_ptr<GameState>
GameConfigLoader::createGameState() {
    auto gameState = std::make_shared<GameState>();

    // If parser sees a number expression node, can add to constants map like this:
    auto createNumConstant = Expression::createConstExpr(10);
    auto createStringConstant = Expression::createConstExpr<std::string_view>("helloworld");
    auto createNumVecConstant = Expression::createConstExpr<std::vector<int>>({1, 2, 3});
    gameState->addConstant("testNum", std::move(createNumConstant));
    gameState->addConstant("testString", std::move(createStringConstant));
    gameState->addConstant("testNumVec", std::move(createNumVecConstant));
    
    std::cout << "---- Printing test expressions: \n";
    
    auto numConstant = gameState->getConstant("testNum");
    if (numConstant) std::cout << numConstant->getValue() << '\n';

    auto stringConstant = gameState->getConstant("testString");
    if (stringConstant) std::cout << stringConstant->getValue() << '\n';

    auto numVecConstant = gameState->getConstant("testNumVec");
    if (numVecConstant) std::cout << numVecConstant->getValue() << '\n';
    
    std::cout << "---- End printing test expressions\n";

    return gameState;
}


void 
GameConfigLoader::setSource(std::string_view path) {
    if (path == m_path) {
        return;
    }

    m_path = path;

    std::ifstream ifs(path.data());
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();
    m_source = buffer.str();
}