/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
#include "ns3/packet.h"
#include "ns3/llc-snap-header.h"
#include "ns3/node.h"

#include "wifi-net-device.h"
#include "wifi-phy.h"
#include "wifi-channel.h"
#include "mac-stations.h"
#include "mac-low.h"
#include "mac-parameters.h"
#include "mac-rx-middle.h"
#include "mac-tx-middle.h"
#include "mac-high-adhoc.h"
#include "mac-high-nqsta.h"
#include "mac-high-nqap.h"
#include "dca-txop.h"
#include "wifi-default-parameters.h"
#include "arf-mac-stations.h"
#include "aarf-mac-stations.h"
#include "ideal-mac-stations.h"
#include "cr-mac-stations.h"

namespace ns3 {

static WifiMode 
GetWifiModeForPhyMode (WifiPhy *phy, enum WifiDefaultParameters::PhyModeParameter mode)
{
  uint32_t phyRate = (uint32_t)mode;
  for (uint32_t i= 0; i < phy->GetNModes (); i++)
    {
      WifiMode mode = phy->GetMode (i);
      if (mode.GetPhyRate () == phyRate)
        {
          return mode;
        }
    }
  NS_ASSERT (false);
  return WifiMode ();
}


WifiNetDevice::WifiNetDevice (Ptr<Node> node)
  : NetDevice (node, Mac48Address::Allocate ())
{
  SetMtu (2300);
  EnableBroadcast (Mac48Address ("ff:ff:ff:ff:ff:ff"));
  Construct ();
}

WifiNetDevice::~WifiNetDevice ()
{}

void
WifiNetDevice::Construct (void)
{
  // the physical layer.
  m_phy = new WifiPhy (this);

  // the rate control algorithm
  switch (WifiDefaultParameters::GetRateControlAlgorithm ()) {
  case WifiDefaultParameters::ARF:
    m_stations = new ArfMacStations (m_phy->GetMode (0), 
                                     WifiDefaultParameters::GetArfRateControlTimerThreshold (),
                                     WifiDefaultParameters::GetArfRateControlSuccessThreshold ());
    break;
  case WifiDefaultParameters::AARF:
    m_stations = new AarfMacStations (m_phy->GetMode (0),
                                      WifiDefaultParameters::GetAarfRateControlMinTimerThreshold (),
                                      WifiDefaultParameters::GetAarfRateControlMinSuccessThreshold (),
                                      WifiDefaultParameters::GetAarfRateControlSuccessK (),
                                      WifiDefaultParameters::GetAarfRateControlMaxSuccessThreshold (),
                                      WifiDefaultParameters::GetAarfRateControlTimerK ());
    break;
  case WifiDefaultParameters::CONSTANT_RATE: {
    WifiMode dataRate = GetWifiModeForPhyMode (m_phy, WifiDefaultParameters::GetConstantDataRate ());
    WifiMode ctlRate = GetWifiModeForPhyMode (m_phy, WifiDefaultParameters::GetConstantCtlRate ());
    m_stations = new CrMacStations (dataRate, ctlRate);
  } break;
  case WifiDefaultParameters::IDEAL: {
    double ber = WifiDefaultParameters::GetIdealRateControlBer ();
    IdealMacStations *ideal = new IdealMacStations (m_phy->GetMode (0));
    uint32_t nModes = m_phy->GetNModes ();
    for (uint32_t i = 0; i < nModes; i++) 
      {
        WifiMode mode = m_phy->GetMode (i);
        ideal->AddModeSnrThreshold (mode, m_phy->CalculateSnr (mode, ber));
      }
    m_stations = ideal;
  } break;
  default:
    // NOTREACHED
    NS_ASSERT (false);
    break;
  }

  // MacParameters
  MacParameters *parameters = new MacParameters ();
  WifiMacHeader hdr;
  hdr.SetType (WIFI_MAC_CTL_CTS);
  Time ctsDelay = m_phy->CalculateTxDuration (hdr.GetSize () + 4, m_phy->GetMode (0), WIFI_PREAMBLE_LONG);
  hdr.SetType (WIFI_MAC_CTL_ACK);
  Time ackDelay = m_phy->CalculateTxDuration (hdr.GetSize () + 4, m_phy->GetMode (0), WIFI_PREAMBLE_LONG);
  parameters->Initialize (ctsDelay, ackDelay);
  m_parameters = parameters;

  // the MacLow
  MacLow *low = new MacLow ();
  low->SetDevice (this);
  low->SetPhy (m_phy);
  low->SetStations (m_stations);
  low->SetParameters (m_parameters);
  m_phy->SetReceiveOkCallback (MakeCallback (&MacLow::ReceiveOk, low));
  m_phy->SetReceiveErrorCallback (MakeCallback (&MacLow::ReceiveError, low));
  m_low = low;

  // the 'middle' rx
  MacRxMiddle *rxMiddle = new MacRxMiddle ();
  low->SetRxCallback (MakeCallback (&MacRxMiddle::Receive, rxMiddle));
  m_rxMiddle = rxMiddle;

  // the 'middle' tx
  MacTxMiddle *txMiddle = new MacTxMiddle ();
  m_txMiddle = txMiddle;

}

DcaTxop *
WifiNetDevice::CreateDca (uint32_t minCw, uint32_t maxCw) const
{
  DcaTxop *dca = new DcaTxop (minCw, maxCw);
  dca->SetParameters (m_parameters);
  dca->SetTxMiddle (m_txMiddle);
  dca->SetLow (m_low);
  dca->SetPhy (m_phy);

  Time difs = m_parameters->GetSifs () + 
    m_parameters->GetSlotTime () + 
    m_parameters->GetSlotTime ();
  // see 802.11 p85 section 9.2.10
  Time eifs = difs + m_parameters->GetSifs () + 
    m_phy->CalculateTxDuration (8 * (2+2+6+4), m_phy->GetMode (0), WIFI_PREAMBLE_LONG);
  dca->SetDifs (difs);
  dca->SetEifs (eifs);
  dca->SetMaxQueueSize (400);
  dca->SetMaxQueueDelay (Seconds (10));
  return dca;
}

void 
WifiNetDevice::ConnectTo (Ptr<WifiChannel> channel)
{
  m_channel = channel;
  m_phy->SetChannel (channel);
  NotifyConnected ();
}
bool
WifiNetDevice::SendTo (const Packet &packet, const Address &to, uint16_t protocolNumber)
{
  NS_ASSERT (Mac48Address::IsMatchingType (to));

  Mac48Address realTo = Mac48Address::ConvertFrom (to);

  Packet p = packet;

  LlcSnapHeader llc;
  llc.SetType (protocolNumber);
  p.AddHeader (llc);

  m_txLogger (p, realTo);

  return DoSendTo (p, realTo);
}
void 
WifiNetDevice::DoForwardUp (Packet packet, const Mac48Address &from)
{
  m_rxLogger (packet, from);

  LlcSnapHeader llc;
  packet.RemoveHeader (llc);
  NetDevice::ForwardUp (packet, llc.GetType (), from);
}
Mac48Address 
WifiNetDevice::GetSelfAddress (void) const
{
  NS_ASSERT (Mac48Address::IsMatchingType (GetAddress ()));
  Mac48Address self = Mac48Address::ConvertFrom (GetAddress ());
  return self;
}
bool 
WifiNetDevice::DoNeedsArp (void) const
{
  return true;
}
Ptr<Channel> 
WifiNetDevice::DoGetChannel (void) const
{
  return m_channel;
}
void 
WifiNetDevice::DoDispose (void)
{
  // chain up.
  NetDevice::DoDispose ();
  // cleanup local
  m_channel = 0;
  delete m_phy;
  delete m_stations;
  delete m_low;
  delete m_rxMiddle;
  delete m_txMiddle;
  delete m_parameters;
  m_phy = 0;
  m_stations = 0;
  m_low = 0;
  m_rxMiddle = 0;
  m_txMiddle = 0;
  m_parameters = 0;
}


/*****************************************************
 *            Adhoc code
 *****************************************************/

AdhocWifiNetDevice::AdhocWifiNetDevice (Ptr<Node> node)
  : WifiNetDevice (node)
{
  m_ssid = WifiDefaultParameters::GetSsid ();
  m_dca = CreateDca (15, 1023);

  MacHighAdhoc *high = new MacHighAdhoc ();
  high->SetDevice (this);
  high->SetDcaTxop (m_dca);
  high->SetForwardCallback (MakeCallback (&AdhocWifiNetDevice::DoForwardUp, 
                                          static_cast<WifiNetDevice *> (this)));
  high->SetPhy (m_phy);
  high->SetStations (m_stations);
  m_rxMiddle->SetForwardCallback (MakeCallback (&MacHighAdhoc::Receive, high));
  m_high = high;
}
AdhocWifiNetDevice::~AdhocWifiNetDevice ()
{}
Mac48Address 
AdhocWifiNetDevice::GetBssid (void) const
{
  return m_high->GetBssid ();
}
Ssid 
AdhocWifiNetDevice::GetSsid (void) const
{
  return m_ssid;
}
void 
AdhocWifiNetDevice::SetSsid (Ssid ssid)
{
  // XXX restart adhoc network join.
  m_ssid = ssid;
}
bool
AdhocWifiNetDevice::DoSendTo (const Packet &packet, Mac48Address const &to)
{
  m_high->Enqueue (packet, to);
  return true;
}
void
AdhocWifiNetDevice::NotifyConnected (void)
{
  NotifyLinkUp ();
}
void 
AdhocWifiNetDevice::DoDispose (void)
{
  // chain up.
  WifiNetDevice::DoDispose ();
  // local cleanup
  delete m_dca;
  delete m_high;
  m_dca = 0;
  m_high = 0;
}


/*****************************************************
 *            STA code
 *****************************************************/

NqstaWifiNetDevice::NqstaWifiNetDevice (Ptr<Node> node)
  : WifiNetDevice (node)
{
  m_ssid = WifiDefaultParameters::GetSsid ();
  m_dca = CreateDca (15, 1023);
  
  MacHighNqsta *high = new MacHighNqsta ();
  high->SetDevice (this);
  high->SetDcaTxop (m_dca);
  high->SetForwardCallback (MakeCallback (&NqstaWifiNetDevice::DoForwardUp, 
                                          this));
  high->SetAssociatedCallback (MakeCallback (&NqstaWifiNetDevice::Associated, 
                                             this));
  high->SetDisAssociatedCallback (MakeCallback (&NqstaWifiNetDevice::DisAssociated, 
                                                this));
  high->SetStations (m_stations);
  high->SetPhy (m_phy);
  m_rxMiddle->SetForwardCallback (MakeCallback (&MacHighNqsta::Receive, high));
  m_high = high;
}
NqstaWifiNetDevice::~NqstaWifiNetDevice ()
{}
Mac48Address 
NqstaWifiNetDevice::GetBssid (void) const
{
  return m_high->GetBssid ();
}
Ssid 
NqstaWifiNetDevice::GetSsid (void) const
{
  return m_ssid;
}
void 
NqstaWifiNetDevice::StartActiveAssociation (Ssid ssid)
{
  m_ssid = ssid;
  m_high->StartActiveAssociation ();
}
bool
NqstaWifiNetDevice::DoSendTo (const Packet &packet, Mac48Address const &to)
{
  m_high->Queue (packet, to);
  return true;
}
void
NqstaWifiNetDevice::NotifyConnected (void)
{
  // do nothing because link status is kept track of in
  // ::Associated and ::Disassociated
}
void 
NqstaWifiNetDevice::Associated (void)
{
  NetDevice::NotifyLinkUp ();
}

void 
NqstaWifiNetDevice::DisAssociated (void)
{
  NetDevice::NotifyLinkDown ();
}
void 
NqstaWifiNetDevice::DoDispose (void)
{
  // chain up.
  WifiNetDevice::DoDispose ();
  // local cleanup
  delete m_dca;
  delete m_high;
  m_dca = 0;
  m_high = 0;
}


/*****************************************************
 *            AP code
 *****************************************************/


NqapWifiNetDevice::NqapWifiNetDevice (Ptr<Node> node)
  : WifiNetDevice (node)
{
  m_ssid = WifiDefaultParameters::GetSsid ();

  m_dca = CreateDca (15, 1023);

  SupportedRates rates;
  for (uint32_t i = 0; i < m_phy->GetNModes (); i++) 
    {
      WifiMode mode = m_phy->GetMode (i);
      rates.AddSupportedRate (mode.GetPhyRate ());
    }

  MacHighNqap *high = new MacHighNqap ();
  high->SetDevice (this);
  high->SetDcaTxop (m_dca);
  high->SetStations (m_stations);
  high->SetForwardCallback (MakeCallback (&NqapWifiNetDevice::DoForwardUp, 
                                          this));
  high->SetSupportedRates (rates);
  m_rxMiddle->SetForwardCallback (MakeCallback (&MacHighNqap::Receive, high));
  m_high = high;
}
NqapWifiNetDevice::~NqapWifiNetDevice ()
{}
Mac48Address 
NqapWifiNetDevice::GetBssid (void) const
{
  return GetSelfAddress ();
}
Ssid 
NqapWifiNetDevice::GetSsid (void) const
{
  return m_ssid;
}
void 
NqapWifiNetDevice::SetSsid (Ssid ssid)
{
  m_ssid = ssid;
}
bool
NqapWifiNetDevice::DoSendTo (const Packet &packet, Mac48Address const & to)
{
  m_high->Queue (packet, to);
  return true;
}
void
NqapWifiNetDevice::NotifyConnected (void)
{
  NotifyLinkUp ();
}
void 
NqapWifiNetDevice::DoDispose (void)
{
  // chain up.
  WifiNetDevice::DoDispose ();
  // local cleanup
  delete m_dca;
  delete m_high;
  m_dca = 0;
  m_high = 0;
}


} // namespace ns3

