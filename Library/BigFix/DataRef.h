#ifndef BigFix_DataRef_h
#define BigFix_DataRef_h

#include <stdint.h>
#include <string>

namespace BigFix
{

class DataRef
{
public:
  DataRef() : m_start( 0 ), m_end( 0 ) {}

  DataRef( const uint8_t* start, const uint8_t* end )
    : m_start( start ), m_end( end )
  {
  }

  explicit DataRef( const std::string& s )
    : m_start( reinterpret_cast<const uint8_t*>( s.data() ) )
    , m_end( m_start + s.size() )
  {
  }

  template <size_t n>
  explicit DataRef( const char ( &literal )[n] )
    : m_start( reinterpret_cast<const uint8_t*>( literal ) )
    , m_end( m_start + n - 1 )
  {
  }

  const uint8_t* Start() const { return m_start; }
  const uint8_t* End() const { return m_end; }

  size_t Length() const { return m_end - m_start; }
  bool IsEmpty() const { return m_start == m_end; }

private:
  const uint8_t* m_start;
  const uint8_t* m_end;
};

}

#endif
