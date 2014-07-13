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
