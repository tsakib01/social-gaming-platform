#include "Evaluator.h"
#include <stdexcept>
#include <iostream>
#include <stdexcept>

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
            value.value = std::string_view(std::string(left) + std::string(right));
            return value;
        }

        template <typename T, typename U>
        GameEnvironment::Value operator()([[maybe_unused]] const T& left, [[maybe_unused]] const U& right){
            throw std::runtime_error("Unsupported types for addition");
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
            throw std::runtime_error("Unsupported types for addition");
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
            throw std::runtime_error("Unsupported types for addition");
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
    
    // Operation specification not meed
    if (!operationItr->second->getSpecification(values)){
        std::runtime_error("The operation specification is not met");
    }

    return operationItr->second->evaluate(values);
}

// Make a default evaluator
Evaluator Evaluator::defaultEvaluatorFactory(){
    Evaluator evaluator;
    evaluator.registerOperation(OPERATOR::ADD, std::make_unique<AddOperation>());
    evaluator.registerOperation(OPERATOR::SUBTRACT, std::make_unique<SubtractOperation>());
    evaluator.registerOperation(OPERATOR::MULTIPLY, std::make_unique<MultiplyOperation>());
    evaluator.registerOperation(OPERATOR::DIVIDE, std::make_unique<DivideOperation>());
    return evaluator;
}