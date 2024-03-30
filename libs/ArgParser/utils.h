#pragma once
#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>
#include <cstring>

void ThrowError(const char* error_message);

std::vector<std::string> SplitString(const std::string& str);

int32_t ParseNumber(const std::string &str);