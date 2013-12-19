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

#ifndef USBHID_HPP_
#define USBHID_HPP_

#include "util/vamath.hpp"
#include "generic/endian.hpp"

namespace Platform {
  namespace Protocols {
    namespace USB {
      using ::Platform::Endian::LittleEndian;
      using ::Platform::Util::VaMath::va_or;

      typedef Descriptor<0x21, uint16_t, uint8_t, uint8_t, uint8_t, uint16_t> HIDDescriptorType;

      class HIDDescriptor : public HIDDescriptorType
      {
      public:
        constexpr HIDDescriptor(uint16_t bcdHID, uint8_t bCountryCode, uint8_t bNumDescriptors,
            uint8_t bDescriptorType, uint16_t wDescriptorLength) : HIDDescriptorType(bcdHID,
                bCountryCode, bNumDescriptors, bDescriptorType, wDescriptorLength) {}
      };

      template <uint8_t type, uint8_t tag, typename ...Args>
      struct ShortItem : public LittleEndian<uint8_t, Args...>
      {
        static constexpr uint8_t ItemSizes[] = {0, 1, 2 , 3 ,3};
      public:
        constexpr ShortItem(Args const & ...args) : LittleEndian<uint8_t, Args...>(
            (ItemSizes[LittleEndian<Args...>::size()]) |(0xC & (type << 2)) | (0xF0 & (tag << 4)),
            args...) {}
      };

      template <uint8_t type, uint8_t tag, typename ...Args>
      struct LongItem : public LittleEndian<uint8_t, uint8_t, uint8_t, Args...>
      {
      public:
        constexpr LongItem(Args const & ...args) : LittleEndian<uint8_t, Args...>(
            (2 & 0x3) |(0xC & (3 << 2)) | (0xF0 & (0xf << 4)), type,tag,
            args...) {}
      };

      template<bool yesno, uint8_t type, uint8_t tag, typename ...Args>
      struct ItemStruct {};

      template<uint8_t type, uint8_t tag, typename ...Args>
      struct ItemStruct<false, type,tag, Args...> : public ShortItem<type, tag, Args...>
      {
      public:
        constexpr ItemStruct(Args const & ...args) : ShortItem<type, tag, Args...>(args...) {};
      };

      template<uint8_t type, uint8_t tag, typename ...Args>
      struct ItemStruct<true, type,tag, Args...> : public LongItem<type, tag, Args...>
      {
        constexpr ItemStruct(Args const & ...args) : LongItem<type, tag, Args...>(args...) {};
      };

      template<uint8_t type, uint8_t tag, typename ...Args>
      struct ReportItem : public ItemStruct< (LittleEndian<Args...>::size() > 4), type,tag,Args...>
      {
        constexpr ReportItem(Args const & ...args) : ItemStruct< (LittleEndian<Args...>::size() > 4), type,tag,Args...>(args...) {};
      };

      template<uint8_t tag, typename ...Args>
      using MainItem   = ReportItem<0, tag, Args...>;
      template<uint8_t tag, typename ...Args>
      using GlobalItem = ReportItem<1, tag, Args...>;
      template<uint8_t tag, typename ...Args>
      using LocalItem  = ReportItem<2, tag, Args...>;


      typedef GlobalItem<0, uint8_t> UsagePage;
      typedef LocalItem<0, uint8_t> Usage;
      typedef MainItem<10, uint8_t> Collection;
      typedef MainItem<12> EndCollection;

      typedef GlobalItem<1, uint8_t> LogicalMinimum;
      typedef GlobalItem<2, uint8_t> LogicalMaximum;
      typedef GlobalItem<7, uint8_t> ReportSize;
      typedef GlobalItem<8, uint8_t> ReportID;
      typedef GlobalItem<9, uint8_t> ReportCount;


      template<typename ...Args>
      constexpr MainItem<8,uint16_t> Input(Args const & ...args)
      {
        return { (uint16_t)va_or(static_cast<uint32_t>(args)...)};
      }

      template<typename ...Args>
      constexpr MainItem<9,uint16_t> Output(Args const & ...args)
      {
        return { (uint16_t)va_or(static_cast<uint32_t>(args)...)};
      }

      template<typename ...Args>
      constexpr MainItem<11,uint16_t> Feature(Args const & ...args)
      {
        return { (uint16_t)va_or(static_cast<uint32_t>(args)...)};
      }

      enum {
        AlphanumericDisplay        = 0x01,
        DisplayAttributesReport    = 0x20,
        ASCIICharacterSet          = 0x21,
        DataReadBack               = 0x22,
        FontReadBack               = 0x23,
        DisplayControlReport       = 0x24,
        ClearDisplay               = 0x25,
        DisplayEnable              = 0x26,
        ScreenSaverDelay           = 0x27,
        ScreenSaverEnable          = 0x28,
        VerticalScroll             = 0x29,
        HorizontalScroll           = 0x2A,
        CharacterReport            = 0x2B,
        DisplayData                = 0x2C,
        DisplayStatus              = 0x2D,
        StatNotReady               = 0x2E,
        StatReady                  = 0x2F,
        ErrNotaloadablecharacter   = 0x30,
        ErrFontdatacannotberead    = 0x31,
        CursorPositionReport       = 0x32,
        Row                        = 0x33,
        Column                     = 0x34,
        Rows                       = 0x35,
        Columns                    = 0x36,
        CursorPixelPositioning     = 0x37,
        CursorMode                 = 0x38,
        CursorEnable               = 0x39,
        CursorBlink                = 0x3A,
        FontReport                 = 0x3B,
        FontData                   = 0x3C,
        CharacterWidth             = 0x3D,
        CharacterHeight            = 0x3E,
        CharacterSpacingHorizontal = 0x3F,
        CharacterSpacingVertical   = 0x40,
        UnicodeCharacterSet        = 0x41,
        FontSevenSegment           = 0x42,
        SevenSegmentDirectMap      = 0x43,
        FontFourteenSegment        = 0x44,
        FourteenSegmentDirectMap   = 0x45,
        DisplayBrightness          = 0x46,
        DisplayContrast            = 0x47,
        CharacterAttribute         = 0x48,
        AttributeReadback          = 0x49,
        AttributeData              = 0x4A,
        CharAttrEnhance            = 0x4B,
        CharAttrUnderline          = 0x4C,
        CharAttrBlink              = 0x4D,
      };

      enum {
        Application = 0x01,
        Logical     = 0x02,
      };

      enum {
        Data     = 0,
        Constant = 1 << 0,
        Array    = 0,
        Variable = 1 << 1,
        Absolute = 0,
        Relative = 1 << 2,
        NoWrap   = 0,
        Wrap     = 1 << 3,
        Linear   = 0,
        Nonlinear = 1 << 4,
        PreferredState = 0,
        NoPreferredState    = 1 << 5,
        NoNullPosition = 0,
        NullState      = 1 << 6,
        Nonvolatile    = 0,
        Volatile       = 1 << 7,
        BitField       = 0,
        BufferedBytes  = 1 << 8

      };
    };
  };
};



#endif /* HID_HPP_ */
