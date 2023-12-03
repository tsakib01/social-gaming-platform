#include "GameEnvironment.h"

std::unique_ptr<GameEnvironment::Value> GameEnvironment::Value::getValueInMap(std::string_view identifier){
    getValueVisitor visitor(identifier);
    return std::move(std::visit(visitor, value));
}