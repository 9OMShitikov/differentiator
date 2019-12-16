#ifndef POEM_SORT_FINAL_POEM_SORT_FUNCTIONS_H
#define POEM_SORT_FINAL_POEM_SORT_FUNCTIONS_H

#include <string_view>
#include <assert.h>
#include <cstdio>
#include <iostream>
#ifndef PRINT_CHECKING
    #define ASSERT(expr) if (!(expr)) throw std::exception()
#else
    #define ASSERT(expr) printf("Checking %s\n", #expr); if (!(expr)) throw std::exception()
#endif

//Don't forget: strchr, memchr, strtok

template <class T>
class AutoFree {
public:
    T* ptr;

    AutoFree() {
        ptr = nullptr;
    }
    ~AutoFree() {
        std::free(ptr);
    }
    AutoFree(const AutoFree&) =  delete;
    void operator=(const AutoFree&) = delete;
};
///Function which reads file input_name and puts it to char array
///which buff is pointing to.
/**
 *
 * @param buff pointer to a char array, char** (not nullptr)
 * @param input_name name of the input file, const char* (file should exist)
 * @return size of array with file symbols, size_t
 */
size_t read_file (AutoFree<char> *buff, const char* input_name){
    ASSERT(buff);
    ASSERT(input_name);
    FILE *input_file = nullptr;
    input_file = fopen(input_name, "r");
    ASSERT(input_file);
    ASSERT(fseek(input_file, 0L, SEEK_END) == 0);
    long size = ftell(input_file);
    ASSERT(size >= 0);
    ASSERT(fseek(input_file, 0L, SEEK_SET) == 0);
    (*buff).ptr = (char *) calloc(size + 1, sizeof(char));
    if (fread((*buff).ptr, sizeof(char), size + 1, input_file) != size + 1) {
        ASSERT(feof(input_file));
    }
    ASSERT(fclose(input_file) == 0);
    ++size;

    char *cur_buff_symbol = (*buff).ptr + (size - 2);
    while (size > 1 && *cur_buff_symbol == '\0') {
        --size;
        --cur_buff_symbol;
    }

    return size;
}

///Divides array of char *buff to lines and puts them as string_view to array *poem
/**
 *
 * @param poem pointer to string_view pointer, string_view** (not nullptr)
 * @param buff pointer to char pointer, char** (not nullptr)
 * @param buff_size size of char array, size_t
 * @return size of string_view array, size_t
 */
size_t make_string_array(AutoFree<std::string_view> *poem, AutoFree<char> *buff, size_t buff_size) {
    ASSERT(buff);
    ASSERT(poem);

    bool is_last_eoln = true;
    int lines_count = 0;
    for (int i = 0; i < buff_size - 1; ++i) {
        if ((*buff).ptr[i] == '\n' || (*buff).ptr[i] == ' ') {
            (*buff).ptr[i] = '\0';
            if (i > 0 && (*buff).ptr[i - 1] != '\0') ++lines_count;
        }
    }

    if ((*buff).ptr[buff_size - 2] != '\0') {
        is_last_eoln = false;
        reinterpret_cast<char*>((*buff).ptr)[buff_size - 1] = '\0';
        ++lines_count;
    }

    (*poem).ptr = (std::string_view *) calloc (lines_count, sizeof(std::string_view));

    if ((*buff).ptr[0] != '\0')
        (*poem).ptr[0] = std::string_view((*buff).ptr);
    int j = 1;
    for (int i = 1; i < buff_size - is_last_eoln; ++i) {
        if ((*buff).ptr[i - 1] == '\0' && (*buff).ptr[i] != '\0') {
            (*poem).ptr[j] = std::string_view((*buff).ptr + i);
            i += (*poem).ptr[j].size() - 1;
            ++j;
        }
    }

    return lines_count;
}

size_t make_string_array_with_brackets(AutoFree<std::string_view> *poem, AutoFree<char> *buff, size_t buff_size,
                                       char first_bracket, char second_bracket) {
    ASSERT(buff);
    ASSERT(poem);

    bool is_last_eoln = true;
    int lines_count = 0;
    bool brackets_open = 0;
    for (int i = 0; i < buff_size - 1; ++i) {
        if (!brackets_open && ((*buff).ptr[i] == '\n' || (*buff).ptr[i] == ' ')) {
            (*buff).ptr[i] = '\0';
            if (i > 0 && (*buff).ptr[i - 1] != '\0') ++lines_count;
        }
        else if (!brackets_open && (*buff).ptr[i] == first_bracket) {
            brackets_open = 1;
            (*buff).ptr[i] = '\0';
            if (i > 0 && (*buff).ptr[i - 1] != '\0') ++lines_count;
        }
        else if (brackets_open && (*buff).ptr[i] == second_bracket) {
            brackets_open = 0;
            (*buff).ptr[i] = '\0';
            if (i > 0 && (*buff).ptr[i - 1] != '\0') ++lines_count;
        }
    }

    if ((*buff).ptr[buff_size - 2] != '\0') {
        is_last_eoln = false;
        reinterpret_cast<char*>((*buff).ptr)[buff_size - 1] = '\0';
        ++lines_count;
    }

    (*poem).ptr = (std::string_view *) calloc (lines_count, sizeof(std::string_view));

    if ((*buff).ptr[0] != '\0')
        (*poem).ptr[0] = std::string_view((*buff).ptr);
    int j = 1;
    for (int i = 1; i < buff_size - is_last_eoln; ++i) {
        if ((*buff).ptr[i - 1] == '\0' && (*buff).ptr[i] != '\0') {
            (*poem).ptr[j] = std::string_view((*buff).ptr + i);
            i += (*poem).ptr[j].size() - 1;
            ++j;
        }
    }

    return lines_count;
}

char to_lowerc (const char a) {
    if ('A' <= a && a <= 'Z') return a - 'A' + 'a';
    else return a;
}

bool str_cmp (const char* start_1, const char* end_1,
              const char* start_2, const char* end_2,
              int step) {
    while (start_1 != end_1 && !isalpha(*start_1)) start_1 += step;
    while (start_2 != end_2 && !isalpha(*start_2)) start_2 += step;
    int pos = 0;
    while (start_1 != end_1 &&
           start_2 != end_2 &&
           to_lowerc(*start_1) == to_lowerc(*start_2)) {
        start_1 += step;
        start_2 += step;
    }

    if (end_1 == start_1 && start_2 != end_2) return true;
    if (end_1 == start_1 || end_2 == start_2) return false;
    return (to_lowerc(*start_1) < to_lowerc(*start_2));
}

///Compares two strings from beginning (starts from the first letter)
/**
 *
 * @param str_1 first string, string_view
 * @param str_2 second string, string_view
 * @return the result of comparison, bool
 */
bool begin_cmp (std::string_view str_1, std::string_view str_2) {
    return str_cmp(str_1.data(), str_1.data() + str_1.size(),
                   str_2.data(), str_2.data() + str_2.size(), 1);
}

///Compares two strings from ending (starts from the last letter)
/**
 *
 * @param str_1 first string, string_view
 * @param str_2 second string, string_view
 * @return the result of comparison, bool
 */
bool end_cmp (std::string_view str_1, std::string_view str_2) {
    return str_cmp(str_1.data() + str_1.size() - 1, str_1.data() - 1,
                   str_2.data() + str_2.size() - 1, str_2.data() - 1, -1);
}

///Clears the output file
/**
 *
 * @param output_name name of the output file, const char*
 */
void clear_output(const char *output_name) {
    ASSERT(output_name);
    FILE *file = fopen(output_name, "w");
    ASSERT(file);
    ASSERT(fclose(file) == 0);
}

///Prints lines from the poem array to output file
/**
 *
 * @param count count of lines, int
 * @param poem pointer to array of lines, string_view** (not nullptr)
 * @param output_name name of the output file, const char* (file should exist)
 */
void print_lines(size_t count, AutoFree<std::string_view> *poem, const char *output_name) {
    ASSERT(poem);
    ASSERT(output_name);
    FILE *file = fopen(output_name, "a");
    ASSERT(file);
    for (int i = 0; i < count; i++) {
        fputs((*poem).ptr[i].data(), file);
        fputc('\n', file);
    }
    fputs("-------------\n", file);
    ASSERT(fclose(file) == 0);
}

///Prints buffer to output file
/**
 *
 * @param size size of buffer, int
 * @param buff pointer to buffer, char**
 * @param output_name name of the output file, const char* (file should exist)
 */
void print_buff(size_t size, AutoFree <char> *buff, const char *output_name) {
    ASSERT(buff);
    ASSERT(output_name);

    FILE *file = fopen(output_name, "a");
    ASSERT(file);

    for (int i = 0; i < size; i++) {
        if (i == 0 || (*buff).ptr[i - 1] == '\0') {
            ASSERT(fputs((*buff).ptr + i, file) != EOF);
            fputc('\n', file);
            ASSERT(!ferror(file));
        }
    }
    ASSERT(fclose(file) == 0);
}

template <class T>
void print_bin(size_t size, T **buff, const char *output_name) {
    ASSERT(buff);
    ASSERT(output_name);

    FILE *file = fopen(output_name, "wb");
    ASSERT(file);
    ASSERT(fwrite(*buff, sizeof(T), size, file) == size);
    ASSERT(fclose(file) == 0);
}

template <class T>
size_t read_bin (AutoFree<T> *buff, const char* input_name){
    ASSERT(buff);
    ASSERT(input_name);
    FILE *input_file = nullptr;
    input_file = fopen(input_name, "rb");
    ASSERT(input_file);
    ASSERT(fseek(input_file, 0L, SEEK_END) == 0);
    long size = ftell(input_file)/sizeof(T);
    ASSERT(size >= 0);
    ASSERT(fseek(input_file, 0L, SEEK_SET) == 0);
    (*buff).ptr = (T *) calloc(size + 1, sizeof(T));
    if (fread((*buff).ptr, sizeof(T), size + 1, input_file) != size + 1) {
        ASSERT(feof(input_file));
    }
    ASSERT(fclose(input_file) == 0);

    return size;
}

template <class T>
size_t read_file_with_free_place (AutoFree<T> *buff, const char* input_name, int elems_to_reserve){
    ASSERT(buff);
    ASSERT(input_name);
    FILE *input_file = nullptr;
    input_file = fopen(input_name, "rb");
    ASSERT(input_file);
    ASSERT(fseek(input_file, 0L, SEEK_END) == 0);
    long size = ftell(input_file)/sizeof(T);
    ASSERT(size >= 0);
    ASSERT(fseek(input_file, 0L, SEEK_SET) == 0);
    (*buff).ptr = (T *) calloc(size + elems_to_reserve + 1, sizeof(T));
    if (fread((*buff).ptr, sizeof(T), size + 1, input_file) != size + 1) {
        ASSERT(feof(input_file));
    }
    ASSERT(fclose(input_file) == 0);

    return size;
}

int get_magic_int (char* str) {
    ASSERT(str);
    return (*reinterpret_cast<int*>(str));
}

void get_line_with_endl (char* str) {
    ASSERT(str);
    str[0] = '\n';
    while (str[0] == '\n') {
        fgets(str, 2, stdin);
    }
    fgets(str + 1, 128, stdin);
    void* name_end = strchr(str, '\n');
    if (name_end) {
        *reinterpret_cast<char*>(name_end) = '\0';
    }
}

void say_and_print (const char* s, bool use_audio) {
    ASSERT(s);
    char buff[strlen(s) + 32];
    strcpy(buff, "echo \"");
    strcat(buff, s);
    printf("%s\n", buff + 6);
    strncat(buff, "\" | festival --tts", 32);
    if (use_audio)
        system(buff);
}

void add_string_to_buff (char** buff, size_t& size, size_t& taken, const char* add) {
    ASSERT(buff);
    ASSERT(add);
    if (taken + strlen(add) + 2 >= size) {
        size += std::max(size, strlen(add));
        char* new_buff = (char*) calloc(size, sizeof(char));
        ASSERT(new_buff);
        strcpy(new_buff, *buff);
        free(*buff);
        *buff = new_buff;
    }
    strcpy(*buff + taken, add);
    taken += strlen(add);
}

void add_int_to_buff (char** buff, size_t& size, size_t& taken, const int add_value) {
    char add[17];
    sprintf(add, "%d", add_value);
    add_string_to_buff(buff, size, taken, add);
}

#endif //POEM_SORT_FINAL_POEM_SORT_FUNCTIONS_H
