add_executable(${REVERSI_CLIENT} main.cc)

target_link_libraries(
  ${REVERSI_CLIENT}
  PRIVATE ${REVERSI_CLIENT_LIB}
)
target_include_directories(
  ${REVERSI_CLIENT}
  PUBLIC ${REVERSI_CLIENT_INCLUDE}
)
