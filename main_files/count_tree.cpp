
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
#include "tree_print.h"
#include "tree_dfss.h"

int main(int argc, char* argv[]) {
    const char *input_file = nullptr;
    if (argc > 1) input_file = argv[1];
    else input_file = "result.txt";

    expression_tree tree;

    operators_definitions op_defs;
    functions_definitions func_defs;

    AutoFree<char> str_buff;
    str_buff.ptr = new char(0);
    size_t buff_size = 1;
    size_t buff_taken = 0;

    tree.read_tree(input_file);

    printf("Please, enter values of these variables:\n");
    for (int i = 0; i < tree.variables_count; ++i) {
        printf("%s = ", tree.variables.ptr[i].name);
        scanf("%lf", &tree.variables.ptr[i].value);
    }

    double answer = 0;
    dfs_count(tree, 0, answer, op_defs, func_defs);
    printf("Result is %lf\n", answer);
}