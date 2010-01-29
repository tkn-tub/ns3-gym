/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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

#include <iostream>
#include <fstream>

#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/core-module.h"
#include "ns3/helper-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("PerfPcap");

bool g_addheader = false;
RadiotapHeader g_header;
Time g_t(0.);

static void
WritePacket (Ptr<PcapFileObject> file, Ptr<const Packet>   packet)
{
  if (g_addheader)
    {
      Ptr<Packet> p = packet->Copy ();
      p->AddHeader (g_header);
      file->Write (g_t, p);
    }
  else
    {
      file->Write (g_t, packet);
    }
}

int 
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.AddValue ("addheader", "Add a header to the traces to trigger a deep copy", g_addheader);
  cmd.Parse (argc, argv);

  PcapHelper pcapHelper;
  Ptr<PcapFileObject> file = pcapHelper.CreateFile ("perf-pcap.pcap", "w", 9999);

  Ptr<Packet> p = Create<Packet> (1024);

  NS_LOG_UNCOND ("timing pcap file write of 1K packet");
  NS_LOG_UNCOND ("g_addheader = " << g_addheader);

  SystemWallClockMs ms;
  ms.Start ();

  for (uint32_t i = 0; i < 100000; ++i)
    {
      WritePacket (file, p);
    }

  uint64_t et = ms.End ();

  NS_LOG_UNCOND ("elapsed time = " << et);
}
