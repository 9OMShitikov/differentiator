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

    if (args <= 2) output_file = "referat.tex";
    else output_file = argv[2];

    int expr_size = read_file_with_free_place(&buff, input_file, 2);

    AutoFree<char> str_buff;
    str_buff.ptr = new char(0);
    size_t buff_size = 1;
    size_t buff_taken = 0;

    add_string_to_buff(&str_buff.ptr, buff_size, buff_taken,
"\\documentclass{article}\n\
\\usepackage[utf8]{inputenc}\n\
\\usepackage{amssymb,amsmath,amsthm}\n\
\\begin{document}\n\
\\par It's obvious that for \\[F = ");

    expression_tree my_tree;
    read_tree(buff, my_tree, expr_size);

    AutoFree<char> expr_buff;

    print_latex_tree(expr_buff, my_tree);

    add_string_to_buff(&str_buff.ptr, buff_size, buff_taken, expr_buff.ptr);

    add_string_to_buff(&str_buff.ptr, buff_size, buff_taken,"\\]\n");
    add_string_to_buff(&str_buff.ptr, buff_size, buff_taken,"\\begin{gather*}\n");

    expression_tree buff_diff_tree;
    expression_tree buff_simplify_tree;

    for (int i = 0; i < my_tree.variables_count; ++i) {
        add_string_to_buff(&str_buff.ptr, buff_size, buff_taken, "F_{");
        add_string_to_buff(&str_buff.ptr, buff_size, buff_taken, my_tree.variables.ptr[i].name);
        add_string_to_buff(&str_buff.ptr, buff_size, buff_taken, "} = ");

        differentiate_tree(buff_diff_tree, my_tree, i);
        simplify_tree(buff_simplify_tree, buff_diff_tree);

        free(expr_buff.ptr);
        print_latex_tree(expr_buff, buff_simplify_tree);

        add_string_to_buff(&str_buff.ptr, buff_size, buff_taken, expr_buff.ptr);

        add_string_to_buff(&str_buff.ptr, buff_size, buff_taken, "\\\\\n");
    }
    add_string_to_buff(&str_buff.ptr, buff_size, buff_taken,"\\end{gather*}\n");
    add_string_to_buff(&str_buff.ptr, buff_size, buff_taken, "\\end{document}\n");
    clear_output(output_file);
    print_buff(buff_taken, &str_buff, output_file);
    return 0;
}
