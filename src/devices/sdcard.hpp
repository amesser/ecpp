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
#ifndef SDCARD_H_
#define SDCARD_H_

#include <stdint.h>


namespace Platform {
  namespace Devices {

  template<typename DEVICE>
  class SDCard : public DEVICE {
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

    enum {
      STATE_INVALID = 0,
      STATE_INIT,
      STATE_IDLE,
      STATE_READ,
      STATE_WRITE,
    } State;

    uint16_t  BlockSize;
    uint16_t  BlockIndex;

    static constexpr uint8_t u8ResponseTimeout = 8;
  public:
    bool getError() const {
      return STATE_INVALID == State;
    }

    uint16_t getBlockSize() const {
      return BlockSize;
    }

    auto getDefaultTransport () -> decltype(this->getTransport(0)) {
      return this->getTransport(25000000);
    }

    template<typename CONTEXT>
    uint8_t initCard(CONTEXT &ctx) {
      BlockSize    = 0;
      BlockIndex   = 0;

      State      = STATE_INIT;

      auto transport = this->getSlowTransport();

      this->deselectChip();
      this->disablePowerSupply();
      this->enablePowerSupply();
      this->selectChip();
      transport.sendIdle((uint8_t)((74 +7) / 8));
      this->deselectChip();
      transport.sendIdle((uint8_t)(2));

      this->selectChip();

      commandR1(transport,0,0);

      uint8_t response;
      const uint16_t timeout = 1024 + ctx.u16getClock1ms();
      do {
        const uint8_t mask = 0x80;

        if (mask & commandR1(transport,55)) break;

        response = commandR1(transport,41);

        if (mask & response)
          break;
        else if (response == 0)
          break;
      } while (static_cast<int16_t>(ctx.u16getClock1ms() - timeout) <= 0);

      if (response != 0) State = STATE_INVALID;

      if (State == STATE_INIT)
      {
        auto transport = this->getTransport(25000000);

        sendCommand(transport,9,0);
        if (0 == transport.waitToken(0,0x80, u8ResponseTimeout))
        {
          State      = STATE_READ;
          BlockSize  = 16;
          BlockIndex = 0;

          uint8_t READ_BL_SIZE = 0;

          for(uint8_t cnt = 0; cnt < 16; cnt++)
          {
            if (cnt == 5)
              READ_BL_SIZE = readByte() & 0x0F;
            else
              readByte();
          }

          transport.transferByte(0xFF);
          State = STATE_INIT;

          BlockSize = 0x0001 << READ_BL_SIZE;
        }
        else
          State = STATE_INVALID;
      }

      this->deselectChip();
      transport.transferByte(0xFF);

      if (State == STATE_INIT) {
        State = STATE_IDLE;
        return 0;
      }
      else {
        this->disablePowerSupply();
        return 1;
      }
    };

    template<typename TRANSPORT>
    static void waitBusy(TRANSPORT& transport) {
      while (0 == transport.transferByte(0xFF));
    }

    template<typename TRANSPORT>
    static void sendCommand(TRANSPORT& transport, uint8_t command, uint32_t argument) {
      transport.transferByte(0x40 | command);
      transport.transferByte((argument >> 24) & 0xFF);
      transport.transferByte((argument >> 16) & 0xFF);
      transport.transferByte((argument >>  8) & 0xFF);
      transport.transferByte((argument >>  0) & 0xFF);
      transport.transferByte(0x95);
    }

    template<typename TRANSPORT>
    uint8_t commandR1(TRANSPORT& transport, uint8_t command, uint32_t argument = 0) {
      sendCommand(transport,command,argument);

      uint8_t response = transport.waitToken(0,0x80, u8ResponseTimeout);
      transport.transferByte(0xFF); // Make 8 extra Clocks

      if (response == 0xFF)
        State = STATE_INVALID;

      return response;
    }

    void setError() {
      this->deselectChip();
      this->getSlowTransport().transferByte(0xFF);

      State = STATE_INVALID;
    }

    void stopMultipleRead()
    {
      if ( STATE_READ != State)
        return;

      auto transport = this->getDefaultTransport();

      sendCommand(transport,12,0);
      transport.transferByte(0xFF);

      if(transport.waitToken(0,0x80, u8ResponseTimeout) & 0x80)
      {
        State = STATE_INVALID;
      }
      else
      {
        waitBusy(transport);
        State = STATE_IDLE;
      }

      this->deselectChip();
      transport.transferByte(0xFF);
    }

    void stopMultipleWrite()
    {
      if ( STATE_WRITE != State)
        return;

      auto transport = this->getDefaultTransport();

      while (BlockIndex < BlockSize)
      {
        this->writeByte(0xFF);
        waitBusy(transport);
      }

      transport.transferByte(0xFD); // Stop Tran
      transport.transferByte(0xFF);
      waitBusy(transport);

      State = STATE_IDLE;

      this->deselectChip();
      transport.transferByte(0xFF);
    }

    int8_t startMultipleRead(uint32_t addr) {
      if (STATE_IDLE != State)
        return -1;

      auto transport = this->getDefaultTransport();

      this->selectChip();

      sendCommand(transport,18, addr);

      if (0 != transport.waitToken(0,0x80, u8ResponseTimeout))
      {
        setError();
        return -2;
      }
      else
      {
        State = STATE_READ;
        BlockIndex = 0;
        return 0;
      }
    }

    int8_t startMultipleWrite(uint32_t addr) {
      if (STATE_IDLE != State)
        return -1;

      auto transport = this->getDefaultTransport();

      this->selectChip();

      sendCommand(transport,25, addr);

      if (0 != transport.waitToken(0,0x80, u8ResponseTimeout))
      {
        setError();
        return -2;
      }
      else
      {
        transport.transferByte(0xFF);
        State = STATE_WRITE;
        BlockIndex = 0;
        return 0;
      }
    }

    uint8_t readByte() {
      if (STATE_READ != State)
        return 0xFF;

      auto transport = this->getDefaultTransport();

      if (0 == BlockIndex)
      {
        if (transport.waitToken(0xFE, 0xFF, (uint8_t)255) != 0xFE)
        {
          setError();
          return 0xFF;
        }
      }

      const uint8_t data = transport.transferByte(0xFF);
      BlockIndex++;

      if(BlockIndex == BlockSize)
      {
        transport.sendIdle((uint8_t)2); /* CRC */
        BlockIndex = 0;
      }

      return data;
    }

    int8_t writeByte(uint8_t data) {
      if (STATE_WRITE != State)
        return -1;

      auto transport = this->getDefaultTransport();

      if ( BlockSize == BlockIndex)
      {
        if (0 == transport.transferByte(0xFF))
          return 0;

        BlockIndex = 0;
      }

      if (0 == BlockIndex)
        transport.transferByte(0xFC);

      transport.transferByte(data);
      BlockIndex++;

      if (BlockSize == BlockIndex)
      {
         transport.sendIdle(2); // CRC

         if ((transport.transferByte(0xFF) & 0x1F) != 5)
         {
           setError();
           return -2;
         }

         transport.transferByte(0xFF); // Busy byte
      }

      return 1;
    }

    template<typename SIZET>
    SIZET read(uint8_t* buf, SIZET length)
    {
      SIZET actual = 0;

      while(actual < length)
      {
         *buf = readByte();

         if (STATE_READ != State)
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

      if (STATE_WRITE == State)
        return actual;
      else
        return -1;
    }

    };
  }
}


#endif /* SDCARD_H_ */
