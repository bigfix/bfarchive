#ifndef BigFix_ArchiveWriter_h
#define BigFix_ArchiveWriter_h

#include "BigFix/ArchiveStream.h"
#include "BigFix/Stream.h"

namespace BigFix
{

class ArchiveWriter : public ArchiveStream
{
public:
  explicit ArchiveWriter( Stream& output );

  virtual void Directory( const char* name,
                          Encoding nameEncoding,
                          const DateTime& mtime );

  virtual void FileStart( const char* name,
                          Encoding nameEncoding,
                          const DateTime& mtime,
                          uint64_t length );

  virtual void FileWrite( DataRef );

  virtual void FileEnd();

  virtual void End();

private:
  void WriteHeader( const char* name,
                    Encoding nameEncoding,
                    const DateTime& mtime,
                    uint64_t length );

  Stream& m_output;
};

}

#endif
