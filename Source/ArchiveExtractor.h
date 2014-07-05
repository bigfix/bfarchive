#ifndef ArchiveExtractor_h
#define ArchiveExtractor_h

#include "BigFix/ArchiveStream.h"
#include "BigFix/Filesystem.h"
#include <string>

class ArchiveExtractor : public BigFix::ArchiveStream
{
public:
  explicit ArchiveExtractor( const char* outputDir, bool verbose );

  virtual void Directory( const char* path,
                          const BigFix::DateTime& mtime );

  virtual BigFix::Stream& File( const char* path,
                                const BigFix::DateTime& mtime,
                                uint64_t length );

  virtual void End();

private:
  std::string m_outputDir;
  bool m_verbose;
  BigFix::FileStream m_fileStream;
};

#endif
