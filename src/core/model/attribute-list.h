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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef ATTRIBUTE_LIST_H
#define ATTRIBUTE_LIST_H

#include <string>
#include <vector>
#include "attribute.h"
#include "type-id.h"

namespace ns3 {

/**
 * \ingroup attribute
 *
 * \brief a container of attributes to be used during object's construction
 *        and in ns3::Object::Set.
 *
 */
class AttributeList
{
public:
  AttributeList ();
  AttributeList (const AttributeList &o);
  AttributeList &operator = (const AttributeList &o);
  ~AttributeList ();
  /**
   * \param name the full name of the attribute to set
   * \param value the value to set
   *
   * This method checks that a attribute with the requested
   * name exists and that the value specified is an acceptable
   * value of that attribute. If any of these checks fails,
   * the program terminates with a message.
   */
  void Set (std::string name, const AttributeValue &value);
  /**
   * \param name the full name of the attribute to set
   * \param value the value to set
   * \returns true if the requested attribute exists and could be
   *          stored, false otherwise.
   */
  bool SetFailSafe (std::string name, const AttributeValue &value);
  /**
   * \param tid the TypeId associated to this attribute
   * \param name the name (not full!) of the attribute
   * \param value the value to set
   *
   * This method checks that a attribute with the requested
   * name exists and that the value specified is an acceptable
   * value of that attribute. If any of these checks fails,
   * the program terminates with a message.
   */
  void SetWithTid (TypeId tid, std::string name, const AttributeValue &value);

  /**
   * Clear the content of this instance.
   */
  void Reset (void);

  /**
   * \returns the global attribute container
   *
   * The global attribute container can be used to specify
   * a set of attribute values without having to re-specify
   * them for each object when it is created. This container
   * is checked only during object construction and 
   * it is always checked last, after any per-object
   * container is checked.
   */
  static AttributeList *GetGlobal (void);

  std::string SerializeToString (void) const;
  bool DeserializeFromString (std::string value);
private:
  friend class ObjectBase;
  struct Attr {
    Ptr<const AttributeChecker> checker;
    Ptr<const AttributeValue> value;
  };
  typedef std::vector<struct Attr> Attrs;
  typedef Attrs::iterator Iterator;
  typedef Attrs::const_iterator CIterator;



  bool DoSet (struct TypeId::AttributeInfo *info, const AttributeValue &param);
  void DoSetOne (Ptr<const AttributeChecker> checker, const AttributeValue &param);
  std::string LookupAttributeFullNameByChecker (Ptr<const AttributeChecker> checker) const;

  Attrs m_attributes;
};

class UnsafeAttributeList
{
public:
  UnsafeAttributeList ();
  UnsafeAttributeList (const UnsafeAttributeList &o);
  UnsafeAttributeList &operator = (const UnsafeAttributeList &o);
  ~UnsafeAttributeList ();
  
  void Set (std::string name, const AttributeValue &param);

  AttributeList GetSafe (std::string name) const;
private:
  std::vector<std::pair<std::string,Ptr<AttributeValue> > > m_attributes;
};

} // namespace ns3

#endif /* ATTRIBUTE_LIST_H */
