#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include "strings_read_functions.h"
#include "bor.h"
#include "my_stack.h"
#include "my_stack.cpp"
#include "node_structure.h"
#include "info.h"
#include "print_tree_functions.h"

#include "tree_process_functions.h"

int main(int argc, char* argv[]) {
    const char *input_file = nullptr;
    if (argc > 1) input_file = argv[1];
    else input_file = "result.txt";

    const char *output_file = nullptr;
    if (argc > 2) output_file = argv[2];
    else output_file = "tree_graph.dot";

    expression_tree tree;
    AutoFree<char> str_buff;

    tree.read_tree(input_file);

    int buff_taken = print_dot_tree(str_buff, tree);

    clear_output(output_file);
    print_bin(buff_taken, &str_buff.ptr, output_file);

    char str[256];
    sprintf(str, "dot -O -Tpng %s", output_file);
    system(str);
}
