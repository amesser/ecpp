/*
 *  Copyright 2013 Andreas Messer <andi@bastelmap.de>
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

#ifndef AVR8_HPP_
#define AVR8_HPP_

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include <util/datatypes.hpp>
#include <generic/buffer.hpp>

namespace Platform {
  namespace Architecture {
    namespace AVR8 {
      class CriticalSectionGuard {
      public:
        CriticalSectionGuard()  {cli();}
        ~CriticalSectionGuard() {sei();}
      };

      using namespace ::Platform::Buffer;

      template<size_t Count, typename Type>
      class FlashBufferBase;


      template<size_t Count, typename Type>
      class FlashBufferBase : public BufferBase<Count, Type>
      {
      public:
        typedef ConstBufferIterator<Type, FlashBufferBase> const_iterator;
        typedef FlashBufferBase<Count, Type> BaseType;

        constexpr FlashBufferBase(const Type (& init)[Count]) : BufferBase<Count,Type>(init) {}
      };

      template<size_t Count, typename Type>
      class EEPROMBufferBase : public BufferBase<Count, Type>
      {
      public:

        constexpr EEPROMBufferBase(const Type (& init)[Count]) : BufferBase<Count,Type>(init) {}
      };

      template<size_t Count>
      class EEPROMBufferBase<Count,uint8_t> : public BufferBase<Count,uint8_t>
      {
      public:
        typedef ConstBufferIterator<uint8_t, EEPROMBufferBase> const_iterator;
        typedef BufferIterator<uint8_t, EEPROMBufferBase> iterator;

        typedef EEPROMBufferBase<Count, uint8_t> BaseType;
        constexpr EEPROMBufferBase(const uint8_t (& init)[Count]) : BufferBase<Count,uint8_t>(init) {}
        uint8_t at(size_t index) const {
          EEAR = static_cast<uint8_t>(reinterpret_cast<uint16_t>(&(this->_data[index])));
          EECR |= _BV(EERE);
          return EEDR;
        }
      };

      template<size_t Count, typename Type = uint8_t>
      using FlashBuffer  = ::Platform::Buffer::Buffer<Count, Type, FlashBufferBase<Count,Type> >;

      template<size_t Count, typename Type = uint8_t>
      using EEPROMBuffer  = ::Platform::Buffer::Buffer<Count, Type, EEPROMBufferBase<Count,Type> >;

      template<typename Init, template <size_t, typename> class Storage>
      class ConstantArrayBuffer{};


      template<typename Init>
      class ConstantArrayBuffer<Init, FlashBuffer>
      {
      public:
        typedef typename Init::template OtherBuffer<FlashBuffer> Type;
        static const Type PROGMEM value ;
      };

      template<typename Init>
      const typename ConstantArrayBuffer<Init, FlashBuffer>::Type PROGMEM
        ConstantArrayBuffer<Init, FlashBuffer>::value = ConstantArrayBuffer<Init, FlashBuffer>::Type(Init());

      template<typename Init>
      class ConstantArrayBuffer<Init, EEPROMBuffer>
      {
      public:
        typedef typename Init::template OtherBuffer<EEPROMBuffer> Type;
        static const Type EEMEM value ;
      };

      template<typename Init>
      const typename ConstantArrayBuffer<Init, EEPROMBuffer>::Type EEMEM
        ConstantArrayBuffer<Init, EEPROMBuffer>::value = ConstantArrayBuffer<Init, EEPROMBuffer>::Type(Init());

      template<typename TYPE>
      constexpr TYPE max(const TYPE & a, const TYPE & b) {
        return (a > b) ? a : b;
      };

      template<typename TYPE>
      constexpr TYPE min(const TYPE & a, const TYPE & b) {
        return (a < b) ? a : b;
      };


    }
  }

  namespace Buffer {
    using namespace ::Platform::Architecture::AVR8;
    using ::Platform::Util::Datatypes::Complex;

    template<typename Type, template<size_t, typename> class Buffer>
    class ConstBufferIteratorValue;

    template<typename TYPE>
    class ConstBufferIterator<TYPE, FlashBufferBase>
    {
    private:
      const   TYPE           *_data;
    public:
      typedef ConstBufferIterator<TYPE, FlashBufferBase> self_type;

      constexpr ConstBufferIterator() : _data(0) {};
      constexpr ConstBufferIterator(const TYPE *data) : _data(data) {}

      TYPE operator * () const;

      constexpr bool operator == (const self_type &rhs) const {return this->_data == rhs._data;}
      constexpr bool operator != (const self_type &rhs) const {return this->_data != rhs._data;}
      constexpr bool operator >  (const self_type &rhs) const {return this->_data > rhs._data;}

      self_type operator + (size_t count) const {
        const TYPE *data = _data + count;

        if (data < _data)
          data = reinterpret_cast<TYPE*>(~0);

        return {data};
      }

      self_type & operator ++ () {_data++; return *this;}

      ConstBufferIteratorValue<TYPE, FlashBufferBase> operator ++ (int); // {return self_type(_data++);}
    };

    template<typename TYPE>
    class ConstBufferIterator<Complex<TYPE>, FlashBufferBase>
    {
    private:
      const   Complex<TYPE> *_data;
    public:
      typedef ConstBufferIterator<Complex<TYPE>, FlashBufferBase> self_type;

      constexpr ConstBufferIterator() : _data(0) {};
      constexpr ConstBufferIterator(const Complex<TYPE> *data) : _data(data) {}

      Complex<TYPE> operator * () const;

      constexpr bool operator == (const self_type &rhs) const {return this->_data == rhs._data;}
      constexpr bool operator != (const self_type &rhs) const {return this->_data != rhs._data;}
      constexpr bool operator >  (const self_type &rhs) const {return this->_data > rhs._data;}

      self_type operator + (size_t count) const {
        const Complex<TYPE> *data = _data + count;

        if (data < _data)
          data = reinterpret_cast<Complex<TYPE>*>(~0);

        return {data};
      }

      self_type & operator ++ () {_data++; return *this;}

      ConstBufferIteratorValue<Complex<TYPE>, FlashBufferBase> operator ++ (int);
    };

    template<typename Type>
    class ConstBufferIteratorValue<Type, FlashBufferBase> : public ConstBufferIterator<Type, FlashBufferBase>
    {
    private:
      const Type _val;
    public:
      constexpr ConstBufferIteratorValue(const Type *data, uint8_t val) :
      ConstBufferIterator<Type,FlashBufferBase>(data), _val(val) {}
      Type operator * () const { return _val; }
    };

    template<>
    ConstBufferIteratorValue<uint8_t, FlashBufferBase>
    ConstBufferIterator<uint8_t, FlashBufferBase>::operator ++ (int)
    {
      const uint8_t *bck = _data;
      uint16_t address = reinterpret_cast<uint16_t>(_data);
      uint8_t value;

      asm volatile (
          "lpm %0, %a1+"
          : "=r" (value), "=z" (address)
          : "1" (address)
      );

      _data = reinterpret_cast<uint8_t*>(address);

      return {bck, value};
    }

    template<typename TYPE>
    ConstBufferIteratorValue<TYPE, FlashBufferBase>
    ConstBufferIterator<TYPE, FlashBufferBase>::operator ++ (int)
    {
      const TYPE *bck = _data;
      uint16_t address = reinterpret_cast<uint16_t>(_data);
      uint8_t cnt = sizeof(TYPE);

      TYPE value;
      uint8_t *p = reinterpret_cast<uint8_t*>(&value);

      while(cnt--)
      {
        uint8_t byte;

        asm volatile (
            "lpm %0, %a1+"
            : "=r" (byte), "=z" (address)
            : "1" (address)
        );

        *(p++) = byte;
      }

      _data = reinterpret_cast<const TYPE*>(address);

      return {bck, value};
    }

    template<>
    uint8_t ConstBufferIterator<uint8_t, FlashBufferBase>::operator * () const
    {
      uint8_t  value;
      uint16_t address = reinterpret_cast<uint16_t>(_data);

      asm volatile (
          "lpm %0, %a1+"
          : "=r" (value), "=z" (address)
          : "1" (address)
      );
      return value;
    }

    template<>
    uint16_t ConstBufferIterator<uint16_t, FlashBufferBase>::operator * () const
    {
      const uint8_t *p = reinterpret_cast<const uint8_t*>(_data);
      ConstBufferIterator<uint8_t, FlashBufferBase> it(p);

      uint16_t value = *(it++);
      value |= (*it) << 8;

      return value;
    }


    template<typename TYPE>
    Complex<TYPE> ConstBufferIterator<Complex<TYPE>, FlashBufferBase>::operator * () const
    {
      const TYPE *p = reinterpret_cast<const TYPE*>(_data);
      ConstBufferIterator<TYPE, FlashBufferBase> it(p);

      return { *(it++), *(it)};
    }

    template<typename TYPE>
    TYPE ConstBufferIterator<TYPE, FlashBufferBase>::operator * () const
    {
      TYPE     value;

      uint8_t  cnt = sizeof(TYPE);

      uint16_t address = reinterpret_cast<uint16_t>(_data);
      uint8_t  *p      = reinterpret_cast<uint8_t*>(&value);

      while(cnt--)
      {
        uint8_t  byte;

        asm volatile (
            " lpm %0, %a1+"
            : "=r" (byte), "=z" (address)
            : "1" (address)
        );

        *(p++) = byte;
      }
      return value;
    }


    template<>
    class ConstBufferIterator<uint8_t, EEPROMBufferBase>
    {
    private:
      uint8_t           _addr;

    public:
      typedef ConstBufferIterator<uint8_t, EEPROMBufferBase> self_type;

      constexpr ConstBufferIterator() : _addr(0) {};
      constexpr ConstBufferIterator(const uint8_t addr)  : _addr(addr) {}
      constexpr ConstBufferIterator(const uint8_t *data) : _addr(reinterpret_cast<uint16_t>(data)) {}

      uint8_t operator * () const {
        EEAR  = _addr;
        EECR |= 1 << EERE;

        return EEDR;
      }

      constexpr bool operator == (const self_type &rhs) const {return this->_addr == rhs._addr;}
      constexpr bool operator != (const self_type &rhs) const {return this->_addr != rhs._addr;}
      constexpr bool operator >  (const self_type &rhs) const {return this->_addr > rhs._addr;}

      self_type operator + (size_t count) const { return {static_cast<uint8_t>(_addr + count)}; }

      self_type & operator ++ ()     {_addr++; return *this;}
      self_type   operator ++ (int)  {uint8_t bck = _addr; _addr++; return {bck};}
    };


  };
};


#endif /* AVR8_HPP_ */
