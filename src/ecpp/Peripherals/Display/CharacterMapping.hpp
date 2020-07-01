/*
 *  Copyright 2018-2020 Andreas Messer <andi@bastelmap.de>
 *
 *  This file is part of the Musicbox projects firmware
 *
 *  Musicbox's firmware is free software: you can
 *  redistribute it and/or modify it under the terms of the GNU General
 *  Public License as published by the Free Software Foundation,
 *  either version 3 of the License, or (at your option) any later
 *  version.
 *
 *  Musicbox's firmware is distributed in the hope that it
 *  will be useful, but WITHOUT ANY WARRANTY; without even the implied
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with musicbox.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef ECPP_PERIPHERALS_DISPLAY_CHARACTERMAPPING_HPP_
#define ECPP_PERIPHERALS_DISPLAY_CHARACTERMAPPING_HPP_

#include "ecpp/Text/Utf8.hpp"

namespace ecpp::Peripherals::Display
{
  template<typename DisplayDriver>
  class MappedTextArea;

  template<typename DisplayDriver>
  class MappedDisplayCharacter
  {
  public:

    void operator = (::ecpp::Text::CodePoint cp)
    {
      c_ = DisplayDriver::TextProcessor::encode(cp);
    }

    void operator = (char c)
    {
      c_ = DisplayDriver::TextProcessor::encode(c);
    }

  private:
    constexpr MappedDisplayCharacter(typename DisplayDriver::Character & c) : c_(c) {};

    typename DisplayDriver::Character & c_;

    friend class MappedTextArea<DisplayDriver>;
  };

  template<typename DisplayDriver>
  class MappedTextArea
  {
  public:
    typedef typename DisplayDriver::Character               BufferElement;
    typedef typename DisplayDriver::Location                Location;
    typedef typename DisplayDriver::TextProcessor           TextProcessor;

    typedef MappedDisplayCharacter<DisplayDriver>           Character;

    typedef decltype(Location::col)                         Column;
    typedef decltype(Location::row)                         Row;

    typedef uint_least8_t IndexType;

    constexpr MappedTextArea(BufferElement* buffer, Column num_col, Row num_row, IndexType row_add) :
      buffer_(buffer), num_col_(num_col), num_row_(num_row), row_add_(row_add) {};

    template<size_t c, size_t r>
    constexpr MappedTextArea(BufferElement (&buffer)[r][c], Column col = 0, Row row = 0) :
      buffer_(&(buffer[row][col])), num_col_(c - col), num_row_(r - row), row_add_(c) {};

    /* Textarea for one row starting with optional offset */
    constexpr MappedTextArea(MappedTextArea &parent, Location start) :
      buffer_( parent.buffer_ + start.col + start.row * parent.row_add_),
      num_col_(parent.num_col_ - start.col), num_row_(1),
      row_add_(0) {};

    /* Textarea for an area */
    constexpr MappedTextArea(MappedTextArea &parent, Location start, Location end) :
      buffer_( parent.buffer_ + start.col + start.row * parent.row_add_),
      num_col_(end.col - start.col), num_row_(end.row - start.row),
      row_add_(parent.row_add_) {};

    constexpr Character operator [] (Location loc) { return Character(*(buffer_ + loc.col + loc.row * row_add_)); }

  protected:
    BufferElement* const buffer_;
    /** number of columns */
    const Column         num_col_;
    /** number of rows */
    const Row            num_row_;
    /** add value to go to next row */
    const Row            row_add_;
  };
}
#endif /* ECPP_PERIPHERALS_DISPLAY_CHARACTERMAPPING_HPP_ */