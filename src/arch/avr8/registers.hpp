/*  Copyright 2015 Andreas Messer <andi@bastelmap.de>
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
#ifndef REGISTERS_HPP_
#define REGISTERS_HPP_

#include <stdint.h>
#include <avr/io.h>

namespace Platform {
  namespace Architecture {
    namespace AVR8 {
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
    }
  }
};
#endif /* REGISTERS_HPP_ */
