#ifndef PRINT_TREE_LATEX_TREE_PROCESS_FUNCTIONS_H
#define PRINT_TREE_LATEX_TREE_PROCESS_FUNCTIONS_H

#include "expression_parser.h"
#include "tree_dfss.h"
#include "print_tree_functions.h"
void read_tree (AutoFree<char>& buff, expression_tree& my_tree, int expr_size) {
    expr_size --;
    buff.ptr[expr_size] = '$';
    buff.ptr[expr_size + 1] = '\0';
    operators_definitions op_defs;
    functions_definitions func_defs;
    bor variables_bor;
    my_stack<tree_node> tree;
    char* variables_names[100];
    for (int i = 0; i < 100; ++i) variables_names[i] = new char[100];
    tree.push_back(tree_node());
    int s = read_simple_expression (buff.ptr, op_defs, func_defs, variables_bor,  variables_names, tree, 0);
    ASSERT(s == expr_size);

    int nodes_count = tree.size();
    int variables_count = variables_bor.get_size();
    variable* variables = new variable [variables_count];
    for (int i = 0; i < variables_count; ++i) {
        strcpy(variables[i].name, variables_names[i]);
    }

    my_tree.generate_tree(variables_count, nodes_count, tree.get_ptr(), variables);
    for (int i = 0; i < 100; ++i) delete(variables_names[i]);
}

void differentiate_tree(expression_tree& dest, expression_tree& tree, int k) {
    operators_definitions op_defs;
    functions_definitions func_defs;

    my_stack<tree_node> new_tree;
    new_tree.push_back(tree_node());

    dfs_differentiate(tree, new_tree, 0, 0, op_defs, func_defs, k);

    dest.copy_tree(tree);
    dest.copy_tree_nodes(new_tree.size(),  new_tree.get_ptr());
}

void simplify_tree(expression_tree& dest, expression_tree& tree) {
    dest.copy_tree(tree);
    operators_definitions op_defs;
    functions_definitions func_defs;

    my_stack<tree_node> new_tree;
    new_tree.push_back(tree_node());
    int f = 1;
    while (f) {
        f = dfs_simplify(dest, new_tree, 0, 0, op_defs, func_defs);
        dest.copy_tree_nodes(new_tree.size(),  new_tree.get_ptr());
        while (new_tree.size() > 0) new_tree.pop_back();
        new_tree.push_back(tree_node());
    }
}

size_t print_dot_tree (AutoFree<char>& str_buff, expression_tree& tree) {
    operators_definitions op_defs;
    functions_definitions func_defs;

    str_buff.ptr = new char(0);
    size_t buff_size = 1;
    size_t buff_taken = 0;

    add_string_to_buff(&(str_buff.ptr), buff_size, buff_taken, "digraph expression_tree {\n");

    for (int i = 0; i < tree.nodes_count; ++i) {
        add_node(&(str_buff.ptr), tree.tree_nodes.ptr + i, i, buff_size, buff_taken, &op_defs, &func_defs, &tree.variables);
    }
    for (int i = 0; i < tree.nodes_count; ++i) {
        add_edges(&(str_buff.ptr), tree.tree_nodes.ptr + i, i, buff_size, buff_taken);
    }
    add_string_to_buff(&(str_buff.ptr), buff_size, buff_taken, "}\n");
    return buff_taken;
}

size_t print_latex_tree (AutoFree<char>& str_buff, expression_tree& tree) {
    operators_definitions op_defs;
    functions_definitions func_defs;

    str_buff.ptr = new char(0);
    size_t buff_size = 1;
    size_t buff_taken = 0;

    dfs_latex(tree, 0, str_buff, buff_size, buff_taken, op_defs, func_defs);

    return buff_taken;
}

double count_tree (expression_tree& tree) {
    operators_definitions op_defs;
    functions_definitions func_defs;

    AutoFree<char> str_buff;
    str_buff.ptr = new char(0);
    size_t buff_size = 1;
    size_t buff_taken = 0;


    printf("Please, enter values of these variables:\n");
    for (int i = 0; i < tree.variables_count; ++i) {
        printf("%s = ", tree.variables.ptr[i].name);
        scanf("%lf", &tree.variables.ptr[i].value);
    }

    double answer = 0;
    dfs_count(tree, 0, answer, op_defs, func_defs);
    return answer;
}
#endif //PRINT_TREE_LATEX_TREE_PROCESS_FUNCTIONS_H
