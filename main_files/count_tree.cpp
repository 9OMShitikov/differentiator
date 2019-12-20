
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

#include "tree_process_functions.h"

int main(int argc, char* argv[]) {
    const char *input_file = nullptr;
    if (argc > 1) input_file = argv[1];
    else input_file = "result.txt";

    expression_tree tree;
    tree.read_tree(input_file);

    double answer = count_tree(tree);
    printf("Result is %lf\n", answer);
}