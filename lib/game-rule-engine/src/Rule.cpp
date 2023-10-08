#include "Rule.h"
#include <iostream>

void ForLoopRule::execute() {
  std::cout << "Executing for loop\n";
}

void MatchRule::execute() {
  std::cout << "Executing match\n";
}