cmake_minimum_required(VERSION 2.8)

project(BFArchive C CXX)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(BIGFIX_COMPILE_OPTIONS -Wall -Werror -O3 -fcolor-diagnostics)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  set(BIGFIX_COMPILE_OPTIONS -Wall -Werror -O3)
else()
  set(BIGFIX_COMPILE_OPTIONS /W3 /WX /DUNICODE /D_UNICODE)
  set(BFARCHIVE_RESOURCE_FILE Source/Version.rc)
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
  ${BFARCHIVE_RESOURCE_FILE}
)

target_compile_options(bfarchive PRIVATE ${BIGFIX_COMPILE_OPTIONS})
target_link_libraries(bfarchive bigfix)

if(WIN32)
  set_target_properties(bfarchive PROPERTIES OUTPUT_NAME BFArchive)
endif()
