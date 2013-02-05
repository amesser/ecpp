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

namespace Platform {
  namespace Architecture {
    namespace AVR8 {
      class CriticalSectionGuard {
      public:
        CriticalSectionGuard()  {cli();}
        ~CriticalSectionGuard() {sei();}
      };


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
          return static_cast<uint8_t>(min(255.,max((int32_t)0,(int32_t)((double) F_CPU / (double) SPEED / 2.) - 7 + 2) / 3 + 1.));
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
}


#endif /* AVR8_HPP_ */
