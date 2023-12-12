#pragma once
#include "../../utils.h"
#include <string>
#include <vector>

template<typename T>
class Argument {
private:
    std::string long_name_;
    char short_name_;
    std::string description_;
    bool is_multi_value_ = false;
    bool is_positional_ = false;
    bool is_default_ = false;
    T default_value_;
    size_t count_min_parameters_ = 0;
    size_t count_parameters_ = 0;
    std::vector<T> parameters_;
    T* store_value_ = nullptr;
    std::vector<T>* store_values_ = nullptr;
public:
    Argument(std::string_view long_name, char short_name = '\0', std::string_view description = ""):
            long_name_(long_name), short_name_(short_name), description_(description) {}

    std::string GetLongName() const {
        return long_name_;
    }

    char GetShortName() const {
        return short_name_;
    }

    std::string GetDescription() const {
        return description_;
    }

    T GetParameter(uint32_t pos) const {
        return parameters_[pos];
    }

    T GetDefault() const {
        return default_value_;
    }

    bool GetIsParameters() const {
        if (parameters_.empty()) {
            return false;
        }
        return true;
    }

    bool GetIsStoreValue() const {
        if (store_value_ == nullptr) {
            return false;
        }
        return true;
    }

    bool GetIsStoreValues() const {
        if (store_values_ == nullptr) {
            return false;
        }
        return true;
    }

    bool GetIsMultiValue() const {
        return is_multi_value_;
    }

    bool GetIsPositional() const {
        return is_positional_;
    }

    bool GetIsDefault() const {
        return is_default_;
    }

    size_t GetCountMinParameters() const {
        return count_min_parameters_;
    }

    size_t GetCountParameters() const {
        return count_parameters_;
    }

    void SetFirstParameter(const T& parameter) {
        count_parameters_++;
        parameters_.push_back(parameter);
    }

    void IncreaseCountOfParameters() {
        count_parameters_++;
    }

    void AddStoreValue(const T& value) {
        *store_value_ = value;
    }

    void AddToStoreValues(const T& value) {
        store_values_->push_back(value);
    }

    Argument<T>& Default(const T& default_value) {
        is_default_ = true;
        default_value_ = default_value;
        if (count_parameters_ > 0) {
            parameters_[0] = default_value;
        } else {
            count_parameters_++;
            parameters_.push_back(default_value);
        }
        return *this;
    }

    Argument<T>& AddToParameters(const T& value) {
        if (is_multi_value_ || parameters_.empty()) {
            parameters_.push_back(value);
        } else {
            parameters_[0] = value;
        }
        return *this;
    }

    Argument<T>& MultiValue(size_t count_min_parameters = 0) {
        is_multi_value_ = true;
        count_min_parameters_ = count_min_parameters;
        return *this;
    }

    Argument<T>& Positional() {
        is_positional_ = true;
        return *this;
    }

    void StoreValue(T& value) {
        store_value_ = &value;
        if (!parameters_.empty()) {
            *store_value_ = parameters_[0];
        }
    }

    void StoreValues(std::vector<T>& vec) {
        store_values_ = &vec;
        if (!parameters_.empty()) {
            vec.push_back(parameters_[0]);
        }
    }
};
