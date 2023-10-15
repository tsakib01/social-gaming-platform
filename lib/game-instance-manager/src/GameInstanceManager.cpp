#include "GameInstanceManager.h"

GameInstanceManager::GameInstanceManager() {
    std::cout << "GameInstanceManager created." << std::endl;
}

std::string_view GameInstanceManager::sendGameList() {
    // detect list of games in /games
    // send to msg handler
    return "";
}

void GameInstanceManager::sendGameConfiguration(std::string_view path) {

}

void GameInstanceManager::sendInviteCode() {
    // generate code
    // send to msg handler
}

void GameInstanceManager::createGameInstance() {

}

void GameInstanceManager::addGameToActiveList() {
    // 
}

void GameInstanceManager::removeGameFromActiveList() {

}

void GameInstanceManager::addPlayer() {
    // find game
    // inject player into game
}

void GameInstanceManager::runCycle() {
    // receiveFromMessageHandler();
    // for each game in activeGameList
    //      game.execute

    std::cout << "Enter loop" << std::endl;

    // sendToMessageHandler();
}

void GameInstanceManager::sendToMessageHandler() {
    // for each message in outgoing messages -> send to user
}

void GameInstanceManager::receiveFromMessageHandler() {
    // for each message in incoming messages -> send to specific game (function)
}