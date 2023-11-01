#include "GameSetupLoader.h"
#include <iostream>
Setup::Setup(std::string_view identifier,KIND kind,std::string_view prompt):identifier(identifier),kind(kind),prompt(prompt),restInfo(std::string_view{}){}

Setup::Setup(std::string_view identifier,KIND kind,std::string_view prompt,std::string_view restInfo):identifier(identifier),kind(kind),prompt(prompt),restInfo(restInfo){}
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
void Setup::intProcess(){
    this->print();
}
void Setup::boolProcess(){
    this->print();
}
void Setup::strProcess(){
    this->print();
}
void Setup::processSetup(){
    if(kind==KIND::INTEGER){
        this->intProcess();
    }
    if(kind==KIND::STRING){
        this->strProcess();
    }
    if(kind==KIND::BOOLEAN){
        this->boolProcess();
    }
}
void Setup::print(){
    std::cout<<identifier<<std::endl;
    std::cout<<KINDToString(kind)<<std::endl;
    std::cout<<prompt<<std::endl;
    std::cout<<restInfo<<std::endl;
    std::cout<<std::endl;
}
GameSetupLoader::GameSetupLoader(std::string_view source):source(source){}

std::unique_ptr<Setup> GameSetupLoader::convertNodetoSetup(const ts::Node& node){
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
        return std::make_unique<Setup>(Setup{identifier,kind,prompt,restInfo});
    }
    return std::make_unique<Setup>(Setup{identifier,kind,prompt});

}
std::vector<std::unique_ptr<Setup>> GameSetupLoader::getGameSetup (const ts::Node& node){
    int numChildCount=node.getNumNamedChildren();

    std::vector< std::unique_ptr<Setup>> setups;

    for(int index=3;index<numChildCount;index++){
        setups.push_back(convertNodetoSetup(node.getNamedChild(index)));
    }
    return setups;
}

