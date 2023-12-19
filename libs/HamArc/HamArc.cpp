#include "HamArc.h"

namespace HammingArchive {

    HamArc::HamArc(const std::string& file_name) : name_(file_name) {}

    void HamArc::CreateArchive() {
        archive_out_.open(name_ + kHammingArchiveExtension);
    }

    void HamArc::OpenArchive() {
        archive_in_.open(name_ + kHammingArchiveExtension);
    }

    void HamArc::WriteVectorToArchive(const std::vector<bool>& data) {
        archive_size_ += data.size() / kBitsInByte + std::min(static_cast<uint64_t>(1), data.size() % kBitsInByte);
        uint8_t byte = 0;
        for (int i = 0; i < data.size(); ++i) {
            byte += data[i] ? (1 << (i % kBitsInByte)) : 0;
            if (((i + 1) % kBitsInByte == 0) || (i == data.size() - 1)) {
                archive_out_ << byte;
                byte = 0;
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
        archive_out_ << "filsz";
        archive_size_ += 5;
        std::vector<bool> data;
        uint8_t cnt = 0;
        while (stream >> std::noskipws >> byte) {
            WriteByteToVector(data, byte);
            ++cnt;
            if (cnt == kCountOfByteToHammingCode) {
                std::vector<bool> hamming_code = HammingCode::EncodeToHammingCode(data);;
                WriteVectorToArchive(hamming_code);
                file_size += kCountOfByteToHammingCode * (kCountOfByteToHammingCode + 1);
                data.clear();
                cnt = 0;
            }
        }
        if (cnt != 0) {
            std::vector<bool> hamming_code = HammingCode::EncodeToHammingCode(data);
            WriteVectorToArchive(hamming_code);
            file_size += hamming_code.size();
        }
        uint32_t pos = archive_size_ -
                (file_size / kBitsInByte + std::min(static_cast<uint32_t>(1), file_size % kBitsInByte)) - kBytesInHammingCodeUINT32_T;
        archive_out_.seekp(pos);
        NumToArchive(file_size);
        archive_size_ -= kBytesInHammingCodeUINT32_T;
        archive_out_.seekp(archive_size_);
    }

    void HamArc::WriteFileNameToArchive(std::string_view file_name) {
        std::istringstream stream_file_name(file_name.data());
        BytesToArchive(stream_file_name);
    }

    void HamArc::EncodeFile(std::string_view file_name) {
        std::ifstream file(file_name.data());
        WriteFileNameToArchive(file_name);
        BytesToArchive(file);
    }

    uint32_t HamArc::VectorToNum(const std::vector<bool> &data) {
        uint32_t num = 0;
        for (size_t i = 0; i < sizeof(uint32_t) * kBitsInByte; ++i) {
            num += data[i] ? (1 << i) : 0;
        }
        return num;
    }

    bool HamArc::GetSize(uint32_t &size) {
        uint8_t byte;
        std::vector<bool> hamming_code;
        uint8_t cnt = 0;
        while(archive_in_ >> std::noskipws >> byte) {
            WriteByteToVector(hamming_code, byte);
            ++cnt;
            if (cnt == 5) {
                hamming_code.pop_back();
                std::optional<std::vector<bool>> data = HammingCode::DecodeToData(hamming_code);
                if (data != std::nullopt) {
                    size = VectorToNum(*data);
                    return true;
                }
            }
        }
        return false;
    }

    std::string HamArc::VectorToString(std::vector<bool>& data) {
        std::string str;
        char sym = 0;
        for (size_t i = 0 ; i < data.size(); ++i) {
            sym += data[i] ? (1 << (i % kBitsInByte)) : 0;
            if (((i + 1) % kBitsInByte == 0) || (i == data.size() - 1)) {
                str += sym;
                sym = 0;
            }
        }
        return str;
    }

    std::string HamArc::GetFileName(uint32_t bytes, uint32_t file_name_size) {
        std::string file_name;
        std::vector<bool> hamming_code;
        uint8_t cnt = 0;
        uint8_t byte;
        while (bytes) {
            archive_in_ >> std::noskipws >> byte;
            WriteByteToVector(hamming_code, byte);
            ++cnt;
            if (cnt == 9) {
                std::optional<std::vector<bool>> data = HammingCode::DecodeToData(hamming_code);
                if (data == std::nullopt) {
                    std::cout << "Double error!";
                }
                file_name += VectorToString(*data);
                hamming_code.clear();
                cnt = 0;
            }
            --bytes;
        }
        if (cnt != 0) {
            uint32_t remain = 8 - (file_name_size % 8);
            for (size_t i = 0; i < remain; ++i) {
                hamming_code.pop_back();
            }
            std::optional<std::vector<bool>> data = HammingCode::DecodeToData(hamming_code);
            if (data == std::nullopt) {
                std::cout << "Double error!";
            }
            file_name += VectorToString(*data);
        }
        return file_name;
    }

    void HamArc::WriteVectorToFile(std::ofstream& file, const std::vector<bool>& data) {
        uint8_t byte = 0;
        for (int i = 0; i < data.size(); ++i) {
            byte += data[i] ? (1 << (i % kBitsInByte)) : 0;
            if (((i + 1) % kBitsInByte == 0) || (i == data.size() - 1)) {
                file << byte;
                byte = 0;
            }
        }
    }

    void HamArc::GetFile(std::ofstream &file, uint32_t bytes, uint32_t file_size) {
        std::vector<bool> hamming_code;
        uint8_t cnt = 0;
        uint8_t byte;
        while (bytes) {
            archive_in_ >> std::noskipws >> byte;
            WriteByteToVector(hamming_code, byte);
            ++cnt;
            if (cnt == 9) {
                std::optional<std::vector<bool>> data = HammingCode::DecodeToData(hamming_code);
                if (data == std::nullopt) {
                    std::cout << "Double error!";
                }
                WriteVectorToFile(file, *data);
                hamming_code.clear();
                cnt = 0;
            }
            --bytes;
        }
        if (cnt != 0) {
            uint32_t remain = 8 - (file_size % 8);
            for (size_t i = 0; i < remain; ++i) {
                hamming_code.pop_back();
            }
            std::optional<std::vector<bool>> data = HammingCode::DecodeToData(hamming_code);
            if (data == std::nullopt) {
                std::cout << "Double error!";
            }
            WriteVectorToFile(file, *data);
        }
    }

    void HamArc::ExtractAllFiles() {
        uint32_t file_name_size;
        while (GetSize(file_name_size)) {
            uint32_t bytes_count_to_read = file_name_size / 8 + std::min(static_cast<uint32_t>(1), file_name_size % 8);
            std::string file_name = GetFileName(bytes_count_to_read, file_name_size);
            uint32_t file_size;
            GetSize(file_size);
            uint32_t bytes_count_to_read_file = file_size / 8 + std::min(static_cast<uint32_t>(1), file_size % 8);
            // убрать локальную директорию
            std::ofstream file("/home/dmitry/labs/" + file_name);
            GetFile(file, bytes_count_to_read_file, file_size);
        }

    }

    void HamArc::ExtractFile(std::string_view file_name) {

    }

    void HamArc::PrintFilesName() {

    }

    void HamArc::Merge(std::string_view first_archive_name, std::string_view second_archive_name) {

    }

}