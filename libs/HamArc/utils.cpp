#include "utils.h"


void PrintErrorAndExit(const char* error_message) {
    std::cerr << error_message << '\n';
    std::exit(EXIT_FAILURE);
}

uint32_t FromBitsToBytes(uint32_t bits) {
    const uint8_t kBitsInByte = 8;
    return bits / kBitsInByte + std::min(static_cast<uint32_t>(1), bits % kBitsInByte);
}