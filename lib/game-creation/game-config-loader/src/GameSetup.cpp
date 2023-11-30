#include "GameSetup.h"
GameSetup::GameSetup (std::shared_ptr<GameSetupLoader> SetupLoader):
    setups(std::vector<std::unique_ptr<SetupInstance>>()), SetupLoader(SetupLoader){}

void GameSetup::addSetups(ts::Node node){
    setups=std::move(SetupLoader->getGameSetup(node));
}
void GameSetup::processGameSetups(){
    for(auto const& setup : setups){
        setup->processSetup();
    }
}
std::vector<std::string_view> GameSetup::getIdentifiers(){
    std::vector<std::string_view> identifiers;
    for(auto const& setup :setups){
        identifiers.push_back(setup->getIdentifier());
    }
    return identifiers;
}
std::vector<std::string_view> GameSetup::getPrompts(){
    std::vector<std::string_view> prompts;
    for(auto const& setup :setups){
        prompts.push_back(setup->getPrompt());
    }
    return prompts;
}
std::vector<std::string_view> GameSetup::getRestInfos(){
    std::vector<std::string_view> restInfos;
    for(auto const& setup :setups){
        restInfos.push_back(setup->getRestInfo());
    }
    return restInfos;
}
bool GameSetup::isResponseValid(std::string_view identifier,std::string_view response){
    for(auto const& setup :setups){
        if(identifier == setup->getIdentifier()){
            return setup->checkResponse(response);
        }
    }
    return false;
}
bool GameSetup::hasSetup() { 
    return setups.size() > 0;
}