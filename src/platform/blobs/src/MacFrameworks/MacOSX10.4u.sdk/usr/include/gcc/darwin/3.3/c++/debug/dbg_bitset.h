/* APPLE LOCAL file libstdc++ debug mode */
// Debugging bitset implementation -*- C++ -*-

// Copyright (C) 2003
// Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.

// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.

#ifndef _GLIBCXX_DEBUG_BITSET_H
#define _GLIBCXX_DEBUG_BITSET_H

#include <debug/safe_sequence.h>
#include <debug/safe_iterator.h>

_GLIBCXX_DEBUG_NAMESPACE_OPEN(std)

template<size_t _Nb> 
  class _GLIBCXX_DEBUG_CLASS(bitset) bitset 
  : public _GLIBCXX_DEBUG_BASE(std, bitset)<_Nb>, 
    public std::__debug::_Safe_sequence_base
  {
    typedef _GLIBCXX_DEBUG_BASE(std, bitset)<_Nb> _Base;
    typedef std::__debug::_Safe_sequence_base     _Safe_base;

  public:
    // bit reference:
    // Here we use only the trivial iterator portion of the
    // _Safe_iterator machinery. References don't move like iterators,
    // but they can be invalidated, e.g., when the container is destroyed.
    class reference 
    : private _Base::reference, public std::__debug::_Safe_iterator_base
    {
      typedef typename _Base::reference _Base_ref;

      friend class bitset;
      reference();

      reference(const _Base_ref& __base, bitset* __seq) 
      : _Base_ref(__base), _Safe_iterator_base(__seq, false)
      { }

    public:
      reference(const reference& __x)
      : _Base_ref(__x), _Safe_iterator_base(__x, false)
      { }

      reference& 
      operator=(bool __x)
      {
	_GLIBCXX_DEBUG_VERIFY(! this->_M_singular(),
	  _M_message(::std::__debug::__dbg_msg_bad_bitset_write)
	  ._M_iterator(*this));
	*static_cast<_Base_ref*>(this) = __x;
	return *this;
      }

      reference& 
      operator=(const reference& __x)
      {
	_GLIBCXX_DEBUG_VERIFY(! __x._M_singular(),
	  _M_message(::std::__debug::__dbg_msg_bad_bitset_read)
	  ._M_iterator(__x));
	_GLIBCXX_DEBUG_VERIFY(! this->_M_singular(),
	  _M_message(::std::__debug::__dbg_msg_bad_bitset_write)
	  ._M_iterator(*this));
	*static_cast<_Base_ref*>(this) = __x;
	return *this;
      }

      bool 
      operator~() const
      {
	_GLIBCXX_DEBUG_VERIFY(! this->_M_singular(),
	  _M_message(::std::__debug::__dbg_msg_bad_bitset_read)
	  ._M_iterator(*this));
	return ~(*static_cast<const _Base_ref*>(this));
      }

      operator bool() const
      {
	_GLIBCXX_DEBUG_VERIFY(! this->_M_singular(),
	  _M_message(::std::__debug::__dbg_msg_bad_bitset_read)
	  ._M_iterator(*this));
	return *static_cast<const _Base_ref*>(this);
      }

      reference& 
      flip()
      {
	_GLIBCXX_DEBUG_VERIFY(! this->_M_singular(),
	  _M_message(::std::__debug::__dbg_msg_bad_bitset_flip)
	  ._M_iterator(*this));
	_Base_ref::flip();
	return *this;
      }
    };

    // 23.3.5.1 constructors:
    bitset() : _Base() { }

    bitset(unsigned long __val) : _Base(__val) { }

    template<class _CharT, class _Traits, class _Allocator>
      explicit 
      bitset(const std::basic_string<_CharT,_Traits,_Allocator>& __str,
	     typename std::basic_string<_CharT,_Traits,_Allocator>::size_type
	       __pos = 0,
	     typename std::basic_string<_CharT,_Traits,_Allocator>::size_type
  	       __n = (std::basic_string<_CharT,_Traits,_Allocator>::npos))
      : _Base(__str, __pos, __n)
      { }

    bitset(const _Base& __x) : _Base(__x), _Safe_base() { }

    // 23.3.5.2 bitset operations:
    bitset<_Nb>& 
    operator&=(const bitset<_Nb>& __rhs)
    {
      _M_base() &= __rhs;
      return *this;
    }

    bitset<_Nb>& 
    operator|=(const bitset<_Nb>& __rhs)
    {
      _M_base() != __rhs;
      return *this;
    }

    bitset<_Nb>& 
    operator^=(const bitset<_Nb>& __rhs)
    {
      _M_base() ^= __rhs;
      return *this;
    }

    bitset<_Nb>& 
    operator<<=(size_t __pos)
    {
      _M_base() <<= __pos;
      return *this;
    }

    bitset<_Nb>& 
    operator>>=(size_t __pos)
    {
      _M_base() >>= __pos;
      return *this;
    }

    bitset<_Nb>& 
    set()
    {
      _Base::set();
      return *this;
    }

    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 186. bitset::set() second parameter should be bool 
    bitset<_Nb>& 
    set(size_t __pos, bool __val = true)
    {
      _Base::set(__pos, __val);
      return *this;
    }

    bitset<_Nb>& 
    reset()
    {
      _Base::reset();
      return *this;
    }

    bitset<_Nb>& 
    reset(size_t __pos)
    {
      _Base::reset(__pos);
      return *this;
    }

    bitset<_Nb> operator~() const { return bitset(~_M_base()); }

    bitset<_Nb>& 
    flip()
    {
      _Base::flip();
      return *this;
    }

    bitset<_Nb>& 
    flip(size_t __pos)
    {
      _Base::flip(__pos);
      return *this;
    }

    // element access:
    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 11. Bitset minor problems 
    reference 
    operator[](size_t __pos)
    { 
      __glibcxx_check_subscript(__pos);
      return reference(_M_base()[__pos], this); 
    }

    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 11. Bitset minor problems 
    bool 
    operator[](size_t __pos) const 
    { 
      __glibcxx_check_subscript(__pos);
      return _M_base()[__pos]; 
    }

    using _Base::to_ulong;

    template <class _CharT, class _Traits, class _Allocator>
      std::basic_string<_CharT, _Traits, _Allocator> 
      to_string() const
      { return _M_base().template to_string<_CharT, _Traits, _Allocator>(); }

    using _Base::count;
    using _Base::size;
    
    bool 
    operator==(const bitset<_Nb>& __rhs) const
    { return _M_base() == __rhs; }

    bool 
    operator!=(const bitset<_Nb>& __rhs) const
    { return _M_base() != __rhs; }

    using _Base::test;
    using _Base::any;
    using _Base::none;

    bitset<_Nb> 
    operator<<(size_t __pos) const
    { return bitset<_Nb>(_M_base() << __pos); }

    bitset<_Nb> 
    operator>>(size_t __pos) const
    { return bitset<_Nb>(_M_base() >> __pos); }

    _Base&       _M_base()       { return *this; }
    const _Base& _M_base() const { return *this; }
  };

template <size_t _Nb>
  bitset<_Nb> operator&(const bitset<_Nb>& __x, const bitset<_Nb>& __y)
  { return bitset<_Nb>(__x) &= __y; }

template <size_t _Nb>
  bitset<_Nb> operator|(const bitset<_Nb>& __x, const bitset<_Nb>& __y)
  { return bitset<_Nb>(__x) |= __y; }

template <size_t _Nb>
  bitset<_Nb> operator^(const bitset<_Nb>& __x, const bitset<_Nb>& __y)
  { return bitset<_Nb>(__x) ^= __y; }

template <class _CharT, class _Traits, size_t _Nb>
  std::basic_istream<_CharT, _Traits>&
  operator>>(std::basic_istream<_CharT, _Traits>& __is, bitset<_Nb>& __x)
  { return __is >> __x._M_base(); }

template <class _CharT, class _Traits, size_t _Nb>
  std::basic_ostream<_CharT, _Traits>&
  operator<<(std::basic_ostream<_CharT, _Traits>& __os, const bitset<_Nb>& __x)
  { return __os << __x._M_base(); }

_GLIBCXX_DEBUG_NAMESPACE_CLOSE

#endif /* _GLIBCXX_DEBUG_BITSET_H */
