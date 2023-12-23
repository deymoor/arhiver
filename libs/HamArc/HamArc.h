#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <ctime>
#include "HammingCode/Hamming.h"
#include "BoolVectorHandler/BoolVectorHandler.h"
#include "utils.h"

namespace HammingArchive {
    class HamArc {
    public:
        HamArc(const std::string& file_name);

        void CreateArchive();
        void OpenArchiveToAppend();
        void OpenArchive();

        void EncodeFile(std::string_view file_name);
        void AppendFile(std::string_view file_name);
        void DeleteFile(std::string_view file_name);
        void PrintFileNames();
        void ExtractAllFiles();
        void ExtractFile(std::string_view file_name);
        void Merge(std::string_view first_archive_name, std::string_view second_archive_name);
    private:
        struct HeaderHAF {
            std::string name;
            uint32_t name_size;
            uint32_t data_size;
        };

        bool GetHeader(HeaderHAF& header);
        bool GetSize(uint32_t& size);
        std::string GetFileName(uint32_t name_size);
        void GetFile(std::ofstream& file, uint32_t data_size);

        void WriteFileNameToArchive(std::string_view file_name);
        void DataToArchive(std::istream& stream);

        void WriteVectorToArchive(const std::vector<bool>& data);
        void NumToArchive(uint32_t num);
        void MoveData(HamArc& archive, HeaderHAF& header);
        void Move(uint8_t byte);

        const char* kHammingArchiveExtension = ".haf";
        const uint8_t kBitsInByte = 8;
        const uint8_t kBytesInHammingCodeUINT32_T = 5;
        const uint8_t kBytesInEncodedSizes = 5;
        const uint8_t kBytesInHammingCode = 9;
        std::string name_;
        std::fstream archive_in_;
        std::fstream archive_out_;
        uint8_t encode_argument_ = 8;
        uint8_t decode_argument_ = 8;
    };
} // namespace HammingArchive