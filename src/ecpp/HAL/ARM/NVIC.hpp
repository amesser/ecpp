
#ifndef ECPP_HAL_ARM_NVIC_HPP_
#define ECPP_HAL_ARM_NVIC_HPP_

namespace ecpp::HAL::ARM
{
  class InterruptCritical
  {
  public:
    typedef unsigned int StateType;

    static StateType enter();
    static void leave(StateType state);
  };
};

#endif