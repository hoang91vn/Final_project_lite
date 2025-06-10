#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <map>

namespace utils {

// Split "YYYY-MM-DD,value"  →  {date, value}
bool parse_csv_line(const std::string& line, std::string& date, double& value);

// Simple moving-average helper
std::vector<double> moving_average(const std::vector<double>& v, std::size_t window);

// Load FRED formatted CSV file and return {date -> value}
std::map<std::string, double> load_fred_csv(const std::string& filePath);

} // namespace utils

#endif
