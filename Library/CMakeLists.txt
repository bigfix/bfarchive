list(APPEND SOURCES
  BigFix/ArchiveReader.cpp
  BigFix/ArchiveStream.cpp
  BigFix/ArchiveWriter.cpp
  BigFix/ArgParser.cpp
  BigFix/DataRef.cpp
  BigFix/DateTime.cpp
  BigFix/DeflateStream.cpp
  BigFix/Error.cpp
  BigFix/Filesystem.cpp
  BigFix/InflateStream.cpp
  BigFix/Number.cpp
  BigFix/Stream.cpp
  BigFix/UTF8.cpp
)

if(WIN32)
  list(APPEND SOURCES BigFix/WindowsFilesystem.cpp)
else()
  list(APPEND SOURCES BigFix/UnixFilesystem.cpp)
endif()

add_library(bigfix ${SOURCES})

target_compile_options(bigfix PRIVATE ${BIGFIX_COMPILE})
target_include_directories(bigfix PUBLIC .)
target_link_libraries(bigfix zlibstatic)

list(APPEND TESTING_SOURCES
  ${SOURCES}
  BigFix/TestSeams.cpp
)

if (WIN32)
  list(APPEND TESTING_SOURCES BigFix/WindowsTestSeams.cpp)
else()
  list(APPEND TESTING_SOURCES BigFix/UnixTestSeams.cpp)
endif()

add_library(bigfix_testing EXCLUDE_FROM_ALL ${TESTING_SOURCES})

target_compile_options(bigfix_testing PRIVATE ${BIGFIX_COMPILE_TEST})
target_include_directories(bigfix_testing PUBLIC .)
target_link_libraries(bigfix_testing zlibstatic)
target_compile_definitions(bigfix_testing PRIVATE ENABLE_TEST_SEAMS=1)
