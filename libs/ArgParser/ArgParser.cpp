#include "ArgParser.h"

ArgumentParser::ArgParser::ArgParser(std::string_view name): name_(name) {}

void ArgumentParser::ArgParser::ParseIntPositional(const std::vector<std::string>& args, size_t& index) {
    for (; index < args.size() && args[index].front() != '-'; ++index) {
        int_pos_->AddToParameters(ParseNumber(args[index]));
        if (int_pos_->GetIsStoreValue()) {
            int_pos_->AddStoreValue(ParseNumber(args[index]));
        } else if (int_pos_->GetIsStoreValues()) {
            int_pos_->AddToStoreValues(ParseNumber(args[index]));
        }
        int_pos_->IncreaseCountOfParameters();
    }
}

void ArgumentParser::ArgParser::ParseStrPositional(const std::vector<std::string>& args, size_t& index) {
    for (; index < args.size() && args[index].front() != '-'; ++index) {
        str_pos_->AddToParameters(args[index]);
        if (str_pos_->GetIsStoreValue()) {
            str_pos_->AddStoreValue(args[index]);
        } else if (str_pos_->GetIsStoreValues()) {
            str_pos_->AddToStoreValues(args[index]);
        }
        str_pos_->IncreaseCountOfParameters();
    }
}

void ArgumentParser::ArgParser::ParseBoolPositional(const std::vector<std::string>& args, size_t& index) {
    for (; index < args.size() && args[index].front() != '-'; ++index) {
        bool flag = (args[index].front() == '1');
        bool_pos_->AddToParameters(flag);
        if (bool_pos_->GetIsStoreValue()) {
            bool_pos_->AddStoreValue(flag);
        } else if (bool_pos_->GetIsStoreValues()) {
            bool_pos_->AddToStoreValues(flag);
        }
        bool_pos_->IncreaseCountOfParameters();
    }
}

bool ArgumentParser::ArgParser::Parse(const std::vector<std::string>& arguments) {
    FindPositional(int_arguments_, int_pos_);
    FindPositional(str_arguments_, str_pos_);
    FindPositional(bool_arguments_, bool_pos_);
    for (size_t i = kCountOfStarterArguments; i < arguments.size(); ++i) {
        const std::string& cmd = arguments[i];
        size_t pos_delimiter = cmd.find('=');
        size_t pos_last_dash = cmd.rfind('-');
        size_t count_dash = pos_last_dash + 1;
        // If positional parameter
        if (pos_delimiter == std::string::npos && pos_last_dash == std::string::npos) {
            if (int_pos_ == nullptr && str_pos_ == nullptr && bool_pos_ == nullptr) {
                return false;
            }
            if (int_pos_) {
                ParseIntPositional(arguments, i);
            } else if (str_pos_) {
                ParseStrPositional(arguments, i);
            } else if (bool_pos_) {
                ParseBoolPositional(arguments, i);
            }
            continue;
        }
        std::string argument = cmd.substr(pos_last_dash + 1, pos_delimiter - pos_last_dash - 1);
        if (argument == long_help_name_ || argument == short_help_name_) {
            is_help_ = true;
            break;
        }
        // If flag parameter
        if (pos_delimiter == std::string::npos) {
            if (count_dash == kCountOfDashLongName) {
                if (CheckArguments(bool_arguments_, argument, true, count_dash)) {
                    continue;
                }
                ThrowError("Argument didn't found!");
                return false;
            }
            for (size_t i = 0; i < argument.size(); ++i) {
                std::string ch{argument[i]};
                if (!CheckArguments(bool_arguments_, ch, true, count_dash)) {
                    ThrowError("Argument didn't found!");
                    return false;
                }
            }
            continue;
        }
        std::string parameter = cmd.substr(pos_delimiter + 1, cmd.size());
        // If string or int parameter
        if (CheckArguments(str_arguments_, argument, parameter, count_dash) ||
            CheckArguments(int_arguments_, argument, ParseNumber(parameter), count_dash)) {
            continue;
        }
        ThrowError("Argument didn't found!\n");
        return false;
    }
    if (is_help_) {
        return true;
    }
    if (CheckCountOfParameters(int_arguments_) || CheckCountOfParameters(str_arguments_) ||
        CheckCountOfParameters(bool_arguments_)) {
        return false;
    }
    return true;
}

bool ArgumentParser::ArgParser::Parse(const std::string& str) {
    std::vector<std::string> arguments;
    std::istringstream iss(str);
    std::string argument;
    while (iss >> argument) {
        arguments.push_back(argument);
    }
    return Parse(arguments);
}

bool ArgumentParser::ArgParser::Parse(int argc, char** argv) {
    std::vector<std::string> arguments;
    for (size_t i = 0; i < argc; ++i) {
        std::string str = argv[i];
        arguments.push_back(str);
    }
    return Parse(arguments);
}

Argument<std::string> &ArgumentParser::ArgParser::AddStringArgument(std::string_view long_name,
                                                                    std::string_view description) {
    str_arguments_.emplace_back(long_name, NULL, description);
    return str_arguments_[str_arguments_.size() - 1];
}

Argument<std::string> &ArgumentParser::ArgParser::AddStringArgument(char short_name, std::string_view long_name,
                                                                    std::string_view description) {
    str_arguments_.emplace_back(long_name, short_name, description);
    return str_arguments_.back();
}

Argument<int> &ArgumentParser::ArgParser::AddIntArgument(std::string_view long_name,
                                                                    std::string_view description) {
    int_arguments_.emplace_back(long_name, NULL, description);
    return int_arguments_.back();
}

Argument<int> &ArgumentParser::ArgParser::AddIntArgument(char short_name, std::string_view long_name,
                                                                    std::string_view description) {
    int_arguments_.emplace_back(long_name, short_name, description);
    return int_arguments_.back();
}

Argument<bool> &ArgumentParser::ArgParser::AddFlag(std::string_view long_name,
                                                                    std::string_view description) {
    bool_arguments_.emplace_back(long_name, NULL, description);
    bool_arguments_[bool_arguments_.size() - 1].SetFirstParameter(false);
    return bool_arguments_.back();
}

Argument<bool> &ArgumentParser::ArgParser::AddFlag(char short_name, std::string_view long_name,
                                                                    std::string_view description) {
    bool_arguments_.emplace_back(long_name, short_name, description);
    bool_arguments_[bool_arguments_.size() - 1].SetFirstParameter(false);
    return bool_arguments_.back();
}

void ArgumentParser::ArgParser::AddHelp(char short_name, std::string_view long_name,
                                                   std::string_view description) {
    long_help_name_ = long_name;
    short_help_name_ = short_name;
    help_description_ = description;
}

std::optional<std::string> ArgumentParser::ArgParser::GetStringValue(std::string_view long_name, int32_t pos) {
    for (size_t i = 0; i < str_arguments_.size(); ++i) {
        if (str_arguments_[i].GetLongName() != long_name) {
            continue;
        }
        if ((str_arguments_[i].GetCountParameters() == 0 && pos == 0) ||
            (pos > 0 && !str_arguments_[i].GetIsMultiValue()) ||
            (pos + 1 > str_arguments_[i].GetCountParameters())) {

            if (str_arguments_[i].GetCountParameters() == 0 && pos == 0) {
                ThrowError("GetStrValue: Please, write at least one parameter");
            } else if (pos > 0 && !str_arguments_[i].GetIsMultiValue()) {
                ThrowError("GetStrValue: Set MultiValue to have a few parameters");
            } else {
                ThrowError("GetStrValue: Position out of range!");
            }
            return std::nullopt;
        }
        return str_arguments_[i].GetParameter(pos);
    }
    ThrowError("GetStringValue: Argument didn't found!");
    return std::nullopt;
}

std::optional<std::string> ArgumentParser::ArgParser::GetStringValue(char short_name, int32_t pos) {
    for (size_t i = 0; i < str_arguments_.size(); ++i) {
        if (str_arguments_[i].GetShortName() != short_name) {
            continue;
        }
        if ((str_arguments_[i].GetCountParameters() == 0 && pos == 0) ||
            (pos > 0 && !str_arguments_[i].GetIsMultiValue()) ||
            (pos + 1 > str_arguments_[i].GetCountParameters())) {

            if (str_arguments_[i].GetCountParameters() == 0 && pos == 0) {
                ThrowError("GetStrValue: Please, write at least one parameter");
            } else if (pos > 0 && !str_arguments_[i].GetIsMultiValue()) {
                ThrowError("GetStrValue: Set MultiValue to have a few parameters");
            } else {
                ThrowError("GetStrValue: Position out of range!");
            }
            return std::nullopt;
        }
        return str_arguments_[i].GetParameter(pos);
    }
    ThrowError("GetStringValue: Argument didn't found!");
    return std::nullopt;
}

std::optional<int32_t> ArgumentParser::ArgParser::GetIntValue(std::string_view long_name, int32_t pos) {
    for (size_t i = 0; i < int_arguments_.size(); ++i) {
        if (int_arguments_[i].GetLongName() != long_name) {
            continue;
        }
        if ((int_arguments_[i].GetCountParameters() == 0 && pos == 0) ||
            (pos > 0 && !int_arguments_[i].GetIsMultiValue()) ||
            (pos + 1 > int_arguments_[i].GetCountParameters())) {

            if (int_arguments_[i].GetCountParameters() == 0 && pos == 0) {
                ThrowError("GetIntValue: Please, write at least one parameter");
            } else if (pos > 0 && !int_arguments_[i].GetIsMultiValue()) {
                ThrowError("GetIntValue: Set MultiValue to have a few parameters");
            } else {
                ThrowError("GetIntValue: Position out of range!");
            }
            return std::nullopt;
        }
        return int_arguments_[i].GetParameter(pos);
    }
    ThrowError("GetIntValue: Argument didn't found!");
    return std::nullopt;
}

std::optional<int32_t> ArgumentParser::ArgParser::GetIntValue(char short_name, int32_t pos) {
    for (size_t i = 0; i < int_arguments_.size(); ++i) {
        if (int_arguments_[i].GetShortName() != short_name) {
            continue;
        }
        if ((int_arguments_[i].GetCountParameters() == 0 && pos == 0) ||
            (pos > 0 && !int_arguments_[i].GetIsMultiValue()) ||
            (pos + 1 > int_arguments_[i].GetCountParameters())) {

            if (int_arguments_[i].GetCountParameters() == 0 && pos == 0) {
                ThrowError("GetIntValue: Please, write at least one parameter");
            } else if (pos > 0 && !int_arguments_[i].GetIsMultiValue()) {
                ThrowError("GetIntValue: Set MultiValue to have a few parameters");
            } else {
                ThrowError("GetIntValue: Position out of range!");
            }
            return std::nullopt;
        }
        return int_arguments_[i].GetParameter(pos);
    }
    ThrowError("GetIntValue: Argument didn't found!");
    return std::nullopt;
}

std::optional<bool> ArgumentParser::ArgParser::GetFlag(std::string_view long_name, int32_t pos) {
    for (size_t i = 0; i < bool_arguments_.size(); ++i) {
        if (bool_arguments_[i].GetLongName() != long_name) {
            continue;
        }
        if ((bool_arguments_[i].GetCountParameters() == 0 && pos == 0) ||
            (pos > 0 && !bool_arguments_[i].GetIsMultiValue()) ||
            (pos + 1 > bool_arguments_[i].GetCountParameters())) {

            if (bool_arguments_[i].GetCountParameters() == 0 && pos == 0) {
                ThrowError("GetFlag: Please, write at least one parameter");
            } else if (pos > 0 && !bool_arguments_[i].GetIsMultiValue()) {
                ThrowError("GetFlag: Set MultiValue to have a few parameters");
            } else {
                ThrowError("GetFlag: Position out of range!");
            }
            return std::nullopt;
        }
        return bool_arguments_[i].GetParameter(pos);
    }
    ThrowError("GetFlag: Argument didn't found!");
    return std::nullopt;
}

std::optional<bool> ArgumentParser::ArgParser::GetFlag(char short_name, int32_t pos) {
    for (size_t i = 0; i < bool_arguments_.size(); ++i) {
        if (bool_arguments_[i].GetShortName() != short_name) {
            continue;
        }
        if ((bool_arguments_[i].GetCountParameters() == 0 && pos == 0) ||
            (pos > 0 && !bool_arguments_[i].GetIsMultiValue()) ||
            (pos + 1 > bool_arguments_[i].GetCountParameters())) {

            if (bool_arguments_[i].GetCountParameters() == 0 && pos == 0) {
                ThrowError("GetFlag: Please, write at least one parameter");
            } else if (pos > 0 && !bool_arguments_[i].GetIsMultiValue()) {
                ThrowError("GetFlag: Set MultiValue to have a few parameters");
            } else {
                ThrowError("GetFlag: Position out of range!");
            }
            return std::nullopt;
        }
        return bool_arguments_[i].GetParameter(pos);
    }
    ThrowError("GetFlag: Argument didn't found!");
    return std::nullopt;
}

bool ArgumentParser::ArgParser::Help() const {
    return is_help_;
}

void ArgumentParser::ArgParser::DescriptionAdditionalStrParameters(std::string &response, size_t index) const {
    size_t count_args = str_arguments_[index].GetIsMultiValue() + str_arguments_[index].GetIsPositional() +
                        str_arguments_[index].GetIsDefault();
    response += " [";
    if (str_arguments_[index].GetIsDefault()) {
        response += "default = " + str_arguments_[index].GetDefault();
        count_args--;
        if (count_args) {
            response += "; ";
        }
    }
    if (str_arguments_[index].GetIsMultiValue()) {
        response += "repeated";
        if (str_arguments_[index].GetCountMinParameters()) {
            response += ", min args = ";
            response += std::to_string(str_arguments_[index].GetCountMinParameters());
        }
        count_args--;
        if (count_args) {
            response += "; ";
        }
    }
    if (str_arguments_[index].GetIsPositional()) {
        response += "positional";
    }
    response += "]";
}

void ArgumentParser::ArgParser::DescriptionAdditionalBoolParameters(std::string &response, size_t index) const {
    size_t count_args = bool_arguments_[index].GetIsMultiValue() + bool_arguments_[index].GetIsPositional() +
                        bool_arguments_[index].GetIsDefault();
    response += " [";
    if (bool_arguments_[index].GetIsDefault()) {
        response += "default = ";
        if (bool_arguments_[index].GetIsDefault()) {
            response += "true";
        } else {
            response += "false";
        }
        count_args--;
        if (count_args) {
            response += "; ";
        }
    }
    if (bool_arguments_[index].GetIsMultiValue()) {
        response += "repeated";
        if (bool_arguments_[index].GetCountMinParameters()) {
            response += ", min args = ";
            response += std::to_string(bool_arguments_[index].GetCountMinParameters());
        }
        count_args--;
        if (count_args) {
            response += "; ";
        }
    }
    if (bool_arguments_[index].GetIsPositional()) {
        response += "positional";
    }
    response += "]";
}

void ArgumentParser::ArgParser::DescriptionAdditionalIntParameters(std::string &response, size_t index) const {
    size_t count_args = int_arguments_[index].GetIsMultiValue() + int_arguments_[index].GetIsPositional() +
                        int_arguments_[index].GetIsDefault();
    response += " [";
    if (int_arguments_[index].GetIsDefault()) {
        response += "default = " + std::to_string(int_arguments_[index].GetDefault());
        count_args--;
        if (count_args) {
            response += "; ";
        }
    }
    if (int_arguments_[index].GetIsMultiValue()) {
        response += "repeated";
        if (int_arguments_[index].GetCountMinParameters()) {
            response += ", min args = ";
            response += std::to_string(int_arguments_[index].GetCountMinParameters());
        }
        count_args--;
        if (count_args) {
            response += "; ";
        }
    }
    if (int_arguments_[index].GetIsPositional()) {
        response += "positional";
    }
    response += "]";
}

void ArgumentParser::ArgParser::DescriptionStrArguments(std::string& response) const {
    for (size_t i = 0; i < str_arguments_.size(); ++i) {
        DescriptionShortName(str_arguments_, i, response);
        response += "--" + str_arguments_[i].GetLongName() + "=<string>,  ";
        response += str_arguments_[i].GetDescription();
        if (str_arguments_[i].GetIsMultiValue() + str_arguments_[i].GetIsPositional() +
            str_arguments_[i].GetIsDefault()) {
            DescriptionAdditionalStrParameters(response, i);
        }
        response += "\n";
    }
}

void ArgumentParser::ArgParser::DescriptionBoolArguments(std::string &response) const {
    for (size_t i = 0; i < bool_arguments_.size(); ++i) {
        DescriptionShortName(bool_arguments_, i, response);
        response += "--" + bool_arguments_[i].GetLongName() + ",  ";
        response += bool_arguments_[i].GetDescription();
        if (bool_arguments_[i].GetIsMultiValue() + bool_arguments_[i].GetIsPositional() +
            bool_arguments_[i].GetIsDefault()) {
            DescriptionAdditionalBoolParameters(response, i);
        }
        response += "\n";
    }
}

void ArgumentParser::ArgParser::DescriptionIntArguments(std::string &response) const {
    for (size_t i = 0; i < int_arguments_.size(); ++i) {
        DescriptionShortName(int_arguments_, i, response);
        response += "--" + int_arguments_[i].GetLongName() + "=<int>,  ";
        response += int_arguments_[i].GetDescription();
        if (int_arguments_[i].GetIsMultiValue() + int_arguments_[i].GetIsPositional() +
            int_arguments_[i].GetIsDefault()) {
            DescriptionAdditionalIntParameters(response, i);
        }
        response += "\n";
    }
}

std::string ArgumentParser::ArgParser::HelpDescription() const {
    std::string response;
    response += name_ + "\n";
    response += help_description_ + "\n\n";
    DescriptionStrArguments(response);
    DescriptionBoolArguments(response);
    DescriptionIntArguments(response);
    response += "\n";
    response += "-" + short_help_name_ + ", ";
    response += "--" + long_help_name_ + "\n";
    return response;
}