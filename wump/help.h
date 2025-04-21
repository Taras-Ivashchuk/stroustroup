#ifndef HELP_H
#define HELP_H
#include <string>

namespace Help {
    using namespace std;
    enum Input_type {
        R1 = 1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11,
        R12, R13, R14, R15, R16, R17, R18, R19, R20,
        YES = 'y', NO = 'n', SHOOT = 's', MOVE = 'm', QUIT = 'q',
    };

    void show_help();
    Input_type prompt_help();
    Input_type get_input(const string& prompt, const string& pattern);
}

#endif
