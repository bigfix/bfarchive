#ifndef ArchiveExtractor_h
#define ArchiveExtractor_h

#include "BigFix/ArchiveStream.h"
#include "BigFix/Filesystem.h"

class ArchiveExtractor : public BigFix::ArchiveStream
{
public:
  virtual void Directory( const char* name,
                          const BigFix::DateTime& mtime );

  virtual BigFix::Stream& File( const char* name,
                                const BigFix::DateTime& mtime,
                                uint64_t length );

  virtual void End();

private:
  BigFix::FileStream m_fileStream;
};

#endif
