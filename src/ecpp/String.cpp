/*
 * String.cpp
 *
 *  Created on: 04.10.2015
 *      Author: andi
 */
#include "ecpp/String.hpp"

namespace ecpp
{

  static const char s_hex_lookup[] = "0123456789ABCDEF";

  bool
  String::convertToHex(char *hex, uint8_t length, uint16_t value)
  {
    uint_fast8_t i;

    for(i = length; i > 0; --i)
    {
      hex[i-1] = s_hex_lookup[value & 0xF];
      value = value / 16;
    }

    return value > 0;
  }

  /** convert a number into decimal format
   *
   * Implementation uses double dabble algorithm
   */
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
}


