#ifndef BigFix_InflateStream_h
#define BigFix_InflateStream_h

#include "BigFix/Stream.h"
#include <stdint.h>
#include <zlib.h>

namespace BigFix
{

class InflateStream : public Stream
{
public:
  explicit InflateStream( Stream& output );
  virtual ~InflateStream();

  virtual void Write( DataRef );
  virtual void End();

private:
  const uint8_t* Header( const uint8_t* start, const uint8_t* end );
  const uint8_t* Checksum( const uint8_t* start, const uint8_t* end );
 
  void Pump( int zflags );

  enum State { STATE_HEADER, STATE_CHECKSUM, STATE_COMPRESSED, STATE_RAW };

  Stream& m_output;
  State m_state;
  z_stream m_zstream;
  int m_zstatus;
  size_t m_pos;
  uint8_t m_header[8];
};

}

#endif
