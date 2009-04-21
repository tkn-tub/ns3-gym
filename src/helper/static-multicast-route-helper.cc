/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Washington
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
 * Author: Tom Henderson <tomhend@u.washington.edu>
 */

#include <vector>
#include "ns3/ptr.h"
#include "ns3/assert.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4.h"
#include "ns3/names.h"
#include "static-multicast-route-helper.h"

namespace ns3 {

StaticMulticastRouteHelper::StaticMulticastRouteHelper ()
{
}

void  
StaticMulticastRouteHelper::AddMulticastRoute (
  Ptr<Node> n,
  Ipv4Address source, 
  Ipv4Address group,  
  Ptr<NetDevice> input, 
  NetDeviceContainer output)
{
  Ptr<Ipv4> ipv4 = n->GetObject<Ipv4> ();

  // We need to convert the NetDeviceContainer to an array of interface
  std::vector<uint32_t> outputInterfaces;
  for (NetDeviceContainer::Iterator i = output.Begin (); i != output.End (); ++i)
    {
      Ptr<NetDevice> nd = *i;
      uint32_t ointerface = ipv4->FindInterfaceForDevice (nd);
      outputInterfaces.push_back(ointerface);
    }
  uint32_t iinterface = ipv4->FindInterfaceForDevice (input);
  ipv4->AddMulticastRoute (source, group, iinterface, outputInterfaces);
}

void  
StaticMulticastRouteHelper::AddMulticastRoute (
  Ptr<Node> n,
  Ipv4Address source, 
  Ipv4Address group,  
  std::string inputName, 
  NetDeviceContainer output)
{
  Ptr<NetDevice> input = Names::Find<NetDevice> (inputName);
  AddMulticastRoute (n, source, group, input, output);
}

void  
StaticMulticastRouteHelper::AddMulticastRoute (
  std::string nName,
  Ipv4Address source, 
  Ipv4Address group,  
  Ptr<NetDevice> input, 
  NetDeviceContainer output)
{
  Ptr<Node> n = Names::Find<Node> (nName);
  AddMulticastRoute (n, source, group, input, output);
}

void  
StaticMulticastRouteHelper::AddMulticastRoute (
  std::string nName,
  Ipv4Address source, 
  Ipv4Address group,  
  std::string inputName, 
  NetDeviceContainer output)
{
  Ptr<NetDevice> input = Names::Find<NetDevice> (inputName);
  Ptr<Node> n = Names::Find<Node> (nName);
  AddMulticastRoute (n, source, group, input, output);
}

void
StaticMulticastRouteHelper::SetDefaultMulticastRoute (
  Ptr<Node> n, 
  Ptr<NetDevice> nd)
{
  Ptr<Ipv4> ipv4 = n->GetObject<Ipv4> ();
  uint32_t interfaceSrc = ipv4->FindInterfaceForDevice (nd);
  ipv4->SetDefaultMulticastRoute (interfaceSrc);
}

void
StaticMulticastRouteHelper::SetDefaultMulticastRoute (
  Ptr<Node> n, 
  std::string ndName)
{
  Ptr<NetDevice> nd = Names::Find<NetDevice> (ndName);
  SetDefaultMulticastRoute (n, nd);
}

void
StaticMulticastRouteHelper::SetDefaultMulticastRoute (
  std::string nName, 
  Ptr<NetDevice> nd)
{
  Ptr<Node> n = Names::Find<Node> (nName);
  SetDefaultMulticastRoute (n, nd);
}

void
StaticMulticastRouteHelper::SetDefaultMulticastRoute (
  std::string nName, 
  std::string ndName)
{
  Ptr<Node> n = Names::Find<Node> (nName);
  Ptr<NetDevice> nd = Names::Find<NetDevice> (ndName);
  SetDefaultMulticastRoute (n, nd);
}

void
StaticMulticastRouteHelper::JoinMulticastGroup (
  Ptr<Node> n, 
  Ipv4Address source, 
  Ipv4Address group)
{
  Ptr<Ipv4> ipv4 = n->GetObject<Ipv4> ();
  ipv4->JoinMulticastGroup (source, group);
}

void
StaticMulticastRouteHelper::JoinMulticastGroup (
  std::string nName, 
  Ipv4Address source, 
  Ipv4Address group)
{
  Ptr<Node> n = Names::Find<Node> (nName);
  JoinMulticastGroup (n, source, group);
}

} // namespace ns3

