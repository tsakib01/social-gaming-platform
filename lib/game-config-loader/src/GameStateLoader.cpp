#include "GameStateLoader.h"
#include <iostream>
#include <cpp-tree-sitter.h>
#include <string>
#include<algorithm>
#include <queue>

namespace SYMBOL{
    const int NUMBER = 81;
    const int STRING = 84;
    const int BOOL = 123;
    const int LIST = 126;
    const int MAP = 128;
}

GameStateLoader::GameStateLoader(std::string_view source)
:source(source)
{}

bool GameStateLoader::isPrimitive(const ts::Node& node){
    int symbol = node.getSymbol();
    return symbol == SYMBOL::NUMBER || symbol == SYMBOL::STRING || symbol == SYMBOL::BOOL;
}

bool GameStateLoader::isList(const ts::Node& node){
    int symbol = node.getSymbol();
    return symbol == SYMBOL::LIST;
}

bool GameStateLoader::isMap(const ts::Node& node){
    int symbol = node.getSymbol();
    return symbol == SYMBOL::MAP;
}

Primitive GameStateLoader::convertNodeToPrimitive(const ts::Node& node){
    Primitive toReturn;
    if (node.getSymbol() == SYMBOL::NUMBER){
        toReturn = std::stoi(std::string(node.getSourceRange(source)));
    }
    else if (node.getSymbol() == SYMBOL::BOOL){
        std::string upperCaseString(node.getSourceRange(source));
        std::transform(upperCaseString.begin(), upperCaseString.end(), upperCaseString.begin(), ::toupper);
        toReturn = (upperCaseString == "TRUE");
    }
    else if (node.getSymbol() == SYMBOL::STRING){
        toReturn = node.getSourceRange(source);
    }
    return toReturn;
}

List GameStateLoader::convertNodeToList(const ts::Node& node){
    ts::Node listNode = node.getNamedChild(0);
    std::vector<std::variant<Primitive, Map>> toReturn;
    for (uint32_t i = 0; i < listNode.getNumNamedChildren(); i++){
        ts::Node elementNode = listNode.getNamedChild(i).getNamedChild(0);
        std::variant<Primitive, Map> element; 
        if (isPrimitive(elementNode)){  
            element = convertNodeToPrimitive(elementNode);
        }else if (isMap(elementNode)){
            element = convertNodeToMap(elementNode);
        }else{
            std::runtime_error("Not supported type");
        }
        toReturn.push_back(element);
    }
    return toReturn;
}

Map GameStateLoader::convertNodeToMap(const ts::Node& node){
    std::map<std::string_view, Primitive> toReturn;
    for (uint32_t i = 0; i < node.getNumNamedChildren(); i++){
        ts::Node mapEntry = node.getNamedChild(i);
        ts::Node identifierNode = mapEntry.getNamedChild(0);
        ts::Node valueNode = mapEntry.getNamedChild(1);

        // Convert an identifier of a node to string 
        std::string_view identifier = identifierNode.getSourceRange(source);
        ts::Node value = valueNode.getNamedChild(0);
        Primitive toStore;
        if (isPrimitive(value)){
            toStore = convertNodeToPrimitive(value);
        }
        else{
            std::runtime_error("Not supported type");
        }
        toReturn.insert(std::make_pair(identifier, toStore));
    }
    return toReturn;
}

std::unique_ptr<Environment> GameStateLoader::getEnvironment(const ts::Node& root){
    std::unique_ptr<Environment> environment = std::make_unique<Environment>();

    // Loop each map entry
    for (uint32_t i = 0; i < root.getNumNamedChildren(); i++){
        // Map enty has identifier should have identifier and expression which is a value of constant
        ts::Node mapEntry = root.getNamedChild(i);
        ts::Node identifierNode = mapEntry.getNamedChild(0);
        ts::Node valueNode = mapEntry.getNamedChild(1);

        // Convert an identifier of a node to string 
        std::string_view identifier = identifierNode.getSourceRange(source);

        ts::Node value = valueNode.getNamedChild(0);
        Value toStore;
        if (isPrimitive(value)){
            toStore = convertNodeToPrimitive(value);
        }
        else if (value.getSymbol() == SYMBOL::LIST){
            toStore = convertNodeToList(value);
        }
        else if (value.getSymbol() == SYMBOL::MAP){
            toStore = convertNodeToMap(value);
        }
        else{
            std::runtime_error("Not supported type");
        }
        environment->insert(std::make_pair(identifier, toStore));
    }
    return environment;
}

// Print node by level order
void GameStateLoader::printByLevelOrder(const ts::Node& node){
    if (node.isNull()){
        return;
    }
    std::queue<ts::Node> nodeQueue;
    nodeQueue.push(node);

    while (!nodeQueue.empty()){
        int queueSize = nodeQueue.size();

        for (int i = 0; i < queueSize; i++){
            ts::Node node = nodeQueue.front();
            nodeQueue.pop();
            std::cout << node.getType() << ", ";
            for (uint32_t j = 0; j < node.getNumNamedChildren(); j++){
                nodeQueue.push(node.getNamedChild(j));
            }
        }
        std::cout << "\n";
    }
}