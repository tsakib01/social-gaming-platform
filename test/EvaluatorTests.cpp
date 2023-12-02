#include <gtest/gtest.h>
#include "Evaluator.h"
#include <iostream>

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

// Test case for OR operation
TEST_F(EvaluatorTest, TestValidOrOperation) {
    // Two true values
    GameEnvironment::Value trueValue1, trueValue2;
    trueValue1.value = true;
    trueValue2.value = true;
    EXPECT_EQ(std::get<bool>(evaluator.evaluate(OPERATOR::OR, {&trueValue1, &trueValue2}).value), true);

    // One true and one false value
    GameEnvironment::Value falseValue;
    falseValue.value = false;
    EXPECT_EQ(std::get<bool>(evaluator.evaluate(OPERATOR::OR, {&trueValue1, &falseValue}).value), true);

    // Two false values
    GameEnvironment::Value falseValue2;
    falseValue2.value = false;
    EXPECT_EQ(std::get<bool>(evaluator.evaluate(OPERATOR::OR, {&falseValue, &falseValue2}).value), false);
}

// Test case for invalid OR operations
TEST_F(EvaluatorTest, TestInvalidOrOperations) {
    // Only 1 argument for boolean
    GameEnvironment::Value boolValue;
    boolValue.value = true;
    EXPECT_THROW(evaluator.evaluate(OPERATOR::OR, {&boolValue}), std::runtime_error);

    // Try to OR 3 arguments for booleans
    EXPECT_THROW(evaluator.evaluate(OPERATOR::OR, {&boolValue, &boolValue, &boolValue}), std::runtime_error);

    // Try to OR two integers
    GameEnvironment::Value intValue1, intValue2;
    intValue1.value = 5;
    intValue2.value = 7;
    EXPECT_THROW(evaluator.evaluate(OPERATOR::OR, {&intValue1, &intValue2}), std::runtime_error);

    // Try to OR two strings
    GameEnvironment::Value stringValue1, stringValue2;
    stringValue1.value = std::string_view("Hello");
    stringValue2.value = std::string_view("World");
    EXPECT_THROW(evaluator.evaluate(OPERATOR::OR, {&stringValue1, &stringValue2}), std::runtime_error);

    // Try to OR two vector pointers
    GameEnvironment::Value vectorValue;
    vectorValue.value = std::make_unique<GameEnvironment::List>();
    EXPECT_THROW(evaluator.evaluate(OPERATOR::OR, {&vectorValue, &vectorValue}), std::runtime_error);

    // Try to OR two Map pointers
    GameEnvironment::Value mapValue;
    mapValue.value = std::make_unique<GameEnvironment::Map>();
    EXPECT_THROW(evaluator.evaluate(OPERATOR::OR, {&mapValue, &mapValue}), std::runtime_error);
}

// Test case for AND operation
TEST_F(EvaluatorTest, TestValidAndOperation) {
    // Two true values
    GameEnvironment::Value trueValue1, trueValue2;
    trueValue1.value = true;
    trueValue2.value = true;
    EXPECT_EQ(std::get<bool>(evaluator.evaluate(OPERATOR::AND, {&trueValue1, &trueValue2}).value), true);

    // One true and one false value
    GameEnvironment::Value falseValue;
    falseValue.value = false;
    EXPECT_EQ(std::get<bool>(evaluator.evaluate(OPERATOR::AND, {&trueValue1, &falseValue}).value), false);

    // Two false values
    GameEnvironment::Value falseValue2;
    falseValue2.value = false;
    EXPECT_EQ(std::get<bool>(evaluator.evaluate(OPERATOR::AND, {&falseValue, &falseValue2}).value), false);
}

// Test case for invalid AND operations
TEST_F(EvaluatorTest, TestInvalidAndOperations) {
    // Only 1 argument for boolean
    GameEnvironment::Value boolValue;
    boolValue.value = true;
    EXPECT_THROW(evaluator.evaluate(OPERATOR::AND, {&boolValue}), std::runtime_error);

    // Try to AND 3 arguments for booleans
    EXPECT_THROW(evaluator.evaluate(OPERATOR::AND, {&boolValue, &boolValue, &boolValue}), std::runtime_error);

    // Try to AND two integers
    GameEnvironment::Value intValue1, intValue2;
    intValue1.value = 5;
    intValue2.value = 7;
    EXPECT_THROW(evaluator.evaluate(OPERATOR::AND, {&intValue1, &intValue2}), std::runtime_error);

    // Try to AND two strings
    GameEnvironment::Value stringValue1, stringValue2;
    stringValue1.value = std::string_view("Hello");
    stringValue2.value = std::string_view("World");
    EXPECT_THROW(evaluator.evaluate(OPERATOR::AND, {&stringValue1, &stringValue2}), std::runtime_error);

    // Try to AND two vector pointers
    GameEnvironment::Value vectorValue;
    vectorValue.value = std::make_unique<GameEnvironment::List>();
    EXPECT_THROW(evaluator.evaluate(OPERATOR::AND, {&vectorValue, &vectorValue}), std::runtime_error);

    // Try to AND two Map pointers
    GameEnvironment::Value mapValue;
    mapValue.value = std::make_unique<GameEnvironment::Map>();
    EXPECT_THROW(evaluator.evaluate(OPERATOR::AND, {&mapValue, &mapValue}), std::runtime_error);
}

// Test case for NOT operation
TEST_F(EvaluatorTest, TestValidNotOperation) {
    // NOT true
    GameEnvironment::Value trueValue;
    trueValue.value = true;
    EXPECT_EQ(std::get<bool>(evaluator.evaluate(OPERATOR::NOT, {&trueValue}).value), false);

    // NOT false
    GameEnvironment::Value falseValue;
    falseValue.value = false;
    EXPECT_EQ(std::get<bool>(evaluator.evaluate(OPERATOR::NOT, {&falseValue}).value), true);
}

// Test case for invalid NOT operations
TEST_F(EvaluatorTest, TestInvalidNotOperations) {
    // No argument provided
    EXPECT_THROW(evaluator.evaluate(OPERATOR::NOT, {}), std::runtime_error);

    // Two operands
    GameEnvironment::Value boolValue;
    boolValue.value = true;
    EXPECT_THROW(evaluator.evaluate(OPERATOR::NOT, {&boolValue, &boolValue}), std::runtime_error);

    // Try to NOT an integer
    GameEnvironment::Value intValue;
    intValue.value = 42;
    EXPECT_THROW(evaluator.evaluate(OPERATOR::NOT, {&intValue}), std::runtime_error);

    // Try to NOT a string
    GameEnvironment::Value stringValue;
    stringValue.value = std::string_view("Hello");
    EXPECT_THROW(evaluator.evaluate(OPERATOR::NOT, {&stringValue}), std::runtime_error);

    // Try to NOT a vector pointer
    GameEnvironment::Value vectorValue;
    vectorValue.value = std::make_unique<GameEnvironment::List>();
    EXPECT_THROW(evaluator.evaluate(OPERATOR::NOT, {&vectorValue}), std::runtime_error);

    // Try to NOT a Map pointer
    GameEnvironment::Value mapValue;
    mapValue.value = std::make_unique<GameEnvironment::Map>();
    EXPECT_THROW(evaluator.evaluate(OPERATOR::NOT, {&mapValue}), std::runtime_error);
}

// Test case for EQUAL operation - Primitive Types
TEST_F(EvaluatorTest, TestEqualPrimitiveTypes) {
    // Integer equality
    GameEnvironment::Value intValue1(5), intValue2(5);
    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&intValue1, &intValue2}).value));

    // Non-Equal
    intValue2.value = 10;
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&intValue1, &intValue2}).value));

    // Boolean equality
    GameEnvironment::Value boolValue1(true), boolValue2(true);
    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&boolValue1, &boolValue2}).value));

    // Non-equal
    boolValue2.value = false;
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&boolValue1, &boolValue2}).value));

    // String equality
    GameEnvironment::Value strValue1("Hello"), strValue2("Hello");
    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&strValue1, &strValue2}).value));

    // Non-Equal
    strValue2.value = std::string_view("World");
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&strValue1, &strValue2}).value));
}

// Test case for EQUAL operation - List of integers
TEST_F(EvaluatorTest, TestEqualIntList){
    // List of integers
    std::unique_ptr<GameEnvironment::List> intList1 = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> intList2 = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> intList3 = std::make_unique<GameEnvironment::List>();

    // Populating lists with 5 elements
    for (int i = 0; i < 5; i++) {
        intList1->push_back(std::make_unique<GameEnvironment::Value>(i));
        intList2->push_back(std::make_unique<GameEnvironment::Value>(i));
        intList3->push_back(std::make_unique<GameEnvironment::Value>(i + 1));
    }

    GameEnvironment::Value intListValue1(std::move(intList1));
    GameEnvironment::Value intListValue2(std::move(intList2));
    GameEnvironment::Value intListValue3(std::move(intList3));

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&intListValue1, &intListValue2}).value));
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&intListValue1, &intListValue3}).value));
}

// Test case for EQUAL operation - List of bool
TEST_F(EvaluatorTest, TestEqualBoolList){
    // // List of booleans
    std::unique_ptr<GameEnvironment::List> boolList1 = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> boolList2 = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> boolList3 = std::make_unique<GameEnvironment::List>();

    // Populating lists with 5 elements
    for (int i = 0; i < 5; i++) {
        boolList1->push_back(std::make_unique<GameEnvironment::Value>(i % 2 == 0));
        boolList2->push_back(std::make_unique<GameEnvironment::Value>(i % 2 == 0));
        boolList3->push_back(std::make_unique<GameEnvironment::Value>(i % 2 == 1));
    }

    GameEnvironment::Value boolListValue1(std::move(boolList1));
    GameEnvironment::Value boolListValue2(std::move(boolList2));
    GameEnvironment::Value boolListValue3(std::move(boolList3));

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&boolListValue1, &boolListValue2}).value));
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&boolListValue1, &boolListValue3}).value));
}

// Test case for EQUAL operation - List of string
TEST_F(EvaluatorTest, TestEqualStringList){
    // List of strings
    std::unique_ptr<GameEnvironment::List> stringList1 = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> stringList2 = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> stringList3 = std::make_unique<GameEnvironment::List>();

    // Populating lists with 5 elements
    for (int i = 0; i < 5; i++) {
        stringList1->push_back(std::make_unique<GameEnvironment::Value>(std::string_view("Hello")));
        stringList2->push_back(std::make_unique<GameEnvironment::Value>(std::string_view("Hello")));
        stringList3->push_back(std::make_unique<GameEnvironment::Value>(std::string_view("World")));
    }

    GameEnvironment::Value stringListValue1(std::move(stringList1));
    GameEnvironment::Value stringListValue2(std::move(stringList2));
    GameEnvironment::Value stringListValue3(std::move(stringList3));

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&stringListValue1, &stringListValue2}).value));
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&stringListValue1, &stringListValue3}).value));
}

// Test case for EQUAL operation - List of list of integers
TEST_F(EvaluatorTest, TestEqualListOfIntList) {
    // List of list of integers
    std::unique_ptr<GameEnvironment::List> listOfIntList1 = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> listOfIntList2 = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> listOfIntList3 = std::make_unique<GameEnvironment::List>();

    // Populating lists with 5 elements
    for (int i = 0; i < 5; i++) {
        std::unique_ptr<GameEnvironment::List> innerList1 = std::make_unique<GameEnvironment::List>();
        std::unique_ptr<GameEnvironment::List> innerList2 = std::make_unique<GameEnvironment::List>();
        std::unique_ptr<GameEnvironment::List> innerList3 = std::make_unique<GameEnvironment::List>();

        for (int j = 0; j < 5; j++) {
            innerList1->push_back(std::make_unique<GameEnvironment::Value>(j));
            innerList2->push_back(std::make_unique<GameEnvironment::Value>(j));
            innerList3->push_back(std::make_unique<GameEnvironment::Value>(j + 1));
        }

        listOfIntList1->push_back(std::make_unique<GameEnvironment::Value>(std::move(innerList1)));
        listOfIntList2->push_back(std::make_unique<GameEnvironment::Value>(std::move(innerList2)));
        listOfIntList3->push_back(std::make_unique<GameEnvironment::Value>(std::move(innerList3)));
    }

    GameEnvironment::Value listOfIntListValue1(std::move(listOfIntList1));
    GameEnvironment::Value listOfIntListValue2(std::move(listOfIntList2));
    GameEnvironment::Value listOfIntListValue3(std::move(listOfIntList3));

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&listOfIntListValue1, &listOfIntListValue2}).value));
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&listOfIntListValue1, &listOfIntListValue3}).value));
}

// Test case for EQUAL operation - List of Map
TEST_F(EvaluatorTest, TestEqualListOfMap) {
    // List of maps
    std::unique_ptr<GameEnvironment::List> listOfMap1 = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> listOfMap2 = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> listOfMap3 = std::make_unique<GameEnvironment::List>();

    // Populating lists with 5 elements
    for (int i = 0; i < 5; i++) {
        std::unique_ptr<GameEnvironment::Map> innerMap1 = std::make_unique<GameEnvironment::Map>();
        std::unique_ptr<GameEnvironment::Map> innerMap2 = std::make_unique<GameEnvironment::Map>();
        std::unique_ptr<GameEnvironment::Map> innerMap3 = std::make_unique<GameEnvironment::Map>();

        for (int j = 0; j < 5; j++) {
            innerMap1->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(j));
            innerMap2->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(j));
            innerMap3->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(j + 1));
        }

        listOfMap1->push_back(std::make_unique<GameEnvironment::Value>(std::move(innerMap1)));
        listOfMap2->push_back(std::make_unique<GameEnvironment::Value>(std::move(innerMap2)));
        listOfMap3->push_back(std::make_unique<GameEnvironment::Value>(std::move(innerMap3)));
    }

    GameEnvironment::Value listOfMapValue1(std::move(listOfMap1));
    GameEnvironment::Value listOfMapValue2(std::move(listOfMap2));
    GameEnvironment::Value listOfMapValue3(std::move(listOfMap3));

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&listOfMapValue1, &listOfMapValue2}).value));
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&listOfMapValue1, &listOfMapValue3}).value));
}

// Test case for EQUAL operation - Map of integers
TEST_F(EvaluatorTest, TestEqualMapOfInt) {
    // Map of integers
    std::unique_ptr<GameEnvironment::Map> intMap1 = std::make_unique<GameEnvironment::Map>();
    std::unique_ptr<GameEnvironment::Map> intMap2 = std::make_unique<GameEnvironment::Map>();
    std::unique_ptr<GameEnvironment::Map> intMap3 = std::make_unique<GameEnvironment::Map>();

    // Populating lists with 5 elements
    for (int i = 0; i < 5; i++) {
        intMap1->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(i));
        intMap2->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(i));
        intMap3->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(i + 1));
    }

    GameEnvironment::Value intMapValue1(std::move(intMap1));
    GameEnvironment::Value intMapValue2(std::move(intMap2));
    GameEnvironment::Value intMapValue3(std::move(intMap3));

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&intMapValue1, &intMapValue2}).value));
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&intMapValue1, &intMapValue3}).value));
}

// Test case for EQUAL operation - Map of booleans
TEST_F(EvaluatorTest, TestEqualMapOfBool) {
    // Map of booleans
    std::unique_ptr<GameEnvironment::Map> boolMap1 = std::make_unique<GameEnvironment::Map>();
    std::unique_ptr<GameEnvironment::Map> boolMap2 = std::make_unique<GameEnvironment::Map>();
    std::unique_ptr<GameEnvironment::Map> boolMap3 = std::make_unique<GameEnvironment::Map>();

    // Populating lists with 5 elements
    for (int i = 0; i < 5; i++) {
        boolMap1->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(i % 2 == 0));
        boolMap2->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(i % 2 == 0));
        boolMap3->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(i % 2 == 1));
    }

    GameEnvironment::Value boolMapValue1(std::move(boolMap1));
    GameEnvironment::Value boolMapValue2(std::move(boolMap2));
    GameEnvironment::Value boolMapValue3(std::move(boolMap3));

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&boolMapValue1, &boolMapValue2}).value));
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&boolMapValue1, &boolMapValue3}).value));
}

// Test case for EQUAL operation - Map of strings
TEST_F(EvaluatorTest, TestEqualMapOfString) {
    // Map of strings
    std::unique_ptr<GameEnvironment::Map> stringMap1 = std::make_unique<GameEnvironment::Map>();
    std::unique_ptr<GameEnvironment::Map> stringMap2 = std::make_unique<GameEnvironment::Map>();
    std::unique_ptr<GameEnvironment::Map> stringMap3 = std::make_unique<GameEnvironment::Map>();

    // Populating lists with 5 elements
    for (int i = 0; i < 5; i++) {
        stringMap1->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(std::string_view("Hello")));
        stringMap2->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(std::string_view("Hello")));
        stringMap3->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(std::string_view("World")));
    }

    GameEnvironment::Value stringMapValue1(std::move(stringMap1));
    GameEnvironment::Value stringMapValue2(std::move(stringMap2));
    GameEnvironment::Value stringMapValue3(std::move(stringMap3));

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&stringMapValue1, &stringMapValue2}).value));
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&stringMapValue1, &stringMapValue3}).value));
}

// Test case for EQUAL operation - Map of lists
TEST_F(EvaluatorTest, TestEqualMapOfList) {
    // Map of lists
    std::unique_ptr<GameEnvironment::Map> mapOfList1 = std::make_unique<GameEnvironment::Map>();
    std::unique_ptr<GameEnvironment::Map> mapOfList2 = std::make_unique<GameEnvironment::Map>();
    std::unique_ptr<GameEnvironment::Map> mapOfList3 = std::make_unique<GameEnvironment::Map>();

    // Populating lists with 5 elements
    for (int i = 0; i < 5; i++) {
        std::unique_ptr<GameEnvironment::List> list1 = std::make_unique<GameEnvironment::List>();
        std::unique_ptr<GameEnvironment::List> list2 = std::make_unique<GameEnvironment::List>();
        std::unique_ptr<GameEnvironment::List> list3 = std::make_unique<GameEnvironment::List>();

        for (int j = 0; j < 5; j++) {
            list1->push_back(std::make_unique<GameEnvironment::Value>(j));
            list2->push_back(std::make_unique<GameEnvironment::Value>(j));
            list3->push_back(std::make_unique<GameEnvironment::Value>(j + 1));
        }

        mapOfList1->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(std::move(list1)));
        mapOfList2->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(std::move(list2)));
        mapOfList3->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(std::move(list3)));
    }

    GameEnvironment::Value mapOfListValue1(std::move(mapOfList1));
    GameEnvironment::Value mapOfListValue2(std::move(mapOfList2));
    GameEnvironment::Value mapOfListValue3(std::move(mapOfList3));

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&mapOfListValue1, &mapOfListValue2}).value));
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&mapOfListValue1, &mapOfListValue3}).value));
}

// Test case for EQUAL operation - Map of maps
TEST_F(EvaluatorTest, TestEqualMapOfMap) {
    // Map of maps
    std::unique_ptr<GameEnvironment::Map> mapOfMap1 = std::make_unique<GameEnvironment::Map>();
    std::unique_ptr<GameEnvironment::Map> mapOfMap2 = std::make_unique<GameEnvironment::Map>();
    std::unique_ptr<GameEnvironment::Map> mapOfMap3 = std::make_unique<GameEnvironment::Map>();

    // Populating lists with 5 elements
    for (int i = 0; i < 5; i++) {
        std::unique_ptr<GameEnvironment::Map> innerMap1 = std::make_unique<GameEnvironment::Map>();
        std::unique_ptr<GameEnvironment::Map> innerMap2 = std::make_unique<GameEnvironment::Map>();
        std::unique_ptr<GameEnvironment::Map> innerMap3 = std::make_unique<GameEnvironment::Map>();

        for (int j = 0; j < 5; j++) {
            innerMap1->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(i + j));
            innerMap2->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(i + j));
            innerMap3->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>((i + j) + 1));
        }

        mapOfMap1->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(std::move(innerMap1)));
        mapOfMap2->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(std::move(innerMap2)));
        mapOfMap3->emplace(std::string_view("ID"), std::make_unique<GameEnvironment::Value>(std::move(innerMap3)));
    }

    GameEnvironment::Value mapOfMapValue1(std::move(mapOfMap1));
    GameEnvironment::Value mapOfMapValue2(std::move(mapOfMap2));
    GameEnvironment::Value mapOfMapValue3(std::move(mapOfMap3));

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&mapOfMapValue1, &mapOfMapValue2}).value));
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&mapOfMapValue1, &mapOfMapValue3}).value));
}


// -------------------------------------- LIST MODIFYING TESTS ----------------------------------------------------

// Test case for REVERSE list operation - List of integers
TEST_F(EvaluatorTest, TestReverseIntList){
    // List of integers
    std::unique_ptr<GameEnvironment::List> originalIntList = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> expectedIntList = std::make_unique<GameEnvironment::List>();

    // Populating lists with 5 elements
    for (int i = 0; i < 5; i++) {
        originalIntList->push_back(std::make_unique<GameEnvironment::Value>(i));
        expectedIntList->push_back(std::make_unique<GameEnvironment::Value>(5-i-1));
    }

    GameEnvironment::Value originalIntListValue(std::move(originalIntList));
    evaluator.evaluate(MODIFIER::REVERSE, {&originalIntListValue});

    GameEnvironment::Value expectedIntListValue(std::move(expectedIntList));
    GameEnvironment::Value reversedIntListValue(std::move(originalIntListValue));

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&expectedIntListValue, &reversedIntListValue}).value));
}

// Test case for REVERSE list operation - List of strings
TEST_F(EvaluatorTest, TestReverseStringList){
    // List of strings
    std::unique_ptr<GameEnvironment::List> originalStringList = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> expectedStringList = std::make_unique<GameEnvironment::List>();

    // Populating lists with 5 string elements
    std::vector<std::string> stringValues = {"one", "two", "three", "four", "five"};
    for (int i = 0; i < 5; i++) {
        originalStringList->push_back(std::make_unique<GameEnvironment::Value>(stringValues[i]));
        expectedStringList->push_back(std::make_unique<GameEnvironment::Value>(stringValues[4 - i]));
    }

    GameEnvironment::Value originalStringListValue(std::move(originalStringList));
    evaluator.evaluate(MODIFIER::REVERSE, {&originalStringListValue});

    GameEnvironment::Value expectedStringListValue(std::move(expectedStringList));
    GameEnvironment::Value reversedStringListValue(std::move(originalStringListValue));

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&expectedStringListValue, &reversedStringListValue}).value));
}

// Test case for REVERSE list operation - List of booleans
TEST_F(EvaluatorTest, TestReverseListOfBooleans) {
    std::unique_ptr<GameEnvironment::List> originalList = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> expectedList = std::make_unique<GameEnvironment::List>();

    // Populating the list with booleans
    std::vector<bool> boolValues = {true, false, true, false, true};
    for (bool val : boolValues) {
        originalList->push_back(std::make_unique<GameEnvironment::Value>(val));
        expectedList->insert(expectedList->begin(), std::make_unique<GameEnvironment::Value>(val));
    }

    GameEnvironment::Value originalListValue(std::move(originalList));
    evaluator.evaluate(MODIFIER::REVERSE, {&originalListValue});

    GameEnvironment::Value expectedListValue(std::move(expectedList));

    // Verify that the reversed list matches the expected list
    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&expectedListValue, &originalListValue}).value));
}

// Test case for REVERSE list operation - List of lists of integers
TEST_F(EvaluatorTest, TestReverseListOfListOfIntegers) {
    // List of list of integers
    std::unique_ptr<GameEnvironment::List> originalListOfLists = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> expectedListOfLists = std::make_unique<GameEnvironment::List>();

    // Populating the list with lists of integers
    for (int i = 0; i < 5; i++) {
        std::unique_ptr<GameEnvironment::List> innerListOriginal = std::make_unique<GameEnvironment::List>();
        std::unique_ptr<GameEnvironment::List> innerListExpected = std::make_unique<GameEnvironment::List>();
        for (int j = 0; j < 1; j++) {
            innerListOriginal->push_back(std::make_unique<GameEnvironment::Value>(i));
            innerListExpected->push_back(std::make_unique<GameEnvironment::Value>(4-i));
        }
        originalListOfLists->push_back(std::make_unique<GameEnvironment::Value>(std::move(innerListOriginal)));
        expectedListOfLists->push_back(std::make_unique<GameEnvironment::Value>(std::move(innerListExpected)));
    }

    GameEnvironment::Value originalListValue(std::move(originalListOfLists));
    evaluator.evaluate(MODIFIER::REVERSE, {&originalListValue});
    GameEnvironment::Value expectedListValue(std::move(expectedListOfLists));

    // Verify that the reversed list of lists matches the expected list of lists
    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&expectedListValue, &originalListValue}).value));
}

TEST_F(EvaluatorTest, TestReverseListOfMaps) {
    // List of maps
    std::unique_ptr<GameEnvironment::List> originalListOfMaps = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> expectedListOfMaps = std::make_unique<GameEnvironment::List>();

    // Populating the list with maps
    for (int i = 0; i < 5; i++) {
        std::unique_ptr<GameEnvironment::Map> map = std::make_unique<GameEnvironment::Map>();
        (*map)["key"] = std::make_unique<GameEnvironment::Value>(i);
        originalListOfMaps->push_back(std::make_unique<GameEnvironment::Value>(std::move(map)));

        // For expected list, we insert at the beginning to reverse the order
        std::unique_ptr<GameEnvironment::Map> expectedMap = std::make_unique<GameEnvironment::Map>();
        (*expectedMap)["key"] = std::make_unique<GameEnvironment::Value>(i);
        expectedListOfMaps->insert(expectedListOfMaps->begin(), std::make_unique<GameEnvironment::Value>(std::move(expectedMap)));
    }

    GameEnvironment::Value originalListValue(std::move(originalListOfMaps));
    evaluator.evaluate(MODIFIER::REVERSE, {&originalListValue});
    GameEnvironment::Value expectedListValue(std::move(expectedListOfMaps));

    // Verify that the reversed list of maps matches the expected list of maps
    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&expectedListValue, &originalListValue}).value));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------

// Test case for SHUFFLE list operation - List of integers
TEST_F(EvaluatorTest, TestShuffleIntList) {
    // List of integers
    std::unique_ptr<GameEnvironment::List> originalIntList1 = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> originalIntList2 = std::make_unique<GameEnvironment::List>();

    // Populating lists with 5 elements
    for (int i = 0; i < 5; i++) {
        originalIntList1->push_back(std::make_unique<GameEnvironment::Value>(i));
        originalIntList2->push_back(std::make_unique<GameEnvironment::Value>(i));
    }

    GameEnvironment::Value originalIntList1Value(std::move(originalIntList1));
    evaluator.evaluate(MODIFIER::SHUFFLE, {&originalIntList1Value});

    GameEnvironment::Value originalIntListValue(std::move(originalIntList2));
    GameEnvironment::Value shuffledIntListValue(std::move(originalIntList1Value));
    //checks for case when shuffling might return the original list
    while (std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&shuffledIntListValue, &originalIntListValue}).value)) {
        evaluator.evaluate(MODIFIER::SHUFFLE, {&shuffledIntListValue});
    }

    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&originalIntListValue, &shuffledIntListValue}).value));
}

// Test case for SHUFFLE list operation - List of strings
TEST_F(EvaluatorTest, TestShuffleStringList) {
    // List of strings
    std::unique_ptr<GameEnvironment::List> originalStringList1 = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> originalStringList2 = std::make_unique<GameEnvironment::List>();

    // Populating lists with 5 string elements
    std::vector<std::string> stringValues = {"one", "two", "three", "four", "five"};
    for (int i = 0; i < 5; i++) {
        originalStringList1->push_back(std::make_unique<GameEnvironment::Value>(stringValues[i]));
        originalStringList2->push_back(std::make_unique<GameEnvironment::Value>(stringValues[i]));
    }

    GameEnvironment::Value originalStringList1Value(std::move(originalStringList1));
    evaluator.evaluate(MODIFIER::SHUFFLE, {&originalStringList1Value});

    GameEnvironment::Value originalStringListValue(std::move(originalStringList2));
    GameEnvironment::Value shuffledStringListValue(std::move(originalStringList1Value));
    //checks for case when shuffling might return the original list
    while (std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&shuffledStringListValue, &originalStringListValue}).value)) {
        evaluator.evaluate(MODIFIER::SHUFFLE, {&shuffledStringListValue});
    }

    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&originalStringListValue, &shuffledStringListValue}).value));
}

// Test case for SHUFFLE list operation - List of booleans
TEST_F(EvaluatorTest, TestShuffleBooleanList) {
    std::unique_ptr<GameEnvironment::List> originalBoolList1 = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> originalBoolList2 = std::make_unique<GameEnvironment::List>();

    // Populating the list with booleans
    std::vector<bool> boolValues = {true, false, true, false, true};
    for (bool value : boolValues) {
        originalBoolList1->push_back(std::make_unique<GameEnvironment::Value>(value));
        originalBoolList2->push_back(std::make_unique<GameEnvironment::Value>(value));
    }

    GameEnvironment::Value originalBoolList1Value(std::move(originalBoolList1));
    evaluator.evaluate(MODIFIER::SHUFFLE, {&originalBoolList1Value});

    GameEnvironment::Value originalBoolListValue(std::move(originalBoolList2));
    GameEnvironment::Value shuffledBoolListValue(std::move(originalBoolList1Value));
    //checks for case when shuffling might return the original list
    while (std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&shuffledBoolListValue, &originalBoolListValue}).value)) {
        evaluator.evaluate(MODIFIER::SHUFFLE, {&shuffledBoolListValue});
    }

    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&originalBoolListValue, &shuffledBoolListValue}).value));
}

// Test case for SHUFFLE list operation - List of lists of integers
TEST_F(EvaluatorTest, TestShuffleListOfListOfIntegers) {
    // List of list of integers
    std::unique_ptr<GameEnvironment::List> originalListOfListOfIntList1 = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> originalListOfListOfIntList2 = std::make_unique<GameEnvironment::List>();

    // Populating the list with lists of integers
    for (int i = 0; i < 5; i++) {
        std::unique_ptr<GameEnvironment::List> innerListOriginal1 = std::make_unique<GameEnvironment::List>();
        std::unique_ptr<GameEnvironment::List> innerListOriginal2 = std::make_unique<GameEnvironment::List>();
        for (int j = 0; j < 1; j++) {
            innerListOriginal1->push_back(std::make_unique<GameEnvironment::Value>(i));
            innerListOriginal2->push_back(std::make_unique<GameEnvironment::Value>(i));
        }
        originalListOfListOfIntList1->push_back(std::make_unique<GameEnvironment::Value>(std::move(innerListOriginal1)));
        originalListOfListOfIntList2->push_back(std::make_unique<GameEnvironment::Value>(std::move(innerListOriginal2)));
    }

    GameEnvironment::Value shuffledListOfListOfIntValue(std::move(originalListOfListOfIntList1));
    GameEnvironment::Value originalListOfListOfIntValue(std::move(originalListOfListOfIntList2));
    //checks for case when shuffling might return the original list
    while (std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&shuffledListOfListOfIntValue, &originalListOfListOfIntValue}).value)) {
        evaluator.evaluate(MODIFIER::SHUFFLE, {&shuffledListOfListOfIntValue});
    }
    // Verify that the reversed list of lists matches the expected list of lists
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&originalListOfListOfIntValue, &shuffledListOfListOfIntValue}).value));
}

// Test case for SHUFFLE list operation - List of maps
TEST_F(EvaluatorTest, TestShuffleListOfMaps) {
    // List of maps
    std::unique_ptr<GameEnvironment::List> originalListOfMap1 = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> originalListOfMap2 = std::make_unique<GameEnvironment::List>();

    // Populating the list with maps
    for (int i = 0; i < 5; i++) {
        std::unique_ptr<GameEnvironment::Map> map1 = std::make_unique<GameEnvironment::Map>();
        (*map1)["key"] = std::make_unique<GameEnvironment::Value>(i);
        originalListOfMap1->push_back(std::make_unique<GameEnvironment::Value>(std::move(map1)));

        std::unique_ptr<GameEnvironment::Map> map2 = std::make_unique<GameEnvironment::Map>();
        (*map2)["key"] = std::make_unique<GameEnvironment::Value>(i);
        originalListOfMap2->push_back(std::make_unique<GameEnvironment::Value>(std::move(map2)));
    }

    GameEnvironment::Value shuffledListOfMapValue(std::move(originalListOfMap1));
    GameEnvironment::Value originalListOfMapValue(std::move(originalListOfMap2));
    //checks for case when shuffling might return the original list
    while (std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&shuffledListOfMapValue, &originalListOfMapValue}).value)) {
        evaluator.evaluate(MODIFIER::SHUFFLE, {&shuffledListOfMapValue});
    }
    // Verify that the shuffled list of maps is different from the original list of maps
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&shuffledListOfMapValue, &originalListOfMapValue}).value));
}


// ------------------------------------------------------------------------------------------------------------------------------------------------

// Test case for EXTEND list operation - Two List of integers
TEST_F(EvaluatorTest, TestExtendListOfIntegers) {
    std::unique_ptr<GameEnvironment::List> list1 = std::make_unique<GameEnvironment::List>();
    for (int i = 0; i < 5; i++) {
        list1->push_back(std::make_unique<GameEnvironment::Value>(i));
    }

    std::unique_ptr<GameEnvironment::List> list2 = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> list3 = std::make_unique<GameEnvironment::List>();
    for (int i = 5; i < 10; i++) {
        list2->push_back(std::make_unique<GameEnvironment::Value>(i));
        list3->push_back(std::make_unique<GameEnvironment::Value>(i));
    }

    std::unique_ptr<GameEnvironment::List> expectedList = std::make_unique<GameEnvironment::List>();
    for (int i = 0; i < 10; i++) {
        expectedList->push_back(std::make_unique<GameEnvironment::Value>(i));
    }

    GameEnvironment::Value list1Value(std::move(list1));
    GameEnvironment::Value list2Value(std::move(list2));
    GameEnvironment::Value list3Value(std::move(list3));
    GameEnvironment::Value expectedListValue(std::move(expectedList));
    evaluator.evaluate(MODIFIER::EXTEND, {&list1Value, &list2Value});

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&list1Value, &expectedListValue}).value));
    // Verify that list2 remains same after extending
    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&list2Value, &list3Value}).value));
}

// Test case for EXTEND list operation - Two List of strings
TEST_F(EvaluatorTest, TestExtendListOfStrings) {
    std::unique_ptr<GameEnvironment::List> list1 = std::make_unique<GameEnvironment::List>();
    std::vector<std::string> strings1 = {"apple", "banana", "cherry"};
    for (const auto& str : strings1) {
        list1->push_back(std::make_unique<GameEnvironment::Value>(str));
    }

    std::unique_ptr<GameEnvironment::List> list2 = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> list3 = std::make_unique<GameEnvironment::List>();
    std::vector<std::string> strings2 = {"date", "elderberry", "fig"};
    for (const auto& str : strings2) {
        list2->push_back(std::make_unique<GameEnvironment::Value>(str));
        list3->push_back(std::make_unique<GameEnvironment::Value>(str));
    }

    std::unique_ptr<GameEnvironment::List> expectedList = std::make_unique<GameEnvironment::List>();
    for (const auto& str : strings1) {
        expectedList->push_back(std::make_unique<GameEnvironment::Value>(str));
    }
    for (const auto& str : strings2) {
        expectedList->push_back(std::make_unique<GameEnvironment::Value>(str));
    }
    

    GameEnvironment::Value list1Value(std::move(list1));
    GameEnvironment::Value list2Value(std::move(list2));
    GameEnvironment::Value list3Value(std::move(list3));
    GameEnvironment::Value expectedListValue(std::move(expectedList));
    evaluator.evaluate(MODIFIER::EXTEND, {&list1Value, &list2Value});

    // Verify that list1 has been extended correctly
    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&list1Value, &expectedListValue}).value));
    // Verify that list2 remains same after extending
    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&list2Value, &list3Value}).value));
}

// Test case for EXTEND list operation - Two List of boolean
TEST_F(EvaluatorTest, TestExtendListOfBooleans) {
    std::unique_ptr<GameEnvironment::List> list1 = std::make_unique<GameEnvironment::List>();
    std::vector<bool> bools1 = {true, false, true};
    for (bool val : bools1) {
        list1->push_back(std::make_unique<GameEnvironment::Value>(val));
    }

    std::unique_ptr<GameEnvironment::List> list2 = std::make_unique<GameEnvironment::List>();
    std::unique_ptr<GameEnvironment::List> list3 = std::make_unique<GameEnvironment::List>();
    std::vector<bool> bools2 = {false, true, false};
    for (bool val : bools2) {
        list2->push_back(std::make_unique<GameEnvironment::Value>(val));
        list3->push_back(std::make_unique<GameEnvironment::Value>(val));
    }

    std::unique_ptr<GameEnvironment::List> expectedList = std::make_unique<GameEnvironment::List>();
    for (bool val : bools1) {
        expectedList->push_back(std::make_unique<GameEnvironment::Value>(val));
    }
    for (bool val : bools2) {
        expectedList->push_back(std::make_unique<GameEnvironment::Value>(val));
    }

    GameEnvironment::Value list1Value(std::move(list1));
    GameEnvironment::Value list2Value(std::move(list2));
    GameEnvironment::Value list3Value(std::move(list3));
    GameEnvironment::Value expectedListValue(std::move(expectedList));
    evaluator.evaluate(MODIFIER::EXTEND, {&list1Value, &list2Value});

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&list1Value, &expectedListValue}).value));
    // Verify that list2 remains same after extending
    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&list2Value, &list3Value}).value));
}

// Test case for EXTEND list operation - Two List of List of Integers
TEST_F(EvaluatorTest, TestExtendListOfListOfIntegers) {
    std::unique_ptr<GameEnvironment::List> list1 = std::make_unique<GameEnvironment::List>();
    for (int i = 0; i < 3; i++) {
        std::unique_ptr<GameEnvironment::List> innerList = std::make_unique<GameEnvironment::List>();
        for (int j = 0; j < 3; j++) {
            innerList->push_back(std::make_unique<GameEnvironment::Value>(i * 3 + j));
        }
        list1->push_back(std::make_unique<GameEnvironment::Value>(std::move(innerList)));
    }

    std::unique_ptr<GameEnvironment::List> list2 = std::make_unique<GameEnvironment::List>();
    for (int i = 3; i < 5; i++) {
        std::unique_ptr<GameEnvironment::List> innerList = std::make_unique<GameEnvironment::List>();
        for (int j = 0; j < 3; j++) {
            innerList->push_back(std::make_unique<GameEnvironment::Value>(i * 3 + j));
        }
        list2->push_back(std::make_unique<GameEnvironment::Value>(std::move(innerList)));
    }

    std::unique_ptr<GameEnvironment::List> list3 = std::make_unique<GameEnvironment::List>();
    for (int i = 3; i < 5; i++) {
        std::unique_ptr<GameEnvironment::List> innerList = std::make_unique<GameEnvironment::List>();
        for (int j = 0; j < 3; j++) {
            innerList->push_back(std::make_unique<GameEnvironment::Value>(i * 3 + j));
        }
        list3->push_back(std::make_unique<GameEnvironment::Value>(std::move(innerList)));
    }

    std::unique_ptr<GameEnvironment::List> expectedList = std::make_unique<GameEnvironment::List>();
    for (int i = 0; i < 5; i++) {
        std::unique_ptr<GameEnvironment::List> innerList = std::make_unique<GameEnvironment::List>();
        for (int j = 0; j < 3; j++) {
            innerList->push_back(std::make_unique<GameEnvironment::Value>(i * 3 + j));
        }
        expectedList->push_back(std::make_unique<GameEnvironment::Value>(std::move(innerList)));
    }

    GameEnvironment::Value list1Value(std::move(list1));
    GameEnvironment::Value list2Value(std::move(list2));
    GameEnvironment::Value list3Value(std::move(list3));
    GameEnvironment::Value expectedListValue(std::move(expectedList));
    evaluator.evaluate(MODIFIER::EXTEND, {&list1Value, &list2Value});

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&list1Value, &expectedListValue}).value));
    // Verify that list2 remains same after extending
    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&list2Value, &list3Value}).value));
}

// Test case for EXTEND list operation - Two List of Maps
TEST_F(EvaluatorTest, TestExtendListOfMaps) {
    std::unique_ptr<GameEnvironment::List> list1 = std::make_unique<GameEnvironment::List>();
    for (int i = 0; i < 2; i++) {
        std::unique_ptr<GameEnvironment::Map> map = std::make_unique<GameEnvironment::Map>();
        (*map)["key"] = std::make_unique<GameEnvironment::Value>(i);
        list1->push_back(std::make_unique<GameEnvironment::Value>(std::move(map)));
    }

    std::unique_ptr<GameEnvironment::List> list2 = std::make_unique<GameEnvironment::List>();
    for (int i = 2; i < 4; i++) {
        std::unique_ptr<GameEnvironment::Map> map = std::make_unique<GameEnvironment::Map>();
        (*map)["key"] = std::make_unique<GameEnvironment::Value>(i);
        list2->push_back(std::make_unique<GameEnvironment::Value>(std::move(map)));
    }

    std::unique_ptr<GameEnvironment::List> list3 = std::make_unique<GameEnvironment::List>();
    for (int i = 2; i < 4; i++) {
        std::unique_ptr<GameEnvironment::Map> map = std::make_unique<GameEnvironment::Map>();
        (*map)["key"] = std::make_unique<GameEnvironment::Value>(i);
        list3->push_back(std::make_unique<GameEnvironment::Value>(std::move(map)));
    }

    std::unique_ptr<GameEnvironment::List> expectedList = std::make_unique<GameEnvironment::List>();
    for (int i = 0; i < 4; i++) {
        std::unique_ptr<GameEnvironment::Map> map = std::make_unique<GameEnvironment::Map>();
        (*map)["key"] = std::make_unique<GameEnvironment::Value>(i);
        expectedList->push_back(std::make_unique<GameEnvironment::Value>(std::move(map)));
    }

    GameEnvironment::Value list1Value(std::move(list1));
    GameEnvironment::Value list2Value(std::move(list2));
    GameEnvironment::Value list3Value(std::move(list3));
    GameEnvironment::Value expectedListValue(std::move(expectedList));
    evaluator.evaluate(MODIFIER::EXTEND, {&list1Value, &list2Value});

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&list1Value, &expectedListValue}).value));
    // Verify that list2 remains same after extending
    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&list2Value, &list3Value}).value));
}




