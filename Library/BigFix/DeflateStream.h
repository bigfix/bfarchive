#ifndef BigFix_DeflateStream_h
#define BigFix_DeflateStream_h

#include "BigFix/Stream.h"
#include <zlib.h>

namespace BigFix
{

class DeflateStream : public Stream
{
public:
  explicit DeflateStream( Stream& output );
  virtual ~DeflateStream();

  virtual void Write( DataRef );
  virtual void End();

private:
  void Pump( int zflags );

  Stream& m_output;
  bool m_sentHeader;
  z_stream m_zstream;
  int m_zstatus;
};

}

#endif
