#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <map>
#include <memory>
#include <GameEnvironment.h>
#include <stdexcept>

// Allowed operations by evaluator
enum class OPERATOR {
    ADD, SUBTRACT, MULTIPLY, DIVIDE, OR, AND, NOT, EQUAL, SIZE, CONTAIN, COLLECT
};

// Allowed operations for modifying lists by evaluator
enum class MODIFIER {
    EXTEND, REVERSE, SHUFFLE
};

// An interface of Operation
class Operation {
public:
    // Evaluates given arguments to GameEnvironment::Value
    GameEnvironment::Value evaluate(std::vector<const GameEnvironment::Value*> values) const {
        // Operation specification not meet
        if (!getSpecification(values)){
            throw std::runtime_error("The operation specification is not met");
        }
        return evaluateImpl(values);
    }
    // Check given arguments meets specification of Operation
    bool getSpecification(std::vector<const GameEnvironment::Value*> values) const {
        return getSpecificationImpl(values);
    }
private:
    virtual GameEnvironment::Value evaluateImpl(std::vector<const GameEnvironment::Value*> values) const = 0;
    virtual bool getSpecificationImpl(std::vector<const GameEnvironment::Value*> values) const = 0;
};

// An interface of Operation
class ModifyOperation {
public:
    // Evaluates given arguments to GameEnvironment::Value
    void evaluate(std::vector<GameEnvironment::Value*> values) {
        // Operation specification not meet
        if (!getSpecification(values)){
            throw std::runtime_error("The operation specification is not met");
        }
        evaluateImpl(values);
    }
    // Check given arguments meets specification of Operation
    bool getSpecification(std::vector<GameEnvironment::Value*> values) const {
        return getSpecificationImpl(values);
    }
private:
    virtual void evaluateImpl(std::vector<GameEnvironment::Value*> values) = 0;
    virtual bool getSpecificationImpl(std::vector<GameEnvironment::Value*> values) const = 0;
};

class Evaluator{
public:
    // Evaluate given arguments with given operator
    GameEnvironment::Value evaluate(OPERATOR operationEnum, std::vector<const GameEnvironment::Value*> values);
    // Modify lists based on given action
    void evaluate(MODIFIER MODIFIEREnum, std::vector<GameEnvironment::Value*> values);
    // Add an associate from operator to operation
    void registerOperation(OPERATOR operatorEnum, std::unique_ptr<Operation> operation);
    void registerOperation(MODIFIER MODIFIEREnum, std::unique_ptr<ModifyOperation> ModifyOperation);
    // Default factory method
    static Evaluator defaultEvaluatorFactory();
private:
    // Map that can associate from operator to operation 
    std::map<OPERATOR, std::unique_ptr<Operation>> operatorToOperation; 
    std::map<MODIFIER, std::unique_ptr<ModifyOperation>> MODIFIERToModifyOperation;
};

#endif
