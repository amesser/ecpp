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
#include "ecpp/HAL/ATSAM4S/EEFC.hpp"

using namespace ::ecpp::HAL::ATSAM4S;

void
EEFC::waitReady()
{
  while(0 == (Regs.FSR & MSK_FSR_FRDY));
}

void
EEFC::eraseSectors(unsigned int num_pages, void* start)
{
  uint32_t addr = (uint32_t)start;
  uint32_t cmd;

  cmd  = VAL_FCMD_EPA;
  cmd |= ((num_pages / PagesPerSector)      << 8) & 0x00000300;
  cmd |= (((addr - 0x00400000)) / PageSize  << 8) & 0x00FFFC00;
  cmd |= VAL_FCR_KEY;

  Regs.FCR = cmd;
  waitReady();
}

void
EEFC::writePages(unsigned int num_pages, void* start, const void* src)
{
  const uint32_t *q = static_cast<const uint32_t*>(src);
  uint32_t *p       = static_cast<uint32_t*>(start);

  while(num_pages > 0)
  {
    uint32_t addr = reinterpret_cast<uint32_t>(p);
    uint32_t cmd;

    unsigned int i;

    /* Fill  latchbuffer with one page */
    for(i = 0; i < (PageSize / sizeof(*p)); ++i)
      p[i] = q[i];

    /* setup command for writing one page */
    cmd  = VAL_FCMD_WP;
    cmd |= (((addr - 0x00400000)) / PageSize << 8) & 0x00FFFF00;
    cmd |= VAL_FCR_KEY;

    Regs.FCR = cmd;
    waitReady();

    /* one page was written */
    num_pages -= 1;

    /* move to next page to write */
    p+=i;
    q+=i;
  }
}
