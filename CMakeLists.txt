cmake_minimum_required(VERSION 3.18)
project(EconomicForecasting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# vcpkg toolchain integration
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

find_package(Eigen3 CONFIG REQUIRED)
find_package(Matplot++ CONFIG REQUIRED)

add_executable(EconomicForecasting
    src/main.cpp
)

target_link_libraries(EconomicForecasting PRIVATE Eigen3::Eigen Matplot++::matplot)

install(TARGETS EconomicForecasting DESTINATION .)
