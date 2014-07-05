#ifndef ArchiveCreator_h
#define ArchiveCreator_h

#include "BigFix/ArchiveWriter.h"

class ArchiveCreator
{
public:
  ArchiveCreator( BigFix::Stream& output, bool verbose );

  void Create( const char* path );

private:
  void RecursivelyAddDir( const char* path );

  BigFix::ArchiveWriter m_writer;
  bool m_verbose;
};

#endif
