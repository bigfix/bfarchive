#include "DateTime.h"
#include "BigFix/DataRef.h"
#include "BigFix/Error.h"
#include "BigFix/Number.h"

namespace BigFix
{

static DataRef months[12] =
{
  DataRef( "Jan" ),
  DataRef( "Feb" ),
  DataRef( "Mar" ),
  DataRef( "Apr" ),
  DataRef( "May" ),
  DataRef( "Jun" ),
  DataRef( "Jul" ),
  DataRef( "Aug" ),
  DataRef( "Sep" ),
  DataRef( "Oct" ),
  DataRef( "Nov" ),
  DataRef( "Dec" )
};

static uint8_t ReadMonth( DataRef month )
{
  for ( uint8_t i = 0; i < 12; i++ )
    if ( month == months[i] )
      return i;

  throw Error( "Invalid datetime: bad month" );
}

static DataRef days[7] =
{
  DataRef( "Sun" ),
  DataRef( "Mon" ),
  DataRef( "Tue" ),
  DataRef( "Wed" ),
  DataRef( "Thu" ),
  DataRef( "Fri" ),
  DataRef( "Sat" )
};

static void VerifyDayOfWeek( DataRef day )
{
  for ( size_t i = 0; i < 7; i++ )
    if ( day == days[i] )
      return;

    throw Error( "Invalid datetime: bad day of week" );
}

static void Verify( bool condition )
{
  if ( !condition )
    throw Error( "Invalid datetime" );
}

static int32_t ReadTimeZone( DataRef timeZone )
{
  if ( timeZone[0] != '-' && timeZone[0] != '+' )
    throw Error( "Invalid datetime: bad time zone" );

  int32_t number = ReadAsciiNumber<int32_t>( timeZone.Slice( 1, 4 ) );
  return ( timeZone[0] == '-' ) ? (-number) : number;
}

DateTime::DateTime()
  : m_year( 1970 )
  , m_month( 0 )
  , m_day( 0 )
  , m_hour( 0 )
  , m_minute( 0 )
  , m_second( 0 )
  , m_timeZone( 0 )
{
}

DateTime::DateTime( DataRef date )
{
  // Example: Sun, 11 Mar 1984 00:00:00 +0000

  Verify( date.Length() == 31 );
  VerifyDayOfWeek( date.Slice( 0, 3 ) );
  Verify( date.Slice( 3, 2 ) == DataRef( ", " ) );
  m_day = ReadAsciiNumber<uint8_t>( date.Slice( 5, 2 ) );
  Verify( date[7] == ' ' );
  m_month = ReadMonth( date.Slice( 8, 3 ) );
  Verify( date[11] == ' ' );
  m_year = ReadAsciiNumber<uint32_t>( date.Slice( 12, 4 ) );
  Verify( date[16] == ' ' );
  m_hour = ReadAsciiNumber<uint8_t>( date.Slice( 17, 2 ) );
  Verify( date[19] == ':' );
  m_minute = ReadAsciiNumber<uint8_t>( date.Slice( 20, 2 ) );
  Verify( date[22] == ':' );
  m_second = ReadAsciiNumber<uint8_t>( date.Slice( 23, 2 ) );
  Verify( date[25] == ' ' );
  m_timeZone = ReadTimeZone( date.Slice( 26, 5 ) );
}

std::string MakeString()
{
  return "";
}

}
