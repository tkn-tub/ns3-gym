/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef ENUM_VALUE_H
#define ENUM_VALUE_H

#include "attribute.h"
#include "attribute-accessor-helper.h"
#include <list>

namespace ns3 {

/**
 * \brief hold variables of type 'enum'
 *
 * This class can be used to hold variables of any kind
 * of enum.
 */
class Enum : public AttributeValue
{
public:
  Enum ();
  Enum (int v);
  void Set (int v);
  int Get (void) const;

  virtual Attribute Copy (void) const;
  virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker);

  Enum (Attribute value);
  operator Attribute () const;
private:
  int m_v;
};

class EnumChecker : public AttributeChecker
{
public:
  EnumChecker ();

  void AddDefault (int v, std::string name);
  void Add (int v, std::string name);

  virtual bool Check (Attribute value) const;
  virtual std::string GetType (void) const;
  virtual bool HasTypeConstraints (void) const;
  virtual std::string GetTypeConstraints (void) const;
  virtual Attribute Create (void) const;

private:
  friend class Enum;
  typedef std::list<std::pair<int,std::string> > ValueSet;
  ValueSet m_valueSet;
};

template <typename T1>
Ptr<const AttributeAccessor> MakeEnumAccessor (T1 a1);

template <typename T1, typename T2>
Ptr<const AttributeAccessor> MakeEnumAccessor (T1 a1, T2 a2);

Ptr<const AttributeChecker> MakeEnumChecker (int v1, std::string n1,
					     int v2 = 0, std::string n2 = "",
					     int v3 = 0, std::string n3 = "",
					     int v4 = 0, std::string n4 = "",
					     int v5 = 0, std::string n5 = "",
					     int v6 = 0, std::string n6 = "",
					     int v7 = 0, std::string n7 = "",
					     int v8 = 0, std::string n8 = "",
					     int v9 = 0, std::string n9 = "",
					     int v10 = 0, std::string n10 = "",
					     int v11 = 0, std::string n11 = "",
					     int v12 = 0, std::string n12 = "");


} // namespace ns3

namespace ns3 {

template <typename T1>
Ptr<const AttributeAccessor> MakeEnumAccessor (T1 a1)
{
  return MakeAccessorHelper<Enum> (a1);
}

template <typename T1, typename T2>
Ptr<const AttributeAccessor> MakeEnumAccessor (T1 a1, T2 a2)
{
  return MakeAccessorHelper<Enum> (a1, a2);
}

} // namespace ns3

#endif /* ENUM_VALUE_H */
