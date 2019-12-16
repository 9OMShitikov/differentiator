//
// Created by oleg on 23.11.2019.
//

#ifndef AKINATOR_DEV_AKINATOR_FUNCTIONS_H
#include "strings_read_functions.h"
#include "my_stack.h"
#include "my_stack.cpp"

bool check_cmd (char* cmd, bool& use_audio) {
    ASSERT(cmd);
    if (strcmp(cmd, "-EA") == 0) {
        use_audio = 1;
        return 1;
    }
    else if (strcmp(cmd, "-DA") == 0) {
        use_audio = 0;
        return 1;
    }
    else
        return 0;
}

bool check_name (tree_node* tree, int pos, size_t size, int& last_ans, bool& use_audio) {
    ASSERT(tree);
    ASSERT(pos < size);
    ASSERT(pos >= 0);
    tree[pos].name[0] = toupper(tree[pos].name[0]);
    char buff[512];
    sprintf(buff, "%s%c", tree[pos].name, '?');
    say_and_print(buff, use_audio);
    say_and_print("Answer \"N\" if no, \"Y\" if yes.", use_audio);
    if (tree[pos].type == question)
        say_and_print("If probably yes, print \"PY\", if probably no, print \"PN\"", use_audio);
    char ans[100];

    if (tree[pos].type == question) {

        while (true) {
            scanf("%s", ans);
            if (check_cmd(ans, use_audio)) {}
            else if (strcmp(ans, "Y") == 0) {
                return check_name(tree, tree[pos].pos_yes, size, last_ans, use_audio);
            } else if (strcmp(ans, "N") == 0) {
                return check_name(tree, tree[pos].pos_no, size, last_ans, use_audio);
            } else if (strcmp(ans, "PN") == 0) {
                bool fl = check_name(tree, tree[pos].pos_no, size, last_ans, use_audio);
                if (!fl) {
                    int tmp;
                    fl = check_name(tree, tree[pos].pos_yes, size, tmp, use_audio);
                }
                return fl;
            } else if (strcmp(ans, "PY") == 0) {
                bool fl = check_name(tree, tree[pos].pos_yes, size, last_ans, use_audio);
                if (!fl) {
                    int tmp;
                    fl = check_name(tree, tree[pos].pos_no, size, tmp, use_audio);
                }
                return fl;
            } else {
                say_and_print("Please, answer correctly.", use_audio);
            }
        }
    }
    else if (tree[pos].type == result) {
        while (true) {
            scanf("%s", ans);
            last_ans = pos;
            if (check_cmd(ans, use_audio)) {}
            else if (strcmp(ans, "Y") == 0) {
                return true;
            } else if (strcmp(ans, "N") == 0) {
                return false;
            } else {
                say_and_print("Please, answer correctly.\n", use_audio);
            }
        }
    }
    else {
        std::cerr<<"What???";
        throw std::exception();
    }
}

void add_name(tree_node* tree, size_t& size, int split_pos, bool& use_audio) {
    ASSERT(tree);
    ASSERT(split_pos >= 0);
    ASSERT(split_pos < size);

    new (tree + size) tree_node;
    new (tree + (size + 1)) tree_node;
    strcpy(tree[size].name, tree[split_pos].name);
    tree[split_pos].type = question;
    tree[split_pos].pos_no = size;
    tree[split_pos].pos_yes = size + 1;

    say_and_print("What was it?", use_audio);
    do get_line_with_endl(tree[size + 1].name);
    while (check_cmd(tree[size + 1].name, use_audio));

    tree[size + 1].name[0] = tolower(tree[size + 1].name[0]);
    tree[size].name[0] = tolower(tree[size].name[0]);

    char buff[512];
    sprintf(buff, "Print, in what %s is different from %s?(Avoid no, not, etc.)", tree[size + 1].name, tree[size].name);
    say_and_print(buff, use_audio);
    do get_line_with_endl(tree[split_pos].name);
    while (check_cmd(tree[split_pos].name, use_audio));

    tree[split_pos].name[0] = toupper(tree[split_pos].name[0]);
    size += 2;
}

bool find_elem (tree_node* tree, int pos, char* ans, my_stack <int>& nodes, my_stack <int>& moves) {
    if (tree[pos].type == result) {
        tree[pos].name[0] = toupper(tree[pos].name[0]);
        ans[0] = toupper(ans[0]);
        if (strcmp(ans, tree[pos].name) == 0) return 1;
        else return 0;
    }
    else {
        nodes.push_back(tree[pos].pos_no);
        moves.push_back(0);
        if (find_elem(tree, tree[pos].pos_no, ans, nodes, moves)) return 1;
        nodes.pop_back();
        moves.pop_back();
        nodes.push_back(tree[pos].pos_yes);
        moves.push_back(1);
        if (find_elem(tree, tree[pos].pos_yes, ans, nodes, moves)) return 1;
        nodes.pop_back();
        moves.pop_back();
        return 0;
    }
}

void print_description(tree_node* tree, bool& use_audio) {
    ASSERT(tree);
    say_and_print("Print name, please.", use_audio);

    char ans[128];
    do get_line_with_endl(ans);
    while (check_cmd(ans, use_audio));

    my_stack <int> nodes;
    nodes.push_back(0);
    my_stack <int> moves;

    if (!find_elem(tree, 0, ans, nodes, moves)) {
        say_and_print("No such element\n", use_audio);
    }
    else {
        char* buff = new char (0);
        size_t buff_size = 1;
        size_t buff_taken = 0;
        for (int i = 0; i < moves.size(); ++i) {
            if (moves.get_ptr()[i] == 0) {
                add_string_to_buff(&buff, buff_size, buff_taken, "Not ");
                tree[nodes.get_ptr()[i]].name[0] = tolower(tree[nodes.get_ptr()[i]].name[0]);
            }
            else {
                tree[nodes.get_ptr()[i]].name[0] = toupper(tree[nodes.get_ptr()[i]].name[0]);
            }
            add_string_to_buff(&buff, buff_size, buff_taken, tree[nodes.get_ptr()[i]].name);
        }
        if (buff_taken > 0) buff[buff_taken - 1] = '\0';
        say_and_print(buff, use_audio);
        say_and_print("(Sorry for English)\n", use_audio);
    }
}

void print_difference(tree_node* tree, bool& use_audio) {
    ASSERT(tree);
    say_and_print("Print first name, please.", use_audio);

    my_stack <int> first_nodes;
    first_nodes.push_back(0);
    my_stack <int> first_moves;

    char first_ans[128];

    while (true) {
        do get_line_with_endl(first_ans);
        while (check_cmd(first_ans, use_audio));

        if (!find_elem(tree, 0, first_ans, first_nodes, first_moves)) {
            say_and_print("No such element\n", use_audio);
        }
        else break;
    }

    say_and_print("Print second name, please.", use_audio);

    my_stack <int> second_nodes;
    first_nodes.push_back(0);
    my_stack <int> second_moves;

    char second_ans[128];

    while (true) {
        do get_line_with_endl(second_ans);
        while (check_cmd(second_ans, use_audio));

        if (!find_elem(tree, 0, second_ans, second_nodes, second_moves)) {
            say_and_print("No such element\n", use_audio);
        }
        else break;
    }

    for (int i = 0; i < std::min(first_moves.size(), second_moves.size()); ++i) {
        if (first_moves.get_ptr()[i] != second_moves.get_ptr()[i]) {
            char* buff = new char (0);
            size_t buff_size = 1;
            size_t buff_taken = 0;
            add_string_to_buff(&buff, buff_size, buff_taken, "Unlike ");
            if (first_moves.get_ptr()[i] == 0)
            {
                add_string_to_buff(&buff, buff_size, buff_taken, first_ans);
                add_string_to_buff(&buff, buff_size, buff_taken, ", ");
                add_string_to_buff(&buff, buff_size, buff_taken, second_ans);
                add_string_to_buff(&buff, buff_size, buff_taken, " ");
            }
            else {
                add_string_to_buff(&buff, buff_size, buff_taken, second_ans);
                add_string_to_buff(&buff, buff_size, buff_taken, ", ");
                add_string_to_buff(&buff, buff_size, buff_taken, first_ans);
                add_string_to_buff(&buff, buff_size, buff_taken, " ");
            }
            add_string_to_buff(&buff, buff_size, buff_taken, "is ");
            tree[first_nodes.get_ptr()[i]].name[0] = tolower(tree[first_nodes.get_ptr()[i]].name[0]);
            add_string_to_buff(&buff, buff_size, buff_taken, tree[first_nodes.get_ptr()[i]].name);
            say_and_print(buff, use_audio);
            say_and_print("(Sorry for English)\n", use_audio);
            return;
        }
    }
    say_and_print("Equal", use_audio);
}
#define AKINATOR_DEV_AKINATOR_FUNCTIONS_H

#endif //AKINATOR_DEV_AKINATOR_FUNCTIONS_H
