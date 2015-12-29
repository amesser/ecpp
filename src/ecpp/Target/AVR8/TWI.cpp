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
#include "ecpp/Target/AVR8/TWI.hpp"

using namespace ecpp;

#if defined(TWCR)
static constexpr uint8_t mapTWIStatus(uint8_t RegisterValue)
{
  return (RegisterValue & 0xF8) >> 3;
}

static uint8_t getTWIStatus()
{
  return mapTWIStatus(TWSR);
}


void TWIMaster::sendStartAndTransfer(uint_fast8_t AddressDir, uint8_t *pbBuffer, uint_fast8_t Len)
{
  if (0 == Len)
  { /* make a dummy pointer if no data shall be transferred to triger at least start + addressdir */
    pbBuffer = &m_Len;
  }

  if(STATUS_BUSY != getStatus())
  {
    m_AddressDir = AddressDir;
    m_Len        = Len;
    m_pBuffer    = pbBuffer;

    TWCR = _BV(TWSTA) | _BV(TWEN) | _BV(TWINT);
  }
}

void TWIMaster::continuedRead(uint8_t *pbBuffer, uint_fast8_t Len)
{
  if(Len > 0 && (STATUS_BUSY != getStatus()))
  {
    uint8_t TWStatus = getTWIStatus();

    if( TWStatus == mapTWIStatus(0x50)) /* read + ack */
    {
      m_Len        = Len;
      m_pBuffer    = pbBuffer;

      TWCR = _BV(TWEN) | _BV(TWINT);
    }
    else if(TWStatus == mapTWIStatus(0x40)) /* sla+r + ack */
    {
      m_Len        = Len;
      m_pBuffer    = pbBuffer;

      TWCR = _BV(TWEN) | _BV(TWINT);
    }
  }
}

void TWIMaster::continuedWrite(uint8_t *pbBuffer, uint_fast8_t Len)
{
  if(Len > 0 && (STATUS_BUSY != getStatus()))
  {
    uint8_t TWStatus = getTWIStatus();

    if( TWStatus == mapTWIStatus(0x28)) /* write + ack */
    {
      m_Len        = Len + 1; /* compensate for switch case */
      m_pBuffer    = pbBuffer;
    }
    else if (TWStatus == mapTWIStatus(0x18))   /* sla+w + ack */
    {
      m_Len        = Len;
      m_pBuffer    = pbBuffer;
    }
  }
}

void TWIMaster::sendStop()
{
  if(STATUS_BUSY != getStatus())
  {
    m_pBuffer = &m_AddressDir;
    TWCR = _BV(TWSTO) | _BV(TWEN) | _BV(TWINT);
  }
}

void TWIMaster::handleCyclic()
{
  uint8_t *pBuffer = m_pBuffer;

  if(pBuffer == &(m_AddressDir))
  { /* this means a stop was requested */
    if (0 == (TWCR & _BV(TWSTO)))
    {
      m_AddressDir = STATUS_OK;
      m_pBuffer = 0;
    }
  }
  else if(0 != pBuffer)
  {
    if(TWCR & _BV(TWINT))
    {
      uint8_t TWStatus = getTWIStatus();

      switch(TWStatus)
      {
        case mapTWIStatus(0x08): /* start condition */
        case mapTWIStatus(0x10): /* repeated start */
          TWDR = m_AddressDir;
          TWCR = _BV(TWEN) | _BV(TWINT);
          break;

        case mapTWIStatus(0x28): /* write + ack  */
          m_Len -= 1;
          /* fall through */
        case mapTWIStatus(0x18): /* sla+w + ack */
          if(m_Len > 0)
          {
            TWDR      = *pBuffer;
            TWCR = _BV(TWEN) | _BV(TWINT);
            m_pBuffer = pBuffer + 1;
          }
          else
          {
            m_AddressDir = STATUS_OK;
            m_pBuffer = 0;
          }
          break;
        case mapTWIStatus(0x20): /* sla+w + nack */
        case mapTWIStatus(0x30): /* write + nack */
          m_AddressDir = STATUS_NACK;
          m_pBuffer    = 0;
          break;

        case mapTWIStatus(0x50): /* read  + ack return */
          *pBuffer  = TWDR;
          m_pBuffer = pBuffer + 1;
          m_Len -= 1;
          /* fall through */
        case mapTWIStatus(0x40): /* sla+r + ack */
          if(m_Len > 0)
          {
            TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);
          }
          else
          {
            m_AddressDir = STATUS_OK;
            m_pBuffer = 0;
          }
          break;
        case mapTWIStatus(0x48): /* sla+r + nack, */
        case mapTWIStatus(0x58): /* read  + nack return */
          m_AddressDir = STATUS_NACK;
          m_pBuffer = 0;
          break;
        default:
          m_AddressDir = STATUS_ERR;
          m_pBuffer = 0;
          break;
      }
    }
  }
}
#endif
