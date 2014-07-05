#ifndef ArchiveCreator_h
#define ArchiveCreator_h

#include "BigFix/ArchiveWriter.h"
#include <string>

class ArchiveCreator
{
public:
  ArchiveCreator( BigFix::Stream& output, bool verbose );

  void Create( const std::string& path );

private:
  void RecursivelyAddDir( const std::string& filePath,
                          const std::string& archivePath );

  BigFix::ArchiveWriter m_writer;
  bool m_verbose;
};

#endif
