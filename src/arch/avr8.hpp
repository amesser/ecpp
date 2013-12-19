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


      class FlashBufferReader
      {
      private:
        uint16_t _address;
        uint16_t _size;

      public:
        constexpr FlashBufferReader() : _address(0), _size(0) {}

        template< size_t Size, typename Type>
        constexpr FlashBufferReader(const FlashBufferBase<Size, Type> & buf) :
          _address(reinterpret_cast<uint16_t>(&(buf.data()))),
          _size(Size) {}

        FlashBufferReader(const FlashBufferReader & rhs) :
          _address(rhs._address), _size(rhs._size) {}

        template< size_t Size, typename Type>
        void init(const FlashBufferBase<Size, Type> & buf){
          _address = reinterpret_cast<uint16_t>(&(buf.data()));
          _size = Size;
        }

        uint8_t at(size_t offset) const
        {
          uint16_t address = _address + offset;
          uint8_t value;

          asm volatile (
              "lpm %0, %a1+"
              : "=r" (value), "=z" (address)
              : "1" (address)
          );

          return value;
        }

        size_t size() const {return _size;}
      };

      template<size_t Count, typename Type>
      class FlashBufferBase : public BufferBase<Count, Type>
      {
      public:
        constexpr FlashBufferBase(const Type (& init)[Count]) : BufferBase<Count,Type>(init) {}
      };

      template<size_t Count>
      class FlashBufferBase<Count, uint8_t> : public BufferBase<Count, uint8_t>
      {
      public:
        typedef ConstBufferIterator<uint8_t, FlashBufferBase> const_iterator;
        typedef FlashBufferBase<Count, uint8_t> BaseType;

        constexpr FlashBufferBase(const uint8_t (& init)[Count]) : BufferBase<Count, uint8_t>(init) {}

        typedef FlashBufferReader ReaderType;
        uint8_t at(size_t index) const { return FlashBufferReader(*this).at(index); }
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


      template<typename TYPE>
      constexpr TYPE max(const TYPE & a, const TYPE & b) {
        return (a > b) ? a : b;
      };

      template<typename TYPE>
      constexpr TYPE min(const TYPE & a, const TYPE & b) {
        return (a < b) ? a : b;
      };

      class USI {
      public:
        enum UsiMode {
          SPI_MODE0 = 0,
          SPI_MODE1 = 1,
          SPI_MODE2 = 2,
          SPI_MODE3 = 3,
        };

        static void initialize(const enum UsiMode mode) {
          switch (mode) {
          case SPI_MODE0:
          case SPI_MODE3:
            USICR = _BV(USIWM0) | _BV(USICS1) | _BV(USICLK);
            break;
          case SPI_MODE1:
          case SPI_MODE2:
            USICR = _BV(USIWM0) | _BV(USICS1) | _BV(USICLK) | _BV(USICS0);
            break;
          }

        }


        static uint8_t           transferByteSPI(const uint8_t data, const uint8_t delay);
      };

      template<unsigned long SPEED>
      class USISPIMaster : private USI {
      public:
        static void initialize(const uint8_t mode) {
          switch(mode) {
          case 0: USI::initialize(USI::SPI_MODE0); break;
          case 1: USI::initialize(USI::SPI_MODE1); break;
          case 2: USI::initialize(USI::SPI_MODE2); break;
          case 3: USI::initialize(USI::SPI_MODE3); break;
          }
        }

        template<unsigned long SELSPEED>
        static constexpr USISPIMaster<SELSPEED> selectSpeed() {
          return USISPIMaster<SELSPEED>();
        }

        static constexpr uint8_t getDelaySPI() {
          return static_cast<uint8_t>(min((int32_t)255,(max((int32_t)0,(int32_t)((double) F_CPU / (double) SPEED / 2.) - 7) + 2) / 3));
        }


        static uint8_t transferByte(const uint8_t data) {
          USIDR  = data;
          USISR  = 0xF0;

          do {
            USICR |= _BV(USITC);

            if(getDelaySPI() > 0)
              _delay_loop_1(getDelaySPI());

          } while (0 == (USISR & _BV(USIOIF)));

          return USIDR;
        }

        static uint8_t transferBytes(uint8_t data1, uint8_t data2)
        {
          transferByte(data1);
          return transferByte(data2);
        }

        template<typename COUNT>
        static void sendIdle(COUNT count) {
          waitToken(0xFF,0x00,count - 1);
        }

        template<typename RETRY>
        static uint8_t waitToken(const uint8_t token, const uint8_t mask, RETRY retry) {
          do {
              const uint8_t value = transferByte(0xFF);

              if ((value & mask) == token)
                return value;

          } while(retry--);

          return ~token;
        }
      };
    }
  }

  namespace Buffer {
    using namespace ::Platform::Architecture::AVR8;

    class ConstBufferIteratorValue;

    template<>
    class ConstBufferIterator<uint8_t, FlashBufferBase>
    {
    private:
      const uint8_t           *_data;
    public:
      typedef ConstBufferIterator<uint8_t, FlashBufferBase> self_type;

      constexpr ConstBufferIterator() : _data(0) {};
      constexpr ConstBufferIterator(const uint8_t *data) : _data(data) {}

      uint8_t operator * () const {
        uint16_t address = reinterpret_cast<uint16_t>(_data);
        uint8_t value;

        asm volatile (
            "lpm %0, %a1+"
            : "=r" (value), "=z" (address)
            : "1" (address)
        );
        return value;
      }

      constexpr bool operator == (const self_type &rhs) const {return this->_data == rhs._data;}
      constexpr bool operator != (const self_type &rhs) const {return this->_data != rhs._data;}
      constexpr bool operator >  (const self_type &rhs) const {return this->_data > rhs._data;}
      self_type operator + (size_t count) const {
        const uint8_t *data = _data + count;

        if (data < _data)
          data = reinterpret_cast<uint8_t*>(~0);

        return {data};
      }

      self_type & operator ++ () {_data++; return *this;}
      ConstBufferIteratorValue operator ++ (int); // {return self_type(_data++);}
    };

    class ConstBufferIteratorValue : public ConstBufferIterator<uint8_t, FlashBufferBase>
    {
    private:
      const uint8_t _val;
    public:
      constexpr ConstBufferIteratorValue(const uint8_t *data, uint8_t val) :
      ConstBufferIterator(data), _val(val) {}
      uint8_t operator * () const { return _val; }
    };

    ConstBufferIteratorValue
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

      return ConstBufferIteratorValue(bck, value);
    }

  };
}


#endif /* AVR8_HPP_ */
