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
#include "ecpp/Ui/Text/Painter.hpp"

#include "ecpp/Ui/Text/Painter.hpp"

namespace ecpp {
  using ::ecpp::Ui::Text::TextPainter;

  /* A context which paints to a field in a text display row */
  template<typename _PainterType>
  class FieldContext : public _PainterType
  {
  public:
    typedef _PainterType PainterType;
    typedef typename PainterType::IndexType IndexType;

    constexpr FieldContext(PainterType &parent, IndexType ncols) : PainterType(parent.subPainter(0, 0, ncols, 1)) {};

    char & operator[] (IndexType i) {return this->Buffer[i]; }

    explicit operator char *() {return this->Buffer; }

    FieldContext subField(IndexType offset, IndexType len) const
    {
      if (offset >= this->Cols)
      {
        offset = 0;
        len    = 0;
      }
      else if ((offset + len) >= this->Cols)
      {
        len = this->Cols - offset;
      }

      return FieldContext(this->Buffer + offset, len);
    }

    FieldContext subField(IndexType offset) const
    {
      return subField(offset, this->Cols);
    }

    constexpr IndexType getSize() const {return this->Cols; }
    constexpr char * getBuffer() { return this->Buffer;}

  };

  class DrawContext
  {

  };
};
#endif