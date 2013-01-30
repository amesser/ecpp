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
#ifndef CHECKSUM_HPP_
#define CHECKSUM_HPP_

#include "util/datatypes.hpp"

namespace Platform {
  namespace Arithmetics {
    namespace Checksum {

      template<bool>class YesNo {};

      template<> class YesNo<false> {public: static constexpr int Value = 0;};
      template<> class YesNo<true>  {public: static constexpr int Value = 1;};

      template<unsigned long MASK>
      class HighestBit {
      private:
        typedef typename ::Platform::Util::Datatypes::UnsignedIntTypeEstimator<MASK>::Type MaskType;
      public:
        static constexpr int Bit       = HighestBit<(MASK >> 1)>::Bit + 1;
        static constexpr MaskType Mask = (static_cast<MaskType>(0x1) << Bit);
      };

      template<>
      class HighestBit<0> {
      public:
        static constexpr int Bit = -1;
      };

      template<unsigned long POLYNOM>
      class CRCGenerator {
      private:
        typedef typename ::Platform::Util::Datatypes::UnsignedIntTypeEstimator<(POLYNOM >> 1)>::Type AccumulatorType;
        AccumulatorType Accumulator;
      public:
        void reset() {
          Accumulator = 0;
        }

        void calcCRC(uint8_t byte) {
          for(uint8_t mask = 0x80; mask != 0; mask = mask >> 1) {
              if ( ((this->Accumulator & HighestBit<(POLYNOM>>1)>::Mask) != 0) !=
                   ((byte & mask) != 0))
                this->Accumulator = (this->Accumulator << 1) ^ static_cast<AccumulatorType>(POLYNOM);
              else
                this->Accumulator = (this->Accumulator << 1);
          }
        }

        template<typename SIZE>
        void calcCRC(const uint8_t* data, SIZE length) {
          for(SIZE cnt = 0; cnt < length; cnt++)
            calcCRC(data[cnt]);
        }

        const AccumulatorType getCRC() const {
          return this->Accumulator & (HighestBit<POLYNOM>::Mask - 1);
        }

      };
    }
  }
}



#endif /* CHECKSUM_HPP_ */
