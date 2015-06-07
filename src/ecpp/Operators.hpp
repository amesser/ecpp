/*
 * Operators.hpp
 *
 *  Created on: 07.06.2015
 *      Author: andi
 */

#ifndef ECPP_OPERATORS_HPP_
#define ECPP_OPERATORS_HPP_

#include "ecpp/Datatypes.hpp"

namespace ecpp
{
  template<typename T>
  T addWrapped(T & lhs, T rhs, T wrap)
  {
    typename DoubleWidthType<T>::t_DoubleWidth accu;
    T overflows = 0;

    accu = lhs + rhs;

    while(accu >= wrap)
    {
      accu     -= wrap;
      overflows += 1;
    }

    lhs = accu;
    return overflows;
  }

  template<typename T>
  T subWrapped(T & lhs, T rhs, T wrap)
  {
    T accu;
    T overflows = 0;

    accu = lhs;

    while (accu < rhs)
    {
      accu += wrap;
      overflows += 1;
    }

    lhs = accu - rhs;
    return overflows;
  }
}




#endif /* EXTERNALS_ECPP_SRC_ECPP_OPERATORS_HPP_ */
