#ifndef GAME_ENVIRONMENT_H
#define GAME_ENVIRONMENT_H


#include<map>
#include<string_view>
#include<variant>
#include<vector>
#include <memory>
#include <stdexcept>

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
    using Map = std::map<Identifier, std::unique_ptr<Value>>;
    using List = std::vector<std::unique_ptr<Value>>;
    
    class Value{
    private:
        // Visitor for copy constructor
        struct CopyVisitor{
            std::unique_ptr<Value> operator()(int number) const {
                return std::make_unique<GameEnvironment::Value>(number);
            }

            std::unique_ptr<Value> operator()(bool boolean) const {
                return std::make_unique<GameEnvironment::Value>(boolean);
            }

            std::unique_ptr<Value> operator()(std::string_view string) const {
                return std::make_unique<GameEnvironment::Value>(string);
            }

            std::unique_ptr<Value> operator()(const std::unique_ptr<List>& list){
                auto copyList = std::make_unique<List>();

                for (const auto& element : *list){
                    copyList->push_back(std::move(std::visit(*this, element->value)));
                }
                return std::make_unique<Value>(std::move(copyList));
            }

            std::unique_ptr<Value> operator()(const std::unique_ptr<Map>& map){
                auto copyMap = std::make_unique<Map>();

                for (const auto& [key, value] : *map){
                    copyMap->emplace(key, std::move(std::visit(*this, value->value)));
                }
                return std::make_unique<Value>(std::move(copyMap));
            }

            template <typename T>
            std::unique_ptr<Value> operator()([[maybe_unused]] const T& value){
                throw std::runtime_error("Unsupported value");
            }
        };
    public:
        Value() = default;
        Value(int value) : value(value) {}
        Value(bool value) : value(value) {}
        Value(std::string_view value) : value(value) {}
        Value(std::unique_ptr<Map> value) : value(std::move(value)) {}
        Value(std::unique_ptr<List> value) : value(std::move(value)) {}
        
        // Copy constructor does deep copy
        Value(const Value& other) 
        : value(std::move(std::visit(CopyVisitor{}, other.value)->value))
        {}
        
        std::variant<int, bool, std::string_view, std::unique_ptr<Map>, std::unique_ptr<List> > value;
    };

    using Environment = std::map<Identifier, std::unique_ptr<Value>>;
};
#endif

