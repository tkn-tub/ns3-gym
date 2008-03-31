/* -*-	Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef PCAP_TRACE_H
#define PCAP_TRACE_H

#include <string>
#include <vector>
#include "ns3/ptr.h"

namespace ns3 {

class Packet;
class TraceContext;
class PcapWriter;

class PcapTrace 
{
public:
  PcapTrace (std::string filename);
  ~PcapTrace ();

  void TraceAllIp (void);
private:
  PcapWriter *GetStream (uint32_t nodeId, uint32_t interfaceId);
  void LogRxIp (std::string context, Ptr<const Packet> p, uint32_t interfaceIndex);
  void LogTxIp (std::string context, Ptr<const Packet> p, uint32_t interfaceIndex);
  uint32_t GetNodeIndex (std::string context) const;
  std::string m_filename;
  struct Trace {
    uint32_t nodeId;
    uint32_t interfaceId;
    PcapWriter *writer;
  };
  std::vector<Trace> m_traces;
};

}//namespace ns3

#endif /* PCAP_TRACE_H */
