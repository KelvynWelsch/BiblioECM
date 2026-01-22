#include "strings.h"
#include <windows.h>

 int visualLength(const std::string& s) {
	if (s.empty()) return 0;

	int wlen = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
	if (wlen == 0) return s.size();

	std::wstring ws(wlen, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, ws.data(), wlen);

	return static_cast<int>(ws.length() - 1); // tira o \0
}

 std::wstring utf8_to_wstring(const std::string& s) {
	if (s.empty()) return L"";

	int len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
	std::wstring ws(len, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, ws.data(), len);
	ws.pop_back(); // remove \0
	return ws;
}

 std::string wstring_to_utf8(const std::wstring& ws) {
	if (ws.empty()) return "";

	int len = WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string s(len, '\0');
	WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, s.data(), len, nullptr, nullptr);
	s.pop_back(); // remove \0
	return s;
}

 std::string utf8_substr(const std::string& s, size_t max_chars) {
	auto ws = utf8_to_wstring(s);

	if (ws.size() <= max_chars)
		return s;

	auto cut = ws.substr(0, max_chars);
	return wstring_to_utf8(cut);
}


 std::string strToPrint(std::string s, int size) {
	if (size >= 0) {
		return s + std::string((size_t)size, ' ');
	}
	else {
		int limit = visualLength(s) + size - 3;
		return utf8_substr(s, limit) + "...";
	}
}
