#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include "strings_read_functions.h"
#include "bor.h"
#include "my_stack.h"
#include "my_stack.cpp"
#include "node_structure.h"

#include "tree_process_functions.h"

int main(int args, char* argv[]) {
    AutoFree<char> buff;
    const char* input_file;
    const char* output_file;

    if (args <= 1) input_file = "input.txt";
    else input_file = argv[1];

    if (args <= 2) output_file = "result.txt";
    else output_file = argv[2];

    int expr_size = read_file_with_free_place(&buff, input_file, 2);

    expression_tree my_tree;
    read_tree(buff, my_tree, expr_size);
    my_tree.print_tree(output_file);
    return 0;
}

