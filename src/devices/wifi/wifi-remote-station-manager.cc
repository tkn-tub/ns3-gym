/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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

#include "wifi-remote-station-manager.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/tag.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/wifi-phy.h"
#include "ns3/trace-source-accessor.h"

NS_LOG_COMPONENT_DEFINE ("WifiRemoteStationManager");

namespace ns3 {

/**
 * _all_ broadcast and multicast frames are transmitted
 * at the same constant default rate because since we don't
 * have any kind of feedback from their transmission,
 * we cannot adjust the rate, so, we pick one which ensures
 * that all frames reach destination.
 */
class NonUnicastWifiRemoteStation : public WifiRemoteStation
{
public:
  NonUnicastWifiRemoteStation (Ptr<WifiRemoteStationManager> stations);
protected:
  virtual void DoReportRxOk (double rxSnr, WifiMode txMode);
  virtual void DoReportRtsFailed (void);
  virtual void DoReportDataFailed (void);
  virtual void DoReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr);
  virtual void DoReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr);
  virtual void DoReportFinalRtsFailed (void);
  virtual void DoReportFinalDataFailed (void);
private:
  virtual Ptr<WifiRemoteStationManager> GetManager (void) const;
  virtual WifiMode DoGetDataMode (uint32_t size);
  virtual WifiMode DoGetRtsMode (void);
  Ptr<WifiRemoteStationManager> m_stations;
};

NonUnicastWifiRemoteStation::NonUnicastWifiRemoteStation (Ptr<WifiRemoteStationManager> stations)
  : m_stations (stations)
{
  RecordDisassociated ();
}
void 
NonUnicastWifiRemoteStation::DoReportRxOk (double rxSnr, WifiMode txMode)
{
  NS_ASSERT (false);
}
void 
NonUnicastWifiRemoteStation::DoReportRtsFailed (void)
{
  NS_ASSERT (false);
}
void 
NonUnicastWifiRemoteStation::DoReportDataFailed (void)
{
  NS_ASSERT (false);
}
void 
NonUnicastWifiRemoteStation::DoReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  NS_ASSERT (false);
}
void 
NonUnicastWifiRemoteStation::DoReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr)
{
  NS_ASSERT (false);
}
void 
NonUnicastWifiRemoteStation::DoReportFinalRtsFailed (void)
{}
void 
NonUnicastWifiRemoteStation::DoReportFinalDataFailed (void)
{}

WifiMode 
NonUnicastWifiRemoteStation::DoGetDataMode (uint32_t size)
{
  WifiMode mode = m_stations->GetBasicMode (0);
  NS_LOG_DEBUG ("non-unicast size="<<size<<", mode="<<mode);
  return mode;
}
WifiMode 
NonUnicastWifiRemoteStation::DoGetRtsMode (void)
{
  NS_ASSERT (false);
  // theoretically, no rts for broadcast/multicast packets.
  return m_stations->GetBasicMode (0);
}
Ptr<WifiRemoteStationManager>
NonUnicastWifiRemoteStation::GetManager (void) const
{
  return m_stations;
}


} // namespace ns3

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (WifiRemoteStationManager);

TypeId 
WifiRemoteStationManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WifiRemoteStationManager")
    .SetParent<Object> ()
    .AddAttribute ("IsLowLatency", "XXX",
                   BooleanValue (true),
                   MakeBooleanAccessor (&WifiRemoteStationManager::m_isLowLatency),
                   MakeBooleanChecker ())
    .AddAttribute ("MaxSsrc", "XXX",
                   UintegerValue (7),
                   MakeUintegerAccessor (&WifiRemoteStationManager::m_maxSsrc),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MaxSlrc", "XXX",
                   UintegerValue (7),
                   MakeUintegerAccessor (&WifiRemoteStationManager::m_maxSlrc),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("RtsCtsThreshold", "XXX",
                   UintegerValue (1500),
                   MakeUintegerAccessor (&WifiRemoteStationManager::m_rtsCtsThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("FragmentationThreshold", "XXX",
                   UintegerValue (1500),
                   MakeUintegerAccessor (&WifiRemoteStationManager::m_fragmentationThreshold),
                   MakeUintegerChecker<uint32_t> ())
    ;
  return tid;
}

WifiRemoteStationManager::WifiRemoteStationManager ()
  : m_nonUnicast (new NonUnicastWifiRemoteStation (this))
{}

WifiRemoteStationManager::~WifiRemoteStationManager ()
{
}
void 
WifiRemoteStationManager::DoDispose (void)
{
  for (Stations::const_iterator i = m_stations.begin (); i != m_stations.end (); i++) 
    {
      delete (*i).second;
    }
  m_stations.clear ();
  delete m_nonUnicast;
}
void
WifiRemoteStationManager::SetupPhy (Ptr<WifiPhy> phy)
{
  m_defaultTxMode = phy->GetMode (0);
  Reset ();
}

uint32_t 
WifiRemoteStationManager::GetMaxSsrc (void) const
{
  return m_maxSsrc;
}
uint32_t 
WifiRemoteStationManager::GetMaxSlrc (void) const
{
  return m_maxSlrc;
}
uint32_t 
WifiRemoteStationManager::GetRtsCtsThreshold (void) const
{
  return m_rtsCtsThreshold;
}
uint32_t 
WifiRemoteStationManager::GetFragmentationThreshold (void) const
{
  return m_fragmentationThreshold;
}
void 
WifiRemoteStationManager::SetMaxSsrc (uint32_t maxSsrc)
{
  m_maxSsrc = maxSsrc;
}
void 
WifiRemoteStationManager::SetMaxSlrc (uint32_t maxSlrc)
{
  m_maxSlrc = maxSlrc;
}
void 
WifiRemoteStationManager::SetRtsCtsThreshold (uint32_t threshold)
{
  m_rtsCtsThreshold = threshold;
}
void 
WifiRemoteStationManager::SetFragmentationThreshold (uint32_t threshold)
{
  m_fragmentationThreshold = threshold;
} 

WifiRemoteStation *
WifiRemoteStationManager::Lookup (Mac48Address address)
{
  if (address.IsBroadcast () ||
      address.IsMulticast ())
    {
      return m_nonUnicast;
    }
  for (Stations::const_iterator i = m_stations.begin (); i != m_stations.end (); i++) 
    {
      if ((*i).first == address)
        {
          return (*i).second;
        }
    }
  WifiRemoteStation *station = CreateStation ();
  station->Reset ();
  m_stations.push_back (std::make_pair (address, station));
  return station;
}

WifiRemoteStation *
WifiRemoteStationManager::LookupNonUnicast (void)
{
  return m_nonUnicast;
}

WifiMode 
WifiRemoteStationManager::GetDefaultMode (void) const
{
  return m_defaultTxMode;
}
void
WifiRemoteStationManager::Reset (void)
{
  for (Stations::const_iterator i = m_stations.begin (); i != m_stations.end (); i++)
    {
      delete i->second;
    }
  m_stations.clear ();
  m_basicModes.clear ();
  m_basicModes.push_back (m_defaultTxMode);
  NS_ASSERT (m_defaultTxMode.IsMandatory ());
}
void 
WifiRemoteStationManager::AddBasicMode (WifiMode mode)
{
  for (uint32_t i = 0; i < GetNBasicModes (); i++)
    {
      if (GetBasicMode (i) == mode)
        {
          return;
        }
    }
  m_basicModes.push_back (mode);
}
uint32_t 
WifiRemoteStationManager::GetNBasicModes (void) const
{
  return m_basicModes.size ();
}
WifiMode 
WifiRemoteStationManager::GetBasicMode (uint32_t i) const
{
  NS_ASSERT (i < m_basicModes.size ());
  return m_basicModes[i];
}
WifiRemoteStationManager::BasicModesIterator 
WifiRemoteStationManager::BeginBasicModes (void) const
{
  return m_basicModes.begin ();
}
WifiRemoteStationManager::BasicModesIterator 
WifiRemoteStationManager::EndBasicModes (void) const
{
  return m_basicModes.end ();
}
bool
WifiRemoteStationManager::IsLowLatency (void) const
{
  return m_isLowLatency;
}

} // namespace ns3

/***************************************************************
 *           Packet Mode Tagger
 ***************************************************************/ 

namespace ns3 {

class TxModeTag : public Tag
{
public:
  TxModeTag ();
  TxModeTag (WifiMode rtsMode, WifiMode dataMode);
  WifiMode GetRtsMode (void) const;
  WifiMode GetDataMode (void) const;

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
private:
  WifiMode m_rtsMode;
  WifiMode m_dataMode;
};

TxModeTag::TxModeTag ()
{}
TxModeTag::TxModeTag (WifiMode rtsMode, WifiMode dataMode)
  : m_rtsMode (rtsMode),
    m_dataMode (dataMode)
{}
WifiMode 
TxModeTag::GetRtsMode (void) const
{
  return m_rtsMode;
}
WifiMode 
TxModeTag::GetDataMode (void) const
{
  return m_dataMode;
}
TypeId 
TxModeTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TxModeTag")
    .SetParent<Tag> ()
    .AddConstructor<TxModeTag> ()
    .AddAttribute ("RtsTxMode", 
                   "Tx mode of rts to use later",
                   EmptyAttributeValue (),
                   MakeWifiModeAccessor (&TxModeTag::GetRtsMode),
                   MakeWifiModeChecker ())
    .AddAttribute ("DataTxMode", 
                   "Tx mode of data to use later",
                   EmptyAttributeValue (),
                   MakeWifiModeAccessor (&TxModeTag::GetDataMode),
                   MakeWifiModeChecker ())
    ;
  return tid;
}
TypeId 
TxModeTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t 
TxModeTag::GetSerializedSize (void) const
{
  return sizeof (WifiMode) * 2;
}
void 
TxModeTag::Serialize (TagBuffer i) const
{
  i.Write ((uint8_t *)&m_rtsMode, sizeof (WifiMode));
  i.Write ((uint8_t *)&m_dataMode, sizeof (WifiMode));
}
void 
TxModeTag::Deserialize (TagBuffer i)
{
  i.Read ((uint8_t *)&m_rtsMode, sizeof (WifiMode));
  i.Read ((uint8_t *)&m_dataMode, sizeof (WifiMode));
}

} // namespace ns3


/***************************************************************
 *           WifiRemoteStation below.
 ***************************************************************/ 

namespace ns3 {

TypeId 
WifiRemoteStation::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WifiRemoteStation")
    .SetParent<Object> ()
    .AddTraceSource ("Ssrc", "XXX",
                     MakeTraceSourceAccessor (&WifiRemoteStation::m_ssrc))
    .AddTraceSource ("Slrc", "XXX",
                     MakeTraceSourceAccessor (&WifiRemoteStation::m_slrc))
    ;
  return tid;
}

WifiRemoteStation::WifiRemoteStation ()
  : m_state (BRAND_NEW),
    m_ssrc (0),
    m_slrc (0)
{}
WifiRemoteStation::~WifiRemoteStation ()
{}

bool
WifiRemoteStation::IsBrandNew (void) const
{
  return m_state == BRAND_NEW;
}

bool 
WifiRemoteStation::IsAssociated (void) const
{
  return m_state == GOT_ASSOC_TX_OK;
}
bool 
WifiRemoteStation::IsWaitAssocTxOk (void) const
{
  return m_state == WAIT_ASSOC_TX_OK;
}
void 
WifiRemoteStation::RecordWaitAssocTxOk (void)
{
  m_state = WAIT_ASSOC_TX_OK;
}
void 
WifiRemoteStation::RecordGotAssocTxOk (void)
{
  m_state = GOT_ASSOC_TX_OK;
}
void 
WifiRemoteStation::RecordGotAssocTxFailed (void)
{
  m_state = DISASSOC;
}
void 
WifiRemoteStation::RecordDisassociated (void)
{
  m_state = DISASSOC;
}

void 
WifiRemoteStation::Reset (void)
{
  m_modes.clear ();
  AddSupportedMode (GetManager ()->GetDefaultMode ());
}
void 
WifiRemoteStation::AddSupportedMode (WifiMode mode)
{
  if (IsIn (mode))
    {
      return;
    }
  m_modes.push_back (mode);
}

bool
WifiRemoteStation::IsIn (WifiMode mode) const
{
  for (SupportedModes::const_iterator i = m_modes.begin (); i != m_modes.end (); i++)
    {
      if ((*i) == mode)
        {
          return true;
        }
    }
  return false;
}

WifiMode
WifiRemoteStation::GetControlAnswerMode (WifiMode reqMode)
{
  /**
   * see ieee 802.11e, section 9.6:
   * 
   * To allow the transmitting STA to calculate the contents of 
   * the Duration/ID field, a STA responding to a received frame 
   * shall transmit its Control Response frame (either CTS or ACK) 
   * frames, other than the Block-Ack control frame, at the highest 
   * rate in the BSSBasicRateSet parameter that is less than or equal 
   * to the rate of the immediately previous frame in the frame 
   * exchange sequence (as defined in 9.79.12) and that is of the
   * same modulation type as the received frame. If no rate in the 
   * basic rate set meets these conditions, then the control frame 
   * sent in response to a received frame shall be transmitted at 
   * the highest mandatory rate of the PHY that is less than or equal 
   * to the rate of the received frame, and that is of the same 
   * modulation type as the received frame. In addition, the Control 
   * Response frame shall be sent using the same PHY options as the
   * received frame, unless they conflict with the requirement to use 
   * the BSSBasicRateSet parameter.
   */
  WifiMode mode = GetManager ()->GetDefaultMode ();
  bool found = false;

  // First, search the BSS Basic Rate set
  for (WifiRemoteStationManager::BasicModesIterator i = GetManager ()->BeginBasicModes (); 
       i != GetManager ()->EndBasicModes (); i++)
    {
      if (i->GetPhyRate () > mode.GetPhyRate () &&
          i->GetPhyRate () <= reqMode.GetPhyRate () &&
          i->GetModulationType () == reqMode.GetModulationType ())
        {
          mode = *i;
          found = true;
        }
    }
  // no need to search Mandatory rate set because it is included
  // within the Basic rate set.
  return mode;
}

WifiMode 
WifiRemoteStation::GetCtsMode (WifiMode rtsMode)
{
  return GetControlAnswerMode (rtsMode);
}
WifiMode 
WifiRemoteStation::GetAckMode (WifiMode dataMode)
{
  return GetControlAnswerMode (dataMode);
}

uint32_t 
WifiRemoteStation::GetNSupportedModes (void) const
{
  return m_modes.size ();
}
WifiMode 
WifiRemoteStation::GetSupportedMode (uint32_t i) const
{
  NS_ASSERT (i < m_modes.size ());
  return m_modes[i];
}
void 
WifiRemoteStation::PrepareForQueue (Ptr<const Packet> packet, uint32_t fullPacketSize)
{
  if (GetManager ()->IsLowLatency ())
    {
      return;
    }
  TxModeTag tag = TxModeTag (DoGetRtsMode (), DoGetDataMode (fullPacketSize));
  packet->AddTag (tag);
}
WifiMode 
WifiRemoteStation::GetDataMode (Ptr<const Packet> packet, uint32_t fullPacketSize)
{
  if (GetManager ()->IsLowLatency ())
    {
      return DoGetDataMode (fullPacketSize);
    }
  TxModeTag tag;
  bool found;
  found = packet->FindFirstMatchingTag (tag);
  NS_ASSERT (found);
  return tag.GetDataMode ();
}
WifiMode 
WifiRemoteStation::GetRtsMode (Ptr<const Packet> packet)
{
  if (GetManager ()->IsLowLatency ())
    {
      return DoGetRtsMode ();
    }
  TxModeTag tag;
  bool found;
  found = packet->FindFirstMatchingTag (tag);
  NS_ASSERT (found);
  return tag.GetRtsMode ();
}

bool
WifiRemoteStation::NeedRts (Ptr<const Packet> packet)
{
  if (packet->GetSize () > GetManager ()->GetRtsCtsThreshold ()) 
    {
      return true;
    } 
  else 
    {
      return false;
    }
}
bool
WifiRemoteStation::NeedRtsRetransmission (Ptr<const Packet> packet)
{
  return (m_ssrc < GetManager ()->GetMaxSsrc ());
}

bool
WifiRemoteStation::NeedDataRetransmission (Ptr<const Packet> packet)
{
  return (m_slrc < GetManager ()->GetMaxSlrc ());
}

bool
WifiRemoteStation::NeedFragmentation (Ptr<const Packet> packet)
{
  if (packet->GetSize () > GetManager ()->GetFragmentationThreshold ()) 
    {
      return true;
    } 
  else 
    {
      return false;
    }
}
uint32_t
WifiRemoteStation::GetNFragments (Ptr<const Packet> packet)
{
  uint32_t nFragments = packet->GetSize () / GetManager ()->GetFragmentationThreshold () + 1;
  return nFragments;
}

uint32_t
WifiRemoteStation::GetFragmentSize (Ptr<const Packet> packet, uint32_t fragmentNumber)
{
  uint32_t nFragment = GetNFragments (packet);
  if (fragmentNumber >= nFragment)
    {
      return 0;
    }
  if (fragmentNumber == nFragment - 1)
    {
      uint32_t lastFragmentSize = packet->GetSize () % GetManager ()->GetFragmentationThreshold ();
      return lastFragmentSize;
    }
  else
    {
      return GetManager ()->GetFragmentationThreshold ();
    }
}

bool
WifiRemoteStation::IsLastFragment (Ptr<const Packet> packet, uint32_t fragmentNumber) 
{
  if (fragmentNumber == (GetNFragments (packet) - 1)) 
    {
      return true;
    } 
  else 
    {
      return false;
    }
}

void 
WifiRemoteStation::ReportRtsFailed (void)
{
  m_ssrc++;
  DoReportRtsFailed ();
}

void 
WifiRemoteStation::ReportDataFailed (void)
{
  m_slrc++;
  DoReportDataFailed ();
}

void 
WifiRemoteStation::ReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  m_ssrc = 0;
  DoReportRtsOk (ctsSnr, ctsMode, rtsSnr);
}

void 
WifiRemoteStation::ReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr)
{
  m_slrc = 0;
  DoReportDataOk (ackSnr, ackMode, dataSnr);
}

void 
WifiRemoteStation::ReportFinalRtsFailed (void)
{
  m_ssrc = 0;
  DoReportFinalRtsFailed ();
}

void 
WifiRemoteStation::ReportFinalDataFailed (void)
{
  m_slrc = 0;
  DoReportFinalDataFailed ();
}

void 
WifiRemoteStation::ReportRxOk (double rxSnr, WifiMode txMode)
{
  DoReportRxOk (rxSnr, txMode);
}
} // namespace ns3

