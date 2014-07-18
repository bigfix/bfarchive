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

#include "BigFix/Error.h"
#include "BigFix/DataRef.h"
#include "BigFix/Number.h"
#include <gtest/gtest.h>

using namespace BigFix;

TEST( NumberTest, Read4ByteIntger )
{
  uint8_t number[4] = { 0x01, 0x02, 0x03, 0x04 };

  EXPECT_EQ( 67305985ull, ReadLittleEndian( DataRef( number, number + 4 ) ) );
}

TEST( NumberTest, Read8ByteIntger )
{
  uint8_t number[8] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };

  EXPECT_EQ( 578437695752307201ull,
             ReadLittleEndian( DataRef( number, number + 8 ) ) );
}

TEST( NumberTest, Write4ByteInteger )
{
  uint8_t number[4];
  WriteLittleEndian( 67305985, number, 4 );

  EXPECT_EQ( 1, number[0] );
  EXPECT_EQ( 2, number[1] );
  EXPECT_EQ( 3, number[2] );
  EXPECT_EQ( 4, number[3] );
}

TEST( NumberTest, Write8ByteInteger )
{
  uint8_t number[8];
  WriteLittleEndian( 578437695752307201, number, 8 );

  EXPECT_EQ( 1, number[0] );
  EXPECT_EQ( 2, number[1] );
  EXPECT_EQ( 3, number[2] );
  EXPECT_EQ( 4, number[3] );
  EXPECT_EQ( 5, number[4] );
  EXPECT_EQ( 6, number[5] );
  EXPECT_EQ( 7, number[6] );
  EXPECT_EQ( 8, number[7] );
}

TEST( NumberTest, ReadAsciiNumber )
{
  EXPECT_EQ( 42ul, ReadAsciiNumber<uint8_t>( DataRef( "42" ) ) );
  EXPECT_EQ( 1970, ReadAsciiNumber<int32_t>( DataRef( "1970" ) ) );
  EXPECT_THROW( ReadAsciiNumber<uint8_t>( DataRef( "hello" ) ), Error );
}
