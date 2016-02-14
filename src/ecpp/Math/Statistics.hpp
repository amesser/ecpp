/*
 * Statistics.hpp
 *
 *  Created on: 14.02.2016
 *      Author: andi
 */

#ifndef ECPP_MATH_STATISTICS_HPP_
#define ECPP_MATH_STATISTICS_HPP_

#include <ecpp/Datatypes.hpp>

namespace ecpp
{
  template<typename SUMTYPE, typename CNTTYPE, unsigned long MAXCNT = TypeProperties<CNTTYPE>::Max>
  class Averager
  {
  public:
    typedef SUMTYPE SumType;
    typedef CNTTYPE CntType;

  private:
    static constexpr CntType s_MaxCnt = MAXCNT;

    SUMTYPE m_Sum;
    CNTTYPE m_Cnt;
  public:
    constexpr CNTTYPE counts()
    {
      return m_Cnt;
    }

    void init()
    {
      m_Sum = 0;
      m_Cnt = 0;
    }

    template<typename VTYPE>
    void sample(VTYPE value)
    {
      if (m_Cnt < s_MaxCnt)
      {
        m_Cnt = m_Cnt + 1;
        m_Sum = m_Sum + value;
      }
    }

    const SUMTYPE value(void)
    {
      if(m_Cnt > 0)
      {
        return (m_Sum + m_Cnt / 2) / m_Cnt;
      }
      else
      {
        return 0;
      }
    }
  };
};




#endif /* ECPP_MATH_STATISTICS_HPP_ */
