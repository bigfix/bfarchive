#ifndef BigFix_ArchiveReader_h
#define BigFix_ArchiveReader_h

#include "BigFix/ArchiveStream.h"
#include "BigFix/DateTime.h"
#include "BigFix/Stream.h"
#include <stdint.h>
#include <time.h>

namespace BigFix
{

class ArchiveReader : public Stream
{
public:
  explicit ArchiveReader( ArchiveStream& output );

  virtual void Write( DataRef );
  virtual void End();

private:
  const uint8_t* NameEncoding( const uint8_t* start, const uint8_t* end );
  const uint8_t* FileLengthLength( const uint8_t* start, const uint8_t* end );
  const uint8_t* NameLength( const uint8_t* start, const uint8_t* end );
  const uint8_t* Name( const uint8_t* start, const uint8_t* end );
  const uint8_t* DateLength( const uint8_t* start, const uint8_t* end );
  const uint8_t* Date( const uint8_t* start, const uint8_t* end );
  const uint8_t* FileLength( const uint8_t* start, const uint8_t* end );
  const uint8_t* File( const uint8_t* start, const uint8_t* end );

  enum State
  {
    STATE_NAME_ENCODING,
    STATE_FILE_LENGTH_LENGTH,
    STATE_NAME_LENGTH,
    STATE_NAME,
    STATE_DATE_LENGTH,
    STATE_DATE,
    STATE_FILE_LENGTH,
    STATE_FILE,
    STATE_DONE
  };

  ArchiveStream& m_output;
  State m_state;
  Encoding m_nameEncoding;
  size_t m_fileLengthLength;
  bool m_isDirectory;
  DateTime m_mtime;
  size_t m_pos;
  size_t m_length;
  uint64_t m_filePos;
  uint64_t m_fileLength;
  uint8_t m_name[256];
  uint8_t m_buffer[256];
};

}

#endif
