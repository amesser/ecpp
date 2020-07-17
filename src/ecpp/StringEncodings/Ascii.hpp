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
#ifndef ECPP_STRINGENCODINGS_ASCII_H_
#define ECPP_STRINGENCODINGS_ASCII_H_

#include "ecpp/String.hpp"
#include "ecpp/StringEncoding.hpp"

namespace ecpp::StringEncodings
{
  class Ascii : public ::ecpp::StringEncoding
  {
  public:
    using BufferElement           = char;
    class Codepoint;

    static bool convertToHex(char *hex, uint8_t length, uint16_t value);
    static bool convertToDecimal(char *decimal, uint8_t length, uint16_t value);
  };

  class Ascii::Codepoint
  {
  public:
    constexpr Codepoint(char    val) : cp(val) {}

    constexpr Codepoint(const ecpp::String<Ascii>::SpanIterator<char> &it) :
      cp(it.buffer_size_ > 0 ? *(it.buffer_) : '\0') {}

    constexpr char getRaw() const { return cp;}

    static constexpr Codepoint kSTRING_END()     { return '\0'; }
    static constexpr Codepoint kMAPPING_FAILED() { return '~';}

  protected:
    char  cp;
  };


}

#endif
