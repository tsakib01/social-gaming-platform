#include "GameStateLoader.h"
#include <iostream>
#include <cpp-tree-sitter.h>
#include <string>
#include<algorithm>
#include <queue>

// symbol for node that represents a data type in ts::Node
namespace SYMBOL{
    const int NUMBER = 81;
    const int STRING = 84;
    const int BOOL = 123;
    const int LIST = 126;
    const int MAP = 128;
    const int NUMBER_RANGE = 124;
    const int SETUP = 92;
}

class ConvertNodeToNumber final : public ConvertInterface {
public:
    ConvertNodeToNumber(const GameStateLoader* gameStateLoader)
    : ConvertInterface(gameStateLoader)
    {}
private:
    std::unique_ptr<GameEnvironment::Value> convertNodeImpl(const ts::Node& node) const{
        auto toReturn = std::make_unique<GameEnvironment::Value>();
        toReturn->value = std::stoi(std::string(node.getSourceRange(gameStateLoader->getSource())));
        return toReturn;
    }
};

class ConvertNodeToBool final : public ConvertInterface{
public:
    ConvertNodeToBool(const GameStateLoader* gameStateLoader)
    : ConvertInterface(gameStateLoader)
    {}
private:
    std::unique_ptr<GameEnvironment::Value> convertNodeImpl(const ts::Node& node) const{
        auto toReturn = std::make_unique<GameEnvironment::Value>();
        std::string upperCaseString(node.getSourceRange(gameStateLoader->getSource()));
        std::transform(upperCaseString.begin(), upperCaseString.end(), upperCaseString.begin(), ::toupper);
        toReturn->value = (upperCaseString == "TRUE");
        return toReturn;
    }
};

class ConvertNodeToString final : public ConvertInterface{
public:
    ConvertNodeToString(const GameStateLoader* gameStateLoader)
    : ConvertInterface(gameStateLoader)
    {}
private:
    std::unique_ptr<GameEnvironment::Value> convertNodeImpl(const ts::Node& node) const{
        auto toReturn = std::make_unique<GameEnvironment::Value>();
        toReturn->value = node.getSourceRange(gameStateLoader->getSource());
        return toReturn;
    }
};

class ConvertNodeToList final : public ConvertInterface{
public:
    ConvertNodeToList(const GameStateLoader* gameStateLoader)
    : ConvertInterface(gameStateLoader)
    {}
private:
    std::unique_ptr<GameEnvironment::Value> convertNodeImpl(const ts::Node& node) const{
        ts::Node listNode = node.getNamedChild(0);
        auto toReturn = std::make_unique<GameEnvironment::Value>();
        
        if(listNode.isNull()) {
            return toReturn;
        }
        
        auto list = std::make_unique<GameEnvironment::List>();
        for (uint32_t i = 0; i < listNode.getNumNamedChildren(); i++){
            ts::Node elementNode = listNode.getNamedChild(i).getNamedChild(0);
            int nodeSymbol = elementNode.getSymbol();
            auto conversionMap = gameStateLoader->getNodeSymbolToConvert();
            auto value = conversionMap->at(nodeSymbol)->convertNode(elementNode);
            list->push_back(std::move(value));
        }
        
        toReturn->value = std::move(list);
        return toReturn;
    }
};

class ConvertNodeToMap final : public ConvertInterface{
public:
    ConvertNodeToMap(const GameStateLoader* gameStateLoader)
    : ConvertInterface(gameStateLoader)
    {}
private:
    std::unique_ptr<GameEnvironment::Value> convertNodeImpl(const ts::Node& node) const{
        auto toReturn = std::make_unique<GameEnvironment::Value>();
        auto map = std::make_unique<GameEnvironment::Map>();

        for (uint32_t i = 0; i < node.getNumNamedChildren(); i++){
            ts::Node mapEntry = node.getNamedChild(i);
            ts::Node identifierNode = mapEntry.getNamedChild(0);
            ts::Node valueNode = mapEntry.getNamedChild(1);

            // Convert an identifier of a node to string 
            GameEnvironment::Identifier identifier = identifierNode.getSourceRange(gameStateLoader->getSource());
            ts::Node value = valueNode.getNamedChild(0);
            int nodeSymbol = value.getSymbol();
            auto conversionMap = gameStateLoader->getNodeSymbolToConvert();
            auto toStore = conversionMap->at(nodeSymbol)->convertNode(value);
            map->insert(std::make_pair(identifier, std::move(toStore)));
        }
        
        toReturn->value = std::move(map);
        return toReturn;
    }
};

GameStateLoader::GameStateLoader(std::string_view source)
: source(source)
{}


std::unique_ptr<GameEnvironment::Environment> GameStateLoader::getEnvironment(const ts::Node& root){
    std::unique_ptr<GameEnvironment::Environment> environment = std::make_unique<GameEnvironment::Environment>();
    // Loop each map entry
    for (uint32_t i = 0; i < root.getNumNamedChildren(); i++){
        // Map enty has identifier should have identifier and expression which is a value of constant
        ts::Node mapEntry = root.getNamedChild(i);
        ts::Node identifierNode = mapEntry.getNamedChild(0);
        ts::Node valueNode = mapEntry.getNamedChild(1);
        // Convert an identifier of a node to string 
        std::string_view identifier = identifierNode.getSourceRange(source);

        ts::Node value = valueNode.getNamedChild(0);
        int nodeSymbol = value.getSymbol();
        auto toStore = nodeSymbolToConvert[nodeSymbol]->convertNode(value);
        environment->insert(std::make_pair(identifier, std::move(toStore)));
    }
    return environment;
}

std::unique_ptr<GameEnvironment::Value> convertNumRangeToValue(std::string_view input){
    auto range =  convertToRange(input);
    auto max = std::make_unique<GameEnvironment::Value>();
    auto min = std::make_unique<GameEnvironment::Value>();
    auto rangeValue = std::make_unique<GameEnvironment::Value>();
    std::unique_ptr<GameEnvironment::Map> rangeMap = std::make_unique<GameEnvironment::Environment>();
    min->value=range.first;
    max->value=range.second;
    rangeMap->insert(std::make_pair("min", std::move(min)));
    rangeMap->insert(std::make_pair("max", std::move(max)));
    rangeValue->value = std::move(rangeMap);
    return rangeValue;
}

std::unique_ptr<GameEnvironment::Environment>  GameStateLoader::getConfigEnvironment(const ts::Node& root){
    std::unique_ptr<GameEnvironment::Map> configMap = std::make_unique<GameEnvironment::Environment>();
    std::unique_ptr<GameEnvironment::Environment> toReturnEnvironment = std::make_unique<GameEnvironment::Environment>();
    auto toStore = std::make_unique<GameEnvironment::Value>();
    std::string_view configIdentifier = root.getChild(0).getSourceRange(source);
    int index=0;
    int numNamedChildren=root.getNumNamedChildren();
    while (index<numNamedChildren&&root.getNamedChild(index).getSymbol()!=SYMBOL::SETUP){

        ts::Extent identifierRange = root.getNamedChild(index).getPreviousSibling().getByteRange();
        std::string_view identifier = source.substr(identifierRange.start,
                                                    identifierRange.end - 1 - identifierRange.start);
        if(root.getNamedChild(index).getSymbol()==SYMBOL::NUMBER_RANGE){
            ts::Extent dataRange = root.getNamedChild(index).getByteRange();
            std::string_view data = source.substr(dataRange.start, dataRange.end - dataRange.start);
            auto rangeValue=convertNumRangeToValue(data);
            configMap->insert(std::make_pair(identifier, std::move(rangeValue)));
        }
        else {
            int nodeSymbol = root.getNamedChild(index).getSymbol();
            auto toStore = nodeSymbolToConvert[nodeSymbol]->convertNode(root.getNamedChild(index));
            configMap->insert(std::make_pair(identifier, std::move(toStore)));
        }
        index++;
    }
    //setup will add into configMap here later.
    toStore->value=std::move(configMap);
    toReturnEnvironment->insert(std::make_pair(configIdentifier, std::move(toStore)));
    return toReturnEnvironment;
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
};

void GameStateLoader::registerConversion(int symbol, std::unique_ptr<ConvertInterface> convert){
    nodeSymbolToConvert[symbol] = std::move(convert);
}

std::string_view GameStateLoader::getSource() const {
    return source;
}

const std::map<int, std::unique_ptr<ConvertInterface>>* GameStateLoader::getNodeSymbolToConvert() const {
    return &nodeSymbolToConvert;
}

std::unique_ptr<GameStateLoader> GameStateLoader::createDefaultGameStateLoader(std::string_view source){
    auto gameStateLoader = std::make_unique<GameStateLoader>(source);
    gameStateLoader->registerConversion(SYMBOL::NUMBER, std::make_unique<ConvertNodeToNumber>(gameStateLoader.get()));
    gameStateLoader->registerConversion(SYMBOL::BOOL, std::make_unique<ConvertNodeToBool>(gameStateLoader.get()));
    gameStateLoader->registerConversion(SYMBOL::STRING, std::make_unique<ConvertNodeToString>(gameStateLoader.get()));
    gameStateLoader->registerConversion(SYMBOL::LIST, std::make_unique<ConvertNodeToList>(gameStateLoader.get()));
    gameStateLoader->registerConversion(SYMBOL::MAP, std::make_unique<ConvertNodeToMap>(gameStateLoader.get()));
    return gameStateLoader;
}

std::unique_ptr<GameEnvironment::Value> 
GameStateLoader::convertNode(const ts::Node& node) const {
    int nodeSymbol = node.getSymbol();
    return nodeSymbolToConvert.at(nodeSymbol)->convertNode(node);
}