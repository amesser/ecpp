/*
 * Time.hpp
 *
 *  Created on: 09.05.2015
 *      Author: andi
 */

#ifndef ECPP_TIME_HPP_
#define ECPP_TIME_HPP_

#include "ecpp/Datatypes.hpp"
#include "ecpp/Operators.hpp"

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

  class Time
  {
  public:
    typedef uint8_t SecondType;
    typedef uint8_t MinuteType;
    typedef uint8_t HourType;
  protected:
    SecondType m_Second;
    MinuteType m_Minute;
    HourType   m_Hour;

  public:
    SecondType getSecond() const {return m_Second;}
    MinuteType getMinute() const {return m_Minute;}
    HourType   getHour()   const {return m_Hour;}


    void setSeconds(SecondType Seconds)
    {
      m_Second = Seconds;
    }

    void setMinute(MinuteType Minute)
    {
      m_Minute = Minute;
    }

    void setHour(HourType Hour)
    {
      m_Hour = Hour;
    }

    static uint32_t calculateSecondsOfDay(uint8_t Hour, uint8_t Minute, uint8_t Second);

    static constexpr HourType getHoursPerDay()
    {
      return 24;
    }

    static constexpr MinuteType getMinutesPerHour()
    {
      return 60;
    }

    uint32_t getSecondsOfDay() const { return calculateSecondsOfDay(m_Hour, m_Minute, m_Second);}

    static constexpr uint32_t getMaxMonotonic() { return 60UL * 60 * 24 - 1;};
    uint32_t getMonotonic() const { return getSecondsOfDay();}

    bool tick();

    class Timer
    {
    protected:
      uint32_t m_Timout;

    public:
      uint16_t hasTimedOut(const Time & Time)
      {
        auto wrap = Time::getMaxMonotonic();
        auto dTime = Time.getMonotonic();

        wrap += 1;

        if(dTime < m_Timout)
          dTime = dTime + wrap - m_Timout;
        else
          dTime = dTime - m_Timout;

        /* we hopefully get called at elast once in an hour */
        if (dTime > 3600)
        {
          dTime = 0;
        }
        else if (dTime == 0)
        {
          dTime = 1;
        }

        return dTime;
      }

      void updateTimeout(uint16_t Hours, uint16_t Minutes, uint16_t Seconds)
      {
        auto wrap    = Time::getMaxMonotonic();
        auto Timeout = Time::calculateSecondsOfDay(Hours, Minutes, Seconds);

        wrap += 1;

        Timeout += m_Timout;

        while (Timeout >= wrap)
          Timeout -= wrap;

        m_Timout = Timeout;
      }

      void initTimeout(const Time & time, uint16_t Hours, uint16_t Minutes, uint16_t Seconds)
      {
        m_Timout = time.getMonotonic();
        updateTimeout(Hours, Minutes, Seconds);
      }
    };
  };

  class Date
  {
  public:
    typedef uint16_t YearType;
    typedef uint8_t  MonthType;
    typedef uint8_t  DayType;
  protected:
    YearType  m_Year;
    MonthType m_Month;
    DayType   m_Day;
  public:
    YearType  getYear()  const {return m_Year;}
    MonthType getMonth() const {return m_Month;}
    DayType   getDay()   const {return m_Day;}

    void      setYear  (YearType Year)  {m_Year = Year;}
    void      setMonth(MonthType Month) {m_Month = Month;}
    void      setDay  (DayType   Day)   {m_Day = Day;}

    DayType             getDaysPerMonth()  const;
    constexpr MonthType getMonthsPerYear() {return 12;}
    constexpr YearType  getMaxYear()       {return ~(static_cast<YearType>(0));}

    void next();
  };

  class DateTime
  {
  protected:
    Date m_Date;
    Time m_Time;
  public:
    Date::YearType   getYear()  const {return m_Date.getYear();}
    Date::MonthType  getMonth() const {return m_Date.getMonth();}
    Date::DayType    getDay()   const {return m_Date.getDay();}

    Time::SecondType getSecond() const {return m_Time.getSecond();}
    Time::MinuteType getMinute() const {return m_Time.getMinute();}
    Time::HourType   getHour()   const {return m_Time.getHour();}


    const Date & getDate() const
    {
      return m_Date;
    }

    void setDate(const Date & date)
    {
      m_Date = date;
      m_Time.setSeconds(0);
    }

    const Time & getTime() const
    {
      return m_Time;
    }

    void setTime(const Time & Time)
    {
      m_Time = Time;
      m_Time.setSeconds(0);
    }

    void formatUTCTime(char* Buffer) const;

    void tick();

    /** Check if the date time object contains valid values
     * for date and time
     */
    bool isValid(void) const;
  };

  template<class DT>
  class Clock : public DT
  {
  public:

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
