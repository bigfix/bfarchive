#ifndef TestUtility_h
#define TestUtility_h

#include "BigFix/Stream.h"
#include <string>
#include <vector>

class VectorStream : public BigFix::Stream
{
public:
  VectorStream();

  virtual void Write( BigFix::DataRef data );
  virtual void End();

  std::vector<uint8_t> output;
  bool ended;
};

class StringStream : public BigFix::Stream
{
public:
  StringStream();

  virtual void Write( BigFix::DataRef data );
  virtual void End();

  std::string output;
  bool ended;
};

void WriteOneByOne( BigFix::Stream& stream, BigFix::DataRef data );

#endif
