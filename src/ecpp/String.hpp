/*
 *  Copyright 2015-2020 Andreas Messer <andi@bastelmap.de>
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
#ifndef ECPP_STRING_HPP_
#define ECPP_STRING_HPP_

#include "ecpp/Datatypes.hpp"
#include "ecpp/Iterator.hpp"

#include <type_traits>
#include <cstring>
#include <iterator>
#include <algorithm>

namespace ecpp
{
  template<typename SpanElement>
  class StringSpanBase
  {
  public:
    constexpr StringSpanBase(const StringSpanBase<typename std::remove_const<SpanElement>::type > &init)
      : buffer_(init.buffer_), buffer_size_(init.buffer_size_) {};

    constexpr StringSpanBase(SpanElement* buffer, std::size_t buffer_size) :
      buffer_(buffer), buffer_size_(buffer_size) {};

    operator StringSpanBase<const SpanElement>() const
    {
      return StringSpanBase<const SpanElement>(buffer_, buffer_size_);
    }

  protected:
    SpanElement* buffer_       {0};
    std::size_t  buffer_size_  {0};

    friend class StringSpanBase<const typename std::remove_const<SpanElement>::type >;
  };

  template<typename _Encoding>
  class String
  {
  public:
    class Span;
    class ConstSpan;

    template<typename ReadEncoding>
    class Reader;

    template<typename ReadEncoding>
    class ReadIterator;

    template<typename WriteEncoding>
    class Writer;

    template<typename WriteEncoding>
    class WriteIterator;

  protected:
    template<typename SpanElement>
    class SpanIterator;

    template<typename SpanElement>
    class SpanBase;

    friend _Encoding;
    friend typename _Encoding::Codepoint;
  };

  template<typename _Encoding>
  template<typename SpanElement>
  class String<_Encoding>::SpanIterator : public StringSpanBase<SpanElement>
  {
  public:
    constexpr SpanIterator(const StringSpanBase<SpanElement> &init) : StringSpanBase<SpanElement>(init) {};
    using StringSpanBase<SpanElement>::StringSpanBase;

  protected:
    void         next();

    friend _Encoding;
    friend typename _Encoding::Codepoint;
  };

  template<typename _Encoding>
  template<typename SpanElement>
  class String<_Encoding>::SpanBase : public StringSpanBase<SpanElement>
  {
  public:
    constexpr SpanBase(const StringSpanBase<SpanElement> &init) : StringSpanBase<SpanElement>(init) {};
    using StringSpanBase<SpanElement>::StringSpanBase;

    template<typename ReadEncoding>
    constexpr Reader<ReadEncoding> createReader() const { return Reader<ReadEncoding>(*this); }

  protected:
    friend SpanIterator<SpanElement>;
    friend _Encoding;
    friend typename _Encoding::Codepoint;
  };

  template<typename _Encoding>
  template<typename ReadEncoding>
  class String<_Encoding>::Reader : String<_Encoding>::ConstSpan
  {
  public:
    using Iterator = String<_Encoding>::ReadIterator<ReadEncoding>;

    constexpr Reader(String<_Encoding>::ConstSpan &init) : String<_Encoding>::ConstSpan(init) {}
    using String<_Encoding>::ConstSpan::ConstSpan;

    constexpr Iterator            begin() { return Iterator(*this); }
    constexpr EndIterationTag     end()   { return EndIterationTag(); }

    friend SpanIterator<const typename _Encoding::BufferElement>;
  };

  template<typename _Encoding>
  template<typename ReadEncoding>
  class String<_Encoding>::ReadIterator : public std::iterator<std::forward_iterator_tag, typename ReadEncoding::Codepoint>,
                                          public SpanIterator<const typename _Encoding::BufferElement>
  {
  public:
    using InputCodepoint   = typename _Encoding::Codepoint;
    using OutputCodepoint  = typename  ReadEncoding::Codepoint;

    using SpanIterator<const typename _Encoding::BufferElement>::SpanIterator;

    ReadIterator & operator++ ()
    {
      using SpanType = String<_Encoding>::SpanBase<const typename std::remove_const<typename _Encoding::BufferElement>::type>;
      _Encoding::advance(reinterpret_cast<SpanType&>(*this));
      return (*this);
    }

    constexpr bool operator < (const EndIterationTag&) const
    {
      return (this->buffer_size_ > 0) && (InputCodepoint(*this) != _Encoding::Codepoint::kSTRING_END() );
    }

    constexpr OutputCodepoint operator* () const
    {
      return toCodepoint();
    }

    template<typename W>
    ReadIterator& operator >>(W &out)
    {
      using SpanType = String<_Encoding>::SpanBase<const typename std::remove_const<typename _Encoding::BufferElement>::type>;

      out << toCodepoint();
      _Encoding::advance(reinterpret_cast<SpanType&>(*this));

      return (*this);
    }
  protected:
    constexpr OutputCodepoint toCodepoint() const
    {
      return (this->buffer_size_ > 0) ? static_cast<OutputCodepoint>(InputCodepoint(*this)) :  OutputCodepoint::kSTRING_END() ;
    }

    friend _Encoding;
  };

  template<typename _Encoding>
  template<typename WriteEncoding>
  class String<_Encoding>::Writer : String<_Encoding>::Span
  {
  public:
    using Iterator = String<_Encoding>::WriteIterator<WriteEncoding>;

    constexpr Writer(String<_Encoding>::Span &init) : String<_Encoding>::Span(init) {}

    constexpr Iterator            begin() { return Iterator(*this); }
    constexpr EndIterationTag     end()   { return EndIterationTag(); }

    friend SpanIterator<typename       _Encoding::BufferElement>;
    friend SpanIterator<const typename _Encoding::BufferElement>;
  };

  template<typename _Encoding>
  template<typename WriteEncoding>
  class String<_Encoding>::WriteIterator : public std::iterator<std::output_iterator_tag, typename WriteEncoding::Codepoint>,
                                           public SpanIterator<typename _Encoding::BufferElement>
  {
  protected:
    class Proxy;
  public:
    using InputCodepoint   = typename WriteEncoding::Codepoint;
    using OutputCodepoint  = typename     _Encoding::Codepoint;

    using SpanIterator<typename _Encoding::BufferElement>::SpanIterator;

    using Codepoint = typename WriteEncoding::Codepoint;

    WriteIterator & operator ++()
    {
      using SpanType = String<_Encoding>::SpanBase<const typename std::remove_const<typename _Encoding::BufferElement>::type>;
      _Encoding::advance(reinterpret_cast<SpanType&>(*this));
      return (*this);
    }
    constexpr bool operator < (const EndIterationTag&) const { return (this->buffer_size_ > 0); }

    WriteIterator & operator << (InputCodepoint cp)
    {
      using SpanType = String<_Encoding>::SpanBase<const typename std::remove_const<typename _Encoding::BufferElement>::type>;
      _Encoding::assign(reinterpret_cast<String<_Encoding>::SpanBase<typename _Encoding::BufferElement>&>(*this), cp);
      _Encoding::advance(reinterpret_cast<SpanType&>(*this));
      return(*this);
    }

    constexpr Proxy operator* ()
    {
      return Proxy(*this);
    }
  protected:
    friend _Encoding;
  };

  template<typename _Encoding>
  template<typename WriteEncoding>
  class String<_Encoding>::WriteIterator<WriteEncoding>::Proxy : public SpanBase<typename _Encoding::BufferElement>
  {
  public:
    constexpr Proxy(const SpanBase<typename _Encoding::BufferElement> &init) : SpanBase<typename _Encoding::BufferElement>(init) {};

    void operator = (const InputCodepoint &cp)
    {
      _Encoding::assign(*this, cp);
    }

    InputCodepoint operator* () const
    {
      return (this->buffer_size_ > 0) ? static_cast<InputCodepoint>(OutputCodepoint(*this)) :  InputCodepoint::kSTRING_END() ;
    }
  };


  template<typename _Encoding>
  class String<_Encoding>::Span : public SpanBase<typename _Encoding::BufferElement>
  {
  public:
    constexpr Span(const Span &init) :
      SpanBase<typename _Encoding::BufferElement>(init) {};
    constexpr Span(const SpanBase<typename _Encoding::BufferElement> &init)
      : SpanBase<typename _Encoding::BufferElement>(init) {};

    using SpanBase<typename _Encoding::BufferElement>::SpanBase;

    template<typename WriteEncoding>
    constexpr Writer<WriteEncoding> createWriter() { return Writer<WriteEncoding>(*this); }
  };

  template<typename _Encoding>
  class String<_Encoding>::ConstSpan : public SpanBase<const typename _Encoding::BufferElement>
  {
  public:
    constexpr ConstSpan(const ConstSpan &init)
      : SpanBase<const typename _Encoding::BufferElement>(init) {};
    constexpr ConstSpan(const SpanBase<const typename _Encoding::BufferElement> &init)
      : SpanBase<const typename _Encoding::BufferElement>(init) {};

    using SpanBase<const typename _Encoding::BufferElement>::SpanBase;
  };


  class String_
  {
  public:
    static bool convertToHex(char *hex, uint8_t length, uint16_t value);
    static bool convertToDecimal(char *decimal, uint8_t length, uint16_t value);

    static size_t getLTrimLen(const char* text, size_t len);

    template<size_t ARRSIZE>
    static size_t getLTrimLen(const char (& text)[ARRSIZE]) { return getLTrimLen(text, ARRSIZE);}

    static size_t getRTrimStringLen(const char* text, size_t len);

    template<size_t ARRSIZE>
    static size_t getRTrimStringLen(const char (& text)[ARRSIZE]) { return getRTrimStringLen(text, ARRSIZE);}


    static void
    formatUnsigned(char *buffer, uint8_t digits, uint16_t value, char fill =' ')
    {
     uint_fast8_t i;
     bool overflow;

     overflow = convertToDecimal(buffer, digits, value);

     if(overflow)
     {
       fill = '#';
     }

     for(i = 0; i < digits; ++i)
     {
       if (overflow)
       {
         buffer[i] = fill;
       }
       else if ((i + 1) >= digits)
       {
         break;
       }
       else if (buffer[i] != '0')
       {
         break;
       }
       else
       {
         buffer[i] = fill;
       }
     }
    }

    /** insert a decimal point into the given formated number */
    static void
    formatFrac(char *buffer, uint8_t digits, uint8_t prec)
    {
      uint8_t i;

      for (i = digits; (i > 1); --i)
      {
        if (i < (digits - prec - 1))
          break;

        if (buffer[i-1] == '-')
        {
          buffer[i-2] = '-';
          buffer[i-1] = '0';
        }
        else if (buffer[i-1] == ' ')
        {
          buffer[i-1] = '0';
        }
      }

      while(digits >= 2)
      {
        digits = digits - 1;

        if (prec > 0)
        {
          buffer[digits] = buffer[digits-1];
          prec--;
        }
        else
        {
          buffer[digits] = '.';
          break;
        }
      }
    }

    static void
    formatSigned(char *buffer, int_fast8_t digits, int16_t value, char fill =' ', char sign = '\x00')
    {
      int_fast8_t i;
      bool overflow;

      if(value >= 0)
      {
        overflow = convertToDecimal(buffer, digits, value);
        if (sign == 0)
        {
          sign = fill;
        }
      }
      else
      {
        overflow = convertToDecimal(buffer, digits, -value);
        sign = '-';
        overflow = overflow | (buffer[0] != '0');
      }

      if (overflow)
      {
        memset(buffer, '#', digits);
      }
      else if (buffer[0] == '0' && (digits > 1))
      {
        for(i = 1; i < (digits-1); ++i)
        {
          buffer[i-1] = fill;

          if(buffer[i] != '0')
            break;
        }
        buffer[i-1] = sign;
      }
    }
  };
};




#endif /* ECPP_STRING_HPP_ */
