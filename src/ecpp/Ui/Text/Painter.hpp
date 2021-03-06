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
#include "ecpp/String.hpp"
#include "ecpp/StringEncodings/Unicode.hpp"

#include <stdio.h>
#include <cstdarg>
#include <cstdint>
#include <cstring>

namespace ecpp::Ui::Text
{
  using ::std::strlen;
  using ::std::memcpy;
  using ::std::int_least8_t;

  template<typename TextPainterBase>
  class TextPainterText : public ecpp::String<typename TextPainterBase::Encoding>::ConstSpan
  {
  public:
    constexpr TextPainterText(const char *text) : ecpp::String<typename TextPainterBase::Encoding>::ConstSpan(text, 0xFFFFFFFF) {}
  };

  /** Container to contain a text and its associated character count.
   *  characters != length of the buffer */
  template<typename TextPainterBase>
  class TextPainterTextWithLen : public ecpp::String<typename TextPainterBase::Encoding>::ConstSpan
  {
  public:
    using ecpp::String<typename TextPainterBase::Encoding>::ConstSpan;

    explicit constexpr TextPainterTextWithLen(const typename TextPainterBase::Encoding::BufferElement *text)
      : ecpp::String<typename TextPainterBase::Encoding>::ConstSpan(text, 0xFFFFFFFF) {}

#if 0
    constexpr TextPainterTextWithLen(Iterator start, Iterator end)      : TextPainterText<TextPainterBase>(start.GetBuffer()), text_len_{start.GetBufferLen() - end.GetBufferLen()} {}
#endif

    static TextPainterTextWithLen trim(TextPainterTextWithLen text);

    size_t CountCharacters() const
    {
      return TextPainterBase::TextProcessor::CountCharacters(this->text_, text_len_);
    }
  protected:
    /** Size of the text buffer */
    const size_t text_len_;
  };

  template<typename TextPainterBase>
  class TextPainter : public TextPainterBase
  {
  public:
    using   TextSpan = ecpp::String<ecpp::StringEncodings::Utf8>::ConstSpan;

    typedef typename TextPainterBase::Location       Location;
    typedef typename TextPainterBase::Column         Column;
    typedef typename TextPainterBase::Row            Row;
    typedef typename TextPainterBase::IndexType    IndexType;
    typedef size_t                                 TextLenType;

    using TextPainterBase::TextPainterBase;

    using TextPainterBase::num_col;
    using TextPainterBase::num_row;

    constexpr TextPainter(TextPainterBase parent)                               : TextPainterBase {parent, Location(0,0), Location(parent.num_col(), parent.num_row()) } {}
    constexpr TextPainter(TextPainterBase parent, Location start)               : TextPainterBase {parent, start} {}

    constexpr TextPainter CreateSubPainter(Location upper_left, Location lower_right)
    {
      return TextPainter(*this, upper_left, lower_right);
    }

    constexpr TextPainter CreateFieldPainter(Column col_start, Row r)
    {
      return CreateFieldPainter(col_start, r, num_col());
    }

    constexpr TextPainter CreateFieldPainter(Column col_start, Row r, Column col_end)
    {
      return TextPainter(*this, Location(col_start, r), Location(col_end, r + 1));
    }

    constexpr TextPainter CreateRowPainter(Row r)
    {
      return CreateFieldPainter(0, r, num_col());
    }

    void clear();
    void fill(char c);
    void putChar(const char text, Location loc);

    void putText(const char* text);
    void putText(const char* text, size_t len);
    void putText(const char* text, size_t len, Column offset);
    void putText(TextSpan text, Location loc = {0,0});

    template<size_t ARRSIZE>
    void putText(const int8_t (& text)[ARRSIZE]) {putText((const char*)text, ARRSIZE);}

    template<size_t ARRSIZE>
    void putText(const int8_t (& text)[ARRSIZE], IndexType offset) {putText((const char*)text, ARRSIZE, offset);}

    void centerText(const char *text);
    void centerText(TextSpan   text);

    template<typename T>
    void putProgress(const ::ecpp::Units::Fraction<T> fraction);

    void iprintf(const char* format, ...);

    void centerTrimmedText(TextSpan text)
    {
      centerText(text.trim());
    }
  };

  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::clear()
  {
    fill(' ');
  }

  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::fill(char c)
  {
    Location l(0,0);

    for(l.row = 0; l.row < num_row(); l.row++)
    {
      for (l.col = 0; l.col < num_col(); l.col++)
      {
        (*this)[l] = c;
      }

    }
  }

  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::putChar(const char text, Location loc)
  {
    if( (loc.col >= num_col()) || (loc.row >= num_row()))
      return;

    (*this)[loc] = text;
  }

  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::putText(const char* text)
  {
    putText(text, strlen(text));
  }

  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::putText(const char* text, size_t len)
  {
    putText( {text, len}, {0,0});
  }

  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::putText(const char* text, size_t len, Column offset)
  {
    putText( {text, len}, { offset, 0 });
  }

  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::putText(TextSpan text, Location loc)
  {
    auto it = text.begin();

    if(loc.col >= this->num_col())
      return;

    while(loc.row < this->num_row())
    {
      /* manual copy of text till terminating 0 */
      for(; loc.col < this->num_col(); ++loc.col, ++it)
      {
        auto cp = *it;

        if (cp.kSTRING_END() == cp)
          break;
        else
          (*this)[loc] = cp;
      }

      for(; loc.col < this->num_col(); ++loc.col)
        (*this)[loc] = ' ';

      loc.row++;
      loc.col = 0;
    }
  }

  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::centerText(const char *text)
  {
    centerText(TextSpan(text));
  }

  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::centerText(TextSpan text)
  {
    Location l(0,0);
    auto it = text.begin();
    auto c  = text.countCharacters();

    if (c < num_col())
      l.col = (num_col() - c) / 2;

    for(;l.col < num_col(); ++l.col, ++it)
      (*this)[l] = *it;
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
      Location l(0,0);
      Column n = num_col();

      if(fraction.Nominator < fraction.Denominator)
        n = n * fraction.Nominator / fraction.Denominator;

      for(;l.col < n; ++l.col)
        (*this)[l] = '#';
    }
  }

  template<typename TextPainterBase>
  void TextPainter<TextPainterBase>::iprintf(const char* format, ...)
  {
    char buffer[this->num_col()*2];
    ::std::va_list args;

    va_start(args, format);
    vsniprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    putText(buffer);
  }

};
#endif /* ECPP_UI_TEXT_PAINTER_H_ */
