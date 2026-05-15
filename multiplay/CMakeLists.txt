cmake_minimum_required(VERSION 3.20)
project(playccpp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)

# comment all to test 22.1.5 is useable
# clang-cl
# if(MSVC)
#   # target_compile_options(foo PRIVATE /std:c++latest) # 特定时才用这个

#   # must use this or "/std:c++latest" + .clangd(CompileFlags.Add=["/clang:-std=c++23" | "/std:c++latest"])
#   set(CMAKE_CXX23_STANDARD_COMPILE_OPTION "/clang:-std=c++23")
#   # set(CMAKE_CXX23_STANDARD_COMPILE_OPTION "/clang:-std=c++latest") # error: invalid and show all value note
#   # set(CMAKE_CXX23_STANDARD_COMPILE_OPTION "/std:c++latest") # will discard by clangd translation
#   set(CMAKE_CXX23_EXTENSION_COMPILE_OPTION "${CMAKE_CXX23_STANDARD_COMPILE_OPTION}")
#   message("set ${CMAKE_CXX23_STANDARD_COMPILE_OPTION}")
# endif()


# Keep compile_commands.json in source root so clangd can find it.
add_custom_target(UpdateCompileCommandsSymlink ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/compile_commands.json"
        "${CMAKE_SOURCE_DIR}/compile_commands.json"
    COMMENT "Updating compile_commands.json in source root for clangd"
)
