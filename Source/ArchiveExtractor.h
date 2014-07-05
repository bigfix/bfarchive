#ifndef ArchiveExtractor_h
#define ArchiveExtractor_h

#include "BigFix/ArchiveStream.h"
#include "BigFix/Filesystem.h"
#include <string>

class ArchiveExtractorFileStream : public BigFix::FileStream
{
public:
  void Reset( std::auto_ptr<BigFix::File>, const BigFix::DateTime& mtime );

  virtual void End();

private:
  BigFix::DateTime m_mtime;
};

class ArchiveExtractor : public BigFix::ArchiveStream
{
public:
  explicit ArchiveExtractor( const std::string& outputDir, bool verbose );

  virtual void Directory( const char* path,
                          const BigFix::DateTime& mtime );

  virtual BigFix::Stream& File( const char* path,
                                const BigFix::DateTime& mtime,
                                uint64_t length );

  virtual void End();

private:
  std::string m_outputDir;
  bool m_verbose;
  ArchiveExtractorFileStream m_fileStream;
};

#endif
