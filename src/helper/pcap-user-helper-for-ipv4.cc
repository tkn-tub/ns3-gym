/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 University of Washington
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
 */

#include "ns3/abort.h"
#include "ns3/names.h"
#include "pcap-user-helper-for-ipv4.h"

namespace ns3 {

void 
PcapUserHelperForIpv4::EnablePcap (std::string prefix, Ptr<Ipv4> ipv4, uint32_t interface)
{
  EnablePcapInternal (prefix, ipv4, interface);
}

void 
PcapUserHelperForIpv4::EnablePcap (std::string prefix, std::string ipv4Name, uint32_t interface)
{
  Ptr<Ipv4> ipv4 = Names::Find<Ipv4> (ipv4Name);
  EnablePcap (prefix, ipv4, interface);
}

void 
PcapUserHelperForIpv4::EnablePcap (std::string prefix, Ipv4InterfaceContainer c)
{
  for (Ipv4InterfaceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      std::pair<Ptr<Ipv4>, uint32_t> pair = *i;
      EnablePcap (prefix, pair.first, pair.second);
    }
}

void
PcapUserHelperForIpv4::EnablePcap (std::string prefix, NodeContainer n)
{
  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
      if (ipv4)
        {
          for (uint32_t j = 0; j < ipv4->GetNInterfaces (); ++j)
            {
              EnablePcap (prefix, ipv4, j);
            }
        }
    }
}

void
PcapUserHelperForIpv4::EnablePcapAll (std::string prefix)
{
  EnablePcap (prefix, NodeContainer::GetGlobal ());
}

void 
PcapUserHelperForIpv4::EnablePcap (std::string prefix, uint32_t nodeid, uint32_t interface)
{
  NodeContainer n = NodeContainer::GetGlobal ();

  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      if (node->GetId () != nodeid) 
        {
          continue;
        }
      
      Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
      if (ipv4)
        {
          EnablePcap (prefix, ipv4, interface);
        }
      return;
    }
}

} // namespace ns3
