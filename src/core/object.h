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

class InterfaceId
{
public:
  static InterfaceId LookupByName (std::string name);
  static InterfaceId LookupParent (InterfaceId iid);
  ~InterfaceId ();
private:
  InterfaceId (uint32_t iid);
  friend InterfaceId MakeInterfaceId (std::string name, const InterfaceId &parent);
  friend InterfaceId MakeObjectInterfaceId (void);
  friend bool operator == (const InterfaceId &a, const InterfaceId &b);
  friend bool operator != (const InterfaceId &a, const InterfaceId &b);
  uint32_t m_iid;
};

InterfaceId
MakeInterfaceId (std::string name, const InterfaceId &parent);

class Object
{
public:
  static const InterfaceId iid;

  Object ();
  virtual ~Object ();
  inline void Ref (void) const;
  inline void Unref (void) const;
  template <typename T>
  Ptr<T> QueryInterface (InterfaceId iid) const;
  void Dispose (void);
  void AddInterface (Ptr<Object> other);
protected:
  void SetInterfaceId (InterfaceId iid);
  virtual void DoDispose (void);
private:
  Ptr<Object> DoQueryInterface (InterfaceId iid) const;
  bool Check (void) const;
  void MaybeDelete (void) const;
  mutable uint32_t m_count;
  InterfaceId m_iid;
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
Object::QueryInterface (InterfaceId iid) const
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

