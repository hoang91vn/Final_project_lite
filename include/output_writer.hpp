#ifndef OUTPUT_WRITER_HPP
#define OUTPUT_WRITER_HPP

#include <string>
#include <vector>

bool write_forecast_csv(const std::string& path,
                        const std::string& series,
                        const std::string& method,
                        const std::vector<std::string>& weeks,
                        const std::vector<double>& values);

#endif
