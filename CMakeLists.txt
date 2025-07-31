cmake_minimum_required(VERSION 3.10)
project(PipelineRunner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include external/CLI11)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)

file(GLOB SOURCES "src/*.cpp")

add_executable(pipeline_runner ${SOURCES})

target_link_libraries(pipeline_runner
    PRIVATE
    fmt::fmt
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    fftw3
)

