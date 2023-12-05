#include "Evaluator.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <random>

//--------------------------------------------------------------- OPERATION CLASS IMPLEMENTATIONS --------------------------------- 

// Add operation supports
// 1. Addition of two integers
class AddOperation final : public Operation {
private:
    struct AddVisitor {
        GameEnvironment::Value operator()(const int& left, const int& right){
            GameEnvironment::Value value;
            value.value = left + right;
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

class UpFromOperation : public Operation {
private:
    struct UpFromOperationVisitor {
        GameEnvironment::Value operator()(const int& count, const int& upfrom){
            std::unique_ptr<GameEnvironment::List> list = std::make_unique<GameEnvironment::List>();
            list->reserve(count - upfrom);
            for (int i = upfrom; i <= count; i++) {
                list->emplace_back(std::make_unique<GameEnvironment::Value>(i));
            }
            return GameEnvironment::Value(std::move(list));
        }

        template <typename T, typename U>
        GameEnvironment::Value operator()([[maybe_unused]] const T& count, [[maybe_unused]] const U& upfrom){
            throw std::runtime_error("Unsupported types for UpFrom");
        }
    };

    GameEnvironment::Value evaluateImpl(std::vector<const GameEnvironment::Value*> values) const override {
        return std::visit(UpFromOperationVisitor{}, values[0]->value, values[1]->value);
    }
    bool getSpecificationImpl(std::vector<const GameEnvironment::Value*> values) const override {
        return values.size() == 2;
    }
};

// SIZE operation supports
// Return size of operand which should be list
class SizeOperation final : public Operation {
private:
    struct SizeOperationVisitor {
        GameEnvironment::Value operator()(const std::unique_ptr<GameEnvironment::List>& list){
            int listSize = list->size();
            return GameEnvironment::Value(listSize);
        }

        template <typename T>
        GameEnvironment::Value operator()([[maybe_unused]] const T& op){
            throw std::runtime_error("Unsupported types for SIZE");
        }
    };

    // SIZE operation requires only 1 argument
    bool getSpecificationImpl(std::vector<const GameEnvironment::Value*> values) const override {
        return values.size() == 1;
    };

    GameEnvironment::Value evaluateImpl(std::vector<const GameEnvironment::Value*> values) const override{
        return std::visit(SizeOperationVisitor{}, values[0]->value);
    } 
};

// CONTAIN operation supports
// If it is a list, return whether the given value is in the list or not
// TODO -> Support Map
class ContainOperation final : public Operation {
public:
    ContainOperation(Evaluator& evaluator)
    : evaluator(evaluator)
    {}
private:
    struct ContainOperationVisitor {
        GameEnvironment::Value operator()(const std::unique_ptr<GameEnvironment::List>& list){
            auto result = GameEnvironment::Value(true);
            
            // Check any of the element in the list is equal to target
            for (const auto& value : *list){
                auto isEqual = evaluator.evaluate(OPERATOR::EQUAL, {value.get(), target});
                result = evaluator.evaluate(OPERATOR::AND, {&result, &isEqual});
            }
            return result;
        }

        template <typename T>
        GameEnvironment::Value operator()([[maybe_unused]] const T& value){
            throw std::runtime_error("Unsupported types for CONTAIN");
        }
        
        Evaluator& evaluator;
        const GameEnvironment::Value* target;
    };

    // CONTAIN operation requires 2 arguments
    bool getSpecificationImpl(std::vector<const GameEnvironment::Value*> values) const override {
        return values.size() == 2;
    };

    GameEnvironment::Value evaluateImpl(std::vector<const GameEnvironment::Value*> values) const override{
        return std::visit(ContainOperationVisitor{evaluator, values[1]}, values[0]->value);
    }

    Evaluator& evaluator;
};

// COLLECT operation supports
// Return all elements in the list such that they are equal to given value
// TODO -> Support Map
class CollectOperation final : public Operation {
public:
    CollectOperation(Evaluator& evaluator)
    : evaluator(evaluator)
    {}
private:
    // This can be moved to Modify Operation
    // If the given value is true, push_back the target into list
    // If the given value is false, it does not do anything
    struct FilterVisiter{
        void operator()(bool value){
            if (!value){
                return;
            }
            list.push_back(std::make_unique<GameEnvironment::Value>(*target));
        }

        template <typename T>
        void operator()([[maybe_unused]] const T& value){
            throw std::runtime_error("Filter Visitor expects bool");
        }

        std::vector<std::unique_ptr<GameEnvironment::Value>>& list;
        const GameEnvironment::Value* target;
    };

    struct CollectOperationVisitor {
        CollectOperationVisitor(Evaluator& evaluator, const GameEnvironment::Value* target)
        : evaluator(evaluator), target(target)
        {}

        GameEnvironment::Value operator()(const std::unique_ptr<GameEnvironment::List>& list){
            auto result = std::make_unique<GameEnvironment::List>();

            for (const auto& value : *list){
                // Evaluate the given target and list are equal
                auto isEqual = evaluator.evaluate(OPERATOR::EQUAL, {value.get(), target});
                std::visit(FilterVisiter{*result, target}, isEqual.value);
            }

            return GameEnvironment::Value(std::move(result));
        }

        template <typename T>
        GameEnvironment::Value operator()([[maybe_unused]] const T& value){
            throw std::runtime_error("Unsupported types for COLLECT");
        }
        
        Evaluator& evaluator;
        const GameEnvironment::Value* target;
    };

    // COLLECT operation requires 2 arguments
    bool getSpecificationImpl(std::vector<const GameEnvironment::Value*> values) const override {
        return values.size() == 2;
    };

    GameEnvironment::Value evaluateImpl(std::vector<const GameEnvironment::Value*> values) const override{
        return std::visit(CollectOperationVisitor{evaluator, values[1]}, values[0]->value);
    }

    Evaluator& evaluator;
};

// Register operation to the map
void Evaluator::registerOperation(OPERATOR operatorEnum, std::unique_ptr<Operation> operation){
    auto [it, succeeded] = operatorToOperation.try_emplace(operatorEnum, std::move(operation));
    
    // When the given operation is already registered
    if (!succeeded){
        std::runtime_error("You cannot add two operations to same operator.");
    }
}

GameEnvironment::Value Evaluator::evaluate(OPERATOR operationEnum, std::vector<const GameEnvironment::Value*> values) const {
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

            if (extendList) {
                // Iterate over the extendList
                for (const auto& item : *extendList) {
                    // Make a deep copy of the item
                    std::unique_ptr<GameEnvironment::Value> copiedItem;
                    switch (item->value.index()) {
                        case 0:
                            copiedItem = std::move(getValue(item));
                            break;
                        case 1:
                            copiedItem = std::move(getValue(item));
                            break;
                        case 2:
                            copiedItem = std::move(getValue(item));
                            break;
                        case 3: { // Works for List of Maps of Integers/Strings/Booleans
                            std::unique_ptr<GameEnvironment::Map> copiedMap = std::make_unique<GameEnvironment::Map>();
                            auto& map = std::get<3>(item->value);
                            for(auto it = map->begin(); it != map->end(); ++it) {
                                auto& v = it->second;
                                std::unique_ptr<GameEnvironment::Value> mapItemValue = getValue(v);
                                std::pair<std::string_view, std::unique_ptr<GameEnvironment::Value>> keyValuePair = std::make_pair(it->first, std::move(mapItemValue));
                                copiedMap->insert(std::move(keyValuePair));
                            }
                            copiedItem = std::move(std::make_unique<GameEnvironment::Value>(std::move(copiedMap)));
                            break;
                        }
                        case 4: { // Works for List of List of Integers/Strings/Booleans
                            std::unique_ptr<GameEnvironment::List> copiedList = std::make_unique<GameEnvironment::List>();
                            auto& list = std::get<4>(item->value);
                            for (const auto& listItem : *list) {
                                copiedList->push_back(std::move(getValue(listItem)));
                            }
                            copiedItem = std::move(std::make_unique<GameEnvironment::Value>(std::move(copiedList)));
                            break;
                        }
                    }

                    targetList->push_back(std::move(copiedItem));

                }
            }
        }

        std::unique_ptr<GameEnvironment::Value> 
        getValue(const std::unique_ptr<GameEnvironment::Value>& v) {
            std::unique_ptr<GameEnvironment::Value> value;
            if (std::holds_alternative<int>(v->value)) value = std::make_unique<GameEnvironment::Value>(std::get<int>(v->value));
            if (std::holds_alternative<bool>(v->value)) value = std::make_unique<GameEnvironment::Value>(std::get<bool>(v->value));
            if (std::holds_alternative<std::string_view>(v->value)) value = std::make_unique<GameEnvironment::Value>(std::get<std::string_view>(v->value));
            return value;
        }

        template <typename T, typename U>
        void operator()([[maybe_unused]] T& targetList, [[maybe_unused]] const U& extendList) {
            throw std::runtime_error("Unsupported types for extend");
        }
    };

    bool getSpecificationImpl(std::vector<GameEnvironment::Value*> values) const override {
        // Ensure there are exactly two arguments and both are lists
        return values.size() == 2;
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
Evaluator::evaluate(MODIFIER MODIFIEREnum, std::vector<GameEnvironment::Value*> values) {
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
    evaluator.registerOperation(OPERATOR::SIZE,         std::make_unique<SizeOperation>());
    evaluator.registerOperation(OPERATOR::CONTAIN,         std::make_unique<ContainOperation>(evaluator));
    evaluator.registerOperation(OPERATOR::UPFROM,        std::make_unique<UpFromOperation>());
    evaluator.registerOperation(MODIFIER::REVERSE,  std::make_unique<ReverseListOperation>());
    evaluator.registerOperation(MODIFIER::SHUFFLE,  std::make_unique<ShuffleListOperation>());
    evaluator.registerOperation(MODIFIER::EXTEND,   std::make_unique<ExtendListOperation>());
    return evaluator;
}

