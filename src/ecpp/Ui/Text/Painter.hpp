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
#ifndef ECPP_UI_TEXT_PAINTER_H_
#define ECPP_UI_TEXT_PAINTER_H_


#include "ecpp/Datatypes.hpp"
#include "ecpp/Units/Fraction.hpp"

#include <stdio.h>
#include <cstdarg>
#include <cstdint>
#include <cstring>

namespace ecpp::Ui::Text
{
  using ::std::strlen;
  using ::std::memcpy;
  using ::std::int_least8_t;

  class TextPainterText
  {
  public:
    constexpr TextPainterText(const char *text) : Text(text) {}

    constexpr const char* getText() const {return Text;}

  protected:
    const char* const Text;
  };

  class TextPainterTextWithLen : public TextPainterText
  {
  public:
    template<size_t ARRSIZE>
    constexpr TextPainterTextWithLen(const int8_t (& text)[ARRSIZE]) : TextPainterText(text), Len{ARRSIZE} {}

    template<size_t ARRSIZE>
    constexpr TextPainterTextWithLen(const char   (& text)[ARRSIZE]) : TextPainterText(text), Len{ARRSIZE} {}

    explicit constexpr TextPainterTextWithLen(const char *text)    : TextPainterText(text), Len{strlen(text)} {}
    constexpr TextPainterTextWithLen(const char *text, size_t len) : TextPainterText(text), Len{len} {}

    template<size_t ARRSIZE>
    static TextPainterTextWithLen trim(const int8_t (& text)[ARRSIZE])
    {
      return trim(text, ARRSIZE);
    }

    template<size_t ARRSIZE>
    static TextPainterTextWithLen trim(const char (& text)[ARRSIZE])
    {
      return trim(text, ARRSIZE);
    }

    static TextPainterTextWithLen trim(const char *text, size_t len);

    constexpr size_t getLen() const { return Len; }
  protected:

    size_t Len;
  };


  class TextPainterDefaultBase
  {
  public:
    typedef int_least8_t IndexType;
    typedef size_t       TextLenType;

    constexpr TextPainterDefaultBase(char* buffer, IndexType cols, IndexType rows, IndexType row_add)
      : Buffer{buffer}, Cols {cols}, Rows{rows}, RowAdd{row_add} {};

    constexpr TextPainterDefaultBase subPainter(IndexType col, IndexType row, IndexType ncol, IndexType nrow)
    {
      return TextPainterDefaultBase(Buffer + col + row * RowAdd,
                                ::ecpp::min(::ecpp::max<IndexType>(0, Cols - col), ncol),
                                ::ecpp::min(::ecpp::max<IndexType>(0, Rows - row), nrow),
                                 RowAdd);
    }

    class LocationType
    {
    public:
      IndexType Col;
      IndexType Row;
    };

    static constexpr IndexType  StartIndex = 0;
  protected:
    char*      Buffer;
    /** number of columns */
    IndexType  Cols;
    /** number of rows */
    IndexType  Rows;
    /** add value to go to next row */
    IndexType  RowAdd;
  };

  template<typename TextPainterBase = TextPainterDefaultBase >
  class TextPainter : protected TextPainterBase
  {
  public:
    typedef typename TextPainterBase::IndexType    IndexType;
    typedef typename TextPainterBase::TextLenType  TextLenType;
    typedef typename TextPainterBase::LocationType LocationType;

    constexpr TextPainter subPainter(IndexType col, IndexType row, IndexType ncol, IndexType nrow)
    {
      return TextPainter(TextPainterBase::subPainter(col,row,ncol,nrow));
    }

    constexpr IndexType getColCount() const {return this->Cols;}
    constexpr IndexType getRowCount() const {return this->Rows;}

    void clear();
    void fill(char c);


    void putChar(const char text, LocationType loc);

    void putText(const char* text);
    void putText(const char* text, TextLenType len);
    void putText(const char* text, TextLenType len, IndexType offset);
    void putText(TextPainterTextWithLen text, LocationType loc);

    template<size_t ARRSIZE>
    void putText(const int8_t (& text)[ARRSIZE]) {putText((const char*)text, ARRSIZE);}

    template<size_t ARRSIZE>
    void putText(const int8_t (& text)[ARRSIZE], IndexType offset) {putText((const char*)text, ARRSIZE, offset);}

    void centerText(const char *text);
    void centerText(TextPainterTextWithLen text);

    template<typename T>
    void putProgress(const ::ecpp::Units::Fraction<T> fraction);

    void iprintf(const char* format, ...);

    template<size_t ARRSIZE>
    void centerTrimmedText(const int8_t (& text)[ARRSIZE])
    {
      centerText(TextPainterTextWithLen::trim(text));
    }

    template<size_t ARRSIZE>
    void centerTrimmedText(const char (& text)[ARRSIZE])
    {
      centerText(TextPainterTextWithLen::trim(text));
    }

  protected:
    constexpr TextPainter(TextPainterBase init) : TextPainterBase {init} {}
  };

  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::clear()
  {
    fill(' ');
  }

  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::fill(char c)
  {
    for(IndexType r = 0; r < this->Rows; r++)
      memset(this->Buffer + r * this->RowAdd, c, this->Cols);
  }

  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::putChar(const char text, LocationType loc)
  {
    if( (loc.Col >= this->Cols) || (loc.Row >= this->Rows))
      return;

    this->Buffer[loc.Col + loc.Row * this->RowAdd] = text;
  }

  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::putText(const char* text)
  {
    putText(text, strlen(text));
  }

  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::putText(const char* text, TextLenType len)
  {
    putText( {text, len}, {0,0});
  }

  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::putText(const char* text, TextLenType len, IndexType offset)
  {
    putText( {text, len}, { offset, 0 });
  }

  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::putText(TextPainterTextWithLen text, LocationType loc)
  {
    auto text_len = text.getLen();
    auto p = text.getText();

    if(loc.Col >= this->Cols)
      return;

    while(text_len > 0)
    {
      TextLenType l;
      IndexType i;

      if(loc.Row >= this->Rows)
        return;

      if(text_len > static_cast<decltype(text_len)>(this->Cols - loc.Col))
        l = this->Cols - loc.Col;
      else
        l = text_len;

      /* manual copy of text till terminating 0 */
      for(i = 0; (i < l) && ('\0' != p[i]); ++i)
        this->Buffer[loc.Col + loc.Row * this->RowAdd + i] = p[i];

      /* clear remainder */
      memset(this->Buffer + loc.Col + loc.Row * this->RowAdd + i, ' ', l - i);

      p        += i;
      text_len -= l;

      loc.Row += 1;
      loc.Col  = 0;
    }
  }


  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::centerText(const char *text)
  {
    centerText(TextPainterTextWithLen(text));
  }

  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::centerText(TextPainterTextWithLen text)
  {
    IndexType offset;
    auto length = text.getLen();

    if (length > static_cast<decltype(length)>(TextPainterBase::Cols))
      length = TextPainterBase::Cols;

    offset = (TextPainterBase::Cols - length) / 2;

    memcpy(TextPainterBase::Buffer + offset, text.getText(), length);
  }

  template<typename TextPainterBase>
  template<typename T>
  void TextPainter<TextPainterBase>::putProgress(const ::ecpp::Units::Fraction<T> fraction)
  {
    if (0 == fraction.Denominator)
    {
      fill('-');
    }
    else
    {
      IndexType n = this->Cols;

      if(fraction.Nominator < fraction.Denominator)
        n = n * fraction.Nominator / fraction.Denominator;

      memset(this->Buffer, '#', n);
    }
  }

  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::iprintf(const char* format, ...)
  {
    ::std::va_list args;
    va_start(args, format);
    vsniprintf(this->Buffer, this->Cols, format, args);
    va_end(args);
  }

};
#endif /* ECPP_UI_TEXT_PAINTER_H_ */
