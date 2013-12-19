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
#ifndef USB_HPP_
#define USB_HPP_

namespace Platform { namespace Protocols {
  namespace USB {
    enum {
      CONFIGURATION_ATTRIBUTE_USBB10_BUSPOWERED = 0b10000000,
      CONFIGURATION_ATTRIBUTE_SELFPOWERED       = 0b01000000,
      CONFIGURATION_ATTRIBUTE_REMOTEWAKEUP      = 0b00100000,
    };

    enum {
      ENDPOINT_DIRECTION_OUT = 0b00000000,
      ENDPOINT_DIRECTION_IN  = 0b10000000,
    };

    enum {
      ENDPOINT_ATTRIBUTE_TRANSFERTYPE_CONTROL     = 0b00000000,
      ENDPOINT_ATTRIBUTE_TRANSFERTYPE_ISOCHRONOUS = 0b00000001,
      ENDPOINT_ATTRIBUTE_TRANSFERTYPE_BULK        = 0b00000010,
      ENDPOINT_ATTRIBUTE_TRANSFERTYPE_INTERRUPT   = 0b00000011,

      ENDPOINT_ATTRIBUTE_SYNCTYPE_NOSYNC            = 0b00000000,
      ENDPOINT_ATTRIBUTE_SYNCTYPE_ASYNC             = 0b00000100,
      ENDPOINT_ATTRIBUTE_SYNCTYPE_ADAPTIVE          = 0b00001000,
      ENDPOINT_ATTRIBUTE_SYNCTYPE_SYNC              = 0b00001100,

      ENDPOINT_ATTRIBUTE_USAGETYPE_DATAEP             = 0b00000000,
      ENDPOINT_ATTRIBUTE_USAGETYPE_FEEDBACKEP         = 0b00010000,
      ENDPOINT_ATTRIBUTE_USAGETYPE_EXPLICITFEEDBACKEP = 0b00100000,
    };

  };
}; };


#endif /* USB_HPP_ */
