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
    static void
    formatDecimal(char *buffer, uint8_t digits, uint16_t value, char fill =' ', int8_t dot = -1)
    {
      char digit = fill;

      uint8_t    i = 5;

      while (digits > 5)
      {
        *(buffer++) = fill;
        digits--;
      }

      while(i > 1)
      {
        uint16_t   sub = 10000;

        switch(i)
        {
        case 4: sub = 1000; break;
        case 3: sub = 100; break;
        case 2: sub = 10; break;
        }

        if (value >= sub)
        {
          fill = '0';
          digit = '0';

          if(digits < i)
          {
            value = '#' - '0';
            break;
          }
          else
          {
            while(value >= sub)
            {
              value -= sub;
              digit ++;
            }
          }

        }
        else
        {
          digit = fill;
        }

        while(digits >= i)
        {
          if (dot == 0)
          {
            *(buffer++) = '.';
          }

          *(buffer++) = digit;

          digits--;
          dot--;

        }

        i -= 1;
      }

      while ((digits--) > 0)
      {
        *(buffer++) = value + '0';
      }
    }
  };

};




#endif /* ECPP_STRING_HPP_ */
