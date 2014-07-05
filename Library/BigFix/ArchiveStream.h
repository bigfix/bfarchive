#ifndef BigFix_ArchiveStream_h
#define BigFix_ArchiveStream_h

#include <stdint.h>

namespace BigFix
{

class DateTime;
class DataRef;
class Stream;

class ArchiveStream
{
public:
  virtual ~ArchiveStream();

  virtual void Directory( const char* path, const DateTime& mtime ) = 0;

  virtual Stream& File( const char* path,
                        const DateTime& mtime,
                        uint64_t length ) = 0;

  virtual void End() = 0;
};

}

#endif
