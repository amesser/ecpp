/*
 * avr8.hpp
 *
 *  Created on: 01.03.2015
 *      Author: andi
 */

#ifndef ECPP_ARCH_AVR8_HPP_
#define ECPP_ARCH_AVR8_HPP_

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

namespace ecpp
{
  class RegisterConfig {
  public:
    uint8_t _u8RegNr;
    uint8_t _u8RegValue;

    constexpr RegisterConfig(volatile uint8_t &reg, const uint8_t value) :
         _u8RegNr{static_cast<uint8_t>(_SFR_IO_ADDR(reg))}, _u8RegValue{value} {}

    constexpr RegisterConfig(const RegisterConfig &init) :
        _u8RegNr(init._u8RegNr), _u8RegValue{init._u8RegValue} {}

        constexpr RegisterConfig() :
            _u8RegNr(0), _u8RegValue{0} {}

    void writeRegister() const {
      const uint8_t ioreg = this->_u8RegNr;
      const uint8_t value = this->_u8RegValue;

      _SFR_IO8(ioreg) = value;
    }
  };

  class Sys_AVR8
  {
  public:
    static void enableSleep()
    {
      sleep_enable();
    }

    static void disableSleep()
    {
      sleep_disable();
    }

    static void enterSleep()
    {
      sleep_cpu();
    }

    static void enableInterrupts()
    {
      sei();
    }

    static void disableInterupts()
    {
      cli();
    }
  };
};


#endif /* AVR8_HPP_ */
