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
class ConvertNodeToNumRange final : public ConvertInterface {
public:
    ConvertNodeToNumRange(const GameStateLoader* gameStateLoader)
            : ConvertInterface(gameStateLoader)
    {}
private:
    std::unique_ptr<GameEnvironment::Value> convertNodeImpl(const ts::Node& node) const{

        auto range = convertToRange(node.getSourceRange(gameStateLoader->getSource()));

        std::unique_ptr<GameEnvironment::Map> rangeMap = std::make_unique<GameEnvironment::Environment>();
        rangeMap->emplace("min", std::make_unique<GameEnvironment::Value>(range.first));
        rangeMap->emplace("max", std::make_unique<GameEnvironment::Value>(range.second));

        auto toReturn = std::make_unique<GameEnvironment::Value>(std::move(rangeMap));
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


std::unique_ptr<GameEnvironment::Environment>  GameStateLoader::getConfigEnvironment(const ts::Node& root){
    std::unique_ptr<GameEnvironment::Map> configMap = std::make_unique<GameEnvironment::Map>();
    std::unique_ptr<GameEnvironment::Environment> toReturnEnvironment = std::make_unique<GameEnvironment::Environment>();

    std::string_view configIdentifier = root.getChild(0).getSourceRange(source);

    int numNamedChildren=root.getNumNamedChildren();
    for (int index=0; index<numNamedChildren; index++){
        if(root.getNamedChild(index).getSymbol()==SYMBOL::SETUP){
            break;
        }
        ts::Extent identifierRange = root.getNamedChild(index).getPreviousSibling().getByteRange();
        std::string_view identifier = source.substr(identifierRange.start,
                                                    identifierRange.end - 1 - identifierRange.start);
        int nodeSymbol = root.getNamedChild(index).getSymbol();
        auto toStore = nodeSymbolToConvert[nodeSymbol]->convertNode(root.getNamedChild(index));
        configMap->emplace(identifier, std::move(toStore));
    }
    // (Jack) Uncomment to use rounds - should delete this after merging Allan's branch
    // configMap->emplace("rounds", std::make_unique<GameEnvironment::Value>(15));

    toReturnEnvironment->emplace(configIdentifier, std::make_unique<GameEnvironment::Value>(std::move(configMap)));
    return toReturnEnvironment;
}

std::unique_ptr<GameEnvironment::Environment>  GameStateLoader::getPerPlayerEnvironment(const ts::Node& root){
    std::unique_ptr<GameEnvironment::Environment> toReturnEnvironment = std::make_unique<GameEnvironment::Environment>();

    int numNamedChildren=root.getNumNamedChildren();
    for (int index=0; index<numNamedChildren; index++){

        std::string_view identifier = root.getNamedChild(index).getPreviousSibling().getSourceRange(source);
        int nodeSymbol = root.getNamedChild(index).getSymbol();
        auto toStore = nodeSymbolToConvert[nodeSymbol]->convertNode(root.getNamedChild(index));
        toReturnEnvironment->emplace(identifier, std::move(toStore));
    }

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
    gameStateLoader->registerConversion(SYMBOL::NUMBER_RANGE, std::make_unique<ConvertNodeToNumRange>(gameStateLoader.get()));
    return gameStateLoader;
}

std::unique_ptr<GameEnvironment::Value> 
GameStateLoader::convertNode(const ts::Node& node) const {
    int nodeSymbol = node.getSymbol();
    return nodeSymbolToConvert.at(nodeSymbol)->convertNode(node);
}