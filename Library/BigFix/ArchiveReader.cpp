#include "ArchiveReader.h"
#include "BigFix/DataRef.h"
#include "BigFix/DateTime.h"
#include "BigFix/Error.h"
#include "BigFix/Number.h"
#include <algorithm>

namespace BigFix
{

ArchiveReader::ArchiveReader( ArchiveStream& output )
  : m_output( output ), m_state( STATE_NAME_ENCODING )
{
}

void ArchiveReader::Write( DataRef data )
{
  const uint8_t* start = data.Start();
  const uint8_t* end = data.End();

  while ( start != end )
  {
    switch ( m_state )
    {
    case STATE_NAME_ENCODING:
      start = NameEncoding( start, end );
      break;

    case STATE_FILE_LENGTH_LENGTH:
      start = FileLengthLength( start, end );
      break;

    case STATE_NAME_LENGTH:
      start = NameLength( start, end );
      break;

    case STATE_NAME:
      start = Name( start, end );
      break;

    case STATE_DATE_LENGTH:
      start = DateLength( start, end );
      break;

    case STATE_DATE:
      start = Date( start, end );
      break;

    case STATE_FILE_LENGTH:
      start = FileLength( start, end );
      break;

    case STATE_FILE:
      start = File( start, end );
      break;

    case STATE_DONE:
      throw Error( "More data received after the end of the archive" );
    }
  }
}

void ArchiveReader::End()
{
}

const uint8_t* ArchiveReader::NameEncoding( const uint8_t* start,
                                            const uint8_t* end )
{
  if ( *start == '2' )
  {
    m_nameEncoding = ARCHIVE_ENCODING_UTF8;
    start++;
  }
  else
  {
    m_nameEncoding = ARCHIVE_ENCODING_LOCAL;
  }

  m_state = STATE_FILE_LENGTH_LENGTH;
  return start;
}

const uint8_t* ArchiveReader::FileLengthLength( const uint8_t* start,
                                                const uint8_t* end )
{
  if ( *start == '1' )
  {
    m_fileLengthLength = 8;
  }
  else if ( *start == '_' )
  {
    m_fileLengthLength = 4;
  }
  else
  {
    throw Error( "Invalid archive" );
  }

  m_state = STATE_NAME_LENGTH;
  return start + 1;
}

const uint8_t* ArchiveReader::NameLength( const uint8_t* start,
                                          const uint8_t* end )
{
  m_length = *start;

  if ( m_length )
  {
    m_pos = 0;
    m_state = STATE_NAME;
  }
  else
  {
    m_state = STATE_DONE;
  }

  return start + 1;
}

const uint8_t* ArchiveReader::Name( const uint8_t* start, const uint8_t* end )
{
  while ( start != end && m_pos != m_length )
  {
    uint8_t c = *start++;

    if ( c == '\\' )
      c = '/';

    m_name[m_pos++] = c;
  }

  if ( m_pos == m_length )
  {
    if ( m_name[m_pos - 1] == 0 )
      m_length--;
    else
      m_name[m_pos] = 0;

    m_isDirectory = ( m_name[m_length - 1] == '/' );
    m_state = STATE_DATE_LENGTH;
  }

  return start;
}

const uint8_t* ArchiveReader::DateLength( const uint8_t* start,
                                          const uint8_t* end )
{
  m_length = *start;
  m_pos = 0;
  m_state = STATE_DATE;
  return start + 1;
}

const uint8_t* ArchiveReader::Date( const uint8_t* start, const uint8_t* end )
{
  while ( start != end && m_pos != m_length )
    m_buffer[m_pos++] = *start++;

  if ( m_pos == m_length )
  {
    m_mtime = ReadDate( m_buffer, m_length );
    m_length = m_fileLengthLength;
    m_pos = 0;
    m_state = STATE_FILE_LENGTH;
  }

  return start;
}

const uint8_t* ArchiveReader::FileLength( const uint8_t* start,
                                          const uint8_t* end )
{
  while ( start != end && m_pos != m_length )
    m_buffer[m_pos++] = *start++;

  if ( m_pos == m_length )
  {
    m_fileLength = ReadNumber( m_buffer, m_length );

    if ( m_isDirectory )
    {
      if ( m_fileLength )
        throw Error( "A directory has a non-zero file length." );

      m_output.Directory(
        reinterpret_cast<const char*>( m_name ), m_nameEncoding, m_mtime );

      m_state = STATE_NAME_ENCODING;
    }
    else
    {
      m_output.FileStart( reinterpret_cast<const char*>( m_name ),
                          m_nameEncoding,
                          m_mtime,
                          m_fileLength );

      m_filePos = 0;
      m_state = STATE_FILE;
    }
  }

  return start;
}

const uint8_t* ArchiveReader::File( const uint8_t* start, const uint8_t* end )
{
  uint64_t amount = std::min<uint64_t>( end - start, m_fileLength - m_filePos );

  if ( amount )
  {
    m_output.FileWrite( start, start + static_cast<size_t>( amount ) );
    start += amount;
    m_filePos += amount;
  }

  if ( m_filePos == m_fileLength )
  {
    m_output.FileEnd();
    m_state = STATE_NAME_ENCODING;
  }

  return start;
}

}
