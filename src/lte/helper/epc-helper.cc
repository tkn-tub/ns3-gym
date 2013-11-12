/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2013 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Jaume Nin <jnin@cttc.es>
 *         Nicola Baldo <nbaldo@cttc.es>
 *         Manuel Requena <manuel.requena@cttc.es>
 */

#include <ns3/epc-helper.h>
#include <ns3/log.h>
#include <ns3/node.h>
#include <ns3/ipv4-address.h>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("EpcHelper");

NS_OBJECT_ENSURE_REGISTERED (EpcHelper);


EpcHelper::EpcHelper () 
{
  NS_LOG_FUNCTION (this);
}

EpcHelper::~EpcHelper ()
{
  NS_LOG_FUNCTION (this);
}

TypeId
EpcHelper::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EpcHelper")
    .SetParent<Object> ()
    ;
  return tid;
}

void
EpcHelper::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  Object::DoDispose ();
}

#define EPC_HELPER_API_CHANGE_MSG "The EpcHelper class is now an abstract class, and should not be used directly any more. Please use one of its derived classes (e.g., PointToPointEpcHelper) instead. For the vast majority of cases, it should be sufficient to search & replace \"<EpcHelper>\" for \"<PointToPointEpcHelper>\" in your code. "


void
EpcHelper::AddEnb (Ptr<Node> enb, Ptr<NetDevice> lteEnbNetDevice, uint16_t cellId)
{
  NS_FATAL_ERROR (EPC_HELPER_API_CHANGE_MSG);
}

void
EpcHelper::AddX2Interface (Ptr<Node> enb1, Ptr<Node> enb2)
{
  NS_FATAL_ERROR (EPC_HELPER_API_CHANGE_MSG);
}

void 
EpcHelper::AddUe (Ptr<NetDevice> ueDevice, uint64_t imsi)
{
  NS_FATAL_ERROR (EPC_HELPER_API_CHANGE_MSG);
}


void
EpcHelper::ActivateEpsBearer (Ptr<NetDevice> ueDevice, uint64_t imsi, Ptr<EpcTft> tft, EpsBearer bearer)
{
  NS_FATAL_ERROR (EPC_HELPER_API_CHANGE_MSG);
}


Ptr<Node>
EpcHelper::GetPgwNode ()
{
  NS_FATAL_ERROR (EPC_HELPER_API_CHANGE_MSG);
  return Ptr<Node> (0);
}

Ipv4InterfaceContainer 
EpcHelper::AssignUeIpv4Address (NetDeviceContainer ueDevices)
{
  NS_FATAL_ERROR (EPC_HELPER_API_CHANGE_MSG);
  return Ipv4InterfaceContainer ();
}


Ipv4Address
EpcHelper::GetUeDefaultGatewayAddress ()
{
  NS_FATAL_ERROR (EPC_HELPER_API_CHANGE_MSG);
  return Ipv4Address ();
}



} // namespace ns3
