/*
 * byteorder.hpp
 *
 *  Created on: 28.02.2015
 *      Author: andi
 */

#ifndef BYTEORDER_HPP_
#define BYTEORDER_HPP_

#include <stdint.h>

namespace ECPP
{
  constexpr uint8_t  hton8(uint8_t val)  {return (val);}
  constexpr uint16_t hton16(uint16_t val) {return (val >> 8 | val << 8);}
  constexpr uint32_t hton32(uint32_t val) {return (val >> 24| val << 24 | ((val >> 8) & 0xFF00) | ((val << 8) & 0xFF0000));}
}


#endif /* BYTEORDER_HPP_ */
