#include "utils.hpp"
#include <sstream>
#include <regex>
#include <numeric>
#include <fstream>

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

std::map<std::string, double> load_fred_csv(const std::string& path) {
    std::map<std::string, double> out;
    std::ifstream file(path);
    if (!file.is_open()) return out;
    std::string line;
    std::getline(file, line); // skip header
    while (std::getline(file, line)) {
        std::string date;
        double val;
        if (parse_csv_line(line, date, val)) {
            out[date] = val;
        }
    }
    return out;
}
