#include "gtest/gtest.h"
#include "GameCommunicator.h"

// Fixture for GameCommunicator tests
class GameCommunicatorTest : public ::testing::Test {
protected:
    GameCommunicator communicator;
};

// Test setGameChoices and getChoicesForUser
TEST_F(GameCommunicatorTest, TestSetAndGetGameChoices) {
    std::vector<std::string_view> choices = {"rock", "paper", "scissors"};
    communicator.setGameChoices(1, choices);
    auto retrievedChoices = communicator.getChoicesForUser(1);
    ASSERT_EQ(retrievedChoices.size(), 3);
    ASSERT_EQ(retrievedChoices[0], "rock");
    ASSERT_EQ(retrievedChoices[1], "paper");
    ASSERT_EQ(retrievedChoices[2], "scissors");
}

// Test storeCurrentUserInput and getCurrentUserInput
TEST_F(GameCommunicatorTest, TestStoreAndGetCurrentUserInput) {
    UserId userID{1};
    Message userMessage;
    userMessage.connection = userID;
    userMessage.text = "rock";
    communicator.storeCurrentUserInput(userMessage);
    auto retrievedInput = communicator.getCurrentUserInput(userID);
    ASSERT_EQ(retrievedInput, "rock");
}

// Test setGameMessage
TEST_F(GameCommunicatorTest, TestSetGameMessage) {
    std::map<UserId, std::string_view> userMessages = {
        {UserId{123}, "hello"},
        {UserId{456}, "hello"}
    };
    communicator.setGameMessage(userMessages);
    auto retrievedMessages = communicator.getGameMessages();
    ASSERT_EQ(retrievedMessages.size(), 2);
    ASSERT_EQ(retrievedMessages[0].text, "hello");
    ASSERT_EQ(retrievedMessages[1].text, "hello");
}