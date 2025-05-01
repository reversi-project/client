add_library(
  ${REVERSI_CLIENT_LIB}

  app.cc
  window.cc
  context.cc
  menu.cc
  wait.cc
  play.cc
  worker.cc

  ../../../include/reversi/client/app.h
  ../../../include/reversi/client/window.h
  ../../../include/reversi/client/context.h
  ../../../include/reversi/client/menu.h
  ../../../include/reversi/client/wait.h
  ../../../include/reversi/client/play.h
  ../../../include/reversi/client/worker.h
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
  ${REVERSI_CLIENT_LIB}
  Qt6::Widgets Qt6::Gui Qt6::Core Qt6::Network Qt6::WebSockets
  reversi-contract-lib
)
target_include_directories(
  ${REVERSI_CLIENT_LIB} PUBLIC
  ${REVERSI_CLIENT_INCLUDE}
  ${reversi-contract_SOURCE_DIR}/include
)

