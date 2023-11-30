#include "Evaluator.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <random>

//--------------------------------------------------------------- OPERATION CLASS IMPLEMENTATIONS --------------------------------- 

// Add operation supports
// 1. Addition of two integers
// 2. Concatenation of two strings
class AddOperation final : public Operation {
private:
    struct AddVisitor {
        GameEnvironment::Value operator()(const int& left, const int& right){
            GameEnvironment::Value value;
            value.value = left + right;
            return value;
        }

        GameEnvironment::Value operator()(const std::string_view& left, const std::string_view& right){
            GameEnvironment::Value value;
            // value.value = std::string(left) + std::string(right);
            return value;
        }

        template <typename T, typename U>
        GameEnvironment::Value operator()([[maybe_unused]] const T& left, [[maybe_unused]] const U& right){
            throw std::runtime_error("Unsupported types for add");
        }
    };

    // Add operation requires 2 arguments
    bool getSpecificationImpl(std::vector<const GameEnvironment::Value*> values) const override {
        return values.size() == 2;
    };

    GameEnvironment::Value evaluateImpl(std::vector<const GameEnvironment::Value*> values) const override{
        return std::visit(AddVisitor{}, values[0]->value, values[1]->value);
    } 
};

// Subtract operation supports
// 1. Subtraction of two integers
// The result will be values[0] - values[1]
class SubtractOperation final : public Operation {
private:
    struct SubtractVisitor {
        GameEnvironment::Value operator()(const int& left, const int& right){
            GameEnvironment::Value value;
            value.value = left - right;
            return value;
        }

        template <typename T, typename U>
        GameEnvironment::Value operator()([[maybe_unused]] const T& left, [[maybe_unused]] const U& right){
            throw std::runtime_error("Unsupported types for subtract");
        }
    };

    // Subtract operation requires 2 arguments
    bool getSpecificationImpl(std::vector<const GameEnvironment::Value*> values) const override {
        return values.size() == 2;
    };

    GameEnvironment::Value evaluateImpl(std::vector<const GameEnvironment::Value*> values) const override{
        return std::visit(SubtractVisitor{}, values[0]->value, values[1]->value);
    } 
};

// Multiply operation supports
// 1. Multiply of two integers
// The result will be values[0] * values[1]
class MultiplyOperation final : public Operation {
private:
    struct MultiplyOperationVisitor {
        GameEnvironment::Value operator()(const int& left, const int& right){
            GameEnvironment::Value value;
            value.value = left * right;
            return value;
        }

        template <typename T, typename U>
        GameEnvironment::Value operator()([[maybe_unused]] const T& left, [[maybe_unused]] const U& right){
            throw std::runtime_error("Unsupported types for multiply");
        }
    };

    // Multiply operation requires 2 arguments
    bool getSpecificationImpl(std::vector<const GameEnvironment::Value*> values) const override {
        return values.size() == 2;
    };

    GameEnvironment::Value evaluateImpl(std::vector<const GameEnvironment::Value*> values) const override{
        return std::visit(MultiplyOperationVisitor{}, values[0]->value, values[1]->value);
    } 
};

// Divide operation supports
// 1. Division of two integers
// The result will be values[0] / values[1]
// values[1] should not be 0
class DivideOperation final : public Operation {
private:
    struct DivideOperationVisitor {
        GameEnvironment::Value operator()(const int& left, const int& right){
            // 0 division
            if (right == 0){
                throw std::runtime_error("0 division is not allowed"); 
            }
            GameEnvironment::Value value;
            value.value = left / right;
            return value;
        }

        template <typename T, typename U>
        GameEnvironment::Value operator()([[maybe_unused]] const T& left, [[maybe_unused]] const U& right){
            throw std::runtime_error("Unsupported types for addition");
        }
    };

    // Divide operation requires 2 arguments
    bool getSpecificationImpl(std::vector<const GameEnvironment::Value*> values) const override {
        return values.size() == 2;
    };

    GameEnvironment::Value evaluateImpl(std::vector<const GameEnvironment::Value*> values) const override{
        return std::visit(DivideOperationVisitor{}, values[0]->value, values[1]->value);
    } 
};

// OR operation supports
// Taking OR for two boolean operands
class OrOperation final : public Operation {
private:
    struct OrOperationVisitor {
        GameEnvironment::Value operator()(const bool& left, const bool& right){
            GameEnvironment::Value value;
            value.value = (left || right);
            return value;
        }

        template <typename T, typename U>
        GameEnvironment::Value operator()([[maybe_unused]] const T& left, [[maybe_unused]] const U& right){
            throw std::runtime_error("Unsupported types for OR");
        }
    };

    // OR operation requires 2 arguments
    bool getSpecificationImpl(std::vector<const GameEnvironment::Value*> values) const override {
        return values.size() == 2;
    };

    GameEnvironment::Value evaluateImpl(std::vector<const GameEnvironment::Value*> values) const override{
        return std::visit(OrOperationVisitor{}, values[0]->value, values[1]->value);
    } 
};

// AND operation supports
// Taking AND for two boolean operands
class AndOperation final : public Operation {
private:
    struct OrOperationVisitor {
        GameEnvironment::Value operator()(const bool& left, const bool& right){
            GameEnvironment::Value value;
            value.value = (left && right);
            return value;
        }

        template <typename T, typename U>
        GameEnvironment::Value operator()([[maybe_unused]] const T& left, [[maybe_unused]] const U& right){
            throw std::runtime_error("Unsupported types for AND");
        }
    };

    // AND operation requires 2 arguments
    bool getSpecificationImpl(std::vector<const GameEnvironment::Value*> values) const override {
        return values.size() == 2;
    };

    GameEnvironment::Value evaluateImpl(std::vector<const GameEnvironment::Value*> values) const override{
        return std::visit(OrOperationVisitor{}, values[0]->value, values[1]->value);
    } 
};

// NOT operation supports
// Taking NOT for one boolean operand
class NotOperation final : public Operation {
private:
    struct NotOperationVisitor {
        GameEnvironment::Value operator()(const bool& operand){
            GameEnvironment::Value value;
            value.value = !operand;
            return value;
        }

        template <typename T>
        GameEnvironment::Value operator()([[maybe_unused]] const T& op){
            throw std::runtime_error("Unsupported types for NOT");
        }
    };

    // NOT operation requires only 1 argument
    bool getSpecificationImpl(std::vector<const GameEnvironment::Value*> values) const override {
        return values.size() == 1;
    };

    GameEnvironment::Value evaluateImpl(std::vector<const GameEnvironment::Value*> values) const override{
        return std::visit(NotOperationVisitor{}, values[0]->value);
    } 
};

// Equal operation supports
// Compare equality of two Values
class EqualOperation final : public Operation {
private:
    struct EqualOperationVisitor {
        GameEnvironment::Value operator()(const int& left, const int& right){
            bool isEqual = (left == right);
            return GameEnvironment::Value(isEqual);
        }

        GameEnvironment::Value operator()(const bool& left, const bool& right){
            bool isEqual = (left == right);
            return GameEnvironment::Value(isEqual);
        }

        GameEnvironment::Value operator()(const std::string_view& left, const std::string_view& right){
            bool isEqual = (left == right);
            return GameEnvironment::Value(isEqual);
        }

        GameEnvironment::Value operator()(const std::unique_ptr<GameEnvironment::List>& left, const std::unique_ptr<GameEnvironment::List>& right){
            GameEnvironment::Value value;
            // Size different
            if (left->size() != right->size()){
                value.value = false;
                return value;
            }

            bool result = true;
            for (size_t i = 0; i < left->size(); i++){
                GameEnvironment::Value isEqual = std::visit(EqualOperationVisitor{}, left->at(i)->value, right->at(i)->value);
                result = result && std::get<bool>(isEqual.value);
            }

            value.value = result;
            return value;
        }

        GameEnvironment::Value operator()(const std::unique_ptr<GameEnvironment::Map>& left, const std::unique_ptr<GameEnvironment::Map>& right){
            GameEnvironment::Value value;
            
            // Size different
            if (left->size() != right->size()){
                value.value = false;
                return value;
            }

            bool result = true;
            for (const auto& [leftKey, leftValue] : *left){
                // When left identifier is not found in right
                auto rightItr = right->find(leftKey);
                if (rightItr == right->end()){
                    result = false;
                    break;
                }
                GameEnvironment::Value isEqual = std::visit(EqualOperationVisitor{}, leftValue->value, rightItr->second->value);
                result = result && std::get<bool>(isEqual.value);
            }
            value.value = result;
            return value;
        }

        template <typename T, typename U>
        GameEnvironment::Value operator()([[maybe_unused]] const T& left, [[maybe_unused]] const U& right){
            return GameEnvironment::Value(false);
        }
    };

    // Divide operation requires 2 arguments
    bool getSpecificationImpl(std::vector<const GameEnvironment::Value*> values) const override {
        return values.size() == 2;
    };

    GameEnvironment::Value evaluateImpl(std::vector<const GameEnvironment::Value*> values) const override{
        return std::visit(EqualOperationVisitor{}, values[0]->value, values[1]->value);
    } 
};

// Register operation to the map
void Evaluator::registerOperation(OPERATOR operatorEnum, std::unique_ptr<Operation> operation){
    auto [it, succeeded] = operatorToOperation.try_emplace(operatorEnum, std::move(operation));
    
    // When the given operation is already registered
    if (!succeeded){
        std::runtime_error("You cannot add two operations to same operator.");
    }
}

GameEnvironment::Value Evaluator::evaluate(OPERATOR operationEnum, std::vector<const GameEnvironment::Value*> values){
    auto operationItr = operatorToOperation.find(operationEnum);
    // No operation registered
    if (operationItr == operatorToOperation.end()){
        std::runtime_error("The operator is not registered");
    }
    return operationItr->second->evaluate(values);
}

//--------------------------------------------------------------- LIST MODDIFY OPERATION CLASS IMPLEMENTATIONS --------------------------------- 

// Reverses items of the list, which can hold any type  
class ReverseListOperation final : public ModifyOperation {
private:
    struct ReverseListVisitor {

        void operator()([[maybe_unused]] std::unique_ptr<GameEnvironment::List>& list){
            std::reverse(list->begin(), list->end());
        }

        template <typename T>
        void operator()([[maybe_unused]] T& list){
            throw std::runtime_error("Unsupported types for reverse");
        }
    };

    // Add operation requires 1 arguments
    bool getSpecificationImpl(std::vector<GameEnvironment::Value*> values) const override {
        return values.size() == 1;
    };

    void evaluateImpl(std::vector<GameEnvironment::Value*> values) override{
        std::visit(ReverseListVisitor{}, values[0]->value);
    }; 
};

// Shuffles items of the list, which can hold any type
class ShuffleListOperation final : public ModifyOperation {
private:
    struct ShuffleListVisitor {
        void operator()(std::unique_ptr<GameEnvironment::List>& list) {
            // Create a random number generator
            std::random_device rd;
            std::mt19937 g(rd());

            // Shuffle the list
            std::shuffle(list->begin(), list->end(), g);
        }

        template <typename T>
        void operator()([[maybe_unused]] T& list) {
            throw std::runtime_error("Unsupported types for shuffle");
        }
    };

    bool getSpecificationImpl(std::vector<GameEnvironment::Value*> values) const override {
        return values.size() == 1;
    }

    void evaluateImpl(std::vector<GameEnvironment::Value*> values) override {
        std::visit(ShuffleListVisitor{}, values[0]->value);
    }
};

// Extends the list by appending items from the second list to the first one
class ExtendListOperation final : public ModifyOperation {
private:
    struct ExtendListVisitor {
        void operator()(std::unique_ptr<GameEnvironment::List>& targetList, 
                        const std::unique_ptr<GameEnvironment::List>& extendList) {
            // Append the contents of extendList to targetList
            targetList->insert(targetList->end(), 
                               std::make_move_iterator(extendList->begin()), 
                               std::make_move_iterator(extendList->end()));
        }

        template <typename T, typename U>
        void operator()([[maybe_unused]] T& targetList, [[maybe_unused]] const U& extendList) {
            throw std::runtime_error("Unsupported types for extend");
        }
    };

    bool getSpecificationImpl(std::vector<GameEnvironment::Value*> values) const override {
        // Ensure there are exactly two arguments and both are lists
        return values.size() == 2 &&
               std::holds_alternative<std::unique_ptr<GameEnvironment::List>>(values[0]->value) &&
               std::holds_alternative<std::unique_ptr<GameEnvironment::List>>(values[1]->value);
    }

    void evaluateImpl(std::vector<GameEnvironment::Value*> values) override {
        std::visit(ExtendListVisitor{}, values[0]->value, values[1]->value);
    }
};

// Register modifying list operations to the map
void 
Evaluator::registerOperation(MODIFIER MODIFIEREnum, std::unique_ptr<ModifyOperation> ModifyOperation){
    auto [it, succeeded] = MODIFIERToModifyOperation.try_emplace(MODIFIEREnum, std::move(ModifyOperation));
    
    // When the given operation is already registered
    if (!succeeded){
        std::runtime_error("A similar list modifying operation already exists.");
    }
}

void
Evaluator::evaluate(MODIFIER MODIFIEREnum, std::vector<GameEnvironment::Value*> values){
    auto operationItr = MODIFIERToModifyOperation.find(MODIFIEREnum);
    // No operation registered
    if (operationItr == MODIFIERToModifyOperation.end()){
        std::runtime_error("The list modifying operator is not registered");
    }
    operationItr->second->evaluate(values);
}



//--------------------------------------------------------------- DEFAULT FACTORY IMPLEMENTATIONS --------------------------------- 
// Make a default evaluator
Evaluator Evaluator::defaultEvaluatorFactory(){
    Evaluator evaluator;
    evaluator.registerOperation(OPERATOR::ADD,          std::make_unique<AddOperation>());
    evaluator.registerOperation(OPERATOR::SUBTRACT,     std::make_unique<SubtractOperation>());
    evaluator.registerOperation(OPERATOR::MULTIPLY,     std::make_unique<MultiplyOperation>());
    evaluator.registerOperation(OPERATOR::DIVIDE,       std::make_unique<DivideOperation>());
    evaluator.registerOperation(OPERATOR::OR,           std::make_unique<OrOperation>());
    evaluator.registerOperation(OPERATOR::AND,          std::make_unique<AndOperation>());
    evaluator.registerOperation(OPERATOR::NOT,          std::make_unique<NotOperation>());
    evaluator.registerOperation(OPERATOR::EQUAL,        std::make_unique<EqualOperation>());
    evaluator.registerOperation(MODIFIER::REVERSE,  std::make_unique<ReverseListOperation>());
    evaluator.registerOperation(MODIFIER::SHUFFLE,  std::make_unique<ShuffleListOperation>());
    evaluator.registerOperation(MODIFIER::EXTEND,   std::make_unique<ExtendListOperation>());
    return evaluator;
}

