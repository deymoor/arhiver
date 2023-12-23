#include "HamArc.h"

namespace HammingArchive {

    HamArc::HamArc(const std::string &file_name, std::optional<int32_t> encode_argument,
                   std::optional<int32_t> decode_argument): name_(file_name) {
        if (encode_argument) {
            encode_argument_ = *encode_argument;
        }
        if (decode_argument) {
            decode_argument_ = *decode_argument;
        }
    }

    void HamArc::CreateArchive() {
        archive_out_.open(name_ + kHammingArchiveExtension, std::ios_base::out);
        if (!archive_out_.is_open()) {
            PrintErrorAndExit("Archive didn't created!");
        }
    }

    void HamArc::OpenArchiveToAppend() {
        archive_out_.open(name_ + kHammingArchiveExtension);
        archive_out_.seekp(0, std::ios_base::end);
        if (!archive_out_.is_open()) {
            PrintErrorAndExit("Archive didn't opened to appending!");
        }
    }

    void HamArc::OpenArchive() {
        archive_in_.open(name_ + kHammingArchiveExtension);
        if (!archive_in_.is_open()) {
            PrintErrorAndExit("Archive didn't opened!");
        }
    }

    void HamArc::EncodeFile(std::string_view file_name) {
        std::ifstream file(file_name.data());
        WriteFileNameToArchive(file_name);
        DataToArchive(file);
    }

    void HamArc::AppendFile(std::string_view file_name) {
        EncodeFile(file_name);
    }

    void HamArc::DeleteFile(std::string_view file_name) {
        this->OpenArchive();
        std::string temp_name = std::to_string(std::time(nullptr));
        HammingArchive::HamArc temp_archive(temp_name, encode_argument_,
                                            decode_argument_);
        temp_archive.CreateArchive();
        HeaderHAF header;
        while (GetHeader(header)) {
            if (header.name == file_name) {
                archive_in_.seekg(FromBitsToBytes(header.data_size), std::ios_base::cur);
            } else {
                archive_in_.seekg(-static_cast<int32_t>(FromBitsToBytes(header.name_size))
                    - 2 * kBytesInHammingCodeUINT32_T, std::ios_base::cur);
                this->MoveData(temp_archive, header);
            }
        }
        rename((temp_name + kHammingArchiveExtension).c_str(), (name_ + kHammingArchiveExtension).c_str());
    }

    void HamArc::PrintFileNames() {
        HeaderHAF header;
        std::cout << "Files in " << name_ << kHammingArchiveExtension << " archive:\n";
        while (GetHeader(header)) {
            std::cout << header.name << '\n';
            archive_in_.seekg(FromBitsToBytes(header.data_size), std::ios_base::cur);
        }
    }

    void HamArc::ExtractAllFiles() {
        HeaderHAF header;
        while (GetHeader(header)) {
            std::ofstream file("/home/dmitry/labs/" + header.name);
            GetFile(file, header.data_size);
        }
    }

    void HamArc::ExtractFile(std::string_view file_name) {
        HeaderHAF header;
        while (GetHeader(header)) {
            if (header.name == file_name) {
                std::ofstream file("/home/dmitry/labs/" + header.name);
                GetFile(file, header.data_size);
                break;
            }
            archive_in_.seekg(FromBitsToBytes(header.data_size), std::ios_base::cur);
        }
    }

    void HamArc::Merge(std::string_view first_archive_name, std::string_view second_archive_name) {
        uint8_t byte;
        std::ifstream first_archive(first_archive_name.data());
        std::ifstream second_archive(second_archive_name.data());
        while (first_archive >> std::noskipws >> byte) {
            archive_out_ << byte;
        }
        while(second_archive >> std::noskipws >> byte) {
            archive_out_ << byte;
        }
    }

    bool HamArc::GetHeader(HeaderHAF& header) {
        if (!GetSize(header.name_size)) {
            return false;
        }
        header.name = GetFileName(header.name_size);
        GetSize(header.data_size);
        return true;
    }

    bool HamArc::GetSize(uint32_t &size) {
        uint8_t byte;
        BoolVectorHandler hamming_code_handler;
        uint8_t cnt = 0;
        while(archive_in_ >> std::noskipws >> byte) {
            hamming_code_handler.Push(byte);
            ++cnt;
            if (cnt == kBytesInEncodedSizes) {
                hamming_code_handler.GetData().pop_back();
                std::optional<BoolVectorHandler> data = DecodeToData(hamming_code_handler.GetData());
                if (data != std::nullopt) {
                    size = data->VectorToNum();
                    return true;
                }
            }
        }
        return false;
    }

    std::string HamArc::GetFileName(uint32_t name_size) {
        std::string file_name;
        uint32_t bytes = FromBitsToBytes(name_size);
        BoolVectorHandler hamming_code;
        uint8_t cnt = 0;
        uint8_t byte;
        while (bytes) {
            archive_in_ >> std::noskipws >> byte;
            hamming_code.Push(byte);
            ++cnt;
            if (cnt == kBytesInHammingCode) {
                std::optional<BoolVectorHandler> data = DecodeToData(hamming_code.GetData());
                if (data == std::nullopt) {
                    PrintErrorAndExit("Double error in hamming code!");
                }
                file_name += data->VectorToString();
                hamming_code.GetData().clear();
                cnt = 0;
            }
            --bytes;
        }
        if (cnt != 0) {
            uint32_t remain = kBitsInByte - (name_size % kBitsInByte);
            for (size_t i = 0; i < remain; ++i) {
                hamming_code.GetData().pop_back();
            }
            std::optional<BoolVectorHandler> data = DecodeToData(hamming_code.GetData());
            if (data == std::nullopt) {
                PrintErrorAndExit("Double error in hamming code!");
            }
            file_name += data->VectorToString();
        }
        return file_name;
    }

    void HamArc::GetFile(std::ofstream &file, uint32_t data_size) {
        BoolVectorHandler hamming_code;
        uint32_t bytes = FromBitsToBytes(data_size);
        uint8_t cnt = 0;
        uint8_t byte;
        while (bytes) {
            archive_in_ >> std::noskipws >> byte;
            hamming_code.Push(byte);
            ++cnt;
            if (cnt == kBytesInHammingCode) {
                std::optional<BoolVectorHandler> data = DecodeToData(hamming_code.GetData());
                if (data == std::nullopt) {
                    PrintErrorAndExit("Double error in hamming code!");
                }
                data->WriteToFile(file);
                hamming_code.GetData().clear();
                cnt = 0;
            }
            --bytes;
        }
        if (cnt != 0) {
            uint32_t remain = kBitsInByte - (data_size % kBitsInByte);
            for (size_t i = 0; i < remain; ++i) {
                hamming_code.GetData().pop_back();
            }
            std::optional<BoolVectorHandler> data = DecodeToData(hamming_code.GetData());
            if (data == std::nullopt) {
                std::cout << "Double error!";
            }
            data->WriteToFile(file);
        }
    }

    void HamArc::WriteFileNameToArchive(std::string_view file_name) {
        std::istringstream stream_file_name(file_name.data());
        DataToArchive(stream_file_name);
    }

    void HamArc::DataToArchive(std::istream& stream) {
        uint8_t byte;
        uint32_t file_size = 0;
        uint8_t cnt = 0;
        BoolVectorHandler data_handler;
        archive_out_ << "filsz";
        while (stream >> std::noskipws >> byte) {
            data_handler.Push(byte);
            ++cnt;
            if (cnt == encode_argument_) {
                BoolVectorHandler hamming_code = EncodeToHammingCode(data_handler.GetData());
                WriteVectorToArchive(hamming_code.GetData());
                file_size += encode_argument_ * (encode_argument_ + 1);
                data_handler.GetData().clear();
                cnt = 0;
            }
        }
        if (cnt != 0) {
            BoolVectorHandler hamming_code = EncodeToHammingCode(data_handler.GetData());
            WriteVectorToArchive(hamming_code.GetData());
            file_size += hamming_code.GetData().size();
        }
        archive_out_.seekp(-static_cast<int32_t>(FromBitsToBytes(file_size)) - kBytesInHammingCodeUINT32_T, std::ios_base::cur);
        NumToArchive(file_size);
        archive_out_.seekp(0, std::ios_base::end);
    }

    void HamArc::WriteVectorToArchive(const std::vector<bool>& data) {
        uint8_t byte = 0;
        for (int i = 0; i < data.size(); ++i) {
            byte += data[i] ? (1 << (i % kBitsInByte)) : 0;
            if (((i + 1) % kBitsInByte == 0) || (i == data.size() - 1)) {
                archive_out_ << byte;
                byte = 0;
            }
        }
    }

    void HamArc::NumToArchive(uint32_t num) {
        BoolVectorHandler data;
        data.Push(num);
        BoolVectorHandler hamming_code = EncodeToHammingCode(data.GetData());
        WriteVectorToArchive(hamming_code.GetData());
    }

    void HamArc::MoveData(HammingArchive::HamArc &archive, HammingArchive::HamArc::HeaderHAF &header) {
        uint8_t byte;
        uint32_t file_size = 2 * kBytesInHammingCodeUINT32_T +
                FromBitsToBytes(header.name_size) + FromBitsToBytes(header.data_size);
        for (size_t i = 0; i < file_size; ++i) {
            archive_in_ >> std::noskipws >> byte;
            archive.Move(byte);
        }
    }

    void HamArc::Move(uint8_t byte) {
        archive_out_ << byte;
    }
}
