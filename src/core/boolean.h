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
#ifndef BOOLEAN_H
#define BOOLEAN_H

#include "attribute.h"
#include "attribute-helper.h"

namespace ns3 {

/**
 * \brief Hold a bool native type
 *
 * \anchor bool
 *
 * This class can be used to hold bool variables
 * which must go through the Attribute system.
 */
class BooleanValue : public AttributeValue
{
public:
  BooleanValue ();
  BooleanValue (bool value);
  void Set (bool value);
  bool Get (void) const;
  
  operator bool () const;

  virtual Ptr<AttributeValue> Copy (void) const;
  virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker);
private:
  bool m_value;
};

std::ostream & operator << (std::ostream &os, const BooleanValue &value);

ATTRIBUTE_CHECKER_DEFINE (Boolean);
ATTRIBUTE_ACCESSOR_DEFINE (Boolean);

} // namespace ns3

#endif /* BOOLEAN_H */
