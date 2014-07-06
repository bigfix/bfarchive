#include "ArchiveWriter.h"
#include "BigFix/DataRef.h"
#include "BigFix/DateTime.h"
#include "BigFix/Error.h"
#include "BigFix/Filesystem.h"
#include "BigFix/Number.h"
#include "BigFix/Stream.h"
#include <limits>
#include <string.h>

namespace BigFix
{

ArchiveWriter::ArchiveWriter( Stream& output ) : m_output( output )
{
}

void ArchiveWriter::Directory( const char* path, const DateTime& mtime )
{
  std::string pathWithSlash = path;
  pathWithSlash += "/";

  WriteHeader( pathWithSlash.c_str(), mtime, 0 );
}

Stream& ArchiveWriter::File( const char* path,
                             const DateTime& mtime,
                             uint64_t length )
{
  WriteHeader( path, mtime, length );

  m_append.Reset( m_output );
  return m_append;
}

void ArchiveWriter::End()
{
  m_output.Write( DataRef( "_\0" ) );
  m_output.End();
}

void ArchiveWriter::WriteHeader( const char* path,
                                 const DateTime& mtime,
                                 uint64_t length )
{
  uint8_t buffer[8];

  if ( !IsAscii( path ) )
     m_output.Write( DataRef( "2" ) );

  if ( length > std::numeric_limits<uint32_t>::max() )
    m_output.Write( DataRef( "1" ) );
  else
    m_output.Write( DataRef( "_" ) );

  size_t pathLengthWithNull = strlen( path ) + 1;

  if ( pathLengthWithNull > 255 )
    throw Error( "File or directory paths must be less than 255 characters" );

  WriteLittleEndian( pathLengthWithNull, buffer, 1 );

  m_output.Write( DataRef( buffer, buffer + 1 ) );
  m_output.Write(
    DataRef( reinterpret_cast<const uint8_t*>( path ),
             reinterpret_cast<const uint8_t*>( path ) + pathLengthWithNull ) );

  std::string mtimeString = mtime.ToString();

  WriteLittleEndian( mtimeString.size(), buffer, 1 );

  m_output.Write( DataRef( buffer, buffer + 1 ) );  
  m_output.Write( DataRef( mtimeString ) );

  if ( length > std::numeric_limits<uint32_t>::max() )
  {
    WriteLittleEndian( length, buffer, 8 );
    m_output.Write( DataRef( buffer, buffer + 8 ) );
  }
  else
  {
    WriteLittleEndian( length, buffer, 4 );
    m_output.Write( DataRef( buffer, buffer + 4 ) );
  }
}

}
