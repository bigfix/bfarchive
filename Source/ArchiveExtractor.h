#ifndef ArchiveExtractor_h
#define ArchiveExtractor_h

#include "BigFix/ArchiveStream.h"
#include "BigFix/Filesystem.h"

class ArchiveExtractor : public BigFix::ArchiveStream
{
public:
  virtual void Directory( const char* name,
                          BigFix::Encoding nameEncoding,
                          const BigFix::DateTime& mtime );

  virtual void FileStart( const char* name,
                          BigFix::Encoding nameEncoding,
                          const BigFix::DateTime& mtime,
                          uint64_t length );

  virtual void FileWrite( BigFix::DataRef );

  virtual void FileEnd();

  virtual void End();

private:
  std::auto_ptr<BigFix::File> m_currentFile;
};

#endif
