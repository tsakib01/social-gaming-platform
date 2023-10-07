#ifndef RULE__H
#define RULE__H

#include <vector>
#include <cpp-tree-sitter.h>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <string_view>

class IRule {
public:
    virtual void execute() = 0;
    virtual ~IRule() {}
};

class ForLoopRule : public IRule {
public:
    ForLoopRule() {
        std::cout << "For Loop Rule created" << std::endl;
    }

    void execute() override {};
};

class MatchRule : public IRule {
public:
  MatchRule() {
    std::cout << "Match Rule created" << std::endl;
  }

  void execute() override {};
};

// This allows a cleaner way to map from a string representing a rule in the 
// treesitter language to their corresponding Rule constructor
class RuleMap {
public:
  using MapType = std::unordered_map<std::string_view, std::function<IRule*()>>; 

  // Takes in a string representing a rule, and returns the appropriate Rule object
  static IRule* getRule(std::string_view ruleName) {
    if (m_map.find(ruleName) != m_map.end()) {
      return m_map[ruleName]();
    }
    return nullptr;
  };

private:
  // Static initializer for the m_map static variable
  static MapType generate() {
    MapType map;
    map["for"] = []() {return new ForLoopRule();};
    map["match"] = []() {return new MatchRule();};
    return map;
  }
  
  static inline MapType m_map{ generate() };
};

//TO-DO: create other types of rule expressions
#endif