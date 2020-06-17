/*
 *  Copyright 2020 Andreas Messer <andi@bastelmap.de>
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
#include "ecpp/Peripherals/Display/NHD/NHD_0420DZW.hpp"
#include "ecpp/Units/Time.hpp"

using namespace ecpp::Peripherals::Display::NHD;
using namespace ecpp::Units;
using namespace ecpp::Target;

void
NHD0420DZW_4Bit::initDisplay()
{
  Bsp::resetLcd(*this);
  Bsp::clearLcdRS(*this);

  /* switch display controller to 4 bit */
  Bsp::writeLcdNibble(*this, 0x2);
  Bsp::writeLcdNibble(*this, 0x2);
  Bsp::writeLcdNibble(*this, 0x8);

  Bsp::delay(Milliseconds<>(1));

  /* display off */
  sendCommand(0x08);

  /* display clear */
  sendCommand(0x01);

  /* entry mode */
  sendCommand(0x06);

  /* home */
  sendCommand(0x02);

  /* display on */
  sendCommand(0x0C);
}

void
NHD0420DZW_4Bit::locateCursor(uint8_t col, uint8_t row)
{
  Bsp::clearLcdRS(*this);

  switch(row)
  {
  case 0: sendCommand(0x80 + 0x00 + col); break;
  case 1: sendCommand(0x80 + 0x40 + col); break;
  case 2: sendCommand(0x80 + 0x14 + col); break;
  case 3: sendCommand(0x80 + 0x54 + col); break;
  }
}

void
NHD0420DZW_4Bit::writeDDRAM(const void* b, uint8_t len)
{
  const uint8_t *p, *pend;

  Bsp::setLcdRS(*this);

  for( p = reinterpret_cast<const uint8_t*>(b), pend = p + len;
       p < pend; ++p)
  {
    if(*p == 0) /* write space instead of null char */
      Bsp::writeLcdByte(*this, 0x20);
    else
      Bsp::writeLcdByte(*this, *p);
  }
}

void
NHD0420DZW_4Bit::sendCommand(uint8_t command)
{
  Bsp::clearLcdRS(*this);
  Bsp::writeLcdByte(*this, command);
  Bsp::delay(Milliseconds<>(3));
}