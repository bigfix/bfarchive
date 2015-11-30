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
#include "BigFix/DeflateStream.h"
#include "BigFix/TestSeams.h"
#include "TestUtility.h"
#include "ScopedMock.h"
#include <gtest/gtest.h>

using namespace BigFix;

TEST( DeflateStreamTest, CompressEmpty )
{
  std::vector<uint8_t> output;
  VectorStream vectorStream( output );
  DeflateStream deflateStream( vectorStream );

  deflateStream.End();

  uint8_t expected[] =
  {
    0x23, 0x23, 0x53, 0x43, 0x30, 0x30, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x78, 0x9c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01
  };

  ASSERT_EQ( sizeof( expected ), output.size() );
  EXPECT_TRUE( std::equal( output.begin(), output.end(), expected ) );
  EXPECT_TRUE( vectorStream.ended );
}

TEST( DeflateStreamTest, CompressHelloWorld )
{
  std::vector<uint8_t> output;
  VectorStream vectorStream( output );
  DeflateStream deflateStream( vectorStream );

  WriteOneByOneAndEnd( deflateStream, DataRef( "Hello, world!" ) );

  uint8_t expected[] =
  {
    0x23, 0x23, 0x53, 0x43, 0x30, 0x30, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x78, 0x9c, 0xf3, 0x48, 0xcd, 0xc9, 0xc9, 0xd7, 0x51, 0x28, 0xcf, 0x2f,
    0xca, 0x49, 0x51, 0x04, 0x00, 0x20, 0x5e, 0x04, 0x8a
  };

  ASSERT_EQ( sizeof( expected ), output.size() );
  EXPECT_TRUE( std::equal( output.begin(), output.end(), expected ) );
  EXPECT_TRUE( vectorStream.ended );
}

static int deflateError( z_stream*, int )
{
  return Z_STREAM_ERROR;
}

TEST( DeflateStreamTest, InitFails )
{
  ScopedMock<Type_deflateInit> guard(
    deflateError, Real_deflateInit, Set_deflateInit );

  try
  {
    std::vector<uint8_t> output;
    VectorStream vectorStream( output );
    DeflateStream deflateStream( vectorStream );
    FAIL();
  }
  catch ( const std::exception& err )
  {
    std::string message = err.what();
    std::string expected = "Failed to initialize zlib";
    EXPECT_EQ( expected, message );
  }
  catch ( ... ) { FAIL(); }
}

TEST( DeflateStreamTest, WriteFails )
{
  std::vector<uint8_t> output;
  VectorStream vectorStream( output );
  DeflateStream deflateStream( vectorStream );

  ScopedMock<Type_deflate> guard( deflateError, Real_deflate, Set_deflate );

  try
  {
    deflateStream.Write( DataRef( "Hello, world!" ) );
    FAIL();
  }
  catch ( const std::exception& err )
  {
    std::string message = err.what();
    std::string expected = "Failed to compress data";
    EXPECT_EQ( expected, message );
  }
  catch ( ... ) { FAIL(); }
}

TEST( DeflateStreamTest, EndFails )
{
  std::vector<uint8_t> output;
  VectorStream vectorStream( output );
  DeflateStream deflateStream( vectorStream );

  ScopedMock<Type_deflate> guard( deflateError, Real_deflate, Set_deflate );

  try
  {
    deflateStream.End();
    FAIL();
  }
  catch ( const std::exception& err )
  {
    std::string message = err.what();
    std::string expected = "Failed to compress data";
    EXPECT_EQ( expected, message );
  }
  catch ( ... ) { FAIL(); }
}
