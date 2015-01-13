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
#ifndef ATTINY2313_HPP_
#define ATTINY2313_HPP_

#include <arch/avr8.hpp>
#include <arch/avr8/usi.hpp>

namespace Platform {
  namespace Target {
    namespace AVR8 {

      class PowerManager {
      public:
        enum SLEEPMODE {
          SLEEPMODE_IDLE     = SLEEP_MODE_IDLE,
          SLEEPMODE_ADCNOISE = SLEEP_MODE_ADC,
          SLEEPMODE_STANDBY  = SLEEP_MODE_STANDBY,
          SLEEPMODE_PWRDOWN  = SLEEP_MODE_PWR_DOWN,
        };

        static void selectSleepMode(enum SLEEPMODE mode) {set_sleep_mode(mode);}
        static void enterSleepMode()                     {sleep_mode();}
      };

      class ATTiny461 {
      public:

      };
    }
  }
}



#endif /* ATTINY2313_HPP_ */
