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
    T _cnt;

  public:
    template<typename C>
    void update(C val)
    {
      const C maskc = ~static_cast<C>(0);
      const C part  = static_cast<C>(_cnt & maskc);

      if (val != part)
      {
        const T maskt = ~static_cast<T>(maskc);

        if (val > part)
        {
          _cnt = (_cnt & maskt) + val;
        }
        else
        {
          const T add = static_cast<T>(maskc) + 1;
          _cnt = (_cnt & maskt) + add + val;
        }
      }
    }

    constexpr T value()
    {
      return _cnt;
    }

    void init(T value)
    {
      _cnt = value;
    }

    template<typename C, unsigned long TMOUTRANGE = TypeProperties<C>::MaxSigned >
    class Timer
    {
    private:
      C _tmout;
    public:
      C hasTimedOut(const Clock & clock);
      void updateTimeout(C iv)
      {
        _tmout += iv;
      }

      void init(C iv);
      void init(const Clock &clock, C iv);

      constexpr C value()
      {
        return _tmout;
      }
    };
  };

  template<typename T>
  template<typename C, unsigned long TMOUTRANGE>
  C Clock<T>::Timer<C,TMOUTRANGE>::hasTimedOut(const Clock & clock)
  {
    const C maskc = ~static_cast<C>(0);
    const C part  =  static_cast<C>(clock.value() & maskc);
    C delta = part - _tmout;

    if (delta > TMOUTRANGE)
      delta = 0;
    else if (delta == 0)
      delta = 1;

    return delta;
  }

  template<typename T>
  template<typename C, unsigned long TMOUTRANGE>
  void Clock<T>::Timer<C,TMOUTRANGE>::init(const Clock &clock, C iv)
  {
    _tmout = clock.value() + iv;
  }

  template<typename T>
  template<typename C, unsigned long TMOUTRANGE>
  void Clock<T>::Timer<C,TMOUTRANGE>::init(C iv)
  {
    _tmout = iv;
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
        uint32_t dTime = Time.getSecondsOfDay() - m_Timout;

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
        uint32_t Timeout = Time::calculateSecondsOfDay(Hours, Minutes, Seconds);

        Timeout += m_Timout;

        while (Timeout > 86400UL)
          Timeout -= 86400;

        m_Timout = Timeout;
      }

      void initTimeout(const Time & time, uint16_t Hours, uint16_t Minutes, uint16_t Seconds)
      {
        m_Timout = time.getSecondsOfDay();
        updateTimeout(Hours, Minutes, Seconds);
      }
    };
  };
};



#endif /* EXTERNALS_ECPP_SRC_ECPP_TIME_HPP_ */
