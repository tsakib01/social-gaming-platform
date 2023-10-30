#ifndef GAME_ENVIRONMENT_H
#define GAME_ENVIRONMENT_H


#include<map>
#include<string_view>
#include<variant>
#include<vector>
#include <memory>

/**
 * This namespace represents an environment in the game state.
 * Environment is a map which maps from identifier to the value.
 * Value must be either an Integer, a string, bool, a map or a list.
 * Map and List can contain anything valid Value can contain.
*/
namespace GameEnvironment{
    // Identifier of value
    using Identifier = std::string_view;

    class Value;
    class Map{
    public:
        std::map<Identifier, std::unique_ptr<Value>> value;
        ~Map(){};
    };

    class Value;
    class List{
    public:
        std::vector<std::unique_ptr<Value>> value;
        ~List(){};
    };
    
    class Value{
    public:
        std::variant<int, bool, std::string_view, std::unique_ptr<Map>, std::unique_ptr<List>> value;
        ~Value(){};
    };

    using Environment = std::map<Identifier, std::unique_ptr<Value>>;
};
#endif

