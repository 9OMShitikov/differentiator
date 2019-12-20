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

    bool generate_tree(int& new_variables_count, int& new_nodes_count,
                    tree_node* new_tree_nodes, variable* new_variables) {
        if (new_variables_count > 0) ASSERT(new_variables);
        if (new_nodes_count > 0) ASSERT(new_tree_nodes);
        variables_count = new_variables_count;
        nodes_count = new_nodes_count;
        tree.ptr = new char[sizeof(int) + variables_count * sizeof(variable) + sizeof(int) +
                                  nodes_count * sizeof(tree_node)];
        variables.ptr = (reinterpret_cast<variable*> (tree.ptr + 4));
        tree_nodes.ptr = (reinterpret_cast<tree_node*> (tree.ptr + 4 + variables_count*sizeof(variable) + 4));

        *reinterpret_cast<int *>(tree.ptr) = variables_count;
        memcpy(variables.ptr, new_variables, variables_count * sizeof(variable));
        *reinterpret_cast<int *>(tree.ptr + sizeof(int) + variables_count * sizeof(variable)) = nodes_count;
        memcpy(tree_nodes.ptr, new_tree_nodes, nodes_count * sizeof(tree_node));
        return 0;
    }

    bool copy_tree_nodes(int new_nodes_count, tree_node* new_tree_nodes) {
        if (new_nodes_count > 0) ASSERT(new_tree_nodes);
        nodes_count = new_nodes_count;
        void* buff = tree.ptr;
        tree.ptr = (char*) realloc(buff, sizeof(int) + variables_count * sizeof(variable) + sizeof(int) +
                            nodes_count * sizeof(tree_node));
        variables.ptr = (reinterpret_cast<variable*> (tree.ptr + 4));
        tree_nodes.ptr = (reinterpret_cast<tree_node*> (tree.ptr + 4 + variables_count*sizeof(variable) + 4));

        *reinterpret_cast<int *>(tree.ptr + sizeof(int) + variables_count * sizeof(variable)) = nodes_count;
        memcpy(tree_nodes.ptr, new_tree_nodes, nodes_count * sizeof(tree_node));
        return 0;
    }

    bool copy_tree(const expression_tree& cp_tree) {
        variables.ptr = NULL;
        tree_nodes.ptr = NULL;
        delete(tree.ptr);
        variables_count = cp_tree.variables_count;
        nodes_count = cp_tree.nodes_count;
        tree.ptr = (char*) calloc(4 + 4 + variables_count*sizeof(variable) + nodes_count*sizeof(tree_node),
                sizeof(char));
        variables.ptr = (reinterpret_cast<variable*> (tree.ptr + 4));
        tree_nodes.ptr = (reinterpret_cast<tree_node*> (tree.ptr + 4 + variables_count*sizeof(variable) + 4));
        memcpy(tree.ptr, cp_tree.tree.ptr, 4 + 4 + variables_count*sizeof(variable) + nodes_count*sizeof(tree_node));
        return 0;
    }

    bool print_tree(const char* output_file) {
        ASSERT(output_file);
        print_bin<char> (4 + 4 + variables_count*sizeof(variable) + nodes_count*sizeof(tree_node),
                &tree.ptr, output_file);
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
        delete(tree.ptr);
        tree.ptr = NULL;
    }
private:
    AutoFree<char> tree;
};

#define DIFFERENTIATOR_NODE_STRUCTURE_H
#endif //DIFFERENTIATOR_NODE_STRUCTURE_H
