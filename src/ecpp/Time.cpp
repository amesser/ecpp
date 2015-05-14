/*
 * Time.cpp
 *
 *  Created on: 13.05.2015
 *      Author: andi
 */


#include "ecpp/Time.hpp"

namespace ecpp
{
  uint32_t Time::calculateSecondsOfDay(uint8_t Hour, uint8_t Minute, uint8_t Second)
  {
    return (uint32_t)((uint16_t)Hour * 60 + Minute) * 60 + Second;
  }
}

