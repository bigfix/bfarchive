#include "ArchiveLister.h"
#include <iostream>

void ArchiveLister::Directory( const char* path, const BigFix::DateTime& mtime )
{
  std::cout << path << std::endl;
}

BigFix::Stream& ArchiveLister::File( const char* path,
                                     const BigFix::DateTime& mtime,
                                     uint64_t length )
{
  std::cout << path << std::endl;
  return m_ignore;
}

void ArchiveLister::End()
{
}
