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
#ifndef ECPP_TEXT_UTF8_HPP_
#define ECPP_TEXT_UTF8_HPP_

#include <cstddef>
#include <cstdint>
#include <iterator>

namespace ecpp::Text
{
  class CodePoint
  {
  public:
    typedef ::std::uint32_t Value ;

    constexpr CodePoint(Value cp = 0) : cp_(cp) {}

    constexpr operator ::std::uint32_t() const { return cp_; }
    constexpr bool isVisible()           const { return cp_ > 0x20; }

    const Value cp_;
  };

  class Utf8TextProcessor
  {
  public:
    class TextIterator;

    static size_t CountCharacters(const char* string, size_t len);

    static size_t CountCharacters(const char* string) { return CountCharacters(string, 0xFFFFFFFF); }

    template<size_t len>
    static size_t CountCharacters(const char (&string)[len]) { return CountCharacters(string, len); }
  };

  class Utf8TextProcessor::TextIterator : public std::iterator<std::input_iterator_tag, CodePoint>
  {
  public:
    constexpr TextIterator(const char* string, size_t string_len) : string_(string), string_len_(string_len) {}
    constexpr TextIterator(TextIterator start, TextIterator end) : string_(start.string_), string_len_(start.string_len_ - end.string_len_) {}

    CodePoint operator* () const;

    TextIterator & operator ++() { next(); return *this; }

    const char* GetBuffer()    { return string_;}
    size_t      GetBufferLen() {return string_len_;}
  private:
    const char* string_;
    size_t      string_len_;

    void next();
  };

};

#endif /* ECPP_TEXT_UTF8_HPP */