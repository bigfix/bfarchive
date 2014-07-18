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
#include "BigFix/Stream.h"
#include "TestUtility.h"
#include <gtest/gtest.h>

using namespace BigFix;

TEST( StreamTest, AppendStreamTest )
{
  std::string output;
  StringStream stringStream( output );
  AppendStream append;

  append.Reset( stringStream );

  append.Write( DataRef( "hello" ) );
  append.End();

  EXPECT_EQ( "hello", output );
  EXPECT_FALSE( stringStream.ended );
}

TEST( StreamTest, WriteAndEndTest )
{
  std::string output;
  StringStream stringStream( output );
  WriteAndEnd( stringStream, DataRef( "hello" ) );

  EXPECT_EQ( "hello", output );
  EXPECT_TRUE( stringStream.ended );
}
