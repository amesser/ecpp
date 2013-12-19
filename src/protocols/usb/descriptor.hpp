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
#ifndef DESCRIPTOR_HPP_
#define DESCRIPTOR_HPP_

#include "util/indices.hpp"
#include "generic/endian.hpp"

namespace Platform {
  namespace Protocols {
    namespace USB {
      using ::Platform::Endian::LittleEndianEncoder;
      using ::Platform::Util::build_indices;
      using ::Platform::Util::indices;

      template <typename ...Args>
      using UsbDescriptorEncoder = LittleEndianEncoder<uint8_t, uint8_t,Args...>;

      template <uint8_t type, typename ...Args>
      class Descriptor : public UsbDescriptorEncoder<Args...>::BufferType
      {
      public:
        constexpr Descriptor(Args const & ...args) : UsbDescriptorEncoder<Args...>::BufferType(UsbDescriptorEncoder<Args...>::encode(UsbDescriptorEncoder<Args...>::size,type, args...)) {}

        constexpr Descriptor(uint8_t const (&init)[UsbDescriptorEncoder<Args...>::BufferType::size()])
         : UsbDescriptorEncoder<Args...>::BufferType(init) {}
      };


      typedef Descriptor<0x01, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint16_t, uint16_t, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t> DeviceDescriptorType;
      typedef Descriptor<0x02, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t> ConfigurationDescriptorType;
      typedef Descriptor<0x04, uint8_t,  uint8_t, uint8_t,  uint8_t, uint8_t, uint8_t, uint8_t > InterfaceDescriptorType;
      typedef Descriptor<0x05, uint8_t,  uint8_t, uint16_t, uint8_t> EndpointDescriptorType;

      class DeviceDescriptor : public DeviceDescriptorType
      {
      public:

        constexpr DeviceDescriptor(uint16_t bcdUSB, uint8_t bDeviceClass, uint8_t bDeviceSubClass,
            uint8_t bDeviceProtocol, uint8_t bMaxPacketSize, uint16_t idVendor, uint16_t idProduct,
            uint16_t bcdDevice, uint8_t bNumConfigurations = 0) : Descriptor(bcdUSB, bDeviceClass, bDeviceSubClass, bDeviceProtocol,
                bMaxPacketSize, idVendor, idProduct, bcdDevice, 0,0,0, bNumConfigurations)
        {

        }

        template<size_t ...Is>
        constexpr DeviceDescriptor(DeviceDescriptor const & orig, uint8_t numconfig,
            indices<Is...> ) : Descriptor({orig.data()[Is]..., static_cast<uint8_t>(orig.data()[size() - 1] + numconfig)})
        {}

        constexpr DeviceDescriptor(DeviceDescriptor const & orig, uint8_t numconfig) :
            DeviceDescriptor(orig, numconfig, build_indices<size()-1>()) {}

        template<typename ...Fields>
        constexpr auto operator + (Descriptor<Fields...> const & descr)
          -> decltype(join(DeviceDescriptor(*this, 1) + descr)) const {

          return join(DeviceDescriptor(*this, 1) + descr);
        }
      };

      class ConfigurationDescriptor : public ConfigurationDescriptorType
      {
      public:
        constexpr ConfigurationDescriptor(uint16_t wTotalLength, uint8_t bNumInterfaces,
            uint8_t bConfigurationValue, uint8_t iConfiguration, uint8_t bmAttributes,
            uint16_t MaxPower1mAb) : Descriptor(wTotalLength, bNumInterfaces,
                bConfigurationValue, iConfiguration, bmAttributes, (MaxPower1mAb + 1) / 2)
        {

        }
      };

      class InterfaceDescriptor : public InterfaceDescriptorType
      {
      public:
        constexpr InterfaceDescriptor(uint8_t bInterfaceNumber, uint8_t bAlternateSetting,
            uint8_t bNumEndpoints, uint8_t bInterfaceClass, uint8_t bInterfaceSubclass,
            uint8_t bInterfaceProtocol, uint8_t iInterface)
        : InterfaceDescriptorType(bInterfaceNumber, bAlternateSetting, bNumEndpoints,
            bInterfaceClass, bInterfaceSubclass, bInterfaceProtocol, iInterface) {}
      };

      class EndpointDescriptor : public EndpointDescriptorType
      {
      public:
        constexpr EndpointDescriptor(uint8_t bEndpointAddress, uint8_t bmAttributes,
            uint16_t wMaxPacketSize, uint8_t bInterval)
        : EndpointDescriptorType(bEndpointAddress, bmAttributes, wMaxPacketSize, bInterval) {}
      };

    };
  };
};


#endif /* DESCRIPTOR_HPP_ */
