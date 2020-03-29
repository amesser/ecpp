/*
 *  Copyright 2019 Andreas Messer <andi@bastelmap.de>
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
#ifndef ECPP_UI_WIDGET_TEXT_DRAWCONTEXT_H_
#define ECPP_UI_WIDGET_TEXT_DRAWCONTEXT_H_

#include "ecpp/String.hpp"
#include "ecpp/Units/Fraction.hpp"

namespace ecpp {

  /* A context which paints to a field in a text display row */
  template<typename INDEXTYPE>
  class FieldContext
  {
  public:
    typedef INDEXTYPE IndexType;

    constexpr FieldContext(char* field, IndexType size) : field(field), size(size) {}

    void clear();
    void fill(char c);

    void putText(const char* text);
    void putText(const char* text, IndexType len);
    void putText(const char* text, IndexType len, IndexType offset);

    template<size_t ARRSIZE>
    void putText(const int8_t (& text)[ARRSIZE]) {putText((const char*)text, ARRSIZE);}

    template<size_t ARRSIZE>
    void putText(const int8_t (& text)[ARRSIZE], IndexType offset) {putText((const char*)text, ARRSIZE, offset);}

    void centerText(const char* text);
    void centerText(const char* text, IndexType len);

    void centerTrimmedText(const char* text);
    void centerTrimmedText(const char* text, IndexType len);

    template<size_t ARRSIZE>
    void centerTrimmedText(const int8_t (& text)[ARRSIZE]) {centerTrimmedText((const char*)text, ARRSIZE);}

    template<size_t ARRSIZE>
    void centerTrimmedText(const char   (& text)[ARRSIZE]) {centerTrimmedText(text, ARRSIZE);}

    template<typename T>
    void putProgress(const ::ecpp::Units::Fraction<T> fraction);

    char & operator[] (IndexType i) {return this->field[i]; }

    explicit operator char *() {return this->field; }

    FieldContext subField(IndexType offset, IndexType len) const
    {
      if (offset >= this->size)
      {
        offset = 0;
        len    = 0;
      }
      else if ((offset + len) >= this->size)
      {
        len = this->size - offset;
      }

      return FieldContext(this->field + offset, len);
    }

    FieldContext subField(IndexType offset) const
    {
      return subField(offset, this->size);
    }

    constexpr IndexType getSize() const {return size; }

    constexpr char * getBuffer() { return this->field;}

  private:
    /** points to memory of the field */
    char* const     field;
    /** length of the field */
    const IndexType size;
  };

  template<typename INDEXTYPE>
  void FieldContext<INDEXTYPE>::clear()
  {
    fill(' ');
  }

  template<typename INDEXTYPE>
  void FieldContext<INDEXTYPE>::fill(char c)
  {
    memset(this->field, c, this->size);
  }

  template<typename INDEXTYPE>
  void FieldContext<INDEXTYPE>::putText(const char* text)
  {
    putText(text, this->size);
  }

  template<typename INDEXTYPE>
  void FieldContext<INDEXTYPE>::putText(const char* text, IndexType len)
  {
    putText(text, len, 0);
  }

  template<typename INDEXTYPE>
  void FieldContext<INDEXTYPE>::putText(const char* text, IndexType len, IndexType offset)
  {
    IndexType i;

    if (offset > this->size)
      return;

    if (len > (this->size - offset))
      len = this->size - offset;

    for(i = 0; i < len; ++i)
    {
      if (text[i] == '\0')
        break;

      this->field[i + offset] = text[i];
    }

    for(;i < len; ++i)
      this->field[i + offset] = ' ';
  }

  template<typename INDEXTYPE>
  void FieldContext<INDEXTYPE>::centerText(const char* text)
  {
    centerText(text, strnlen(text,this->size));
  }


  template<typename INDEXTYPE>
  void FieldContext<INDEXTYPE>::centerText(const char* text, IndexType len)
  {
    clear();

    if (len >= this->size)
      putText(text);
    else
      putText(text, len, (this->size - len - 1) / 2);
  }

  template<typename INDEXTYPE>
  void FieldContext<INDEXTYPE>::centerTrimmedText(const char* text)
  {
    centerTrimmedText(text, this->size);
  }

  template<typename INDEXTYPE>
  void FieldContext<INDEXTYPE>::centerTrimmedText(const char* text, IndexType len)
  {
    size_t start;

    start = String::getLTrimLen(text, len);
    centerText(text+start, String::getRTrimStringLen(text+start, len-start));
  }

  template<typename INDEXTYPE>
  template<typename T>
  void FieldContext<INDEXTYPE>::putProgress(const ::ecpp::Units::Fraction<T> fraction)
  {
    if (0 == fraction.Denominator)
    {
      fill('-');
    }
    else
    {
      INDEXTYPE n = (size * fraction.Nominator / fraction.Denominator);
      memset(field, '#', n);
    }
  }


  class DrawContext
  {

  };
};
#endif