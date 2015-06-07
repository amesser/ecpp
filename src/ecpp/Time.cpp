/*
 * Time.cpp
 *
 *  Created on: 13.05.2015
 *      Author: andi
 */


#include "ecpp/Time.hpp"
#include "ecpp/Operators.hpp"

namespace ecpp
{
  uint32_t Time::calculateSecondsOfDay(uint8_t Hour, uint8_t Minute, uint8_t Second)
  {
    return (uint32_t)((uint16_t)Hour * 60 + Minute) * 60 + Second;
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

