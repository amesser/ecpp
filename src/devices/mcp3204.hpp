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
#ifndef MCP3204_HPP_
#define MCP3204_HPP_

namespace Platform {
  namespace Devices {
    template <class DEVICE>
    class MCP3204 : public DEVICE {
    public:
      uint16_t sample(const uint8_t channel) {
        uint16_t result;

        this->selectChip();

        auto transport = this->getTransport(25000UL * 16UL);

        if (channel < 4)
            transport.transferByte(0x06);
        else
            transport.transferByte(0x04);

        result = (transport.transferByte(channel << 6) & 0x0F) << 8;
        result |= transport.transferByte(0xFF);

        this->deselectChip();

        return result;
      }
    };
  }
}


#endif /* MCP3204_HPP_ */
