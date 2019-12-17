//
// Created by oleg on 17.12.2019.
//

#ifndef PRINT_TREE_TREE_PRINT_H
#define PRINT_TREE_TREE_PRINT_H

bool tree_print(int& variables_count, int& nodes_count,
        tree_node* tree, variable* variables, const char* output_file) {
    if (variables_count > 0) ASSERT(variables);
    if (nodes_count > 0) ASSERT(tree);
    AutoFree<char> buff_print;
    buff_print.ptr = new char[sizeof(int) + variables_count * sizeof(variable) + sizeof(int) +
                              nodes_count * sizeof(tree_node)];
    *reinterpret_cast<int *>(buff_print.ptr) = variables_count;
    memcpy(buff_print.ptr + sizeof(int), reinterpret_cast<char *> (variables), variables_count * sizeof(variable));
    *reinterpret_cast<int *>(buff_print.ptr + sizeof(int) + variables_count * sizeof(variable)) = nodes_count;
    memcpy(buff_print.ptr + sizeof(int) + variables_count * sizeof(variable) + sizeof(int),
           reinterpret_cast<char *> (tree), nodes_count * sizeof(tree_node));
    clear_output(output_file);
    print_bin(sizeof(int) + variables_count * sizeof(variable) + sizeof(int) + nodes_count * sizeof(tree_node),
              &buff_print.ptr, output_file);
}
#endif //PRINT_TREE_TREE_PRINT_H
