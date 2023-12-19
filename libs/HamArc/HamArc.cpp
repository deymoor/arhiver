#include "HamArc.h"

namespace HammingArchive {

    HamArc::HamArc(const std::string& file_name) : name_(file_name) {}

    void HamArc::CreateArchive() {
        archive_out_.open(name_ + kHammingExtension);
    }

    void HamArc::OpenArchive() {
        archive_in_.open(name_ + kHammingExtension);
    }

    void HamArc::WriteVectorToArchive(const std::vector<bool>& data) {
        uint8_t byte = 0;
        for (int i = 0; i < data.size(); ++i) {
            if (((i + 1) % kBitsInByte == 0) || (i == data.size() - 1)) {
                archive_out_ << byte;
                byte = 0;
            } else {
                byte += data[i] ? (1 << i) : 0;
            }
        }
    }

    void HamArc::WriteByteToVector(std::vector<bool>& data, uint8_t byte) {
        for (size_t i = 0; i < kBitsInByte; ++i) {
            data.push_back(byte & 1);
            byte = byte >> 1;
        }
    }

    void HamArc::BytesToArchive(std::istream& stream) {
        uint8_t byte;
        uint32_t file_size = 0;
        std::vector<bool> data;
        uint8_t cnt = 0;
        while (stream >> std::noskipws >> byte) {
            if (cnt == kCountOfByteToHammingCode) {
                std::vector<bool> hamming_code = HammingCode::EncodeToHammingCode(data);;
                WriteVectorToArchive(hamming_code);
                file_size += kCountOfByteToHammingCode * (kCountOfByteToHammingCode + 1);
                data.clear();
                cnt = 0;
            }
            WriteByteToVector(data, byte);
            ++cnt;
        }
        if (cnt != 0) {
            std::vector<bool> hamming_code = HammingCode::EncodeToHammingCode(data);
            WriteVectorToArchive(hamming_code);
            file_size += hamming_code.size();
        }
        NumToArchive(file_size);
    }

    void HamArc::WriteFileNameToArchive(std::string_view file_name) {
        NumToArchive(static_cast<uint8_t>(file_name.size()));
        std::istringstream stream_file_name(file_name.data());
        BytesToArchive(stream_file_name);
    }

    void HamArc::EncodeFile(std::string_view file_name) {
        std::ifstream file(file_name.data());
        WriteFileNameToArchive(file_name);
        BytesToArchive(file);
    }

    void HamArc::DecodeArchive() {
        uint16_t byte;
        archive_in_ >> byte;
        std::vector<bool> hamming_code;
        NumToVector(hamming_code, byte);
        std::optional<std::vector<bool>> data = HammingCode::DecodeToData(hamming_code);
        // доделать проверку ошибок
    }

    void HamArc::PrintFilesName() {

    }

    void HamArc::ExtractAllFiles() {

    }

    void HamArc::ExtractFile(std::string_view file_name) {

    }

    void HamArc::Merge(std::string_view first_archive_name, std::string_view second_archive_name) {

    }

}