cmake_minimum_required(VERSION 3.18)
project(EconomicForecasting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Location of the static Python distribution
set(PYTHON_ROOT "C:/Python39" CACHE PATH "Python 3.9 root containing static build")

# Find the static python39.lib
find_library(PYTHON39_LIB
    NAMES python39
    PATHS "${PYTHON_ROOT}/static"
    NO_DEFAULT_PATH
    REQUIRED)

# Include directories
include_directories(include "${PYTHON_ROOT}/include")

# Source files
add_executable(EconomicForecasting
    src/main.cpp
    src/data_cleaner.cpp
    src/forecast.cpp
    src/plot.cpp
    src/output_writer.cpp
    src/utils.cpp
)

target_link_libraries(EconomicForecasting PRIVATE ${PYTHON39_LIB})

# Bundle minimal Python runtime after build
set(PY_BUNDLE_DIR "${CMAKE_CURRENT_BINARY_DIR}/python")
set(PYTHON_STDLIB "${PYTHON_ROOT}/Lib")
set(PY_SITE "${PYTHON_STDLIB}/site-packages")

add_custom_command(TARGET EconomicForecasting POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PY_BUNDLE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PYTHON_STDLIB}"
            "${PY_BUNDLE_DIR}/Lib"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PY_SITE}"
            "${PY_BUNDLE_DIR}/Lib/site-packages"
    COMMENT "Copy Python standard library and site-packages")

# Install executable and bundled Python runtime
install(TARGETS EconomicForecasting DESTINATION .)
install(DIRECTORY "${PY_BUNDLE_DIR}/Lib" DESTINATION python)

