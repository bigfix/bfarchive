#ifndef BigFix_Stream_h
#define BigFix_Stream_h

namespace BigFix
{

class DataRef;

class Stream
{
public:
  virtual ~Stream();

  virtual void Start() = 0;
  virtual void Write( DataRef ) = 0;
  virtual void End() = 0;
};

}

#endif
