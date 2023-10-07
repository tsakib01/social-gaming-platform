#include "ConstantManager.h"
#include<iostream>


ConstantManager::ConstantManager(const ts::Node & constantRoot, const std::string_view& source)
: source(source)
{    
    this->interpretConstant(constantRoot);
    this->print();
}

// Print node by level order
void ConstantManager::print_by_level_order(const ts::Node& node){
    std::queue<ts::Node> node_queue;
    node_queue.push(node);

    while (!node_queue.empty()){
        int queue_size = node_queue.size();

        for (int i = 0; i < queue_size; i++){
            ts::Node node = node_queue.front();
            node_queue.pop();
            std::cout << node.getType() << ", ";
            for (uint32_t j = 0; j < node.getNumNamedChildren(); j++){
                node_queue.push(node.getNamedChild(j));
            }
        }
        std::cout << "\n";
    }
}

// Print constant list with key: value1, value2, value3....
void ConstantManager::print(){
    for (const auto&[variable_name, values] : constant_list){
        std::cout << variable_name << ": ";
        for (std::string_view value : values){
            std::cout << value << ", ";
        }
        std::cout << std::endl;
    }
}

// Convert a given node to string_view
std::string_view ConstantManager::convert_quoted_string_to_string_view(ts::Node node){
    int startByte = node.getByteRange().start;
    int length = node.getByteRange().end - startByte;
    return this->source.substr(startByte, length);
}

// Take an expression list containing strings, return it as a vector 
std::vector<std::string_view> ConstantManager::get_string_list(const ts::Node& expression_list_node){
    std::vector<std::string_view> to_return;
    for (uint32_t i = 0; i < expression_list_node.getNumNamedChildren(); i++){
        ts::Node expression_element_node = expression_list_node.getNamedChild(i).getChild(0);
        // If an expression_element is a string, put it to vector
        if (expression_element_node.getType() == "quoted_string"){
            to_return.push_back(convert_quoted_string_to_string_view(expression_element_node));
        }
    }
    return to_return;
}


void ConstantManager::interpretConstant(const ts::Node& constantRoot){
    ts::Node value_map_node = constantRoot.getNamedChild(0);
    
    // Loop each map entry
    for (uint32_t i = 0; i < value_map_node.getNumNamedChildren(); i++){
        // Map enty has identifeir should have identifier and expression which is a value of constant
        ts::Node map_entry = value_map_node.getNamedChild(i);
        ts::Node identifier_node = map_entry.getNamedChild(0);
        ts::Node value_node = map_entry.getNamedChild(1);

        // Convert an identifier of a constant node to string 
        std::string_view identifier = convert_quoted_string_to_string_view(identifier_node);
        
        // Check the value type is list or not
        ts::Node value_expression_node = value_node.getNamedChild(0);
        if (value_expression_node.getType() == "list_literal"){
            // If it is a list, get an expression list
            ts::Node expression_list_node = value_expression_node.getNamedChild(0);
            // Get a list of strings from expression list
            std::vector<std::string_view> values = get_string_list(expression_list_node); 
            // Insert a value to constant list
            constant_list.emplace(identifier, values);
        }
    }
}