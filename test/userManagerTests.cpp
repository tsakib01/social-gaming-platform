#include <gtest/gtest.h>
#include <vector>
#include "UserManager.h"

TEST(UserManagerTests, AddUser) {
    Connection code1{1};
    UserManager userMgr;
    userMgr.addUser(code1);

    User user = *(userMgr.getUserByID(code1));
    ASSERT_EQ(user.userID, code1);
}

TEST(UserManagerTests, AddTwoUsers) {
    Connection code1{1};
    Connection code2{2};

    UserManager userMgr;
    userMgr.addUser(code1);
    userMgr.addUser(code2);

    User user = *(userMgr.getUserByID(code2));
    ASSERT_EQ(user.userID, code2);
}

TEST(UserManagerTests, AddUserName) {
    Connection code1{1};
    UserManager userMgr;
    userMgr.addUser(code1);
    userMgr.setUserName(code1, "Nick");

    User user = *(userMgr.getUserByID(code1));
    ASSERT_EQ(user.username, "Nick");

    // Check re-assignment
    userMgr.setUserName(code1, "Sumner");
    user = *(userMgr.getUserByID(code1));
    ASSERT_EQ(user.username, "Sumner");
}

TEST(UserManagerTests, AddUserRole) {
    Connection code1{1};
    Connection code2{2};
    Connection code3{3};
    Connection code4{4};
    UserManager userMgr;

    userMgr.addUser(code1);
    userMgr.addUser(code2);
    userMgr.addUser(code3);
    userMgr.addUser(code4);

    userMgr.setUserRole(code1, Role::OWNER);
    userMgr.setUserRole(code2, Role::PLAYER);
    userMgr.setUserRole(code3, Role::AUDIENCE);
    userMgr.setUserRole(code4, Role::NONE);

    User user1 = *(userMgr.getUserByID(code1));
    User user2 = *(userMgr.getUserByID(code2));
    User user3 = *(userMgr.getUserByID(code3));
    User user4 = *(userMgr.getUserByID(code4));

    ASSERT_EQ(user1.role, Role::OWNER);
    ASSERT_EQ(user2.role, Role::PLAYER);
    ASSERT_EQ(user3.role, Role::AUDIENCE);
    ASSERT_EQ(user4.role, Role::NONE);

    // Check re-assignment
    userMgr.setUserRole(code1, Role::PLAYER);
    user1 = *(userMgr.getUserByID(code1));
    ASSERT_EQ(user1.role, Role::PLAYER);
}

TEST(UserManagerTests, AddUserRoomCode) {
    Connection code1{1};
    UserManager userMgr;
    userMgr.addUser(code1);
    userMgr.setUserRoomCode(code1, (uint8_t)123);

    User user = *(userMgr.getUserByID(code1));
    ASSERT_EQ(user.roomCode, (uint8_t)123);

    // Check re-assignment
    userMgr.setUserRoomCode(code1, (uint8_t)312);
    user = *(userMgr.getUserByID(code1));
    ASSERT_EQ(user.roomCode, (uint8_t)312);
}

TEST(UserManagerTests, CheckCollectionOfUsers) {
    Connection code1{1};
    Connection code2{2};
    UserManager userMgr;

    userMgr.addUser(code1);
    userMgr.addUser(code2);

    userMgr.setUserRoomCode(code1, (uint8_t)123);
    userMgr.setUserRoomCode(code2, (uint8_t)123);

    std::vector<User> vec = userMgr.getUsersInGame((uint8_t)123);
    ASSERT_EQ(vec.size(), 2);
}

TEST(UserManagerTests, RemoveUser) {
    Connection code1{1};
    UserManager userMgr;
    userMgr.addUser(code1);
    userMgr.setUserRoomCode(code1, (uint8_t)123);

    std::vector<User> vec = userMgr.getUsersInGame((uint8_t)123);
    ASSERT_EQ(vec.size(), 1);

    userMgr.removeUser(code1);
    vec = userMgr.getUsersInGame((uint8_t)123);
    ASSERT_EQ(vec.size(), 0);

}