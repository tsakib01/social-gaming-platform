#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstdio>
#include <memory>
#include <string>
#include <cpp-tree-sitter.h>

extern "C" {
TSLanguage* tree_sitter_socialgaming();
}

void printContents(ts::Node node, std::string_view src) {
  ts::Extent<uint32_t> byteRange{node.getByteRange()};
  std::cout << src.substr(byteRange.start, byteRange.end - byteRange.start) << '\n';
}

void printNamedChildren(ts::Node root) {
  for (int i{0}; i < root.getNumNamedChildren(); ++i) {
    ts::Node node{root.getNamedChild(i)};
    std::cout << node.getType() << '\n';
    std::cout << node.getParent().getType() << '\n';
  }
}

void printChildren(ts::Node root) {
  for (int i{0}; i < root.getNumChildren(); ++i) {
    std::cout << root.getChild(i).getType() << '\n';
  }
}

void printChildrenDecoded(ts::Node root, std::string_view src) {
  ts::Node child{root.getChild(0)};
  bool hasContent{false};
  while (!child.isNull()) {
    if (hasContent) {
      printContents(child, src);      
    } else {
      std::cout << child.getType() << '\n';
    }

    if (child.getType().ends_with(":")) {
      hasContent = true;
    } else {
      hasContent = false;
    }

    child = child.getNextSibling();
  }
}

void printRec(ts::Node root) {
  if (root.isNull() || !root.isNamed()) return;
  std::cout << root.getType() << " ";
  printRec(root.getNamedChild(0));
  printRec(root.getNextSibling());
}


int main() {
  // Create a language and parser.
  ts::Language language = tree_sitter_socialgaming();
  ts::Parser parser{language};

  // Open file containing game spec
  std::ifstream ifs("games/rock-paper-scissors.game");
  std::stringstream buffer;
  buffer << ifs.rdbuf();
  std::string sourcecode = buffer.str();
  ifs.close();

  // std::cout << sourcecode << '\n';

  // Parse the provided string into a syntax tree.
  ts::Tree tree = parser.parseString(sourcecode);

  // Get the root node of the syntax tree. 
  ts::Node root = tree.getRootNode();
  auto numChildren = root.getNumNamedChildren();

  for (int i{0}; i < numChildren; ++i) {
    ts::Node node{root.getNamedChild(i)};
    std::cout << node.getType() << '\n';
  }

  ts::Node first{root.getNamedChild(2)};
  ts::Extent<ts::Point> point{first.getPointRange()};
  std::cout << point.start.row << ", " << point.start.column << '\n';
  std::cout << point.end.row << ", " << point.end.column << '\n';

  std::cout << root.getNumNamedChildren() << '\n'; 
  std::cout << first.getType() << '\n';
  std::cout << first.getNumNamedChildren() << '\n';

  std::cout << "=====\n";
  // need a function that prints all children of a subtree

  ts::Node configuration = root.getChildByFieldName("configuration");
  // printNamedChildren(configuration);
  ts::Node setup_rule = configuration.getNamedChild(3);
  std::cout << setup_rule.getType() << '\n';
  std::cout << setup_rule.getNumChildren() << '\n';
  printNamedChildren(setup_rule);
  std::cout << "=====\n";
  printChildren(configuration);
  std::cout << "=====\n";
  // printChildren(setup_rule, sourcecode);
  // ts::Node childByName{setup_rule.getChildByFieldName("name")};
  // std::cout << childByName.getType() << '\n';
  // printContents(childByName, sourcecode);
  // for (int i{0}; i < configuration.getNumChildren(); ++i) {
  //   std::cout << configuration.getChild(i).getType() << '\n';
  // }

  std::cout << configuration.getNumChildren() << '\n';
  printChildrenDecoded(configuration, sourcecode);
  printChildrenDecoded(setup_rule, sourcecode);
  std::cout << "=====\n";
  printContents(setup_rule.getChild(0), sourcecode);

  // for (int i{0}; i < root.getNumNamedChildren(); ++i) {
  //   printChildrenDecoded(root.getNamedChild(i), sourcecode);
  // }

  std::cout << "=====\n";
  // printRec(root);

  // Print the syntax tree as an S-expression.
  // auto treestring = root.getSExpr();
  // printf("Syntax tree: %s\n", treestring.get());

  return 0;
}