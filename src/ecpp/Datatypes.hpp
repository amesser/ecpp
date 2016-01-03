/*
 *  Copyright 2015 Andreas Messer <andi@bastelmap.de>
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
#ifndef ECPP_DATATYPES_HPP_
#define ECPP_DATATYPES_HPP_

#include <stdint.h>

namespace ecpp {

  template<typename T>
  class TypeProperties {};

  template<>
  class TypeProperties<uint8_t>
  {
    public:
      static constexpr uint8_t MaxSigned   = 0x7F;
      static constexpr uint8_t MaxUnsigned = 0xFF;
  };

  template<>
  class TypeProperties<int8_t> : public TypeProperties<uint8_t> {};

  template<>
  class TypeProperties<uint16_t>
  {
    public:
      static constexpr uint16_t MaxSigned   = 0x7FFF;
      static constexpr uint16_t MaxUnsigned = 0xFFFF;
  };

  template<>
  class TypeProperties<uint32_t>
  {
    public:
      static constexpr uint32_t MaxSigned   = 0x7FFFFFFF;
      static constexpr uint32_t MaxUnsigned = 0xFFFFFFFF;
  };

  template<unsigned int BYTES>
  class UnsignedIntType
  {
  public:
    typedef typename UnsignedIntType<BYTES + 1>::Type      Type;
    typedef typename UnsignedIntType<BYTES + 1>::FastType  FastType;
    typedef typename UnsignedIntType<BYTES + 1>::LeastType LeastType;
  };

  template<> class UnsignedIntType<1>
  {
  public:
    typedef uint8_t       Type;
    typedef uint_fast8_t  FastType;
    typedef uint_least8_t LeastType;
  };

  template<> class UnsignedIntType<2>
  {
  public:
    typedef uint16_t       Type;
    typedef uint_fast16_t  FastType;
    typedef uint_least16_t LeastType;
  };

  template<> class UnsignedIntType<4>
  {
  public:
    typedef uint32_t       Type;
    typedef uint_fast32_t  FastType;
    typedef uint_least32_t LeastType;
  };

  template<> class UnsignedIntType<8>
  {
  public:
    typedef uint64_t       Type;
    typedef uint_fast64_t  FastType;
    typedef uint_least64_t LeastType;
  };

  template<unsigned int BYTES>
  class IntType
  {
  public:
    typedef typename IntType<BYTES + 1>::Type      Type;
    typedef typename IntType<BYTES + 1>::FastType  FastType;
    typedef typename IntType<BYTES + 1>::LeastType LeastType;
  };

  template<> class IntType<1>
  {
  public:
    typedef int8_t       Type;
    typedef int_fast8_t  FastType;
    typedef int_least8_t LeastType;
  };

  template<> class IntType<2>
  {
  public:
    typedef int16_t       Type;
    typedef int_fast16_t  FastType;
    typedef int_least16_t LeastType;
  };

  template<> class IntType<4>
  {
  public:
    typedef int32_t       Type;
    typedef int_fast32_t  FastType;
    typedef int_least32_t LeastType;
  };

  template<> class IntType<8>
  {
  public:
    typedef int64_t       Type;
    typedef int_fast64_t  FastType;
    typedef int_least64_t LeastType;
  };

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
  constexpr typename UnsignedIntTypeEstimator<sizeof(TYPE)>::Type SizeOf(const TYPE& type) {
    return UnsignedIntTypeEstimator<sizeof(TYPE)>::Value;
  };

  template<typename TYPE>
  constexpr void ElementCount(const TYPE (& type));

  template<typename TYPE, unsigned long COUNT>
  constexpr typename UnsignedIntTypeEstimator<COUNT>::Type ElementCount(const TYPE (&)[COUNT]) {
    return UnsignedIntTypeEstimator<COUNT>::Value;
  };

  template<typename T, int ELEMENTS = 1> class EEVariable;
  template<typename T, int ELEMENTS = 1> class FlashVariable;

  template<typename TYPE>
  constexpr TYPE max(const TYPE & a, const TYPE & b) {
    return (a > b) ? a : b;
  };

  template<typename T, int N>
  T max(const T (& Values)[N])
  {
    auto Idx = UnsignedIntTypeEstimator<N>::Value - 1;
    auto Max = Values[Idx];

    while(Idx > 0)
    {
      Max = max(Values[--Idx], Max);
    }

    return Max;
  };

  template<typename TYPE>
  constexpr TYPE min(const TYPE & a, const TYPE & b) {
    return (a < b) ? a : b;
  };

  template<typename T, int N>
  T min(const T (& Values)[N])
  {
    auto Idx = UnsignedIntTypeEstimator<N>::Value - 1;
    auto Min = Values[Idx];

    while(Idx > 0)
    {
      Min = min(Values[--Idx], Min);
    }

    return Min;
  };

  template<typename T, int N>
  typename UnsignedIntTypeEstimator<N * TypeProperties<T>::MaxUnsigned>::Type
  sum(const T (& Values)[N])
  {
    auto Idx = UnsignedIntTypeEstimator<N>::Value;
    typename UnsignedIntTypeEstimator<N * TypeProperties<T>::MaxUnsigned>::Type Sum = 0;

    while(Idx > 0)
    {
        Sum += Values[--Idx];
    }

    return Sum;
  }
};



#endif /* DATATYPES_HPP_ */
