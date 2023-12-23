#pragma once
#include <vector>
#include <cstdint>
#include <iostream>
#include <optional>

uint32_t HammingSize(uint8_t data_size);

std::vector<bool> EncodeToHammingCode(const std::vector<bool>& data);

std::optional<std::vector<bool>> DecodeToData(std::vector<bool>& hamming_data);