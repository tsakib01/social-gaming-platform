#include "ConstantManager.h"
#include<iostream>
#include <queue>
#include <stdexcept> 

ConstantManager::ConstantManager(const ts::Node & constantRoot, std::string_view source)
: source(source)
{    
    this->interpretConstant(constantRoot);
}

// Print node by level order
void ConstantManager::printByLevelOrder(const ts::Node& node){
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

// Print constant list with key: value1, value2, value3....
void ConstantManager::print(){
    for (const auto&[variableName, values] : constantListOfString){
        std::cout << variableName << ": ";
        for (std::string_view value : values){
            std::cout << value << ", ";
        }
        std::cout << std::endl;
    }

    for (const auto&[variableName, value] : constantListOfNumber){
        std::cout << variableName << ": ";
        std::cout << value << std::endl;
    }
}

// Convert a given node to string_view
std::string_view ConstantManager::convertQuotedStringToStringView(ts::Node node){
    int startByte = node.getByteRange().start;
    int length = node.getByteRange().end - startByte;
    return this->source.substr(startByte, length);
}

// Take an expression list containing strings, return it as a vector 
std::vector<std::string_view> ConstantManager::getStringList(const ts::Node& expressionListNode){
    std::vector<std::string_view> toReturn;
    for (uint32_t i = 0; i < expressionListNode.getNumNamedChildren(); i++){
        ts::Node expressionElementNode = expressionListNode.getNamedChild(i).getChild(0);
        // If an expression_element is a string, put it to vector
        if (expressionElementNode.getType() == "quoted_string"){
            toReturn.push_back(convertQuotedStringToStringView(expressionElementNode));
        }
    }
    return toReturn;
}


void ConstantManager::interpretConstant(const ts::Node& constantRoot){
    ts::Node valueMapNode = constantRoot.getNamedChild(0);
    
    // Loop each map entry
    for (uint32_t i = 0; i < valueMapNode.getNumNamedChildren(); i++){
        // Map enty has identifeir should have identifier and expression which is a value of constant
        ts::Node mapEntry = valueMapNode.getNamedChild(i);
        ts::Node identifierNode = mapEntry.getNamedChild(0);
        ts::Node valueNode = mapEntry.getNamedChild(1);

        // Convert an identifier of a constant node to string 
        std::string_view identifier = convertQuotedStringToStringView(identifierNode);
        
        // Check the value type is list or not
        ts::Node valueExpressionNode = valueNode.getNamedChild(0);
        if (valueExpressionNode.getType() == "list_literal"){
            // If it is a list, get an expression list
            ts::Node expressionListNode = valueExpressionNode.getNamedChild(0);
            // Get a list of strings from expression list
            std::vector<std::string_view> values = getStringList(expressionListNode); 
            // Insert a value to constant list
            constantListOfString.emplace(identifier, values);
        }
        else if (valueExpressionNode.getType() == "number"){
            int number = std::stoi(std::string(convertQuotedStringToStringView(valueExpressionNode)));
            constantListOfNumber.emplace(identifier, number);
        }
    }
}

int ConstantManager::getNumberByName(std:: string_view name){
    // Find an iterator of the given name
    auto itr = constantListOfNumber.find(name);
    // If the name not found
    if (itr == constantListOfNumber.end()){
        throw std::runtime_error("Name not found.");
    }
    return (*itr).second;
}

