cmake_minimum_required(VERSION 3.20)
project(reversi-client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_CLANG_TIDY "clang-tidy")

set(REVERSI_CLIENT_LIB reversi-client-lib)
set(REVERSI_CLIENT reversi-client)
set(REVERSI_CLIENT_INCLUDE "${PROJECT_SOURCE_DIR}/include")

add_subdirectory(bin)
add_subdirectory(src/reversi/client)

# enable_testing()
# add_subdirectory(test)
