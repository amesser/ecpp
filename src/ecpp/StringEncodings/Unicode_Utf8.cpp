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
#include "ecpp/StringEncodings/Unicode.hpp"
#include "ecpp/String.hpp"

using namespace ::ecpp;
using namespace ::ecpp::StringEncodings;

#if 0
template<>
template<>
void String<Unicode>::SpanIterator<char>::next()
#endif

void Utf8::advance(ecpp::String<Utf8>::SpanBase<const BufferElement> &span)
{
  size_t l;

  if(span.buffer_size_ < 1)
    return;

  if (*span.buffer_ > 0)
  {
    l = 1;
  }
  else if (*span.buffer_ < 0)
  {
    uint8_t t = static_cast<uint8_t>(*span.buffer_);

    l = 1;
    while(t & 0x40)
    {
      t <<= 1;
      l  += 1;
    }
  }
  else
  {
    l = span.buffer_size_;
  }

  if(span.buffer_size_ < l)
    l = span.buffer_size_;

  span.buffer_size_   -= l;
  span.buffer_        += l;
}


Unicode::Codepoint::Codepoint(const ecpp::String<Utf8>::ConstSpan &it)
  : val_(0)
{
  if(it.buffer_size_ > 0)
  {
    if (*(it.buffer_) >= 0)
    {
      val_ = *(it.buffer_);
    }
    else
    {
      uint8_t t = static_cast<uint8_t>(*(it.buffer_));

      if (0x80 == (t & 0xC0))
        val_ = *(it.buffer_) & 0x3F;
      else if ( (0xC0 == (t & 0xE0)) && it.buffer_size_ >= 2)
        val_ = (*(it.buffer_) & 0x1F) << 6 | (*((it.buffer_)+1) & 0x3F);
      else
        val_ = 0;
    }
  }
}

void
Utf8::assign(Codepoint cp, ecpp::String<Utf8>::Span& dest)
{
  if(dest.buffer_size_  > 0)
  {
    if (cp.val_ < 128)
    {
      *(dest.buffer_) = cp.val_;
    }
    else if (cp.val_ > 0x7FF)
    {
      *(dest.buffer_) = 0x1A;
    }
    else if ((cp.val_ <= 0x7FF) && (dest.buffer_size_ >= 2))
    {
      *(dest.buffer_ + 0) = 0xC0 | ((cp.val_ >> 6) & 0x1F);
      *(dest.buffer_ + 1) = 0x80 | ((cp.val_ >> 6) & 0x3F);
    }
    else
    {
      *(dest.buffer_) = 0;
    }
  }

}