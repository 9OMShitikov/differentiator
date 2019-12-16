//
// Created by oleg on 16.12.2019.
//

#ifndef DIFFERENTIATOR_INFO_H
#define DIFFERENTIATOR_INFO_H
enum operators {
    SUM,
    SUB,
    MUL,
    DIV
};

class operators_definitions {
public:
    const static int operators_count = 4;
    char operators_names [operators_count] [10];
    bor operators_bor;

    operators_definitions() {
        strcpy(operators_names[SUM], "+");
        operators_bor.add("+", SUM);

        strcpy(operators_names[SUB], "-");
        operators_bor.add("-", SUB);

        strcpy(operators_names[MUL], "*");
        operators_bor.add("*", MUL);

        strcpy(operators_names[DIV], "/");
        operators_bor.add("/", DIV);
    }

    double compute(operators op, double x1, double x2) {
        switch (op) {
            case SUM: return x1 + x2;
            case SUB: return x1 - x2;
            case MUL: return x1 * x2;
            case DIV: return x1 / x2;
        }
    }
};

enum functions {
    SIN,
    COS,
    TAN,
    SQRT,
};

class functions_definitions {
public:
    const static int functions_count = 4;
    char functions_names [functions_count] [10];
    bor functions_bor;

    functions_definitions() {
        strcpy(functions_names[SIN], "sin");
        functions_bor.add("sin", SIN);

        strcpy(functions_names[COS], "cos");
        functions_bor.add("cos", COS);

        strcpy(functions_names[TAN], "tan");
        functions_bor.add("tan", TAN);

        strcpy(functions_names[SQRT], "sqrt");
        functions_bor.add("sqrt", SQRT);
    }

    double compute(functions op, double arg) {
        switch (op) {
            case SIN: return sin(arg);
            case COS: return cos(arg);
            case TAN: return tan(arg);
            case SQRT: return sqrt(arg);
        }
    }
};
#endif //DIFFERENTIATOR_INFO_H
