/*
 *  Copyright 2015 Andreas Messer <andi@bastelmap.de>
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
#ifndef ECPP_TARGET_AVR8_TWI_HPP_
#define ECPP_TARGET_AVR8_TWI_HPP_

#include <ecpp/Arch/AVR8.hpp>
#include <ecpp/Datatypes.hpp>

//#define TWCR

#if defined (TWCR)
namespace ecpp
{
  class TWIMaster
  {
  public:
    enum State
    {
      STATUS_OK   =  0,
      STATUS_NACK = -1,
      STATUS_ERR  = -2,
      STATUS_BUSY = -3,
    };

  private:

    uint8_t*   m_pBuffer;
    uint8_t    m_Len;
    uint8_t    m_AddressDir;

    void sendStartAndTransfer(uint_fast8_t AddressDir, uint8_t *pbBuffer, uint_fast8_t Len);

  public:
    void init(uint32_t Frequency)
    {
      /* TWBR * 4^TWPS */
      uint16_t divider   = (F_CPU / Frequency - 16) / 2;
      uint8_t  prescaler = 0;

      while(divider > 255 && prescaler < 3)
      {
        divider    = divider / 4;
        prescaler += 1;
      }

      TWCR = 0;
      TWBR = divider;
      TWSR = prescaler;
      TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN) /* | _BV(TWIE) */;
    }

    void handleCyclic();
    void close()
    {
      TWCR      = 0;
      m_pBuffer = 0;
    }

    enum State getStatus() const {return m_pBuffer ? STATUS_BUSY : (enum State)m_AddressDir;}

    void sendStartAndRead(uint_fast8_t SlaveAddress, uint8_t *pbBuffer, uint_fast8_t Len)
    {
      sendStartAndTransfer((SlaveAddress << 1) | 0x01, pbBuffer, Len);
    }

    void sendStartAndWrite(uint_fast8_t SlaveAddress, uint8_t *pbBuffer, uint_fast8_t Len)
    {
      sendStartAndTransfer((SlaveAddress << 1), pbBuffer, Len);
    }

    void continuedRead(uint8_t *pbBuffer, uint_fast8_t Len);

    void continuedWrite(uint8_t *pbBuffer, uint_fast8_t Len);

    void sendStop();
  };
};
#endif
#endif /* ECPP_TARGET_AVR8_TWI_HPP_ */
