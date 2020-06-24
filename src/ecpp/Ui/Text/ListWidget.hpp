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
#ifndef ECPP_UI_TEXT_LISTWIDGET_HPP_
#define ECPP_UI_TEXT_LISTWIDGET_HPP_

#include "ecpp/Ui/Text/Painter.hpp"
#include <array>

namespace ecpp::Ui::Text
{
  template<typename LISTITEM>
  class ListItemDrawer
  {
  protected:
    template<typename Painter>
    static void draw(Painter &painter, const LISTITEM & item, bool selected);
  };

  template<typename LISTITEM>
  template<typename Painter>
  void ListItemDrawer<LISTITEM>::draw(Painter & painter, const LISTITEM & item, bool selected)
  {
    auto cols = painter.num_col();

    painter.CreateFieldPainter(1, 0, cols - 1).putText(item.getText());

    if(selected)
    {
      painter[typename Painter::Location(0,0)]        = '[';
      painter[typename Painter::Location(cols - 1,0)] = ']';
    }
  }

  /** Helper class convert an sequence into its size automatically */
  template<typename SEQUENCE>
  class SequenceSizeGetter
  {
  public:
    constexpr SequenceSizeGetter(const SEQUENCE & model) : Size {model.size()} {}
    typename SEQUENCE::size_type Size;
  };

  template<typename LISTMODEL>
  class ArrayListWidget : public ListItemDrawer<typename LISTMODEL::value_type >
  {

  public:
    typedef ListItemDrawer<typename LISTMODEL::value_type > DrawerType;
    typedef typename LISTMODEL::size_type                   SizeType;

    template<typename Painter>
    void draw(Painter &painter, const LISTMODEL & model);

    constexpr SizeType getSelection() const {return CurrentSelection;}

    void setSelection(SizeType pos, SequenceSizeGetter<LISTMODEL> size_dsc);
    void nextSelection(SequenceSizeGetter<LISTMODEL> size_dsc);
    void prevSelection(SequenceSizeGetter<LISTMODEL> size_dsc);

  private:
    SizeType CurrentSelection;
  };

  template<typename LISTMODEL>
  void ArrayListWidget<LISTMODEL>::setSelection(SizeType pos, SequenceSizeGetter<LISTMODEL> size_dsc)
  {
    if(pos < size_dsc.Size)
      CurrentSelection = pos;
  }

  template<typename LISTMODEL>
  void ArrayListWidget<LISTMODEL>::nextSelection(SequenceSizeGetter<LISTMODEL> size_dsc)
  {
    if((CurrentSelection + 1) < size_dsc.Size)
      CurrentSelection = CurrentSelection + 1;
    else
      CurrentSelection = 0;
  }

  template<typename LISTMODEL>
  void ArrayListWidget<LISTMODEL>::prevSelection(SequenceSizeGetter<LISTMODEL> size_dsc)
  {
    if(CurrentSelection > 0)
      CurrentSelection = CurrentSelection - 1;
    else if (size_dsc.Size > 0)
      CurrentSelection = size_dsc.Size - 1;
    else
      CurrentSelection = 0;
  }


  template<typename LISTMODEL>
  template<typename Painter>
  void ArrayListWidget<LISTMODEL>::draw(Painter &painter, const LISTMODEL & model)
  {
    auto i    = CurrentSelection;
    auto rows = painter.num_row();

    /* ensure we fit into the model */
    if(CurrentSelection >= model.size())
      i = CurrentSelection = 0;

    /* rewind iterator to first entry to display */
    for(auto r = (rows - 1) / 2; (i > 0) && (r > 0); --r)
      --i;

    painter.clear();
    for(auto r = 0; (r < rows) && (i < model.size()); ++r, ++i)
    {
      auto s = painter.CreateRowPainter(r);
      DrawerType::draw(s, model[i], i == CurrentSelection);
    }
  }
};

#endif