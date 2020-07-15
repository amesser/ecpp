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
#include "ecpp/StringEncodings/Ascii.hpp"

using namespace ecpp::StringEncodings;

const char Ascii::kHEX_LOOKUP_TABLE[16] = "0123456789ABCDEF";

bool
Ascii::convertToHex(char *hex, uint8_t length, uint16_t value)
{
  uint_fast8_t i;

  for(i = length; i > 0; --i)
  {
    hex[i-1] = kHEX_LOOKUP_TABLE[value & 0xF];
    value = value / 16;
  }

  return value > 0;
}

/** convert a number into decimal format
 *
 * Implementation uses double dabble algorithm
 */
bool
Ascii::convertToDecimal(char *decimal, uint8_t length, uint16_t value)
{
  uint_fast8_t overflow;
  uint_fast8_t j;

  for(j = 0; j < length; ++j)
  {
    decimal[j] = '0';
  }

  overflow = 0;
  for(j = 0; j < 16; ++j)
  {
    uint_fast8_t i;
    uint8_t      mask;

    mask = (value & 0x8000) ? 0x01 : 0;
    value = value << 1;

    i = length;
    while(i--)
    {
      uint_fast8_t bcd;

      bcd = static_cast<uint8_t>(decimal[i]) - '0';

      if(bcd >= 5)
      {
        bcd += 3;
      }

      bcd   = (bcd << 1) | mask;

      mask  = (bcd & 0x10) >> 4;
      bcd   = bcd & 0x0F;

      decimal[i] = bcd + '0';
    }

    overflow = overflow | mask;
  }

  return overflow != 0;
}
