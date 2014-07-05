#ifndef TestUtility_h
#define TestUtility_h

#include "BigFix/Stream.h"
#include <stdint.h>
#include <string>
#include <vector>

class VectorStream : public BigFix::Stream
{
public:
  explicit VectorStream( std::vector<uint8_t>& );

  virtual void Write( BigFix::DataRef data );
  virtual void End();

  std::vector<uint8_t>& output;
  bool ended;
};

class StringStream : public BigFix::Stream
{
public:
  StringStream();
  explicit StringStream( std::string& );

  virtual void Write( BigFix::DataRef data );
  virtual void End();

  std::string& output;
  bool ended;
};

void WriteOneByOneAndEnd( BigFix::Stream& stream, BigFix::DataRef data );

#endif
