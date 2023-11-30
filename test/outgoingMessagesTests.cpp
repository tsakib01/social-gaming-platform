#include <gtest/gtest.h>
#include "OutgoingMessages.h"

const auto TestMessage = "test";

TEST(OutgoingMessagesTests, TestNoUsers) {
    auto outgoing = OutgoingMessages{{}};
    auto messages = outgoing.getMessages();
    ASSERT_EQ(messages.size(), 0);

    EXPECT_THROW(outgoing.setMessage({1}, TestMessage), std::runtime_error);
}

TEST(OutgoingMessagesTests, TestSetMessageAmongMultipleUsers) {
    auto users = std::vector<UserId>{{1}, {2}, {3}};
    auto outgoing = OutgoingMessages{users};
    auto messages = outgoing.getMessages();

    ASSERT_EQ(messages.size(), 3);
    for(auto& [user, message] : messages)
        ASSERT_EQ(message, "");
    
    auto msgUser = UserId{1};
    outgoing.setMessage(msgUser, TestMessage);
    messages = outgoing.getMessages();

    ASSERT_EQ(messages.size(), 3);
    ASSERT_EQ(messages[msgUser], TestMessage);
    messages.erase(msgUser);
    for(auto& [user, message] : messages)
        ASSERT_EQ(message, "");
}

TEST(OutgoingMessagesTests, TestSetAllMessages) {
    auto users = std::vector<UserId>{{1}, {2}, {3}};
    auto outgoing = OutgoingMessages{users};

    outgoing.setMessageForAll(TestMessage);
    auto messages = outgoing.getMessages();

    ASSERT_EQ(messages.size(), 3);
    for(auto& [user, message] : messages)
        ASSERT_EQ(message, TestMessage);
}

TEST(OutgoingMessagesTests, TestClearMessages) {
    auto users = std::vector<UserId>{{1}, {2}, {3}};
    auto outgoing = OutgoingMessages{users};

    outgoing.setMessageForAll(TestMessage);
    outgoing.clear();
    auto messages = outgoing.getMessages();

    ASSERT_EQ(messages.size(), 3);
    for(auto& [user, message] : messages)
        ASSERT_EQ(message, "");
}
