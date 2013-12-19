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
#ifndef VAMATH_HPP_
#define VAMATH_HPP_

namespace Platform {
  namespace Util {
    namespace VaMath {
      template<typename ...Args>
      constexpr uint8_t sum(Args const & ...args)
      {
        return 0;
      }

      template<typename t>
      constexpr t sum(const t & arg)
      {
        return arg;
      }

      template<typename t, typename ...Rest>
      constexpr auto sum(const t & val, Rest... rest)
        -> decltype((t)0 + sum(rest...))
      {
        return val + sum(rest...);
      }

      template<typename ...Args>
      constexpr size_t argcount(Args const & ...args)
      {
        return 0;
      }

      template<typename Arg, typename ...Args>
      constexpr size_t argcount(Arg const & arg, Args const & ...args)
      {
        return 1 + argcount(args...);
      }

      template<typename ...Args>
      struct args {static constexpr size_t count() {return 0;} };

      template<typename Arg, typename ...Args>
      struct args<Arg, Args...> {static constexpr size_t count() {return 0 + args<Args...>::count();} };

      template<typename ...Args>
      constexpr uint8_t va_or(Args const & ...arg)
      {
        return 0;
      }

      template<typename Arg>
      constexpr Arg va_or(Arg const &arg)
      {
        return arg;
      }

      template<typename Arg, typename ...Rest>
      constexpr Arg va_or(Arg const &arg, Rest const & ...rest)
      {
        return arg | va_or(rest...);
      }
    }
  }
}


#endif /* VAMATH_HPP_ */
