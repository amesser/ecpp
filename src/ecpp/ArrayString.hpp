/*
 *  Copyright 2020 Andreas Messer <andi@bastelmap.de>
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
#ifndef ECPP_ARRAYSTRING_HPP_
#define ECPP_ARRAYSTRING_HPP_

#include <array>

#include "ecpp/String.hpp"
#include "ecpp/StringEncodings/Unicode.hpp"

namespace ecpp
{
  template<typename _Encoding, std::size_t BufferSize>
  class ArrayString : public String<_Encoding>, protected ::std::array<typename _Encoding::BufferElement, BufferSize>
  {
  public:
    using Encoding = _Encoding;

    using array_type = ::std::array<typename Encoding::BufferElement, BufferSize>;

    using array_type::data;
    using array_type::front;
    using array_type::end;


    using  Span      = typename String<Encoding>::Span;
    using  ConstSpan = typename String<Encoding>::ConstSpan;

    void assignRawValue(const typename _Encoding::BufferElement* value, std::size_t value_size)
    {
      memcpy(data(), value, std::min(value_size, BufferSize));
    }

    using Decoder  = typename String<Encoding>::template Reader<StringEncodings::Unicode>;

    Decoder decode() const
    {
      return ConstSpan(array_type::data(), array_type::size()).template createReader<ecpp::StringEncodings::Unicode>();
    }

    template<typename OtherEncoding, std::size_t OtherBufferSize>
    constexpr bool operator ==( const ArrayString<OtherEncoding,OtherBufferSize> &rhs) const
    {
      return this->asStdArray() == rhs.asStdArray();
    }

    template<typename OtherEncoding, std::size_t OtherBufferSize>
    constexpr bool operator !=( const ArrayString<OtherEncoding,OtherBufferSize> &rhs) const
    {
      return this->asStdArray() != rhs.asStdArray();
    }

    template<typename RhsReader>
    ArrayString & operator = (RhsReader decoder)
    {
      auto encoder = Span(array_type::data(), array_type::size()).template createWriter<StringEncodings::Unicode>();
      auto out = encoder.begin();
      auto in = decoder.begin();

      while((in < decoder.end()) && (out < encoder.end()))
        in >> out;

      out << Encoding::Codepoint::kSTRING_END();

      return *this;
    }

  protected:
    const array_type & asStdArray() const {return *this; }
  };

}

#endif