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
#ifndef TUPLE_HPP_
#define TUPLE_HPP_

#include "util/indices.hpp"

namespace Platform {
  namespace Util {
    template<typename... Args>
    struct tuple;

    template<>
    struct tuple<>
    {
      static constexpr size_t size = 0;
      static constexpr build_indices<size> indices = build_indices<size>();

      template<typename ...Args>
      static constexpr void get_value(Args const & ...args) {};
    };

    template<typename Type, typename... Rest>
    struct tuple<Type, Rest...> : public tuple<Rest...>
    {
      typedef Type ValueType;
      Type value;

      constexpr tuple(Type const & val, Rest const & ...rest) : tuple<Rest...>(rest...), value(val) {};
      static constexpr size_t size = 1 + tuple<Rest...>::size;
      static constexpr build_indices<size> indices = build_indices<size>();

      template<size_t N>
      constexpr Type const & get_value(::indices<N> const &) const {return value;}


      template<size_t N, size_t ...Is>
      constexpr auto get_value(::indices<N, Is...> const &) const -> decltype(((tuple<Rest...>*)0)->get_value(::indices<Is...>())) {
        return tuple<Rest...>::get_value(::indices<Is...>());
      }

    };

    template<typename ...Args>
    constexpr tuple<Args...>
    make_tuple(Args const & ...args)
    {
      return tuple<Args...>(args...);
    }

    template<size_t N, typename ...Args>
    constexpr auto
    get_value(tuple<Args...> const & t) ->decltype(t.get_value(build_indices<N+1>()))
    {
      return t.get_value(build_indices<N+1>());
    }
  }
};



#endif /* TUPLE_HPP_ */
