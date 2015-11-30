cmake_minimum_required(VERSION 2.8)

project(BFArchive C CXX)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(BIGFIX_COMPILE_OPTIONS -Wall -Werror -fcolor-diagnostics)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  set(BIGFIX_COMPILE_OPTIONS -Wall -Werror)
else()
  set(BIGFIX_COMPILE_OPTIONS /W3 /WX /DUNICODE /D_UNICODE)
endif()

if(WIN32)
  SET(BIGFIX_COMPILE ${BIGFIX_COMPILE_OPTIONS})
  SET(BIGFIX_COMPILE_TEST ${BIGFIX_COMPILE_OPTIONS} /Od)
else()
  SET(BIGFIX_COMPILE ${BIGFIX_COMPILE_OPTIONS} -O3)
  SET(BIGFIX_COMPILE_TEST ${BIGFIX_COMPILE_OPTIONS} -O0)

  if (${COVERAGE})
    LIST(APPEND BIGFIX_COMPILE_TEST -coverage)
    LIST(APPEND BIGFIX_LINK_TEST -coverage)
  endif()
endif()

# Statically link the CRT on Windows. There's got to be a better way to do this.
if(MSVC)
  foreach(flag
    CMAKE_C_FLAGS CMAKE_CXX_FLAGS
    CMAKE_C_FLAGS_DEBUG CMAKE_CXX_FLAGS_DEBUG
    CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELEASE
    CMAKE_C_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_MINSIZEREL 
    CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    string(REPLACE "/MD" "/MT" ${flag} "${${flag}}")
  endforeach()
endif(MSVC)

add_subdirectory(External/zlib)
add_subdirectory(External/gtest)
add_subdirectory(Library)
add_subdirectory(Test)

if(WIN32)
  set(BFARCHIVE_SOURCES Source/Version.rc Source/WindowsMain.cpp)
else()
  set(BFARCHIVE_SOURCES Source/UnixMain.cpp)
endif()

list(APPEND BFARCHIVE_SOURCES
  Source/ArchiveCreator.cpp
  Source/ArchiveExtractor.cpp
  Source/ArchiveLister.cpp
  Source/Main.cpp
)

add_executable(bfarchive ${BFARCHIVE_SOURCES})
target_compile_options(bfarchive PRIVATE ${BIGFIX_COMPILE})
target_link_libraries(bfarchive bigfix)

if(WIN32)
  set_target_properties(bfarchive PROPERTIES OUTPUT_NAME BFArchive)
endif()

add_executable(bfarchive_testing ${BFARCHIVE_SOURCES})
target_compile_options(bfarchive_testing PRIVATE ${BIGFIX_COMPILE_TEST})
target_link_libraries(bfarchive_testing bigfix_testing ${BIGFIX_LINK_TEST})
