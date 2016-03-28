/*
 *  Copyright 2015 Andreas Messer <andi@bastelmap.de>
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
#ifndef ECPP_TARGET_AVR8_MEMORY_HPP_
#define ECPP_TARGET_AVR8_MEMORY_HPP_


#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#include "ecpp/VaTemplate.hpp"
#include "ecpp/Datatypes.hpp"
#include "ecpp/Array.hpp"

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
  struct MemoryHelper<0>
  {
  public:
    void readEEPROM(const void* p, uint16_t Size)
    {
      eeprom_read_block(this, p, Size);
    }

    void writeEEPROM(void* p, uint16_t Size) const
    {
      eeprom_update_block(this, p, Size);
    }

    void readFlash(const void* p, uint16_t Size)
    {
      memcpy_P(this, p, Size);
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

    void readFlash(const void *p)
    {
      value = pgm_read_dword(reinterpret_cast<const uint32_t*>(p));
    }
  };

  template<typename T, int ELEMENTS>
  class EEVariable
  {
  private:
    T   m_Value[ELEMENTS];
  public:
    template<typename ...I>
    constexpr EEVariable(const I&... init) : m_Value{ static_cast<T>(init)...} {};

    T operator [] (const int index) const
    {
      MemoryHelper<sizeof(T)> b;
      b.readEEPROM(&m_Value[index]);
      return *reinterpret_cast<T*>(&b);
    }
  };

  template<typename T>
  class EEVariable<T,1>
  {
  private:
    T   _Value;
  public:
    constexpr EEVariable() {};
    constexpr EEVariable(const T init) : _Value(init) {};

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
  class ConstFlashIterator
  {
  private:
    T const * m_Ptr;
  public:
    constexpr ConstFlashIterator(const T *Ptr) : m_Ptr(Ptr) {}
    constexpr bool operator < (ConstFlashIterator & rhs) const { return m_Ptr < rhs.m_Ptr;}

    int operator - (const ConstFlashIterator & rhs) const {return m_Ptr - rhs.m_Ptr;}

    ConstFlashIterator operator ++ (int) {return {m_Ptr++};}

    T operator * () const {
      MemoryHelper<sizeof(T)> b;
      b.readFlash(m_Ptr);
      return *reinterpret_cast<T*>(&b);
    }
  };


  template<typename T, int ELEMENTS>
  class FlashVariable
  {
  private:
    T m_Value[ELEMENTS];

    template<typename INIT, int INITELEMENTS, ::ecpp::indices_type ...Is>
    constexpr FlashVariable(const INIT (&init)[INITELEMENTS], ::ecpp::indices<Is...>) : m_Value{init[Is]...} {};

    template<typename INIT, ::ecpp::indices_type ...Is>
    constexpr FlashVariable(const RamBuffer<ELEMENTS, INIT> &Init, ::ecpp::indices<Is...>) : m_Value{static_cast<T>(Init[Is])...} {};

  public:
    template<typename INIT>
    constexpr FlashVariable(const RamBuffer<ELEMENTS, INIT> &Init) : FlashVariable(Init, ::ecpp::build_indices<ELEMENTS>()) {};

    template<typename ...I>
    constexpr FlashVariable(const T& a, const T& b, const I&... rem) : m_Value{a, b, static_cast<T>(rem)...} {};

    template<typename INIT, int INITELEMENTS>
    constexpr FlashVariable(const INIT (&init)[INITELEMENTS]) : FlashVariable(init, ::ecpp::build_indices<min(INITELEMENTS, ELEMENTS)>()) {};

    template<::ecpp::indices_type ...Is>
    constexpr FlashVariable(const T *init, ::ecpp::indices<Is...>) : m_Value{init[Is]...} {};

    constexpr FlashVariable(const T *init) : FlashVariable(init, ::ecpp::build_indices<ELEMENTS>()) {};

    T operator [] (int index) const
    {
      MemoryHelper<sizeof(T)> b;
      b.readFlash(&(m_Value[index]));
      return *reinterpret_cast<T*>(&b);
    }

    template<typename B>
    void read(B &buffer) const
    {
      MemoryHelper<min(sizeof(FlashVariable), sizeof(B))> *b = reinterpret_cast<MemoryHelper<min(sizeof(FlashVariable), sizeof(B))>*>(&buffer);
      b->readFlash(&m_Value);
    }

    template<typename B, typename SIZETYPE>
    void read(B * Buffer, SIZETYPE Size) const
    {
      MemoryHelper<0> *b = reinterpret_cast<MemoryHelper<0>*>(Buffer);
      b->readFlash(&m_Value, min(sizeof(FlashVariable), Size));
    }

    ConstFlashIterator<T>
    begin() const
    {
      return &m_Value[0];
    }

    ConstFlashIterator<T>
    end() const
    {
      return &m_Value[ELEMENTS];
    }
  };

  template<typename T>
  class FlashVariable<T,1>
  {
  private:
    T   m_Value;
  public:
    constexpr FlashVariable() : m_Value(0) {};

    constexpr FlashVariable(const T& init) : m_Value(init) {};

    inline operator T() const __attribute__((always_inline))
    {
      union {
        T value;
        MemoryHelper<sizeof(T)> b;
      };

      b.readFlash(&m_Value);
      return value;
    }

    template<typename B>
    void read(B &buffer) const
    {
      MemoryHelper<min(sizeof(FlashVariable), sizeof(B))> *b = reinterpret_cast<MemoryHelper<min(sizeof(FlashVariable), sizeof(B))>*>(&buffer);
      b->readFlash(&m_Value);
    }

    bool operator == (const T & rhs) const
    {
      return 0 == memcmp_P(&rhs, this, sizeof(*this));
    }

    bool operator != (const T & rhs) const
    {
      return 0 != memcmp_P(&rhs, this, sizeof(*this));
    }

    ConstFlashIterator<T>
    begin() const
    {
      return &m_Value;
    }

    ConstFlashIterator<T>
    end() const
    {
      return &m_Value + 1;
    }

  };


}


#endif /* ECPP_TARGET_AVR8_MEMORY_HPP_ */
