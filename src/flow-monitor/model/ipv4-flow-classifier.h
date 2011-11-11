/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2009 INESC Porto
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: Gustavo J. A. M. Carneiro  <gjc@inescporto.pt> <gjcarneiro@gmail.com>
//

#ifndef IPV4_FLOW_CLASSIFIER_H
#define IPV4_FLOW_CLASSIFIER_H

#include <stdint.h>
#include <map>

#include "ns3/ipv4-header.h"
#include "ns3/flow-classifier.h"

namespace ns3 {

class Packet;

/// Classifies packets by looking at their IP and TCP/UDP headers.
/// From these packet headers, a tuple (source-ip, destination-ip,
/// protocol, source-port, destination-port) is created, and a unique
/// flow identifier is assigned for each different tuple combination
class Ipv4FlowClassifier : public FlowClassifier
{
public:

  struct FiveTuple
  {
    Ipv4Address sourceAddress;
    Ipv4Address destinationAddress;
    uint8_t protocol;
    uint16_t sourcePort;
    uint16_t destinationPort;
  };

  Ipv4FlowClassifier ();

  /// \brief try to classify the packet into flow-id and packet-id
  /// \return true if the packet was classified, false if not (i.e. it
  /// does not appear to be part of a flow).
  bool Classify (const Ipv4Header &ipHeader, Ptr<const Packet> ipPayload,
                 uint32_t *out_flowId, uint32_t *out_packetId);

  /// Searches for the FiveTuple corresponding to the given flowId
  FiveTuple FindFlow (FlowId flowId) const;

  virtual void SerializeToXmlStream (std::ostream &os, int indent) const;

private:

  std::map<FiveTuple, FlowId> m_flowMap;

};


bool operator < (const Ipv4FlowClassifier::FiveTuple &t1, const Ipv4FlowClassifier::FiveTuple &t2);
bool operator == (const Ipv4FlowClassifier::FiveTuple &t1, const Ipv4FlowClassifier::FiveTuple &t2);


} // namespace ns3

#endif /* IPV4_FLOW_CLASSIFIER_H */
