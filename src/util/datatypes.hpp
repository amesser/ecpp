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

namespace Platform {
  namespace Util {
    namespace Datatypes {
      template<unsigned int BYTES>
      class UnsignedIntType { public: typedef UnsignedIntType<BYTES + 1> Type; };

      template<> class UnsignedIntType<1> { public: typedef uint8_t  Type; };
      template<> class UnsignedIntType<2> { public: typedef uint16_t Type; };
      template<> class UnsignedIntType<4> { public: typedef uint32_t Type; };
      template<> class UnsignedIntType<8> { public: typedef uint16_t Type; };

      template<unsigned int BYTES>
      class IntType { public: typedef IntType<BYTES + 1> Type; };

      template<> class IntType<1> { public: typedef int8_t  Type; };
      template<> class IntType<2> { public: typedef int16_t Type; };
      template<> class IntType<4> { public: typedef int32_t Type; };
      template<> class IntType<8> { public: typedef int16_t Type; };

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
    }
  }
}



#endif /* DATATYPES_HPP_ */
