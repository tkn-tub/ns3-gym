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

//  Additional docs for class DoubleValue:
/**
 * Hold variables of type \c enum
 *
 * This class can be used to hold variables of any kind
 * of enum.
 *
 * This is often used with ObjectFactory and Config to bind
 * the value of a particular enum to an Attribute or Config name.
 * For example,
 * \code
 *   Ptr<RateErrorModel> model = CreateObjectWithAttributes<RateErrorModel> (
 *     "ErrorRate", DoubleValue (0.05),
 *     "ErrorUnit", EnumValue (RateErrorModel::ERROR_UNIT_PACKET));
 *
 *   Config::SetDefault ("ns3::RipNg::SplitHorizon",
 *                       EnumValue (RipNg::NO_SPLIT_HORIZON));
 * \endcode
 */
class EnumValue : public AttributeValue
{
public:
  EnumValue ();
  /**
   * Construct from an explicit value.
   *
   * \param [in] value The value to begin with.
   */
  EnumValue (int value);
  void Set (int  value);
  int Get (void) const;
  template <typename T>
  bool GetAccessor (T & value) const;

  virtual Ptr<AttributeValue> Copy (void) const;
  virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker);

private:
  int m_value;  //!< The stored integer value.
};

template <typename T>
bool
EnumValue::GetAccessor (T &value) const
{
  value = T (m_value);
  return true;
}

class EnumChecker : public AttributeChecker
{
public:
  EnumChecker ();

  /**
   * Add a default value.
   * \param [in] value The value.
   * \param [in] name Then enum symbol name.
   */
  void AddDefault (int value, std::string name);
  /**
   * Add a new value.
   * \param [in] value The value.
   * \param [in] name Then enum symbol name.
   */
  void Add (int value, std::string name);

  virtual bool Check (const AttributeValue &value) const;
  virtual std::string GetValueTypeName (void) const;
  virtual bool HasUnderlyingTypeInformation (void) const;
  virtual std::string GetUnderlyingTypeInformation (void) const;
  virtual Ptr<AttributeValue> Create (void) const;
  virtual bool Copy (const AttributeValue &src, AttributeValue &dst) const;

private:
  friend class EnumValue;
  /** Type of container for storing Enum values and symbol names. */
  typedef std::list<std::pair<int,std::string> > ValueSet;
  /** The stored Enum values and symbol names. */
  ValueSet m_valueSet;
};

template <typename T1>
Ptr<const AttributeAccessor> MakeEnumAccessor (T1 a1);

template <typename T1, typename T2>
Ptr<const AttributeAccessor> MakeEnumAccessor (T1 a1, T2 a2);

/**
 * Make an EnumChecker pre-configured with a set of allowed
 * values by name.
 *
 * Values are normally given as fully qualified enum symbols
 * with matching names.  For example,
 * \c MakeEnumChecker (RipNg::SPLIT_HORIZON, "ns3::RipNg::SplitHorizon");
 *
 * \see AttributeChecker
 *
 * \returns The AttributeChecker
 * \param [in] v1  An enum value
 * \param [in] n1  The corresponding name.
 * \param [in] v2  A enum value
 * \param [in] n2  The corresponding name.
 * \param [in] v3  A enum value
 * \param [in] n3  The corresponding name.
 * \param [in] v4  A enum value
 * \param [in] n4  The corresponding name.
 * \param [in] v5  A enum value
 * \param [in] n5  The corresponding name.
 * \param [in] v6  A enum value
 * \param [in] n6  The corresponding name.
 * \param [in] v7  A enum value
 * \param [in] n7  The corresponding name.
 * \param [in] v8  A enum value
 * \param [in] n8  The corresponding name.
 * \param [in] v9  A enum value
 * \param [in] n9  The corresponding name.
 * \param [in] v10 An enum value
 * \param [in] n10 The enum name.
 * \param [in] v11 An enum value
 * \param [in] n11 The corresponding name.
 * \param [in] v12 A enum value
 * \param [in] n12 The corresponding name.
 * \param [in] v13 A enum value
 * \param [in] n13 The corresponding name.
 * \param [in] v14 A enum value
 * \param [in] n14 The corresponding name.
 * \param [in] v15 A enum value
 * \param [in] n15 The corresponding name.
 * \param [in] v16 A enum value
 * \param [in] n16 The corresponding name.
 * \param [in] v17 A enum value
 * \param [in] n17 The corresponding name.
 * \param [in] v18 A enum value
 * \param [in] n18 The corresponding name.
 * \param [in] v19 A enum value
 * \param [in] n19 The corresponding name.
 * \param [in] v20 An enum value
 * \param [in] n20 The enum name.
 * \param [in] v21 An enum value
 * \param [in] n21 The corresponding name.
 * \param [in] v22 A enum value
 * \param [in] n22 The corresponding name.
 */ 
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
