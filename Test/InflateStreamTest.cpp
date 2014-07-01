#include "BigFix/DataRef.h"
#include "BigFix/InflateStream.h"
#include <gtest/gtest.h>
#include <string>

using namespace BigFix;

class StringStream : public Stream
{
public:
  explicit StringStream( std::string& output ) : m_output( output ) {}

  virtual void Start() {}

  virtual void Write( DataRef data )
  {
    m_output.insert( m_output.end(), data.Start(), data.End() );
  }

  virtual void End() {}

private:
  std::string& m_output;
};

static void WriteOneByOne( Stream& stream, DataRef data )
{
  for ( const uint8_t* it = data.Start(); it != data.End(); it++ )
    stream.Write( DataRef( it, it + 1 ) );
}

TEST( InflateStreamTest, ShortRaw )
{
  std::string output;
  StringStream stringStream( output );
  InflateStream inflateStream( stringStream );

  DataRef data( "hello" );

  WriteOneByOne( inflateStream, data );
  inflateStream.End();

  EXPECT_EQ( "hello", output );
}

TEST( InflateStreamTest, LongRaw )
{
  std::string output;
  StringStream stringStream( output );
  InflateStream inflateStream( stringStream );

  DataRef data( "hello, world! blah blah blah" );

  WriteOneByOne( inflateStream, data );
  inflateStream.End();

  EXPECT_EQ( "hello, world! blah blah blah", output );
}

TEST( InflateStreamTest, Compressed )
{
  std::string output;
  StringStream stringStream( output );
  InflateStream inflateStream( stringStream );

  uint8_t data[] =
  {
    0x23, 0x23, 0x53, 0x43, 0x30, 0x30, 0x31, 0x00, 0x8e, 0x9a, 0x77, 0x06,
    0x78, 0x9c, 0xf3, 0x48, 0xcd, 0xc9, 0xc9, 0xd7, 0x51, 0x28, 0xcf, 0x2f,
    0xca, 0x49, 0x51, 0x04, 0x00, 0x20, 0x5e, 0x04, 0x8a
  };

  WriteOneByOne( inflateStream, DataRef( data, data + sizeof( data ) ) );
  inflateStream.End();

  EXPECT_EQ( "Hello, world!", output );
}
