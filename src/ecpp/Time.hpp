/*
 * Time.hpp
 *
 *  Created on: 09.05.2015
 *      Author: andi
 */

#ifndef ECPP_TIME_HPP_
#define ECPP_TIME_HPP_

#include "ecpp/Datatypes.hpp"

namespace ecpp
{
  template<typename T>
  class Clock
  {
  private:
    T Counter;

  public:
    template<typename C>
    C delta(C val)
    {
      return val - static_cast<C>(Counter);
    }

    template<typename C>
    void advance(C val)
    {
      Counter += val;
    }

    template<typename C>
    void update(C val)
    {
      advance(delta(val));
    }

    constexpr T value()
    {
      return Counter;
    }

    void init(T value)
    {
      Counter = value;
    }
  };

  template<typename C>
  class SimpleTimer
  {
  private:
    C Counter;
  public:
    void start (C timeout)    {Counter = timeout;}
    void stop  ()             {Counter = 0;}
    bool hasTimedOut() const  {return 0 == Counter;}

    template<typename T>
    void update (T TimePassed)
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
  protected:
    uint8_t m_Seconds;
    uint8_t m_Minutes;
    uint8_t m_Hours;
  public:


    uint8_t getSeconds() const {return m_Seconds;}
    uint8_t getMinutes() const {return m_Minutes;}
    uint8_t getHours()   const {return m_Hours;}

    static uint32_t calculateSecondsOfDay(uint8_t Hour, uint8_t Minute, uint8_t Second);

    uint32_t getSecondsOfDay() const { return calculateSecondsOfDay(m_Hours, m_Minutes, m_Seconds);}

    static constexpr uint32_t getMaxMonotonic() { return 60UL * 60 * 24 - 1;};
    uint32_t getMonotonic() const { return getSecondsOfDay();}

    void tick()
    {
      if (m_Seconds < 59)
      {
        m_Seconds += 1;
      }
      else if (m_Minutes < 59)
      {
        m_Seconds = 0;
        m_Minutes += 1;
      }
      else if (m_Hours < 23)
      {
        m_Seconds = 0;
        m_Minutes = 0;
        m_Hours   += 1;
      }
      else
      {
        m_Seconds = 0;
        m_Minutes = 0;
        m_Hours   = 0;
      }
    }

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
};



#endif /* EXTERNALS_ECPP_SRC_ECPP_TIME_HPP_ */
