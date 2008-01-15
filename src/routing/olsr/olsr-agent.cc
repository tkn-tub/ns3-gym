/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INESC Porto
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
 * Author: Gustavo J. A. M. Carneiro  <gjc@inescporto.pt>
 */

#include "olsr-agent.h"
#include "ns3/interface-id-default-value.h"

namespace ns3 {
namespace olsr {

static TypeIdDefaultValue g_defaultImpl =
  TypeIdDefaultValue ("OlsrAgentType",
                           "The type of OlsrAgent implementation",
                           Agent::GetTypeId (),
                           "OlsrAgentImpl");

NS_OBJECT_ENSURE_REGISTERED (Agent);

TypeId 
Agent::GetTypeId (void)
{
  static TypeId iid = TypeId ("OlsrAgent")
    .SetParent<Object> ();
  return iid;
}

Ptr<Agent> 
Agent::CreateDefault (Ptr<Node> node)
{
  TypeId iid = g_defaultImpl.GetValue ();
  Ptr<Agent> agent = iid.CreateObject (node)->QueryInterface<Agent> ();
  return agent;
}


}}
