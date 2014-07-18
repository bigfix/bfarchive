/*
  Copyright 2014 International Business Machines, Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#include "BigFix/Filesystem.h"
#include <gtest/gtest.h>

using namespace BigFix;

TEST( LocalPathToUTF8PathTest, AsciiPath )
{
  EXPECT_EQ( "hodor", LocalPathToUTF8Path( "hodor" ) );
}

// On Windows, we transcode from the local code page to UTF-8 if the file path
// is not marked as UTF-8. On all other platforms, we just use the file path as
// is because there's not an awesome solution for file name encoding.

// To have this test work the same in all locales, we always use code page 932,
// which is shift-jis. The actual implementation uses CP_ACP.

static const uint8_t konnichiwa_shiftjis[] =
{
  0x82, 0xB1, 0x82, 0xF1, 0x82, 0xC9, 0x82, 0xBF, 0x82, 0xCD, 0x00
};

#ifdef _WIN32

static const uint8_t konnichiwa_utf8[] =
{
  0xe3, 0x81, 0x93, 0xe3, 0x82, 0x93, 0xe3, 0x81, 0xab, 0xe3, 0x81, 0xa1,
  0xe3, 0x81, 0xaf, 0x00
};

TEST( LocalPathToUTF8PathTest, LocalJapanesePath )
{
  std::string expected = reinterpret_cast<const char*>( konnichiwa_utf8 );
  std::string actual = LocalPathToUTF8Path(
    reinterpret_cast<const char*>( konnichiwa_shiftjis ), 932 );

  EXPECT_EQ( expected, actual );
}

#else

TEST( LocalPathToUTF8PathTest, LocalJapanesePath )
{
  std::string expected = reinterpret_cast<const char*>( konnichiwa_shiftjis );
  std::string actual = LocalPathToUTF8Path(
    reinterpret_cast<const char*>( konnichiwa_shiftjis ), 932 );

  EXPECT_EQ( expected, actual );
}

#endif
