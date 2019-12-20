#ifndef DIFFERENTIATOR_BOR_H
#include "strings_read_functions.h"
class bor {
public:
    bor() {
        size = 0;
        zero_node = new bor_node;
    }
    ~bor() {
        delete(zero_node);
    }
    int get_size() {
        return size;
    }
    int add (const char* str, int value) {
        size++;
        ASSERT(str);
        bor_node* cur_node = zero_node;
        int i = 0;
        while (str[i] != '\0' && str[i] != ':') {
            ASSERT(str[i] >= 0);
            if (cur_node->nodes[str[i]] == nullptr) {
                cur_node->nodes[str[i]] = new bor_node;
            }
            cur_node = cur_node->nodes[str[i]];
            ++i;
        }
        cur_node->value = value;
        return size;
    }
    int check (const char* str) {
        ASSERT(str);
        bor_node* cur_node = zero_node;
        int i = 0;
        while (str[i] != '\0' && str[i] != ':') {
            ASSERT(str[i] >= 0);
            if (cur_node->nodes[str[i]] == nullptr) {
                return -1;
            }
            cur_node = cur_node->nodes[str[i]];
            ++i;
        }
        return cur_node -> value;
    }

    int check_max (const char* str) {
        ASSERT(str);
        ASSERT(str);
        bor_node* cur_node = zero_node;
        int i = 0;
        int val = -1;
        while (str[i] != '\0' && str[i] != ':') {
            ASSERT(str[i] >= 0);
            if (cur_node->nodes[str[i]] == nullptr) {
                return val;
            }
            cur_node = cur_node->nodes[str[i]];
            if (cur_node -> value != -1) val = cur_node -> value;
            ++i;
        }
        return val;
    }

private:
    struct bor_node {
        bor_node* nodes[128];
        int value;
        bor_node() {
            for (int i = 0; i < 128; ++i)
                nodes[i] = nullptr;
            value = -1;
        }
        ~bor_node() {
            for (int i = 0; i < 128; ++i)
                if (nodes[i] != nullptr) delete(nodes[i]);
        }
    };
    int size;
    bor_node* zero_node;
};
#define DIFFERENTIATOR_BOR_H

#endif //DIFFERENTIATOR_BOR_H