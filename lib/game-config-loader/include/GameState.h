#include <nlohmann/json.hpp>
#include <string_view>
using json = nlohmann::json;

class GameState{
public:
    GameState(std::string_view source);
private:
    json data;
};