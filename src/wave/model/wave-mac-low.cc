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
#include "ns3/event-id.h"
#include "ns3/wifi-phy.h"
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
    .SetGroupName ("Wave")
    .AddConstructor<WaveMacLow> ()
  ;
  return tid;
}
WaveMacLow::WaveMacLow ()
{
  NS_LOG_FUNCTION (this);
}
WaveMacLow::~WaveMacLow ()
{
  NS_LOG_FUNCTION (this);
}

void
WaveMacLow::SetWaveNetDevice (Ptr<WaveNetDevice> device)
{
  m_scheduler  =  device->GetChannelScheduler ();
  m_coordinator =  device->GetChannelCoordinator ();
  NS_ASSERT (m_scheduler != 0 && m_coordinator != 0);
}

WifiTxVector
WaveMacLow::GetDataTxVector (Ptr<const Packet> packet, const WifiMacHeader *hdr) const
{
  NS_LOG_FUNCTION (this << packet << hdr);
  HigherLayerTxVectorTag datatag;
  bool found;
  found = ConstCast<Packet> (packet)->PeekPacketTag (datatag);
  // if high layer has not controlled transmit parameters, the real transmit parameters
  // will be determined by MAC layer itself.
  if (!found)
    {
      return MacLow::GetDataTxVector (packet, hdr);
    }

  // if high layer has set the transmit parameters with non-adaption mode,
  // the real transmit parameters are determined by high layer.
  if (!datatag.IsAdaptable ())
    {
      return datatag.GetTxVector ();
    }

  // if high layer has set the transmit parameters with non-adaption mode,
  // the real transmit parameters are determined by both high layer and MAC layer.
  WifiTxVector txHigher = datatag.GetTxVector ();
  WifiTxVector txMac = MacLow::GetDataTxVector (packet, hdr);
  WifiTxVector txAdapter;
  txAdapter.SetChannelWidth (10);
  // the DataRate set by higher layer is the minimum data rate
  // which is the lower bound for the actual data rate.
  if (txHigher.GetMode ().GetDataRate (txHigher.GetChannelWidth ()) > txMac.GetMode ().GetDataRate (txMac.GetChannelWidth ()))
    {
      txAdapter.SetMode (txHigher.GetMode ());
      txAdapter.SetPreambleType (txHigher.GetPreambleType ());
    }
  else
    {
      txAdapter.SetMode (txMac.GetMode ());
      txAdapter.SetPreambleType (txMac.GetPreambleType ());
    }
  // the TxPwr_Level set by higher layer is the maximum transmit
  // power which is the upper bound for the actual transmit power;
  txAdapter.SetTxPowerLevel (std::min (txHigher.GetTxPowerLevel (), txMac.GetTxPowerLevel ()));

  return txAdapter;
}

void
WaveMacLow::StartTransmission (Ptr<const Packet> packet,
                               const WifiMacHeader* hdr,
                               MacLowTransmissionParameters params,
                               Ptr<Txop> dca)
{
  NS_LOG_FUNCTION (this << packet << hdr << params << dca);
  Ptr<WifiPhy> phy = MacLow::GetPhy ();
  uint32_t curChannel = phy->GetChannelNumber ();
  // if current channel access is not AlternatingAccess, just do as MacLow.
  if (!m_scheduler->IsAlternatingAccessAssigned (curChannel))
    {
      MacLow::StartTransmission (packet, hdr, params, dca);
      return;
    }

  Time transmissionTime = MacLow::CalculateTransmissionTime (packet, hdr, params);
  Time remainingTime = m_coordinator->NeedTimeToGuardInterval ();

  if (transmissionTime > remainingTime)
    {
      // The attempt for this transmission will be canceled;
      // and this packet will be pending for next transmission by QosTxop class
      NS_LOG_DEBUG ("Because the required transmission time = " << transmissionTime.GetMilliSeconds ()
                                                                << "ms exceeds the remainingTime = " << remainingTime.GetMilliSeconds ()
                                                                << "ms, currently this packet will not be transmitted.");
    }
  else
    {
      MacLow::StartTransmission (packet, hdr, params, dca);
    }
}

} // namespace ns3
