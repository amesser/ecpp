/*
 * avr8.hpp
 *
 *  Created on: 01.03.2015
 *      Author: andi
 */

#ifndef ECPP_ARCH_AVR8_HPP_
#define ECPP_ARCH_AVR8_HPP_

#include "avr/io.h"
#include "avr/sleep.h"

namespace ECPP
{
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
