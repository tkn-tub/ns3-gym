/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Mathieu Lacage
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
#ifndef ATTRIBUTE_CONSTRUCTION_LIST_H
#define ATTRIBUTE_CONSTRUCTION_LIST_H

#include "attribute.h"
#include <list>

namespace ns3 {

class AttributeConstructionList
{
public:
  struct Item 
  {
    Ptr<const AttributeChecker> checker;
    Ptr<AttributeValue> value;
    std::string name;
  };
  typedef std::list<struct Item>::const_iterator CIterator;

  AttributeConstructionList ();
  void Add (std::string name, Ptr<const AttributeChecker> checker, Ptr<AttributeValue> value);
  Ptr<AttributeValue> Find (Ptr<const AttributeChecker> checker) const;
  CIterator Begin (void) const;
  CIterator End (void) const;
private:
  std::list<struct Item> m_list;
};

} // namespace ns3

#endif /* ATTRIBUTE_CONSTRUCTION_LIST_H */
