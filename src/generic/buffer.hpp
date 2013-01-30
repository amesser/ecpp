/*
 *  Copyright 2013 Andreas Messer <andi@bastelmap.de>
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
#ifndef BUFFER_HPP_
#define BUFFER_HPP_

#include <util/datatypes.hpp>

namespace Platform {
  namespace Buffer {
    template<typename TYPE, unsigned long SIZE>
    class Ringbuffer {
    public:
      typedef typename ::Platform::Util::Datatypes::UnsignedIntTypeEstimator<SIZE>::Type IndexType;

    private:

      volatile IndexType _WriteIndex;
      volatile IndexType _ReadIndex;

      TYPE _buf[SIZE];

    public:

      constexpr IndexType getSize() const {return SIZE;}
      IndexType getCount() const {return _WriteIndex - _ReadIndex;}

      void reset() {
        _WriteIndex = 0;
        _ReadIndex = 0;
      }

      TYPE (& getBuffer()) [SIZE] { return _buf;}

      TYPE pop() {
        const IndexType ReadIndex = _ReadIndex;

        while (static_cast<IndexType>((_WriteIndex - ReadIndex)) == 0);

        TYPE data = _buf[ReadIndex % getSize()];
        _ReadIndex = ReadIndex + 1;

        return data;
      }

      TYPE popForced() {
        const IndexType ReadIndex = _ReadIndex;

        TYPE data = _buf[ReadIndex % getSize()];
        _ReadIndex = ReadIndex + 1;

        return data;
      }

      void push(const TYPE& data) {
        const IndexType WriteIndex = _WriteIndex;

        while (static_cast<IndexType>(WriteIndex - _ReadIndex) == getSize());

        _buf[WriteIndex % getSize()] = data;
        _WriteIndex = WriteIndex+1;

      }

      void pushForced(const TYPE& data) {
        const IndexType WriteIndex = _WriteIndex;

        _buf[WriteIndex % getSize()] = data;
        _WriteIndex = WriteIndex+1;

      }

      inline TYPE & front() {
        return _buf[_ReadIndex % getSize()];
      }

      inline TYPE & back() {
        return _buf[(_WriteIndex - 1) % getSize()];
      }

    };
  }
}



#endif /* BUFFER_HPP_ */
