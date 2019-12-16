//
// Created by oleg on 23.11.2019.
//

#ifndef AKINATOR_DEV_PRINT_TREE_FUNCTIONS_H
#include "akinator_functions.h"
void add_node (char** buff, tree_node* node_ptr, ssize_t ind, size_t& size, size_t& taken) {
    ASSERT(buff);
    ASSERT(node_ptr);
    add_string_to_buff(buff, size, taken, "v");
    add_int_to_buff(buff, size, taken, ind);
    add_string_to_buff(buff, size, taken, " [label = \"");
    add_string_to_buff(buff, size, taken, node_ptr->name);
    if (node_ptr->type == question) {
        add_string_to_buff(buff, size, taken, "?\", style = filled, color = powderblue]\n");
    }
    else {
        add_string_to_buff(buff, size, taken, "\", style = filled, color = orangered]\n");
    }
}

void add_edges (char** buff, tree_node* node_ptr, ssize_t ind, size_t& size, size_t& taken) {
    ASSERT(buff);
    ASSERT(node_ptr);
    if (node_ptr->type == question) {
        add_string_to_buff(buff, size, taken, "v");
        add_int_to_buff(buff, size, taken, ind);
        add_string_to_buff(buff, size, taken, "->v");
        add_int_to_buff(buff, size, taken, node_ptr->pos_no);
        add_string_to_buff(buff, size, taken, " [label = \"No\"]\n");

        add_string_to_buff(buff, size, taken, "v");
        add_int_to_buff(buff, size, taken, ind);
        add_string_to_buff(buff, size, taken, "->v");
        add_int_to_buff(buff, size, taken, node_ptr->pos_yes);
        add_string_to_buff(buff, size, taken, " [label = \"Yes\"]\n");

        add_string_to_buff(buff, size, taken, "{rank = same; ");
        add_string_to_buff(buff, size, taken, "v");
        add_int_to_buff(buff, size, taken, node_ptr->pos_no);
        add_string_to_buff(buff, size, taken, ", ");
        add_string_to_buff(buff, size, taken, "v");
        add_int_to_buff(buff, size, taken, node_ptr->pos_yes);
        add_string_to_buff(buff, size, taken, "}\n");
    }
}
#define AKINATOR_DEV_PRINT_TREE_FUNCTIONS_H

#endif //AKINATOR_DEV_PRINT_TREE_FUNCTIONS_H
