#ifndef ArchiveLister_h
#define ArchiveLister_h

#include "BigFix/ArchiveStream.h"
#include "BigFix/Stream.h"

class ArchiveLister : public BigFix::ArchiveStream
{
public:
  virtual void Directory( const char* path, const BigFix::DateTime& mtime );

  virtual BigFix::Stream& File( const char* path,
                                const BigFix::DateTime& mtime,
                                uint64_t length );

  virtual void End();

private:
  BigFix::NullStream m_ignore;
};

#endif
