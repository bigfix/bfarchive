add_library(gtest STATIC EXCLUDE_FROM_ALL
  fused-src/gtest/gtest-all.cc
  fused-src/gtest/gtest_main.cc)

target_include_directories(gtest PUBLIC fused-src)
target_compile_definitions(gtest PRIVATE -DGTEST_HAS_PTHREAD=0)

if (WIN32)
  target_compile_definitions(gtest PUBLIC -DGTEST_OS_WINDOWS=1)
endif()

# Visual Studio 2012 only supports up to 8 template parameters in
# std::tr1::tuple by default, but gtest requires 10
if (MSVC AND MSVC_VERSION EQUAL 1700)
  target_compile_definitions(gtest PUBLIC -D_VARIADIC_MAX=10)
endif()
