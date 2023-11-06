#include "GameCommunicator.h"

// game instance sends the game code (which is also the room Code) with the actions (which is a map with some key and list of options)
// assignAction stores the actions along with the roomCode in a vector
void 
GameCommunicator::assignAction(uint8_t roomCode, const std::map<std::string_view, std::vector<std::string_view>>& gameActions) {
    auto users = userManager->getUsersByRoomCode(roomCode);
    Actions currentGameActions;
    currentGameActions.roomCode = roomCode;
    currentGameActions.actions = gameActions;

    actions.push_back(currentGameActions);
}

// returns the available choices for the game inputs by looking up the roomCode first, then finding the actions for the game played in that particular game
std::vector<std::string_view> 
GameCommunicator::getOptionsForUser(const Message& message, std::string_view key) const {
    auto roomCode = userManager->getUserGameCode(message.connection);

    auto it = std::find_if(actions.begin(), actions.end(), [roomCode](const Actions& action) {
        return action.roomCode == roomCode;
    });

    if (it != actions.end()) {
        return it->actions.at(key);
    }

    return {};
}

// returns the valid current user input, which will be sent to the game instance in server manager
std::string_view
GameCommunicator::getCurrentUserInput(Connection userID) const {
    
    auto it = std::find_if(userInputs.begin(), userInputs.end(), [userID](const UserInput& userInput) {
        return userInput.userID == userID;
    });

    if (it != userInputs.end()) {
        return it->input;
    }

    return "";
}

// stores the current valid user input for a particular user
void 
GameCommunicator::storeCurrentUserInput(const Message& message) {

    auto it = std::find_if(userInputs.begin(), userInputs.end(), [message](const UserInput& userInput) {
        return userInput.userID == message.connection;
    });

    if (it != userInputs.end()) {
        it->input = message.text;
    }
}

