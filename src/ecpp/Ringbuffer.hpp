/*
 * Ringbuffer.hpp
 *
 *  Created on: 14.05.2015
 *      Author: andi
 */

#ifndef ECPP_RINGBUFFER_HPP_
#define ECPP_RINGBUFFER_HPP_

#include "ecpp/Datatypes.hpp"

namespace ecpp {
  template<typename TYPE, unsigned long SIZE>
  class Ringbuffer {
  public:
    typedef typename UnsignedIntTypeEstimator<SIZE>::Type IndexType;

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

    constexpr IndexType getInsertPos() const
    {
      return _WriteIndex % getSize();
    }

    TYPE* getInsertElem()
    {
      if(getCount() < getSize())
      {
        return &_buf[getInsertPos()];
      }
      {
        return 0;
      }
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

    void pushForced()
    {
      const IndexType WriteIndex = _WriteIndex;
      _WriteIndex = WriteIndex+1;
    }

    inline TYPE & front() {
      return _buf[_ReadIndex % getSize()];
    }

    inline const TYPE & front() const {
      return _buf[_ReadIndex % getSize()];
    }

    inline TYPE & back() {
      return _buf[(_WriteIndex - 1) % getSize()];
    }

    inline const TYPE & back() const {
      return _buf[(_WriteIndex - 1) % getSize()];
    }

  };
}

#endif /* ECPP_RINGBUFFER_HPP_ */
