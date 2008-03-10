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
 * This class can be used to hold bool variables
 * which must go through the Attribute system.
 */
class Boolean
{
public:
  Boolean ();
  Boolean (bool value);
  void Set (bool value);
  bool Get (void) const;

  operator bool () const;

  ATTRIBUTE_CONVERTER_DEFINE (Boolean);
private:
  bool m_value;
};

std::ostream & operator << (std::ostream &os, const Boolean &value);
std::istream & operator >> (std::istream &is, Boolean &value);

ATTRIBUTE_VALUE_DEFINE (Boolean);
ATTRIBUTE_CHECKER_DEFINE (Boolean);
ATTRIBUTE_ACCESSOR_DEFINE (Boolean);

} // namespace ns3

#endif /* BOOLEAN_H */
