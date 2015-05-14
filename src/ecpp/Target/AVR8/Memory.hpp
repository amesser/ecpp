/*
 * eeprom.hpp
 *
 *  Created on: 11.04.2015
 *      Author: andi
 */

#ifndef ECPP_TARGET_AVR8_MEMORY_HPP_
#define ECPP_TARGET_AVR8_MEMORY_HPP_


#include <avr/eeprom.h>
#include <stdint.h>

namespace ecpp
{
  template<int SIZE>
  struct MemoryHelper
  {
  private:
    uint8_t bytes[SIZE];
  public:
    void readEEPROM(const void* p)
    {
      eeprom_read_block(this, p, SIZE);
    }
  };

  template<>
  struct MemoryHelper<1>
  {
  private:
    union {
      uint8_t bytes[1];
      uint8_t value;
    };
  public:
    void readEEPROM(const void* p)
    {
      value = eeprom_read_byte(reinterpret_cast<const uint8_t*>(p));
    }
  };

  template<>
  struct MemoryHelper<2>
  {
  private:
    union {
      uint8_t  bytes[2];
      uint16_t value;
    };
  public:
    static uint16_t getEEPROM(const void* p);

    void readEEPROM(const void* p)
    {
#if 1
      value = getEEPROM(p);
#else
      bytes[0] = eeprom_read_byte(reinterpret_cast<const uint8_t*>(p));
      bytes[1] = eeprom_read_byte(reinterpret_cast<const uint8_t*>(p) + 1);
#endif
    }
  };

  template<>
  struct MemoryHelper<4>
  {
  private:
    union {
      uint8_t  bytes[4];
      uint32_t value;
    };
  public:
    void readEEPROM(const void* p)
    {
      value = eeprom_read_dword(reinterpret_cast<const uint32_t*>(p));
    }
  };

  template<typename T>
  class EEVariable
  {
  private:
    T   _Value;
  public:
    constexpr EEVariable(const T& init) : _Value(init) {};

    operator T() const
    {
      MemoryHelper<sizeof(T)> b;
      b.readEEPROM(&_Value);
      return *reinterpret_cast<T*>(&b);
    }
  };

}


#endif /* ECPP_TARGET_AVR8_MEMORY_HPP_ */
