/*
 *  Copyright 2016 Andreas Messer <andi@bastelmap.de>
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
#ifndef ECPP_MATH_FIXEDPOINT_HPP_
#define ECPP_MATH_FIXEDPOINT_HPP_

#include "ecpp/Datatypes.hpp"

namespace ecpp {
  template<typename TYPE, unsigned long SCALE>
  class FixedPoint;

#ifdef  _GLIBCXX_OSTREAM
  template<typename TYPE, unsigned long SCALE>
  std::ostream& operator<<(std::ostream& stream, const FixedPoint<TYPE,SCALE>& fp);
#endif

    template<typename TYPE, unsigned long SCALE>
    class FixedPoint
    {
    private:
      typedef typename DoubleWidthType<TYPE>::t_DoubleWidth t_DoubleWidth;
      TYPE m_value;

      template<typename VALUE>
      static constexpr TYPE rescale(const VALUE & value, const unsigned long & scale)
      {
        return static_cast<TYPE>( (SCALE >= scale) ?
            (value * static_cast<TYPE>(SCALE / scale)) : (value / (scale / SCALE)));
      }

    public:
      constexpr FixedPoint() {};

      constexpr FixedPoint(const FixedPoint<TYPE,SCALE> &init) :
        m_value( init.m_value) {};

      template<typename INIT>
      constexpr FixedPoint(const INIT &value, const unsigned long & scale = 1) :
        m_value( rescale(value,scale)) {};


      template<typename CAST>
      constexpr operator CAST () const {return static_cast<CAST>(m_value) / SCALE;}

      template<typename RHS>
      FixedPoint & operator = (const RHS & rhs)
      {
        m_value = rescale(rhs, 1);
        return *this;
      }

      template<typename RHS_TYPE, int RHS_SCALE>
      FixedPoint & operator = (const FixedPoint<RHS_TYPE, RHS_SCALE> & rhs)
      {
        m_value = rescale(rhs.m_value, RHS_SCALE);
        return *this;
      }

      template<typename RHS>
      constexpr FixedPoint operator * (const RHS & rhs) const
      {
        return {static_cast<t_DoubleWidth>(m_value) * rhs, SCALE};
      }

      template<typename RHS_TYPE, unsigned long RHS_SCALE>
      constexpr FixedPoint operator * (const FixedPoint<RHS_TYPE, RHS_SCALE> & rhs) const
      {
        return {static_cast<t_DoubleWidth>(m_value) * rhs.m_value / RHS_SCALE, SCALE};
      }

      template<typename RHS>
      constexpr FixedPoint operator / (const RHS & rhs) const
      {
        return {m_value / rhs, SCALE};
      }

      template<typename RHS_TYPE, unsigned long RHS_SCALE>
      constexpr FixedPoint operator / (const FixedPoint<RHS_TYPE, RHS_SCALE> & rhs) const
      {
        return {static_cast<t_DoubleWidth>(m_value) * RHS_SCALE / rhs.m_value, SCALE};
      }

      template<typename RHS>
      constexpr FixedPoint operator + (const RHS & rhs) const
      {
        return { m_value + rescale(rhs, 1), SCALE};
      }

      template<typename RHS_TYPE, unsigned long RHS_SCALE>
      constexpr FixedPoint operator + (const FixedPoint<RHS_TYPE, RHS_SCALE> & rhs) const
      {
        return {m_value + rescale(rhs.m_value, RHS_SCALE),SCALE};
      }

      template<typename RHS>
      constexpr FixedPoint operator - (const RHS & rhs) const
      {
        return { m_value - rescale(rhs, 1), SCALE};
      }

      template<typename RHS_TYPE, unsigned long RHS_SCALE>
      constexpr FixedPoint operator - (const FixedPoint<RHS_TYPE, RHS_SCALE> & rhs) const
      {
        return { m_value - rescale(rhs.m_value, RHS_SCALE), SCALE};
      }

      constexpr FixedPoint operator - () const
      {
        return {-m_value,SCALE};
      }

      FixedPoint operator -- (int)
      {
        TYPE bck = m_value;
        m_value -= rescale(1,1);

        return { bck, SCALE};
      }

      constexpr FixedPoint operator >> (uint_fast8_t shiftw) const
      {
        return {m_value >> shiftw,SCALE};
      }

      template<typename RHS>
      constexpr bool operator > (const RHS & rhs) const
      {
        return m_value > rescale(rhs,1);
      }

      template<typename RHS>
      constexpr bool operator >= (const RHS & rhs) const
      {
        return m_value >= rescale(rhs,1);
      }

      template<typename RHS_TYPE, unsigned long RHS_SCALE>
      constexpr bool operator >= (const FixedPoint<RHS_TYPE, RHS_SCALE> & rhs) const
      {
        return (static_cast<t_DoubleWidth>(m_value) * RHS_SCALE) >= \
            (static_cast<typename FixedPoint<RHS_TYPE, RHS_SCALE>::t_DoubleWidth>(rhs.m_value) * SCALE);
      }

      template<typename RHS>
      constexpr bool operator < (const RHS & rhs) const
      {
        return m_value < rescale(rhs,1);
      }

      template<typename RHS>
      constexpr bool operator <= (const RHS & rhs) const
      {
        return m_value <= rescale(rhs,1);
      }

    constexpr TYPE get_raw() const {return m_value;}

    constexpr TYPE set_raw(TYPE & value) {return (m_value = value);}

    constexpr TYPE set_raw(TYPE   value) {return (m_value = value);}

    template<typename FRIEND_TYPE, unsigned long FRIEND_SCALE>
    friend class FixedPoint;

#if 0
    template<typename FRIEND_TYPE>
    friend class Complex;
#endif

#ifdef  _GLIBCXX_OSTREAM
    friend std::ostream& operator<< <TYPE,SCALE>(std::ostream& stream, const FixedPoint& fp);
#endif
  };


#ifdef  _GLIBCXX_OSTREAM
  template<typename TYPE, unsigned long SCALE>
  std::ostream& operator<<(std::ostream& stream, const FixedPoint<TYPE,SCALE>& fp)
  {
    return (stream << static_cast<double>(fp.m_value) / SCALE);
  }
#endif
}

#endif /* ECPP_MATH_FIXEDPOINT_HPP_ */
