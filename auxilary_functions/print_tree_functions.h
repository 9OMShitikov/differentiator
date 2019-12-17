//
// Created by oleg on 23.11.2019.
//

#ifndef DIFFERENTIATOR_PRINT_TREE_FUNCTIONS_H
#include "node_structure.h"
void add_node (char** buff, tree_node* node_ptr, ssize_t ind, size_t& size, size_t& taken,
        operators_definitions* op_defs,functions_definitions* func_defs, AutoFree<variable> * variables) {
    ASSERT(buff);
    ASSERT(node_ptr);
    add_string_to_buff(buff, size, taken, "v");
    add_int_to_buff(buff, size, taken, ind);

    //add_string_to_buff(buff, size, taken, node_ptr->name);
    switch (node_ptr->type) {
        case operator_node: {
            add_string_to_buff(buff, size, taken, " [label = \"");
            add_string_to_buff(buff, size, taken, op_defs->operators_names[node_ptr->index]);
            add_string_to_buff(buff, size, taken, "\", style = filled, color = aliceblue]\n");
        }
            break;
        case function_node: {
            add_string_to_buff(buff, size, taken, " [label = \"");
            add_string_to_buff(buff, size, taken, func_defs->functions_names[node_ptr->index]);
            add_string_to_buff(buff, size, taken, "\", style = filled, color = antiquewhite]\n");
        }
            break;
        case variable_node: {
            add_string_to_buff(buff, size, taken, " [label = \"");
            add_string_to_buff(buff, size, taken, (variables->ptr[node_ptr->index]).name);
            add_string_to_buff(buff, size, taken, "\", style = filled, color = chartreuse]\n");
        }
            break;
        case constant_node: {
            char w[128];
            snprintf(w, 128, "%lf", node_ptr->value);
            add_string_to_buff(buff, size, taken, " [label = \"");

            add_string_to_buff(buff, size, taken, w);
            add_string_to_buff(buff, size, taken, "\", style = filled, color = coral1]\n");
        }
            break;
        case empty_node: {
            add_string_to_buff(buff, size, taken, " [label = \"\", style = filled, color = ghostwhite]\n");
        }
            break;
    }
}

void add_edges (char** buff, tree_node* node_ptr, ssize_t ind, size_t& size, size_t& taken) {
    ASSERT(buff);
    ASSERT(node_ptr);
    if (node_ptr->type != variable_node) {
        if (node_ptr->first_link >= 0) {
            add_string_to_buff(buff, size, taken, "v");
            add_int_to_buff(buff, size, taken, ind);
            add_string_to_buff(buff, size, taken, "->v");
            add_int_to_buff(buff, size, taken, node_ptr->first_link);
            add_string_to_buff(buff, size, taken, "\n");
        }

        if (node_ptr->second_link >= 0) {
            add_string_to_buff(buff, size, taken, "v");
            add_int_to_buff(buff, size, taken, ind);
            add_string_to_buff(buff, size, taken, "->v");
            add_int_to_buff(buff, size, taken, node_ptr->second_link);
            add_string_to_buff(buff, size, taken, "\n");
        }

        if (node_ptr->second_link >= 0 && node_ptr->first_link >= 0) {
            add_string_to_buff(buff, size, taken, "{rank = same; ");
            add_string_to_buff(buff, size, taken, "v");
            add_int_to_buff(buff, size, taken, node_ptr->first_link);
            add_string_to_buff(buff, size, taken, ", ");
            add_string_to_buff(buff, size, taken, "v");
            add_int_to_buff(buff, size, taken, node_ptr->second_link);
            add_string_to_buff(buff, size, taken, "}\n");
        }
    }
}
#define DIFFERENTIATOR_PRINT_TREE_FUNCTIONS_H

#endif //AKINATOR_DEV_PRINT_TREE_FUNCTIONS_H
