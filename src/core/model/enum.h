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
 * \ingroup attribute
 *
 * \brief hold variables of type 'enum'
 *
 * This class can be used to hold variables of any kind
 * of enum.
 */
class EnumValue : public AttributeValue
{
public:
  EnumValue ();
  EnumValue (int v);
  void Set (int v);
  int Get (void) const;
  template <typename T>
  bool GetAccessor (T &v) const;

  virtual Ptr<AttributeValue> Copy (void) const;
  virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker);

private:
  int m_v;
};

template <typename T>
bool EnumValue::GetAccessor (T &v) const
{
  v = T (m_v);
  return true;
}

class EnumChecker : public AttributeChecker
{
public:
  EnumChecker ();

  void AddDefault (int v, std::string name);
  void Add (int v, std::string name);

  virtual bool Check (const AttributeValue &value) const;
  virtual std::string GetValueTypeName (void) const;
  virtual bool HasUnderlyingTypeInformation (void) const;
  virtual std::string GetUnderlyingTypeInformation (void) const;
  virtual Ptr<AttributeValue> Create (void) const;
  virtual bool Copy (const AttributeValue &src, AttributeValue &dst) const;

private:
  friend class EnumValue;
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
                                             int v12 = 0, std::string n12 = "",
                                             int v13 = 0, std::string n13 = "",
                                             int v14 = 0, std::string n14 = "",
                                             int v15 = 0, std::string n15 = "",
                                             int v16 = 0, std::string n16 = "",
                                             int v17 = 0, std::string n17 = "",
                                             int v18 = 0, std::string n18 = "",
                                             int v19 = 0, std::string n19 = "",
                                             int v20 = 0, std::string n20 = "",
                                             int v21 = 0, std::string n21 = "",
                                             int v22 = 0, std::string n22 = "");


} // namespace ns3

namespace ns3 {

template <typename T1>
Ptr<const AttributeAccessor> MakeEnumAccessor (T1 a1)
{
  return MakeAccessorHelper<EnumValue> (a1);
}

template <typename T1, typename T2>
Ptr<const AttributeAccessor> MakeEnumAccessor (T1 a1, T2 a2)
{
  return MakeAccessorHelper<EnumValue> (a1, a2);
}

} // namespace ns3

#endif /* ENUM_VALUE_H */
