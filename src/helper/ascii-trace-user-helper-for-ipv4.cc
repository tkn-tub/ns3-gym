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
#include "ascii-trace-user-helper-for-ipv4.h"

namespace ns3 {

//
// Public API
//
void 
AsciiTraceUserHelperForIpv4::EnableAscii (std::string prefix, Ptr<Ipv4> ipv4, uint32_t interface)
{
  EnableAsciiInternal (Ptr<OutputStreamObject> (), prefix, ipv4, interface);
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv4::EnableAscii (Ptr<OutputStreamObject> stream, Ptr<Ipv4> ipv4, uint32_t interface)
{
  EnableAsciiInternal (stream, std::string (), ipv4, interface);
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv4::EnableAscii (std::string prefix, std::string ipv4Name, uint32_t interface)
{
  EnableAsciiImpl (Ptr<OutputStreamObject> (), prefix, ipv4Name, interface);
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv4::EnableAscii (Ptr<OutputStreamObject> stream, std::string ipv4Name, uint32_t interface)
{
  EnableAsciiImpl (stream, std::string (), ipv4Name, interface);
}

//
// Private API
//
void 
AsciiTraceUserHelperForIpv4::EnableAsciiImpl (
  Ptr<OutputStreamObject> stream, 
  std::string prefix, 
  std::string ipv4Name, 
  uint32_t interface)
{
  Ptr<Ipv4> ipv4 = Names::Find<Ipv4> (ipv4Name);
  EnableAsciiInternal (stream, prefix, ipv4, interface);
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv4::EnableAscii (std::string prefix, Ipv4InterfaceContainer c)
{
  EnableAsciiImpl (Ptr<OutputStreamObject> (), prefix, c);
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv4::EnableAscii (Ptr<OutputStreamObject> stream, Ipv4InterfaceContainer c)
{
  EnableAsciiImpl (stream, std::string (), c);
}

//
// Private API
//
void 
AsciiTraceUserHelperForIpv4::EnableAsciiImpl (Ptr<OutputStreamObject> stream, std::string prefix, Ipv4InterfaceContainer c)
{
  for (Ipv4InterfaceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      std::pair<Ptr<Ipv4>, uint32_t> pair = *i;
      EnableAsciiInternal (stream, prefix, pair.first, pair.second);
    }
}

//
// Public API
//
void
AsciiTraceUserHelperForIpv4::EnableAscii (std::string prefix, NodeContainer n)
{
  EnableAsciiImpl (Ptr<OutputStreamObject> (), prefix, n);
}

//
// Public API
//
void
AsciiTraceUserHelperForIpv4::EnableAscii (Ptr<OutputStreamObject> stream, NodeContainer n)
{
  EnableAsciiImpl (stream, std::string (), n);
}

//
// Private API
//
void
AsciiTraceUserHelperForIpv4::EnableAsciiImpl (Ptr<OutputStreamObject> stream, std::string prefix, NodeContainer n)
{
  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
      if (ipv4)
        {
          for (uint32_t j = 0; j < ipv4->GetNInterfaces (); ++j)
            {
              EnableAsciiInternal (stream, prefix, ipv4, j);
            }
        }
    }
}

//
// Public API
//
void
AsciiTraceUserHelperForIpv4::EnableAsciiAll (std::string prefix)
{
  EnableAsciiImpl (Ptr<OutputStreamObject> (), prefix, NodeContainer::GetGlobal ());
}

//
// Public API
//
void
AsciiTraceUserHelperForIpv4::EnableAsciiAll (Ptr<OutputStreamObject> stream)
{
  EnableAsciiImpl (stream, std::string (), NodeContainer::GetGlobal ());
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv4::EnableAscii (Ptr<OutputStreamObject> stream, uint32_t nodeid, uint32_t interface)
{
  EnableAsciiImpl (stream, std::string (), nodeid, interface);
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv4::EnableAscii (std::string prefix, uint32_t nodeid, uint32_t interface)
{
  EnableAsciiImpl (Ptr<OutputStreamObject> (), prefix, nodeid, interface);
}

//
// Private API
//
void 
AsciiTraceUserHelperForIpv4::EnableAsciiImpl (
  Ptr<OutputStreamObject> stream, 
  std::string prefix, 
  uint32_t nodeid, 
  uint32_t interface)
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
          EnableAsciiInternal (stream, prefix, ipv4, interface);
        }

      return;
    }
}

} // namespace ns3
