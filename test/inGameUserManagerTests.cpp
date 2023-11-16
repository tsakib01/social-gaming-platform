#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "InGameUserManager.h"
TEST(InGameUserManagerTests, BasicUserCount){
    InGameUserManager inGameUManager;
    uint32_t dummyUserID = 1;
    Role dummyRole = Role::PLAYER;
    GameEnvironment::Environment dummyEnvironment;
    
    uint32_t dummyUserID2 = 2;
    Role dummyRole2 = Role::PLAYER;
    GameEnvironment::Environment dummyEnvironment2;

    inGameUManager.addNewUser(dummyUserID, dummyRole, std::move(dummyEnvironment));
    inGameUManager.addNewUser(dummyUserID2, dummyRole2, std::move(dummyEnvironment2));

    ASSERT_EQ(std::move(inGameUManager.getAllUserStates()).size(), 2);
}

TEST(InGameUserManagerTests, CanDeleteAllUsers){
    InGameUserManager inGameUManager;
    uint32_t dummyUserID = 1;
    Role dummyRole = Role::PLAYER;
    GameEnvironment::Environment dummyEnvironment;
    
    uint32_t dummyUserID2 = 2;
    Role dummyRole2 = Role::PLAYER;
    GameEnvironment::Environment dummyEnvironment2;
    
    inGameUManager.addNewUser(dummyUserID, dummyRole, std::move(dummyEnvironment));
    inGameUManager.addNewUser(dummyUserID2, dummyRole2, std::move(dummyEnvironment2));

    inGameUManager.deleteUser(dummyUserID);
    inGameUManager.deleteUser(dummyUserID2);
    ASSERT_EQ(std::move(inGameUManager.getAllUserStates()).size(), 0);
}

TEST(InGameUserManagerTests, CanGetStatesOfUser){
    InGameUserManager inGameUManager;
    uint32_t dummyUserID = 1;
    Role dummyRole = Role::PLAYER;
    
    // We care about using the Identifier so that we can find the user's Value.
    GameEnvironment::Identifier testIdentifier = "testidentifier";
    std::unique_ptr<GameEnvironment::Value> testValue;
    std::map<GameEnvironment::Identifier, std::unique_ptr<GameEnvironment::Value>> testMap;
    testMap.insert({testIdentifier, std::move(testValue)});

    GameEnvironment::Environment nonNullDummyEnvironment = std::move(testMap);
    inGameUManager.addNewUser(dummyUserID, dummyRole, std::move(nonNullDummyEnvironment));
    GameEnvironment::Environment testReturnedStates = std::move(inGameUManager.getStatesOfUser(dummyUserID));
    ASSERT_TRUE(std::move(testReturnedStates).find(testIdentifier) != testReturnedStates.end());
}

TEST(InGameUserManagerTests, CanSetStatesOfExistingUser){
    InGameUserManager inGameUManager;
    uint32_t dummyUserID = 1;
    Role dummyRole = Role::PLAYER;
    
    // We care about using the Identifier so that we can find the user's Value.
    GameEnvironment::Identifier testIdentifier = "testidentifier";
    
    // Test overwriting an Environment with nothing in it.
    GameEnvironment::Environment nullEnvironment;

    std::unique_ptr<GameEnvironment::Value> testValue;
    std::map<GameEnvironment::Identifier, std::unique_ptr<GameEnvironment::Value>> testMap;
    testMap.insert({testIdentifier, std::move(testValue)});
    GameEnvironment::Environment nonNullDummyEnvironment = std::move(testMap);
    inGameUManager.addNewUser(dummyUserID, dummyRole, std::move(nullEnvironment));
    
    // Without using the nonNullDummyEnvironment, the assert would fail.
    inGameUManager.setStatesOfUser(dummyUserID, dummyRole, std::move(nonNullDummyEnvironment));
    GameEnvironment::Environment testReturnedStates = std::move(inGameUManager.getStatesOfUser(dummyUserID));
    ASSERT_TRUE(testReturnedStates.find(testIdentifier) != testReturnedStates.end());
    
    // Check that the identifier exists in both and is mapped to the same value.
    ASSERT_EQ(testReturnedStates[testIdentifier], nonNullDummyEnvironment[testIdentifier]);
}