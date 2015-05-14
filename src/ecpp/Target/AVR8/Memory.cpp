/*
 * eeprom.hpp
 *
 *  Created on: 11.04.2015
 *      Author: andi
 */

#include "Memory.hpp"

namespace ecpp
{
  /* template<> */
  uint16_t MemoryHelper<2>::getEEPROM(const void *p)
  {
    union {
      uint8_t  bytes[2];
      uint16_t value;
    };

    eeprom_busy_wait();

    EEAR  = reinterpret_cast<uint16_t>(p);
    EECR |= 1 << EERE;
    bytes[0] = EEDR;

    EEAR  = reinterpret_cast<uint16_t>(p) + 1;
    EECR |= 1 << EERE;
    bytes[1] = EEDR;

    return value;
  }
}
