#ifndef ECPP_TARGET_DISPLAYBSP_HPP_
#define ECPP_TARGET_DISPLAYBSP_HPP_

#include <cstdint>

namespace ecpp::Target::Bsp
{
  using namespace std;

  class DisplayDriver {};

  template<typename T>
  void delay(T timeout);

  void ResetDisplay(DisplayDriver &inst);
  void setLcdRS(DisplayDriver &inst);
  void clearLcdRS(DisplayDriver &inst);
  void writeLcdNibble(DisplayDriver &inst, uint8_t val);
  void writeLcdByte(DisplayDriver &inst, uint8_t val);

  void SendSPI(DisplayDriver &inst, const uint8_t *buffer, size_t buffer_len);
};
#endif
