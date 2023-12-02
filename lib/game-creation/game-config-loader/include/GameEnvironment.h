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
class Value;
class getValueVisitor;
namespace GameEnvironment{
    // Identifier of value
    using Identifier = std::string_view;

    class Value;
    using Map = std::map<Identifier, std::unique_ptr<Value>>;
    using List = std::vector<std::unique_ptr<Value>>;
    class Value{
    public:
        Value() = default;
        Value(int value) : value(value) {}
        Value(bool value) : value(value) {}
        Value(std::string_view value) : value(value) {}
        Value(std::unique_ptr<Map> value) : value(std::move(value)) {}
        Value(std::unique_ptr<List> value) : value(std::move(value)) {}
        std::unique_ptr<Value> getValueInMap(std::string_view identifier);
        std::variant<int, bool, std::string_view, std::unique_ptr<Map>, std::unique_ptr<List> > value;
    };

    using Environment = std::map<Identifier, std::unique_ptr<Value>>;
};
class getValueVisitor{
public:
    explicit getValueVisitor(GameEnvironment::Identifier identifier) : identifier(identifier) {}
    std::unique_ptr<GameEnvironment::Value> operator()( std::unique_ptr<GameEnvironment::Map>& value)  {
        if(value->find(identifier)!=value->end()){
            return std::move(value->find(identifier)->second);
        }
        else{
            return std::make_unique<GameEnvironment::Value>();
        }
    }
    std::unique_ptr<GameEnvironment::Value> operator()(int ) {
        return std::make_unique<GameEnvironment::Value>();
    }
    std::unique_ptr<GameEnvironment::Value> operator()(bool)  {
        return std::make_unique<GameEnvironment::Value>();
    }
    std::unique_ptr<GameEnvironment::Value> operator()( std::string_view& )  {
        return std::make_unique<GameEnvironment::Value>();
    }
    std::unique_ptr<GameEnvironment::Value> operator()( std::unique_ptr<GameEnvironment::List>& )  {
        return std::make_unique<GameEnvironment::Value>();
    }
private:
    std::string_view identifier;
};

#endif

