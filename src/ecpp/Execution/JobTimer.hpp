/*
 *  Copyright 2019 Andreas Messer <andi@bastelmap.de>
 *
 *  This file is part of the Embedded C++ Platform Project.
 *
 *  Embedded C++ Platform Project (ECPP) is free software: you can
 *  redistribute it and/or modify it under the terms of the GNU General
 *  Public License as published by the Free Software Foundation,
 *  either version 3 of the License, or (at your option) any later
 *  version.
 *
 *  Embedded C++ Platform Project is distributed in the hope that it
 *  will be useful, but WITHOUT ANY WARRANTY; without even the implied
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ECPP.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  As a special exception, the copyright holders of ECPP give you
 *  permission to link ECPP with independent modules to produce an
 *  executable, regardless of the license terms of these independent
 *  modules, and to copy and distribute the resulting executable under
 *  terms of your choice, provided that you also meet, for each linked
 *  independent module, the terms and conditions of the license of that
 *  module.  An independent module is a module which is not derived from
 *  or based on ECPP.  If you modify ECPP, you may extend this exception
 *  to your version of ECPP, but you are not obligated to do so.  If you
 *  do not wish to do so, delete this exception statement from your
 *  version.
 *  */
#ifndef ECPP_EXECUTION_JOBTIMER_HPP_
#define ECPP_EXECUTION_JOBTIMER_HPP_

#include <cstdint>

#include "ecpp/Execution/JobQueue.hpp"
#include "ecpp/Units/Time.hpp"

namespace ecpp::Execution
{
  /* Implementation of a simple function pointer based task queue
    * mechanism. Intended to be used for serialize and asynchronous processing
    * of tasks of limited duration. */
  template<class TIMER>
  class TimerQueue;

  template<typename BASE>
  class Timer : public BASE
  {
  public:
    typedef typename BASE::TicksType      TicksType;
    typedef typename TicksType::DeltaType TickDeltaType;

    template<typename T, signed POWER, unsigned EXP>
    void start(::ecpp::Units::FixedScaleTime<T, POWER, EXP> timeout, Job & job)
    {
      start(TickDeltaType(timeout), job);
    }

    template<typename T, signed POWER, unsigned EXP>
    void cont(::ecpp::Units::FixedScaleTime<T, POWER, EXP> timeout, Job & job)
    {
      cont(TickDeltaType(timeout), job);
    }

    void  stop();

    constexpr bool isRunning() const { return m_NextTimer != this; }

    constexpr bool operator > (const Timer & timer) const
    {
      return (m_TimeoutTicks > timer.m_TimeoutTicks);
    }

    constexpr bool operator < (const Timer & timer) const
    {
      return (m_TimeoutTicks < timer.m_TimeoutTicks);
    }

    Job* getJob() const { return m_pJob; }

  private:
    TicksType   m_TimeoutTicks  {};
    Timer*      m_NextTimer     {this};
    Job*        m_pJob          {nullptr};

    void  cont(TickDeltaType timeout, Job &job);
    void start(TickDeltaType ticks,   Job &job);

    friend class TimerQueue<Timer>;
  };

  template<typename BASE>
  void Timer<BASE>::start(TickDeltaType timeout, Job& job)
  {
    stop();

    m_pJob = &job;
    m_TimeoutTicks = BASE::getTicks() + timeout;

    BASE::getQueue().insertTimer(*this);
  }

  template<typename BASE>
  void Timer<BASE>::cont(TickDeltaType timeout, Job& job)
  {
    stop();

    m_pJob = &job;
    m_TimeoutTicks += timeout;

    BASE::getQueue().insertTimer(*this);
  }

  template<typename BASE>
  void Timer<BASE>::stop()
  {
    BASE::getQueue().removeTimer(*this);
  }

  template<class TIMER>
  class TimerQueue
  {
  public:
    typedef TIMER TimerType;

    TimerType* next();

  private:
    TimerType* m_NextTimer {nullptr};

    void insertTimer(TimerType & timer);
    void removeTimer(TimerType & timer);

    friend TimerType;
  };

  template<class TIMER>
  void TimerQueue<TIMER>::insertTimer(TimerType & timer)
  {
    TimerType** p = &m_NextTimer;
    TimerType* pIt;

    /* sanity check */
    if(timer.m_NextTimer != &timer)
      return;

    while(0 != (pIt = *p))
    {
      if (timer < *pIt)
        break;

      p = &(pIt->m_NextTimer);
    }

    timer.m_NextTimer = *p;
    *p = &timer;
  }

  template<class TIMER>
  void TimerQueue<TIMER>::removeTimer(TimerType & timer)
  {
    TimerType** p = &m_NextTimer;
    TimerType* pIt;

    while(0 != (pIt = *p))
    {
      if(pIt == &timer)
      {
        *p = pIt->m_NextTimer;
        pIt->m_NextTimer = pIt;
        break;
      }

      p = &(pIt->m_NextTimer);
    }
  }

  template<class TIMER>
  typename TimerQueue<TIMER>::TimerType* TimerQueue<TIMER>::next()
  {
    TimerType* pIt = m_NextTimer;

    if(pIt)
    {
      if ( pIt->getTicks() >= pIt->m_TimeoutTicks )
      {
        m_NextTimer = pIt->m_NextTimer;
        pIt->m_NextTimer = pIt;
      }
      else
      {
        pIt = nullptr;
      }
    }

    return pIt;
  }
}


#endif /* ECPP_EXECUTION_JOBQUEUE_HPP_ */
