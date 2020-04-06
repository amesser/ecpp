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
#ifndef ECPP_UNITS_COMMON_HPP_
#define ECPP_UNITS_COMMON_HPP_

#include "ecpp/Meta/Conditional.hpp"
#include "ecpp/Meta/Math.hpp"

namespace ecpp::Units
{

  template<unsigned long Scale>
  class IntegerDownScaler
  {
  public:
    template<typename OutType, typename InType>
    static constexpr OutType rescale(InType value)
    {
      return (value + Scale / 2) / Scale;
    };
  };

  template<unsigned long Scale>
  class IntegerUpScaler
  {
  public:
    template<typename OutType, typename InType>
    static constexpr OutType rescale(InType value)
    {
      return (value * Scale);
    };
  };


  template<bool DOWN, signed TO_POWER, signed FROM_POWER, unsigned EXP>
  class PowerScaler {};

  template<signed TO_POWER, signed FROM_POWER, unsigned EXP>
  class PowerScaler<true, TO_POWER, FROM_POWER, EXP>
  {
  public:
    typedef  IntegerDownScaler<::ecpp::MetaExp<EXP, TO_POWER - FROM_POWER>::Result> ScalerType;
  };

  template<signed TO_POWER, signed FROM_POWER, unsigned EXP>
  class PowerScaler<false, TO_POWER, FROM_POWER, EXP>
  {
  public:
    typedef  IntegerUpScaler<::ecpp::MetaExp<EXP, FROM_POWER - TO_POWER>::Result> ScalerType;
  };


  /*
  template<signed TO_POWER, signed FROM_POWER, unsigned EXP>
  using IntegerScaler = typename ::ecpp::Meta::MetaIf< (TO_POWER > FROM_POWER),
                                                       IntegerDownScaler<::ecpp::MetaExp<EXP, TO_POWER - FROM_POWER>::Result>,
                                                       IntegerUpScaler<::ecpp::MetaExp<EXP, FROM_POWER - TO_POWER>::Result> >;
  */

  template<signed TO_POWER, signed FROM_POWER, unsigned EXP>
  using IntegerScaler = typename PowerScaler< (TO_POWER > FROM_POWER), TO_POWER, FROM_POWER, EXP>::ScalerType;


  /* represents a quantitiy with a fixed scale*/
  template<typename T, signed POWER, unsigned EXP = 10>
  class FixedScaleQuantity
  {
  public:
    typedef T ValueType;

    ValueType Value {};

    static constexpr unsigned Exp   = EXP;
    static constexpr signed   Power = POWER;

    constexpr FixedScaleQuantity() {}

    template<typename T_RHS>
    constexpr FixedScaleQuantity(const FixedScaleQuantity<T_RHS, POWER, 10> & init) : Value {init.Value} {}

    template<typename T_RHS>
    FixedScaleQuantity & operator = (const FixedScaleQuantity<T_RHS, POWER, 10> &rhs)
    {
      Value = rhs.Value;
      return *this;
    }

  protected:
    constexpr FixedScaleQuantity(const ValueType &init) : Value(init) {}

    template<typename TYPE, signed INPOWER>
    ValueType rescale(const FixedScaleQuantity<TYPE, INPOWER, EXP> & quantity);


    /* template<typename TYPE, signed INPOWER, unsigned INEXP>
       ValueType rescale(const FixedScaleQuantity<TYPE, INPOWER, INEXP> & quantity); */
  };

  template<typename T, signed POWER, unsigned EXP>
  template<typename TYPE, signed INPOWER>
  typename FixedScaleQuantity<T, POWER, EXP>::ValueType FixedScaleQuantity<T, POWER, EXP>::rescale(const FixedScaleQuantity<TYPE, INPOWER, EXP> & quantity)
  {
    return { IntegerScaler<POWER, INPOWER, EXP>::rescale(quantity.Value) };

    /*
    if (POWER > INPOWER)
      return { (quantity.Value + (EXP**(POWER- INPOWER)) / 2) / (EXP**(POWER- INPOWER)) };
    else if (POWER == INPOWER)
      return { quantity.Value };
    else
      return { quantity.Value * (EXP**(INPOWER - POWER)) }; */
  }


}
#endif