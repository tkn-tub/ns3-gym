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
		 std::string n0 = "", Attribute v0 = Attribute (),
		 std::string n1 = "", Attribute v2 = Attribute (),
		 std::string n2 = "", Attribute v2 = Attribute (),
		 std::string n3 = "", Attribute v3 = Attribute (),
		 std::string n4 = "", Attribute v4 = Attribute (),
		 std::string n5 = "", Attribute v5 = Attribute (),
		 std::string n6 = "", Attribute v6 = Attribute (),
		 std::string n7 = "", Attribute v7 = Attribute ());

  void Enable (NodeContainer container);
  void Enable (Ptr<Node> node);
  void EnableAll (void);
private:
  ObjectFactory m_agentFactory;
};

} // namespace ns3

#endif /* OLSR_HELPER_H */
