/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 */
#include "ns3/mesh-interface-helper.h"
#include "dot11s-installer.h"
#include "peer-management-protocol.h"
#include "hwmp-protocol.h"

namespace ns3 {
using namespace dot11s;
NS_OBJECT_ENSURE_REGISTERED (Dot11sStack);
TypeId
Dot11sStack::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Dot11sStack")
    .SetParent<Object> ()
    .AddConstructor<Dot11sStack> ()
    ;
  return tid;
}
Dot11sStack::Dot11sStack ()
{}
Dot11sStack::~Dot11sStack ()
{}
void
Dot11sStack::DoDispose ()
{
}
void
Dot11sStack::SetRoot (Ptr<MeshPointDevice> mp)
{
   Ptr <HwmpProtocol> hwmp = mp->GetObject<HwmpProtocol> ();
   NS_ASSERT(hwmp != 0);
   hwmp->SetRoot ();
}
bool
Dot11sStack::InstallStack (Ptr<MeshPointDevice> mp)
{
  //Install Peer management protocol:
  Ptr<PeerManagementProtocol> pmp = CreateObject<PeerManagementProtocol> ();
  pmp->SetMeshId("mesh");
  bool install_ok = pmp->Install (mp);
  if(!install_ok)
    return false;
  //Install HWMP:
  Ptr<HwmpProtocol> hwmp = CreateObject<HwmpProtocol> ();
  install_ok = hwmp->Install (mp);
  if(!install_ok)
    return false;
  //Install interaction between HWMP and Peer management protocol:
  pmp->SetPeerLinkStatusCallback(MakeCallback(&HwmpProtocol::PeerLinkStatus, hwmp));
  hwmp->SetNeighboursCallback(MakeCallback(&PeerManagementProtocol::GetActiveLinks, pmp));
  return true;
}
void
Dot11sStack::Report (const Ptr<MeshPointDevice> mp, std::ostream& os)
{
  mp->Report (os);
  
  std::vector<Ptr<NetDevice> > ifaces = mp->GetInterfaces ();
  for (std::vector<Ptr<NetDevice> >::const_iterator i = ifaces.begin(); i != ifaces.end(); ++i)
  {
    Ptr<WifiNetDevice> device = (*i)->GetObject<WifiNetDevice> ();
    NS_ASSERT (device != 0);
    MeshInterfaceHelper::Report(device, os);
  }
  Ptr <HwmpProtocol> hwmp = mp->GetObject<HwmpProtocol> ();
  NS_ASSERT(hwmp != 0);
  hwmp->Report (os);

  Ptr <PeerManagementProtocol> pmp = mp->GetObject<PeerManagementProtocol> ();
  NS_ASSERT(pmp != 0);
  pmp->Report (os);
}
void
Dot11sStack::ResetStats (const Ptr<MeshPointDevice> mp)
{
  mp->ResetStats ();
  
  std::vector<Ptr<NetDevice> > ifaces = mp->GetInterfaces ();
  for (std::vector<Ptr<NetDevice> >::const_iterator i = ifaces.begin(); i != ifaces.end(); ++i)
  {
    Ptr<WifiNetDevice> device = (*i)->GetObject<WifiNetDevice> ();
    NS_ASSERT (device != 0);
    MeshInterfaceHelper::ResetStats (device);
  }
  Ptr <HwmpProtocol> hwmp = mp->GetObject<HwmpProtocol> ();
  NS_ASSERT(hwmp != 0);
  hwmp->ResetStats ();

  Ptr <PeerManagementProtocol> pmp = mp->GetObject<PeerManagementProtocol> ();
  NS_ASSERT(pmp != 0);
  pmp->ResetStats ();
}
} //namespace ns3
