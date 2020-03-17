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
#ifndef ECPP_ABI_SIMPLEBINFORMAT_HPP_
#define ECPP_ABI_SIMPLEBINFORMAT_HPP_

#include <elf.h>

namespace ecpp::Abi
{
  extern "C" Elf32_Rel __rel_start__;
  extern "C" Elf32_Rel __rel_end__;

  /** A simple dynamic loader for binary code.
   *
   * Can be used to implement dynamic loading and execution of code from any source
   * in ram memory.
   *
   * For use, derive your own class from this one and define function pointer members.
   * */
  class SimpleBinaryFormat
  {
  public:
    constexpr SimpleBinaryFormat() : SelfPointer(this) {};

    struct SimpleBinaryFormat    * const SelfPointer;

    const Elf32_Rel              * const RelocsStartPtr = &__rel_start__;

    const Elf32_Rel              * const RelocsEndPtr   = &__rel_end__;

    bool relocate();
  };
}

#endif