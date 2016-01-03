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
#ifndef ECPP_TARGET_AVR8_SPI_HPP_
#define ECPP_TARGET_AVR8_SPI_HPP_

#include <ecpp/Arch/AVR8.hpp>
#include <ecpp/Datatypes.hpp>

#if defined(SPCR)
namespace ecpp
{
  template<unsigned int DIVISOR>
  class SPIParameters;

  template<unsigned int DIVISOR>
  class SPIParameters : public SPIParameters<DIVISOR+1>
  {};

  template <>
  class SPIParameters<1000>
  {};

  template <>
  class SPIParameters<2>
  {
  public:
    static constexpr uint8_t CR  = 0;
    static constexpr uint8_t SR  = _BV(SPI2X);
  };

  template <>
  class SPIParameters<4>
  {
  public:
    static constexpr uint8_t CR  = 0;
    static constexpr uint8_t SR  = 0;
  };

  template <>
  class SPIParameters<8>
  {
  public:
    static constexpr uint8_t CR  = _BV(SPR0);
    static constexpr uint8_t SR  = _BV(SPI2X);
  };

  template <>
  class SPIParameters<16>
  {
  public:
    static constexpr uint8_t CR  = _BV(SPR0);
    static constexpr uint8_t SR  = 0;
  };

  template <>
  class SPIParameters<32>
  {
  public:
    static constexpr uint8_t CR  = _BV(SPR1);
    static constexpr uint8_t SR  = _BV(SPI2X);
  };

  template <>
  class SPIParameters<64>
  {
  public:
    static constexpr uint8_t CR  = _BV(SPR1);
    static constexpr uint8_t SR  = 0;
  };

  template <>
  class SPIParameters<128>
  {
  public:
    static constexpr uint8_t CR  = _BV(SPR1) | _BV(SPR0);
    static constexpr uint8_t SR  = 0;
  };

  class SPI
  {
  public:
    template<unsigned long MODE, unsigned long SPEED>
    static void configureMasterSPI()
    {
      SPCR = SPIParameters<F_CPU/SPEED>::CR | (MODE << CPHA) | _BV(MSTR) | _BV(SPE);
      SPSR = SPIParameters<F_CPU/SPEED>::SR;
    }

  };

  class SPIMaster : public SPI
  {
  public:
    static uint8_t
    transferByte(uint8_t data)
    {
      SPDR = data;
      while(0 == (SPSR & _BV(SPIF)));

      return SPDR;
    }

    template<typename COUNT>
    static void sendIdle(COUNT count)
    {
      waitToken(0xFF,0x00,count - 1);
    }

    template<typename RETRY>
    static uint8_t waitToken(const uint8_t token, const uint8_t mask, RETRY retry)
    {
      do {
          const uint8_t value = transferByte(0xFF);

          if ((value & mask) == token)
            return value;

      } while(retry--);

      return ~token;
    }

  };
};
#endif


#endif /* EXTERNALS_ECPP_SRC_ECPP_TARGET_AVR8_SPI_HPP_ */
