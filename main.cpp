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

int main(int args, char* argv[]) {
    AutoFree<char> buff;
    const char* input_file;
    const char* output_file;

    if (args <= 1) input_file = "input.txt";
    else input_file = argv[1];

    if (args <= 2) output_file = "result.txt";
    else output_file = argv[2];

    int expr_size = read_file_with_free_place(&buff, input_file, 1);
    buff.ptr[expr_size] = '$';

    operators_definitions op_defs;
    functions_definitions func_defs;
    bor variables_bor;
    my_stack<tree_node> tree;
    char a[MAX_NAME_SIZE];
    char* variables_names[100];
    for (int i = 0; i < 100; ++i) variables_names[i] = new char[100];

    tree.push_back(tree_node());
    int s = read_simple_expression (a, op_defs, func_defs, variables_bor,  variables_names, tree, 0);
    ASSERT(s == expr_size);

    int variables_count = variables_bor.get_size();
    variable* variables = new variable [variables_count];
    for (int i = 0; i < variables_count; ++i) {
        strcpy(variables[i].name, variables_names[i]);
    }

    AutoFree<char> buff_print;
    buff_print.ptr = new char [sizeof(int) + variables_count * sizeof(variable) + sizeof(int) + tree.size() * sizeof(tree)];
    *reinterpret_cast<int*>(buff_print.ptr) = variables_count;
    memcpy(buff_print.ptr + sizeof(int), reinterpret_cast<char*> (variables), variables_count * sizeof(variable));
    *reinterpret_cast<int*>(buff_print.ptr + sizeof(int) + variables_count * sizeof(variable)) = tree.size();
    memcpy(buff_print.ptr + sizeof(int) + variables_count * sizeof(variable) + sizeof(int), reinterpret_cast<char*> (tree.get_ptr()), tree.size() * sizeof(tree_node));

    clear_output(output_file);
    print_buff(sizeof(int) + variables_count * sizeof(variable) + sizeof(int) + tree.size() * sizeof(tree),
            &buff_print, output_file);

    for (int i = 0; i < 100; ++i) delete(variables_names[i]);
    return 0;
}
