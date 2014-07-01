add_library(gtest STATIC EXCLUDE_FROM_ALL
  fused-src/gtest/gtest-all.cc
  fused-src/gtest/gtest_main.cc)

target_include_directories(gtest PUBLIC fused-src)
