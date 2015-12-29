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
    AVR_IO_PD7 = 0x37,

  };

  template<int PORT>
  class IOPortRegisters;

  template<int PORT>
  class IOPort;

  class IOMask
  {
  private:
    uint8_t m_Mask;
  public:
    constexpr IOMask(const uint8_t init) : m_Mask(init) {}
  };

  template<int PIN>
  class IOPinMask
  {
  public:
    static constexpr uint8_t MASK = 0x01 << (PIN & 0xF);
  };

  template<int PIN>
  class IOPinStateOutputHigh  : public IOPinMask<PIN> {};

  template<int PIN>
  class IOPinStateOutputLow   : public IOPinMask<PIN> {};

  template<int PIN>
  class IOPinStateInputPullUp : public IOPinMask<PIN> {};

  template<int PIN>
  class IOPinStateInput       : public IOPinMask<PIN> {};


  class IOPortState
  {
  private:
    const uint8_t m_DDR;
    const uint8_t m_PORT;
  public:
    constexpr uint8_t getDDR()  const {return m_DDR;}
    constexpr uint8_t getPORT() const {return m_PORT;}

    template<int PIN>
    constexpr IOPortState(const IOPinStateOutputHigh<PIN> &outhigh) : m_DDR(outhigh.MASK), m_PORT(outhigh.MASK) {};

    template<int PIN>
    constexpr IOPortState(const IOPinStateOutputLow<PIN> &outlow)   : m_DDR(outlow.MASK), m_PORT(0) {};

    template<int PIN>
    constexpr IOPortState(const IOPinStateInputPullUp<PIN> &inpullup)   : m_DDR(0), m_PORT(inpullup.MASK) {};

    template<int PIN>
    constexpr IOPortState(const IOPinStateInput<PIN> &in)   : m_DDR(0), m_PORT(0) {};

    constexpr IOPortState(uint8_t DDR, uint8_t PORT) : m_DDR(DDR), m_PORT(PORT) {};


    constexpr IOPortState operator | (const IOPortState &rhs) const __attribute__((always_inline))
    {
      return IOPortState(m_DDR | rhs.m_DDR, m_PORT | rhs.m_PORT);
    }
  };

  template<template<int> class LHS, template<int> class RHS, int LHSPIN, int RHSPIN>
  constexpr IOPortState operator | (const LHS<LHSPIN> lhs, const RHS<RHSPIN> rhs) __attribute__((always_inline));

  template<template<int> class LHS, template<int> class RHS, int LHSPIN, int RHSPIN>
  constexpr IOPortState operator | (const LHS<LHSPIN> lhs, const RHS<RHSPIN> rhs)
  {
    return IOPortState(lhs) | IOPortState(rhs);
  }

  template<int PIN>
  class IOPin : public IOPortRegisters<PIN & 0xF0>
  {
  public:
    static constexpr IOPinStateOutputHigh<PIN>  OutHigh  = {};
    static constexpr IOPinStateOutputLow<PIN>   OutLow   = {};
    static constexpr IOPinStateInputPullUp<PIN> InPullUp = {};
    static constexpr IOPinStateInput<PIN>       In       = {};

    static constexpr uint8_t MASK = 0x01 << (PIN & 0xF);

    static void enableOutput()  __attribute__((always_inline))
    {
      *(IOPin::DDR) |=  MASK;
    }

    static void disableOutput() __attribute__((always_inline))
    {
      *(IOPin::DDR) &= ~MASK;
    }

    static void setOutput()     __attribute__((always_inline))
    {
      *(IOPin::PORT) |= MASK;
    }

    static void clearOutput()   __attribute__((always_inline))
    {
      *(IOPin::PORT) &= ~MASK;
    }

    static bool getOutput() __attribute__((always_inline))
    {
      return 0 != (*(IOPin::PORT) & MASK);
    }

    static bool getInput() __attribute__((always_inline))
    {
      return 0 != (*(IOPin::PIN) & MASK);
    }

    void operator = (bool state) __attribute__((always_inline))
    {
      if(state)
        setOutput();
      else
        clearOutput();
    }

    constexpr operator bool() __attribute__((always_inline))
    {
      return getInput();
    }

    template<int RHSPIN>
    constexpr IOMask operator | (const IOPin<RHSPIN> & rhs) const
    {
      return MASK | rhs.MASK;
    }
  };


  template<int PORT>
  class IOPortRegisters {};

  template<>
  class IOPortRegisters<AVR_IO_PB>
  {
  protected:
    static constexpr volatile uint8_t * DDR  = &DDRB;
    static constexpr volatile uint8_t * PIN  = &PINB;
    static constexpr volatile uint8_t * PORT = &PORTB;
  };

#if defined(DDRC)
  template<>
  class IOPortRegisters<AVR_IO_PC>
  {
  protected:
    static constexpr volatile uint8_t * DDR  = &DDRC;
    static constexpr volatile uint8_t * PIN  = &PINC;
    static constexpr volatile uint8_t * PORT = &PORTC;
  };
#endif

#if defined(DDRD)
  template<>
  class IOPortRegisters<AVR_IO_PD>
  {
  protected:
    static constexpr volatile uint8_t * DDR  = &DDRD;
    static constexpr volatile uint8_t * PIN  = &PIND;
    static constexpr volatile uint8_t * PORT = &PORTD;
  };
#endif

  template<int PORT>
  class IOPort : public IOPortRegisters<PORT>
  {
  public:
    void clearOutputs(uint8_t outputs)
    {
      *(this->PORT) = *(this->PORT) & ~(outputs);
    }

    void setOutputs(uint8_t outputs)
    {
      *(this->PORT) = *(this->PORT) | outputs;
    }

    void updateOutputs(uint8_t outputs, uint8_t mask)
    {

      *(this->PORT) = (*(this->PORT) & ~(mask)) | outputs;
    }

    void operator = (uint8_t out)
    {
      *(this->PORT) = out;
    }

    void operator = (const IOPortState state)
    {
      *(IOPortRegisters<PORT>::DDR)  &= state.getDDR();
      *(IOPortRegisters<PORT>::PORT) =  state.getPORT();
      *(IOPortRegisters<PORT>::DDR)  =  state.getDDR();
    }

    static void setState (const IOPortState state)
    {
      *(IOPortRegisters<PORT>::DDR)  &= state.getDDR();
      *(IOPortRegisters<PORT>::PORT) =  state.getPORT();
      *(IOPortRegisters<PORT>::DDR)  =  state.getDDR();
    }

    operator uint8_t () const
    {
      return *(this->PIN);
    }

    void setDirection(uint8_t direction)
    {
      *(this->DDR) = direction;
    }

    void updateDirection(uint8_t direction, uint8_t mask)
    {

      *(this->DDR) = (*(this->DDR) & ~(mask)) | direction;
    }
  };
}


#endif /* ECPP_TARGET_AVR8_IOPORT_HPP_ */
