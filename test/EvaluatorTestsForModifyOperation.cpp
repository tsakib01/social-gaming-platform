#include <gtest/gtest.h>
#include "Evaluator.h"
#include "GameConfigLoader.h"
#include <iostream>

class EvaluatorModifyOperationTest : public ::testing::Test {
protected:
    Evaluator evaluator;

    // Use a default evaluator
    void SetUp() override {
        evaluator = Evaluator::defaultEvaluatorFactory();
    }
};

// -------------------------------------- LIST MODIFYING TESTS ----------------------------------------------------

// Test case for REVERSE list operation - List of integers
TEST_F(EvaluatorModifyOperationTest, TestReverseIntList){
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
TEST_F(EvaluatorModifyOperationTest, TestReverseStringList){
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
TEST_F(EvaluatorModifyOperationTest, TestReverseListOfBooleans) {
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
TEST_F(EvaluatorModifyOperationTest, TestReverseListOfListOfIntegers) {
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

TEST_F(EvaluatorModifyOperationTest, TestReverseListOfMaps) {
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
TEST_F(EvaluatorModifyOperationTest, TestShuffleIntList) {
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
TEST_F(EvaluatorModifyOperationTest, TestShuffleStringList) {
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
TEST_F(EvaluatorModifyOperationTest, TestShuffleBooleanList) {
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
TEST_F(EvaluatorModifyOperationTest, TestShuffleListOfListOfIntegers) {
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
TEST_F(EvaluatorModifyOperationTest, TestShuffleListOfMaps) {
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
TEST_F(EvaluatorModifyOperationTest, TestExtendListOfIntegers) {
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
TEST_F(EvaluatorModifyOperationTest, TestExtendListOfStrings) {
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
TEST_F(EvaluatorModifyOperationTest, TestExtendListOfBooleans) {
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
TEST_F(EvaluatorModifyOperationTest, TestExtendListOfListOfIntegers) {
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
TEST_F(EvaluatorModifyOperationTest, TestExtendListOfMaps) {
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

TEST_F(EvaluatorModifyOperationTest, TestSetToConfigValue){
    GameConfigLoader gameConfigLoader{"./test/games/setup-parser-test.game"};
    auto gameState = gameConfigLoader.createGameState();
    auto configuration = gameState->getValue("configuration");
    std::string_view idenfitier1=std::string_view("rounds");
    auto identifier = GameEnvironment::Value(idenfitier1);
    auto toStore = GameEnvironment::Value(10);
    evaluator.evaluate(MODIFIER::SET, {&configuration, &identifier, &toStore});
    auto rounds = evaluator.evaluate(OPERATOR::LOOK_UP, {&configuration, &identifier});
    auto information = std::get<int>(rounds.value);
    EXPECT_EQ(information, 10);
}

TEST_F(EvaluatorModifyOperationTest, TestSetMapValue){
    std::unique_ptr<GameEnvironment::Map> map1 = std::make_unique<GameEnvironment::Map>();

    map1->emplace("int1", std::make_unique<GameEnvironment::Value>(5));
    map1->emplace("string1", std::make_unique<GameEnvironment::Value>(std::string_view("string")));
    map1->emplace("bool1", std::make_unique<GameEnvironment::Value>(true));
    GameEnvironment::Value mapValue1(std::move(map1));

    std::vector< GameEnvironment::Value> identifiers={GameEnvironment::Value(std::string_view("int1")),
                                                      GameEnvironment::Value(std::string_view("string1")),GameEnvironment::Value(std::string_view("bool1")) };

    std::unique_ptr<GameEnvironment::Map> map2 = std::make_unique<GameEnvironment::Map>();
    GameEnvironment::Value mapValue2(std::move(map2));
    GameEnvironment::Value int1(5);
    GameEnvironment::Value string1(std::string_view("string"));
    GameEnvironment::Value bool1(true);
    evaluator.evaluate(MODIFIER::SET, {&mapValue2, &identifiers[0], &int1});
    evaluator.evaluate(MODIFIER::SET, {&mapValue2, &identifiers[1], &string1});
    evaluator.evaluate(MODIFIER::SET, {&mapValue2, &identifiers[2], &bool1});
    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&mapValue1, &mapValue2}).value));
}

TEST_F(EvaluatorModifyOperationTest, TestSetListValue){
    GameEnvironment::Value intValue(4);
    GameEnvironment::Value boolValue(true);

    std::unique_ptr<GameEnvironment::List> list1 = std::make_unique<GameEnvironment::List>();
    list1->push_back(std::make_unique<GameEnvironment::Value>(intValue));
    list1->push_back(std::make_unique<GameEnvironment::Value>(boolValue));
    GameEnvironment::Value listValue1(std::move(list1));
    std::unique_ptr<GameEnvironment::List> list2 = std::make_unique<GameEnvironment::List>();
    GameEnvironment::Value listValue2(std::move(list2));
    GameEnvironment::Value index1(1);
    GameEnvironment::Value index2(0);
    evaluator.evaluate(MODIFIER::SET, {&listValue2, &index2, &intValue});
    evaluator.evaluate(MODIFIER::SET, {&listValue2, &index1, &boolValue});
    EXPECT_TRUE(std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&listValue1, &listValue2}).value));
}

TEST_F(EvaluatorModifyOperationTest, TestInvalidValue){
    std::unique_ptr<GameEnvironment::List> list = std::make_unique<GameEnvironment::List>();
    GameEnvironment::Value intValue(4);
    GameEnvironment::Value boolValue(true);
    list->push_back(std::make_unique<GameEnvironment::Value>(intValue));
    GameEnvironment::Value listValue(std::move(list));
    EXPECT_THROW(evaluator.evaluate(MODIFIER::SET, {&listValue, &intValue, &intValue}),std::runtime_error);
    EXPECT_THROW(evaluator.evaluate(MODIFIER::SET, {&intValue, &boolValue, &listValue}),std::runtime_error);
    EXPECT_THROW(evaluator.evaluate(MODIFIER::SET, {&boolValue, &intValue, &listValue}),std::runtime_error);
}





