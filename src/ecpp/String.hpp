/*
 *  Copyright 2015-2019 Andreas Messer <andi@bastelmap.de>
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
#ifndef ECPP_STRING_HPP_
#define ECPP_STRING_HPP_

#include <ecpp/Datatypes.hpp>
#include <cstring>

namespace ecpp
{
  class String
  {
  public:
    static bool convertToHex(char *hex, uint8_t length, uint16_t value);
    static bool convertToDecimal(char *decimal, uint8_t length, uint16_t value);

    static size_t getLTrimLen(const char* text, size_t len);

    template<size_t ARRSIZE>
    static size_t getLTrimLen(const char (& text)[ARRSIZE]) { return getLTrimLen(text, ARRSIZE);}

    static size_t getRTrimStringLen(const char* text, size_t len);

    template<size_t ARRSIZE>
    static size_t getRTrimStringLen(const char (& text)[ARRSIZE]) { return getRTrimStringLen(text, ARRSIZE);}


    static void
    formatUnsigned(char *buffer, uint8_t digits, uint16_t value, char fill =' ')
    {
     uint_fast8_t i;
     bool overflow;

     overflow = convertToDecimal(buffer, digits, value);

     if(overflow)
     {
       fill = '#';
     }

     for(i = 0; i < digits; ++i)
     {
       if (overflow)
       {
         buffer[i] = fill;
       }
       else if ((i + 1) >= digits)
       {
         break;
       }
       else if (buffer[i] != '0')
       {
         break;
       }
       else
       {
         buffer[i] = fill;
       }
     }
    }

    /** insert a decimal point into the given formated number */
    static void
    formatFrac(char *buffer, uint8_t digits, uint8_t prec)
    {
      uint8_t i;

      for (i = digits; (i > 1); --i)
      {
        if (i < (digits - prec - 1))
          break;

        if (buffer[i-1] == '-')
        {
          buffer[i-2] = '-';
          buffer[i-1] = '0';
        }
        else if (buffer[i-1] == ' ')
        {
          buffer[i-1] = '0';
        }
      }

      while(digits >= 2)
      {
        digits = digits - 1;

        if (prec > 0)
        {
          buffer[digits] = buffer[digits-1];
          prec--;
        }
        else
        {
          buffer[digits] = '.';
          break;
        }
      }
    }

    static void
    formatSigned(char *buffer, int_fast8_t digits, int16_t value, char fill =' ', char sign = '\x00')
    {
      int_fast8_t i;
      bool overflow;

      if(value >= 0)
      {
        overflow = convertToDecimal(buffer, digits, value);
        if (sign == 0)
        {
          sign = fill;
        }
      }
      else
      {
        overflow = convertToDecimal(buffer, digits, -value);
        sign = '-';
        overflow = overflow | (buffer[0] != '0');
      }

      if (overflow)
      {
        memset(buffer, '#', digits);
      }
      else if (buffer[0] == '0' && (digits > 1))
      {
        for(i = 1; i < (digits-1); ++i)
        {
          buffer[i-1] = fill;

          if(buffer[i] != '0')
            break;
        }
        buffer[i-1] = sign;
      }
    }
  };
};




#endif /* ECPP_STRING_HPP_ */
