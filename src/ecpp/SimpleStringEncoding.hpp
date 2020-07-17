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
#ifndef ECPP_SIMPLESTRINGENCODING_H_
#define ECPP_SIMPLESTRINGENCODING_H_

#include "ecpp/StringEncoding.hpp"
#include "ecpp/StringEncodings/Unicode.hpp"

namespace ecpp
{
  template<typename _BufferElement>
  class SimpleStringEncoding : public ecpp::StringEncoding
  {
  protected:
    template<typename DisplayEncoding>
    class SimpleCodepoint : public ecpp::StringEncoding::Codepoint
    {
    public:
      using BufferElement = typename DisplayEncoding::BufferElement;

      constexpr SimpleCodepoint(BufferElement val) : val_(val) {}

      constexpr SimpleCodepoint(const typename ecpp::String<DisplayEncoding>::template ReadIterator<DisplayEncoding> &it)
        : val_(it.buffer_size_ > 0 ? *it.buffer_ : 0) {}

#if 0
      constexpr SimpleCodepoint(typename ecpp::String<DisplayEncoding>::template SpanBase<const BufferElement> &span)
        : val_(span.buffer_size_ > 0 ? *span.buffer_ : 0) {}
#endif

      SimpleCodepoint(ecpp::StringEncodings::Unicode::Codepoint cp) : val_(DisplayEncoding::createFromUnicode(cp.getRaw()))  {}

      constexpr bool operator ==(const SimpleCodepoint &rhs) const
      {
        return val_ == rhs.val_;
      }

      constexpr bool operator !=(const SimpleCodepoint &rhs) const
      {
        return val_ != rhs.val_;
      }

      static SimpleCodepoint createFromUnicode(char32_t uc)   { return DisplayEncoding::createFromUnicode(uc); };
      static constexpr SimpleCodepoint kSTRING_END()          { return 0x00; }
      static constexpr SimpleCodepoint kMAPPING_FAILED()      { return DisplayEncoding::kMAPPING_FAILED_CHAR; }
    protected:
      BufferElement val_;

      friend DisplayEncoding;
    };
  public:
    using BufferElement = _BufferElement;

    static void advance(typename ecpp::String<SimpleStringEncoding>::template SpanBase<const BufferElement> &span);
  };

  template<typename _BufferElement>
  void SimpleStringEncoding<_BufferElement>::advance(typename ecpp::String<SimpleStringEncoding>::template SpanBase<const BufferElement> &span)
  {
    if (span.buffer_size_ > 0)
    {
      if((*span.buffer_) != 0)
      {
        span.buffer_++;
        span.buffer_size_--;
      }
      else
      {
        span.buffer_ += span.buffer_size_;
        span.buffer_size_ = 0;
      }
    }
  }
}

#endif
