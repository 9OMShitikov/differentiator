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

#include "tree_process_functions.h"

int main(int argc, char* argv[]) {
    const char *input_file = nullptr;
    if (argc > 1) input_file = argv[1];
    else input_file = "result.txt";

    const char *output_file = nullptr;
    if (argc > 2) output_file = argv[2];
    else output_file = "latex_expr.txt";
    expression_tree tree;
    tree.read_tree(input_file);

    AutoFree<char> str_buff;

    int buff_taken = print_latex_tree(str_buff, tree);
    clear_output(output_file);
    print_buff(buff_taken, &str_buff, output_file);
    return 0;
}
