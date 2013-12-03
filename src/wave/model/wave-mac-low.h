/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
 * Copyright (c) 2013 Dalian University of Technology
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
 *         Junling Bu <linlinjavaer@gmail.com>
 */
#ifndef WAVE_MAC_LOW_H
#define WAVE_MAC_LOW_H

#include "ns3/mac-low.h"

namespace ns3 {
/**
 * \ingroup wave
 * This class allows higher layer control data rate and tx power level.
 * If higher layer do not select, it will select by WifiRemoteStationManager
 * of MAC layer;
 * If higher layer selects tx arguments without adapter set, the data rate
 * and tx power level will be used to send the packet.
 * If higher layer selects tx arguments with adapter set, the data rate
 * will be lower bound for the actual data rate, and the power level
 * will be upper bound for the actual transmit power.
 */
class WaveMacLow : public MacLow
{
public:
  static TypeId GetTypeId (void);
  WaveMacLow (void);
  virtual ~WaveMacLow (void);
private:
  virtual WifiTxVector GetDataTxVector (Ptr<const Packet> packet, const WifiMacHeader *hdr) const;
};

} // namespace ns3

#endif /* WAVE_MAC_LOW_H*/
