cmake_minimum_required(VERSION 3.18)
project(EconomicForecasting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Python / NumPy (for matplotlib-cpp)
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)

# Include directories
include_directories(include)
include_directories(${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})

# Source files
add_executable(EconomicForecasting
    src/main.cpp
    src/data_cleaner.cpp
    src/forecast.cpp
    src/plot.cpp
    src/output_writer.cpp
    src/utils.cpp
)

target_link_libraries(EconomicForecasting PRIVATE ${Python3_LIBRARIES})
