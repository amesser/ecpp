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

#include <ecpp/Datatypes.hpp>
#include <ecpp/VaTemplate.hpp>
#include <util/vamath.hpp>


namespace ecpp {
  using namespace ::Platform::Util::VaMath;
  using ::ecpp::indices;
  using ::ecpp::build_indices;

  template<size_t Count, typename TYPE>
  class BufferBase
  {
  public:
    typedef TYPE Type;
    typedef TYPE ArrayType[Count];

    static constexpr size_t count() {return Count;}
    static constexpr size_t size()  {return count() * sizeof(Type);}

  protected:
    ArrayType _data;

  public:
    template<indices_type ...Is>
    constexpr BufferBase(const Type (&init)[Count], indices<Is...>) : _data{init[Is]...} {}

    template<indices_type ...Is>
    constexpr BufferBase(const Type (&init), indices<Is...>) : _data{(&init)[0*Is]...} {}

    constexpr BufferBase(const Type (&init)[Count]) : BufferBase(init, build_indices<Count>()) {}
    constexpr BufferBase(const Type &init) : BufferBase(init, build_indices<Count>()) {}
    constexpr BufferBase() : BufferBase(0, build_indices<Count>()) {}

    constexpr ArrayType const & data() const {return this->_data;}
    ArrayType & data() {return this->_data;}
  };


  template<typename Type, template<size_t, typename> class Buffer>
  class BufferIterator;

  template<typename Type, template<size_t, typename> class Buffer>
  class ConstBufferIterator;

  template<typename Type, template<size_t, typename> class Buffer>
  class BufferIterator {};

  template<typename Type, template<size_t, typename> class Buffer>
  class ConstBufferIterator {};

  template<size_t Count, typename Type>
  class RamBufferBase : public BufferBase<Count,Type>
  {
  public:
    typedef BufferIterator<Type, RamBufferBase > iterator;
    typedef ConstBufferIterator<Type, RamBufferBase> const_iterator;

    typedef RamBufferBase<Count, Type> BaseType;

    constexpr RamBufferBase() : BufferBase<Count,Type>() {}
    constexpr RamBufferBase(const Type &init) : BufferBase<Count,Type>(init) {}
    constexpr RamBufferBase(const Type (&init)[Count]) : BufferBase<Count,Type>(init) {}

    constexpr Type at(size_t index) const {return this->_data[index];}
    Type & at(size_t index) {return this->_data[index];}
  };

  template<typename Type>
  class ConstBufferIterator<Type, RamBufferBase>
  {
  private:
    const Type           *_data;
  public:
    typedef ConstBufferIterator<Type, RamBufferBase> self_type;

    constexpr ConstBufferIterator() : _data(0) {};
    constexpr ConstBufferIterator(const Type *data) : _data(data) {}

    constexpr const Type & operator * () const {return *_data;}

    constexpr bool operator == (const self_type &rhs) {return this->_data == rhs._data;}
    constexpr bool operator != (const self_type &rhs) {return this->_data != rhs._data;}
    self_type & operator ++ () {_data++; return *this;}
    self_type operator ++ (int) {return self_type(_data++);}

    constexpr self_type operator + (size_t count) const
    {
      return {_data + count};
    }
  };


  template<size_t Count, typename Type = uint8_t, typename Storage = RamBufferBase<Count,Type> >
  class Buffer : public Storage
  {
  public:

    constexpr Buffer() : Storage() {};
    constexpr Buffer(const Type &init) : Storage(init) {}

    constexpr Buffer(const Type (&init)[Count]) : Storage(init) {}
    constexpr Buffer(const Type (*init)[Count]) : Storage(*init) {}


    template<typename T>
    constexpr Buffer(Buffer<Count, Type, T> const &init) : Storage(init.data()) {}

    constexpr Type operator [] (size_t index) const {return Storage::at(index);}
    Type & operator [] (size_t index)  {return this->at(index);}

    constexpr typename Storage::const_iterator begin() const { return &this->_data[0];}
    constexpr typename Storage::const_iterator end()   const { return &this->_data[Count];}

    template< template<size_t, typename> class NewStorage>
    constexpr Buffer<Count,Type, NewStorage<Count,Type> > in () const { return {this->_data}; }

    template<size_t lhs_size, size_t rhs_size, size_t ...Is, size_t ...Ir>
    static constexpr Buffer<lhs_size + rhs_size, Type>
    join(BufferBase<lhs_size, Type> const & lhs, BufferBase<rhs_size, Type> const & rhs,
        indices<Is...>, indices<Ir...>)
    {
      return {{lhs.data()[Is]..., rhs.data()[Ir]...}};
    }

    template<size_t lhs_size, size_t rhs_size>
    static constexpr Buffer<lhs_size + rhs_size, Type>
    join(BufferBase<lhs_size, Type> const & lhs, BufferBase<rhs_size, Type> const & rhs)
    {
      return join(lhs,rhs, build_indices<lhs_size>(), build_indices<rhs_size>());
    }

    template<size_t OtherCount, template<size_t, typename> class OtherStorage>
    constexpr Buffer<Count + OtherCount, Type>
    operator+(Buffer<OtherCount, Type, OtherStorage<Count,Type> > const & rhs) {
      return join(*this,rhs);
    }

    template<template<size_t, typename> class OtherStorage>
    using OtherBuffer = Buffer<Count, Type, typename OtherStorage<Count, Type>::BaseType >;
  };


  template<size_t Count, typename Type, typename Storage>
  constexpr size_t bufferSize(Buffer<Count, Type, Storage> const & buf)
  {
    return buf.size();
  }

  template<size_t Count, typename Type, typename Storage>
  constexpr size_t bufferCount(Buffer<Count, Type, Storage> const & buf)
  {
    return buf.count();
  }

  template<size_t Size, typename Type = uint8_t>
  using RamBuffer = Buffer<Size, Type, RamBufferBase<Size, Type> >;

  template<typename Buffer, size_t ...Is>
  constexpr RamBuffer<Buffer::count()+1, typename Buffer::Type>
  _append(Buffer const& left, typename Buffer::Type const& val, indices<Is...>)
  {
    return {{left[Is]..., val}};
  }


  template<typename Type, typename StorageLeft, typename StorageRight, size_t ...Is, size_t ...Ir>
  constexpr RamBuffer<indices<Is..., Ir...>::count(), Type>
  _join(Buffer<indices<Is...>::count(), Type, StorageLeft> const& left,
        Buffer<indices<Ir...>::count(), Type, StorageRight> const& right,
        indices<Is...>, indices<Ir...>)
  {
    return {{left[Is]..., right[Ir]...}};
  }

  template<typename Buffer>
  constexpr RamBuffer<Buffer::count()+1, typename Buffer::Type>
  append(Buffer const &left, typename Buffer::Type const& val)
  {
    return _append(left,val, build_indices<Buffer::count()>{});
  }


  constexpr RamBuffer<0, uint8_t> join()
  {
    return {};
  }

  template<typename Type, typename Storage>
  constexpr RamBuffer<Storage::count(), Type>
  join(Buffer<Storage::count(), Type, Storage> const & left)
  {
    return {left};
  }

  template<typename Type, typename StorageLeft, typename StorageRight, typename ...Storages>
  constexpr RamBuffer<sum(StorageLeft::count(), StorageRight::count(), Storages::count()...), Type>
  join(Buffer<StorageLeft::count(), Type, StorageLeft> const & left,
       Buffer<StorageRight::count(), Type, StorageRight> const & right,
       Buffer<Storages::count(), Type, Storages> const & ...rest)
  {
    return {
      join(
        _join(left, right, build_indices<left.count()>{}, build_indices<right.count()>{}),
         rest...)
    };
  }

  template<unsigned int Count, typename Type, typename Base >
  constexpr typename UnsignedIntTypeEstimator<Count>::Type ElementCount(const Buffer<Count,Type,Base>&)
  {
    return UnsignedIntTypeEstimator<Count>::Value;
  };
}

#endif /* BUFFER_HPP_ */
