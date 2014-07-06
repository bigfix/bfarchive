add_executable(run_unit EXCLUDE_FROM_ALL
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

target_compile_options(run_unit PRIVATE ${BIGFIX_COMPILE_OPTIONS})
target_link_libraries(run_unit bigfix gtest)

add_custom_target(unit
  DEPENDS run_unit

  # Reset the filesystem sandbox
  COMMAND ${CMAKE_COMMAND} -E remove_directory FilesystemTestSandbox
  COMMAND ${CMAKE_COMMAND} -E make_directory FilesystemTestSandbox

  # Run tests
  COMMAND run_unit --gtest_color=yes

  # Clean the filesystem sandbox
  COMMAND ${CMAKE_COMMAND} -E remove_directory FilesystemTestSandbox
)

add_custom_target(integration
  DEPENDS bfarchive unit
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/BFArchiveTest.py
)

add_custom_target(test DEPENDS unit integration)
