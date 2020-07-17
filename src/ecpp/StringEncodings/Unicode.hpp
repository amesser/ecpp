/*
 *  Copyright 2015-2020 Andreas Messer <andi@bastelmap.de>
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
#ifndef ECPP_STRINGENCODINGS_UNICODE_H_
#define ECPP_STRINGENCODINGS_UNICODE_H_

#include "ecpp/String.hpp"
#include "ecpp/StringEncoding.hpp"

namespace ecpp::StringEncodings
{
  class Unicode : public ::ecpp::StringEncoding
  {
  public:
    class Codepoint;
  };

  class Utf8 : public Unicode
  {
  public:
    using Unicode::Codepoint;
    using BufferElement           = char;

    static void assign(Codepoint cp, ecpp::String<Utf8>::Span& dest);
    static void advance(ecpp::String<Utf8>::SpanBase<const BufferElement> &span);
  };

  class Unicode::Codepoint : public ecpp::StringEncoding::Codepoint
  {
  public:
    constexpr Codepoint(char32_t    val) : val_(val) {}

    bool isVisible() const;
    Codepoint(const ecpp::String<Utf8>::ConstSpan &it);

    constexpr char32_t getRaw() const { return val_;}

    constexpr bool operator ==(const Codepoint &rhs) { return rhs.val_ == val_; }
    constexpr bool operator !=(const Codepoint &rhs) { return rhs.val_ != val_; }

    static constexpr Codepoint kSTRING_END()     { return 0; }
    static constexpr Codepoint kMAPPING_FAILED() { return U'¿';}

  protected:
    char32_t  val_;

    friend Utf8;
  };


}

#endif
