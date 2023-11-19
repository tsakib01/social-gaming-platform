#ifndef RULE_H
#define RULE_H

#include <string_view>
#include <vector>
#include <memory>
#include <variant>
#include <map>
#include "Expression.h"
#include "GameStateLoader.h"

class BodyRule;
class ForRule;
class MatchRule;
class DiscardRule;
class MessageRule;
class ParallelForRule;
class InputChoiceRule;
class ExtendRule;
class Rule;

class RuleVisitor {
public:
    virtual void visit(BodyRule& rule) = 0;
    virtual void visit(ForRule& rule) = 0;
    virtual void visit(MatchRule& rule) = 0;
    virtual void visit(DiscardRule& rule) = 0;
    virtual void visit(MessageRule& rule) = 0;
    virtual void visit(ParallelForRule& rule) = 0;
    virtual void visit(InputChoiceRule& rule) = 0;
    virtual void visit(ExtendRule& rule) = 0;
    virtual void visit(Rule& rule) = 0;
};

/// A rule interface that represents a control flow statement in the game config
class Rule {
public:
    virtual ~Rule() = default;
};

/// A rule that represents a series of rules in sequential order - e.x. the body of a for loop
class BodyRule : public Rule {
public:
    std::vector<std::unique_ptr<Rule>> rules;
};

/// A rule that represents a for loop executing a body rule for each item in a list
class ForRule : public Rule {
public:
    /// An expression that evaluates to the current item in the list
    IdentifierExpression currentItem;
    /// An expression that evaluates to the list that the for rule will iterate over.
    /// Use unique_ptr to hold a base Expression class to leverage polymorphism
    std::unique_ptr<Expression> list;
    BodyRule body; 
};

/// A rule that represents a pattern matching statement
class MatchRule : public Rule {
public:
    /// The target expression to match against
    std::unique_ptr<Expression> target;
    /// A map of the cases to match the target against.
    /// If a match is found, the corresponding body rule represents the path to take
    std::map<std::unique_ptr<Expression>, BodyRule> cases; 
};

/// A rule that removes << count >> items from a list << source >> 
class DiscardRule : public Rule {
public:
    std::unique_ptr<Expression> count;
    QualifiedIdentifier source;
};

/// A rule that represents a message to be sent to a list of players
class MessageRule : public Rule {
public:
    LiteralExpression<std::string_view> content;
    std::unique_ptr<Expression> players;
};

/// A rule that represents a for loop executing a body rule for each item in a list.
/// In contrast to the ForRule, the ParallelForRule runs all iterations in one execution unit of the game instance.
class ParallelForRule : public Rule {
public:
    /// An expression that evaluates to the current item in the list
    IdentifierExpression currentItem;
    /// An expression that evaluates to the list that the for rule will iterate over
    std::unique_ptr<Expression> list;
    BodyRule body;
};

/// A rule that represents a prompt for a choice styled input from a player
class InputChoiceRule : public Rule {
public:
    /// An IdentifierExpression that evaluates to the player to prompt for input
    IdentifierExpression to;
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
    QualifiedIdentifier target;
    /// The item to add
    std::unique_ptr<Expression> value;
};

// A rule that prints a scoreboard on the global display using the given attribute(s) of each player defined by the key list.
class ScoresRule : public Rule {
public:
    /// The list of keys
    std::unique_ptr<Expression> listOfKeys;
};

// Assigns value to target
class AssignmentRule : public Rule {
public:
    QualifiedIdentifier target;
    std::unique_ptr<Expression> value;
};

#endif