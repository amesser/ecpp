/*
 *  Copyright 2013 Andreas Messer <andi@bastelmap.de>
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
#ifndef DATATYPES_HPP_
#define DATATYPES_HPP_

#include <stdint.h>

namespace std {
  typedef __SIZE_TYPE__ size_t;
}

namespace Platform {
  namespace Util {
    namespace Datatypes {

      template<unsigned int BYTES>
      class UnsignedIntType { public: typedef UnsignedIntType<BYTES + 1> Type; };

      template<> class UnsignedIntType<1> { public: typedef uint8_t  Type; };
      template<> class UnsignedIntType<2> { public: typedef uint16_t Type; };
      template<> class UnsignedIntType<4> { public: typedef uint32_t Type; };
      template<> class UnsignedIntType<8> { public: typedef uint64_t Type; };

      template<unsigned int BYTES>
      class IntType { public: typedef IntType<BYTES + 1> Type; };

      template<> class IntType<1> { public: typedef int8_t  Type; };
      template<> class IntType<2> { public: typedef int16_t Type; };
      template<> class IntType<4> { public: typedef int32_t Type; };
      template<> class IntType<8> { public: typedef int64_t Type; };

      template<typename TYPE>  class DoubleWidthType {};

      template<> class DoubleWidthType<uint8_t>  { public: typedef uint16_t t_DoubleWidth;};
      template<> class DoubleWidthType<uint16_t> { public: typedef uint32_t t_DoubleWidth;};
      template<> class DoubleWidthType<uint32_t> { public: typedef uint64_t t_DoubleWidth;};
      template<> class DoubleWidthType<int8_t>   { public: typedef int16_t  t_DoubleWidth;};
      template<> class DoubleWidthType<int16_t>  { public: typedef int32_t  t_DoubleWidth;};
      template<> class DoubleWidthType<int32_t>  { public: typedef int64_t  t_DoubleWidth;};

      template<unsigned long VALUE>
      class BytesUnsignedInt { public: static constexpr uint8_t Bytes = 1 + BytesUnsignedInt<VALUE/256>::Bytes;};
      template<>
      class BytesUnsignedInt<0> { public: static constexpr uint8_t Bytes = 0;};

      template<long VALUE>
      class BytesSignedInt    { public: static constexpr uint8_t Bytes = 1 + BytesUnsignedInt<VALUE/256>::Bytes;};
      template<>
      class BytesSignedInt<0> { public: static constexpr uint8_t Bytes = 0;};


      template<unsigned long VALUE>
      class UnsignedIntTypeEstimator {
      public:
        typedef typename UnsignedIntType<BytesUnsignedInt<VALUE>::Bytes>::Type Type;
        static constexpr Type Value = VALUE;
      };

      template<long VALUE>
      class IntTypeEstimator {
      public:
        typedef typename IntType<BytesSignedInt<VALUE>::Bytes>::Type Type;
        static constexpr Type Value = VALUE;
      };

      template<typename TYPE>
      constexpr auto SizeOf(const TYPE& type) -> typename UnsignedIntTypeEstimator<sizeof(TYPE)>::Type {
        return UnsignedIntTypeEstimator<sizeof(TYPE)>::Value;
      };

      template<typename TYPE, unsigned long COUNT>
      constexpr auto ElementCount(const TYPE (& type)[COUNT]) -> typename UnsignedIntTypeEstimator<COUNT>::Type {
        return UnsignedIntTypeEstimator<COUNT>::Value;
      };


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

        template<typename INIT>
        constexpr FixedPoint(const INIT &value, const unsigned long & scale = 1) :
          m_value( rescale(value,scale)) {};

        template<typename CAST>
        operator CAST () const {return static_cast<CAST>(m_value) / SCALE;}

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

        constexpr FixedPoint operator >> (size_t shiftw) const
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

      template<typename FRIEND_TYPE, unsigned long FRIEND_SCALE>
      friend class FixedPoint;

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

         constexpr Complex operator >> (size_t shiftw) const
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
  }
}



#endif /* DATATYPES_HPP_ */
