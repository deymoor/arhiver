#include <libs/ArgParser/ArgParser.h>
#include <libs/HamArc/HamArc.h>
#include <libs/HamArc/utils.h>
#include <iostream>
#include <vector>
#include <string>

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
    parser.AddStringArgument("files", "files to archives").MultiValue().Positional().AdditionalArgument().StoreValues(files);
    parser.AddFlag('c', "create", "create archive").StoreValue(options.create);
    parser.AddStringArgument('f', "file", "set archive name or choose archive");
    parser.AddFlag('l', "list", "print file's name in archive").StoreValue(options.list);
    parser.AddFlag('e', "extract", "extract files from archive").StoreValue(options.extract);
    parser.AddStringArgument('a', "append", "append file to archive").AdditionalArgument();
    parser.AddStringArgument('d', "delete", "delete file from archive").AdditionalArgument();
    parser.AddFlag('c', "concatenate", "concatenate two archives").StoreValue(options.concatenate);
    parser.AddIntArgument("encode", "argument for encode(bytes)").AdditionalArgument();
    parser.AddIntArgument("decode", "argument for decode(bytes)").AdditionalArgument();
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
    HammingArchive::HamArc archive(*parser.GetStringValue("file"),
                                   parser.GetIntValue("encode"),
                                   parser.GetIntValue("decode"));
    if (options.create) {
        archive.CreateArchive();
        for (size_t i = 0; i < files.size(); ++i) {
            archive.EncodeFile(files[i]);
        }
    } else if (options.list) {
        archive.OpenArchive();
        archive.PrintFileNames();
    } else if (options.extract) {
        archive.OpenArchive();
        if (files.empty()) {
            archive.ExtractAllFiles();
        } else {
            for (size_t i = 0; i < files.size(); ++i) {
                archive.ExtractFile(files[i]);
            }
        }
    } else if (options.concatenate) {
        if (files.size() != 2) {
            PrintErrorAndExit("You must to write only two archives!");
        }
        archive.CreateArchive();
        archive.Merge(files[0], files[1]);
    } else if (parser.GetStringValue("append")) {
        archive.OpenArchiveToAppend();
        archive.AppendFile(*parser.GetStringValue("append"));
    } else if (parser.GetStringValue("delete")) {
        archive.DeleteFile(*parser.GetStringValue("delete"));
    } else {
        PrintErrorAndExit("Something went wrong...");
    }
}
