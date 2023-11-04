#include <gtest/gtest.h>
#include "InGameUserManager.h"

// Environment = std::map<Identifier, Value>;
TEST(InGameUserManagerTests, BasicUserCount){
    InGameUserManager inGameUManager;
    uint32_t dummyUserID = 1;
    Role dummyRole = Role::Player;
    Environment dummyEnvironment;
    
    uint32_t dummyUserID2 = 1;
    Role dummyRole2 = Role::PLAYER;
    Environment dummyEnvironment2;

    InGameUserManager::addNewUser(userID, dummyRole, dummyEnvironment);
    InGameUserManager::addNewUser(userID2, dummyRole2, dummyEnvironment2);

    ASSERT_EQ(m_userStates.size(), 2);
}

TEST(InGameUserManagerTests, CanDeleteAllUsers){
    InGameUserManager inGameUManager;
    uint32_t dummyUserID = 1;
    Role dummyRole = Role::Player;
    Environment dummyEnvironment;
    
    uint32_t dummyUserID2 = 1;
    Role dummyRole2 = Role::Player;
    Environment dummyEnvironment2;
    
    InGameUserManager::addNewUser(userID, dummyRole, dummyEnvironment);
    InGameUserManager::addNewUser(userID2, dummyRole2, dummyEnvironment2);

    InGameUserManager::deleteUser(userID);
    InGameUserManager::deleteUser(userID2);
    ASSERT_EQ(m_userStates.size(), 0);
}

// using Identifier = std::string_view;
// using Primitive = std::variant<int, bool, std::string_view>;
TEST(InGameUserManagerTests, CanGetStatesOfUserWithIntAndBool){
    InGameUserManager inGameUManager;
    uint32_t dummyUserID = 1;
    Role dummyRole = Role::Player;
    Environment dummyEnvironment = std::map<Identifier, Value> testMap;
    std::string_view testIdentifier = "testIdentifier";
    std::variant<int, boolean> testVariant;
    testVariant = 1;
    int testInt = std::get<int>(testVariant);

    InGameUserManager::addNewUser(userID, dummyRole, dummyEnvironment);
    Environment testReturnedStates = InGameUserManager::getStatesOfUser(userID);

    uint32_t dummyUserID2 = 1;
    Role dummyRole2 = Role::Audience;
    Environment dummyEnvironment2 = std::map<Identifier, Value> testMap;
    std::string_view testIdentifier2 = "testIdentifier2";
    std::variant<int, boolean> testVariant2;
    testVariant2 = false;
    int testInt2 = std::get<bool>(testVariant2);
    testMap.insert({testIdentifier2, testVariant2});

    InGameUserManager::addNewUser(userID2, dummyRole2, dummyEnvironment2);
    Environment testReturnedStates2 = InGameUserManager::getStatesOfUser(userID2);

    ASSERT_EQ(testReturnedStates["testIdentifier"], 1);
    ASSERT_FALSE(testReturnedStates["testIdentifier2"]);
}

TEST(InGameUserManagerTests, CanSetStatesOfExistingUser){
    uint32_t dummyUserID = 1;
    Role dummyRole = Role::Owner;
    Environment nullDummyEnvironment = NULL;
    Environment nonNullDummyEnvironment = std::map<Identifier, Value> testMap;
    std::string_view testIdentifier = "testIdentifier";
    std::variant<int, boolean> testVariant;
    testVariant = true;
    int testInt = std::get<bool>(testVariant);
    testMap.insert({testStringView, testVariant});
    
    InGameUserManager::addNewUser(userID, dummyRole, nullDummyEnvironment);
    InGameUserManager::setStatesOfUser(userID, dummyRole, nonNullDummyEnvironment);
    
    Environment testReturnedStates2 = InGameUserManager::getStatesOfUser(userID2);
    ASSERT_TRUE(testReturnedStates["testIdentifier"]);
}