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
                          const DateTime& mtime );

  virtual Stream& File( const char* name,
                        const DateTime& mtime,
                        uint64_t length );

  virtual void End();

private:
  void WriteHeader( const char* name,
                    const DateTime& mtime,
                    uint64_t length );

  Stream& m_output;
  AppendStream m_append;
};

}

#endif
