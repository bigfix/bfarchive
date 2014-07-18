#include "UTF8.h"
#include <stddef.h>
#include <stdint.h>

namespace BigFix
{

static bool InRange( uint8_t c, uint8_t low, uint8_t high )
{
  return low <= c && c <= high;
}

static bool IsTrail( uint8_t c )
{
  return 0x80 <= c && c <= 0xBF;
}

bool IsValidUTF8( const char* signedName )
{
  const uint8_t* name = reinterpret_cast<const uint8_t*>( signedName );

  while ( *name )
  {
    if ( InRange( name[0], 0x00, 0x7F ) )
    {
      name += 1;
    }
    else if ( InRange( name[0], 0xC2, 0xDF ) && IsTrail( name[1] ) )
    {
      name += 2;
    }
    else if ( name[0] == 0xE0 && InRange( name[1], 0xA0, 0xBF ) &&
              IsTrail( name[2] ) )
    {
      name += 3;
    }
    else if ( InRange( name[0], 0xE1, 0xEC ) && IsTrail( name[1] ) &&
              IsTrail( name[2] ) )
    {
      name += 3;
    }
    else if ( name[0] == 0xED && InRange( name[1], 0x80, 0x9F ) &&
              IsTrail( name[2] ) )
    {
      name += 3;
    }
    else if ( InRange( name[0], 0xEE, 0xEF ) && IsTrail( name[1] ) &&
              IsTrail( name[2] ) )
    {
      name += 3;
    }
    else if ( name[0] == 0xF0 && InRange( name[1], 0x90, 0xBF ) &&
              IsTrail( name[2] ) && IsTrail( name[3] ) )
    {
      name += 4;
    }
    else if ( InRange( name[0], 0xF1, 0xF3 ) && IsTrail( name[1] ) &&
              IsTrail( name[2] ) && IsTrail( name[3] ) )
    {
      name += 4;
    }
    else if ( name[0] == 0xF4 && InRange( name[1], 0x80, 0x8F ) &&
              IsTrail( name[2] ) && IsTrail( name[3] ) )
    {
      name += 4;
    }
    else
    {
      return false;
    }
  }

  return true;
}

}
