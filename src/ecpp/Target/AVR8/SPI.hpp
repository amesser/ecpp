/*
 * SPI.hpp
 *
 *  Created on: 13.05.2015
 *      Author: andi
 */

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
      _delay_us(10);
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
