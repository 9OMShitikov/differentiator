//
// Created by oleg on 17.12.2019.
//

#ifndef PRINT_TREE_TREE_DFSS_H
#define PRINT_TREE_TREE_DFSS_H

#include "info.h"
#include "bor.h"
const double eps = 0.001;
bool is_equal_to (double a, double b) {
    return ((a - eps <= b) && (a + eps >= b));
}
bool is_zero (double a) {
    return is_equal_to(a, 0);
}

bool is_one (double a) {
    return is_equal_to(a, 1);
}

int dfs_count (expression_tree& tree, int cur_node, double& value, operators_definitions & op_defs, functions_definitions & func_defs) {
    ASSERT(cur_node >= 0);
    tree_node node = tree.tree_nodes.ptr[cur_node];
    switch (node.type) {
        case function_node: {
            ASSERT(node.index >= 0);
            ASSERT(node.first_link >= 0);
            ASSERT(node.second_link == -1);
            double first_value = 0;
            dfs_count(tree, node.first_link, first_value, op_defs, func_defs);
            value = func_defs.compute(functions(node.index), first_value);
        }
        break;
        case operator_node: {
            ASSERT(node.index >= 0);
            ASSERT(node.first_link >= 0);
            ASSERT(node.second_link >= 0);
            double first_value = 0, second_value = 0;
            dfs_count(tree, node.first_link, first_value, op_defs, func_defs);
            dfs_count(tree, node.second_link, second_value, op_defs, func_defs);
            value = op_defs.compute(operators(node.index), first_value, second_value);
        }
        break;
        case variable_node: {
            ASSERT(node.index >= 0);
            value = tree.variables.ptr[node.index].value;
        }
        break;
        case constant_node: {
            value = node.value;
        }
        break;
    }
    return 0;
}

int dfs_find (expression_tree& tree,
        int cur_node,
        operators_definitions & op_defs,
        functions_definitions & func_defs,
        int k) {
    ASSERT(cur_node >= 0);
    tree_node node = tree.tree_nodes.ptr[cur_node];
    int result = 0;
    if (node.type == variable_node)
        if (node.index == k)
            return 1;
    if (node.first_link != -1) {
        if (dfs_find(tree, node.first_link, op_defs, func_defs, k))
            result = 1;
    }
    if (node.second_link != -1) {
        if (dfs_find(tree, node.second_link, op_defs, func_defs, k))
            result = 1;
    }
    return result;
}

int dfs_copy (expression_tree& tree,
              my_stack<tree_node>& new_tree,
              int cur_tree_node,
              int cur_new_tree_node,
              operators_definitions & op_defs,
              functions_definitions & func_defs) {
    tree_node node = tree.tree_nodes.ptr[cur_tree_node];
    new_tree.get_ptr()[cur_new_tree_node] = node;
    if (node.first_link != -1) {
        new_tree.get_ptr()[cur_new_tree_node].first_link = new_tree.size();
        new_tree.push_back(tree_node());
        dfs_copy(tree, new_tree, node.first_link, new_tree.size() - 1, op_defs, func_defs);
    }
    if (node.second_link != -1) {
        new_tree.get_ptr()[cur_new_tree_node].second_link = new_tree.size();
        new_tree.push_back(tree_node());
        dfs_copy(tree, new_tree, node.second_link, new_tree.size() - 1, op_defs, func_defs);
    }
    return 0;
}


int dfs_differentiate (expression_tree& tree,
        my_stack<tree_node>& new_tree,
        int cur_tree_node,
        int cur_new_tree_node,
        operators_definitions & op_defs,
        functions_definitions & func_defs,
        int diff_var) {
    ASSERT(cur_tree_node >= 0);
    ASSERT(cur_new_tree_node >= 0);
    tree_node node = tree.tree_nodes.ptr[cur_tree_node];
    int last_size = new_tree.size();

    switch (node.type) {
        case function_node: {
            ASSERT(node.index >= 0);
            ASSERT(node.first_link >= 0);
            ASSERT(node.second_link == -1);
            bor cur_bor;
            const int x = 0;
            const int dx = 1;
            cur_bor.add("x", x);
            cur_bor.add("dx", dx);
            char*temp[100];
            for (int i = 0; i < 10; ++i) {temp[i] = new char[100]; temp[i][0] = 0;}
            int res = read_simple_expression(func_defs.function_differentials[node.index],
                    op_defs,
                    func_defs,
                    cur_bor,
                    temp,
                    new_tree,
                    cur_new_tree_node);
            int cur_size = new_tree.size();
            for (int i = last_size; i < cur_size; ++i) {
                if(new_tree.get_ptr()[i].type == variable_node) {
                    switch(new_tree.get_ptr()[i].index) {
                        case x: {
                            dfs_copy(tree, new_tree, node.first_link, i, op_defs, func_defs);
                        }
                        break;
                        case dx: {
                            if (!dfs_find(tree, node.first_link, op_defs, func_defs, diff_var)) {
                                new_tree.get_ptr()[i] = tree_node( 0);
                            }
                            else dfs_differentiate(tree, new_tree, node.first_link, i, op_defs, func_defs, diff_var);
                        }
                        break;
                    }
                }
            }
        }
            break;
        case operator_node: {
            bor cur_bor;
            const int x1 = 0;
            const int dx1 = 1;
            const int x2 = 2;
            const int dx2 = 3;
            cur_bor.add("x1", x1);
            cur_bor.add("dx1", dx1);
            cur_bor.add("x2", x2);
            cur_bor.add("dx2", dx2);
            char** temp = NULL;
            if (node.index == POW) {
                if (dfs_find(tree, node.second_link, op_defs, func_defs, diff_var)) {
                    node.index = POW1;
                }
            }
            int res = read_simple_expression(op_defs.operators_differentials[node.index],
                                             op_defs,
                                             func_defs,
                                             cur_bor,
                                             temp,
                                             new_tree,
                                             cur_new_tree_node);
            int cur_size = new_tree.size();
            for (int i = last_size; i < cur_size; ++i) {
                if(new_tree.get_ptr()[i].type == variable_node) {
                    switch(new_tree.get_ptr()[i].index) {
                        case x1: {
                            dfs_copy(tree, new_tree, node.first_link, i, op_defs, func_defs);
                        }
                        break;
                        case dx1: {
                            if (!dfs_find(tree, node.first_link, op_defs, func_defs, diff_var)) {
                                new_tree.get_ptr()[i] = tree_node( 0);
                            }
                            //printf("$%d\n", i);
                            else dfs_differentiate(tree, new_tree, node.first_link, i, op_defs, func_defs, diff_var);
                        }
                        break;
                        case x2: {
                            //printf("$%d\n", i);
                            dfs_copy(tree, new_tree, node.second_link, i, op_defs, func_defs);
                        }
                        break;
                        case dx2: {
                            if (!dfs_find(tree, node.second_link, op_defs, func_defs, diff_var)) {
                                new_tree.get_ptr()[i] = tree_node( 0);
                            }
                            else dfs_differentiate(tree, new_tree, node.second_link, i, op_defs, func_defs, diff_var);
                        }
                        break;
                    }
                }
            }
        }
            break;
        case variable_node: {
            //printf("%d %d\n", node.index, cur_new_tree_node);
            ASSERT(node.index >= 0);
            if (node.index == diff_var) {
                new_tree.get_ptr()[cur_new_tree_node] = tree_node(1);
                //printf("#%d\n", node.index);
            }
            else
                new_tree.get_ptr()[cur_new_tree_node] = tree_node(0);
        }
            break;
        case constant_node: {
            new_tree.get_ptr()[cur_new_tree_node] = tree_node( 0);
        }
            break;
    }
    return 0;
}

int dfs_simplify (expression_tree& tree,
                       my_stack<tree_node>& new_tree,
                       int cur_tree_node,
                       int cur_new_tree_node,
                       operators_definitions & op_defs,
                       functions_definitions & func_defs) {
    ASSERT(cur_tree_node >= 0);
    ASSERT(cur_new_tree_node >= 0);
    tree_node node = tree.tree_nodes.ptr[cur_tree_node];
    int last_size = new_tree.size();
    int ret_val = 0;
    switch (node.type) {
        case function_node: {
            ASSERT(node.index >= 0);
            ASSERT(node.first_link >= 0);
            ASSERT(node.second_link == -1);
            if (tree.tree_nodes.ptr[node.first_link].type == constant_node) {
                new_tree.get_ptr()[cur_new_tree_node] = tree_node(
                        func_defs.compute(functions(node.index), tree.tree_nodes.ptr[node.first_link].value));
                ret_val = 1;
            }
            else{
                new_tree.get_ptr()[cur_new_tree_node] = node;
                new_tree.get_ptr()[cur_new_tree_node].first_link = new_tree.size();
                new_tree.push_back(tree_node());
                if (dfs_simplify(tree,
                                new_tree,
                                node.first_link,
                                new_tree.size() - 1,
                                op_defs,
                                func_defs))
                    ret_val = 1;
            }
        }
            break;
        case operator_node: {
            bool fl = 0;
            if (tree.tree_nodes.ptr[node.first_link].type == constant_node)
            {
                if (tree.tree_nodes.ptr[node.second_link].type == constant_node)
                {
                    new_tree.get_ptr()[cur_new_tree_node] = tree_node(
                            op_defs.compute(operators(node.index),
                                    tree.tree_nodes.ptr[node.first_link].value,
                                    tree.tree_nodes.ptr[node.second_link].value));
                    fl = 1;
                    ret_val = 1;
                }
                else {
                    switch (node.index) {
                        case SUM:
                            {
                                if (is_zero(tree.tree_nodes.ptr[node.first_link].value)) {
                                   dfs_simplify(tree,
                                                     new_tree,
                                                     node.second_link,
                                                     cur_new_tree_node,
                                                     op_defs,
                                                     func_defs);
                                   fl = 1;
                                   ret_val = 1;
                                }
                            }
                            break;
                        case MUL:
                            {
                                if (is_one(tree.tree_nodes.ptr[node.first_link].value)) {
                                    dfs_simplify(tree,
                                                 new_tree,
                                                 node.second_link,
                                                 cur_new_tree_node,
                                                 op_defs,
                                                 func_defs);
                                    fl = 1;
                                    ret_val = 1;
                                }

                                if (is_zero(tree.tree_nodes.ptr[node.first_link].value)) {
                                    new_tree.get_ptr()[cur_new_tree_node] = tree_node(0);
                                    fl = 1;
                                    ret_val = 1;
                                }
                            }
                            break;
                        case DIV:
                            {
                                if (is_zero(tree.tree_nodes.ptr[node.first_link].value)) {
                                    new_tree.get_ptr()[cur_new_tree_node] = tree_node(0);
                                    fl = 1;
                                    ret_val = 1;
                                }
                            }
                            break;
                    }
                }
            }
            else if (tree.tree_nodes.ptr[node.second_link].type == constant_node) {
                switch (node.index) {
                    case SUM:
                    {
                        if (is_zero(tree.tree_nodes.ptr[node.second_link].value)) {
                            dfs_simplify(tree,
                                         new_tree,
                                         node.first_link,
                                         cur_new_tree_node,
                                         op_defs,
                                         func_defs);
                            fl = 1;
                            ret_val = 1;
                        }
                    }
                        break;
                    case SUB:
                    {
                        if (is_zero(tree.tree_nodes.ptr[node.second_link].value)) {
                            dfs_simplify(tree,
                                         new_tree,
                                         node.first_link,
                                         cur_new_tree_node,
                                         op_defs,
                                         func_defs);
                            fl = 1;
                            ret_val = 1;
                        }
                    }
                        break;
                    case MUL:
                    {
                        if (is_one(tree.tree_nodes.ptr[node.second_link].value)) {
                            dfs_simplify(tree,
                                         new_tree,
                                         node.first_link,
                                         cur_new_tree_node,
                                         op_defs,
                                         func_defs);
                            fl = 1;
                            ret_val = 1;
                        }

                        if (is_zero(tree.tree_nodes.ptr[node.second_link].value)) {
                            new_tree.get_ptr()[cur_new_tree_node] = tree_node(0);
                            fl = 1;
                            ret_val = 1;
                        }
                    }
                        break;
                    case DIV:
                    {
                        ASSERT(!is_zero(tree.tree_nodes.ptr[node.second_link].value));
                    }
                        break;
                }
            }
            if (!fl) {
                new_tree.get_ptr()[cur_new_tree_node] = node;
                new_tree.get_ptr()[cur_new_tree_node].first_link = new_tree.size();
                new_tree.push_back(tree_node());
                if (dfs_simplify(tree,
                        new_tree,
                        node.first_link,
                        new_tree.size() - 1,
                        op_defs,
                        func_defs))
                    ret_val = 1;
                new_tree.get_ptr()[cur_new_tree_node].second_link = new_tree.size();
                new_tree.push_back(tree_node());
                if (dfs_simplify(tree,
                        new_tree,
                        node.second_link,
                        new_tree.size() - 1,
                        op_defs, func_defs))
                    ret_val = 1;
            }
        }
            break;
        default: {
            new_tree.get_ptr()[cur_new_tree_node] = node;
        }
    }
    return ret_val;
}

int dfs_latex (expression_tree& tree, int cur_node, AutoFree<char>& str_buff, size_t& size, size_t& taken, operators_definitions & op_defs, functions_definitions & func_defs) {
    ASSERT(cur_node >= 0);
    tree_node node = tree.tree_nodes.ptr[cur_node];
    switch (node.type) {
        case function_node: {
            ASSERT(node.index >= 0);
            ASSERT(node.first_link >= 0);
            ASSERT(node.second_link == -1);
            double first_value = 0;
            add_string_to_buff(&(str_buff.ptr), size, taken, func_defs.functions_names[node.index]);
            add_string_to_buff(&(str_buff.ptr), size, taken, "\\left(");
            dfs_latex(tree, node.first_link, str_buff, size, taken, op_defs, func_defs);
            add_string_to_buff(&(str_buff.ptr), size, taken, "\\right)");
        }
            break;
        case operator_node: {
            ASSERT(node.index >= 0);
            ASSERT(node.first_link >= 0);
            ASSERT(node.second_link >= 0);
            switch (node.index) {
                case DIV: {
                    add_string_to_buff(&(str_buff.ptr), size, taken, " \\frac {");
                } break;
                default: {
                    add_string_to_buff(&(str_buff.ptr), size, taken, "\\left(");
                }
            }
            dfs_latex(tree, node.first_link, str_buff, size, taken, op_defs, func_defs);
            switch (node.index) {
                case DIV: {
                    add_string_to_buff(&(str_buff.ptr), size, taken, "} {");
                } break;
                case MUL: {
                    add_string_to_buff(&(str_buff.ptr), size, taken, " \\cdot ");
                } break;
                case SUM: {
                    add_string_to_buff(&(str_buff.ptr), size, taken, " + ");
                } break;
                case SUB: {
                    add_string_to_buff(&(str_buff.ptr), size, taken, " - ");
                }
            }
            dfs_latex(tree, node.second_link, str_buff, size, taken, op_defs, func_defs);
            switch (node.index) {
                case DIV: {
                    add_string_to_buff(&(str_buff.ptr), size, taken, "}");
                } break;
                default: {
                    add_string_to_buff(&(str_buff.ptr), size, taken, "\\right)");
                }
            }
        }
            break;
        case variable_node: {
            ASSERT(node.index >= 0);
            add_string_to_buff(&(str_buff.ptr), size, taken, tree.variables.ptr[node.index].name);
        }
            break;
        case constant_node: {
            char c[128];
            snprintf(c, 127, "%.2f", node.value);
            add_string_to_buff(&(str_buff.ptr), size, taken, c);
        }
            break;
    }
    return 0;
}
#endif //PRINT_TREE_TREE_DFSS_H
