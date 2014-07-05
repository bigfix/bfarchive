#include "BigFix/Stream.h"
#include "TestUtilitiy.h"
#include <gtest/gtest.h>

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
