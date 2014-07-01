#ifndef BigFix_ArchiveStream_h
#define BigFix_ArchiveStream_h

#include <stdint.h>

namespace BigFix
{

class DateTime;

enum ArchiveEncoding
{
  ARCHIVE_ENCODING_LOCAL,
  ARCHIVE_ENCODING_UTF8
};

class ArchiveStream
{
public:
  virtual ~ArchiveStream();

  virtual void Directory( const char* name,
                          ArchiveEncoding nameEncoding,
                          const DateTime& mtime ) = 0;

  virtual void FileStart( const char* name,
                          ArchiveEncoding nameEncoding,
                          const DateTime& mtime,
                          uint64_t length ) = 0;

  virtual void FileWrite( const uint8_t* start, const uint8_t* end ) = 0;

  virtual void FileEnd() = 0;
};

}

#endif
