/*
 *  Copyright 2019 Andreas Messer <andi@bastelmap.de>
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
#ifndef ECPP_UNITS_TIME_HPP_
#define ECPP_UNITS_TIME_HPP_

#include "ecpp/Units/Common.hpp"

namespace ecpp::Units
{
  template<typename T = unsigned, signed POWER = 0, unsigned EXP = 10>
  class FixedScaleTime : public FixedScaleQuantity<T, POWER, EXP>
  {
  public:
    constexpr FixedScaleTime() : FixedScaleQuantity<T, POWER, EXP>() {}
    explicit  constexpr FixedScaleTime(const T & t) : FixedScaleQuantity<T, POWER, EXP>(t) {}

    template<typename T_RHS>
    FixedScaleTime & operator= (const FixedScaleTime<T_RHS, POWER, EXP> & rhs)
    {
      FixedScaleQuantity<T, POWER, EXP>::operator=(rhs);
      return *this;
    }

    template<typename T_RHS>
    constexpr bool operator> (const FixedScaleTime<T_RHS, POWER, EXP> & rhs) const
    {
      return this->Value > rhs.Value;
    }
  };

  template<typename T = unsigned>
  class Milliseconds : public FixedScaleTime<T, -3>
  {
  public:
    constexpr Milliseconds(const T & ms) : FixedScaleTime<T, -3>(ms) {}

    template<typename T_RHS>
    Milliseconds & operator= (const FixedScaleTime<T_RHS, -3> & rhs)
    {
      FixedScaleTime<T, -3>::operator=(rhs);
      return *this;
    }

    using FixedScaleTime<T, -3>::operator> ;
  };

}

#endif