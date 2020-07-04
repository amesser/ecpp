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
#include "ecpp/Text/Utf8.hpp"

using namespace ecpp::Text;

size_t
Utf8TextProcessor::CountCharacters(const char* string, size_t len)
{
  TextIterator it(string, len);
  size_t count;

  for(count = 0; 0 != *it; ++count, ++it);
  return count;
}

CodePoint Utf8TextProcessor::TextIterator::operator * () const
{
  uint32_t cp;
  uint8_t  t;

  if(string_len_ < 1)
    return 0;

  if (*string_ >= 0)
    return *string_;

  t = static_cast<uint8_t>(*string_);

  if (0x80 == (t & 0xC0))
    cp = *string_ & 0x3F;
  else if ( (0xC0 == (t & 0xE0)) && string_len_ >= 2)
    cp = (*string_ & 0x1F) << 6 | (*(string_+1) & 0x3F);
  else
    cp = 0;

  return cp;
}

void Utf8TextProcessor::TextIterator::next()
{
  size_t l;

  if(string_len_ < 1)
    return;

  if (*string_ > 0)
  {
    l = 1;
  }
  else if (*string_ < 0)
  {
    uint8_t t = static_cast<uint8_t>(*string_);

    l = 1;
    while(t & 0x40)
    {
      t <<= 1;
      l  += 1;
    }
  }
  else
  {
    l = 0;
  }

  if(string_len_ < l)
    l = string_len_;

  string_len_ -= l;
  string_     += l;
}
