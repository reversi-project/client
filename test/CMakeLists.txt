find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network WebSockets Test)
include_directories(${Qt6_INCLUDE_DIRS})

qt_add_executable(
  test_app
  test_main.cc
)
add_test(NAME test_app COMMAND test_app)
target_link_libraries(
  test_app PRIVATE
  Qt::Test
  ${REVERSI_CLIENT_LIB}
)
