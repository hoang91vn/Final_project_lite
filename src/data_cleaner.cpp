#include "data_cleaner.hpp"
#include "utils.hpp"
#include <fstream>
#include <regex>
#include <iostream>
#include <limits>

namespace {
bool write_weekly_csv(const std::string& path,
                      const std::map<std::string, RowData>& table) {
    std::ofstream out(path);
    if (!out.is_open()) return false;
    out << "Date,ICSA,UNRATE,JTSJOL\n";
    for (const auto& [date, row] : table) {
        out << date << ',' << row.icsa << ',' << row.unrate << ',' << row.jtsjol << "\n";
    }
    return true;
}
} // namespace

std::map<std::string, RowData> merge_data(const std::string& icsaPath,
                                          const std::string& unratePath,
                                          const std::string& jtsjolPath,
                                          const std::string& startDate,
                                          const std::string& endDate) {
    auto icsaMap = load_fred_csv(icsaPath);
    auto unrateMap = load_fred_csv(unratePath);
    auto jtsjolMap = load_fred_csv(jtsjolPath);

    if (icsaMap.empty() || unrateMap.empty() || jtsjolMap.empty()) {
        if (icsaMap.empty()) std::cerr << "ICSA data missing\n";
        if (unrateMap.empty()) std::cerr << "UNRATE data missing\n";
        if (jtsjolMap.empty()) std::cerr << "JTSJOL data missing\n";
        return {};
    }

    auto fetch_latest = [](const std::map<std::string, double>& m,
                           const std::string& date) {
        auto it = m.upper_bound(date);
        if (it == m.begin()) return std::numeric_limits<double>::quiet_NaN();
        --it;
        return it->second;
    };

    std::map<std::string, RowData> table;
    for (const auto& [date, val] : icsaMap) {
        if (date < startDate || date > endDate) continue;
        RowData row;
        row.icsa = val;
        row.unrate = fetch_latest(unrateMap, date);
        row.jtsjol = fetch_latest(jtsjolMap, date);
        table[date] = row;
    }

    write_weekly_csv("data/weekly_data.csv", table);
    return table;
}
