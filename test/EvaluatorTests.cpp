#include <gtest/gtest.h>
#include "Evaluator.h"

class EvaluatorTest : public ::testing::Test {
protected:
    Evaluator evaluator;

    // Use a default evaluator
    void SetUp() override {
        evaluator = Evaluator::defaultEvaluatorFactory();
    }
};

// Test case for ADD operation
TEST_F(EvaluatorTest, TestValidAddOperation) {
    // Positive numbers
    GameEnvironment::Value intValue1, intValue2;
    intValue1.value = 5;
    intValue2.value = 7;
    EXPECT_EQ(std::get<int>(evaluator.evaluate(OPERATOR::ADD, {&intValue1, &intValue2}).value), 12);

    // Different sign numbers
    intValue1.value = 5;
    intValue2.value = -3;
    EXPECT_EQ(std::get<int>(evaluator.evaluate(OPERATOR::ADD, {&intValue1, &intValue2}).value), 2);

    // Two negative numbers
    intValue1.value = -5;
    intValue2.value = -3;
    EXPECT_EQ(std::get<int>(evaluator.evaluate(OPERATOR::ADD, {&intValue1, &intValue2}).value), -8);

    // Adding two empty strings
    GameEnvironment::Value emptyStringValue;
    emptyStringValue.value = std::string_view("");
    EXPECT_EQ(std::get<std::string_view>(evaluator.evaluate(OPERATOR::ADD, {&emptyStringValue, &emptyStringValue}).value), "");

    // Adding an empty string and a non-empty string
    GameEnvironment::Value nonEmptyStringValue;
    nonEmptyStringValue.value = std::string_view("Hello");
    EXPECT_EQ(std::get<std::string_view>(evaluator.evaluate(OPERATOR::ADD, {&emptyStringValue, &nonEmptyStringValue}).value), "Hello");

    // Adding a non-empty string and an empty string
    EXPECT_EQ(std::get<std::string_view>(evaluator.evaluate(OPERATOR::ADD, {&emptyStringValue, &nonEmptyStringValue}).value), "Hello");
}

// Test case for invalid ADD operations
TEST_F(EvaluatorTest, TestInvalidAddOperations) {
    // Only 1 argument for integer
    GameEnvironment::Value intValue;
    intValue.value = 5;
    EXPECT_THROW(evaluator.evaluate(OPERATOR::ADD, {&intValue}), std::runtime_error);

    // Only 1 argument for string
    GameEnvironment::Value stringValue;
    stringValue.value = std::string_view("Hello");
    EXPECT_THROW(evaluator.evaluate(OPERATOR::ADD, {&stringValue}), std::runtime_error);

    // Try to add 3 arguments for integers
    EXPECT_THROW(evaluator.evaluate(OPERATOR::ADD, {&intValue, &intValue, &intValue}), std::runtime_error);

    // Try to add 3 arguments for string
    EXPECT_THROW(evaluator.evaluate(OPERATOR::ADD, {&stringValue, &stringValue, &stringValue}), std::runtime_error);

    // Try to add two booleans
    GameEnvironment::Value boolValue1, boolValue2;
    boolValue1.value = true;
    boolValue2.value = false;
    EXPECT_THROW(evaluator.evaluate(OPERATOR::ADD, {&boolValue1, &boolValue2}), std::runtime_error);

    // Try to add two vector pointers
    GameEnvironment::Value vectorValue;
    vectorValue.value = std::make_unique<GameEnvironment::List>();
    EXPECT_THROW(evaluator.evaluate(OPERATOR::ADD, {&vectorValue, &vectorValue}), std::runtime_error);

    // Try to add two Map pointers
    GameEnvironment::Value mapValue;
    mapValue.value = std::make_unique<GameEnvironment::Map>();
    EXPECT_THROW(evaluator.evaluate(OPERATOR::ADD, {&mapValue, &mapValue}), std::runtime_error);
}

// Test case for SUBTRACT operation
TEST_F(EvaluatorTest, TestValidSubtractOperation) {
    // Positive numbers
    GameEnvironment::Value intValue1, intValue2;
    intValue1.value = 5;
    intValue2.value = 7;
    EXPECT_EQ(std::get<int>(evaluator.evaluate(OPERATOR::SUBTRACT, {&intValue1, &intValue2}).value), -2);

    // Different sign numbers
    intValue1.value = 5;
    intValue2.value = -3;
    EXPECT_EQ(std::get<int>(evaluator.evaluate(OPERATOR::SUBTRACT, {&intValue1, &intValue2}).value), 8);

    // Two negative numbers
    intValue1.value = -5;
    intValue2.value = -3;
    EXPECT_EQ(std::get<int>(evaluator.evaluate(OPERATOR::SUBTRACT, {&intValue1, &intValue2}).value), -2);

    // Two same number
    intValue1.value = 5;
    intValue2.value = 5;
    EXPECT_EQ(std::get<int>(evaluator.evaluate(OPERATOR::SUBTRACT, {&intValue1, &intValue2}).value), 0);
}

// Test case for invalid SUBTRACT operations
TEST_F(EvaluatorTest, TestInvalidSubtractOperations) {
    // Try to subtract only 1 argument for integer
    GameEnvironment::Value intValue;
    intValue.value = 5;
    EXPECT_THROW(evaluator.evaluate(OPERATOR::SUBTRACT, {&intValue}), std::runtime_error);

    // Try to subtract 3 arguments for integers
    EXPECT_THROW(evaluator.evaluate(OPERATOR::SUBTRACT, {&intValue, &intValue, &intValue}), std::runtime_error);

    // Try to subtract integers with string
    GameEnvironment::Value stringValue;
    stringValue.value = std::string_view("Hello");
    EXPECT_THROW(evaluator.evaluate(OPERATOR::SUBTRACT, {&intValue, &stringValue}), std::runtime_error);

    // Try to subtract two booleans
    GameEnvironment::Value boolValue1, boolValue2;
    boolValue1.value = true;
    boolValue2.value = false;
    EXPECT_THROW(evaluator.evaluate(OPERATOR::SUBTRACT, {&boolValue1, &boolValue2}), std::runtime_error);

    // Try to subtract two vector pointers
    GameEnvironment::Value vectorValue;
    vectorValue.value = std::make_unique<GameEnvironment::List>();
    EXPECT_THROW(evaluator.evaluate(OPERATOR::SUBTRACT, {&vectorValue, &vectorValue}), std::runtime_error);

    // Try to subtract two Map pointers
    GameEnvironment::Value mapValue;
    mapValue.value = std::make_unique<GameEnvironment::Map>();
    EXPECT_THROW(evaluator.evaluate(OPERATOR::SUBTRACT, {&mapValue, &mapValue}), std::runtime_error);
}


// Test case for MULTIPLY operation
TEST_F(EvaluatorTest, TestValidMultiplyOperation) {
    // Positive numbers
    GameEnvironment::Value intValue1, intValue2;
    intValue1.value = 5;
    intValue2.value = 7;
    EXPECT_EQ(std::get<int>(evaluator.evaluate(OPERATOR::MULTIPLY, {&intValue1, &intValue2}).value), 35);

    // Different sign numbers
    intValue1.value = 5;
    intValue2.value = -3;
    EXPECT_EQ(std::get<int>(evaluator.evaluate(OPERATOR::MULTIPLY, {&intValue1, &intValue2}).value), -15);

    // Two negative numbers
    intValue1.value = -5;
    intValue2.value = -3;
    EXPECT_EQ(std::get<int>(evaluator.evaluate(OPERATOR::MULTIPLY, {&intValue1, &intValue2}).value), 15);

    // One number is 0
    intValue1.value = 0;
    intValue2.value = 5;
    EXPECT_EQ(std::get<int>(evaluator.evaluate(OPERATOR::MULTIPLY, {&intValue1, &intValue2}).value), 0);

    // One number is 1
    intValue1.value = 1;
    intValue2.value = 5;
    EXPECT_EQ(std::get<int>(evaluator.evaluate(OPERATOR::MULTIPLY, {&intValue1, &intValue2}).value), 5);
}

// Test case for invalid MULTIPLY operations
TEST_F(EvaluatorTest, TestInvalidMultiplyOperations) {
    // Try to multiply only 1 argument for integer
    GameEnvironment::Value intValue;
    intValue.value = 5;
    EXPECT_THROW(evaluator.evaluate(OPERATOR::MULTIPLY, {&intValue}), std::runtime_error);

    // Try to multiply 3 arguments for integers
    EXPECT_THROW(evaluator.evaluate(OPERATOR::MULTIPLY, {&intValue, &intValue, &intValue}), std::runtime_error);

    // Try to multiply integers with string
    GameEnvironment::Value stringValue;
    stringValue.value = std::string_view("Hello");
    EXPECT_THROW(evaluator.evaluate(OPERATOR::MULTIPLY, {&intValue, &stringValue}), std::runtime_error);

    // Try to multiply two booleans
    GameEnvironment::Value boolValue1, boolValue2;
    boolValue1.value = true;
    boolValue2.value = false;
    EXPECT_THROW(evaluator.evaluate(OPERATOR::MULTIPLY, {&boolValue1, &boolValue2}), std::runtime_error);

    // Try to multiply two vector pointers
    GameEnvironment::Value vectorValue;
    vectorValue.value = std::make_unique<GameEnvironment::List>();
    EXPECT_THROW(evaluator.evaluate(OPERATOR::MULTIPLY, {&vectorValue, &vectorValue}), std::runtime_error);

    // Try to multiply two Map pointers
    GameEnvironment::Value mapValue;
    mapValue.value = std::make_unique<GameEnvironment::Map>();
    EXPECT_THROW(evaluator.evaluate(OPERATOR::MULTIPLY, {&mapValue, &mapValue}), std::runtime_error);
}

// Test case for DIVIDE operation
TEST_F(EvaluatorTest, TestValidDivideOperation) {
    // Positive numbers
    GameEnvironment::Value intValue1, intValue2;
    intValue1.value = 10;
    intValue2.value = 2;
    EXPECT_EQ(std::get<int>(evaluator.evaluate(OPERATOR::DIVIDE, {&intValue1, &intValue2}).value), 5);

    // Different sign numbers
    intValue1.value = 5;
    intValue2.value = -1;
    EXPECT_EQ(std::get<int>(evaluator.evaluate(OPERATOR::DIVIDE, {&intValue1, &intValue2}).value), -5);

    // Two negative numbers
    intValue1.value = -15;
    intValue2.value = -3;
    EXPECT_EQ(std::get<int>(evaluator.evaluate(OPERATOR::DIVIDE, {&intValue1, &intValue2}).value), 5);

    // Left operand is 0
    intValue1.value = 0;
    intValue2.value = 5;
    EXPECT_EQ(std::get<int>(evaluator.evaluate(OPERATOR::DIVIDE, {&intValue1, &intValue2}).value), 0);

    // Right operand is 1
    intValue1.value = 5;
    intValue2.value = 1;
    EXPECT_EQ(std::get<int>(evaluator.evaluate(OPERATOR::DIVIDE, {&intValue1, &intValue2}).value), 5);

    // Check result becomes integer
    intValue1.value = 5;
    intValue2.value = 2;
    EXPECT_EQ(std::get<int>(evaluator.evaluate(OPERATOR::DIVIDE, {&intValue1, &intValue2}).value), 2);
}

// Test case for invalid DIVIDE operations
TEST_F(EvaluatorTest, TestInvalidDivideOperations) {
    // Try to divide only 1 argument for integer
    GameEnvironment::Value intValue1;
    intValue1.value = 5;
    EXPECT_THROW(evaluator.evaluate(OPERATOR::DIVIDE, {&intValue1}), std::runtime_error);

    // Try to divide 3 arguments for integers
    EXPECT_THROW(evaluator.evaluate(OPERATOR::DIVIDE, {&intValue1, &intValue1, &intValue1}), std::runtime_error);

    // Try to divide by 0
    GameEnvironment::Value intValue2;
    intValue2.value = 0;
    EXPECT_THROW(evaluator.evaluate(OPERATOR::DIVIDE, {&intValue1, &intValue2}), std::runtime_error);

    // Try to divide integers with string
    GameEnvironment::Value stringValue;
    stringValue.value = std::string_view("Hello");
    EXPECT_THROW(evaluator.evaluate(OPERATOR::DIVIDE, {&intValue1, &stringValue}), std::runtime_error);

    // Try to divide two booleans
    GameEnvironment::Value boolValue1, boolValue2;
    boolValue1.value = true;
    boolValue2.value = false;
    EXPECT_THROW(evaluator.evaluate(OPERATOR::DIVIDE, {&boolValue1, &boolValue2}), std::runtime_error);

    // Try to divide two vector pointers
    GameEnvironment::Value vectorValue;
    vectorValue.value = std::make_unique<GameEnvironment::List>();
    EXPECT_THROW(evaluator.evaluate(OPERATOR::DIVIDE, {&vectorValue, &vectorValue}), std::runtime_error);

    // Try to divide two Map pointers
    GameEnvironment::Value mapValue;
    mapValue.value = std::make_unique<GameEnvironment::Map>();
    EXPECT_THROW(evaluator.evaluate(OPERATOR::DIVIDE, {&mapValue, &mapValue}), std::runtime_error);
}