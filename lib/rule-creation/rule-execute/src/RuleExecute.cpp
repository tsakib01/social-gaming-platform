#include "RuleExecute.h"
#include <iostream>

void RuleExecuteVisitor::visit(BodyRule& rule) {
    std::cout << "executing body rule" << std::endl;
    auto index = context.gameState.getValue(&rule);
    if (index < rule.rules.size()) {
        context.instructionStack.push(rule.rules.at(index).get());
        context.gameState.updateState(&rule, index + 1);
        return;
    }
    context.gameState.removeValue(&rule);
    context.instructionStack.pop();
}

void RuleExecuteVisitor::visit(ForRule& rule) {
    std::cout << "executing for rule" << std::endl;
    auto value = rule.list->accept(exprVisitor);
    auto list = std::get_if<std::unique_ptr<GameEnvironment::List>>(&value->value);

    auto handleList = [&](const GameEnvironment::Identifier& identifier) -> std::unique_ptr<GameEnvironment::List> {
        if (!context.gameState.hasValue(identifier)) {
            throw std::runtime_error("For rule doesn't have a list");
        }
        auto stateValue = context.gameState.getValue(identifier);
        auto* listVal = std::get_if<std::unique_ptr<GameEnvironment::List>>(&stateValue.value);
        if (!listVal) {
            throw std::runtime_error("Invalid list type in gameState");
        }
        return std::move(*listVal);
    };

    std::unique_ptr<GameEnvironment::List> currentListPtr;
    if (list) {
        currentListPtr = std::move(*list);
    } else {
        currentListPtr = handleList(std::get<GameEnvironment::Identifier>(value->value));
    }

    auto* currentList = currentListPtr.get();
    auto index = context.gameState.getValue(&rule);

    if (index < currentList->size()) {
        if (context.gameState.hasValue(rule.currentItem.identifier)) {
            context.gameState.updateState(rule.currentItem.identifier, std::move((*currentList)[index]));
        } else {
            context.gameState.addState(rule.currentItem.identifier, std::move((*currentList)[index]));
        }
        context.instructionStack.push(&rule.body);
        context.gameState.updateState(&rule, index + 1);
    } else {
        context.gameState.removeValue(&rule);
        context.instructionStack.pop();
    }
}

void RuleExecuteVisitor::visit(MatchRule& rule) {
    (void)rule;
    std::cout << "executing match rule" << std::endl;
    context.instructionStack.pop();
}

void RuleExecuteVisitor::visit(DiscardRule& rule) {
    (void)rule;
    std::cout << "executing discard rule" << std::endl;
    context.instructionStack.pop();
}

void RuleExecuteVisitor::visit(MessageRule& rule) {
    std::cout << "executing message rule" << std::endl;

    auto content = rule.content->accept(exprVisitor);
    auto message = std::get_if<std::string_view>(&content->value);
    if(!message)
        throw new std::runtime_error("Invalid message content");

    auto players = rule.players->accept(exprVisitor);
    
    auto playerDesc = std::get_if<std::string_view>(&players->value);
    if(playerDesc) {
        if(playerDesc->compare(ALL_PLAYERS) != 0)
            throw new std::runtime_error("Unknown player description");
        
        context.outgoingMessages.setMessageForAll(*message);
    }
    else if(std::holds_alternative<std::unique_ptr<GameEnvironment::List>>(players->value)) {
        throw new std::runtime_error("Unimplemented feature: message rule with non-all players");        
    }

    context.instructionStack.pop();
}

void RuleExecuteVisitor::visit(ParallelForRule& rule) {
    std::cout << "executing parallel for rule" << std::endl;
    auto value = rule.list->accept(exprVisitor);
    auto list = std::get_if<std::unique_ptr<GameEnvironment::List>>(&value->value);

    auto handleList = [&](const GameEnvironment::Identifier& identifier) -> std::unique_ptr<GameEnvironment::List> {
        if (!context.gameState.hasValue(identifier)) {
            throw std::runtime_error("parallel for rule doesn't have a list");
        }
        auto stateValue = context.gameState.getValue(identifier);
        auto* listVal = std::get_if<std::unique_ptr<GameEnvironment::List>>(&stateValue.value);
        if (!listVal) {
            throw std::runtime_error("Invalid list type in gameState");
        }
        return std::move(*listVal);
    };

    std::unique_ptr<GameEnvironment::List> currentListPtr;
    if (list) {
        currentListPtr = std::move(*list);
    } else {
        currentListPtr = handleList(std::get<GameEnvironment::Identifier>(value->value));
    }

    auto* currentList = currentListPtr.get();
    auto index = context.gameState.getValue(&rule);

    if (index < currentList->size()) {
        if (context.gameState.hasValue(rule.currentItem.identifier)) {
            context.gameState.updateState(rule.currentItem.identifier, std::move((*currentList)[index]));
        } else {
            context.gameState.addState(rule.currentItem.identifier, std::move((*currentList)[index]));
        }
        context.instructionStack.push(&rule.body);
        context.gameState.updateState(&rule, index + 1);
    } else {
        context.gameState.removeValue(&rule);
        context.instructionStack.pop();
    }
}

void RuleExecuteVisitor::visit(InputChoiceRule& rule) {
    (void)rule;
    std::cout << "executing input rule" << std::endl;
    context.instructionStack.pop();
}

void RuleExecuteVisitor::visit(ExtendRule& rule) {
    (void)rule;
    std::cout << "executing extend rule" << std::endl;
    context.instructionStack.pop();
}

void RuleExecuteVisitor::visit(ScoresRule& rule) {
    (void)rule;
    std::cout << "executing scores rule" << std::endl;
    // Pop while unimplemented
    context.instructionStack.pop();
}

void RuleExecuteVisitor::visit([[maybe_unused]] Rule& rule) {
    throw std::runtime_error("Case Unreachable");
}
