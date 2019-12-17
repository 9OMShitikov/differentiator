#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include "strings_read_functions.h"
#include "bor.cpp"
#include "my_stack.h"
#include "my_stack.cpp"
#include "node_structure.h"
#include "info.h"
#include "print_tree_functions.h"

int main(int argc, char* argv[]) {
    const char *input_file = nullptr;
    if (argc > 1) input_file = argv[1];
    else input_file = "result.txt";

    const char *output_file = nullptr;
    if (argc > 2) output_file = argv[2];
    else output_file = "tree_graph.dot";

    expression_tree tree;

    operators_definitions op_defs;
    functions_definitions func_defs;

    AutoFree<char> str_buff;
    str_buff.ptr = new char(0);
    size_t buff_size = 1;
    size_t buff_taken = 0;

   tree.read_tree(input_file);

    add_string_to_buff(&(str_buff.ptr), buff_size, buff_taken, "digraph expression_tree {\n");

    for (int i = 0; i < tree.nodes_count; ++i) {
        add_node(&(str_buff.ptr), tree.tree_nodes.ptr + i, i, buff_size, buff_taken, &op_defs, &func_defs, &tree.variables);
    }
    for (int i = 0; i < tree.nodes_count; ++i) {
        add_edges(&(str_buff.ptr), tree.tree_nodes.ptr + i, i, buff_size, buff_taken);
    }
    add_string_to_buff(&(str_buff.ptr), buff_size, buff_taken, "}\n");

    clear_output(output_file);
    print_bin(buff_taken, &str_buff.ptr, output_file);

    char str[256];
    sprintf(str, "dot -O -Tpng %s", output_file);
    system(str);
}
