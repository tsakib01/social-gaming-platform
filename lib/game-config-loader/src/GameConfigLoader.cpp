#include <cpp-tree-sitter.h>
#include <iostream>
#include <string_view>

extern "C" {
    TSLanguage* tree_sitter_json();
}

void printNode(ts::Node node) {
    std::cout <<
        "\ttype - " << node.getType() << "\n" <<
        "\tis named - " << (node.isNamed() ? "yes" : "no") << "\n" <<
        "\tnum children - " << node.getNumChildren() << "\n" <<
        "\tnum named children - " << node.getNumNamedChildren() << "\n";
}

void printTestConfig() {
    ts::Language language = tree_sitter_json();
    ts::Parser parser{language};

    std::string input = "[1, null]";
    ts::Tree tree = parser.parseString(input);

    ts::Node root = tree.getRootNode();

    ts::Node array = root.getNamedChild(0);
    ts::Node number = array.getNamedChild(0);

    std::cout << "Root node:\n";
    printNode(root);
    std::cout << "Array node:\n";
    printNode(array);
    std::cout << "Number node:\n";
    printNode(number);

    std::cout << "Whole tree:\n" << root.getSExpr().get() << "\n";
}