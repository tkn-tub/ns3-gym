/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA, Gustavo Carneiro
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
 * Authors: Gustavo Carneiro <gjcarneiro@gmail.com>,
 *          Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef INTERFACE_OBJECT_H
#define INTERFACE_OBJECT_H

#include <stdint.h>
#include <string>
#include "ptr.h"

namespace ns3 {

class MyInterfaceId
{
public:
  static MyInterfaceId LookupByName (std::string name);
  static MyInterfaceId LookupParent (MyInterfaceId iid);
private:
  MyInterfaceId (uint32_t iid);
  friend MyInterfaceId MakeInterfaceId (std::string name, const MyInterfaceId &parent);
  friend MyInterfaceId MakeObjectInterfaceId (void);
  friend class AggregateObject;
  friend bool operator == (const MyInterfaceId &a, const MyInterfaceId &b);
  friend bool operator != (const MyInterfaceId &a, const MyInterfaceId &b);
  uint32_t m_iid;
};

MyInterfaceId
MakeInterfaceId (std::string name, const MyInterfaceId &parent);

class AggregateObject;

class InterfaceObject
{
public:
  static const MyInterfaceId iid;

  InterfaceObject ();
  virtual ~InterfaceObject ();
  inline void Ref (void);
  inline void Unref (void);
  template <typename T>
  Ptr<T> QueryInterface (MyInterfaceId iid);
  void Dispose (void);
  void Add (Ptr<InterfaceObject> other);
protected:
  void SetInterfaceId (MyInterfaceId iid);
private:
  friend class AggregateObject;
  virtual void DoDispose (void);
  Ptr<InterfaceObject> DoQueryInterface (MyInterfaceId iid);
  bool Check (void);
  void MaybeDelete (void);
  uint32_t m_count;
  MyInterfaceId m_iid;
  AggregateObject *m_aggregate;
};

} // namespace ns3

namespace ns3 {

void
InterfaceObject::Ref (void)
{
  m_count++;
}
void
InterfaceObject::Unref (void)
{
  NS_ASSERT (Check ());
  m_count--;
  if (m_count == 0)
    {
      MaybeDelete ();
    }
}

template <typename T>
Ptr<T> 
InterfaceObject::QueryInterface (MyInterfaceId iid)
{
  Ptr<InterfaceObject> found = DoQueryInterface (iid);
  if (found != 0)
    {
      return Ptr<T> (dynamic_cast<T *> (PeekPointer (found)));
    }
  return 0;
}

} // namespace ns3

#endif /* INTERFACE_OBJECT_H */

