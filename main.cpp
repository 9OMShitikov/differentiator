#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include "auxilary_functions/strings_read_functions.h"
#include "auxilary_functions/bor.cpp"
#include "auxilary_functions/my_stack.h"
#include "auxilary_functions/my_stack.cpp"
#include "auxilary_functions/node_structure.h"

#include "auxilary_functions/expression_parser.h"

int main() {
    operators_definitions op_defs;
    functions_definitions func_defs;
    double variables[100];
    bor variables_bor;
    my_stack<tree_node> tree;
    char a[100];
    get_line_with_endl(a);
    double val = 0;
    tree.push_back(tree_node());
    int s = read_simple_expression (a, op_defs, func_defs, variables, variables_bor, tree, 0);
    printf("%d %f", s, val);
    return 0;
}
