#ifndef DATA_CLEANER_HPP
#define DATA_CLEANER_HPP

#include <map>
#include <string>
#include <limits>

struct RowData {
    double icsa = std::numeric_limits<double>::quiet_NaN();
    double unrate = std::numeric_limits<double>::quiet_NaN();
    double jtsjol = std::numeric_limits<double>::quiet_NaN();
};

std::map<std::string, RowData> merge_data(const std::string& icsaPath,
                                          const std::string& unratePath,
                                          const std::string& jtsjolPath,
                                          const std::string& startDate,
                                          const std::string& endDate);

#endif
