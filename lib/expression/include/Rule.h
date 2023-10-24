#ifndef RULE_H
#define RULE_H


#include <string_view>
#include <vector>
#include <memory>
#include <variant>
#include <map>
#include "Expresssion.h"

enum class InputType{
    CHOICE
};

class Rule{
public:
};

class BodyRule : Rule{
public:
    std::vector<Rule> rules;
};

class ForRule : public Rule{
public:
    // For player in playerList
    std::unique_ptr<IdentifierExpression> currentItem;
    std::unique_ptr<Expression> list;
    std::unique_ptr<BodyRule> body; 
};

class MatchRule : public Rule{
public:
    std::unique_ptr<Expression> target;
    std::map<std::unique_ptr<Expression>, std::unique_ptr<BodyRule>> cases; 
};

class DiscardRule : public Rule {
public:
    std::unique_ptr<Expression> index;
    std::unique_ptr<Expression> list;
};

class MessageRule : public Rule {
public:
    std::unique_ptr <Expression> message;
    std::unique_ptr<Expression> toList;
};

class ParallelForRule : public Rule {
public:
    // For player in playerList
    std::unique_ptr<IdentifierExpression> currentItem;
    std::unique_ptr<Expression> list;
    std::unique_ptr<BodyRule> body;
};

class InputRule : public Rule {
public:
    InputType inputType;
    std::unique_ptr<IdentifierExpression> to;
    std::unique_ptr<Expression> prompt;
    std::unique_ptr<Expression> choices;
    std::unique_ptr<Expression> target;
    std::unique_ptr<Expression> timeout;
};

class ExtendRule : public Rule {
public:
    std::unique_ptr<Expression> target;
    std::unique_ptr<Expression> source;
};

#endif