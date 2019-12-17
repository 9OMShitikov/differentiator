#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include "strings_read_functions.h"
#include "bor.cpp"
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

    expression_tree tree;
    tree.read_tree(input_file);

    operators_definitions op_defs;
    functions_definitions func_defs;

    my_stack<tree_node> new_tree;
    new_tree.push_back(tree_node());

    dfs_simplify(tree, new_tree, 0, 0, op_defs, func_defs);
    int nodes_count = new_tree.size();
    tree_print(tree.variables_count, nodes_count,  new_tree.get_ptr(), tree.variables.ptr, output_file);
}
