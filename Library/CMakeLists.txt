add_library(bigfix
  BigFix/ArchiveReader.cpp
  BigFix/ArchiveStream.cpp
  BigFix/DateTime.cpp
  BigFix/Error.cpp
  BigFix/InflateStream.cpp
  BigFix/Number.cpp
  BigFix/Stream.cpp
)

target_compile_options(bigfix PRIVATE ${BIGFIX_COMPILE_OPTIONS})
target_include_directories(bigfix PUBLIC .)
target_link_libraries(bigfix zlibstatic)
