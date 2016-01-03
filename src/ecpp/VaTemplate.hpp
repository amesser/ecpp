/*
 *  Copyright 2013-2016 Andreas Messer <andi@bastelmap.de>
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
#ifndef ECPP_VATEMPLATE_HPP_
#define ECPP_VATEMPLATE_HPP_

namespace ecpp
{
  typedef unsigned long indices_type;

  template<indices_type... Is>
  struct indices {
    static constexpr indices_type count() {return 0;};
  };

  template<indices_type N, indices_type... Is>
  struct indices<N, Is...> {
    static constexpr indices_type count() {return 1 + indices<Is...>::count();}
  };

  template <indices_type... Is>
  struct build_indices {};

  template <indices_type N, indices_type... Is>
  struct build_indices<N, Is...> : build_indices<N-1, N-1, Is...> {};

  template <indices_type... Is>
  struct build_indices<0, Is...> : indices<Is...> {};

};


#endif /* ECPP_VATEMPLATE_HPP_ */
