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
#ifndef ECPP_UNITS_TICKS_HPP_
#define ECPP_UNITS_TICKS_HPP_

#include <type_traits>

#include "ecpp/Units/Time.hpp"
#include "ecpp/Datatypes.hpp"
#include "ecpp/Meta/Math.hpp"

namespace ecpp::Units {

  template<unsigned FREQ, typename VALUETYPE = unsigned>
  class Power10Tick;

  template<unsigned FREQ, typename VALUETYPE = unsigned>
  class TickDelta
  {
  public:
    /* Tickdelta is give as signed value */
    typedef typename ::std::make_signed<VALUETYPE>::type ValueType;

    static constexpr unsigned Frequency = FREQ;

    template<typename T, signed POWER>
    constexpr TickDelta( const FixedScaleTime<T, POWER, 10> & init) : Delta( IntegerScaler<-(::ecpp::MetaLog<10, FREQ>::Result), POWER, 10>::template rescale <ValueType> (init.Value) + 1 ) {}

    template<typename T>
    constexpr bool operator > (const Milliseconds<T> & rhs) const
    {
      return Delta > static_cast< typename ::std::make_signed< typename Milliseconds<T>::ValueType>::type>((rhs.Value + (1000 / FREQ) - 1) / (1000 / FREQ));
    }


  private:
    ValueType Delta {0};

    constexpr TickDelta() {};
    constexpr TickDelta(ValueType init) : Delta(init) {};
    constexpr TickDelta(VALUETYPE init) : Delta( static_cast<ValueType>(init)) {};

    constexpr operator ValueType() const { return Delta; }

    template<typename T>
    constexpr bool operator >= (const T & rhs) const
    {
      return Delta >= static_cast< typename ::std::make_signed< T>::type>(rhs);
    }

    template<typename T>
    constexpr bool operator < (const T & rhs) const
    {
      return Delta < static_cast< typename ::std::make_signed< T>::type>(rhs);
    }

    friend class Power10Tick<FREQ, VALUETYPE>;
  };

  template<unsigned FREQ, typename VALUETYPE>
  class Power10Tick : protected FixedScaleTime<VALUETYPE, -(::ecpp::MetaLog<10, FREQ>::Result),10>
  {
  public:
    typedef FixedScaleTime<VALUETYPE, -(::ecpp::MetaLog<10, FREQ>::Result),10> BaseType;
    typedef TickDelta<FREQ, VALUETYPE> DeltaType;

    static constexpr unsigned Frequency = FREQ;

    constexpr Power10Tick() : BaseType() {};

    template<typename T, unsigned INPOWER>
    constexpr Power10Tick(const FixedScaleTime<T, INPOWER, 10> & init) : BaseType(init) {}
    //constexpr StaticFreqTick(ValueType init) : Value(init) {};

    /*
    constexpr TickValue(const ::ecpp::Units::Milliseconds init) : Value((init.Value * FREQ + 999) / 1000) {};
    */
    const Power10Tick & operator++ ()
    {
      BaseType::Value++; return *this;
    }

    Power10Tick & operator+=(const DeltaType & rhs)
    {
      BaseType::Value+= rhs.Delta;
      return *this;
    }

    constexpr Power10Tick operator + (const DeltaType & rhs)
    {
      return { BaseType::Value + rhs.Delta } ;
    }

    DeltaType  operator-(const Power10Tick & rhs) const
    {
      return DeltaType(BaseType::Value - rhs.Value);
    }

    bool operator < (const Power10Tick & rhs) const
    {
      return (*this - rhs) < 0;
    }

    bool operator >= (const Power10Tick & rhs) const
    {
      return (*this - rhs) >= 0;
    }
  private:
    template<typename T>
    constexpr Power10Tick(const T & init) : BaseType(init) {}
  };

  class UnimplementedTick {};

  template<unsigned FREQ, typename VALUETYPE = unsigned>
  using StaticFreqTick = typename ::ecpp::Meta::MetaIf< ::ecpp::MetaLog<10, FREQ>::Remain,
                                                        UnimplementedTick,
                                                        Power10Tick<FREQ, VALUETYPE> >;
}

#endif