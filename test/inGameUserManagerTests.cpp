#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Server.h"
#include "InGameUserManager.h"
#include "GameEnvironment.h"
#include "Evaluator.h"

#include <iostream>

// This class is copied from gameEnvirionmentTests.cpp
class ValueCopyConstructorTest2 : public ::testing::Test {
protected:
    bool valuesEqual(const GameEnvironment::Value& v1, const GameEnvironment::Value& v2) {
        return std::get<bool>(evaluator.evaluate(OPERATOR::EQUAL, {&v1, &v2}).value);
    }
    void SetUp() override {
        evaluator = Evaluator::defaultEvaluatorFactory();
    }
    Evaluator evaluator;
};

TEST_F(ValueCopyConstructorTest2, CanGetValueOfExistingUserMAYBE) {
    auto original = std::make_unique<GameEnvironment::Value>("Hello");
    auto copy = std::make_unique<GameEnvironment::Value>(*original);
    std::unique_ptr<InGameUserManager> m_inGameUserManager;
    //InGameUserManager inGameUManager;
    m_inGameUserManager = std::make_unique<InGameUserManager>();
    UserId dummyConnection;
    dummyConnection.id = 1;
    
    // We care about using the Identifier so that we can find the user's Value.
    GameEnvironment::Identifier testIdentifier = "testidentifier";
    std::map<GameEnvironment::Identifier, std::unique_ptr<GameEnvironment::Value>> testMap;

    testMap.insert({testIdentifier, std::move(copy)});
    m_inGameUserManager -> addNewUser(dummyConnection, std::move(testMap));
    //std::map<uintptr_t, GameEnvironment::Environment> tester = m_inGameUserManager -> getAllUserStates();
    // ASSERT_EQ(std::move(inGameUManager.getAllUserStates()).size(), 1);
    // ASSERT_EQ(tester.size(), 1);
    //auto copyTwo = std::make_unique<GameEnvironment::Value>(m_inGameUserManager -> getValueOfUser(dummyConnection, testIdentifier));
    GameEnvironment::Value returnValue = m_inGameUserManager -> getValueOfUser(dummyConnection, testIdentifier);
    auto copy2 = std::make_unique<GameEnvironment::Value>(returnValue);
    //EXPECT_TRUE(valuesEqual(*copy2, *copy));
    // EXPECT_TRUE(copy2.get()->value == copy.get()->value);
}

// From an existing user, get their identifier
TEST_F(ValueCopyConstructorTest2, CanSetIdentifierOfExistingUserMAYBE) {
    auto oldValue = std::make_unique<GameEnvironment::Value>(500);
    auto newValue = std::make_unique<GameEnvironment::Value>(501);
    
    std::unique_ptr<InGameUserManager> m_inGameUserManager;
    //InGameUserManager inGameUManager;
    m_inGameUserManager = std::make_unique<InGameUserManager>();
    UserId dummyConnection;
    
    dummyConnection.id = 1;
    GameEnvironment::Identifier testIdentifier = "testidentifier";

    std::map<GameEnvironment::Identifier, std::unique_ptr<GameEnvironment::Value>> testMap;
    testMap.insert({testIdentifier, std::move(oldValue)});
    m_inGameUserManager -> addNewUser(dummyConnection, std::move(testMap));

    // Value should now change from 500 to 501
    m_inGameUserManager -> setIdentifierOfUser(dummyConnection, testIdentifier, std::move(newValue));
    GameEnvironment::Value takenValue = m_inGameUserManager -> getValueOfUser(dummyConnection, testIdentifier);
    auto returnValue = std::make_unique<GameEnvironment::Value>(takenValue);
    //ASSERT_TRUE(returnValue -> value == newValue -> value);
}

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

TEST(InGameUserManagerTests, CanGetStatesOfUser){
    InGameUserManager inGameUManager;
    UserId dummyConnection;
    dummyConnection.id = 1;
    
    // We care about using the Identifier so that we can find the user's Value.
    GameEnvironment::Identifier testIdentifier = "testidentifier";
    std::unique_ptr<GameEnvironment::Value> testValue;
    std::map<GameEnvironment::Identifier, std::unique_ptr<GameEnvironment::Value>> testMap;
    testMap.insert({testIdentifier, std::move(testValue)});

    GameEnvironment::Environment& nonNullDummyEnvironment = testMap;
    inGameUManager.addNewUser(dummyConnection, std::move(nonNullDummyEnvironment));
    GameEnvironment::Environment& testReturnedStates = inGameUManager.getStatesOfUser(dummyConnection);
    ASSERT_TRUE(testReturnedStates.find(testIdentifier) != testReturnedStates.end());
}

// Test overwriting an Environment with nothing in it, with an Environment that does have something in it.
TEST(InGameUserManagerTests, CanSetStatesOfExistingUser){
    InGameUserManager inGameUManager;
    UserId dummyConnection;
    dummyConnection.id = 1;
    
    // We care about using the Identifier so that we can find the user's Value.
    GameEnvironment::Identifier testIdentifier = "testidentifier";

    GameEnvironment::Environment nullEnvironment;

    // Load Value with an int which nonNullDummyEnvironment will get (and nullEnvironment won't)
    GameEnvironment::Value testValue;
    testValue.value = 500;
    testValue.value = true;
    std::unique_ptr<GameEnvironment::Value> testValuePtr;
    testValuePtr -> value;
    
    // (ENVIRONMENT)
    std::map<GameEnvironment::Identifier, std::unique_ptr<GameEnvironment::Value>> testMap;
    testMap.insert({testIdentifier, std::move(testValuePtr)});

    // Begins with nullEnvironment which doesn't have any elements in its map.
    GameEnvironment::Environment& nonNullDummyEnvironment = testMap;
    inGameUManager.addNewUser(dummyConnection, std::move(nullEnvironment));

    inGameUManager.setStatesOfUser(dummyConnection, std::move(nonNullDummyEnvironment));
    GameEnvironment::Environment& testReturnedStates = inGameUManager.getStatesOfUser(dummyConnection);

    // These prove that the testIdentifier can be found in testReturnedStates but not nullEnvironmentStates
    // Without using the setStatesOfUser function and therefore nonNullDummyEnvironment, the next assert would fail.
    ASSERT_TRUE(testReturnedStates.find(testIdentifier) != testReturnedStates.end());
    ASSERT_TRUE(nullEnvironment.find(testIdentifier) == nullEnvironment.end());

    // Finally, this test proves that testResturnedStates's first element
    // (testIdentifier) is not equivalent to nullEnvironment's first element (which has nothing).
    ASSERT_TRUE(testReturnedStates.begin() != nullEnvironment.begin());
}