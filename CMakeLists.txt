cmake_minimum_required(VERSION 2.8)

set(BIGFIX_COMPILE_OPTIONS
  -fcolor-diagnostics
  -Wall
  -Werror
  -g
)

add_subdirectory(External/zlib)
add_subdirectory(External/gtest)
add_subdirectory(Library)
add_subdirectory(Test)

add_executable(bfarchive
  Source/Main.cpp
)

target_compile_options(bfarchive PRIVATE ${BIGFIX_COMPILE_OPTIONS})
target_link_libraries(bfarchive bigfix)
