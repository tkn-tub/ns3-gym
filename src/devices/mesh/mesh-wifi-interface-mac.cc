/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 *          Pavel Boyko <boyko@iitp.ru>
 */

#include "ns3/mesh-wifi-interface-mac.h"
#include "ns3/mesh-wifi-beacon.h"
#include "ns3/log.h"
#include "ns3/wifi-phy.h"
#include "ns3/dcf-manager.h"
#include "ns3/mac-rx-middle.h"
#include "ns3/mac-low.h"
#include "ns3/dca-txop.h"
#include "ns3/random-variable.h"
#include "ns3/simulator.h"
#include "ns3/yans-wifi-phy.h"

NS_LOG_COMPONENT_DEFINE ("MeshWifiInterfaceMac");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (MeshWifiInterfaceMac);

TypeId
MeshWifiInterfaceMac::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::MeshWifiInterfaceMac")
    .SetParent<WifiMac> ()
    .AddConstructor<MeshWifiInterfaceMac> ()
    .AddAttribute ("BeaconInterval", "Beacon Interval",
        TimeValue (Seconds (1.0)),
        MakeTimeAccessor (&MeshWifiInterfaceMac::m_beaconInterval),
        MakeTimeChecker ()
        )
    .AddAttribute ("RandomStart", "Window when beacon generating starts (uniform random) in seconds",
        TimeValue (Seconds (0.1)),
        MakeTimeAccessor (&MeshWifiInterfaceMac::m_randomStart),
        MakeTimeChecker ()
        )
    .AddAttribute ("BeaconGeneration", "Enable/Disable Beaconing.",
        BooleanValue (true),
        MakeBooleanAccessor (
          &MeshWifiInterfaceMac::SetBeaconGeneration,
          &MeshWifiInterfaceMac::GetBeaconGeneration
          ),
        MakeBooleanChecker ()
        );
  return tid;
}

MeshWifiInterfaceMac::MeshWifiInterfaceMac ()
{
  NS_LOG_FUNCTION (this);

  m_rxMiddle = new MacRxMiddle ();
  m_rxMiddle->SetForwardCallback (MakeCallback (&MeshWifiInterfaceMac::Receive, this));

  m_low = CreateObject<MacLow> ();
  m_low->SetRxCallback (MakeCallback (&MacRxMiddle::Receive, m_rxMiddle));

  m_dcfManager = new DcfManager ();
  m_dcfManager->SetupLowListener (m_low);

  m_beaconDca = CreateObject<DcaTxop> ();
  m_beaconDca->SetLow (m_low);
  m_beaconDca->SetMinCw (0);
  m_beaconDca->SetMaxCw (0);
  m_beaconDca->SetAifsn (1);
  m_beaconDca->SetManager (m_dcfManager);

  m_VO = CreateObject<DcaTxop> ();
  m_VO->SetLow (m_low);
  m_VO->SetMinCw (3);
  m_VO->SetMaxCw (7);
  m_VO->SetManager (m_dcfManager);

  m_BE = CreateObject<DcaTxop> ();
  m_BE->SetLow (m_low);
  m_BE->SetManager (m_dcfManager);
}

MeshWifiInterfaceMac::~MeshWifiInterfaceMac ()
{
  NS_LOG_FUNCTION (this);
}

//-----------------------------------------------------------------------------
// WifiMac inherited
//-----------------------------------------------------------------------------
void
MeshWifiInterfaceMac::SetSlot (Time slotTime)
{
  NS_LOG_FUNCTION (this << slotTime);
  m_dcfManager->SetSlot (slotTime);
  m_slot = slotTime;
}

void
MeshWifiInterfaceMac::SetSifs (Time sifs)
{
  NS_LOG_FUNCTION (this << sifs);
  m_dcfManager->SetSifs (sifs);
  m_sifs = sifs;
}
void
MeshWifiInterfaceMac::SetAckTimeout (Time ackTimeout)
{
  m_low->SetAckTimeout (ackTimeout);
}

void
MeshWifiInterfaceMac::SetCtsTimeout (Time ctsTimeout)
{
  m_low->SetCtsTimeout (ctsTimeout);
}

void
MeshWifiInterfaceMac::SetPifs (Time pifs)
{
  NS_LOG_FUNCTION (this << pifs);
  m_pifs = pifs;
}
void
MeshWifiInterfaceMac::SetEifsNoDifs (Time eifsNoDifs)
{
  NS_LOG_FUNCTION (this << eifsNoDifs);
  m_dcfManager->SetEifsNoDifs (eifsNoDifs);
  m_eifsNoDifs = eifsNoDifs;
}

Time
MeshWifiInterfaceMac::GetSlot () const
{
  return m_slot;
}

Time
MeshWifiInterfaceMac::GetSifs () const
{
  return m_sifs;
}

Time
MeshWifiInterfaceMac::GetEifsNoDifs () const
{
  return m_eifsNoDifs;
}

Time
MeshWifiInterfaceMac::GetAckTimeout () const
{
  return m_low->GetAckTimeout ();
}

Time
MeshWifiInterfaceMac::GetCtsTimeout () const
{
  return m_low->GetCtsTimeout ();
}

Time
MeshWifiInterfaceMac::GetPifs () const
{
  return m_low->GetPifs ();
}

void
MeshWifiInterfaceMac::SetWifiPhy (Ptr<WifiPhy> phy)
{
  NS_LOG_FUNCTION (this << phy);
  m_phy = phy;
  m_dcfManager->SetupPhyListener (phy);
  m_low->SetPhy (phy);

  NS_LOG_DEBUG("SetWifiPhy: Can switch channel now: " << CanSwitchChannel() ); // TMP
}

void
MeshWifiInterfaceMac::SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> stationManager)
{
  NS_LOG_FUNCTION (this << stationManager);
  m_stationManager = stationManager;
  m_BE->SetWifiRemoteStationManager (stationManager);
  m_VO->SetWifiRemoteStationManager (stationManager);
  m_beaconDca->SetWifiRemoteStationManager (stationManager);
  m_low->SetWifiRemoteStationManager (stationManager);
}

void
MeshWifiInterfaceMac::Enqueue (Ptr<const Packet> packet, Mac48Address to, Mac48Address from)
{
  NS_LOG_FUNCTION (this << packet << to << from);
  ForwardDown (packet, from, to);
}

void
MeshWifiInterfaceMac::Enqueue (Ptr<const Packet> packet, Mac48Address to)
{
  NS_LOG_FUNCTION (this << packet << to);
  ForwardDown (packet, m_low->GetAddress (), to);
}

bool
MeshWifiInterfaceMac::SupportsSendFrom () const
{
  return true;
}

void
MeshWifiInterfaceMac::SetForwardUpCallback (Callback<void,Ptr<Packet>, Mac48Address, Mac48Address> upCallback)
{
  NS_LOG_FUNCTION (this);
  m_upCallback = upCallback;
}

void
MeshWifiInterfaceMac::SetLinkUpCallback (Callback<void> linkUp)
{
  NS_LOG_FUNCTION (this);
  if (!linkUp.IsNull ())
    {
      linkUp ();
    }
}

void
MeshWifiInterfaceMac::SetLinkDownCallback (Callback<void> linkDown)
{
  NS_LOG_FUNCTION (this);
}

Mac48Address
MeshWifiInterfaceMac::GetAddress () const
{
  return m_address;
}
Mac48Address
MeshWifiInterfaceMac::GetBssid () const
{
  return m_address;
}

Ssid
MeshWifiInterfaceMac::GetSsid () const
{
  return m_meshId;
}

void
MeshWifiInterfaceMac::SetAddress (Mac48Address address)
{
  NS_LOG_FUNCTION (address);
  m_low->SetAddress (address);
  m_address = address;
}

void
MeshWifiInterfaceMac::SetSsid (Ssid ssid)
{
  NS_LOG_FUNCTION (ssid);
  m_meshId = ssid;
}

void
MeshWifiInterfaceMac::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_rxMiddle;
  delete m_dcfManager;
  //Delete smart pointers:
  m_rxMiddle = 0;
  m_low = 0;
  m_dcfManager = 0;
  m_phy = 0;
  m_BE = 0;
  m_VO = 0;
  m_beaconSendEvent.Cancel ();
  m_beaconDca = 0;

  WifiMac::DoDispose ();
}

//-----------------------------------------------------------------------------
// Plugins
//-----------------------------------------------------------------------------
void
MeshWifiInterfaceMac::InstallPlugin ( Ptr<MeshWifiInterfaceMacPlugin> plugin)
{
  NS_LOG_FUNCTION (this);

  plugin->SetParent (this);
  m_plugins.push_back (plugin);
}

//-----------------------------------------------------------------------------
// Switch channels
//-----------------------------------------------------------------------------
bool MeshWifiInterfaceMac::CanSwitchChannel () const
{
  NS_LOG_FUNCTION (this);

  // now only YansWifiPhy can switch channels runtime
  if (m_phy != 0)
    {
      Ptr<YansWifiPhy> phy = m_phy->GetObject<YansWifiPhy> ();
      return (phy != 0);
    }
  else
    return false;
}

uint16_t MeshWifiInterfaceMac::GetFrequencyChannel () const
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (m_phy != 0); // need PHY to set/get channel

  Ptr<YansWifiPhy> phy = m_phy->GetObject<YansWifiPhy> ();
  if (phy != 0)
    return phy->GetFrequencyChannel ();
  else
    return 0;
}

void MeshWifiInterfaceMac::SwitchFrequencyChannel (uint16_t new_id)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (m_phy != 0); // need PHY to set/get channel
  /* TODO
   *
   * Correct channel switching is:
   *
   * 1. Interface down, e.g. to stop packets from layer 3
   * 2. Wait before all output queues will be empty
   * 3. Switch PHY channel
   * 4. Interface up
   *
   * Now we use dirty channel switch -- just change frequency
   */
  NS_ASSERT(CanSwitchChannel());

  Ptr<YansWifiPhy> phy = m_phy->GetObject<YansWifiPhy> ();
  phy->SetFrequencyChannel (new_id);
  // Don't know NAV on new channel
  m_dcfManager->NotifyNavResetNow (Seconds (0));
}

//-----------------------------------------------------------------------------
// Forward frame up/down
//-----------------------------------------------------------------------------
void
MeshWifiInterfaceMac::ForwardUp (Ptr<Packet> packet, Mac48Address src, Mac48Address dst)
{
  NS_LOG_FUNCTION (this << packet << src);
  m_upCallback (packet, src, dst);
}

void
MeshWifiInterfaceMac::ForwardDown (Ptr<const Packet> const_packet, Mac48Address from, Mac48Address to)
{
  // copy packet to allow modifications
  Ptr<Packet> packet = const_packet->Copy ();
  WifiMacHeader hdr;
  hdr.SetTypeData ();
  hdr.SetAddr2 (GetAddress ());
  hdr.SetAddr3 (to);
  hdr.SetAddr4 (from);
  hdr.SetDsFrom ();
  hdr.SetDsTo ();

  // Address 1 is unknwon here. Routing plugin is responsible to correctly set it.
  hdr.SetAddr1 (Mac48Address ());

  // Filter packet through all installed plugins
  for (PluginList::const_iterator i = m_plugins.end()-1; i != m_plugins.begin()-1; i--)
    {
      bool drop = ! ((*i)->UpdateOutcomingFrame(packet, hdr, from, to));
      if (drop) return; // plugin drops frame
    }

  // Assert that address1 is set. Assert will fail e.g. if there is no installed routing plugin.
  NS_ASSERT (hdr.GetAddr1() != Mac48Address() );

  // Queue frame
  WifiRemoteStation *destination = m_stationManager->Lookup (to);

  if (destination->IsBrandNew ())
    {
      // in adhoc mode, we assume that every destination
      // supports all the rates we support.
      for (uint32_t i = 0; i < m_phy->GetNModes (); i++)
        {
          destination->AddSupportedMode (m_phy->GetMode (i));
        }
      destination->RecordDisassociated ();
    }
  m_stats.sentFrames ++;
  m_stats.sentBytes += packet->GetSize ();
  m_BE->Queue (packet, hdr);
}

void
MeshWifiInterfaceMac::SendManagementFrame (Ptr<Packet> packet, const WifiMacHeader& hdr)
{
  //Filter management frames:
  WifiMacHeader header = hdr;
  for (PluginList::const_iterator i = m_plugins.end()-1; i != m_plugins.begin()-1; i--)
    {
      bool drop = ! ((*i)->UpdateOutcomingFrame(packet, header, Mac48Address (), Mac48Address ()));
      if (drop) return; // plugin drops frame
    }
  m_stats.sentFrames ++;
  m_stats.sentBytes += packet->GetSize ();
  m_VO->Queue (packet, header);
}

SupportedRates
MeshWifiInterfaceMac::GetSupportedRates () const
{
  // set the set of supported rates and make sure that we indicate
  // the Basic Rate set in this set of supported rates.
  SupportedRates rates;
  for (uint32_t i = 0; i < m_phy->GetNModes (); i++)
    {
      WifiMode mode = m_phy->GetMode (i);
      rates.AddSupportedRate (mode.GetDataRate ());
    }
  // set the basic rates
  for (uint32_t j = 0; j < m_stationManager->GetNBasicModes (); j++)
    {
      WifiMode mode = m_stationManager->GetBasicMode (j);
      rates.SetBasicRate (mode.GetDataRate ());
    }
  return rates;
}
bool
MeshWifiInterfaceMac::CheckMeshId(Ssid meshId) const
{
  return true;
}
bool
MeshWifiInterfaceMac::CheckSupportedRates(SupportedRates rates) const
{
  for (uint32_t i = 0; i < m_stationManager->GetNBasicModes (); i++)
  {
    WifiMode mode = m_stationManager->GetBasicMode (i);
    if (!rates.IsSupportedRate (mode.GetDataRate ()))
      return false;
  }
  return true;
}

//-----------------------------------------------------------------------------
// Beacons
//-----------------------------------------------------------------------------
void 
MeshWifiInterfaceMac::SetRandomStartDelay (Time interval)
{
  NS_LOG_FUNCTION (this << interval);
  m_randomStart = interval;
}

void
MeshWifiInterfaceMac::SetBeaconInterval (Time interval)
{
  NS_LOG_FUNCTION (this << interval);
  m_beaconInterval = interval;
}

Time
MeshWifiInterfaceMac::GetBeaconInterval () const
{
  return m_beaconInterval;
}

void
MeshWifiInterfaceMac::SetBeaconGeneration (bool enable)
{
  NS_LOG_FUNCTION (this << enable);
  if (enable)
    {
      // Now start sending beacons after some random delay (to avoid collisions)
      UniformVariable coefficient (0.0, m_randomStart.GetSeconds());
      Time randomStart = Seconds (coefficient.GetValue());

      m_beaconSendEvent = Simulator::Schedule (randomStart, &MeshWifiInterfaceMac::SendBeacon, this);
      m_tbtt = Simulator::Now() + randomStart;
    }
  else
    // stop sending beacons
    m_beaconSendEvent.Cancel ();
}

bool
MeshWifiInterfaceMac::GetBeaconGeneration () const
{
  return m_beaconSendEvent.IsRunning ();
}

Time
MeshWifiInterfaceMac::GetTbtt () const
{
  return m_tbtt;
}

void MeshWifiInterfaceMac::ShiftTbtt (Time shift)
{
  // User of ShiftTbtt () must take care don't shift it to the past
  NS_ASSERT (GetTbtt() + shift > Simulator::Now());

  m_tbtt += shift;
  // Shift scheduled event
  Simulator::Cancel (m_beaconSendEvent);
  m_beaconSendEvent = Simulator::Schedule (GetTbtt () - Simulator::Now(), &MeshWifiInterfaceMac::SendBeacon, this);
}

void
MeshWifiInterfaceMac::ScheduleNextBeacon ()
{
  m_tbtt += GetBeaconInterval ();
  m_beaconSendEvent = Simulator::Schedule (GetBeaconInterval(), &MeshWifiInterfaceMac::SendBeacon, this);
}

void
MeshWifiInterfaceMac::SendBeacon ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG (GetAddress() <<" is sending beacon");

  NS_ASSERT (! m_beaconSendEvent.IsRunning());
  NS_ASSERT (Simulator::Now().GetMicroSeconds() == GetTbtt().GetMicroSeconds());     // assert that beacon is just on time

  // Form & send beacon
  MeshWifiBeacon beacon (GetSsid (), GetSupportedRates (), m_beaconInterval.GetMicroSeconds ());

  // Ask all plugins to add their specific information elements to beacon
  for (PluginList::const_iterator i = m_plugins.begin(); i != m_plugins.end(); ++i)
    (*i)->UpdateBeacon (beacon);

  m_beaconDca->Queue (beacon.CreatePacket(), beacon.CreateHeader(GetAddress (), GetMeshPointAddress ()));

  ScheduleNextBeacon ();
}

void
MeshWifiInterfaceMac::Receive (Ptr<Packet> packet, WifiMacHeader const *hdr)
{
  // Process beacon
  if((hdr->GetAddr1() != GetAddress()) && (hdr->GetAddr1() != Mac48Address::GetBroadcast()))
    return;
  if (hdr->IsBeacon ())
    {
      m_stats.recvBeacons ++;
      MgtBeaconHeader beacon_hdr;
      Mac48Address from = hdr->GetAddr2 ();

      packet->PeekHeader (beacon_hdr);

      NS_LOG_DEBUG ("Beacon received from "<<hdr->GetAddr2()<<
                   " I am "<<GetAddress ()<<
                   " at "<<Simulator::Now ().GetMicroSeconds ()<<
                   " microseconds");

      // update supported rates
      if (beacon_hdr.GetSsid ().IsEqual(GetSsid()))
        {
          SupportedRates rates = beacon_hdr.GetSupportedRates ();
          WifiRemoteStation * peerSta = m_stationManager->Lookup (hdr->GetAddr2 ());

          for (uint32_t i = 0; i < m_phy->GetNModes (); i++)
          {
            WifiMode mode = m_phy->GetMode (i);
            if (rates.IsSupportedRate (mode.GetDataRate ()))
              {
                peerSta->AddSupportedMode (mode);
                if (rates.IsBasicRate (mode.GetDataRate ()))
                  m_stationManager->AddBasicMode (mode);
              }
          }
        }
    }
  else
  {
    m_stats.recvBytes += packet->GetSize ();
    m_stats.recvFrames ++;
  }
  // Filter frame through all installed plugins
  for (PluginList::iterator i = m_plugins.begin (); i != m_plugins.end(); ++i)
    {
      bool drop = ! ((*i)->Receive(packet, *hdr));
      if (drop) return; // plugin drops frame
    }

  // Forward data up
  if (hdr->IsData ())
      ForwardUp (packet, hdr->GetAddr4(), hdr->GetAddr3());
}

uint32_t
MeshWifiInterfaceMac::GetLinkMetric (Mac48Address peerAddress)
{
  uint32_t metric = 1;
  if(!m_linkMetricCallback.IsNull ())
    metric = m_linkMetricCallback(peerAddress, this);
  return metric;
}

void
MeshWifiInterfaceMac::SetLinkMetricCallback (Callback<uint32_t, Mac48Address, Ptr<MeshWifiInterfaceMac> > cb)
{
  m_linkMetricCallback = cb;
}

Ptr<WifiRemoteStationManager>
MeshWifiInterfaceMac::GetStationManager()
{
  return m_stationManager;
}

void
MeshWifiInterfaceMac::SetMeshPointAddress (Mac48Address a)
{
  m_mpAddress = a;
}

Mac48Address 
MeshWifiInterfaceMac::GetMeshPointAddress () const
{
  return m_mpAddress;
}
//Statistics:
void
MeshWifiInterfaceMac::Statistics::Print (std::ostream & os) const
{
  os << "<Statistics "
    "recvBeacons=\"" << recvBeacons << "\" "
    "sentFrames=\"" << sentFrames << "\" "
    "sentBytes=\"" << (double)sentBytes / 1024.0 << "K\" "
    "recvFrames=\"" << recvFrames << "\" "
    "recvBytes=\"" << (double)recvBytes / 1024.0 << "K\"/>\n";
}
void
MeshWifiInterfaceMac::Report (std::ostream & os) const
{
  os << "<Interface "
    "BeaconInterval=\"" << GetBeaconInterval ().GetSeconds() << "s\" "
    "Channel=\"" << GetFrequencyChannel () << "\" "
    "Address = \"" << GetAddress () << "\">\n";
  m_stats.Print (os);
  os << "</Interface>\n";
}
void
MeshWifiInterfaceMac::ResetStats ()
{
  m_stats = Statistics::Statistics ();
}
} // namespace ns3

