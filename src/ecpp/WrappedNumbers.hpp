/*
 * WrappedNumbers.hpp
 *
 *  Created on: 15.08.2015
 *      Author: andi
 */

#ifndef ECPP_WRAPPEDNUMBERS_HPP_
#define ECPP_WRAPPEDNUMBERS_HPP_

#include <ecpp/Datatypes.hpp>
#include <ecpp/VaTemplate.hpp>

namespace ecpp {



  template<typename T, int MAX>
  class WrappedInteger
  {
  private:
    T m_value;

    static T wrapNumber(T value)
    {
      while (value > MAX)
      {
        value -= (MAX + 1);
      }

      while (value < 0)
      {
        value += (MAX + 1);
      }

      return value;
    }
  public:
    constexpr WrappedInteger() {};
    constexpr WrappedInteger(T init) : m_value(init) {};

    WrappedInteger operator - (WrappedInteger rhs)
    {
      return wrapNumber(m_value - rhs.m_value + MAX + 1);
    }

    WrappedInteger & operator += (WrappedInteger rhs)
    {
      m_value = wrapNumber(m_value + rhs.m_value);
      return *this;
    }

    WrappedInteger & operator ++ ()
    {
      m_value = wrapNumber(m_value + 1);
      return *this;
    }

    WrappedInteger operator ++ (int)
    {
      T backup = m_value;
      ++(*this);
      return backup;
    }

    WrappedInteger & operator -- ()
    {
      m_value = wrapNumber(m_value + 3 - 1);
      return *this;
    }

    WrappedInteger operator -- (int)
    {
      T backup = m_value;
      --(*this);
      return backup;
    }

    bool operator == (WrappedInteger rhs) const
    {
      return m_value == rhs.m_value;
    }

    bool operator != (WrappedInteger rhs) const
    {
      return m_value != rhs.m_value;
    }

    constexpr T asInteger() {return m_value;}
  };
}




#endif /* EXTERNALS_ECPP_SRC_ECPP_WRAPPEDNUMBERS_HPP_ */
