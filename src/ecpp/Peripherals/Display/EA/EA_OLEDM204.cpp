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
#include "ecpp/Peripherals/Display/EA/EA_OLEDM204.hpp"
#include "ecpp/Units/Time.hpp"

using namespace ecpp::Peripherals::Display;
using namespace ecpp::Units;
using namespace ecpp::Target;

/** Translates an utf8 code-point to EO OLEDM204 ROM A  display coding */
uint8_t
EAOLEDM204_ROM_ID<'A'>::TextProcessor::encode(::ecpp::Text::CodePoint cp)
{
  if(cp == 0)
    return 0x20;
  else if (cp < 127)
    return cp;
  else
    return '?';
}

static constexpr uint8_t EAOLEDM204_SpiModeDataMap[16] =
{
  0, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xf0
};

void
EAOLEDM204_SpiMode::SendCommand(uint_least8_t command)
{
  uint8_t buffer[3] =
  {
    0xF8,
    EAOLEDM204_SpiModeDataMap[command & 0x0F],
    EAOLEDM204_SpiModeDataMap[(command >> 4) & 0x0F],
  };

  ecpp::Target::Bsp::SendSPI(*this, buffer, sizeof(buffer));
  /* ecpp::Target::Bsp::delay(Milliseconds<>(1)); */
}

void
EAOLEDM204_SpiMode::SendData(uint_least8_t data)
{
  uint8_t buffer[3] =
  {
    0xF8 | 0x02,
    EAOLEDM204_SpiModeDataMap[data & 0x0F],
    EAOLEDM204_SpiModeDataMap[(data >> 4) & 0x0F],
  };

  ecpp::Target::Bsp::SendSPI(*this, buffer, sizeof(buffer));
}


void
EAOLEDM204_SpiMode::Init(uint_fast8_t rom_id)
{
  ecpp::Target::Bsp::ResetDisplay(*this);

  SendCommand(0x3A);
  SendCommand(0x09);
  SendCommand(0x05);
  SendCommand(0x38);
  SendCommand(0x3A);
  SendCommand(0x72);
  SendData(rom_id << 2);
  SendCommand(0x38);
  SendCommand(0x0C);
  SendCommand(0x01);
}

void
EAOLEDM204_SpiMode::LocateCursor(uint8_t col, uint8_t row)
{
  SendCommand(0x80 | (col + row * 0x20));
}

void EAOLEDM204_SpiMode::WriteDDRAM(const uint8_t* b, uint8_t len)
{

  while(len--)
    SendData(*(b++));
}