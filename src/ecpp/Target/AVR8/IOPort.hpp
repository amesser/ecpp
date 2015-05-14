/*
 * Port.hpp
 *
 *  Created on: 08.05.2015
 *      Author: andi
 */

#ifndef ECPP_TARGET_AVR8_IOPORT_HPP_
#define ECPP_TARGET_AVR8_IOPORT_HPP_

#include <ecpp/Arch/AVR8.hpp>

namespace ecpp
{
  enum
  {
    AVR_IO_PB  = 0x10,

    AVR_IO_PB0 = 0x10,
    AVR_IO_PB1 = 0x11,
    AVR_IO_PB2 = 0x12,
    AVR_IO_PB3 = 0x13,
    AVR_IO_PB4 = 0x14,
    AVR_IO_PB5 = 0x15,

    AVR_IO_PC  = 0x20,

    AVR_IO_PC0 = 0x20,
    AVR_IO_PC1 = 0x21,
    AVR_IO_PC2 = 0x22,
    AVR_IO_PC3 = 0x23,
    AVR_IO_PC4 = 0x24,
    AVR_IO_PC5 = 0x25,
    AVR_IO_PC6 = 0x26,

    AVR_IO_PD  = 0x30,

    AVR_IO_PD0 = 0x30,
    AVR_IO_PD1 = 0x31,
    AVR_IO_PD2 = 0x32,
    AVR_IO_PD3 = 0x33,
    AVR_IO_PD4 = 0x34,
    AVR_IO_PD5 = 0x35,
    AVR_IO_PD6 = 0x36,

  };

  template<int PORT>
  class IOPort {};

  template<>
  class IOPort<AVR_IO_PB>
  {
  public:
    static constexpr volatile uint8_t * DDR  = &DDRB;
    static constexpr volatile uint8_t * PIN  = &PINB;
    static constexpr volatile uint8_t * PORT = &PORTB;
  };

#if defined(DDRC)
  template<>
  class IOPort<AVR_IO_PC>
  {
  public:
    static constexpr volatile uint8_t * DDR  = &DDRC;
    static constexpr volatile uint8_t * PIN  = &PINC;
    static constexpr volatile uint8_t * PORT = &PORTC;
  };
#endif

#if defined(DDRD)
  template<>
  class IOPort<AVR_IO_PD>
  {
  public:
    static constexpr volatile uint8_t * DDR  = &DDRD;
    static constexpr volatile uint8_t * PIN  = &PIND;
    static constexpr volatile uint8_t * PORT = &PORTD;
  };
#endif

  template<int PIN>
  class IOPin
  {
  public:
    static constexpr uint8_t MASK = 0x01 << (PIN & 0xF);

    static void enableOutput()  __attribute__((always_inline))
    {
      *(IOPort<PIN & 0xF0>::DDR) |=  MASK;
    }

    static void disableOutput() __attribute__((always_inline))
    {
      *(IOPort<PIN & 0xF0>::DDR) &= ~MASK;
    }

    static void setOutput()     __attribute__((always_inline))
    {
      *(IOPort<PIN & 0xF0>::PORT) |= MASK;
    }

    static void clearOutput()   __attribute__((always_inline))
    {
      *(IOPort<PIN & 0xF0>::PORT) &= ~MASK;
    }

    static bool getInput()      __attribute__((always_inline))
    {
      return 0 != (*(IOPort<PIN & 0xF0>::PIN) & MASK);
    }
  };
}


#endif /* ECPP_TARGET_AVR8_IOPORT_HPP_ */
