/*
 *  Copyright 2020 Andreas Messer <andi@bastelmap.de>
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
#ifndef ECPP_PERIPHERALS_DISPLAY_EA_EA_OLEDM204_HPP_
#define ECPP_PERIPHERALS_DISPLAY_EA_EA_OLEDM204_HPP_

#include "ecpp/Peripherals/Display/CharacterDisplay.hpp"
#include "ecpp/Target/Bsp.hpp"
#include "ecpp/SimpleStringEncoding.hpp"
#include "ecpp/StringEncodings/Unicode.hpp"

namespace ecpp::Peripherals::Display
{
  using namespace ::std;

  template<char rom>
  class EAOLEDM204_ROM_ID {};

  template<>
  class EAOLEDM204_ROM_ID<'A'>
  {
  public:
    static constexpr uint_least8_t krom_id_ = 0;

    class DisplayEncoding : public ecpp::SimpleStringEncoding<uint8_t>
    {
    public:
      using Codepoint = ecpp::SimpleStringEncoding<uint8_t>::SimpleCodepoint<DisplayEncoding>;

      static Codepoint createFromUnicode(char32_t uc);

      static constexpr BufferElement kMAPPING_FAILED_CHAR = 0x60;

      static void assign(Codepoint cp, ecpp::String<DisplayEncoding>::SpanBase<uint8_t> &dest)
      {
        if(dest.buffer_size_ > 0)
          *(dest.buffer_) = cp.val_;
      }

      static void assign(ecpp::StringEncodings::Unicode::Codepoint cp, ecpp::String<DisplayEncoding>::SpanBase<uint8_t> &dest)
      {
        assign(createFromUnicode(cp.getRaw()), dest);
      }
    };
  };

  class EAOLEDM204_SpiMode : public ::ecpp::Target::Bsp::DisplayDriver
  {
  public:
    using            Location = CharacterDisplayLocation;
    typedef uint8_t  Character;

    static constexpr Location display_size {20,4};

    void SendCommand(uint_least8_t cmd);
    void SendData(uint_least8_t data);
    void Init(uint_fast8_t rom_id);

    void LocateCursor(uint8_t col, uint8_t row);
    void WriteDDRAM(const uint8_t* b, uint8_t len);
  };

  template<char rom, typename BusDriver = EAOLEDM204_SpiMode >
  class EAOLEDM204 : public BusDriver
  {
  public:
    using            DisplayEncoding = typename EAOLEDM204_ROM_ID<rom>::DisplayEncoding;
    void             Init() { BusDriver::Init(EAOLEDM204_ROM_ID<rom>::krom_id_); }
  };
}

#endif