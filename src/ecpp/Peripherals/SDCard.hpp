/*
 *  Copyright 2013 Andreas Messer <andi@bastelmap.de>
 *
 *  This file is part of the Embedded C++ Platform Project.
 *
 *  Embedded C++ Platform Project (ECPP) is free software: you can
 *  redistribute it and/or modify it under the terms of the GNU General
 *  Public License as published by the Free Software Foundation,
 *  either version 3 of the License, or (at your option) any later
 *  version.
 *
 *  Embedded C++ Platform Project is distributed in the hope that it
 *  will be useful, but WITHOUT ANY WARRANTY; without even the implied
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ECPP.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  As a special exception, the copyright holders of ECPP give you
 *  permission to link ECPP with independent modules to produce an
 *  executable, regardless of the license terms of these independent
 *  modules, and to copy and distribute the resulting executable under
 *  terms of your choice, provided that you also meet, for each linked
 *  independent module, the terms and conditions of the license of that
 *  module.  An independent module is a module which is not derived from
 *  or based on ECPP.  If you modify ECPP, you may extend this exception
 *  to your version of ECPP, but you are not obligated to do so.  If you
 *  do not wish to do so, delete this exception statement from your
 *  version.
 *  */
#ifndef ECPP_PERIPHERALS_SDCARD_H_
#define ECPP_PERIPHERALS_SDCARD_H_

#include <stdint.h>


namespace ecpp {
  namespace Peripherals {

  template<typename DEVICE>
  class SDCard : public DEVICE {
  public:
    enum SDCardState{
      SD_CLOSED,
      SD_W_POWERUP,
      SD_W_INIT,
      SD_IDENTIFY,
      SD_IDLE,
      SD_READSINGLE,
      SD_READMULTIPLE,
      SD_WRITESINGLE,
      SD_WRITEMULTIPLE,
      SD_ERROR,
      SD_W_POWERDOWN,
    };
  private:
    enum {
      R1_IDLE            = 0x01,
      R1_ERASE_RESET     = 0x02,
      R1_ILLEGAL_COMMAND = 0x04,
      R1_CRC_ERROR       = 0x08,
      R1_ERASE_SEQ_ERROR = 0x10,
      R1_ADDR_ERROR      = 0x20,
      R1_PARAMETER_ERROR = 0x40,
    };


    uint8_t   m_state;
    uint16_t  BlockSize;
    uint16_t  BlockIndex;

    void             setState(enum SDCardState state) {m_state = state;}

    static constexpr uint8_t u8ResponseTimeout = 8;

  public:
    enum SDCardState getState() const {return static_cast<enum SDCardState>(m_state);}

    bool getError() const {
      return SD_ERROR <= getState();
    }

    uint16_t getBlockSize()  const { return BlockSize; }
    uint16_t getBlockIndex() const { return BlockIndex; }

    int8_t activate()
    {
      auto state = getState();
      int8_t result;

      if (state == SD_CLOSED)
      {
        this->deselectChip();
        this->enablePower();
        this->startTimer1ms(5000);

        setState(SD_W_POWERUP);
        result = 0;
      }
      else
      {
        result = -1;
      }

      return result;
    }

    void close()
    {
      if (SD_IDLE == getState())
      {
        powerdown();
      }
    }

    void identifyCard()
    {
      auto state = getState();

      if(state == SD_IDLE)
      {
        this->selectChip();
        sendCommand(9,0);

        if (0 == this->waitToken(0,0x80, u8ResponseTimeout))
        {
          state      = SD_READMULTIPLE;
          setState(state);

          BlockSize  = 16;
          BlockIndex = 0;

          uint8_t READ_BL_SIZE = 0;

          for(uint8_t cnt = 0; cnt < 16; cnt++)
          {
            uint8_t val = readByte();

            if (cnt == 5)
              READ_BL_SIZE = val & 0x0F;
          }

          this->transferByte(0xFF);
          state = SD_IDLE;

          BlockSize = 0x0001 << READ_BL_SIZE;
        }
        else
        {
          state = SD_ERROR;
        }

        this->deselectChip();
        this->transferByte(0xFF);

        setState(state);
      }
    }

    int8_t setBlockLen(uint16_t length)
    {
      auto state = getState();
      int8_t result = -1;

      if(state == SD_IDLE)
      {
        uint8_t response;

        this->selectChip();
        response = commandR1(16, length);
        this->deselectChip();
        this->transferByte(0xFF);
      }

      identifyCard();

      if(BlockSize != length)
        result = -1;

      return result;
    }

    void handleSDCard()
    {
      auto state = getState();

      if(state == SD_W_POWERUP)
      {
        if(this->checkTimerExpired())
        {
          state = SD_W_INIT;

          this->setSlowSpiSpeed();

          this->deselectChip();
          this->sendIdle((uint8_t)((74 +7) / 8));
          this->selectChip();

          commandR1(0,0);

          this->startTimer1ms(10000);
        }
      }

      if(state == SD_W_INIT)
      {
        uint8_t result;

        result = commandR1(55);

        if((result & 0x80) == 0)
        {
          result = commandR1(41);
        }


        if (result == 0)
        {
          state = SD_IDENTIFY;
          this->setFullSpiSpeed();
        }
        else if((result & 0x80))
        {
          state = SD_ERROR;
        }
        else if (this->checkTimerExpired())
        {
          state = SD_ERROR;
        }
      }


      if(state == SD_IDENTIFY)
      {
        setState(SD_IDLE);
        identifyCard();
        state = getState();
      }


      if(state == SD_WRITESINGLE)
      {
        if(BlockIndex >= BlockSize)
        {
          uint8_t busy = this->transferByte(0xFF);

          if(busy == 0xFF)
          {
            state = SD_IDLE;
            this->deselectChip();
          }
        }
      }


      if(state == SD_W_POWERDOWN)
      {
        if(this->checkTimerExpired())
        {
          state = SD_CLOSED;
        }
      }

      setState(state);

      if (state == SD_ERROR)
        powerdown();
    }

    void waitBusy()
    {
      while (0 == this->transferByte(0xFF));
    }

    void sendCommand(uint8_t command, uint32_t argument)
    {
      this->transferByte(0x40 | command);
      this->transferByte((argument >> 24) & 0xFF);
      this->transferByte((argument >> 16) & 0xFF);
      this->transferByte((argument >>  8) & 0xFF);
      this->transferByte((argument >>  0) & 0xFF);
      this->transferByte(0x95);
    }

    uint8_t commandR1(uint8_t command, uint32_t argument = 0)
    {
      this->selectChip();
      this->transferByte(0xFF);
      sendCommand(command,argument);

      uint8_t response = this->waitToken(0,0x80, u8ResponseTimeout);
      this->transferByte(0xFF); /* one additional transfer */

      this->deselectChip();
      this->transferByte(0xFF);

      return response;
    }

    void powerdown()
    {
      if(getState() < SD_W_POWERDOWN)
      {
        this->deselectChip();
        this->transferByte(0xFF);

        setState(SD_W_POWERDOWN);
        this->startTimer1ms(5000);

        this->disablePower();
      }
    }

    void stopMultipleRead()
    {
      if (SD_READMULTIPLE == getState())
      {
        sendCommand(12,0);
        this->transferByte(0xFF);

        if(this->waitToken(0,0x80, u8ResponseTimeout) & 0x80)
        {
          setState(SD_ERROR);
        }
        else
        {
          waitBusy();
          setState(SD_IDLE);
        }

        this->deselectChip();
        this->transferByte(0xFF);
      }
    }

    void stopMultipleWrite()
    {
      if ( SD_WRITEMULTIPLE == getState())
      {
        while (BlockIndex < BlockSize)
        {
          this->writeByte(0xFF);
          waitBusy();
        }

        this->transferByte(0xFD); // Stop Tran
        this->transferByte(0xFF);
        waitBusy();

        setState(SD_IDLE);

        this->deselectChip();
        this->transferByte(0xFF);
      }
    }

    int8_t startMultipleRead(uint32_t addr)
    {
      int8_t result = -1;

      if (SD_IDLE == getState())
      {
        this->selectChip();
        sendCommand(18, addr);

        if (0 != this->waitToken(0,0x80, u8ResponseTimeout))
        {
          setState(SD_ERROR);
          result = -2;
        }
        else
        {
          setState(SD_READMULTIPLE);
          BlockIndex = 0;
          result = 0;
        }
      }

      return result;
    }

    int8_t startWriteBlock(uint32_t addr)
    {
      int8_t result = -1;

      if (SD_IDLE == getState())
      {
        this->selectChip();
        sendCommand(24, addr);

        if (0 != this->waitToken(0,0x80, u8ResponseTimeout))
        {
          setState(SD_ERROR);
          result = -2;
        }
        else
        {
          setState(SD_WRITESINGLE);
          this->transferByte(0xFF);
          BlockIndex = 0;
          result = 0;
        }
      }

      return result;
    }

    int8_t startMultipleWrite(uint32_t addr)
    {
      int8_t result = -1;

      if (SD_IDLE == getState())
      {
        this->selectChip();
        sendCommand(25, addr);

        if (0 != this->waitToken(0,0x80, u8ResponseTimeout))
        {
          setState(SD_ERROR);
          result = -2;
        }
        else
        {
          setState(SD_WRITEMULTIPLE);
          this->transferByte(0xFF);
          BlockIndex = 0;
          result = 0;
        }
      }

      return result;
    }

    uint8_t readByte()
    {
      uint8_t byte = 0xFF;

      if (SD_READMULTIPLE == getState())
      {
        if (0 == BlockIndex)
        {
          if (this->waitToken(0xFE, 0xFF, (uint8_t)255) != 0xFE)
          {
            BlockIndex = BlockSize;
            setState(SD_ERROR);
          }
        }

        if(BlockIndex < BlockSize)
        {
          byte = this->transferByte(0xFF);
          BlockIndex++;

          if(BlockIndex == BlockSize)
          {
            this->sendIdle((uint8_t)2); /* CRC */
            BlockIndex = 0;
          }
        }
      }

      return byte;
    }

    int8_t writeByte(uint8_t data)
    {
      int8_t result = -1;
      auto state = getState();

      if (SD_WRITEMULTIPLE == state)
      {
        if ( BlockSize == BlockIndex)
        {
          if (0 == this->transferByte(0xFF)) /* busy wait */
            result = 0;
          else
            BlockIndex = 0;
        }

        if (0 == BlockIndex)
        {
          this->transferByte(0xFC);
        }
      }

      if (SD_WRITESINGLE == state)
      {
        if (0 == BlockIndex)
        {
          this->transferByte(0xFE);
        }
      }

      if (SD_WRITEMULTIPLE == state ||
          SD_WRITESINGLE   == state)
      {
        if(BlockIndex < BlockSize)
        {
          result = 1;

          this->transferByte(data);
          BlockIndex++;

          if (BlockSize == BlockIndex)
          {
             this->sendIdle(2); // CRC

             /* data response */
             if ((this->transferByte(0xFF) & 0x1F) != 5)
             {
               setState(SD_ERROR);
               result = -2;
             }

             this->transferByte(0xFF); // Busy byte
             this->transferByte(0xFF);

          }
        }
        else
        {
          result = 0;
        }
      }

      return result;
    }

    template<typename SIZET>
    SIZET read(uint8_t* buf, SIZET length)
    {
      SIZET actual = 0;

      while(actual < length)
      {
         *buf = readByte();

         if (SD_READMULTIPLE != getState())
           break;

         ++actual;
         ++buf;
      }

      if (actual > 0)
        return actual;
      else
        return -1;
    }

    template<typename SIZET>
    SIZET write(const uint8_t* buf, SIZET length)
    {
      SIZET actual = 0;

      while(actual < length) {
          int8_t result = writeByte(*buf);

          if (result == 0)
            continue;
          else if (result < 0)
            break;

          ++actual;
          ++buf;
      }

      if (SD_WRITEMULTIPLE == getState())
        return actual;
      else
        return -1;
    }

    };
  }
}


#endif /* SDCARD_H_ */
