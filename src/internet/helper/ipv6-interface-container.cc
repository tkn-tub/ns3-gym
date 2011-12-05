/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008-2009 Strasbourg University
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
 * Author: Sebastien Vincent <vincent@clarinet.u-strasbg.fr>
 */

#include "ns3/node-list.h"
#include "ns3/names.h"

#include "ipv6-interface-container.h"
#include "ns3/ipv6-static-routing-helper.h"

namespace ns3 
{

Ipv6InterfaceContainer::Ipv6InterfaceContainer ()
{
}

Ipv6InterfaceContainer::Iterator 
Ipv6InterfaceContainer::Begin (void) const
{
  return m_interfaces.begin ();
}

Ipv6InterfaceContainer::Iterator 
Ipv6InterfaceContainer::End (void) const
{
  return m_interfaces.end ();
}

uint32_t Ipv6InterfaceContainer::GetN () const
{
  return m_interfaces.size ();
}

uint32_t Ipv6InterfaceContainer::GetInterfaceIndex (uint32_t i) const
{
  return m_interfaces[i].second;
}

Ipv6Address Ipv6InterfaceContainer::GetAddress (uint32_t i, uint32_t j) const
{
  Ptr<Ipv6> ipv6 = m_interfaces[i].first;
  uint32_t interface = m_interfaces[i].second;
  return ipv6->GetAddress (interface, j).GetAddress ();
}

void Ipv6InterfaceContainer::Add (Ptr<Ipv6> ipv6, uint32_t interface)
{
  m_interfaces.push_back (std::make_pair (ipv6, interface));
}

void Ipv6InterfaceContainer::Add (std::string ipv6Name, uint32_t interface)
{
  Ptr<Ipv6> ipv6 = Names::Find<Ipv6> (ipv6Name);
  m_interfaces.push_back (std::make_pair (ipv6, interface));
}

void Ipv6InterfaceContainer::Add (Ipv6InterfaceContainer& c)
{
  for (InterfaceVector::const_iterator it = c.m_interfaces.begin (); it != c.m_interfaces.end (); it++)
    {
      m_interfaces.push_back (*it);
    }
}

void Ipv6InterfaceContainer::SetRouter (uint32_t i, bool router)
{
  Ptr<Ipv6> ipv6 = m_interfaces[i].first;
  ipv6->SetForwarding (m_interfaces[i].second, router);

  if (router)
    {
      uint32_t other;
      /* assume first global address is index 1 (0 is link-local) */
      Ipv6Address routerAddress = ipv6->GetAddress (m_interfaces[i].second, 1).GetAddress ();

      for (other = 0; other < m_interfaces.size (); other++)
        {
          if (other != i)
            {
              Ptr<Ipv6StaticRouting> routing = 0;
              Ipv6StaticRoutingHelper routingHelper;

              ipv6 = m_interfaces[other].first;
              routing = routingHelper.GetStaticRouting (ipv6);
              routing->SetDefaultRoute (routerAddress, m_interfaces[other].second);
            }
        }
    }
}

void Ipv6InterfaceContainer::SetDefaultRoute (uint32_t i, uint32_t router)
{
  Ptr<Ipv6> ipv6 = m_interfaces[i].first;
  Ptr<Ipv6> ipv6Router = m_interfaces[router].first;
  Ipv6Address routerAddress = ipv6Router->GetAddress (m_interfaces[router].second, 1).GetAddress ();
  Ptr<Ipv6StaticRouting> routing = 0;
  Ipv6StaticRoutingHelper routingHelper;

  routing = routingHelper.GetStaticRouting (ipv6);
  routing->SetDefaultRoute (routerAddress, m_interfaces[i].second);
}

} /* namespace ns3 */

