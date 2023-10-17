#include "GameSetupLoader.h"
#include <iostream>
Setup::Setup(KIND kind,std::string_view prompt):kind(kind),prompt(prompt),restInfo(std::string_view{}){}

Setup::Setup(KIND kind,std::string_view prompt,std::string_view restInfo):kind(kind),prompt(prompt),restInfo(restInfo){}

constexpr const char* KINDToString(KIND kind) noexcept
{
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
}
GameSetupLoader::GameSetupLoader(std::string_view source){

}
Setup GameSetupLoader::convertNodetoSetup(const ts::Node& node){

}
std::unique_ptr<std::vector<Setup>> GameSetupLoader::getGameSetup (const ts::Node& node){

}

