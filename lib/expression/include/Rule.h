#ifndef RULE_H
#define RULE_H


#include <string_view>
#include <vector>
#include <memory>
#include <variant>
#include <map>
#include "Expresssion.h"

/// The types of an input that can be prompted for
enum class InputType {
    CHOICE
};

/// A rule interface that represents a control flow statement in the game config
class Rule {};

/// A rule that represents a series of rules in sequential order - e.x. the body of a for loop
class BodyRule : Rule {
public:
    std::vector<Rule> rules;
};

/// A rule that represents a for loop executing a body rule for each item in a list
class ForRule : public Rule {
public:
    /// An expression that evaluates to the current item in the list
    std::unique_ptr<IdentifierExpression> currentItem;
    /// An expression that evaluates to the list that the for rule will iterate over
    std::unique_ptr<Expression> list;
    std::unique_ptr<BodyRule> body; 
};

/// A rule that represents a pattern matching statement
class MatchRule : public Rule {
public:
    /// The target expression to match against
    std::unique_ptr<Expression> target;
    /// A map of the cases to match the target against.
    /// If a match is found, the corresponding body rule represents the path to take
    std::map<std::unique_ptr<Expression>, std::unique_ptr<BodyRule>> cases; 
};

/// A rule that removes an item from a list at a given index
class DiscardRule : public Rule {
public:
    std::unique_ptr<Expression> index;
    std::unique_ptr<Expression> list;
};

/// A rule that represents a message to be sent to a list of players
class MessageRule : public Rule {
public:
    std::unique_ptr<Expression> message;
    /// An expression that evaluates to the list of players to message
    std::unique_ptr<Expression> toList;
};

/// A rule that represents a for loop executing a body rule for each item in a list.
/// In contrast to the ForRule, the ParallelForRule runs all iterations in one execution unit of the game instance.
class ParallelForRule : public Rule {
public:
    /// An expression that evaluates to the current item in the list
    std::unique_ptr<IdentifierExpression> currentItem;
    /// An expression that evaluates to the list that the for rule will iterate over
    std::unique_ptr<Expression> list;
    std::unique_ptr<BodyRule> body;
};

/// A rule that represents a prompt for input from a player
class InputRule : public Rule {
public:
    InputType inputType;
    /// An IdentifierExpression that evaluates to the player to prompt for input
    std::unique_ptr<IdentifierExpression> to;
    /// The prompt to display to the player
    std::unique_ptr<Expression> prompt;
    /// The choices the player can make
    std::unique_ptr<Expression> choices;
    /// The name of the player to prompt. (Not sure about this?)
    std::unique_ptr<Expression> target;
    /// The timeout of the prompt
    std::unique_ptr<Expression> timeout;
};

/// A rule that adds an item to a list
class ExtendRule : public Rule {
public:
    /// The list to add to
    std::unique_ptr<Expression> target;
    /// The item to add
    std::unique_ptr<Expression> source;
};

#endif