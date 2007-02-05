/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
 * All rights reserved.
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef REFERENCE_LIST_H
#define REFERENCE_LIST_H

/* This is a reference list implementation. The technique underlying
 * this code was first described in 1995 by Risto Lankinen on Usenet
 * but I have never been able to find his original posting. Instead,
 * this code is based on the description of the technique found in
 * "Modern C++ design" by Andrei Alexandrescu in chapter 7.
 */


namespace ns3 {

template <typename OBJ_PTR>
class ReferenceList;

template <typename OBJ_PTR>
class ReferenceList {
public:
  ReferenceList () 
      :  m_objPtr (),
         m_prev (), 
         m_next () 
  {
      m_prev = this;
      m_next = this;
  }
  ReferenceList (ReferenceList &o) 
      : m_objPtr (),
        m_prev (), 
        m_next ()
  {
      m_prev = this;
      m_next = this;
      InsertSelfInOther (o);
  }
  ReferenceList (ReferenceList const&o) 
      : m_objPtr (),
        m_prev (), 
        m_next ()
  {
      m_prev = this;
      m_next = this;
      InsertSelfInOther (o);
  }
  ReferenceList (OBJ_PTR const &objPtr)
      : m_objPtr (objPtr), 
        m_prev (), 
        m_next ()
  {
      m_prev = this;
      m_next = this;
  }
  ~ReferenceList () {
      RemoveFromList ();
  }
  ReferenceList & operator= (ReferenceList const&o) {
      RemoveFromList ();
      InsertSelfInOther (o);
      return *this;
  }
  OBJ_PTR operator-> () const {
      return m_objPtr;
  }
  void Set (OBJ_PTR objPtr) {
      RemoveFromList ();
      m_objPtr = objPtr;
  }
  OBJ_PTR Get (void) const {
      // explicit conversion to raw pointer type.
      return m_objPtr;
  }
private:
  void InsertSelfInOther (ReferenceList const&o) {
      m_prev = &o;
      m_next = o.m_next;
      m_next->m_prev = this;
      o.m_next = this;
      m_objPtr = o.m_objPtr;
  }
  void RemoveFromList (void) {
      if (m_prev == this) 
        {
          //assert (m_next == this);
          delete m_objPtr;
          m_objPtr = OBJ_PTR ();
        }
      m_prev->m_next = m_next;
      m_next->m_prev = m_prev;
  }
  OBJ_PTR m_objPtr;
  mutable ReferenceList const*m_prev;
  mutable ReferenceList const*m_next;
};

}; // namespace ns3

#endif /* REFERENCE_LIST_H */
