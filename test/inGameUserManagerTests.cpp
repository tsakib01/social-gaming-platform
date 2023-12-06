#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Server.h"
#include "InGameUserManager.h"
#include "GameEnvironment.h"
#include "Evaluator.h"

#include <iostream>

// This class and its functions are mostly copied from gameEnvirionmentTests.cpp
class InGameUserManagerCopyValueTests : public ::testing::Test {
const int threeElements = 3;
protected:
    bool valuesEqual(const GameEnvironment::Value& v1, const GameEnvironment::Value& v2) {
        return std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&v1, &v2}).value);
    }
    void SetUp() override {
        evaluator = Evaluator::defaultEvaluatorFactory();
    }

    std::unique_ptr<GameEnvironment::List>makeIntList(){
        auto list = std::make_unique<GameEnvironment::List>();
        for(int i = 0; i < threeElements; i++){
            list -> push_back(std::make_unique<GameEnvironment::Value>(i));
        }
        return list;
    }

    std::unique_ptr<GameEnvironment::List> makeStringList(){
        auto list = std::make_unique<GameEnvironment::List>();
        for (int i = 0; i < threeElements; i++){
            list->push_back(std::make_unique<GameEnvironment::Value>("String" + std::to_string(i)));
        }
        return list;
    }

    Evaluator evaluator;
    std::string testIdentifier = "testidentifier";
};

// Test that Users can be added to InGameUserManager
TEST(InGameUserManagerTests, BasicUserCount){
    InGameUserManager inGameUManager;
    UserId dummyConnection1;
    dummyConnection1.id = 1;
    GameEnvironment::Environment dummyEnvironment;
    
    UserId dummyConnection2;
    dummyConnection2.id = 2;
    GameEnvironment::Environment dummyEnvironment2;

    inGameUManager.addNewUser(dummyConnection1, std::move(dummyEnvironment));
    inGameUManager.addNewUser(dummyConnection2, std::move(dummyEnvironment2));

    ASSERT_EQ(std::move(inGameUManager.getAllUserStates()).size(), 2);
}

// Test that Users can be deleted from InGameUserManager
TEST(InGameUserManagerTests, CanDeleteAllUsers){
    InGameUserManager inGameUManager;
    UserId dummyConnection1;
    dummyConnection1.id = 1;
    GameEnvironment::Environment dummyEnvironment;
    
    UserId dummyConnection2;
    dummyConnection2.id = 2;
    GameEnvironment::Environment dummyEnvironment2;
    
    inGameUManager.addNewUser(dummyConnection1, std::move(dummyEnvironment));
    inGameUManager.addNewUser(dummyConnection2, std::move(dummyEnvironment2));

    inGameUManager.deleteUser(dummyConnection1);
    inGameUManager.deleteUser(dummyConnection2);
    ASSERT_EQ(std::move(inGameUManager.getAllUserStates()).size(), 0);
}

// Check to get int from existing User from InGameUserManager.
TEST_F(InGameUserManagerCopyValueTests, CanGetIntValueOfExistingUser){
    auto original = std::make_unique<GameEnvironment::Value>(1);
    auto copy = std::make_unique<GameEnvironment::Value>(*original);

    auto  m_inGameUserManager = std::make_unique<InGameUserManager>();
    UserId dummyConnection{1};
    
    GameEnvironment::Environment testMap;

    testMap.insert({std::string_view(testIdentifier), std::move(copy)});
    m_inGameUserManager -> addNewUser(dummyConnection, std::move(testMap));
    GameEnvironment::Value returnValue = m_inGameUserManager -> getValueOfUser(dummyConnection, testIdentifier);
    EXPECT_TRUE(valuesEqual(returnValue, *original));
}

TEST_F(InGameUserManagerCopyValueTests, CanGetStringViewValueOfExistingUser) {
    auto original = std::make_unique<GameEnvironment::Value>(std::string_view("Hello"));
    auto copy = std::make_unique<GameEnvironment::Value>(*original);

    auto  m_inGameUserManager = std::make_unique<InGameUserManager>();
    UserId dummyConnection{1};
    
    GameEnvironment::Environment testMap;

    testMap.insert({std::string_view(testIdentifier), std::move(copy)});
    m_inGameUserManager -> addNewUser(dummyConnection, std::move(testMap));
    GameEnvironment::Value returnValue = m_inGameUserManager -> getValueOfUser(dummyConnection, testIdentifier);
    EXPECT_TRUE(valuesEqual(returnValue, *original));
}

// Check to get list from existing User from InGameUserManager.
TEST_F(InGameUserManagerCopyValueTests, CanGetListOfExistingUser){
    auto originalList = makeIntList();
    auto original = std::make_unique<GameEnvironment::Value>(std::move(originalList));
    auto copy = std::make_unique<GameEnvironment::Value>(*original);

    auto  m_inGameUserManager = std::make_unique<InGameUserManager>();
    UserId dummyConnection{1};
    
    GameEnvironment::Environment testMap;

    testMap.insert({std::string_view(testIdentifier), std::move(copy)});
    m_inGameUserManager -> addNewUser(dummyConnection, std::move(testMap));
    GameEnvironment::Value returnValue = m_inGameUserManager -> getValueOfUser(dummyConnection, testIdentifier);
    EXPECT_TRUE(valuesEqual(returnValue, *original));
}

// From an existing User in InGameUserManager, set their Identifier as an int
TEST_F(InGameUserManagerCopyValueTests, CanSetIntIdentifierOfExistingUser) {

    //Copies are due to std::move()
    auto oldValue = std::make_unique<GameEnvironment::Value>(500);
    auto oldValueCopy = std::make_unique<GameEnvironment::Value>(*oldValue);
    auto newValue = std::make_unique<GameEnvironment::Value>(501);
    auto newValueCopy = std::make_unique<GameEnvironment::Value>(*newValue);
    
    auto m_inGameUserManager = std::make_unique<InGameUserManager>();
    UserId dummyConnection{1};

    GameEnvironment::Environment testMap;

    testMap.emplace(std::string_view(testIdentifier), std::move(oldValue));
    m_inGameUserManager -> addNewUser(dummyConnection, std::move(testMap));

    // Value should now change from 500 to 501... but running both of the next lines causes an error.
    m_inGameUserManager -> setIdentifierOfUser(dummyConnection, std::string_view(testIdentifier), std::move(newValue));
    GameEnvironment::Value returnValue = m_inGameUserManager -> getValueOfUser(dummyConnection, testIdentifier);
    EXPECT_FALSE(valuesEqual(returnValue, *oldValueCopy));
    EXPECT_TRUE(valuesEqual(returnValue, *newValueCopy));
}

// From an existing User in InGameUserManager, set their Identifier as a string_view
// The Value copy constructor does not support strings themselves
TEST_F(InGameUserManagerCopyValueTests, CanSetStringViewIdentifierOfExistingUser) {

    //Copies are due to std::move()
    auto oldValue = std::make_unique<GameEnvironment::Value>(std::string_view("Hello"));
    auto oldValueCopy = std::make_unique<GameEnvironment::Value>(*oldValue);
    auto newValue = std::make_unique<GameEnvironment::Value>(std::string_view("Goodbye"));
    auto newValueCopy = std::make_unique<GameEnvironment::Value>(*newValue);
    
    auto m_inGameUserManager = std::make_unique<InGameUserManager>();
    UserId dummyConnection{1};

    GameEnvironment::Environment testMap;

    testMap.emplace(std::string_view(testIdentifier), std::move(oldValue));
    m_inGameUserManager -> addNewUser(dummyConnection, std::move(testMap));

    // Value should now change from 500 to 501... but running both of the next lines causes an error.
    m_inGameUserManager -> setIdentifierOfUser(dummyConnection, std::string_view(testIdentifier), std::move(newValue));
    GameEnvironment::Value returnValue = m_inGameUserManager -> getValueOfUser(dummyConnection, testIdentifier);
    EXPECT_FALSE(valuesEqual(returnValue, *oldValueCopy));
    EXPECT_TRUE(valuesEqual(returnValue, *newValueCopy));
}

// From an existing User in InGameUserManager, set their Identifier as a List
TEST_F(InGameUserManagerCopyValueTests, CanSetListIdentifierOfExistingUser) {

    auto intList = makeIntList();
    auto originalIntList = std::make_unique<GameEnvironment::Value>(std::move(intList));
    auto newIntListCopy = std::make_unique<GameEnvironment::Value>(std::move(*originalIntList));
    auto stringList = makeStringList();
    auto originalStringList = std::make_unique<GameEnvironment::Value>(std::move(stringList));
    auto newStringListCopy = std::make_unique<GameEnvironment::Value>(std::move(*originalStringList));
    auto m_inGameUserManager = std::make_unique<InGameUserManager>();
    UserId dummyConnection{1};

    GameEnvironment::Environment testMap;

    testMap.emplace(std::string_view(testIdentifier), std::move(originalIntList));
    m_inGameUserManager -> addNewUser(dummyConnection, std::move(testMap));

    // Value should now change from 500 to 501
    m_inGameUserManager -> setIdentifierOfUser(dummyConnection, std::string_view(testIdentifier), std::move(originalStringList));
    GameEnvironment::Value returnValue = m_inGameUserManager -> getValueOfUser(dummyConnection, testIdentifier);
    EXPECT_FALSE(valuesEqual(returnValue, *newIntListCopy));
    EXPECT_TRUE(valuesEqual(returnValue, *newStringListCopy));
}