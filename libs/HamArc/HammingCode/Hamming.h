#pragma once
#include <vector>
#include <cstdint>
#include <iostream>
#include <optional>


std::vector<bool> EncodeToHammingCode(const std::vector<bool>& data);

std::optional<std::vector<bool>> DecodeToData(std::vector<bool>& hamming_data);