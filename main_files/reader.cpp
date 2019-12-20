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
#include "tree_print.h"

//void read_tree ( AutoFree<char>& buff)
int main(int args, char* argv[]) {
    AutoFree<char> buff;
    const char* input_file;
    const char* output_file;

    if (args <= 1) input_file = "input.txt";
    else input_file = argv[1];

    if (args <= 2) output_file = "result.txt";
    else output_file = argv[2];

    int expr_size = read_file_with_free_place(&buff, input_file, 2);
    expr_size --;
    buff.ptr[expr_size] = '$';
     buff.ptr[expr_size + 1] = '\0';
    operators_definitions op_defs;
    functions_definitions func_defs;
    bor variables_bor;
    my_stack<tree_node> tree;
    char* variables_names[100];
    for (int i = 0; i < 100; ++i) variables_names[i] = new char[100];
    tree.push_back(tree_node());
    int s = read_simple_expression (buff.ptr, op_defs, func_defs, variables_bor,  variables_names, tree, 0);
    ASSERT(s == expr_size);

    int nodes_count = tree.size();
    int variables_count = variables_bor.get_size();
    variable* variables = new variable [variables_count];
    for (int i = 0; i < variables_count; ++i) {
        strcpy(variables[i].name, variables_names[i]);
    }

    expression_tree my_tree, my_new_tree;
    my_tree.generate_tree(variables_count, nodes_count, tree.get_ptr(), variables);
    my_new_tree.generate_tree(my_tree.variables_count, my_tree.nodes_count, my_tree.tree_nodes.ptr,
            my_tree.variables.ptr);
    my_new_tree.copy_tree_nodes(my_tree.nodes_count, my_tree.tree_nodes.ptr);
    my_new_tree.print_tree(output_file);
    for (int i = 0; i < 100; ++i) delete(variables_names[i]);
    return 0;
}

