#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "InGameUserManager.h"
TEST(InGameUserManagerTests, BasicUserCount){
    InGameUserManager inGameUManager;
    uint32_t dummyUserID = 1;
    Role dummyRole = Role::Player;
    GameEnvironment::Environment dummyEnvironment;
    
    uint32_t dummyUserID2 = 2;
    Role dummyRole2 = Role::Player;
    GameEnvironment::Environment dummyEnvironment2;

    inGameUManager.addNewUser(dummyUserID, dummyRole, std::move(dummyEnvironment));
    inGameUManager.addNewUser(dummyUserID2, dummyRole2, std::move(dummyEnvironment2));

    //ASSERT_EQ(inGameUManager.getAllUserStates().size(), 2);
}

// TEST(InGameUserManagerTests, CanDeleteAllUsers){
//     InGameUserManager inGameUManager;
//     uint32_t dummyUserID = 1;
//     Role dummyRole = Role::Player;
//     GameEnvironment::Environment dummyEnvironment;
    
//     uint32_t dummyUserID2 = 2;
//     Role dummyRole2 = Role::Player;
//     GameEnvironment::Environment dummyEnvironment2;
    
//     inGameUManager.addNewUser(dummyUserID, dummyRole, dummyEnvironment);
//     inGameUManager.addNewUser(dummyUserID2, dummyRole2, dummyEnvironment2);

//     inGameUManager.deleteUser(dummyUserID);
//     inGameUManager.deleteUser(dummyUserID2);
//     ASSERT_EQ(inGameUManager.getAllUserStates().size(), 0);
// }

// TEST(InGameUserManagerTests, CanGetStatesOfUser){
//     InGameUserManager inGameUManager;
//     uint32_t dummyUserID = 1;
//     Role dummyRole = Role::Player;
//     std::string_view testIdentifier = "testIdentifier";
//     std::variant<int> testVariant;
//     testVariant = 2;
//     int testInt = std::get<int>(testVariant);
//     std::map<Identifier, Value> testMap;
//     std::pair<Role, Environment> roleAndEnv;

//     testMap[testIdentifier] = testInt;
//     GameEnvironment::Environment nonNullDummyEnvironment = testMap;
//     roleAndEnv.first = dummyRole; 
//     roleAndEnv.second = nonNullDummyEnvironment;
//     inGameUManager.addNewUser(dummyUserID, dummyRole, nonNullDummyEnvironment);
//     GameEnvironment::Environment testReturnedStates = inGameUManager.getStatesOfUser(dummyUserID);

//     ASSERT_TRUE(testReturnedStates.find(testIdentifier) != testReturnedStates.end());
// }

// TEST(InGameUserManagerTests, CanSetStatesOfExistingUser){

//     InGameUserManager inGameUManager;
//     uint32_t dummyUserID = 1;
//     Role dummyRole = Role::Player;
//     std::string_view testIdentifier = "testIdentifier";
//     std::variant<int> testVariant;
//     testVariant = 2;
//     int testInt = std::get<int>(testVariant);
//     std::map<Identifier, Value> testMap;

//     testMap[testIdentifier] = testInt;
//     GameEnvironment::Environment dummyEnvironment;
//     GameEnvironment::Environment nonNullDummyEnvironment = testMap;
//     inGameUManager.addNewUser(dummyUserID, dummyRole, dummyEnvironment);
//     inGameUManager.setStatesOfUser(dummyUserID, dummyRole, nonNullDummyEnvironment);
//     GameEnvironment::Environment testReturnedStates = inGameUManager.getStatesOfUser(dummyUserID);

//     ASSERT_TRUE(testReturnedStates.find(testIdentifier) != testReturnedStates.end());
// }