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
#include "tree_dfss.h"

int main(int argc, char* argv[]) {
    const char *input_file = nullptr;
    if (argc > 1) input_file = argv[1];
    else input_file = "result.txt";

    const char *output_file = nullptr;
    if (argc > 2) output_file = argv[2];
    else output_file = "latex_expr.txt";
    expression_tree tree;

    operators_definitions op_defs;
    functions_definitions func_defs;

    AutoFree<char> str_buff;
    str_buff.ptr = new char(0);
    size_t buff_size = 1;
    size_t buff_taken = 0;

    tree.read_tree(input_file);

    dfs_latex(tree, 0, str_buff, buff_size, buff_taken, op_defs, func_defs);
    clear_output(output_file);
    print_buff(buff_taken, &str_buff, output_file);
    return 0;
}
