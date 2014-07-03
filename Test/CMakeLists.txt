add_executable(runtests EXCLUDE_FROM_ALL
  ArchiveReaderTest.cpp
  ArchiveWriterTest.cpp
  DateTimeTest.cpp
  DataRefTest.cpp
  DeflateStreamTest.cpp
  ErrorTest.cpp
  InflateStreamTest.cpp
  NumberTest.cpp
  TestUtility.cpp
)

target_compile_options(runtests PRIVATE ${BIGFIX_COMPILE_OPTIONS})
target_link_libraries(runtests bigfix gtest)

add_custom_target(test
  DEPENDS runtests
  COMMAND runtests --gtest_color=yes
  COMMAND ${CMAKE_COMMAND} -E echo 'ALL TESTS PASSED'
)
