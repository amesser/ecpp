/*
 * Filter.hpp
 *
 *  Created on: 21.04.2015
 *      Author: andi
 */

#ifndef ECPP_FILTER_HPP_
#define ECPP_FILTER_HPP_

namespace ecpp
{
  template<typename TYPE, unsigned long SAMPLES>
  class PT1
  {
  private:
    TYPE _sum;
  public:

    void addValue(const TYPE value);
    TYPE getValue() const;

    TYPE getSum() const;
    TYPE operator - (const PT1 & rhs) const;
  };

  template<typename TYPE, unsigned long SAMPLES>
  void PT1<TYPE, SAMPLES>::addValue(const TYPE value)
  {
    TYPE s = _sum - getValue();
    _sum = s + value;
  }


  template<typename TYPE, unsigned long SAMPLES>
  TYPE PT1<TYPE,SAMPLES>::getValue(void) const
  {
    return (_sum + (TYPE)(SAMPLES/2)) / SAMPLES;
  }

  template<typename TYPE, unsigned long SAMPLES>
  TYPE PT1<TYPE,SAMPLES>::getSum(void) const
  {
    return _sum;
  }


  template<typename TYPE, unsigned long SAMPLES>
  TYPE PT1<TYPE,SAMPLES>::operator - (const PT1 & rhs) const
  {
    return (_sum - rhs._sum + SAMPLES/2) / SAMPLES;
  }
};


#endif /* FILTER_HPP_ */
