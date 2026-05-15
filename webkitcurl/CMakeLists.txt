cmake_minimum_required(VERSION 3.20)
project(playccpp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(CURL REQUIRED)

add_executable(curl_doh main.cpp)
target_link_libraries(curl_doh PRIVATE CURL::libcurl)
