/*
 * VaTemplate.hpp
 *
 *  Created on: 27.06.2015
 *      Author: andi
 */

#ifndef ECPP_VATEMPLATE_HPP_
#define ECPP_VATEMPLATE_HPP_

namespace ecpp
{
  typedef unsigned int indices_type;

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
