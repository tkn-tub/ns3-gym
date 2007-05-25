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
#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>
#include <string>
#include "ptr.h"

namespace ns3 {

class MyInterfaceId
{
public:
  static MyInterfaceId LookupByName (std::string name);
  static MyInterfaceId LookupParent (MyInterfaceId iid);
  ~MyInterfaceId ();
private:
  MyInterfaceId (uint32_t iid);
  friend MyInterfaceId MakeInterfaceId (std::string name, const MyInterfaceId &parent);
  friend MyInterfaceId MakeObjectInterfaceId (void);
  friend bool operator == (const MyInterfaceId &a, const MyInterfaceId &b);
  friend bool operator != (const MyInterfaceId &a, const MyInterfaceId &b);
  uint32_t m_iid;
};

MyInterfaceId
MakeInterfaceId (std::string name, const MyInterfaceId &parent);

class Object
{
public:
  static const MyInterfaceId iid;

  Object ();
  virtual ~Object ();
  inline void Ref (void) const;
  inline void Unref (void) const;
  template <typename T>
  Ptr<T> QueryInterface (MyInterfaceId iid);
  void Dispose (void);
  void AddInterface (Ptr<Object> other);
protected:
  void SetInterfaceId (MyInterfaceId iid);
  virtual void DoDispose (void);
private:
  Ptr<Object> DoQueryInterface (MyInterfaceId iid);
  bool Check (void) const;
  void MaybeDelete (void) const;
  mutable uint32_t m_count;
  MyInterfaceId m_iid;
  Object *m_next;
};

} // namespace ns3

namespace ns3 {

void
Object::Ref (void) const
{
  m_count++;
}
void
Object::Unref (void) const
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
Object::QueryInterface (MyInterfaceId iid)
{
  Ptr<Object> found = DoQueryInterface (iid);
  if (found != 0)
    {
      return Ptr<T> (dynamic_cast<T *> (PeekPointer (found)));
    }
  return 0;
}

} // namespace ns3

#endif /* OBJECT_H */

