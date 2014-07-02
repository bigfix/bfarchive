#ifndef BigFix_DateTime_h
#define BigFix_DateTime_h

#include <stdint.h>
#include <time.h>
#include <string>

namespace BigFix
{

class DataRef;

class DateTime
{
public:
  DateTime();
  explicit DateTime( DataRef );

  uint32_t Year() const { return m_year; }
  uint8_t Month() const { return m_month; }
  uint8_t Day() const { return m_day; }
  uint8_t DayOfWeek() const { return m_dayOfWeek; }
  uint8_t Hour() const { return m_hour; }
  uint8_t Minute() const { return m_minute; }
  uint8_t Second() const { return m_second; }
  int32_t TimeZone() const { return m_timeZone; }

private:
  uint32_t m_year;
  uint8_t m_month;
  uint8_t m_day;
  uint8_t m_dayOfWeek;
  uint8_t m_hour;
  uint8_t m_minute;
  uint8_t m_second;
  int32_t m_timeZone;
};

std::string MakeString( const DateTime& );

}

#endif
