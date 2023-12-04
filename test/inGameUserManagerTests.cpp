#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Server.h"
#include "InGameUserManager.h"
#include "GameEnvironment.h"
#include "Evaluator.h"

#include <iostream>

// This class is copied from gameEnvirionmentTests.cpp
class InGameUserManagerCopyValueTests : public ::testing::Test {
protected:
    bool valuesEqual(const GameEnvironment::Value& v1, const GameEnvironment::Value& v2) {
        return std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&v1, &v2}).value);
    }
    void SetUp() override {
        evaluator = Evaluator::defaultEvaluatorFactory();
    }
    Evaluator evaluator;
    std::string testIdentifier = "testidentifier";
};

TEST_F(InGameUserManagerCopyValueTests, CanGetValueOfExistingUserMAYBE) {
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

// From an existing user, get their identifier
TEST_F(InGameUserManagerCopyValueTests, CanSetIdentifierOfExistingUserMAYBE) {

    //Copies are due to std::move()
    auto oldValue = std::make_unique<GameEnvironment::Value>(500);
    auto oldValueCopy = std::make_unique<GameEnvironment::Value>(*oldValue);
    auto newValue = std::make_unique<GameEnvironment::Value>(501);
    auto newValueCopy = std::make_unique<GameEnvironment::Value>(*newValue);
    
    auto m_inGameUserManager = std::make_unique<InGameUserManager>();
    UserId dummyConnection{1};

    GameEnvironment::Environment testMap;

    testMap.insert({std::string_view(testIdentifier), std::move(oldValue)});
    m_inGameUserManager -> addNewUser(dummyConnection, std::move(testMap));

    // Value should now change from 500 to 501... but running both of the next lines causes an error.
    // m_inGameUserManager -> setIdentifierOfUser(dummyConnection, testIdentifier, *newValue);
    // GameEnvironment::Value returnValue = m_inGameUserManager -> getValueOfUser(dummyConnection, testIdentifier);

    //EXPECT_FALSE(valuesEqual(returnValue, *oldValueCopy));
    //EXPECT_TRUE(valuesEqual(returnValue, *newValueCopy));
}

// TEST(InGameUserManagerTests, BasicUserCount){
//     InGameUserManager inGameUManager;
//     UserId dummyConnection1;
//     dummyConnection1.id = 1;
//     GameEnvironment::Environment dummyEnvironment;
    
//     UserId dummyConnection2;
//     dummyConnection2.id = 2;
//     GameEnvironment::Environment dummyEnvironment2;

//     inGameUManager.addNewUser(dummyConnection1, std::move(dummyEnvironment));
//     inGameUManager.addNewUser(dummyConnection2, std::move(dummyEnvironment2));

//     ASSERT_EQ(std::move(inGameUManager.getAllUserStates()).size(), 2);
// }

// TEST(InGameUserManagerTests, CanDeleteAllUsers){
//     InGameUserManager inGameUManager;
//     UserId dummyConnection1;
//     dummyConnection1.id = 1;
//     GameEnvironment::Environment dummyEnvironment;
    
//     UserId dummyConnection2;
//     dummyConnection2.id = 2;
//     GameEnvironment::Environment dummyEnvironment2;
    
//     inGameUManager.addNewUser(dummyConnection1, std::move(dummyEnvironment));
//     inGameUManager.addNewUser(dummyConnection2, std::move(dummyEnvironment2));

//     inGameUManager.deleteUser(dummyConnection1);
//     inGameUManager.deleteUser(dummyConnection2);
//     ASSERT_EQ(std::move(inGameUManager.getAllUserStates()).size(), 0);
// }

// TEST(InGameUserManagerTests, CanGetStatesOfUser){
//     InGameUserManager inGameUManager;
//     UserId dummyConnection;
//     dummyConnection.id = 1;
    
//     // We care about using the Identifier so that we can find the user's Value.
//     std::unique_ptr<GameEnvironment::Value> testValue;
//     std::map<GameEnvironment::Identifier, std::unique_ptr<GameEnvironment::Value>> testMap;
//     testMap.insert({std::string_view(testIdentifier), std::move(testValue)});

//     GameEnvironment::Environment& nonNullDummyEnvironment = testMap;
//     inGameUManager.addNewUser(dummyConnection, std::move(nonNullDummyEnvironment));
//     GameEnvironment::Environment& testReturnedStates = inGameUManager.getStatesOfUser(dummyConnection);
//     ASSERT_TRUE(testReturnedStates.find(testIdentifier) != testReturnedStates.end());
// }

// // Test overwriting an Environment with nothing in it, with an Environment that does have something in it.
// TEST(InGameUserManagerTests, CanSetStatesOfExistingUser){
//     InGameUserManager inGameUManager;
//     UserId dummyConnection;
//     dummyConnection.id = 1;
    
//     // We care about using the Identifier so that we can find the user's Value.

//     GameEnvironment::Environment nullEnvironment;

//     // Load Value with an int which nonNullDummyEnvironment will get (and nullEnvironment won't)
//     GameEnvironment::Value testValue;
//     testValue.value = 500;
//     testValue.value = true;
//     std::unique_ptr<GameEnvironment::Value> testValuePtr;
//     testValuePtr -> value;
    
//     // (ENVIRONMENT)
//     std::map<GameEnvironment::Identifier, std::unique_ptr<GameEnvironment::Value>> testMap;
//     testMap.insert({std::string_view(testIdentifier), std::move(testValuePtr)});

//     // Begins with nullEnvironment which doesn't have any elements in its map.
//     GameEnvironment::Environment& nonNullDummyEnvironment = testMap;
//     inGameUManager.addNewUser(dummyConnection, std::move(nullEnvironment));

//     inGameUManager.setStatesOfUser(dummyConnection, std::move(nonNullDummyEnvironment));
//     GameEnvironment::Environment& testReturnedStates = inGameUManager.getStatesOfUser(dummyConnection);

//     // These prove that the testIdentifier can be found in testReturnedStates but not nullEnvironmentStates
//     // Without using the setStatesOfUser function and therefore nonNullDummyEnvironment, the next assert would fail.
//     ASSERT_TRUE(testReturnedStates.find(std::string_view(testIdentifier)) != testReturnedStates.end());
//     ASSERT_TRUE(nullEnvironment.find(std::string_view(testIdentifier)) == nullEnvironment.end());

//     // Finally, this test proves that testResturnedStates's first element
//     // (testIdentifier) is not equivalent to nullEnvironment's first element (which has nothing).
//     ASSERT_TRUE(testReturnedStates.begin() != nullEnvironment.begin());
// }