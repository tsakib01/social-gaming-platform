#include "GameSetupLoader.h"
#include <iostream>
Setup::Setup(KIND kind,std::string_view prompt):kind(kind),prompt(prompt),restInfo(std::string_view{}){}

Setup::Setup(KIND kind,std::string_view prompt,std::string_view restInfo):kind(kind),prompt(prompt),restInfo(restInfo){}
KIND convertToKIND(std::string_view kind){
    std::string temp{kind.begin(),kind.end()};
    if(temp=="integer"){
        return KIND::INTEGER;
    }
    else if(temp=="string"){
        return KIND::STRING;
    }
    else if(temp=="boolean"){
        return KIND::BOOLEAN;
    }
    else if(temp=="enum"){
        return KIND::ENUM;
    }
    else {
        std::runtime_error("kind not support");
    }

}
constexpr const char* KINDToString(KIND kind) noexcept
{
    switch (kind)
    {
    case KIND::INTEGER: return "integer";
    case KIND::STRING: return "string";
    case KIND::BOOLEAN: return "boolean";
    case KIND::ENUM: return "enum";
    }
    std::runtime_error("kind not support yet");
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
}
GameSetupLoader::GameSetupLoader(std::string_view source):source(source){}

Setup GameSetupLoader::convertNodetoSetup(const ts::Node& node){
    int childCount =node.getNumChildren();
    if(childCount<7){
        std::runtime_error("no enough information in setup");
    }
    KIND kind=convertToKIND(node.getChild(3).getType());
    std::string_view prompt=node.getChild(5).getSourceRange(source);
    if(childCount>7){

        ts::Extent first=node.getChild(6).getByteRange();

        ts::Extent last=node.getChild(childCount-2).getByteRange();
        std::string_view restInfo=source.substr(first.start,last.end-first.start);
        return {kind,prompt,restInfo};
    }
    return {kind,prompt};

}
std::unique_ptr<std::vector<Setup>> GameSetupLoader::getGameSetup (const ts::Node& node){
    int numChildCount=node.getNumNamedChildren();

    std::unique_ptr<std::vector<Setup>> setups=std::make_unique<std::vector<Setup>>();

    for(int index=3;index<numChildCount;index++){
        setups->push_back(convertNodetoSetup(node.getNamedChild(index)));
    }
    return setups;
}

