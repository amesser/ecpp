/*
 *  Copyright 2014 Andreas Messer <andi@bastelmap.de>
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
#ifndef DFT_HPP_
#define DFT_HPP_

#include <util/datatypes.hpp>
#include <util/indices.hpp>
#include <generic/buffer.hpp>

namespace Platform {
  namespace Algorithm {
    namespace DFT {
      using ::Platform::Util::indices;
      using ::Platform::Util::build_indices;
      using ::Platform::Buffer::RamBuffer;

      template<unsigned long BITS>
      constexpr unsigned long
      reverseBits(unsigned long index);


      template<>
      constexpr unsigned long
      reverseBits<1>(unsigned long index)
      {
        return index;
      }

      template<>
      constexpr unsigned long
      reverseBits<2>(unsigned long index)
      {
        return (index == 1) ? 2 : (index == 2 ? 1 : index);
      }

      template<unsigned long BITS>
      constexpr unsigned long
      reverseBits(unsigned long index)
      {
        return reverseBits<BITS-1>(index >> 1) |
           ((index & 1) ?  (0x1 << (BITS-1)) : 0);
      }

      template<typename TYPE, unsigned long POWER>
      class Radix2DFT
      {
      public:
        static constexpr double        normalization() { return 1.0 / 2.0; }
        static constexpr unsigned long m_power = POWER;
        static constexpr unsigned long m_bins  = (0x1 << POWER);

        typedef Platform::Util::Datatypes::Complex<TYPE> t_Type;

        typedef RamBuffer<m_bins/2, t_Type>  FactorArrayType;
        typedef RamBuffer<m_bins/2, unsigned int> DescrambleArrayTypeHalf;
        typedef RamBuffer<m_bins,   unsigned int> DescrambleArrayTypeFull;

        /* class to generate the dft factors. This includes normalization!!! */
        template<typename MATH>
        class
        w {
        public:
          constexpr w() {}

          constexpr t_Type operator [] (unsigned long k) const
          {
            return {  MATH::cos(2 * MATH::m_pi * k / m_bins), -MATH::sin(2 * MATH::m_pi * k/ m_bins)};
          }

          template<size_t... Is>
          constexpr FactorArrayType asArray_real(indices<Is...>) const
          {
            return { { (*this)[Is]...,}};
          }

          constexpr FactorArrayType asArray() const {
            return this->asArray_real(build_indices<m_bins/2>());
          }
        };

        class
        descramble {
        public:
          constexpr descramble() {};

          constexpr unsigned int operator [] (unsigned int idx) const {
            return reverseBits<POWER>(idx);
          }

          template<size_t... Is>
          constexpr DescrambleArrayTypeHalf asArrayHalf_real(indices<Is...>) const
          {
            return { { (*this)[Is]...,}};
          }

          template<size_t... Is>
          constexpr DescrambleArrayTypeFull asArrayFull_real(indices<Is...>) const
          {
            return { { (*this)[Is]...,}};
          }

          constexpr DescrambleArrayTypeHalf asArrayHalf() const {
            return this->asArrayHalf_real(build_indices<m_bins/2>());
          }

          constexpr DescrambleArrayTypeFull asArrayFull() const {
            return this->asArrayFull_real(build_indices<m_bins>());
          }
        };

        template<typename FACTORS>
        static void decimation_in_f(t_Type (&data)[m_bins], const FACTORS &factors)
        {
          typedef unsigned int t_index;

          t_index delta = m_bins >> 1;
          t_Type  w(1,0);

          while(delta)
          {
            t_index offset = 0;


            do {
              auto a = data[offset] * TYPE(0.5);
              auto b = data[offset + delta] * TYPE(0.5);

              data[offset]       =  (a + b);
              data[offset+delta] =  (a - b) * w;

              t_index double_delta = 2*delta;
              offset += double_delta;

              if (offset >= m_bins)
              {
                offset = (1 + (offset - m_bins)) & (delta - 1);

                t_index factor_index = offset;

                while(double_delta < m_bins)
                {
                  double_delta *= 2;
                  factor_index *= 2; /*test */
                }

                w = factors[factor_index];
              }

            } while(offset);

            delta = delta >> 1;
          }
        }

      };
    };
  };
};
#endif /* DFT_HPP_ */
