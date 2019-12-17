//
// Created by oleg on 16.12.2019.
//

#ifndef DIFFERENTIATOR_NODE_STRUCTURE_H

#include "strings_read_functions.h"
const int MAX_NAME_SIZE = 100;

enum node_types {
    empty_node = 0,
    function_node = 1,
    variable_node = 2,
    constant_node = 3,
    operator_node = 4
};

struct tree_node {
    node_types type;
    int index;
    int first_link;
    int second_link;
    double value;
    tree_node():
        type(node_types(0)),
        index(0),
        first_link(0),
        second_link(0),
        value(0) {
    }
    tree_node(node_types type_, int index_, int first_link_, int second_link_, double value_):
            type(type_),
            index(index_),
            first_link(first_link_),
            second_link(second_link_),
            value(value_) {
    }
    tree_node(double value_):
            type(constant_node),
            index(-1),
            first_link(-1),
            second_link(-1),
            value(value_) {
    }
    ~tree_node() = default;
};

struct variable {
    char name[MAX_NAME_SIZE];
    double value;
};

class expression_tree {
public:
    AutoFree<tree_node> tree_nodes;
    AutoFree<variable> variables;
    int variables_count;
    int nodes_count;

    expression_tree(const expression_tree&) =  delete;
    void operator=(const expression_tree&) = delete;
    bool read_tree(const char* input_file) {
        int tree_size = read_bin<char> (&tree, input_file);
        variables_count = *(reinterpret_cast<int*> (tree.ptr));
        nodes_count = *(reinterpret_cast<int*> (tree.ptr + 4 + variables_count*sizeof(variable)));
        variables.ptr = (reinterpret_cast<variable*> (tree.ptr + 4));
        tree_nodes.ptr = (reinterpret_cast<tree_node*> (tree.ptr + 4 + variables_count*sizeof(variable) + 4));
        return 0;
    }

    expression_tree(){
        tree.ptr = NULL;
        tree_nodes.ptr = NULL;
        variables.ptr = NULL;
        variables_count = 0;
        nodes_count = 0;
    }

    ~expression_tree(){
        tree_nodes.ptr = NULL;
        variables.ptr = NULL;
    }
private:
    AutoFree<char> tree;
};

#define DIFFERENTIATOR_NODE_STRUCTURE_H
#endif //DIFFERENTIATOR_NODE_STRUCTURE_H
