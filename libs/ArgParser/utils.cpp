#include "utils.h"

void ThrowError(const char* error_message) {
    std::cerr << error_message;
}

int32_t ParseNumber(const std::string &str) {
    size_t end;
    auto number = std::stoi(str, &end, 10);
    if (end != str.size()) {
        ThrowError("You must to write integer numbers!");
        return 0;
    }
    return number;
}
