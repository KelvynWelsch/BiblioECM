#pragma once

#include <string>
#include <chrono>
#include <format>

inline std::string timeToString(const std::chrono::system_clock::time_point& tp) {
	std::chrono::sys_time<std::chrono::days> tp_day = std::chrono::floor<std::chrono::days>(tp);
	const std::chrono::time_zone* fuso = std::chrono::current_zone();
	std::chrono::zoned_time tempo_local(fuso, tp_day);
	return std::format("{:%Y%m%d}", tempo_local);
}

inline std::string timeToString(const std::chrono::year_month_day& tp) {
	return std::format("{:%Y%m%d}", tp);
}

inline std::chrono::year_month_day today() {
	return std::chrono::year_month_day{ std::chrono::floor<std::chrono::days>(
		std::chrono::system_clock::now()) };
}

inline std::string timeToStringUser(const std::chrono::year_month_day& tp) {
	return std::format("{:%d/%m/%Y}", tp);
}

inline std::chrono::year_month_day parseDate(const std::string& s) {
	int y = std::stoi(s.substr(0, 4));
	unsigned m = static_cast<unsigned>(std::stoi(s.substr(4, 2)));
	unsigned d = static_cast<unsigned>(std::stoi(s.substr(6, 2)));
	return std::chrono::year{ y } /
		std::chrono::month{ m } /
		std::chrono::day{ d };
}

