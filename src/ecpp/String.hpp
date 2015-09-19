/*
 * String.hpp
 *
 *  Created on: 05.06.2015
 *      Author: andi
 */

#ifndef ECPP_STRING_HPP_
#define ECPP_STRING_HPP_

#include <ecpp/Datatypes.hpp>

namespace ecpp
{
  class String
  {
  public:
    static bool convertToDecimal(char *decimal, uint8_t length, uint16_t value);


    static void
    formatUnsigned(char *buffer, uint8_t digits, uint16_t value, char fill =' ')
    {
     bool overflow;

     overflow = convertToDecimal(buffer, digits, value);

     uint_fast8_t i;
     for(i = 0; i < digits; ++i)
     {
       if (overflow)
       {
         buffer[i] = 'X';
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

    static void
    formatSigned(char *buffer, uint8_t digits, int16_t value, char fill =' ')
    {
      bool overflow;
      char sign;

      if(value >= 0)
      {
        overflow = convertToDecimal(buffer, digits, value);
        sign = fill;
      }
      else
      {
        overflow = convertToDecimal(buffer, digits, -value);
        sign = '-';
        overflow = overflow | (buffer[0] != '0');
      }

      uint_fast8_t i;
      for(i = 0; i < digits; ++i)
      {
        if (overflow)
        {
          buffer[i] = 'X';
        }
        else if (buffer[i] != '0')
        {
          break;
        }
        else
        {
          buffer[i]   = sign;
          buffer[i-1] = fill;
        }
      }
    }
  };

  bool
  String::convertToDecimal(char *decimal, uint8_t length, uint16_t value)
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
};




#endif /* ECPP_STRING_HPP_ */
