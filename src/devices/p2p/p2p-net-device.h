/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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
 * Author: Craig Dowell <craigdo@ee.washington.edu>
 */

#ifndef POINT_TO_POINT_NET_DEVICE_H
#define POINT_TO_POINT_NET_DEVICE_H

#include <string.h>
#include "ns3/mac-address.h"
#include "ns3/node.h"
#include "ns3/net-device.h"
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/callback-trace-source.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"

namespace ns3 {

class PointToPointChannel;
class Queue;

class PointToPointNetDevice : public NetDevice {
public:
  enum TraceType {
    QUEUE,
    RX,
  };

  PointToPointNetDevice(Node* node);
  virtual ~PointToPointNetDevice();

  void SetDataRate(DataRate bps);
  void SetInterframeGap(Time t);
  
private:
  // Don't let the compiler slip in copy and assignment construction
  PointToPointNetDevice(const PointToPointNetDevice&);
  PointToPointNetDevice&operator=(const PointToPointNetDevice&);

public:
  bool Attach(PointToPointChannel* ch);
  void AddQueue(Queue*);
  void Receive (Packet& p);

protected:
  Queue* GetQueue(void) const; 
  PointToPointChannel* GetChannel(void) const;

private:
  virtual bool SendTo (Packet& p, const MacAddress& dest);

  bool TransmitStart (Packet &p);
  void TransmitCompleteEvent (void);
  void TransmitReadyEvent (void);

  virtual TraceResolver *DoCreateTraceResolver (TraceContext const &context);

  enum TxMachineState
    {
      READY,
      BUSY,
      GAP
    };

  TxMachineState m_txMachineState;
  DataRate       m_bps;
  Time           m_tInterframeGap;

  PointToPointChannel* m_channel;
  Queue* m_queue;
  CallbackTraceSource<Packet &> m_rxTrace;
};

}; // namespace ns3

#endif // POINT_TO_POINT_NET_DEVICE_H

