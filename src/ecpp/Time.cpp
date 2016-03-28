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
  bool Time::tick()
  {
    bool Overflow = false;

    if (m_Second < 59)
    {
      m_Second += 1;
    }
    else if (m_Minute < 59)
    {
      m_Second = 0;
      m_Minute += 1;
    }
    else if (m_Hour < 23)
    {
      m_Second = 0;
      m_Minute = 0;
      m_Hour   += 1;
    }
    else
    {
      m_Second = 0;
      m_Minute = 0;
      m_Hour   = 0;

      Overflow = true;
    }

    return Overflow;
  }

  uint32_t Time::calculateSecondsOfDay(uint8_t Hour, uint8_t Minute, uint8_t Second)
  {
    return (uint32_t)((uint16_t)Hour * 60 + Minute) * 60 + Second;
  }

  Date::DayType
  Date::getDaysPerMonth() const
  {
    uint8_t DaysPerMonth;

    /* calculate number of days */
    if(m_Month == 2)
    {
      uint16_t Century,YearCentury;
      uint8_t Selector;

      Century     = m_Year / 100;
      YearCentury = Century * 100;

      if(0 == (m_Year - YearCentury))
      { /* Year is an even multiple of 100 */
        Selector = Century;
      }
      else
      {
        Selector = m_Year;
      }

      if(0 == (Selector % 4))
      {
        DaysPerMonth = 29;
      }
      else
      {
        DaysPerMonth = 28;
      }
    }
    else if(m_Month < 8)
    {
      DaysPerMonth = 30 + ((m_Month) % 2);
    }
    else
    {
      DaysPerMonth = 30 + ((m_Month + 1) % 2);
    }

    return DaysPerMonth;
  }

  void Date::next()
  {
    /* step date */
    if(m_Day < getDaysPerMonth())
    {
      m_Day += 1;
    }
    else if (m_Month < getMonthsPerYear())
    {
      m_Month += 1;
      m_Day    = 1;
    }
    else if (m_Year < getMaxYear())
    {
      m_Year  = m_Year + 1;
      m_Month = 1;
      m_Day   = 1;
    }
  }

  void DateTime::tick()
  {
    if(0 != m_Date.getDay() &&
       0 != m_Date.getMonth())
    {
      bool DayWrap;

      DayWrap = m_Time.tick();

      if(DayWrap)
      {
        m_Date.next();
      }
    }
  }

  bool
  DateTime::isValid(void) const
  {
    bool valid;

    if (0 == getMonth())
    {
      valid = false;
    }
    else if (0 == getDay())
    {
      valid = false;
    }
    else
    {
      valid = true;
    }

    return valid;
  }

  void
  DateTime::formatUTCTime(char* Buffer) const
  {
     /* YYYY-MM-DDTHH:MM:SSZ */
     String::convertToDecimal(&(Buffer[0]),  4, m_Date.getYear());
     Buffer[4] = '-';
     String::convertToDecimal(&(Buffer[5]),  2, m_Date.getMonth());
     Buffer[7] = '-';
     String::convertToDecimal(&(Buffer[8]),  2, m_Date.getDay());
     Buffer[10] = 'T';
     String::convertToDecimal(&(Buffer[11]), 2, m_Time.getHour());
     Buffer[13] = ':';
     String::convertToDecimal(&(Buffer[14]), 2, m_Time.getMinute());
     Buffer[16] = ':';
     String::convertToDecimal(&(Buffer[17]), 2, m_Time.getSecond());
     Buffer[19] = 'Z';
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

}

