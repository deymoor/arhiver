#pragma once
#include "Classes/Argument/Argument.h"
#include "utils.h"
#include <vector>
#include <string>
#include <istream>
#include <optional>

namespace ArgumentParser {

class ArgParser {
private:
    static const uint8_t kCountOfDashShortName = 1;
    static const uint8_t kCountOfDashLongName = 2;
    std::string name_ = "default_parser_name";
    std::vector<Argument<std::string>> str_arguments_;
    std::vector<Argument<int>> int_arguments_;
    std::vector<Argument<bool>> bool_arguments_;
    Argument<std::string>* str_pos_ = nullptr;
    Argument<int>* int_pos_ = nullptr;
    Argument<bool>* bool_pos_ = nullptr;
    std::string long_help_name_;
    std::string short_help_name_;
    std::string help_description_;
    bool is_help_ = false;

    template<typename T>
    void AddParameter(std::vector<Argument<T>>& arguments, size_t pos, T parameter) {
        arguments[pos].AddToParameters(parameter);
        if (arguments[pos].GetIsStoreValue()) {
            arguments[pos].AddStoreValue(parameter);
        } else if (arguments[pos].GetIsStoreValues()) {
            arguments[pos].AddToStoreValues(parameter);
        }
    }

    template<typename T>
    bool CheckArguments(std::vector<Argument<T>>& arguments, std::string& argument,
                        T parameter, size_t count_dash) {
        for (int i = 0; i < arguments.size(); ++i) {
            if ((arguments[i].GetLongName() == argument || (arguments[i].GetShortName() == argument[0] && argument.size() == 1)) ||
                (count_dash == 1 && arguments[i].GetShortName() == argument[0])) {
                AddParameter(arguments, i, parameter);
                arguments[i].IncreaseCountOfParameters();
                return true;
            }
        }
        return false;
    }

    template<typename T>
    bool CheckCountOfParameters(const std::vector<T>& arguments) {
        for (size_t i = 0; i < arguments.size(); ++i) {
            if (!arguments[i].GetIsParameters()) {
                ThrowError("CheckCountOfParameters: You must to write some parameters for argument!\n");
                return true;
            }
            if (arguments[i].GetIsMultiValue() &&
                (arguments[i].GetCountParameters() < arguments[i].GetCountMinParameters())) {
                ThrowError("CheckCountOfParameters: Count of parameters less than minimal!\n");
                return true;
            }
        }
        return false;
    }

    template<typename T>
    void FindPositional(std::vector<T>& arguments, T*& ptr) {
        for (size_t i = 0; i < arguments.size(); ++i) {
            if (arguments[i].GetIsPositional()) {
                ptr = &arguments[i];
            }
        }
    }
    void ParseIntPositional(const std::vector<std::string>& args, size_t& index);
    void ParseStrPositional(const std::vector<std::string>& args, size_t& index);
    void ParseBoolPositional(const std::vector<std::string>& args, size_t& index);

    template<typename T>
    void DescriptionShortName(const std::vector<T>& arguments, size_t index, std::string& response) const {
        if (arguments[index].GetShortName()) {
            response += "-" + std::string{arguments[index].GetShortName()} + ",  ";
        } else {
            response += "     ";
        }
    }

    void DescriptionAdditionalStrParameters(std::string& response, size_t index) const;
    void DescriptionAdditionalBoolParameters(std::string& response, size_t index) const;
    void DescriptionAdditionalIntParameters(std::string& response, size_t index) const;
    void DescriptionStrArguments(std::string& response) const;
    void DescriptionBoolArguments(std::string& response) const;
    void DescriptionIntArguments(std::string& response) const;
public:
    static const uint32_t kCountOfStarterArguments = 1;
    ArgParser(std::string_view name);

    bool Parse(const std::vector<std::string>& arguments);
    bool Parse(const std::string& arguments);
    bool Parse(int argc, char** argv);

    Argument<std::string>& AddStringArgument(std::string_view long_name, std::string_view description = "");
    Argument<std::string>& AddStringArgument(char short_name, std::string_view long_name, std::string_view description = "");
    Argument<int>& AddIntArgument(std::string_view long_name, std::string_view description = "");
    Argument<int>& AddIntArgument(char short_name, std::string_view long_name, std::string_view description = "");
    Argument<bool>& AddFlag(std::string_view long_name, std::string_view description = "");
    Argument<bool>& AddFlag(char short_name, std::string_view long_name, std::string_view description = "");
    void AddHelp(char short_name, std::string_view long_name, std::string_view description = "");

    std::optional<std::string> GetStringValue(std::string_view long_name, int32_t pos = 0);
    std::optional<std::string> GetStringValue(char short_name, int32_t pos = 0);
    std::optional<int32_t> GetIntValue(std::string_view long_name, int32_t pos = 0);
    std::optional<int32_t> GetIntValue(char short_name, int32_t pos = 0);
    std::optional<bool> GetFlag(std::string_view long_name, int32_t pos = 0);
    std::optional<bool> GetFlag(char short_name, int32_t pos = 0);

    bool Help() const;
    std::string HelpDescription() const;
};

} // namespace ArgumentParser