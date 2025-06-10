#include "plot.hpp"
#include "matplotlibcpp.h"
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <Python.h>

namespace plt = matplotlibcpp;

static bool g_checked_matplotlib = false;
static bool g_has_matplotlib = false;

void plot_series(const std::string& title,
                 const std::vector<std::string>& dates,
                 const std::vector<double>& raw,
                 const std::vector<double>& trend,
                 const std::vector<std::string>& forecastDates,
                 const std::vector<double>& forecastVals,
                 const std::string& outputPng) {
    if (!g_checked_matplotlib) {
        g_checked_matplotlib = true;
        if (!Py_IsInitialized()) {
            Py_Initialize();
        }
        PyObject* module = PyImport_ImportModule("matplotlib");
        if (!module) {
            PyErr_Clear();
            std::cerr << "Warning: matplotlib not found. Skipping PNG output." << std::endl;
            g_has_matplotlib = false;
        } else {
            g_has_matplotlib = true;
            Py_DECREF(module);
        }
    }

    if (!g_has_matplotlib) {
        return;
    }

    std::filesystem::create_directories("output");

    plt::figure_size(1200, 780);
    plt::plot(dates, raw, {{"color", "blue"}});

    if (!trend.empty()) {
        std::size_t offset = dates.size() - trend.size();
        std::vector<std::string> trendDates(dates.begin() + offset, dates.end());
        plt::plot(trendDates, trend, {"r--"});
    }

    if (!forecastDates.empty() && forecastDates.size() == forecastVals.size()) {
        plt::plot(forecastDates, forecastVals, {"go"});
    }

    plt::xlabel("Date");
    plt::ylabel("Value");
    plt::title(title);
    plt::legend();
    plt::save(outputPng);
    plt::close();
}

void ascii_sparkline(const std::string& title,
                     const std::vector<double>& data) {
    static const char* blocks[] = {"▁","▂","▃","▄","▅","▆","▇","█"};

    if (data.empty()) {
        std::cout << "No data for " << title << std::endl;
        return;
    }

    double minVal = *std::min_element(data.begin(), data.end());
    double maxVal = *std::max_element(data.begin(), data.end());

    std::cout << title << '\n';
    std::string line;
    for (double v : data) {
        double norm = 0.0;
        if (maxVal > minVal) {
            norm = (v - minVal) / (maxVal - minVal);
        }
        int bin = static_cast<int>(std::floor(norm * 7.0 + 0.5));
        bin = std::clamp(bin, 0, 7);
        line += blocks[bin];
    }
    std::cout << line << std::endl;
}
