/*
 * Time.hpp
 *
 *  Created on: 09.05.2015
 *      Author: andi
 */

#ifndef ECPP_TIME_HPP_
#define ECPP_TIME_HPP_

#include "ecpp/StringEncodings/Ascii.hpp"
#include "ecpp/Datatypes.hpp"
#include "ecpp/Operators.hpp"
#include "ecpp/String.hpp"

namespace ecpp
{
  template<typename C, unsigned int TIMEBASE>
  class SimpleTimer
  {
  private:
    C Counter;

    template<typename T>
    static constexpr C convertMilliseconds(T value) { return value / TIMEBASE;}

    template<typename T>
    static constexpr C convertSeconds(T value)      { return (unsigned long)value * 1000 / TIMEBASE;}


  public:
    constexpr SimpleTimer() {}
    constexpr SimpleTimer(const C init) : Counter(init) {}

    template<typename T>
    void startMilliseconds(T timeout) { Counter = convertMilliseconds(timeout);}

    template<typename T>
    void startSeconds     (T timeout) { Counter = convertSeconds(timeout);}

    void stop  ()             {Counter = 0;}

    template<typename T = C>
    constexpr T getRemainingMilliseconds()   const { return Counter * TIMEBASE;}

    template<typename T = C>
    constexpr T getRemainingSeconds()       const { return Counter * (unsigned long)TIMEBASE / 1000;}

    template<typename T>
    constexpr T getElapsedMilliseconds(T timeout) const { return timeout - getRemainingMilliseconds<T>();}

    template<typename T>
    constexpr T getElapsedSeconds(T timeout) const { return timeout - getRemainingSeconds<T>();}

    constexpr bool hasTimedOut() const {return 0 == Counter;}

    template<typename T>
    void handleTicksPassed (T TimePassed)
    {
      if (TimePassed > Counter)
      {
        Counter = 0;
      }
      else
      {
        Counter = Counter - TimePassed;
      }
    }

    template<typename T>
    void handleMillisecondsPassed (T Milliseconds)
    {
      handleTicksPassed((Milliseconds + TIMEBASE - 1) / TIMEBASE);
    }

    template<typename T>
    void handleSecondsPassed (T Seconds)
    {
      handleTicksPassed((Seconds * 1000UL + TIMEBASE - 1) / TIMEBASE);
    }

  };


  template<typename C, unsigned long TMOUTRANGE = TypeProperties<C>::MaxSigned >
  class Timer
  {
  public:
  private:
    C Timeout;
  public:
    template<typename CLOCK>
    C hasTimedOut(const CLOCK & clock);

    void updateTimeout(C iv)
    {
      Timeout += iv;
    }

    void init(C iv);

    template<typename CLOCK>
    void init(const CLOCK &clock, C iv);

    constexpr C value()
    {
      return Timeout;
    }
  };

  template<typename C, unsigned long TMOUTRANGE>
  template<typename T>
  C Timer<C, TMOUTRANGE>::hasTimedOut(const T & clock)
  {
    C delta = clock.value() - Timeout;

    if (delta > TMOUTRANGE)
      delta = 0;
    else if (delta == 0)
      delta = 1;

    return delta;
  }

  template<typename C, unsigned long TMOUTRANGE>
  template<typename T>
  void Timer<C, TMOUTRANGE>::init(const T &clock, C iv)
  {
    Timeout = clock.value() + iv;
  }

  template<typename C, unsigned long TMOUTRANGE>
  void Timer<C, TMOUTRANGE>::init(C iv)
  {
    Timeout = iv;
  }

  class TimeDelta
  {
  public:
    uint_least8_t Hour;
    uint_least8_t Minute;
    uint_least8_t Second;
  };

  class TimeBase
  {

  };

  class DefaultTime : public TimeBase
  {
  protected:
    uint_least8_t Hour;
    uint_least8_t Minute;
    uint_least8_t Second;
  public:
    constexpr DefaultTime() : Hour(0), Minute(0), Second(0) {};

    constexpr DefaultTime(uint_fast8_t Hour, uint_fast8_t Minute, uint_fast8_t Second)
      : Hour(Hour), Minute(Minute), Second(Second) {};

    template<typename T>
    constexpr DefaultTime(const T & init) : Hour(init.getHour()), Minute(init.getMinute()), Second(init.getSecond()) {};

    constexpr uint_fast8_t getSecond() const {return Second;}
    constexpr uint_fast8_t getMinute() const {return Minute;}
    constexpr uint_fast8_t getHour()   const {return Hour;}

    bool add(TimeDelta rhs);
    bool sub(TimeDelta rhs);

    void setSecond(uint_fast8_t value) {Second = value;}

    void set(const DefaultTime &value)
    {
      Hour   = value.Hour;
      Minute = value.Minute;
      Second = value.Second;
    }

    template<typename RHS>
    constexpr bool operator==(const RHS & rhs)
    {
      return (rhs.getHour() == getHour()) and (rhs.getMinute() == getMinute()) and (rhs.getSecond() == getSecond());
    }

    template<typename RHS>
    bool operator>=(const RHS & rhs) const;

    template<typename RHS>
    bool operator<(const RHS & rhs) const;

    DefaultTime operator + (const TimeDelta & rhs) const
    {
      DefaultTime ret(*this);
      ret.add(rhs);
      return ret;
    }
  };

  template<typename RHS>
  bool DefaultTime::operator>=(const RHS & rhs) const
  {
    if (getHour() > rhs.getHour())
      return true;

    if (getHour() < rhs.getHour())
      return false;

    if (getMinute() > rhs.getMinute())
      return true;

    if (getMinute() < rhs.getMinute())
      return false;

    return getSecond() >= rhs.getSecond();
  }

  template<typename RHS>
  bool DefaultTime::operator<(const RHS & rhs) const
  {
    if (getHour() < rhs.getHour())
      return true;

    if (getHour() > rhs.getHour())
      return false;

    if (getMinute() < rhs.getMinute())
      return true;

    if (getMinute() > rhs.getMinute())
      return false;

    return getSecond() < rhs.getSecond();
  }



  bool isLeapYear(uint_fast8_t Century, uint_fast8_t Year);

  uint_fast8_t calcDaysPerMonth(uint_fast8_t Month, bool isLeapYear);

  class DateDelta
  {
  public:
    uint_least16_t Years;
    uint_least8_t  Months;
    uint_least8_t  Days;
  };

  class DateBase
  {

  };

  class DefaultDate : public DateBase
  {
  protected:
    uint_least16_t Year;
    uint_least8_t  Month;
    uint_least8_t  Day;
  public:
    constexpr DefaultDate() : Year(0), Month(0), Day(0) {}

    template<typename T>
    constexpr DefaultDate(const T & init) : Year(init.getYear()), Month(init.getMonth()), Day(init.getDay()) {}

    constexpr uint_fast16_t getYear()  const {return Year;}
    constexpr uint_fast8_t  getMonth() const {return Month;}
    constexpr uint_fast8_t  getDay()   const {return Day;}

    template<typename RHS>
    constexpr bool operator==(const RHS & rhs)
    {
      return (rhs.getYear() == getYear()) and (rhs.getMonth() == getMonth()) and (rhs.getDay() == getDay());
    }

    template<typename RHS>
    bool operator>(const RHS & rhs) const;

    template<typename RHS>
    bool operator>=(const RHS & rhs) const;

    DefaultDate operator + (const DateDelta & rhs) const;
  };

  template<typename RHS>
  bool DefaultDate::operator>(const RHS & rhs) const
  {
    if (getYear() > rhs.getYear())
      return true;

    if (getYear() < rhs.getYear())
      return false;

    if (getMonth() > rhs.getMonth())
      return true;

    if (getMonth() < rhs.getMonth())
      return false;

    return getDay() > rhs.getDay();
  }

  template<typename RHS>
  bool DefaultDate::operator>=(const RHS & rhs) const
  {
    if (getYear() > rhs.getYear())
      return true;

    if (getYear() < rhs.getYear())
      return false;

    if (getMonth() > rhs.getMonth())
      return true;

    if (getMonth() < rhs.getMonth())
      return false;

    return getDay() >= rhs.getDay();
  }


  template<int CENTURY>
  class FixedCenturyDate : public DateBase
  {
  public:
    static constexpr uint_least8_t  Century   = CENTURY;
    static constexpr uint_least16_t YearStart = CENTURY * 100;

  protected:
     /* Highest Bit of the year variable indicates a leap year */
    uint_least8_t Year;
    uint_least8_t Month;
    uint_least8_t Day;

    static constexpr uint_fast8_t calcYear(uint_fast8_t Year)
    {
      return Year | (ecpp::isLeapYear(Century, Year) ? 0x80 : 0x00);
    }

    static constexpr uint_fast8_t limitYear(uint_fast16_t Year)
    {
      return max(min(Year, YearStart + 99U), (uint_fast16_t)YearStart) - YearStart;
    }
  public:
    FixedCenturyDate() {};

    constexpr FixedCenturyDate(uint_fast16_t Year, uint_fast8_t Month, uint_fast8_t Day)
      : Year(calcYear(limitYear(Year))), Month(Month), Day(Day) {};

    constexpr uint_fast16_t getYear()  const {return YearStart + (Year & 0x7F);}
    constexpr uint_fast8_t  getMonth() const {return Month;}
    constexpr uint_fast8_t  getDay()   const {return Day;}

    uint_fast8_t  getDaysInMonth() const {return calcDaysPerMonth(getMonth(), isLeapYear());}

    bool isLeapYear() const {return (0x80 & Year);}
    bool isValid()    const {return true;}

    bool add(DateDelta rhs);

    void set(FixedCenturyDate value)
    {
      Year  = value.Year;
      Month = value.Month;
      Day   = value.Day;
    }
  };

  template<int CENTURY>
  bool FixedCenturyDate<CENTURY>::add(DateDelta rhs)
  {
    const uint_least8_t mask_year = 0x7F;
    bool overflow;
    uint_fast8_t tmp, days;

    if(rhs.Days > 0)
    {
      days = calcDaysPerMonth(Month, isLeapYear());

      tmp = Day + rhs.Days;

      if (tmp >= days)
      {
        Day = tmp - days;
        tmp = Month + rhs.Months + 1;
      }
      else
      {
        Day = tmp;
        tmp = Month + rhs.Months;
      }
    }
    else
    {
      tmp = Month + rhs.Months;
    }

    if (tmp > 11)
    {
      Month = tmp - 12;
      tmp   = (Year & mask_year) + 1;
    }
    else
    {
      Month = tmp;
      tmp   = (Year & mask_year);
    }

    if(tmp > 99 || (rhs.Years > (99 - tmp)))
    {
      tmp = 99;
      overflow = true;
    }
    else
    {
      tmp = tmp + rhs.Years;
      overflow = false;
    }

    if (tmp != (Year & mask_year))
    { /* when year changes, recalc leap bit */
      Year = calcYear(tmp);
    }

    days = calcDaysPerMonth(Month, isLeapYear());

    if(Day >= days)
    {
      Day = Day - days;
    }

    return overflow;
  }

  template<typename DateType_, typename TimeType_>
  class DateTime
  {
  public:
    typedef DateType_ DateType;
    typedef TimeType_ TimeType;

    constexpr DateTime() : date(), time() {}

    template<typename INIT>
    constexpr DateTime(const INIT & init) : date(init.getDate()), time(init.getTime()) {}

    constexpr const TimeType & getTime() const {return time;}
    constexpr const DateType & getDate() const {return date;}

    constexpr uint_fast16_t getYear()  const { return date.getYear();}
    constexpr uint_fast8_t  getMonth() const { return date.getMonth();}
    constexpr uint_fast8_t  getDay()   const { return date.getDay();}

    constexpr uint_fast8_t getSecond() const { return time.getSecond();}
    constexpr uint_fast8_t getMinute() const { return time.getMinute();}
    constexpr uint_fast8_t getHour()   const { return time.getHour();}


    void set(const TimeType& value) { time.set(value);}
    void set(const DateType& value) { date.set(value);}

    void add(TimeDelta rhs)
    {
      if (time.add(rhs))
        date.add({0,0,1});
    }

    template<typename ...ARGS>
    static DateType makeDate(ARGS... args) {return DateType(args...);}

    template<typename ...ARGS>
    static TimeType makeTime(ARGS... args) {return TimeType(args...);}

    template<typename RHS>
    constexpr bool operator== (const RHS & rhs) {return date == rhs.getDate() and time == rhs.getTime(); }

  private:
    DateType date;
    TimeType time;
  };

  template<typename TimeType>
  void formatTime(const TimeType& dt, char (&Buffer)[8]) __attribute__((noinline));

  template<typename DateType>
  void formatDate(const DateType& dt, char (&Buffer)[10]) __attribute__((noinline));

  template<typename DateTimeType>
  void formatDateTime(const DateTimeType& dt, char (&Buffer)[19]) __attribute__((noinline));


  template<typename TimeType>
  void formatTime(const TimeType& dt, char (&Buffer)[8])
  {
    using ecpp::StringEncodings::Ascii;

    Buffer[2] = ':';
    Buffer[5] = ':';
    Ascii::convertToDecimal(&(Buffer[0]), 2, dt.getHour());
    Ascii::convertToDecimal(&(Buffer[3]), 2, dt.getMinute());
    Ascii::convertToDecimal(&(Buffer[6]), 2, dt.getSecond());
  }

  template<typename DateType>
  void formatDate(const DateType& dt, char (&Buffer)[10])
  {
    using ecpp::StringEncodings::Ascii;
    Buffer[4] = '-';
    Buffer[7] = '-';
    Ascii::convertToDecimal(&(Buffer[0]),  4, dt.getYear());
    Ascii::convertToDecimal(&(Buffer[5]),  2, dt.getMonth() + 1);
    Ascii::convertToDecimal(&(Buffer[8]),  2, dt.getDay()   + 1);
  }

  template<typename DateTimeType>
  void formatDateTime(const DateTimeType& dt, char (&Buffer)[19])
  {
    Buffer[10] = 'T';
    formatDate(dt, *reinterpret_cast< char (*)[10]>(&(Buffer[0])));
    formatTime(dt, *reinterpret_cast< char (*)[8]>(&(Buffer[11])));
  }

  template<class DateTimeType>
  class Clock : public DateTimeType
  {
  public:
    void tick()
    {
      DateTimeType::add({0,0,1});
    }
  };

  class WeekTime
  {
  private:
    uint8_t m_Second;
    uint8_t m_Minute;
    uint8_t m_Hour;
    uint8_t m_WeekDay;
  public:
    WeekTime() : m_Second(0), m_Minute(0), m_Hour(0), m_WeekDay(0) {};

    template<typename T>
    void setMonotonic(T seconds);

    uint8_t getSecond()  const {return m_Second;}
    uint8_t getMinute()  const {return m_Minute;}
    uint8_t getHour()    const {return m_Hour;}
    uint8_t getWeekDay() const {return m_WeekDay;}

    void setSecond(uint8_t Second)   {m_Second  = Second;};
    void setMinute(uint8_t Minute)   {m_Minute  = Minute;};
    void setHour  (uint8_t Hour)     {m_Hour    = Hour;};
    void setWeekDay(uint8_t WeekDay) {m_WeekDay = WeekDay;};

    void incSecond(uint8_t inc);
    void decSecond(uint8_t dec);

    void incMinute(uint8_t inc);
    void decMinute(uint8_t dec);

    void incHour(uint8_t inc);
    void decHour(uint8_t dec);

    void decWeekday(uint8_t dec) { subWrapped(m_WeekDay, dec, (uint8_t)7);}


    uint32_t                  getMonotonic() const;
    static constexpr uint32_t getWrapAround() {return 7UL * 24UL * 60UL * 60UL;}

    uint32_t operator - (const WeekTime & rhs) const;
  };

  template<typename T>
  void WeekTime::setMonotonic(T seconds)
  {
    m_Second = m_Minute = m_Hour = m_WeekDay = 0;
    while(seconds >= 255)
    {
      incSecond(255);
      seconds -= 255;
    }

    incSecond(seconds);
  }
};



#endif /* EXTERNALS_ECPP_SRC_ECPP_TIME_HPP_ */
