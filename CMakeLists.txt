cmake_minimum_required(VERSION 2.8)

project(bfarchive C CXX)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(BIGFIX_COMPILE_OPTIONS -Wall -Werror -O3 -fcolor-diagnostics)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  set(BIGFIX_COMPILE_OPTIONS -Wall -Werror -O3)
else()
  set(BIGFIX_COMPILE_OPTIONS /W3 /WX /DUNICODE)
endif()

add_subdirectory(External/zlib)
add_subdirectory(External/gtest)
add_subdirectory(Library)
add_subdirectory(Test)

add_executable(bfarchive
  Source/ArchiveCreator.cpp
  Source/ArchiveExtractor.cpp
  Source/ArchiveLister.cpp
  Source/Main.cpp
)

target_compile_options(bfarchive PRIVATE ${BIGFIX_COMPILE_OPTIONS})
target_link_libraries(bfarchive bigfix)
