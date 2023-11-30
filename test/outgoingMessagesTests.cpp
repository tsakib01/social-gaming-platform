#include <gtest/gtest.h>
#include "OutgoingMessages.h"

const auto TestMessage = "testMsg";
const auto TestUsers = std::vector<UserId>{{1}, {2}, {3}};

TEST(OutgoingMessagesTests, TestNoTestUsers) {
    auto outgoing = OutgoingMessages{{}};
    auto messages = outgoing.getMessages();
    ASSERT_EQ(messages.size(), 0);

    EXPECT_THROW(outgoing.setMessage({1}, TestMessage), std::runtime_error);
}

TEST(OutgoingMessagesTests, TestSetMessageAmongMultipleTestUsers) {
    auto outgoing = OutgoingMessages{TestUsers};
    auto messages = outgoing.getMessages();

    ASSERT_EQ(messages.size(), 3);
    for(auto& [user, message] : messages)
        ASSERT_EQ(message, "");
    
    auto targetUser = TestUsers.front();
    outgoing.setMessage(targetUser, TestMessage);
    messages = outgoing.getMessages();

    ASSERT_EQ(messages.size(), 3);
    ASSERT_EQ(messages[targetUser], TestMessage);
    messages.erase(targetUser);
    for(auto& [user, message] : messages)
        ASSERT_EQ(message, "");
}

TEST(OutgoingMessagesTests, TestSetAllMessages) {
    auto outgoing = OutgoingMessages{TestUsers};

    outgoing.setMessageForAll(TestMessage);
    auto messages = outgoing.getMessages();

    ASSERT_EQ(messages.size(), 3);
    for(auto& [user, message] : messages)
        ASSERT_EQ(message, TestMessage);
}

TEST(OutgoingMessagesTests, TestClearMessages) {
    auto outgoing = OutgoingMessages{TestUsers};

    outgoing.setMessageForAll(TestMessage);
    outgoing.clear();
    auto messages = outgoing.getMessages();

    ASSERT_EQ(messages.size(), 3);
    for(auto& [user, message] : messages)
        ASSERT_EQ(message, "");
}
