#pragma once

#include <string>

int visualLength(const std::string& s);

std::wstring utf8_to_wstring(const std::string& s);

std::string wstring_to_utf8(const std::wstring& ws);

std::string utf8_substr(const std::string& s, size_t max_chars);

std::string strToPrint(std::string s, int size);

