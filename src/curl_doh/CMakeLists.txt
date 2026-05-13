find_package(CURL REQUIRED)

add_executable(curl_doh main.cpp)
target_link_libraries(curl_doh PRIVATE CURL::libcurl)
