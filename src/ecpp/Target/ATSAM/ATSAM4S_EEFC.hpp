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
#ifndef ECPP_TARGET_ATSAM_ATSAM4S_EEFC_HPP_
#define ECPP_TARGET_ATSAM_ATSAM4S_EEFC_HPP_

#include <cstdint>

namespace ecpp::Target::ATSAM4S
{
  using ::std::uint32_t;

  /* Thin HAL for embedded flash controller of ATSAM4S
   */
  class EEFC
  {
  public:
    static constexpr int PageSize       = 512;
    static constexpr int PagesPerSector = 8;
    static constexpr int SectorSize     = PageSize * PagesPerSector;


    static EEFC & getInstance(int channel = 0)
    {
      return *reinterpret_cast<EEFC*>(0x400E0A00);
    }

    void eraseSectors(unsigned int num_pages, void* start);
    void writePages  (unsigned int num_pages, void* start, const void* src);
  private:
    void waitReady();

    enum : uint32_t
    {
      MSK_FSR_FRDY   = 0x00000001,
      MSK_FSR_FCMDE  = 0x00000002,
      MSK_FSR_FLOCKE = 0x00000004,
      MSK_FSR_FLERR  = 0x00000008,
    };

    enum : uint32_t
    {
      VAL_FCR_KEY             = 0x5A000000,
      MSK_FCR_FARG            = 0x00FFFF00,
      VAL_FCR_FARG_EPA_4PAGES = 0x00000000,
      VAL_FCR_FARG_EPA_8PAGES = 0x00000100,
      MSK_FCR_CMD             = 0x000000FF,
    };

    enum : uint32_t
    {
      VAL_FCMD_WP             = 0x00000001,
      VAL_FCMD_EPA            = 0x00000007,
    };

    /** Register structure of EEFC*/
    struct
    {
      volatile uint32_t FMR;
      volatile uint32_t FCR;
      volatile uint32_t FSR;
      volatile uint32_t FRR;
    } Regs;    
  };
}

#endif // ECPP_TARGET_ATSAM_ATSAM4S_EEFC_HPP_
