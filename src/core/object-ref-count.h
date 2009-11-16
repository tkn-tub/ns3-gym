/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 Mathieu Lacage
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@gmail.com>
 */
#ifndef OBJECT_REF_COUNT_H
#define OBJECT_REF_COUNT_H

#include "empty.h"

namespace ns3 {

/**
 * This templated class provides the refcounting implementation
 * for the ns3::Object class.
 *
 * The tricky aspect of the implementation of this class is that
 * instead of storing a count directly, it keeps track of a pointer
 * to a count to allow multiple instances of the class to share the
 * same pointer/count. This is mainly used by the aggregation support
 * of the ns3::Object class
 */
template <typename T, typename PARENT = empty>
class ObjectRefCount : public PARENT
{
public:
  ObjectRefCount ()
    : m_count (new int (1))
  {}
  ObjectRefCount (const ObjectRefCount &o)
    : m_count (new int (1))
  {}
  ObjectRefCount & operator = (const ObjectRefCount &o)
  {
    return *this;
  }
  ~ObjectRefCount () 
  {
    m_count = 0;
  }

  /**
   * Get the reference count of the object.  Normally not needed; for language bindings.
   */
  int GetReferenceCount (void) const {
    return *m_count;
  }
  /**
   * Increment the reference count. This method should not be called
   * by user code. Object instances are expected to be used in conjunction
   * of the Ptr template which would make calling Ref unecessary and 
   * dangerous.
   */
  inline void Ref (void) const
    {
      (*m_count)++;
    }
  /**
   * Decrement the reference count. This method should not be called
   * by user code. Object instances are expected to be used in conjunction
   * of the Ptr template which would make calling Ref unecessary and 
   * dangerous.
   */
  inline void Unref (void) const
    {
      (*m_count)--;
      if ((*m_count) == 0)
        {
	  const_cast<ObjectRefCount<T,PARENT>*>(this)->DoDelete ();
        }
    }
protected:
  /**
   * \param other another object
   *
   * This method makes this object and the input other object
   * share the same reference count.
   */
  void ShareCount (ObjectRefCount *other)
    {
      (*m_count) += (*other->m_count);
      delete other->m_count;
      other->m_count = m_count;
    }
  /**
   * Called just before deleting this object: when two
   * objects share the same reference count, the user
   * who is deleting them must be careful to delete the
   * associated count only once and this is done by calling 
   * this method to get a reference to the count and, then,
   * calling delete on the count.
   *
   * \sa ns3::Object::DoDelete
   */
  int *PeekCountPtr (void) const
    {
      return m_count;
    }
private:
  virtual void DoDelete (void) = 0;
  mutable int *m_count;
};

} // namespace ns3

#endif /* OBJECT_REF_COUNT_H */
