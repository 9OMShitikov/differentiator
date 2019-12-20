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

#include "tree_process_functions.h"

int main(int argc, char* argv[]) {
    const char *input_file = nullptr;
    if (argc > 1) input_file = argv[1];
    else input_file = "result.txt";

    const char *output_file = nullptr;
    if (argc > 2) output_file = argv[2];
    else output_file = "diff_result.txt";

    expression_tree tree;
    tree.read_tree(input_file);

    printf("Variables: \n");
    bor variables_bor;
    for (int i = 0; i < tree.variables_count; ++i) {
        printf("%s\n", tree.variables.ptr[i].name);
        variables_bor.add( tree.variables.ptr[i].name, i);
    }
    printf("Please, enter a variable which you need\n");
    bool fl = 0;
    int k = -1;
    while (!fl) {
        char line[MAX_NAME_SIZE];
        get_line_with_endl(line);
        k = variables_bor.check(line);
        if (k != -1) fl = 1;
        else printf("Please, repeat\n");
    }

    expression_tree result;
    differentiate_tree(result, tree, k);
    result.print_tree(output_file);
}
