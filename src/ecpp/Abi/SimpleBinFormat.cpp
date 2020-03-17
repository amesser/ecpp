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
#include "ecpp/Abi/SimpleBinFormat.hpp"

using namespace ::ecpp::Abi;

bool
SimpleBinaryFormat::relocate()
{
  Elf32_Rel *rel, *rel_end;
  uintptr_t        offset;
  uintptr_t        base;

  /* calculate relocation offset */
  offset = reinterpret_cast<uintptr_t>(this) - reinterpret_cast<uintptr_t>(SelfPointer);

  /* no relocation required */
  if(0 == offset)
    return true;

  /* manually relocate section pointers */
  rel     = reinterpret_cast<Elf32_Rel*> (reinterpret_cast<uintptr_t>(RelocsStartPtr) + offset);
  rel_end = reinterpret_cast<Elf32_Rel*> (reinterpret_cast<uintptr_t>(RelocsEndPtr) + offset);

#if 0
  if (! validatePointer(rel))
    return false;

  if (! validatePointer(rel_end))
    return false;
#endif

  base    = reinterpret_cast<uintptr_t>(this);
  while(rel < rel_end)
  {
    uintptr_t* p;

    if (R_ARM_RELATIVE != ELF32_R_TYPE(rel->r_info))
      break;

    p  = reinterpret_cast<uintptr_t*>(base + rel->r_offset);
    *p += offset;

    rel++;
  }

  if(rel < rel_end)
    return false;

  return true;
}