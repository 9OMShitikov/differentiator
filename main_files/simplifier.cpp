#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include "strings_read_functions.h"
#include "bor.h"
#include "my_stack.h"
#include "my_stack.cpp"
#include "node_structure.h"
#include "expression_parser.h"
#include "info.h"
#include "print_tree_functions.h"
#include "tree_print.h"
#include "tree_dfss.h"

int main(int argc, char* argv[]) {
    const char *input_file = nullptr;
    if (argc > 1) input_file = argv[1];
    else input_file = "result.txt";

    const char *output_file = nullptr;
    if (argc > 2) output_file = argv[2];
    else output_file = "simplify_result.txt";

    expression_tree tree, buff_tree;
    tree.read_tree(input_file);
    buff_tree.copy_tree(tree);
    operators_definitions op_defs;
    functions_definitions func_defs;

    my_stack<tree_node> new_tree;
    new_tree.push_back(tree_node());
    int f = 1;
    while (f) {
        f = dfs_simplify(buff_tree, new_tree, 0, 0, op_defs, func_defs);
        buff_tree.copy_tree_nodes(new_tree.size(),  new_tree.get_ptr());
        while (new_tree.size() > 0) new_tree.pop_back();
        new_tree.push_back(tree_node());
    }
    buff_tree.print_tree(output_file);
}
