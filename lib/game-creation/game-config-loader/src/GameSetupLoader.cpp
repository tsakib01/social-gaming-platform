#include "GameSetupLoader.h"
#include <iostream>
#include <charconv>
#include <algorithm>
SetupInstance::SetupInstance(std::string_view identifier,KIND kind,std::string_view prompt):identifier(identifier),kind(kind),prompt(prompt),restInfo(std::string_view{}){}

SetupInstance::SetupInstance(std::string_view identifier,KIND kind,std::string_view prompt,std::string_view restInfo, Domain domain):identifier(identifier),kind(kind),prompt(prompt),restInfo(restInfo), domain(domain){}
KIND convertToKIND(std::string_view kind){
    if(kind!="integer"&&kind!="string"&&kind!="boolean"&&kind!="enum"){
        throw std::runtime_error("kind not support");
    }
    if(kind=="integer"){
        return KIND::INTEGER;
    }
    else if(kind=="string"){
        return KIND::STRING;
    }
    else if(kind=="boolean"){
        return KIND::BOOLEAN;
    }
    else {
        return KIND::ENUM;
    }
}

constexpr std::string_view KINDToString(KIND kind)
{
    if(kind!=KIND::INTEGER&&kind!=KIND::STRING&&kind!=KIND::BOOLEAN&&kind!=KIND::ENUM){
        throw std::runtime_error("kind not support yet");
    }
    switch (kind)
    {
    case KIND::INTEGER: return "integer";
    case KIND::STRING: return "string";
    case KIND::BOOLEAN: return "boolean";
    case KIND::ENUM: return "enum";
    }

}
std::string_view SetupInstance::getIdentifier(){
    return identifier;
}
std::string_view SetupInstance::getPrompt(){
    return prompt;
}
std::string_view SetupInstance::getRestInfo(){
    return restInfo;
}
KIND SetupInstance::getKind(){
    return kind;
}
void SetupInstance::intProcess(){
    this->print();
}
void SetupInstance::boolProcess(){
    this->print();
}
void SetupInstance::strProcess(){
    this->print();
}
void SetupInstance::enumProcess(){
    this->print();
}
void SetupInstance::processSetup(){
    if(kind == KIND::INTEGER){
        this->intProcess();
    }
    if(kind == KIND::STRING){
        this->strProcess();
    }
    if(kind == KIND::BOOLEAN){
        this->boolProcess();
    }
    if(kind == KIND::ENUM){
        this-> enumProcess();
    }
}
void SetupInstance::print(){
    std::cout<<identifier<<std::endl;
    std::cout<<KINDToString(kind)<<std::endl;
    std::cout<<prompt<<std::endl;
    std::cout<<restInfo<<std::endl;
    if(kind==KIND::INTEGER||kind==KIND::ENUM){
        std::visit(DomainPrinter{}, domain);
    }

    std::cout<<std::endl;
}
bool SetupInstance::checkResponse(std::string_view response){
    switch (kind)
    {
        case KIND::INTEGER:
            return std::visit([&response](const auto& arg) {
                return DomainChecker{}(arg, response);
            }, domain);

        case KIND::STRING: return true;
        case KIND::BOOLEAN:
            if(response=="y"||response=="n"){
                return true;
            }
            else{
                return false;
            }
        case KIND::ENUM: return std::visit([&response](const auto& arg) {
                return DomainChecker{}(arg, response);
            }, domain);
    }
}

//DomainPrinter
void DomainPrinter::operator()(const Range& range) const {
    std::cout << "Range: (" << range.first << ", " << range.second << ")\n";
}

void DomainPrinter::operator()(const ChoiceList& choices) const {
    std::cout << "ChoiceList:\n";
    for (const auto& [key, value] : choices) {
        std::cout << "  " << key << ": " << value << "\n";
    }
}
//DomainChecker
bool DomainChecker::operator()(const Range& range, std::string_view input) const {
    int num;
    auto[numPtr, numEc] = std::from_chars(input.data(), input.data() + input.size(), num);
    if (numEc != std::errc()) {
        return false;
    }
    if(num >= range.first && num <= range.second){
        return true;
    }
    else{
        return false;
    }
}

bool DomainChecker::operator()(const ChoiceList& choices, std::string_view input) const {
    if(choices.find(input)==choices.end()){
        return false;
    }
    else{
        return true;
    }
}
GameSetupLoader::GameSetupLoader(std::string_view source):source(source){}

Range convertToRange(std::string_view restInfo){
    auto openParen = restInfo.find('(');
    auto closeParen = restInfo.find(')');

    // If either parenthesis is not found, or they are in the wrong order, throw an error
    if (openParen == std::string_view::npos || closeParen == std::string_view::npos || closeParen < openParen) {
        throw std::invalid_argument("Invalid input string");
    }
    auto commaCount=std::count(restInfo.begin(), restInfo.end(), ',');
    if(commaCount!=1){
        throw std::invalid_argument("Invalid input string");
    }
    // Extract the numbers as a substring
    std::string_view numbers = restInfo.substr(openParen + 1, closeParen - openParen - 1);

    // Trim the spaces around the comma to properly find its position
    auto comma = numbers.find(',');
    if (comma == std::string_view::npos) {
        throw std::invalid_argument("Invalid range format");
    }

    // Trim spaces from the first number
    std::string_view firstNum = numbers.substr(0, comma);
    firstNum.remove_prefix(std::min(firstNum.find_first_not_of(' '), firstNum.size()));

    // Trim spaces from the second number
    std::string_view secondNum = numbers.substr(comma + 1);
    secondNum.remove_prefix(std::min(secondNum.find_first_not_of(' '), secondNum.size()));

    // Parse the first number
    int first, second;
    auto[firstPtr, firstEc] = std::from_chars(firstNum.data(), firstNum.data() + firstNum.size(), first);
    if (firstEc != std::errc()) {
        throw std::invalid_argument("Invalid first number");
    }

    // Parse the second number
    auto[secondPtr, secondEc] = std::from_chars(secondNum.data(), secondNum.data() + secondNum.size(), second);
    if (secondEc != std::errc()) {
        throw std::invalid_argument("Invalid second number");
    }

    // Return the pair of numbers
    return {first, second};
}

ChoiceList convertToChoiceList( std::string_view restInfo){
    auto quotesCount=std::count(restInfo.begin(), restInfo.end(), '\'');
    if(quotesCount == 0 || quotesCount % 2 != 0){
        throw std::runtime_error("Wrong number of quote");
    }
    ChoiceList choices;
    while (!restInfo.empty()) {
        // Find the start of the key
        std::size_t keyStart = restInfo.find('\'');
        if (keyStart == std::string_view::npos) break; // No more keys
        restInfo.remove_prefix(keyStart + 1); // Skip past the starting quote

        // Find the end of the key
        std::size_t keyEnd = restInfo.find('\'');
        if (keyEnd == std::string_view::npos) throw std::runtime_error("Mismatched quotes in key");
        std::string_view key = restInfo.substr(0, keyEnd);
        restInfo.remove_prefix(keyEnd + 1); // Skip past the ending quote

        // Find the start of the value
        std::size_t valueStart = restInfo.find('\'');
        if (valueStart == std::string_view::npos) throw std::runtime_error("No starting quote for value");
        restInfo.remove_prefix(valueStart + 1); // Skip past the starting quote

        // Find the end of the value
        std::size_t valueEnd = restInfo.find('\'');
        if (valueEnd == std::string_view::npos) throw std::runtime_error("Mismatched quotes in value");
        std::string_view value = restInfo.substr(0, valueEnd);
        restInfo.remove_prefix(valueEnd + 1); // Skip past the ending quote

        // Add the parsed key-value pair to the map
        choices.emplace(key, value);

        // Skip any trailing commas or whitespace before the next key
        std::size_t nextKeyStart = restInfo.find_first_not_of(", \t\n\r");
        if (nextKeyStart != std::string_view::npos) {
            restInfo.remove_prefix(nextKeyStart);
        }
    }
    return choices;
}


Domain convertStringToDomain(const KIND kind, const std::string_view restInfo){

    if(kind!=KIND::INTEGER&&kind!=KIND::ENUM){
        throw std::runtime_error("kind not support yet");
    }
    Domain toReturn;
    if(kind==KIND::INTEGER){
        toReturn = convertToRange(restInfo);
    }
    if(kind==KIND::ENUM){
        toReturn = convertToChoiceList(restInfo);
    }
    return toReturn;
}


std::unique_ptr<SetupInstance> GameSetupLoader::convertNodetoSetup(const ts::Node& node){
    if(node.getNumChildren()<7){
        throw std::runtime_error("no enough information in setup");
    }
    int childCount =node.getNumChildren();
    ts::Extent  identifierRange=node.getChild(0).getByteRange();
    std::string_view identifier=source.substr(identifierRange.start,identifierRange.end-identifierRange.start);
    KIND kind=convertToKIND(node.getChild(3).getType());
    std::string_view prompt=node.getChild(5).getSourceRange(source);
    if(childCount>7){

        ts::Extent first=node.getChild(6).getByteRange();

        ts::Extent last=node.getChild(childCount-2).getByteRange();
        std::string_view restInfo=source.substr(first.start,last.end-first.start);
        return std::make_unique<SetupInstance>(SetupInstance{identifier,kind,prompt,restInfo,convertStringToDomain(kind,restInfo)});
    }
    return std::make_unique<SetupInstance>(SetupInstance{identifier,kind,prompt});

}
std::vector<std::unique_ptr<SetupInstance>> GameSetupLoader::getGameSetup (const ts::Node& node){
    int numChildCount=node.getNumNamedChildren();

    std::vector< std::unique_ptr<SetupInstance>> setups;

    for(int index=0;index<numChildCount;index++){
        if(node.getNamedChild(index).getSymbol()==92) {
            setups.push_back(convertNodetoSetup(node.getNamedChild(index)));
        }
    }
    return setups;
}

