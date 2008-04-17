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
#ifndef OLSR_HELPER_H
#define OLSR_HELPER_H

#include "ns3/object-factory.h"
#include "ns3/node-container.h"
#include "ns3/node.h"

namespace ns3 {

class OlsrHelper
{
public:
  OlsrHelper ();

  void SetAgent (std::string tid,
		 std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
		 std::string n1 = "", const AttributeValue &v2 = EmptyAttributeValue (),
		 std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
		 std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
		 std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
		 std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
		 std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
		 std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());

  void Enable (NodeContainer container);
  void Enable (Ptr<Node> node);
  void EnableAll (void);
private:
  ObjectFactory m_agentFactory;
};

} // namespace ns3

#endif /* OLSR_HELPER_H */
