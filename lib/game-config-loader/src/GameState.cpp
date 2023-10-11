#include "GameState.h"
#include <iostream>

GameState::GameState(std::string_view source){
    std::string_view string = source.substr(10, source.size() - 10);
    std::cout << string << std::endl;
    data = json::parse(string);
    std::cout << data["x"] << std::endl;
    std::cout << data["names"][0] << std::endl;
}