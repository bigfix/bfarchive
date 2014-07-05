#include "ArchiveCreator.h"
#include "BigFix/DateTime.h"
#include "BigFix/Filesystem.h"
#include <iostream>

using namespace BigFix;

ArchiveCreator::ArchiveCreator( Stream& output, bool verbose )
  : m_writer( output )
  , m_verbose( verbose )
{
}

void ArchiveCreator::Create( const char* path )
{
  RecursivelyAddDir( path );
  m_writer.End();
}

void ArchiveCreator::RecursivelyAddDir( const char* path )
{
  std::vector<DirectoryEntry> entries = ReadDir( path );

  for ( std::vector<DirectoryEntry>::const_iterator it = entries.begin(),
                                                    end = entries.end();
        it != end;
        it++ )
  {
    if ( it->IsDirectory() )
    {
      if ( m_verbose )
        std::cout << it->Path() << std::endl;

      m_writer.Directory( it->Path().c_str(), DateTime() );
      RecursivelyAddDir( it->Path().c_str() );
    }
    else if ( it->IsFile() )
    {
      if ( m_verbose )
        std::cout << it->Path() << std::endl;

      Stream& contentStream =
        m_writer.File( it->Path().c_str(), DateTime(), it->Length() );

      ReadFile( it->Path().c_str(), contentStream );
    }
  }
}
