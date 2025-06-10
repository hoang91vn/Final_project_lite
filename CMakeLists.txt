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

# Bundle minimal Python runtime after build
set(PY_BUNDLE_DIR "${CMAKE_CURRENT_BINARY_DIR}/python")
# Determine the site-packages directory at configure time
execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import sysconfig,sys;sys.stdout.write(sysconfig.get_path('purelib'))"
    OUTPUT_VARIABLE PY_SITE
    OUTPUT_STRIP_TRAILING_WHITESPACE)

add_custom_command(TARGET EconomicForecasting POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PY_BUNDLE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${Python3_STDLIB}"
            "${PY_BUNDLE_DIR}/Lib"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PY_SITE}"
            "${PY_BUNDLE_DIR}/Lib/site-packages"
    COMMENT "Copy Python standard library and site-packages")

# Install executable and bundled Python runtime
install(TARGETS EconomicForecasting DESTINATION .)
install(DIRECTORY "${PY_BUNDLE_DIR}/Lib" DESTINATION python)

