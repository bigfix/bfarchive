#include "ArchiveWriter.h"
#include "BigFix/DataRef.h"
#include "BigFix/DateTime.h"
#include "BigFix/Error.h"
#include "BigFix/Number.h"
#include "BigFix/Stream.h"

namespace BigFix
{

ArchiveWriter::ArchiveWriter( Stream& output ) : m_output( output )
{
}

void ArchiveWriter::Directory( const char* name,
                               Encoding nameEncoding,
                               const DateTime& mtime )
{
  std::string nameWithSlash = name;
  nameWithSlash += "/";

  WriteHeader( nameWithSlash.c_str(), nameEncoding, mtime, 0 );
}

void ArchiveWriter::FileStart( const char* name,
                               Encoding nameEncoding,
                               const DateTime& mtime,
                               uint64_t length )
{
  WriteHeader( name, nameEncoding, mtime, length );
}

void ArchiveWriter::FileWrite( DataRef data )
{
  m_output.Write( data );
}

void ArchiveWriter::FileEnd()
{
}

void ArchiveWriter::End()
{
  m_output.Write( DataRef( "_\0" ) );
  m_output.End();
}

void ArchiveWriter::WriteHeader( const char* name,
                                 Encoding nameEncoding,
                                 const DateTime& mtime,
                                 uint64_t length )
{
  uint8_t buffer[8];

  if ( nameEncoding == ENCODING_UTF8 )
    m_output.Write( DataRef( "2" ) );

  if ( length >= UINT32_MAX )
    m_output.Write( DataRef( "1" ) );
  else
    m_output.Write( DataRef( "_" ) );

  size_t nameLengthWithNull = strlen( name ) + 1;

  if ( nameLengthWithNull >= 255 )
    throw Error( "File or directory name is longer than 254 characters" );

  WriteLittleEndian( nameLengthWithNull, buffer, 1 );

  m_output.Write( DataRef( buffer, buffer + 1 ) );
  m_output.Write(
    DataRef( reinterpret_cast<const uint8_t*>( name ),
             reinterpret_cast<const uint8_t*>( name ) + nameLengthWithNull ) );

  std::string mtimeString = mtime.ToString();

  WriteLittleEndian( mtimeString.size(), buffer, 1 );

  m_output.Write( DataRef( buffer, buffer + 1 ) );  
  m_output.Write( DataRef( mtimeString ) );

  if ( length >= UINT32_MAX )
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
