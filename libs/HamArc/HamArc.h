#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <filesystem>
#include "HammingCode/Hamming.h"

namespace HammingArchive {
    class HamArc {
    public:
        HamArc(const std::string& file_name);

        void CreateArchive();
        void OpenArchive();
        void EncodeFile(std::string_view file_name);
        void DecodeArchive();
        void AppendFile(std::string_view file_name);
        void DeleteFile(std::string_view file_name);
        void PrintFilesName();
        void ExtractAllFiles();
        void ExtractFile(std::string_view file_name);
        void Merge(std::string_view first_archive_name, std::string_view second_archive_name);
    private:
        void BytesToArchive(std::istream& stream);
        void WriteByteToVector(std::vector<bool>& data, uint8_t byte);
        void WriteVectorToArchive(const std::vector<bool>& data);
        void WriteFileNameToArchive(std::string_view file_name);

        template <typename T>
        void NumToVector(std::vector<bool>& data, T num) {
            for (size_t i = 0; i < sizeof(num); ++i) {
                for (size_t j = 0; j < kBitsInByte; ++j) {
                    data.push_back(num & 1);
                    num = num >> 1;
                }
            }
        }

        template <typename T>
        void NumToArchive(T num) {
            std::vector<bool> data;
            std::vector<bool> hamming_code = HammingCode::EncodeToHammingCode(data);
            NumToVector(data, num);
            WriteVectorToArchive(hamming_code);
        }

        const char* kHammingExtension = ".haf";
        const uint8_t kCountOfByteToHammingCode = 8;
        const uint8_t kBitsInByte = 8;
        const uint8_t kBitsInEncodedSizeOfFile = 72;
        const uint8_t kBitsInEncodedSizeOfFileName = 13;
        std::string name_;
        std::string encode_data_;
        std::ofstream archive_out_;
        std::ifstream archive_in_;
    };
} // namespace HammingArchive
