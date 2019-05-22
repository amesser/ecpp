/*
 * String.hpp
 *
 *  Created on: 05.06.2015
 *      Author: andi
 */

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
