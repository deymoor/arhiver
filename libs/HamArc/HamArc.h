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
        std::string VectorToString(std::vector<bool>& data);
        void GetFile(std::ofstream& file, uint32_t bytes, uint32_t file_size);
        std::string GetFileName(uint32_t bytes, uint32_t file_name_size);
        bool GetSize(uint32_t& size);
        uint32_t VectorToNum(const std::vector<bool>& data);
        void BytesToArchive(std::istream& stream);
        void WriteByteToVector(std::vector<bool>& data, uint8_t byte);
        void WriteVectorToArchive(const std::vector<bool>& data);
        void WriteFileNameToArchive(std::string_view file_name);

        template <typename T>
        std::vector<bool> NumToVector(T num) {
            std::vector<bool> data;
            for (size_t i = 0; i < sizeof(num); ++i) {
                for (size_t j = 0; j < kBitsInByte; ++j) {
                    data.push_back(num & 1);
                    num = num >> 1;
                }
            }
            return data;
        }

        template <typename T>
        void NumToArchive(T num) {
            std::vector<bool> data = NumToVector(num);
            std::vector<bool> hamming_code = HammingCode::EncodeToHammingCode(data);
            WriteVectorToArchive(hamming_code);
        }

        const char* kHammingArchiveExtension = ".haf";
        const uint8_t kCountOfByteToHammingCode = 8;
        const uint8_t kBitsInByte = 8;
        const uint8_t kBytesInHammingCodeUINT32_T = 5;
        const uint8_t kBitsInEncodedSizeOfFile = 72;
        const uint8_t kBitsInEncodedSizes = 39;
        std::string name_;
        std::string encode_data_;
        uint32_t archive_size_ = 0;
        std::ofstream archive_out_;
        std::ifstream archive_in_;
    };
} // namespace HammingArchive
