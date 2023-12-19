#include "utils.h"

void PrintErrorAndExit(const char* error_message) {
    std::cerr << error_message << '\n';
    std::exit(EXIT_FAILURE);
}