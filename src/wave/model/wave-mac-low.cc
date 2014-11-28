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
#include "ns3/log.h"
#include "wave-mac-low.h"
#include "higher-tx-tag.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("WaveMacLow");

NS_OBJECT_ENSURE_REGISTERED (WaveMacLow);

TypeId
WaveMacLow::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WaveMacLow")
    .SetParent<MacLow> ()
    .AddConstructor<WaveMacLow> ()
  ;
  return tid;
}
WaveMacLow::WaveMacLow (void)
{
  NS_LOG_FUNCTION (this);
}
WaveMacLow::~WaveMacLow (void)
{
  NS_LOG_FUNCTION (this);
}

WifiTxVector
WaveMacLow::GetDataTxVector (Ptr<const Packet> packet, const WifiMacHeader *hdr) const
{
  NS_LOG_FUNCTION (this << packet << *hdr);
  HigherDataTxVectorTag datatag;
  bool found;
  found = ConstCast<Packet> (packet)->PeekPacketTag (datatag);
  if (!found)
    {
      return MacLow::GetDataTxVector (packet, hdr);
    }

  if (!datatag.IsAdapter ())
    {
      return datatag.GetDataTxVector ();
    }

  WifiTxVector txHigher = datatag.GetDataTxVector ();
  WifiTxVector txMac = MacLow::GetDataTxVector (packet, hdr);
  WifiTxVector txAdapter;
  // if adapter is true, DataRate set by higher layer is the minimum data rate
  // that sets the lower bound for the actual data rate.
  if (txHigher.GetMode ().GetDataRate () > txMac.GetMode ().GetDataRate ())
    {
      txAdapter.SetMode (txHigher.GetMode ());
    }
  else
    {
      txAdapter.SetMode (txMac.GetMode ());
    }

  // if adapter is true, TxPwr_Level set by higher layer is the maximum
  // transmit power that sets the upper bound for the actual transmit power;
  txAdapter.SetTxPowerLevel (std::min (txHigher.GetTxPowerLevel (), txMac.GetTxPowerLevel ()));
  return txAdapter;
}
} // namespace ns3
