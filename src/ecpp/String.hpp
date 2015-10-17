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
    formatSigned(char *buffer, uint8_t digits, int16_t value, char fill =' ')
    {
      uint_fast8_t i;
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

      for(i = 0; i < digits; ++i)
      {
        if (overflow)
        {
          buffer[i] = '#';
        }
        else if ((i+1) >= digits)
        { /* last digit in string */
          break;
        }
        else
        {
          if(buffer[i+1] != '0')
          { /* next digit is first non-zero digit */
            buffer[i] = sign;
            break;
          }
          else
          { /* next digit is zero */
            buffer[i] = fill;
          }
        }
      }
    }
  };


};




#endif /* ECPP_STRING_HPP_ */
