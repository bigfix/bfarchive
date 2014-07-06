add_executable(runtests EXCLUDE_FROM_ALL
  ArchiveReaderTest.cpp
  ArchiveWriterTest.cpp
  ArgParserTest.cpp
  DateTimeTest.cpp
  DataRefTest.cpp
  DeflateStreamTest.cpp
  ErrorTest.cpp
  FilesystemTest.cpp
  LocalPathToUTF8PathTest.cpp
  InflateStreamTest.cpp
  NumberTest.cpp
  TestUtility.cpp
)

target_compile_options(runtests PRIVATE ${BIGFIX_COMPILE_OPTIONS})
target_link_libraries(runtests bigfix gtest)

add_custom_target(test
  DEPENDS runtests

  # Reset the filesystem sandbox
  COMMAND ${CMAKE_COMMAND} -E remove_directory FilesystemTestSandbox
  COMMAND ${CMAKE_COMMAND} -E make_directory FilesystemTestSandbox

  # Run tests
  COMMAND runtests --gtest_color=yes

  # Clean the filesystem sandbox
  COMMAND ${CMAKE_COMMAND} -E remove_directory FilesystemTestSandbox
)
