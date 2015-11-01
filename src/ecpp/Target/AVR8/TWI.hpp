/*
 * TWI.hpp
 *
 *  Created on: 04.10.2015
 *      Author: andi
 */

#ifndef ECPP_TARGET_AVR8_TWI_HPP_
#define ECPP_TARGET_AVR8_TWI_HPP_

#include <ecpp/Arch/AVR8.hpp>
#include <ecpp/Datatypes.hpp>

#if defined (TWCR)
namespace ecpp
{
  template<unsigned int N>
  class TWIMaster
  {
  private:
    uint8_t m_Buffer[N+1];
    uint8_t m_Idx;
    uint8_t m_Len;
  public:
    typedef uint8_t TransferBufferType[N];

    template<typename T = TransferBufferType>
    T & getBuffer()
    {
      union
      {
        T*       pReturn;
        uint8_t* pBuffer;
      };

      pBuffer = &(m_Buffer[1]);
      return *pReturn;
    }

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

    void handleIrq();
    void handleCyclic();

    void sendStartAndWrite(uint8_t address, uint8_t len);
    void sendStartAndRead(uint8_t address, uint8_t len);
    void sendStop();
    void sendStopStartAndRead(uint8_t address, uint8_t len);
    void close() {TWCR = 0;}

    bool    hasFinished() {return m_Idx >= m_Len;}
    bool    hasNack()     {return m_Len == 0;}
    uint8_t getError()    {return (m_Len == 0) ? (TWSR & 0xF8) : 0;}
    uint8_t getStatus()   {return (TWSR & 0xF8);}
  };

  template<unsigned int N>
  void TWIMaster<N>::handleIrq()
  {
    TWCR = TWCR & ~(_BV(TWIE) | _BV(TWSTA));
  }

  template<unsigned int N>
  void TWIMaster<N>::handleCyclic()
  {
    if(TWCR & _BV(TWINT) && m_Len > m_Idx)
    {
      uint8_t status = TWSR & 0xF8;

      switch(status)
      {
      case 0x08: /* start condition */
      case 0x10: /* repeated start cond */
        if(m_Idx < m_Len)
        {
          TWDR  = m_Buffer[m_Idx];
          TWCR  = /*_BV(TWIE) |*/ _BV(TWEN) | _BV(TWINT);
        }
        break;
      case 0x18: /* sla+w + ack */
      case 0x28: /* write + ack  */
        {
          m_Idx += 1;

          if(m_Idx < m_Len)
          {
            TWDR   = m_Buffer[m_Idx];
            TWCR  = /*_BV(TWIE) | */ _BV(TWEN) | _BV(TWINT);
          }
        }
        break;
      case 0x20: /* sla+w + nack */
      case 0x30: /* write + nack */
      case 0x48: /* sla+r + nack, */
      case 0x58: /* read  + nack return */
        m_Len = 0; /* this means nack */
        break;
  #if 0
      case 0x38: /* arbitration lost */
        if(m_Idx < m_Len)
        {
          TWCR = /* _BV(TWIE) | */ _BV(TWEN) | _BV(TWSTA) | _BV(TWINT);
        }
        break;
  #endif
      case 0x40: /* sla+r + ack */
        {
          m_Idx += 1;

          if(m_Idx < m_Len)
          {
            TWCR  = /* _BV(TWIE) | */ _BV(TWEN) | _BV(TWEA) | _BV(TWINT);
          }
        }
        break;
      case 0x50: /* read + ack return */
        {
          m_Buffer[m_Idx] = TWDR;
          m_Idx += 1;

          if(m_Idx < m_Len)
          {
            TWCR  = /* _BV(TWIE) | */ _BV(TWEN) | _BV(TWEA) | _BV(TWINT);
          }
        }
        break;
      }
    }
  }

  template<unsigned int N>
  void TWIMaster<N>::sendStartAndWrite(uint8_t address, uint8_t len)
  {
    m_Buffer[0] = (address << 1);
    m_Idx       = 0;
    m_Len       = len + 1;

    //while(TWCR & _BV(TWSTO));
    TWCR = _BV(TWINT) | _BV(TWSTA) /* | _BV(TWIE) */ | _BV(TWEN);
  }

  template<unsigned int N>
  void TWIMaster<N>::sendStartAndRead(uint8_t address, uint8_t len)
  {
    m_Buffer[0] = (address << 1) | 0x01;
    m_Idx       = 0;
    m_Len       = len + 1;

    //while(TWCR & _BV(TWSTO));
    TWCR = _BV(TWINT) | _BV(TWSTA) /* | _BV(TWIE) */ | _BV(TWEN);
  }

  template<unsigned int N>
  void TWIMaster<N>::sendStop()
  {
    TWCR = _BV(TWINT) | _BV(TWSTO) /* | _BV(TWIE) */ | _BV(TWEN);
  }

  template<unsigned int N>
  void TWIMaster<N>::sendStopStartAndRead(uint8_t address, uint8_t len)
  {
    m_Buffer[0] = (address << 1) | 0x01;
    m_Idx       = 0;
    m_Len       = len + 1;

    //while(TWCR & _BV(TWSTO));
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWSTO) | _BV(TWEN);
  }

};
#endif
#endif /* ECPP_TARGET_AVR8_TWI_HPP_ */
