#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

// Split "YYYY-MM-DD,value"  →  {date, value}
bool parse_csv_line(const std::string& line, std::string& date, double& value);

// Simple moving-average helper
std::vector<double> moving_average(const std::vector<double>& v, std::size_t window);

#endif
