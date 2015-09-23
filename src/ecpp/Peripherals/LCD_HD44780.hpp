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
    class HD44780_CMD
    {
    protected:
      const uint8_t m_Code;

    public:
      constexpr HD44780_CMD(uint8_t Code) :m_Code(Code) {}
      operator uint8_t () const {return m_Code;}
    };

    static constexpr HD44780_CMD HD44780_CMD_CLEAR(0x01);
    static constexpr HD44780_CMD HD44780_CMD_HOME(0x02);

    class HD44780_CMD_ENTRYMODE : public HD44780_CMD
    {
    public:
      constexpr HD44780_CMD_ENTRYMODE(uint8_t Code) : HD44780_CMD(0x04 | (Code & 0x3)) {}
      constexpr HD44780_CMD_ENTRYMODE operator | (HD44780_CMD_ENTRYMODE rhs) {return HD44780_CMD_ENTRYMODE(m_Code | rhs.m_Code);}
    };

    static constexpr HD44780_CMD_ENTRYMODE HD44780_CMD_ENTRYMODE_INCREMENT(0x00);
    static constexpr HD44780_CMD_ENTRYMODE HD44780_CMD_ENTRYMODE_DECREMENT(0x02);

    static constexpr HD44780_CMD_ENTRYMODE HD44780_CMD_ENTRYMODE_NOSHIFT(0x00);
    static constexpr HD44780_CMD_ENTRYMODE HD44780_CMD_ENTRYMODE_SHIFT(0x01);

    class HD44780_CMD_DISPLAYCONTROL : public HD44780_CMD
    {
    public:
      constexpr HD44780_CMD_DISPLAYCONTROL(uint8_t Code) : HD44780_CMD(0x08 | (Code & 0x7)) {}
      constexpr HD44780_CMD_DISPLAYCONTROL operator | (HD44780_CMD_DISPLAYCONTROL rhs) {return HD44780_CMD_DISPLAYCONTROL(m_Code | rhs.m_Code);}
    };

    static constexpr HD44780_CMD_DISPLAYCONTROL HD44780_CMD_DISPLAYCONTROL_DISPLAYOFF(0x00);
    static constexpr HD44780_CMD_DISPLAYCONTROL HD44780_CMD_DISPLAYCONTROL_DISPLAYON(0x04);
    static constexpr HD44780_CMD_DISPLAYCONTROL HD44780_CMD_DISPLAYCONTROL_CURSOROFF(0x00);
    static constexpr HD44780_CMD_DISPLAYCONTROL HD44780_CMD_DISPLAYCONTROL_CURSORON(0x02);
    static constexpr HD44780_CMD_DISPLAYCONTROL HD44780_CMD_DISPLAYCONTROL_BLINKOFF(0x00);
    static constexpr HD44780_CMD_DISPLAYCONTROL HD44780_CMD_DISPLAYCONTROL_BLINKON(0x02);

    class HD44780_CMD_FUNCTIONSET : public HD44780_CMD
    {
    public:
      constexpr HD44780_CMD_FUNCTIONSET(uint8_t Code) : HD44780_CMD(0x20 | (Code & 0x1F)) {}
      constexpr HD44780_CMD_FUNCTIONSET operator | (HD44780_CMD_FUNCTIONSET rhs) {return HD44780_CMD_FUNCTIONSET(m_Code | rhs.m_Code);}
    };

    static constexpr HD44780_CMD_FUNCTIONSET HD44780_CMD_FUNCTIONSET_4BIT(0x00);
    static constexpr HD44780_CMD_FUNCTIONSET HD44780_CMD_FUNCTIONSET_8BIT(0x10);
    static constexpr HD44780_CMD_FUNCTIONSET HD44780_CMD_FUNCTIONSET_1LINE(0x00);
    static constexpr HD44780_CMD_FUNCTIONSET HD44780_CMD_FUNCTIONSET_2LINE(0x08);

    static constexpr HD44780_CMD_FUNCTIONSET HD44780_CMD_FUNCTIONSET_5x7FONT(0x00);
    static constexpr HD44780_CMD_FUNCTIONSET HD44780_CMD_FUNCTIONSET_5x10FONT(0x04);

    template<class BSP>
    class HD44780_MODE_4BIT : public BSP
    {
    protected:
      void delay50us() {BSP::delay(50);}
      void delay1us() {BSP::delay(1);}

      uint8_t readNibble();
      void writeNibble(uint8_t data);
      uint8_t readByte();
      void writeByte(uint8_t data);
      void initBus();
    };

    template<class BSP>
    uint8_t HD44780_MODE_4BIT<BSP>::readNibble()
    {
      uint8_t data;

      BSP::setEnable();
      delay1us();
      data = BSP::getNibble() & 0x0F;
      BSP::clearEnable();
      delay1us();
      return data;
    }

    template<class BSP>
    void HD44780_MODE_4BIT<BSP>::writeNibble(uint8_t data)
    {
      BSP::setNibble(data);
      BSP::clearEnable();
      delay1us();
      BSP::setEnable();
      delay1us();
    }

    template<class BSP>
    uint8_t HD44780_MODE_4BIT<BSP>::readByte()
    {
      uint8_t data;

      BSP::setRW();

      data  = readNibble() << 4;
      data |= readNibble();
      delay50us();

      return data;
    }

    template<class BSP>
    void HD44780_MODE_4BIT<BSP>::writeByte(uint8_t data)
    {
      BSP::clearRW();

      writeNibble((data >> 4) & 0x0F);
      writeNibble(data & 0x0F);
      delay50us();
    }

    template<class BSP>
    void HD44780_MODE_4BIT<BSP>::initBus()
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

    template<template<class> class MODE, class BSP, int ROWS = 2, int COLUMNS = 16>
    class LCD_HD44780 : public MODE<BSP>
    {
    public:
      typedef char RowBufferType[COLUMNS];
    private:
      RowBufferType m_RowBuffer;

      void prepareData()    {BSP::setRS();}
      void prepareCommand() {BSP::clearRS();}

      template<typename IT>
      void writeBytes(IT begin, IT end);

    public:
      constexpr HD44780_CMD Location(uint_fast8_t Column, uint_fast8_t Row) { return 0x80 | (Column + Row * 0x40);}

      RowBufferType & getBuffer() {return m_RowBuffer;}

      void writeBuffer(uint_fast8_t length)
      {
        writeBytes(&m_RowBuffer[0], &m_RowBuffer[length]);
      }

      void writeTextBuffer(uint_fast8_t length)
      {
        prepareData();
        writeBuffer(length);
      }

      void writeData(uint8_t data)
      {
        prepareData();
        this->writeByte(data);
      }

      void writeData(const void *p, uint8_t len)
      {
        const uint8_t *q = (const uint8_t*)p;

        prepareData();
        while(len--)
        {
          MODE<BSP>::writeByte(*(q++));
        }
      }


      void writeCommand(uint8_t data)
      {
        prepareCommand();
        MODE<BSP>::writeByte(data);
        this->delay50us();
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
        MODE<BSP>::InitSequence.read(m_RowBuffer);
        writeBuffer(sizeof(MODE<BSP>::InitSequence));
        MODE<BSP>::delay(5000);
      }

      void clearDisplay() {writeCommand(HD44780_CMD_CLEAR);}

      template<typename IT>
      void displayString(const HD44780_CMD loc, IT begin, IT end);

    };

    template<template<class> class MODE, class BSP, int ROWS, int COLUMNS>
    template<typename IT>
    void LCD_HD44780<MODE,BSP,ROWS,COLUMNS>::writeBytes(IT begin, IT end)
    {
      while(begin < end)
      {
        MODE<BSP>::writeByte(*(begin++));
      }
    }

    template<template<class> class MODE, class BSP, int ROWS, int COLUMNS>
    template<typename IT>
    void LCD_HD44780<MODE,BSP,ROWS,COLUMNS>::displayString(const HD44780_CMD loc, IT begin, IT end)
    {
      writeCommand(loc);
      prepareData();
      while(begin < end)
      {
        MODE<BSP>::writeByte(*(begin++));
      }
    }

  };

};



#endif /* ECPP_PERIPHERALS_LCD_HD44780_HPP_ */
