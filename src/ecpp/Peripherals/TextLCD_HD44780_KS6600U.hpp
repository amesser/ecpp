/*
 * LCD_HD44780.hpp
 *
 *  Created on: 26.07.2015
 *      Author: andi
 */

#ifndef ECPP_PERIPHERALS_LCD_HD44780_HPP_
#define ECPP_PERIPHERALS_LCD_HD44780_HPP_

#include <ecpp/Target.hpp>
#include <ecpp/Graphics/TextFramebuffer.hpp>
#include <stdint.h>

namespace ecpp {
  namespace Peripherals {
    template<class SUBDRIVER>
    class TextLCD_HD44780_KS0066U;

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

    static constexpr HD44780_CMD_ENTRYMODE KS0066U_CMD_ENTRYMODE_INCREMENT(0x02);
    static constexpr HD44780_CMD_ENTRYMODE KS0066U_CMD_ENTRYMODE_DECREMENT(0x00);

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


    template<class BSP, int COLUMNS, int ROWS>
    class HD44780_MODE_4BIT {};

    template<class BSP, int COLUMNS>
    class HD44780_MODE_4BIT<BSP, COLUMNS,2> : public BSP
    {
    public:
      static constexpr int getNumColumns() { return COLUMNS;}
      static constexpr int getNumRows()    { return 2;}

      void init()
      {
        /* HD44780 : 15/40ms
         */
        BSP::delayMicroseconds(20000);
        BSP::delayMicroseconds(20000);

        /* request initialization of display
         * switch to 8 bit */
        BSP::writeCommandNibble(0x30);
        BSP::delayMicroseconds(4100);
        BSP::writeCommandNibble(0x30);
        BSP::delayMicroseconds(100);
        BSP::writeCommandNibble(0x30);

        /* switch to 4 bit */
        BSP::writeCommandNibble(0x20);

        /* write initial commands to display *
         * this sequence is required to properly initialize the controller it must not be changed! */
        BSP::writeCommand(HD44780_CMD_FUNCTIONSET_4BIT | HD44780_CMD_FUNCTIONSET_2LINE | HD44780_CMD_FUNCTIONSET_5x7FONT);
        BSP::writeCommand(HD44780_CMD_DISPLAYCONTROL_DISPLAYOFF);
        BSP::writeCommand(HD44780_CMD_CLEAR);
        BSP::writeCommand(HD44780_CMD_ENTRYMODE_INCREMENT | HD44780_CMD_ENTRYMODE_NOSHIFT);

        /* enable the display */
        BSP::writeCommand(HD44780_CMD_DISPLAYCONTROL_DISPLAYON);
      }
    };


    template<class BSP, int COLUMNS, int ROWS>
    class KS0066U_MODE_4BIT {};

    template<class BSP, int COLUMNS>
    class KS0066U_MODE_4BIT<BSP, COLUMNS,2> : public BSP
    {
    public:
      static constexpr int getNumColumns() { return COLUMNS;}
      static constexpr int getNumRows()    { return 2;}

      void init()
      {
        /* KS0066U : 30 ms
         */
        BSP::delayMicroseconds(30000);

        /* request initialization of display
         * switch to 8 bit */
        BSP::writeCommandNibble(0x30);
        BSP::delayMicroseconds(4100);
        BSP::writeCommandNibble(0x30);
        BSP::delayMicroseconds(100);
        BSP::writeCommandNibble(0x30);

        /* switch to 4 bit */
        BSP::writeCommandNibble(0x20);

        /* write initial commands to display *
         * this sequence is required to properly initialize the controller it must not be changed! */
        BSP::writeCommand(HD44780_CMD_FUNCTIONSET_4BIT | HD44780_CMD_FUNCTIONSET_2LINE | HD44780_CMD_FUNCTIONSET_5x7FONT);
        BSP::writeCommand(HD44780_CMD_DISPLAYCONTROL_DISPLAYOFF);
        BSP::writeCommand(HD44780_CMD_CLEAR);
        BSP::writeCommand(KS0066U_CMD_ENTRYMODE_INCREMENT | HD44780_CMD_ENTRYMODE_NOSHIFT);

        /* enable the display */
        BSP::writeCommand(HD44780_CMD_DISPLAYCONTROL_DISPLAYON);
      }
    };

    template<int COLUMNS, int ROWS>
    class TextLcdCursor
    {
    private:
      uint8_t m_Value;

      static constexpr uint8_t calculateValue(uint_fast8_t Column, uint_fast8_t Row)
      {
        return Column + Row * 0x40;
      }

      template<int C, int R>
      static constexpr uint8_t calculateValue(const TextCoordinate<C,R> Cursor);

      TextLcdCursor(uint8_t Raw) : m_Value (Raw) {};
    public:
      TextLcdCursor() {};

      template<typename CTYPE>
      constexpr TextLcdCursor(const CTYPE & Coordinate)       : m_Value(calculateValue(Coordinate)) {};

      constexpr TextLcdCursor(uint8_t Column, uint8_t Row)    : m_Value (calculateValue(Column,Row)) {};
      constexpr TextLcdCursor(const TextLcdCursor & Init) : m_Value (Init.m_Value) {};

      constexpr HD44780_CMD asCommand() const { return HD44780_CMD(0x80 | m_Value); }

      TextLcdCursor operator ++ (int)
      {
        return TextLcdCursor(m_Value++);
      }

      bool operator != (const TextLcdCursor & Rhs) const {return m_Value != Rhs.m_Value;}

      template<typename CTYPE>
      bool operator != (const CTYPE & Rhs) const
      {
        return m_Value != calculateValue(Rhs);
      }
    };

    template<>
    template<>
    constexpr uint8_t TextLcdCursor<16, 2>::calculateValue(const TextCoordinate<16,2> Cursor)
    {
      return (Cursor.asIndex() + 0x30) & 0x4F;
    }

    template<class SUBDRIVER>
    class TextLCD_HD44780_KS0066U : public SUBDRIVER
    {
    public:
      typedef TextLcdCursor<SUBDRIVER::getNumColumns(),SUBDRIVER::getNumRows()> LocationType;

    private:
      typedef SUBDRIVER LcdType;

      LocationType  m_CurrentLocation;

      template<typename IT>
      void writeBytes(IT begin, IT end);

    public:
      LocationType getCursorLocation() const {return m_CurrentLocation;}
      static LocationType getLocation(uint8_t Column, uint8_t Row) {return LocationType(Column, Row);}

      void putCharacter(char c)
      {
        m_CurrentLocation++;
        LcdType::writeRam(c);
      }

      const LocationType & getCursor() const
      {
        return m_CurrentLocation;
      }

      void moveCursor(LocationType Coordinate)
      {
        m_CurrentLocation = Coordinate;
        LcdType::writeCommand(m_CurrentLocation.asCommand());
      }

      void clearDisplay()
      {
        LcdType::writeCommand(HD44780_CMD_CLEAR);
      }

      template<typename IT>
      void displayString(const HD44780_CMD loc, IT begin, IT end);

    };

    template<class SUBDRIVER>
    template<typename IT>
    void TextLCD_HD44780_KS0066U<SUBDRIVER>::writeBytes(IT begin, IT end)
    {
      while(begin < end)
      {
        putCharacter(*(begin++));
      }
    }

    template<class SUBDRIVER>
    template<typename IT>
    void TextLCD_HD44780_KS0066U<SUBDRIVER>::displayString(const HD44780_CMD loc, IT begin, IT end)
    {
      moveCursor(loc);

      while(begin < end)
      {
        putCharacter(*(begin++));
      }
    }

  };

};



#endif /* ECPP_PERIPHERALS_LCD_HD44780_HPP_ */
