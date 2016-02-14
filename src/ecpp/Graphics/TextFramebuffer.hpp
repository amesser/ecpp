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
#ifndef ECPP_GRAPHICS_TEXTFRAMEBUFFER_HPP_
#define ECPP_GRAPHICS_TEXTFRAMEBUFFER_HPP_

#include "ecpp/Datatypes.hpp"
#include <string.h>

namespace ecpp
{
  template<int COLUMNS, int ROWS>
  class TextCoordinate
  {
  public:
    typedef typename UnsignedIntTypeEstimator<COLUMNS>::Type ColumnIndexType;
    typedef typename UnsignedIntTypeEstimator<ROWS>::Type    RowIndexType;

    static const ColumnIndexType Columns = COLUMNS;
    static const RowIndexType    Rows    = ROWS;

    typedef typename UnsignedIntTypeEstimator<COLUMNS*ROWS>::Type IndexType;

    static constexpr IndexType calculateIndex(ColumnIndexType Column, RowIndexType Row)
    {
      return Column + Row * COLUMNS;
    }
  private:
    IndexType m_Index;
    constexpr TextCoordinate(IndexType Index) : m_Index(Index) {}

  public:
    constexpr TextCoordinate() {}
    constexpr TextCoordinate(ColumnIndexType Column, RowIndexType Row) : m_Index(calculateIndex(Column,Row)) {}

    constexpr ColumnIndexType getColumn() const { return m_Index % Columns;}
    constexpr RowIndexType    getRow()    const { return m_Index / Columns;}

    template<typename RHSTYPE>
    TextCoordinate operator + (RHSTYPE Rhs) const { return TextCoordinate(m_Index + Rhs); }


    constexpr bool operator < (const TextCoordinate & rhs) const {return m_Index < rhs.m_Index;}
    constexpr bool operator > (const TextCoordinate & rhs) const {return m_Index > rhs.m_Index;}

    TextCoordinate operator ++ (int)
    {
      return TextCoordinate(m_Index++);
    }

    constexpr IndexType asIndex() const {return m_Index;}
  };

  template<int COLUMNS, int ROWS>
  class TextRowCoordinate : public TextCoordinate<COLUMNS, ROWS>
  {
  private:
    static constexpr typename TextCoordinate<COLUMNS, ROWS>::IndexType s_RowMask = (ROWS - 1) * COLUMNS;

  public:
    typedef typename UnsignedIntTypeEstimator<COLUMNS>::Type ColumnIndexType;
    typedef typename UnsignedIntTypeEstimator<ROWS>::Type    RowIndexType;

    constexpr TextRowCoordinate(ColumnIndexType Column,  RowIndexType Row)
    : TextCoordinate<COLUMNS, ROWS>(0, Row) {}
    constexpr TextRowCoordinate(TextCoordinate<COLUMNS, ROWS> Init) : TextCoordinate<COLUMNS, ROWS>(Init.m_Index & s_RowMask) {}
  };


  template<int COLUMNS, int ROWS>
  class TextFramebuffer
  {
  public:
    typedef typename UnsignedIntTypeEstimator<COLUMNS*ROWS>::Type IndexType;

    static constexpr IndexType getBufferSize() {return COLUMNS * ROWS;}
    static constexpr IndexType getNumColumns() {return COLUMNS; }
    static constexpr IndexType getNumRows()    {return ROWS; }

    typedef TextCoordinate<COLUMNS,ROWS>    CursorType;
    typedef TextRowCoordinate<COLUMNS,ROWS> RowCursorType;
    typedef char  RowBufferType[COLUMNS];

  private:
    static constexpr IndexType s_RowMask = ~(getNumColumns() - 1) & (getBufferSize() - 1);

    CursorType     m_CursorUpdate;
    uint_least8_t  m_DirtyCnt;

    char           m_Framebuffer[getBufferSize()];
  public:

    template<typename DEVICE>
    void update(DEVICE &Device);

    template<typename ITYPE>
    void displayString(CursorType CoordinateLoc, ITYPE Start, ITYPE End);

    RowBufferType & getRow(const RowCursorType RowCursor)
    {
      m_DirtyCnt |= 0x1;
      return *reinterpret_cast<RowBufferType*>(&(m_Framebuffer[RowCursor.asIndex()]));
    }

    constexpr CursorType begin(void) const {return CursorType(0,0);}
    constexpr CursorType end(void)   const {return CursorType(COLUMNS,ROWS);}

    void clear(char value = ' ');
  };

  template<int COLUMNS, int ROWS>
  template<typename DEVICE>
  void TextFramebuffer<COLUMNS, ROWS>::update(DEVICE &Device)
  {
    const auto Index = m_CursorUpdate.asIndex();

    if(Index < getBufferSize())
    {
      if(!Device.isBusy())
      {
        if (Device.getCursor() != m_CursorUpdate)
        {
          Device.moveCursor(m_CursorUpdate);
        }
        else
        {
          Device.putCharacter(m_Framebuffer[Index]);
          m_CursorUpdate++;
        }
      }
    }
    else
    {
      if (m_DirtyCnt & 0x1)
      {
        m_DirtyCnt     += 1;
        m_CursorUpdate   = {0,0};
      }
    }
  }

  template<int COLUMNS, int ROWS>
  template<typename ITYPE>
  void TextFramebuffer<COLUMNS, ROWS>::displayString(CursorType Coordinate, ITYPE Start, ITYPE End)
  {
    if(getBufferSize() >= (End - Start + Coordinate.asIndex()))
    {
      IndexType Offset = Coordinate.asIndex();
      m_DirtyCnt |= 0x1;

      while(Start < End)
      {
        m_Framebuffer[Offset++] = *(Start++);
      }
    }
  }

  template<int COLUMNS, int ROWS>
  void TextFramebuffer<COLUMNS, ROWS>::clear(char value)
  {
    m_DirtyCnt |= 0x1;
    memset(m_Framebuffer, value, sizeof(m_Framebuffer));
  }

}



#endif /* ECPP_GRAPHICS_TEXTFRAMEBUFFER_HPP_ */
