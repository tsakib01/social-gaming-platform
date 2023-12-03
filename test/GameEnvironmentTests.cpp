#include <gtest/gtest.h>
#include "GameEnvironment.h"
#include "Evaluator.h"

// Test for copy constructor for GameEnvironment::Value
class ValueCopyConstructorTest : public ::testing::Test {
protected:
    // Helper function to check if two values are equal
    bool valuesEqual(const GameEnvironment::Value& v1, const GameEnvironment::Value& v2) {
        return std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&v1, &v2}).value);
    }

    // Use a default evaluator
    void SetUp() override {
        evaluator = Evaluator::defaultEvaluatorFactory();
    }

    // Generate list of number with 5 elements
    std::unique_ptr<GameEnvironment::List> generateNumberList(){
        auto originalList = std::make_unique<GameEnvironment::List>();
        for (int i = 0; i < 5; i++){
            originalList->push_back(std::make_unique<GameEnvironment::Value>(i));
        }
        return originalList;
    }

    // Generate list of string with 5 elements
    std::unique_ptr<GameEnvironment::List> generateStringList(){
        auto originalList = std::make_unique<GameEnvironment::List>();
        for (int i = 0; i < 5; i++){
            originalList->push_back(std::make_unique<GameEnvironment::Value>("Hello" + std::to_string(i)));
        }
        return originalList;
    }

    // Generate list of bool with 5 elements
    std::unique_ptr<GameEnvironment::List> generateBoolList(){
        auto originalList = std::make_unique<GameEnvironment::List>();
        for (int i = 0; i < 5; i++){
            originalList->push_back(std::make_unique<GameEnvironment::Value>(i % 2 == 0));
        }
        return originalList;
    }

    // Generate map of integers with 5 elements
    std::unique_ptr<GameEnvironment::Map> generateNumberMap(){
        auto originalMap = std::make_unique<GameEnvironment::Map>();
        for (int i = 0; i < 5; i++){
            std::string id = "ID" + std::to_string(i);
            originalMap->emplace(id, std::make_unique<GameEnvironment::Value>(i));
        }
        return originalMap;
    }

    // Generate map of bool with 5 elements
    std::unique_ptr<GameEnvironment::Map> generateBoolMap(){
        auto originalMap = std::make_unique<GameEnvironment::Map>();
        for (int i = 0; i < 5; i++){
            std::string id = "ID" + std::to_string(i);
            originalMap->emplace(id, std::make_unique<GameEnvironment::Value>(i % 2 == 0));
        }
        return originalMap;
    }

    // Generate map of string with 5 elements
    std::unique_ptr<GameEnvironment::Map> generateStringMap(){
        auto originalMap = std::make_unique<GameEnvironment::Map>();
        for (int i = 0; i < 5; i++){
            std::string id = "ID" + std::to_string(i);
            originalMap->emplace(id, std::make_unique<GameEnvironment::Value>("Hello" + std::to_string(i)));
        }
        return originalMap;
    }

    Evaluator evaluator;
};

// Copy Constructor Check For Number 
TEST_F(ValueCopyConstructorTest, NumberCopyTest){
    auto original = std::make_unique<GameEnvironment::Value>(1);
    auto copy = std::make_unique<GameEnvironment::Value>(*original);
    EXPECT_TRUE(valuesEqual(*original, *copy));
}

// Copy Constructor Check For Bool 
TEST_F(ValueCopyConstructorTest, BooleanCopyTest){
    auto original = std::make_unique<GameEnvironment::Value>(true);
    auto copy = std::make_unique<GameEnvironment::Value>(*original);
    EXPECT_TRUE(valuesEqual(*original, *copy));
}

// Copy Constructor Check For String 
TEST_F(ValueCopyConstructorTest, StringViewCopyTest){
    auto original = std::make_unique<GameEnvironment::Value>("Hello");
    auto copy = std::make_unique<GameEnvironment::Value>(*original);
    EXPECT_TRUE(valuesEqual(*original, *copy));
}

// Copy Constructor Check For List of Numbers
TEST_F(ValueCopyConstructorTest, ListOfNumbersCopyTest){
    auto originalList = generateNumberList();
    auto original = std::make_unique<GameEnvironment::Value>(std::move(originalList));
    auto copy = std::make_unique<GameEnvironment::Value>(*original);
    EXPECT_TRUE(valuesEqual(*original, *copy));
}

// Copy Constructor Check For List of String
TEST_F(ValueCopyConstructorTest, ListOfStringCopyTest){
    auto originalList = generateStringList();
    auto original = std::make_unique<GameEnvironment::Value>(std::move(originalList));
    auto copy = std::make_unique<GameEnvironment::Value>(*original);
    EXPECT_TRUE(valuesEqual(*original, *copy));
}

// Copy Constructor Check For List of Bool
TEST_F(ValueCopyConstructorTest, ListOfBoolCopyTest){
    auto originalList = generateBoolList();
    auto original = std::make_unique<GameEnvironment::Value>(std::move(originalList));
    auto copy = std::make_unique<GameEnvironment::Value>(*original);
    EXPECT_TRUE(valuesEqual(*original, *copy));
}

// Copy Constructor Check For List of List of Numbers
TEST_F(ValueCopyConstructorTest, ListOfListOfNumberTest){
    auto originalList = std::make_unique<GameEnvironment::List>();
    // Generate list of list of numbers
    for (int i = 0; i < 5; i++){
        auto subList = generateNumberList();
        originalList->push_back(std::make_unique<GameEnvironment::Value>(std::move(subList)));
    }

    auto original = std::make_unique<GameEnvironment::Value>(std::move(originalList));
    auto copy = std::make_unique<GameEnvironment::Value>(*original);
    EXPECT_TRUE(valuesEqual(*original, *copy));
}

// Copy Constructor Check For List of List of List of Numbers
TEST_F(ValueCopyConstructorTest, ListOfListOfListOfNumberTest){
    auto originalList = std::make_unique<GameEnvironment::List>();
    // Generate list of list of of list
    for (int i = 0; i < 5; i++){
        auto subList = std::make_unique<GameEnvironment::List>();
        for (int i = 0; i < 5; i++){
            auto subSubList = generateStringList();
            subList->push_back(std::make_unique<GameEnvironment::Value>(std::move(subSubList)));
        }
        originalList->push_back(std::make_unique<GameEnvironment::Value>(std::move(subList)));
    }

    auto original = std::make_unique<GameEnvironment::Value>(std::move(originalList));
    auto copy = std::make_unique<GameEnvironment::Value>(*original);
    EXPECT_TRUE(valuesEqual(*original, *copy));
}

// Copy Constructor Check For Map of Number
TEST_F(ValueCopyConstructorTest, MapOfNumbersCopyTest){
    auto originalMap = generateNumberMap();
    auto original = std::make_unique<GameEnvironment::Value>(std::move(originalMap));
    auto copy = std::make_unique<GameEnvironment::Value>(*original);
    EXPECT_TRUE(valuesEqual(*original, *copy));
}

// Copy Constructor Check For Map of Bool
TEST_F(ValueCopyConstructorTest, MapOfBoolCopyTest){
    auto originalMap = generateBoolMap();
    auto original = std::make_unique<GameEnvironment::Value>(std::move(originalMap));
    auto copy = std::make_unique<GameEnvironment::Value>(*original);
    EXPECT_TRUE(valuesEqual(*original, *copy));
}

// Copy Constructor Check For Map of String
TEST_F(ValueCopyConstructorTest, MapOfStringCopyTest){
    auto originalMap = generateStringMap();
    auto original = std::make_unique<GameEnvironment::Value>(std::move(originalMap));
    auto copy = std::make_unique<GameEnvironment::Value>(*original);
    EXPECT_TRUE(valuesEqual(*original, *copy));
}

// Copy Constructor Check For Map of List
TEST_F(ValueCopyConstructorTest, MapOfListCopyTest){
    // Generate a map of list
    auto originalMap = std::make_unique<GameEnvironment::Map>();
    for (int i = 0; i < 5; i++){
        std::string id = "ID" + std::to_string(i);
        auto subList = generateNumberList();
        originalMap->emplace(id, std::make_unique<GameEnvironment::Value>(std::move(subList)));
    }
    auto original = std::make_unique<GameEnvironment::Value>(std::move(originalMap));
    auto copy = std::make_unique<GameEnvironment::Value>(*original);
    EXPECT_TRUE(valuesEqual(*original, *copy));
}

// Copy Constructor Check For Map of Map
TEST_F(ValueCopyConstructorTest, MapOfMapCopyTest){
    // Generate a map of map
    auto originalMap = std::make_unique<GameEnvironment::Map>();
    for (int i = 0; i < 5; i++){
        std::string id = "ID" + std::to_string(i);
        auto subMap = generateNumberMap();
        originalMap->emplace(id, std::make_unique<GameEnvironment::Value>(std::move(subMap)));
    }
    auto original = std::make_unique<GameEnvironment::Value>(std::move(originalMap));
    auto copy = std::make_unique<GameEnvironment::Value>(*original);
    EXPECT_TRUE(valuesEqual(*original, *copy));
}