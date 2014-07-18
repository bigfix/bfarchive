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

#include "BigFix/DataRef.h"
#include "BigFix/UTF8.h"
#include <gtest/gtest.h>

using namespace BigFix;

#define GOOD( bytes ) \
  EXPECT_TRUE( IsValidUTF8( reinterpret_cast<const char*>( bytes ) ) );

#define BAD( bytes ) \
  EXPECT_FALSE( IsValidUTF8( reinterpret_cast<const char*>( bytes ) ) );

TEST( UTF8Test, OneByteTests )
{
  {
    uint8_t bytes[2] = { 0x7F, 0x00 };
    GOOD( bytes );
  }

  {
    uint8_t bytes[2] = { 0x80, 0x00 };
    BAD( bytes );
  }
}

TEST( UTF8Test, TwoByteTests )
{
  {
    uint8_t bytes[3] = { 0xC2, 0xBF, 0x00 };
    GOOD( bytes );
  }

  {
    uint8_t bytes[3] = { 0xDF, 0x00, 0x00 };
    BAD( bytes );
  }
}

TEST( UTF8Test, ThreeByteTests )
{
  {
    uint8_t bytes[4] = { 0xE0, 0xA0, 0x80, 0x00 };
    GOOD( bytes );
  }

  {
    uint8_t bytes[4] = { 0xE0, 0x80, 0x80, 0x00 };
    BAD( bytes );
  }

  {
    uint8_t bytes[4] = { 0xE1, 0xBF, 0x90, 0x00 };
    GOOD( bytes );
  }

  {
    uint8_t bytes[4] = { 0xED, 0x9F, 0x90, 0x00 };
    GOOD( bytes );
  }

  {
    uint8_t bytes[4] = { 0xED, 0xBF, 0x90, 0x00 };
    BAD( bytes );
  }

  {
    uint8_t bytes[4] = { 0xEE, 0xBF, 0x90, 0x00 };
    GOOD( bytes );
  }
}

TEST( UTF8Test, FourByteTests )
{
  {
    uint8_t bytes[5] = { 0xF0, 0x89, 0x80, 0x80, 0x00 };
    BAD( bytes );
  }

  {
    uint8_t bytes[5] = { 0xF0, 0x90, 0x80, 0x80, 0x00 };
    GOOD( bytes );
  }

  {
    uint8_t bytes[5] = { 0xF2, 0x90, 0x80, 0x80, 0x00 };
    GOOD( bytes );
  }

  {
    uint8_t bytes[5] = { 0xF4, 0x90, 0x80, 0x80, 0x00 };
    BAD( bytes );
  }

  {
    uint8_t bytes[5] = { 0xF4, 0x89, 0x80, 0x80, 0x00 };
    GOOD( bytes );
  }
}
