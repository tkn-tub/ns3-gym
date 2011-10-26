/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA, Mathieu Lacage
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
 * Authors: Mathieu Lacage <mathieu.lacage@gmail.com>
 */
#ifndef OBJECT_MAP_H
#define OBJECT_MAP_H

#include <map>
#include "object.h"
#include "ptr.h"
#include "attribute.h"
#include "object-ptr-map.h"

namespace ns3 {

typedef ObjectPtrMapValue ObjectMapValue;

template <typename T, typename U>
Ptr<const AttributeAccessor>
MakeObjectMapAccessor (U T::*memberContainer);

template <typename T>
Ptr<const AttributeChecker> MakeObjectMapChecker (void);

template <typename T, typename U, typename INDEX>
Ptr<const AttributeAccessor>
MakeObjectMapAccessor (Ptr<U> (T::*get)(INDEX) const,
                          INDEX (T::*getN)(void) const);

template <typename T, typename U, typename INDEX>
Ptr<const AttributeAccessor>
MakeObjectMapAccessor (INDEX (T::*getN)(void) const,
                          Ptr<U> (T::*get)(INDEX) const);

template <typename T, typename U>
Ptr<const AttributeAccessor>
MakeObjectMapAccessor (U T::*memberMap)
{
  struct MemberStdContainer : public ObjectPtrMapAccessor
  {
    virtual bool DoGetN (const ObjectBase *object, uint32_t *n) const {
      const T *obj = dynamic_cast<const T *> (object);
      if (obj == 0)
        {
          return false;
        }

      *n = (obj->*m_memberMap).size ();
      return true;
    }
    virtual Ptr<Object> DoGet (const ObjectBase *object, uint32_t i, uint32_t *index) const {
      const T *obj = static_cast<const T *> (object);
      typename U::const_iterator begin = (obj->*m_memberMap).begin ();
      typename U::const_iterator end = (obj->*m_memberMap).end ();
      uint32_t k = 0;
      for (typename U::const_iterator j = begin; j != end; j++, k++)
        {
          if (k == i)
            {
              *index = (*j).first;
              return (*j).second;
              break;
            }
        }
      NS_ASSERT (false);
      // quiet compiler.
      return 0;
    }
    U T::*m_memberMap;
  } *spec = new MemberStdContainer ();
  spec->m_memberMap = memberMap;
  return Ptr<const AttributeAccessor> (spec, false);
}

template <typename T>
Ptr<const AttributeChecker> MakeObjectMapChecker (void)
{
  return MakeObjectPtrMapChecker<T> ();
}

template <typename T, typename U, typename INDEX>
Ptr<const AttributeAccessor>
MakeObjectMapAccessor (Ptr<U> (T::*get)(INDEX) const,
		       INDEX (T::*getN)(void) const)
{
  return MakeObjectPtrMapAccessor<T,U,INDEX>(get, getN);
}

template <typename T, typename U, typename INDEX>
Ptr<const AttributeAccessor>
MakeObjectMapAccessor (INDEX (T::*getN)(void) const,
		       Ptr<U> (T::*get)(INDEX) const)
{
  return MakeObjectPtrMapAccessor<T,U,INDEX>(get, getN);
}



} // namespace ns3

#endif /* OBJECT_MAP_H */
