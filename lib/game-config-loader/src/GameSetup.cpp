#include "GameSetup.h"
GameSetup::GameSetup (std::shared_ptr<GameSetupLoader> SetupLoader):setups(std::make_unique<std::vector<Setup>>()), SetupLoader(SetupLoader){}

void GameSetup::addSetups(ts::Node node){
    setups=std::move(SetupLoader->getGameSetup(node));
//    int count=node.getNumNamedChildren();
//    for(int index = 3; index < count; index++){
//
//    }
}
void GameSetup::processGameSetups(){
    for(auto setup : *setups){
        setup.processSetup();
    }
}