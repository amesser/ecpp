/*
 * eeprom.hpp
 *
 *  Created on: 11.04.2015
 *      Author: andi
 */

#ifndef ECPP_TARGET_AVR8_MEMORY_HPP_
#define ECPP_TARGET_AVR8_MEMORY_HPP_


#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdint.h>

namespace ecpp
{
  template<int SIZE>
  struct MemoryHelper
  {
    uint8_t bytes[SIZE];
  public:
    void readEEPROM(const void* p)
    {
      eeprom_read_block(this, p, SIZE);
    }

    void writeEEPROM(void* p) const
    {
      eeprom_update_block(this, p, SIZE);
    }

    void readFlash(const void* p)
    {
      memcpy_P(this, p, SIZE);
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

    void readFlash(const void *p)
    {
      value = pgm_read_byte(reinterpret_cast<const uint8_t*>(p));
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

    void readFlash(const void *p)
    {
      value = pgm_read_word(reinterpret_cast<const uint16_t*>(p));
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
    constexpr EEVariable() {};
    constexpr EEVariable(const T& init) : _Value(init) {};

    void read(T &buffer) const
    {
      MemoryHelper<sizeof(T)> *b = reinterpret_cast<MemoryHelper<sizeof(T)>*>(&buffer);
      b->readEEPROM(&_Value);
    }

    operator T() const
    {
      MemoryHelper<sizeof(T)> b;
      b.readEEPROM(&_Value);
      return *reinterpret_cast<T*>(&b);
    }

    const T & operator = (const T & rhs)
    {
      const MemoryHelper<sizeof(T)> *b = reinterpret_cast<const MemoryHelper<sizeof(T)>*>(&rhs);
      b->writeEEPROM(&_Value);
      return rhs;
    }
  };

  template<typename T>
  class FlashVariable
  {
  private:
    T   m_Value;
  public:
    constexpr FlashVariable(const T& init) : m_Value(init) {};

    operator T() const
    {
      MemoryHelper<sizeof(T)> b;
      b.readFlash(&m_Value);
      return *reinterpret_cast<T*>(&b);
    }
  };

}


#endif /* ECPP_TARGET_AVR8_MEMORY_HPP_ */
