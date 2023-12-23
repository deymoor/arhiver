#pragma once
#include <vector>
#include <fstream>
#include "../HammingCode/Hamming.h"

class BoolVectorHandler {
public:
    BoolVectorHandler() = default;
    BoolVectorHandler(std::vector<bool> data);
    uint32_t VectorToNum();
    std::string VectorToString();
    void WriteToFile(std::ofstream& file);
    std::vector<bool>& GetData();

    BoolVectorHandler& operator=(std::vector<bool> data);

    template <typename T>
    void Push(T num) {
        for (size_t i = 0; i < sizeof(num); ++i) {
            for (size_t j = 0; j < kBitsInByte; ++j) {
                data_.push_back(num & 1);
                num = num >> 1;
            }
        }
    }

private:
    std::vector<bool> data_;
    const uint8_t kBitsInByte = 8;
};