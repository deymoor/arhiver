#include <libs/ArgParser/ArgParser.h>
#include <libs/HamArc/HamArc.h>
#include <libs/HamArc/HammingCode/Hamming.h>
#include <iostream>
#include <vector>
#include <string>
#include <optional>

struct Options {
    bool create = false;
    bool list = false;
    bool extract = false;
    bool concatenate = false;
};

int main(int argc, char** argv) {
    Options options;
    std::vector<std::string> files;
    ArgumentParser::ArgParser parser("HamArc's Arguments");
    parser.AddStringArgument("files", "files to archives").MultiValue().Positional().StoreValues(files);
    parser.AddFlag('c', "create", "create archive").StoreValue(options.create);
    parser.AddStringArgument('f', "file", "set archive name or choose archive");
    parser.AddFlag('l', "list", "print file's name in archive").StoreValue(options.list);
    parser.AddFlag('e', "extract", "extract files from archive").StoreValue(options.extract);
    parser.AddStringArgument('a', "append", "append file to archive").AdditionalArgument();
    parser.AddStringArgument('d', "delete", "delete file from archive").AdditionalArgument();
    parser.AddFlag('c', "concatenate", "concatenate two archives").StoreValue(options.concatenate);
    parser.AddHelp('h', "help", "Program archive files");
    if (!parser.Parse(argc, argv)) {
        std::cout << "Wrong arguments!" << std::endl;
        std::cout << parser.HelpDescription() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if (parser.Help()) {
        std::cout << parser.HelpDescription() << std::endl;
        return 0;
    }
    // переделать positional, добавить -l myfile1 без равно
    HammingArchive::HamArc archive(*parser.GetStringValue("file"));
    if (options.create) {
        archive.CreateArchive();
        for (size_t i = 0; i < files.size(); ++i) {
            archive.EncodeFile(files[i]);
        }
        archive.OpenArchive();
        archive.DecodeArchive();
    } else if (options.list) {
        archive.OpenArchive();
        archive.PrintFilesName();
    } else if (options.extract) {
        if (files.empty()) {
            archive.ExtractAllFiles();
        } else {
            for (size_t i = 0; i < files.size(); ++i) {
                archive.ExtractFile(files[i]);
            }
        }
    } else if (options.concatenate) {
        if (files.size() != 2) {
            // error
        }
        archive.Merge(files[0], files[1]);
    } else {
        std::cout << "print command!";
        // error: print command
    }
    return 0;
}
// size - 5 байт, остальное по 8 байт + остаток
