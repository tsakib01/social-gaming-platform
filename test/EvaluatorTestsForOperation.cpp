#include <gtest/gtest.h>
#include "Evaluator.h"
#include <iostream>

class EvaluatorOperationTest : public ::testing::Test {
protected:
    Evaluator evaluator;
    std::string MAP_KEYS[5] = {"ID1", "ID2", "ID3", "ID4", "ID5"};

    // Use a default evaluator
    void SetUp() override {
        evaluator = Evaluator::defaultEvaluatorFactory();
    }
};

// Test case for ADD operation
TEST_F(EvaluatorOperationTest, TestValidAddOperation) {
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
}

// Test case for invalid ADD operations
TEST_F(EvaluatorOperationTest, TestInvalidAddOperations) {
    // Only 1 argument for integer
    GameEnvironment::Value intValue;
    intValue.value = 5;
    EXPECT_THROW(evaluator.evaluate(OPERATOR::ADD, {&intValue}), std::runtime_error);

    // Try to add 3 arguments for integers
    EXPECT_THROW(evaluator.evaluate(OPERATOR::ADD, {&intValue, &intValue, &intValue}), std::runtime_error);

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
TEST_F(EvaluatorOperationTest, TestValidSubtractOperation) {
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
TEST_F(EvaluatorOperationTest, TestInvalidSubtractOperations) {
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
TEST_F(EvaluatorOperationTest, TestValidMultiplyOperation) {
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
TEST_F(EvaluatorOperationTest, TestInvalidMultiplyOperations) {
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
TEST_F(EvaluatorOperationTest, TestValidDivideOperation) {
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
TEST_F(EvaluatorOperationTest, TestInvalidDivideOperations) {
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
TEST_F(EvaluatorOperationTest, TestValidOrOperation) {
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
TEST_F(EvaluatorOperationTest, TestInvalidOrOperations) {
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
TEST_F(EvaluatorOperationTest, TestValidAndOperation) {
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
TEST_F(EvaluatorOperationTest, TestInvalidAndOperations) {
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
TEST_F(EvaluatorOperationTest, TestValidNotOperation) {
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
TEST_F(EvaluatorOperationTest, TestInvalidNotOperations) {
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
TEST_F(EvaluatorOperationTest, TestEqualPrimitiveTypes) {
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
TEST_F(EvaluatorOperationTest, TestEqualIntList){
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
TEST_F(EvaluatorOperationTest, TestEqualBoolList){
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
TEST_F(EvaluatorOperationTest, TestEqualStringList){
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
TEST_F(EvaluatorOperationTest, TestEqualListOfIntList) {
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
TEST_F(EvaluatorOperationTest, TestEqualListOfMap) {
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
            innerMap1->emplace(MAP_KEYS[j], std::make_unique<GameEnvironment::Value>(j));
            innerMap2->emplace(MAP_KEYS[j], std::make_unique<GameEnvironment::Value>(j));
            innerMap3->emplace(MAP_KEYS[j], std::make_unique<GameEnvironment::Value>(j + 1));
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
TEST_F(EvaluatorOperationTest, TestEqualMapOfInt) {
    // Map of integers
    std::unique_ptr<GameEnvironment::Map> intMap1 = std::make_unique<GameEnvironment::Map>();
    std::unique_ptr<GameEnvironment::Map> intMap2 = std::make_unique<GameEnvironment::Map>();
    std::unique_ptr<GameEnvironment::Map> intMap3 = std::make_unique<GameEnvironment::Map>();

    // Populating lists with 5 elements
    for (int i = 0; i < 5; i++) {
        intMap1->emplace(MAP_KEYS[i], std::make_unique<GameEnvironment::Value>(i));
        intMap2->emplace(MAP_KEYS[i], std::make_unique<GameEnvironment::Value>(i));
        intMap3->emplace(MAP_KEYS[i], std::make_unique<GameEnvironment::Value>(i + 1));
    }

    GameEnvironment::Value intMapValue1(std::move(intMap1));
    GameEnvironment::Value intMapValue2(std::move(intMap2));
    GameEnvironment::Value intMapValue3(std::move(intMap3));

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&intMapValue1, &intMapValue2}).value));
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&intMapValue1, &intMapValue3}).value));
}

// Test case for EQUAL operation - Map of booleans
TEST_F(EvaluatorOperationTest, TestEqualMapOfBool) {
    // Map of booleans
    std::unique_ptr<GameEnvironment::Map> boolMap1 = std::make_unique<GameEnvironment::Map>();
    std::unique_ptr<GameEnvironment::Map> boolMap2 = std::make_unique<GameEnvironment::Map>();
    std::unique_ptr<GameEnvironment::Map> boolMap3 = std::make_unique<GameEnvironment::Map>();

    // Populating lists with 5 elements
    for (int i = 0; i < 5; i++) {
        boolMap1->emplace(MAP_KEYS[i], std::make_unique<GameEnvironment::Value>(i % 2 == 0));
        boolMap2->emplace(MAP_KEYS[i], std::make_unique<GameEnvironment::Value>(i % 2 == 0));
        boolMap3->emplace(MAP_KEYS[i], std::make_unique<GameEnvironment::Value>(i % 2 == 1));
    }

    GameEnvironment::Value boolMapValue1(std::move(boolMap1));
    GameEnvironment::Value boolMapValue2(std::move(boolMap2));
    GameEnvironment::Value boolMapValue3(std::move(boolMap3));

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&boolMapValue1, &boolMapValue2}).value));
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&boolMapValue1, &boolMapValue3}).value));
}

// Test case for EQUAL operation - Map of strings
TEST_F(EvaluatorOperationTest, TestEqualMapOfString) {
    // Map of strings
    std::unique_ptr<GameEnvironment::Map> stringMap1 = std::make_unique<GameEnvironment::Map>();
    std::unique_ptr<GameEnvironment::Map> stringMap2 = std::make_unique<GameEnvironment::Map>();
    std::unique_ptr<GameEnvironment::Map> stringMap3 = std::make_unique<GameEnvironment::Map>();

    // Populating lists with 5 elements
    for (int i = 0; i < 5; i++) {
        stringMap1->emplace(MAP_KEYS[i], std::make_unique<GameEnvironment::Value>(std::string_view("Hello")));
        stringMap2->emplace(MAP_KEYS[i], std::make_unique<GameEnvironment::Value>(std::string_view("Hello")));
        stringMap3->emplace(MAP_KEYS[i], std::make_unique<GameEnvironment::Value>(std::string_view("World")));
    }

    GameEnvironment::Value stringMapValue1(std::move(stringMap1));
    GameEnvironment::Value stringMapValue2(std::move(stringMap2));
    GameEnvironment::Value stringMapValue3(std::move(stringMap3));

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&stringMapValue1, &stringMapValue2}).value));
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&stringMapValue1, &stringMapValue3}).value));
}

// Test case for EQUAL operation - Map of lists
TEST_F(EvaluatorOperationTest, TestEqualMapOfList) {
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
        mapOfList1->emplace(MAP_KEYS[i], std::make_unique<GameEnvironment::Value>(std::move(list1)));
        mapOfList2->emplace(MAP_KEYS[i], std::make_unique<GameEnvironment::Value>(std::move(list2)));
        mapOfList3->emplace(MAP_KEYS[i], std::make_unique<GameEnvironment::Value>(std::move(list3)));
    }

    GameEnvironment::Value mapOfListValue1(std::move(mapOfList1));
    GameEnvironment::Value mapOfListValue2(std::move(mapOfList2));
    GameEnvironment::Value mapOfListValue3(std::move(mapOfList3));

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&mapOfListValue1, &mapOfListValue2}).value));
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&mapOfListValue1, &mapOfListValue3}).value));
}

// Test case for EQUAL operation - Map of maps
TEST_F(EvaluatorOperationTest, TestEqualMapOfMap) {
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
            innerMap1->emplace(MAP_KEYS[j], std::make_unique<GameEnvironment::Value>(i + j));
            innerMap2->emplace(MAP_KEYS[j], std::make_unique<GameEnvironment::Value>(i + j));
            innerMap3->emplace(MAP_KEYS[j], std::make_unique<GameEnvironment::Value>((i + j) + 1));
        }

        mapOfMap1->emplace(MAP_KEYS[i], std::make_unique<GameEnvironment::Value>(std::move(innerMap1)));
        mapOfMap2->emplace(MAP_KEYS[i], std::make_unique<GameEnvironment::Value>(std::move(innerMap2)));
        mapOfMap3->emplace(MAP_KEYS[i], std::make_unique<GameEnvironment::Value>(std::move(innerMap3)));
    }

    GameEnvironment::Value mapOfMapValue1(std::move(mapOfMap1));
    GameEnvironment::Value mapOfMapValue2(std::move(mapOfMap2));
    GameEnvironment::Value mapOfMapValue3(std::move(mapOfMap3));

    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&mapOfMapValue1, &mapOfMapValue2}).value));
    EXPECT_FALSE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&mapOfMapValue1, &mapOfMapValue3}).value));
}

// Test case for SIZE operation
TEST_F(EvaluatorOperationTest, TestValidSizeOperation) {
    // Create a list with some elements
    auto list = std::make_unique<GameEnvironment::List>();
    for (int i = 0; i < 5; ++i) {
        list->push_back(std::make_unique<GameEnvironment::Value>(i));
    }
    GameEnvironment::Value listValue(std::move(list));
    
    // Non-empty list check
    GameEnvironment::Value sizeResult = evaluator.evaluate(OPERATOR::SIZE, {&listValue});
    EXPECT_EQ(std::get<int>(sizeResult.value), 5);

    // Empty list check
    GameEnvironment::Value emptyListValue(std::make_unique<GameEnvironment::List>());
    GameEnvironment::Value sizeResultEmpty = evaluator.evaluate(OPERATOR::SIZE, {&emptyListValue});
    EXPECT_EQ(std::get<int>(sizeResultEmpty.value), 0);
}

// Test case for invalid SIZE operation on non-list type
TEST_F(EvaluatorOperationTest, TestInvalidSizeOperation) {
    // Create values that is not a list
    GameEnvironment::Value intValue(10);
    GameEnvironment::Value boolValue(true);
    GameEnvironment::Value stringValue(std::string_view("Hello"));

    // Check the SIZE operation on a non-list type
    EXPECT_THROW(evaluator.evaluate(OPERATOR::SIZE, {&intValue}), std::runtime_error);
    EXPECT_THROW(evaluator.evaluate(OPERATOR::SIZE, {&boolValue}), std::runtime_error);
    EXPECT_THROW(evaluator.evaluate(OPERATOR::SIZE, {&stringValue}), std::runtime_error);

    // Check passing two arguments
    GameEnvironment::Value listValue(std::make_unique<GameEnvironment::List>());
    EXPECT_THROW(evaluator.evaluate(OPERATOR::SIZE, {&listValue, &listValue}), std::runtime_error);
}

// Test case for CONTAIN operation with a list of integers
TEST_F(EvaluatorOperationTest, TestContainOperationWithListOfIntegers) {
    // Create a list of integers
    auto intList = std::make_unique<GameEnvironment::List>();
    for (int i = 0; i < 5; ++i) {
        intList->push_back(std::make_unique<GameEnvironment::Value>(i * 2));
    }
    GameEnvironment::Value intListValue(std::move(intList));

    // Target integer that exists in the list
    GameEnvironment::Value targetIntValue(6);
    GameEnvironment::Value containResult = evaluator.evaluate(OPERATOR::CONTAIN, {&intListValue, &targetIntValue});
    EXPECT_TRUE(std::get<bool>(containResult.value));

    // Target integer that doesn't exist in the list
    GameEnvironment::Value targetIntValueNotFound(9);
    containResult = evaluator.evaluate(OPERATOR::CONTAIN, {&intListValue, &targetIntValueNotFound});
    EXPECT_FALSE(std::get<bool>(containResult.value));

    // Target that is not an integer
    GameEnvironment::Value targeStringValue(std::string_view("hello"));
    containResult = evaluator.evaluate(OPERATOR::CONTAIN, {&intListValue, &targeStringValue});
    EXPECT_FALSE(std::get<bool>(containResult.value));
}

// Test case for CONTAIN operation with a list of strings
TEST_F(EvaluatorOperationTest, TestContainOperationWithListOfStrings) {
    // Create a list of strings
    auto stringList = std::make_unique<GameEnvironment::List>();
    std::vector<std::string> strings = {"apple", "banana", "orange", "grapes", "watermelon"};
    for (const auto& str : strings) {
        stringList->push_back(std::make_unique<GameEnvironment::Value>(std::string_view(str)));
    }
    GameEnvironment::Value stringListValue(std::move(stringList));

    // Target string that exists in the list
    GameEnvironment::Value targetStrValue(std::string_view("orange"));
    GameEnvironment::Value containResult = evaluator.evaluate(OPERATOR::CONTAIN, {&stringListValue, &targetStrValue});
    EXPECT_TRUE(std::get<bool>(containResult.value));

    // Target string that doesn't exist in the list
    GameEnvironment::Value targetStrValueNotFound(std::string_view("pineapple"));
    containResult = evaluator.evaluate(OPERATOR::CONTAIN, {&stringListValue, &targetStrValueNotFound});
    EXPECT_FALSE(std::get<bool>(containResult.value));

    // Target that is not a string
    GameEnvironment::Value targeIntValue(1);
    containResult = evaluator.evaluate(OPERATOR::CONTAIN, {&stringListValue, &targeIntValue});
    EXPECT_FALSE(std::get<bool>(containResult.value));
}

// Test case for CONTAIN operation with a list of booleans
TEST_F(EvaluatorOperationTest, TestContainOperationWithListOfBooleans) {
    // Create a list of booleans
    auto boolList = std::make_unique<GameEnvironment::List>();
    for (size_t i = 0; i < 5; i++) {
        boolList->push_back(std::make_unique<GameEnvironment::Value>(true));
    }
    GameEnvironment::Value boolListValue(std::move(boolList));

    // Target boolean that exists in the list
    GameEnvironment::Value targetBoolValue(true);
    GameEnvironment::Value containResult = evaluator.evaluate(OPERATOR::CONTAIN, {&boolListValue, &targetBoolValue});
    EXPECT_TRUE(std::get<bool>(containResult.value));

    // Target boolean that doesn't exist in the list
    GameEnvironment::Value targetBoolValueNotFound(false);
    containResult = evaluator.evaluate(OPERATOR::CONTAIN, {&boolListValue, &targetBoolValueNotFound});
    EXPECT_FALSE(std::get<bool>(containResult.value));

    // Target boolean that is not boolean
    GameEnvironment::Value targeIntValue(1);
    containResult = evaluator.evaluate(OPERATOR::CONTAIN, {&boolListValue, &targeIntValue});
    EXPECT_FALSE(std::get<bool>(containResult.value));
}

// Test case for CONTAIN operation with a list of maps
TEST_F(EvaluatorOperationTest, TestContainOperationWithListOfMaps) {
    auto mapList = std::make_unique<GameEnvironment::List>();

    // Map 1
    auto map1 = std::make_unique<GameEnvironment::Map>();
    map1->emplace(std::string_view(MAP_KEYS[0]), std::make_unique<GameEnvironment::Value>(1));
    map1->emplace(std::string_view(MAP_KEYS[1]), std::make_unique<GameEnvironment::Value>("value1"));
    auto map1Value = std::make_unique<GameEnvironment::Value>(std::move(map1));
    GameEnvironment::Value targetMapValue(*map1Value);
    
    // Map 2
    auto map2 = std::make_unique<GameEnvironment::Map>();
    map2->emplace(std::string_view(MAP_KEYS[2]), std::make_unique<GameEnvironment::Value>(2));
    map2->emplace(std::string_view(MAP_KEYS[3]), std::make_unique<GameEnvironment::Value>("value2"));
    auto map2Value = std::make_unique<GameEnvironment::Value>(std::move(map2));

    // Push maps to list
    mapList->push_back(std::move(map1Value));
    mapList->push_back(std::move(map2Value));
    GameEnvironment::Value mapListValue(std::move(mapList));

    // Target map that exists in the list
    GameEnvironment::Value containResult = evaluator.evaluate(OPERATOR::CONTAIN, {&mapListValue, &targetMapValue});
    EXPECT_TRUE(std::get<bool>(containResult.value));

    // Target map that doesn't exist in the list
    GameEnvironment::Value targetMapValueNotFound(std::make_unique<GameEnvironment::Map>());
    containResult = evaluator.evaluate(OPERATOR::CONTAIN, {&mapListValue, &targetMapValueNotFound});
    EXPECT_FALSE(std::get<bool>(containResult.value));
}

// Test case for CONTAIN operation with a list of lists
TEST_F(EvaluatorOperationTest, TestContainOperationWithListOfLists) {
    auto listList = std::make_unique<GameEnvironment::List>();

    // List 1
    auto list1 = std::make_unique<GameEnvironment::List>();
    list1->push_back(std::make_unique<GameEnvironment::Value>(1));
    list1->push_back(std::make_unique<GameEnvironment::Value>(2));
    list1->push_back(std::make_unique<GameEnvironment::Value>(3));
    auto list1Value = std::make_unique<GameEnvironment::Value>(std::move(list1));
    GameEnvironment::Value targetListValue(*list1Value);
    
    // List 2
    auto list2 = std::make_unique<GameEnvironment::List>();
    list2->push_back(std::make_unique<GameEnvironment::Value>(4));
    list2->push_back(std::make_unique<GameEnvironment::Value>(5));
    list2->push_back(std::make_unique<GameEnvironment::Value>(6));
    auto list2Value = std::make_unique<GameEnvironment::Value>(std::move(list2));

    // Push lists to list
    listList->push_back(std::move(list1Value));
    listList->push_back(std::move(list2Value));
    GameEnvironment::Value listListValue(std::move(listList));

    // Target list that exists in the list
    GameEnvironment::Value containResult = evaluator.evaluate(OPERATOR::CONTAIN, {&listListValue, &targetListValue});
    EXPECT_TRUE(std::get<bool>(containResult.value));

    // Target list that doesn't exist in the list
    GameEnvironment::Value targetListValueNotFound(std::make_unique<GameEnvironment::List>());
    containResult = evaluator.evaluate(OPERATOR::CONTAIN, {&listListValue, &targetListValueNotFound});
    EXPECT_FALSE(std::get<bool>(containResult.value));
}
