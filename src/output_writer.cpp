#include "output_writer.hpp"
#include <filesystem>
#include <fstream>

bool write_forecast_csv(const std::string& path,
                        const std::string& series,
                        const std::string& method,
                        const std::vector<std::string>& weeks,
                        const std::vector<double>& values) {
    if (weeks.size() != values.size()) return false;

    std::filesystem::path p(path);
    std::filesystem::create_directories(p.parent_path());

    std::ofstream out(path);
    if (!out.is_open()) return false;

    out << "Series,Method,Week,ForecastValue\n";
    for (std::size_t i = 0; i < weeks.size(); ++i) {
        out << series << ',' << method << ',' << weeks[i] << ',' << values[i] << "\n";
    }
    return true;
}
