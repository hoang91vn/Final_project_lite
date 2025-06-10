#include "utils.hpp"
#include <sstream>
#include <regex>
#include <numeric>
#include <fstream>
#include <map>
#include <iostream>

namespace utils {

bool parse_csv_line(const std::string& line, std::string& date, double& value) {
    std::regex rx(R"(^(\d{4}-\d{2}-\d{2}),([-+]?[0-9]*\.?[0-9]+))");
    std::smatch m;
    if (std::regex_search(line, m, rx)) {
        date = m[1];
        value = std::stod(m[2]);
        return true;
    }
    return false;
}

std::vector<double> moving_average(const std::vector<double>& v, std::size_t window) {
    std::vector<double> out;
    if (v.size() < window) return out;
    for (std::size_t i = window; i <= v.size(); ++i) {
        double sum = std::accumulate(v.begin() + i - window, v.begin() + i, 0.0);
        out.push_back(sum / window);
    }
    return out;
}

std::map<std::string, double> load_fred_csv(const std::string& filePath) {
    std::map<std::string, double> result;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << '\n';
        return result;
    }

    std::string line;
    bool headerFound = false;
    while (std::getline(file, line)) {
        if (!headerFound) {
            if (line.rfind("DATE", 0) == 0 || line.rfind("observation_date", 0) == 0) {
                headerFound = true;
            }
            continue;
        }

        if (line.empty()) {
            continue;
        }

        std::size_t comma = line.find(',');
        if (comma == std::string::npos) {
            std::cerr << "Malformed line: " << line << '\n';
            continue;
        }
        std::string valueToken = line.substr(comma + 1);
        if (valueToken == "." || valueToken == "\"\"") {
            continue;
        }

        std::string date;
        double value;
        if (parse_csv_line(line, date, value)) {
            result[date] = value;
        } else {
            std::cerr << "Malformed line: " << line << '\n';
        }
    }

    return result;
}

} // namespace utils
