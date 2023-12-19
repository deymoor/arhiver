#pragma once
#include <vector>
#include <cstdint>
#include <iostream>
#include <optional>

namespace HammingCode {
    std::vector<bool> EncodeToHammingCode(std::vector<bool>& data);
    std::optional<std::vector<bool>> DecodeToData(std::vector<bool>& hamming_data);
}