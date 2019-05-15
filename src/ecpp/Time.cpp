/*
 * Time.cpp
 *
 *  Created on: 13.05.2015
 *      Author: andi
 */


#include "ecpp/Time.hpp"
#include "ecpp/Operators.hpp"
#include "ecpp/String.hpp"

namespace ecpp
{


  bool isLeapYear(uint_fast8_t Century, uint_fast8_t Year)
  {
    bool leap;

    if(Year == 0)
    {
      leap = (0 == (Century % 4));
    }
    else
    {
      leap = (0 == (Year % 4));
    }

    return leap;
  }

  bool isLeapYear(uint_fast16_t Year)
  {
    return isLeapYear(Year / 100, Year % 100);
  }

  uint_fast8_t calcDaysPerMonth(uint_fast8_t Month, bool isLeapYear)
  {
    uint8_t days;

    if (Month == 1)
    {
      if (isLeapYear)
        days = 29;
      else
        days = 28;
    }
    else if (Month < 7)
    {
      days = 31 - (Month % 2);
    }
    else
    {
      days = 30 + (Month % 2);
    }

    return days;
  }


  DefaultDate
  DefaultDate::operator + (const DateDelta & rhs) const
  {
    DefaultDate ret(*this);
    bool is_leap;
    uint_least8_t days;

    days = rhs.Days;
    is_leap = isLeapYear(ret.Year);
    do
    {
      const auto days_per_month = calcDaysPerMonth(ret.Month, is_leap);

      if ((days + ret.Day) >= days_per_month)
      {
        ret.Month += 1;
        days -= days_per_month;

        if (ret.Month >= 12)
        {
          ret.Month -= 12;
          ret.Year  += 1;
          is_leap = isLeapYear(ret.Year);
        }
      }
      else
      {
        ret.Day += days;
        days = 0;
      }
    } while(days);

    ret.Month += rhs.Months % 12;
    ret.Year  += rhs.Months / 12;
    ret.Year  += rhs.Years;

    return ret;
  }

  void WeekTime::incSecond(uint8_t inc)
  {
    uint8_t overflows;

    overflows = addWrapped(m_Second,  inc, (uint8_t)60);
    incMinute(overflows);
  }

  void WeekTime::incMinute(uint8_t inc)
  {
    uint8_t overflows;

    overflows = addWrapped(m_Minute,  inc, (uint8_t)60);
    incHour(overflows);
  }

  void WeekTime::decMinute(uint8_t dec)
  {
    uint8_t overflows;

    overflows = subWrapped(m_Minute,  dec, (uint8_t)60);
    decHour(overflows);
  }

  void WeekTime::incHour(uint8_t inc)
  {
    uint8_t overflows;

    overflows = addWrapped(m_Hour, inc, (uint8_t)24);
    addWrapped(m_WeekDay, overflows, (uint8_t)7);
  }

  void WeekTime::decHour(uint8_t dec)
  {
    uint8_t overflows;

    overflows = subWrapped(m_Hour,  dec, (uint8_t)24);
    decWeekday(overflows);
  }



  uint32_t WeekTime::getMonotonic() const
  {
    uint32_t x;

    x = m_WeekDay;
    x = x * 24 + m_Hour;
    x = x * 60 + m_Minute;
    x = x * 60 + m_Second;

    return x;
  }

  uint32_t WeekTime::operator - (const WeekTime & rhs) const
  {
    auto l = this->getMonotonic();
    auto r = rhs.getMonotonic();

    subWrapped(l,r, getWrapAround());
    return l;
  }

  bool DefaultTime::add(TimeDelta rhs)
  {
    uint_fast8_t tmp;

    tmp = Second + rhs.Second;

    if (tmp >= 60)
    {
      Second = tmp - 60;
      tmp    = 1 + Minute + rhs.Minute;
    }
    else
    {
      Second = tmp;
      tmp    = Minute + rhs.Minute;
    }

    if (tmp >= 60)
    {
      Minute = tmp - 60;
      tmp    = 1 + Hour + rhs.Hour;
    }
    else
    {
      Minute = tmp;
      tmp    = Hour + rhs.Hour;
    }

    if(tmp >= 24)
    {
      Hour = tmp - 24;
      tmp = 1;
    }
    else
    {
      Hour = tmp;
      tmp = 0;
    }

    return tmp != 0;
  }

  bool DefaultTime::sub(TimeDelta rhs)
  {
    uint_fast8_t tmp;

    tmp = Second - rhs.Second;

    if (tmp >= 60)
    {
      Second = tmp + 60;
      tmp    = Minute - rhs.Minute - 1;
    }
    else
    {
      Second = tmp;
      tmp    = Minute - rhs.Minute;
    }

    if (tmp >= 60)
    {
      Minute = tmp + 60;
      tmp    = Hour - rhs.Hour - 1;
    }
    else
    {
      Minute = tmp;
      tmp    = Hour - rhs.Hour;
    }

    if(tmp >= 24)
    {
      Hour = tmp + 24;
      tmp = 1;
    }
    else
    {
      Hour = tmp;
      tmp = 0;
    }

    return tmp != 0;
  }
}

