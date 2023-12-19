#include "Hamming.h"

namespace HammingCode {
    void Init(uint8_t data_size, std::vector<bool>& hamming_data) {
        uint8_t redundant_bits = 0;
        while ((1 << redundant_bits) < data_size + redundant_bits + 1) {
            ++redundant_bits;
        }
        ++redundant_bits;
        hamming_data.resize(data_size + redundant_bits);
    }

    void FillParityBits(std::vector<bool>& hamming_data) {
        for (size_t parity_bit = 1; parity_bit < hamming_data.size(); parity_bit *= 2) {
            for (size_t hamming_bit = parity_bit; hamming_bit < hamming_data.size(); hamming_bit += parity_bit * 2) {
                for (size_t data_bit = hamming_bit; (data_bit < hamming_bit + parity_bit) && data_bit < hamming_data.size(); ++data_bit) {
                    hamming_data[parity_bit] = hamming_data[parity_bit] ^ hamming_data[data_bit];
                }
            }
        }
        for (size_t hamming_bit = 1; hamming_bit < hamming_data.size(); ++hamming_bit) {
            hamming_data[0] = hamming_data[0] ^ hamming_data[hamming_bit];
        }
    }

    std::vector<bool> EncodeToHammingCode(std::vector<bool>& data) {
        std::vector<bool> hamming_data;
        Init(data.size(), hamming_data);
        size_t hamming_bit = 0;
        for (size_t data_bit = 0; data_bit < data.size(); ++data_bit) {
            if ((hamming_bit & (hamming_bit - 1)) == 0) {
                hamming_data[hamming_bit] = false;
                --data_bit;
            } else {
                hamming_data[hamming_bit] = data[data_bit];
            }
            ++hamming_bit;
        }

        FillParityBits(hamming_data);
        return hamming_data;
    }

    std::optional<std::vector<bool>> DecodeToData(std::vector<bool>& hamming_data) {
        std::vector<bool> error;
        std::vector<bool> data;
        bool single_error = false;
        bool parity_bit_for_all_message = false;
        for (size_t hamming_bit = 1; hamming_bit < hamming_data.size(); ++hamming_bit) {
            parity_bit_for_all_message ^= hamming_data[hamming_bit];
        }
        for (size_t parity_bit = 1; parity_bit < hamming_data.size(); parity_bit *= 2) {
            uint8_t check_parity_bit = 0;
            for (size_t hamming_bit = parity_bit; hamming_bit < hamming_data.size(); hamming_bit += parity_bit * 2) {
                for (size_t check_bit = hamming_bit; (check_bit < hamming_bit + parity_bit) && check_bit < hamming_data.size(); ++check_bit) {
                    check_parity_bit ^= hamming_data[check_bit];
                }
            }
            if (check_parity_bit) {
                error.push_back(true);
                single_error = true;
            } else {
                error.push_back(false);
            }
        }
        if (parity_bit_for_all_message != hamming_data[0]) {
            if (single_error) {
                std::reverse(error.begin(), error.end());
                uint32_t error_index = 0;
                for (size_t i = 0; i < error.size(); ++i) {
                    error_index += error[error.size() - i - 1] ? (1 << i) : 0;
                }
                hamming_data[error_index] = !hamming_data[error_index];
            } else {
                hamming_data[0] = !hamming_data[0];
            }
        } else if (single_error) {
            return std::nullopt;
        }
        for (size_t hamming_bit = 0; hamming_bit < hamming_data.size(); ++hamming_bit) {
            if ((hamming_bit & (hamming_bit - 1)) != 0) {
                data.push_back(hamming_data[hamming_bit]);
            }
        }
        return data;
    }
}