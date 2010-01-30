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
#include "ns3/pcap-writer.h"

using namespace ns3;

//NS_LOG_COMPONENT_DEFINE ("PerfPcap");

bool g_passheader = true;
bool g_addheader = true;
Time g_t(0.);

static void
WritePacket (Ptr<PcapFileObject> file, Ptr<const Packet>   packet)
{
  if (g_passheader)
    {
      RadiotapHeader header;
      header.SetTsft (0);
      header.SetFrameFlags (RadiotapHeader::FRAME_FLAG_SHORT_PREAMBLE);
      header.SetRate (0);
      header.SetChannelFrequencyAndFlags (0, 0);

      file->Write (g_t, header, packet);
      return;
    }

  if (g_addheader)
    {
      RadiotapHeader header;
      header.SetTsft (0);
      header.SetFrameFlags (RadiotapHeader::FRAME_FLAG_SHORT_PREAMBLE);
      header.SetRate (0);
      header.SetChannelFrequencyAndFlags (0, 0);

      Ptr<Packet> p = packet->Copy ();
      p->AddHeader (header);
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
  uint32_t n = 100000;
  bool oldstyle = false;

  CommandLine cmd;
  cmd.AddValue ("addheader", "Add a header to the traces to trigger a deep copy", g_addheader);
  cmd.AddValue ("passheader", "Pass header as reference instead of adding it", g_passheader);
  cmd.AddValue ("n", "How many packets to write (defaults to 100000", n);
  cmd.AddValue ("oldstyle", "run the old style pcap writer stuff if true", oldstyle);
  cmd.Parse (argc, argv);

  uint64_t et;

  if (oldstyle)
    {
      PcapWriter pcapWriter;
      pcapWriter.Open ("perf-pcap.pcap");
      pcapWriter.WriteWifiRadiotapHeader ();

      Ptr<Packet> p = Create<Packet> (1024);

      //NS_LOG_UNCOND ("timing old style pcap file write of 1K packet");
      //NS_LOG_UNCOND ("g_addheader = " << g_addheader);
      //NS_LOG_UNCOND ("g_passheader = " << g_passheader);
      //NS_LOG_UNCOND ("n = " << n);

      SystemWallClockMs ms;
      ms.Start ();

      for (uint32_t i = 0; i < n; ++i)
        {
          if (g_addheader)
            {
              pcapWriter.WriteWifiMonitorPacket (p, 0, 0, 0, true, false, 0., 0.);
            }
          else
            {
              pcapWriter.WritePacket (p);
            }
        }

      et = ms.End ();
    }
  else
    {
      PcapHelper pcapHelper;
      Ptr<PcapFileObject> file = pcapHelper.CreateFile ("perf-pcap.pcap", "w", PcapHelper::DLT_IEEE802_11_RADIO);

      Ptr<Packet> p = Create<Packet> (1024);

      //NS_LOG_UNCOND ("timing new style pcap file write of 1K packet");
      //NS_LOG_UNCOND ("g_addheader = " << g_addheader);
      //NS_LOG_UNCOND ("g_passheader = " << g_passheader);
      //NS_LOG_UNCOND ("n = " << n);

      SystemWallClockMs ms;
      ms.Start ();

      for (uint32_t i = 0; i < n; ++i)
        {
          WritePacket (file, p);
        }

      et = ms.End ();
    }

  //NS_LOG_UNCOND ("elapsed time = " << et);
}
