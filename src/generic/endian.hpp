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
#ifndef ENDIAN_HPP_
#define ENDIAN_HPP_

#include "generic/buffer.hpp"
#include "util/vamath.hpp"

namespace Platform {
  namespace Endian {
    using ::Platform::Buffer::RamBuffer;
    using ::Platform::Buffer::join;
    using ::Platform::Util::VaMath::sum;

    template<typename t>
    class EndianEncoderBase
    {
    public:
      static constexpr size_t size = sizeof(t);
      typedef RamBuffer<size, uint8_t> BufferType;
    };

    template<typename ...Args>
    class LittleEndianEncoder
    {
    public:
      static constexpr size_t size = sum(LittleEndianEncoder<Args>::size...);
      typedef RamBuffer<size, uint8_t> BufferType;

      static constexpr BufferType encode(Args const & ...values) { return join(LittleEndianEncoder<Args>::encode(values)...); }
    };

    template<>
    class LittleEndianEncoder<uint8_t> : public EndianEncoderBase<uint8_t>
    {
    public:
      static constexpr BufferType encode(uint8_t const & value) { return {{value}}; }
    };

    template<>
    class LittleEndianEncoder<uint16_t> : public EndianEncoderBase<uint16_t>
    {
    public:
      static constexpr BufferType encode(uint16_t const & value) { return {{ static_cast<uint8_t>(value & 0xFF), static_cast<uint8_t>((value >> 8) & 0xFF)}}; }
    };

    template<typename ...Args>
    struct LittleEndian : public LittleEndianEncoder<Args...>::BufferType
    {
      constexpr LittleEndian(const Args & ...args) : LittleEndianEncoder<Args...>::BufferType(LittleEndianEncoder<Args...>::encode(args...)) {}
    };

  };
};


#endif /* ENDIAN_HPP_ */
