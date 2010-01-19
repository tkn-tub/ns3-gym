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
#include "ascii-trace-user-helper.h"

namespace ns3 {

void 
AsciiTraceUserHelper::EnableAscii (std::string prefix, Ptr<NetDevice> nd)
{
  EnableAsciiInternal (prefix, nd);
}

void 
AsciiTraceUserHelper::EnableAscii (std::string prefix, std::string ndName)
{
  Ptr<NetDevice> nd = Names::Find<NetDevice> (ndName);
  EnableAscii (prefix, nd);
}

void 
AsciiTraceUserHelper::EnableAscii (std::string prefix, NetDeviceContainer d)
{
  for (NetDeviceContainer::Iterator i = d.Begin (); i != d.End (); ++i)
    {
      Ptr<NetDevice> dev = *i;
      EnableAscii (prefix, dev);
    }
}

void
AsciiTraceUserHelper::EnableAscii (std::string prefix, NodeContainer n)
{
  NetDeviceContainer devs;
  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      for (uint32_t j = 0; j < node->GetNDevices (); ++j)
        {
          devs.Add (node->GetDevice (j));
        }
    }
  EnableAscii (prefix, devs);
}

void
AsciiTraceUserHelper::EnableAsciiAllXXX (std::string prefix)
{
  EnableAscii (prefix, NodeContainer::GetGlobal ());
}

void 
AsciiTraceUserHelper::EnableAscii (std::string prefix, uint32_t nodeid, uint32_t deviceid)
{
  NodeContainer n = NodeContainer::GetGlobal ();

  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      if (node->GetId () != nodeid) 
        {
          continue;
        }
      
      NS_ABORT_MSG_IF (deviceid >= node->GetNDevices (), 
                       "AsciiTraceUserHelper::EnableAscii(): Unknown deviceid = " << deviceid);

      Ptr<NetDevice> nd = node->GetDevice (deviceid);

      EnableAscii (prefix, nd);
      return;
    }
}

} // namespace ns3
