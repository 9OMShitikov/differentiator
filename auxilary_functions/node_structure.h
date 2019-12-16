//
// Created by oleg on 16.12.2019.
//

#ifndef DIFFERENTIATOR_NODE_STRUCTURE_H
#define DIFFERENTIATOR_NODE_STRUCTURE_H
enum node_types {
    operator_node = 0,
    function_node = 1,
    variable_node = 2,
    constant_node = 3,
    empty_node = 0
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
    ~tree_node() = default;
};

#endif //DIFFERENTIATOR_NODE_STRUCTURE_H
