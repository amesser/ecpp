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
#ifndef ECPP_COMPLEX_HPP_
#define ECPP_COMPLEX_HPP_

#include <stdint.h>

namespace ecpp {
  template<typename TYPE>
  class Complex;

#ifdef  _GLIBCXX_OSTREAM
  template<typename TYPE>
  std::ostream& operator << (std::ostream& stream, const Complex<TYPE> &cp);
#endif

  template<typename TYPE>
  class Complex
  {
  private:
     TYPE m_real;
     TYPE m_imag;
  public:
     constexpr Complex() {};

     constexpr Complex(const Complex & cp) :
       m_real(cp.m_real), m_imag(cp.m_imag) {};

     constexpr Complex(const TYPE & real, const TYPE & imag) :
       m_real(real), m_imag(imag) {};

     template<typename RHS>
     Complex & operator = (const RHS & rhs)
     {
       m_real = rhs;
       m_imag = 0;

       return *this;
     }

     template<typename RHS_TYPE>
     Complex & operator = (const Complex<RHS_TYPE> & rhs)
     {
       m_real = rhs.m_real;
       m_imag = rhs.m_imag;
       return *this;
     }

     template<typename RHS>
     constexpr Complex operator* (const RHS & rhs) const
     {
       return {m_real * rhs, m_imag * rhs};
     }

     template<typename RHS_TYPE>
     constexpr Complex operator* (const Complex<RHS_TYPE> & rhs) const
     {
       return {m_real * rhs.m_real - m_imag * rhs.m_imag,
               m_real * rhs.m_imag + m_imag * rhs.m_real};
     }

     template<typename RHS>
     constexpr Complex operator/ (const RHS & rhs) const
     {
       return {m_real / rhs, m_imag / rhs};
     }

     template<typename RHS_TYPE>
     constexpr Complex operator/ (const Complex<RHS_TYPE> & rhs) const
     {
       return {(m_real * rhs.m_real + m_imag * rhs.m_imag) /
               (rhs.m_real * rhs.m_real + rhs.m_imag * rhs.m_imag),
               (m_imag * rhs.m_real - m_real * rhs.m_imag) /
               (rhs.m_real * rhs.m_real + rhs.m_imag * rhs.m_imag)};
     }
     template<typename RHS>
     constexpr Complex operator+ (const RHS & rhs) const
     {
       return {m_real + rhs, m_imag};
     }

     template<typename RHS_TYPE>
     constexpr Complex operator + (const Complex<RHS_TYPE> & rhs) const
     {
       return {m_real + rhs.m_real, m_imag + rhs.m_imag};
     }

     template<typename RHS>
     constexpr Complex operator- (const RHS & rhs) const
     {
       return {m_real - rhs, m_imag};
     }

     template<typename RHS_TYPE>
     constexpr Complex operator - (const Complex<RHS_TYPE> & rhs) const
     {
       return {m_real - rhs.m_real, m_imag - rhs.m_imag};
     }

     constexpr Complex conjugate () const
     {
       return {m_real , - m_imag};
     }

     constexpr Complex operator >> (uint_fast8_t shiftw) const
     {
       return {m_real >> shiftw, m_imag >> shiftw};
     }

     constexpr TYPE real() const { return m_real;}
     constexpr TYPE imag() const { return m_imag;}

#ifdef  _GLIBCXX_OSTREAM
     friend std::ostream& operator << <TYPE>(std::ostream& stream, const Complex &cp);
#endif
  };

#ifdef  _GLIBCXX_OSTREAM
  template<typename TYPE>
  std::ostream& operator<<(std::ostream& stream, const Complex<TYPE>& cp)
  {
    if (cp.m_imag >= 0)
      return (stream << cp.m_real << "+i" << cp.m_imag);
    else
      return (stream << cp.m_real << "-i" << (-cp.m_imag));
  }
#endif
}

#endif /* ECPP_COMPLEX_HPP_ */
