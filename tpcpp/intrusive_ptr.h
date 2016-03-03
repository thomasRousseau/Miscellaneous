#ifndef SMART_PTR_INTRUSIVE_PTR_H
#define SMART_PTR_INTRUSIVE_PTR_H


//
// intrusive_ptr.h
//
// Copyright (c) 2001, 2002 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/smart_ptr/intrusive_ptr.html for documentation.
//
// Modified by elc@telecom-paristech, Oct.2011:
// - intrusive_ptr can be used without installing the Boost distribution
// - added definition of Pointable, a generic base class with a compatible counter
// - misc. checkings if macro SMART_PTR_DEBUG is defined before including this file
//   (and debug messages on stderr if SMART_PTR_DEBUG_MESSAGES is defined)
//


#ifndef SMART_PTR_DEBUG
# define SMART_PTR_ASSERT(expr) ((void)0)
#else
# include <cassert>
# include <iostream>
# define SMART_PTR_ASSERT(expr) assert(expr)   // BOOST_ASSERT(expr)
# define SMART_PTR_ERROR(msg,obj) (std::cerr<<"!Error: "<<msg<<" (on object: "<<(obj)<<')'<< std::endl)
# ifdef SMART_PTR_DEBUG_MESSAGES
#   define SMART_PTR_MESSAGE(func,obj,count) (std::cerr << "calling " << (func) << " on object " << (obj) << " with ptr_count = " << (count) << std::endl)
# else
#   define SMART_PTR_MESSAGE(func,obj,count)
# endif
#endif


#include <ostream>
# define BOOST_SP_NO_SP_CONVERTIBLE


/** Generic base class for objects that can be pointed by an intrusive_ptr.
 * - @see intrusive_ptr for details
 * - misc. checkings are performed if macro SMART_PTR_DEBUG is defined before
 *   including intrusive_ptr.h
 * - debug messages are displayed on stderr if macros SMART_PTR_DEBUG and
 *   if SMART_PTR_DEBUG_MESSAGES are both defined.
 */
class Pointable {
public:
  Pointable() : _ptr_count_(0) {}

  // NB: the destructor MUST be virtual.
  virtual ~Pointable() {
#ifdef SMART_PTR_DEBUG
    SMART_PTR_MESSAGE("~Pointable", this, _ptr_count_);
    if (_ptr_count_ != 0) {
      if (_ptr_count_ > 0)
        SMART_PTR_ERROR("deleting an object pointed by a smart pointer", this);
      else SMART_PTR_ERROR("deleting an object that is already deleted", this);
    }
    // _ptr_count_ is set to -1 for detecting errors if this object is
    // immediately pointed or deleted again
    _ptr_count_ = -1;
#endif
  }

  // NB: the counter must not be copied.
  Pointable(const Pointable&) : _ptr_count_(0) {}
  Pointable& operator=(const Pointable&) {return *this;}

private:
  // number of smart pointers currently pointing to this object.
  long _ptr_count_;

  friend long intrusive_ptr_get_count(Pointable* p);

  // Boost functions required by intrusive_ptr.
  friend void intrusive_ptr_add_ref(Pointable* p);
  friend void intrusive_ptr_release(Pointable* p);
};


inline long intrusive_ptr_get_count(Pointable* p) {
  return p->_ptr_count_;
}


inline void intrusive_ptr_add_ref(Pointable* p) {
#ifndef SMART_PTR_DEBUG
  ++(p->_ptr_count_);
#else
  SMART_PTR_MESSAGE("intrusive_ptr_add_ref", p, p->_ptr_count_);
  if (p->_ptr_count_ >= 0) ++(p->_ptr_count_);
  else SMART_PTR_ERROR("intrusive_ptr_add_ref() on a deleted object", p);
#endif
}


inline void intrusive_ptr_release(Pointable* p) {
#ifndef SMART_PTR_DEBUG
  if (--(p->_ptr_count_) == 0) delete p;
#else
  SMART_PTR_MESSAGE("intrusive_ptr_release", p, p->_ptr_count_);
  if (p->_ptr_count_ > 1) --(p->_ptr_count_);
  else if (--(p->_ptr_count_) == 0) delete p;
  else SMART_PTR_ERROR("intrusive_ptr_release() on a deleted object", p);
#endif
}



/** intrusive_ptr: a smart pointer that uses intrusive reference counting.
 *  Relies on unqualified calls to
 * <pre>
 *      void intrusive_ptr_add_ref(T * p);
 *      void intrusive_ptr_release(T * p);
 * </pre>
 *           (p != 0)
 *
 *  The object is responsible for destroying itself.
 */
template<class T> class intrusive_ptr
{
private:

  typedef intrusive_ptr this_type;
  typedef T * this_type::*unspecified_bool_type;

public:

  typedef T element_type;

  intrusive_ptr(): px( 0 )
  {
  }

  intrusive_ptr( T * p, bool add_ref = true ): px( p )
  {
    if( px != 0 && add_ref ) intrusive_ptr_add_ref( px );
  }

#if !defined(BOOST_NO_MEMBER_TEMPLATES) || defined(BOOST_MSVC6_MEMBER_TEMPLATES)

  template<class U>
#if !defined( BOOST_SP_NO_SP_CONVERTIBLE )
  intrusive_ptr( intrusive_ptr<U> const & rhs, typename detail::sp_enable_if_convertible<U,T>::type = detail::sp_empty() )
#else
  intrusive_ptr( intrusive_ptr<U> const & rhs )
#endif
  : px( rhs.get() )
  {
    if( px != 0 ) intrusive_ptr_add_ref( px );
  }

#endif

  intrusive_ptr(intrusive_ptr const & rhs): px( rhs.px )
  {
    if( px != 0 ) intrusive_ptr_add_ref( px );
  }

  ~intrusive_ptr()
  {
    if( px != 0 ) intrusive_ptr_release( px );
  }

#if !defined(BOOST_NO_MEMBER_TEMPLATES) || defined(BOOST_MSVC6_MEMBER_TEMPLATES)

  template<class U> intrusive_ptr & operator=(intrusive_ptr<U> const & rhs)
  {
    this_type(rhs).swap(*this);
    return *this;
  }

#endif

  // Move support
#if defined( BOOST_HAS_RVALUE_REFS )

  intrusive_ptr(intrusive_ptr && rhs): px( rhs.px )
  {
    rhs.px = 0;
  }

  intrusive_ptr & operator=(intrusive_ptr && rhs)
  {
    this_type(std::move(rhs)).swap(*this);
    return *this;
  }

#endif

  intrusive_ptr & operator=(intrusive_ptr const & rhs)
  {
    this_type(rhs).swap(*this);
    return *this;
  }

  intrusive_ptr & operator=(T * rhs)
  {
    this_type(rhs).swap(*this);
    return *this;
  }

  void reset()
  {
    this_type().swap( *this );
  }

  void reset( T * rhs )
  {
    this_type( rhs ).swap( *this );
  }

  T * get() const
  {
    return px;
  }

  T & operator*() const
  {
    SMART_PTR_ASSERT( px != 0 );
    return *px;
  }

  T * operator->() const
  {
    SMART_PTR_ASSERT( px != 0 );
    return px;
  }

  // implicit conversion to "bool"
  //#include <boost/smart_ptr/detail/operator_bool.hpp>
  operator unspecified_bool_type() const // never throws
  {
    return px == 0? 0: &this_type::px;
  }

  bool operator! () const // never throws
  {
    return px == 0;
  }

  void swap(intrusive_ptr & rhs)
  {
    T * tmp = px;
    px = rhs.px;
    rhs.px = tmp;
  }

private:

  T * px;
};

template<class T, class U> inline bool operator==(intrusive_ptr<T> const & a, intrusive_ptr<U> const & b)
{
  return a.get() == b.get();
}

template<class T, class U> inline bool operator!=(intrusive_ptr<T> const & a, intrusive_ptr<U> const & b)
{
  return a.get() != b.get();
}

template<class T, class U> inline bool operator==(intrusive_ptr<T> const & a, U * b)
{
  return a.get() == b;
}

template<class T, class U> inline bool operator!=(intrusive_ptr<T> const & a, U * b)
{
  return a.get() != b;
}

template<class T, class U> inline bool operator==(T * a, intrusive_ptr<U> const & b)
{
  return a == b.get();
}

template<class T, class U> inline bool operator!=(T * a, intrusive_ptr<U> const & b)
{
  return a != b.get();
}

#if __GNUC__ == 2 && __GNUC_MINOR__ <= 96

// Resolve the ambiguity between our op!= and the one in rel_ops

template<class T> inline bool operator!=(intrusive_ptr<T> const & a, intrusive_ptr<T> const & b)
{
  return a.get() != b.get();
}

#endif

template<class T> inline bool operator<(intrusive_ptr<T> const & a, intrusive_ptr<T> const & b)
{
  return std::less<T *>()(a.get(), b.get());
}

template<class T> void swap(intrusive_ptr<T> & lhs, intrusive_ptr<T> & rhs)
{
  lhs.swap(rhs);
}

// mem_fn support

template<class T> T * get_pointer(intrusive_ptr<T> const & p)
{
  return p.get();
}

template<class T, class U> intrusive_ptr<T> static_pointer_cast(intrusive_ptr<U> const & p)
{
  return static_cast<T *>(p.get());
}

template<class T, class U> intrusive_ptr<T> const_pointer_cast(intrusive_ptr<U> const & p)
{
  return const_cast<T *>(p.get());
}

template<class T, class U> intrusive_ptr<T> dynamic_pointer_cast(intrusive_ptr<U> const & p)
{
  return dynamic_cast<T *>(p.get());
}

// operator<<

#if !defined(BOOST_NO_IOSTREAM)

#if defined(BOOST_NO_TEMPLATED_IOSTREAMS) || ( defined(__GNUC__) &&  (__GNUC__ < 3) )

template<class Y> std::ostream & operator<< (std::ostream & os, intrusive_ptr<Y> const & p)
{
  os << p.get();
  return os;
}

#else

// in STLport's no-iostreams mode no iostream symbols can be used
#ifndef _STLP_NO_IOSTREAMS
template<class E, class T, class Y> std::basic_ostream<E, T> & operator<< (std::basic_ostream<E, T> & os, intrusive_ptr<Y> const & p)
{
  os << p.get();
  return os;
}

#endif // _STLP_NO_IOSTREAMS

#endif // __GNUC__ < 3

#endif // !defined(BOOST_NO_IOSTREAM)


#ifdef BOOST_MSVC
# pragma warning(pop)
#endif

#endif
