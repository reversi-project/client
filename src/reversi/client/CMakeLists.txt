add_library(
  ${REVERSI_CLIENT_LIB}
  app.cc
)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network LinguistTools)
include_directories(${Qt6_INCLUDE_DIRS})
qt6_add_translations(${REVERSI_CLIENT_LIB} TS_FILES resources/ru.ts)

include(FetchContent)
FetchContent_Declare(
  reversi-contract
  GIT_REPOSITORY https://github.com/reversi-project/contract.git
  GIT_TAG master
)
FetchContent_MakeAvailable(reversi-contract)

target_link_libraries(
  ${REVERSI_CLIENT_LIB}
  Qt6::Widgets Qt6::Gui Qt6::Core Qt6::Network
  reversi-contract-lib
)
target_include_directories(
  ${REVERSI_CLIENT_LIB} PRIVATE
  ${REVERSI_CLIENT_INCLUDE}
  ${reversi-contract_SOURCE_DIR}/include
)
