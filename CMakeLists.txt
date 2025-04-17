cmake_minimum_required(VERSION 3.20)
project(reversi-client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_CLANG_TIDY "clang-tidy")

set(REVERSI_CLIENT reversi-client)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(
  SOURCES
  app.cpp
  window.cpp
  context.cpp
  menu.cpp
  wait.cpp
  play.cpp
)

add_executable(
  ${REVERSI_CLIENT}
  ${SOURCES}
  main.cpp
)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network WebSockets Test)
include_directories(${Qt6_INCLUDE_DIRS})

include(FetchContent)
FetchContent_Declare(
  reversi-contract
  GIT_REPOSITORY https://github.com/reversi-project/contract.git
  GIT_TAG master
)
FetchContent_MakeAvailable(reversi-contract)

target_link_libraries(
  ${REVERSI_CLIENT}
  Qt6::Widgets Qt6::Gui Qt6::Core Qt6::Network Qt6::WebSockets
  reversi-contract-lib
)
target_include_directories(
  ${REVERSI_CLIENT} PUBLIC
  ${reversi-contract_SOURCE_DIR}/include
)

enable_testing()
qt_add_executable(
  test_app
  test_main.cpp
  ${SOURCES}
  utils.cpp
)
add_test(NAME test_app COMMAND test_app)
target_link_libraries(
  test_app PRIVATE
  Qt::Test
  Qt6::Widgets Qt6::Gui Qt6::Core Qt6::Network Qt6::WebSockets
  reversi-contract-lib
)
