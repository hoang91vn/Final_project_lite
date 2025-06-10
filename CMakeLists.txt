cmake_minimum_required(VERSION 3.18)
project(EconomicForecasting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------------
# Python interpreter check
# ------------------------------------------------------------------------------
find_package(Python3 COMPONENTS Interpreter Development QUIET)

set(PY_BUNDLE_DIR "${CMAKE_CURRENT_BINARY_DIR}/python")

if(NOT Python3_FOUND)
    message(STATUS "No system Python found - downloading embeddable 3.9 runtime")
    include(ExternalProject)

    set(PY_EMBED_URL "https://www.python.org/ftp/python/3.9.13/python-3.9.13-embed-amd64.zip")

    ExternalProject_Add(EmbeddedPython
        URL                ${PY_EMBED_URL}
        PREFIX             "${CMAKE_BINARY_DIR}/PythonEmbed"
        SOURCE_DIR         "${PY_BUNDLE_DIR}"
        CONFIGURE_COMMAND  ""
        BUILD_COMMAND      ""
        INSTALL_COMMAND    ""
    )

    # After extraction bootstrap pip and install required packages
    ExternalProject_Add_Step(EmbeddedPython bootstrap
        COMMAND "${PY_BUNDLE_DIR}/python.exe" -m ensurepip
        COMMAND "${PY_BUNDLE_DIR}/python.exe" -m pip install matplotlib numpy
        DEPENDEES download
        WORKING_DIRECTORY "${PY_BUNDLE_DIR}"
    )

    set(Python3_EXECUTABLE "${PY_BUNDLE_DIR}/python.exe")

    # Locate the python library inside the embeddable package
    find_library(PYTHON_LIB NAMES python39 python39.dll
        PATHS "${PY_BUNDLE_DIR}" "${PY_BUNDLE_DIR}/DLLs" NO_DEFAULT_PATH)
    set(Python3_LIBRARIES ${PYTHON_LIB})
endif()

# Include directories
include_directories(include)
include_directories(${Python3_INCLUDE_DIRS})

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

# Ensure embedded Python is prepared before building the executable
if(TARGET EmbeddedPython)
    add_dependencies(EconomicForecasting EmbeddedPython)
endif()

# ------------------------------------------------------------------------------
# Bundle minimal Python runtime after build
# ------------------------------------------------------------------------------
if(Python3_FOUND)
    # Determine the site-packages directory for the detected interpreter
    execute_process(
        COMMAND ${Python3_EXECUTABLE} -c "import sysconfig,sys;sys.stdout.write(sysconfig.get_path('purelib'))"
        OUTPUT_VARIABLE PY_SITE
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(
        COMMAND ${Python3_EXECUTABLE} -c "import sysconfig,sys;sys.stdout.write(sysconfig.get_path('stdlib'))"
        OUTPUT_VARIABLE PY_STDLIB
        OUTPUT_STRIP_TRAILING_WHITESPACE)
else()
    set(PY_SITE "${PY_BUNDLE_DIR}/Lib/site-packages")
    set(PY_STDLIB "${PY_BUNDLE_DIR}/Lib")
endif()

add_custom_command(TARGET EconomicForecasting POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PY_BUNDLE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PY_STDLIB}"
            "${PY_BUNDLE_DIR}/Lib"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PY_SITE}"
            "${PY_BUNDLE_DIR}/Lib/site-packages"
    COMMENT "Copy Python standard library and site-packages")

# Install executable and bundled Python runtime
install(TARGETS EconomicForecasting DESTINATION .)
install(DIRECTORY "${PY_BUNDLE_DIR}/Lib" DESTINATION python)

