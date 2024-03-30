#include "BoolVectorHandler.h"

BoolVectorHandler::BoolVectorHandler(std::vector<bool> data) {
    data_ = data;
}

uint32_t BoolVectorHandler::VectorToNum() {
    uint32_t num = 0;
    for (size_t i = 0; i < sizeof(uint32_t) * kBitsInByte; ++i) {
        num += data_[i] ? (1 << i) : 0;
    }
    return num;
}

std::string BoolVectorHandler::VectorToString() {
    std::string str;
    char sym = 0;
    for (size_t i = 0 ; i < data_.size(); ++i) {
        sym += data_[i] ? (1 << (i % kBitsInByte)) : 0;
        if (((i + 1) % kBitsInByte == 0) || (i == data_.size() - 1)) {
            str += sym;
            sym = 0;
        }
    }
    return str;
}

void BoolVectorHandler::WriteToFile(std::ofstream& file) {
    uint8_t byte = 0;
    for (int i = 0; i < data_.size(); ++i) {
        byte += data_[i] ? (1 << (i % kBitsInByte)) : 0;
        if (((i + 1) % kBitsInByte == 0) || (i == data_.size() - 1)) {
            file << byte;
            byte = 0;
        }
    }
}

BoolVectorHandler &BoolVectorHandler::operator=(std::vector<bool> data) {
    data_ = data;
}

std::vector<bool> &BoolVectorHandler::GetData() {
    return data_;
}