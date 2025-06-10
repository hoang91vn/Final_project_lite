#include "forecast.hpp"
#include <numeric>
#include <cmath>

std::vector<double> forecast_sma(const std::vector<double>& series,
                                 std::size_t window,
                                 std::size_t steps) {
    std::vector<double> forecasts;
    if (window == 0 || series.size() < window) return forecasts;

    std::vector<double> extended(series.begin(), series.end());
    forecasts.reserve(steps);

    for (std::size_t i = 0; i < steps; ++i) {
        double sum = 0.0;
        for (std::size_t j = extended.size() - window; j < extended.size(); ++j) {
            sum += extended[j];
        }
        double val = sum / static_cast<double>(window);
        forecasts.push_back(val);
        extended.push_back(val);
    }

    return forecasts;
}

std::vector<double> forecast_exp(const std::vector<double>& series,
                                 double alpha,
                                 std::size_t steps) {
    std::vector<double> forecasts;
    if (series.empty()) return forecasts;

    double s = series.front();
    for (std::size_t i = 1; i < series.size(); ++i) {
        s = alpha * series[i] + (1.0 - alpha) * s;
    }

    forecasts.assign(steps, s);
    return forecasts;
}
