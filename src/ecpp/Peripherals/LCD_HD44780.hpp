/*
 * LCD_HD44780.hpp
 *
 *  Created on: 26.07.2015
 *      Author: andi
 */

#ifndef ECPP_PERIPHERALS_LCD_HD44780_HPP_
#define ECPP_PERIPHERALS_LCD_HD44780_HPP_

#include <ecpp/Target.hpp>
#include <stdint.h>

namespace ecpp {
  namespace Peripherals {
    static constexpr FlashVariable<uint8_t, 5> s_HD44780InitSequence PROGMEM = {0x28, 0x0F, 0x04, 0x02, 0x01};


    template<class BSP>
    class HD44780_MODE_4BIT : public BSP
    {
    protected:
      void delay50us() {BSP::delay(50);}

      uint8_t readNibble()
      {
        uint8_t data;

        BSP::setEnable();
        delay50us();
        data = BSP::getNibble() & 0x0F;
        BSP::clearEnable();

        return data;
      }

      void writeNibble(uint8_t data)
      {
        BSP::setNibble(data);
        BSP::clearEnable();
        delay50us();
        BSP::setEnable();
      }

      uint8_t readByte()
      {
        uint8_t data;

        BSP::setRW();

        data  = readNibble() << 4;
        data |= readNibble();

        return data;
      }

      void writeByte(uint8_t data)
      {
        BSP::clearRW();
        writeNibble(data >> 4);
        writeNibble(data & 0x0F);
      }

      void initBus()
      {
        BSP::clearRW();
        BSP::clearRS();

        writeNibble(0x03);
        BSP::delay(5000);
        writeNibble(0x03);
        BSP::delay(5000);
        writeNibble(0x03);
        writeNibble(0x02);

      }
    };

    template<template<class> class MODE, class BSP>
    class LCD_HD44780 : public MODE<BSP>
    {
    private:
      void prepareData()    {BSP::setRS();}
      void prepareCommand() {BSP::clearRS();}

      template<typename IT>
      void writeBytes(IT begin, IT end)
      {
        while(begin < end)
        {
          MODE<BSP>::writeByte(*(begin++));
        }
      }
    public:
      void writeData(uint8_t data)
      {
        prepareData();
        this->writeByte(data);
      }

      void writeData(uint8_t *ptr, uint8_t len)
      {
        prepareData();
        while(len--)
        {
          this->writeByte(*(ptr++));
        }
      }

      void writeData(char *ptr, uint8_t len)
      {
        prepareData();
        while(len--)
        {
            MODE<BSP>::writeByte(*(ptr++));
        }
      }

      void writeCommand(uint8_t data)
      {
        prepareCommand();
        MODE<BSP>::writeByte(data);
      }

      uint8_t readStatus()
      {
        prepareCommand();
        return MODE<BSP>::readByte();
      }

      void moveCursor(uint8_t pos)
      {
        writeCommand(0x80 | pos);
      }

      void init()
      {
        MODE<BSP>::delay(20000);
        MODE<BSP>::initBus();
        this->writeBytes(s_HD44780InitSequence.begin(), s_HD44780InitSequence.end());
        MODE<BSP>::delay(5000);
      }
    };



  };
};



#endif /* ECPP_PERIPHERALS_LCD_HD44780_HPP_ */
