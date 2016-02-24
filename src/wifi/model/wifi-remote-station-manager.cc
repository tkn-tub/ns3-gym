/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

#include <iostream>
#include "wifi-remote-station-manager.h"
#include "ns3/simulator.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/tag.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/enum.h"
#include "ns3/wifi-phy.h"
#include "ns3/wifi-mac.h"
#include "ns3/trace-source-accessor.h"
#include "wifi-mac-header.h"
#include "wifi-mac-trailer.h"

/***************************************************************
 *           Packet Mode Tagger
 ***************************************************************/

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("WifiRemoteStationManager");

class HighLatencyDataTxVectorTag : public Tag
{
public:
  HighLatencyDataTxVectorTag ();
  HighLatencyDataTxVectorTag (WifiTxVector dataTxVector);
  /**
   * \returns the transmission mode to use to send this packet
   */
  WifiTxVector GetDataTxVector (void) const;

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;
private:
  WifiTxVector m_dataTxVector;
};

HighLatencyDataTxVectorTag::HighLatencyDataTxVectorTag ()
{
}

HighLatencyDataTxVectorTag::HighLatencyDataTxVectorTag (WifiTxVector dataTxVector)
  : m_dataTxVector (dataTxVector)
{
}

WifiTxVector
HighLatencyDataTxVectorTag::GetDataTxVector (void) const
{
  return m_dataTxVector;
}

TypeId
HighLatencyDataTxVectorTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::HighLatencyDataTxVectorTag")
    .SetParent<Tag> ()
    .SetGroupName ("Wifi")
    .AddConstructor<HighLatencyDataTxVectorTag> ()
  ;
  return tid;
}

TypeId
HighLatencyDataTxVectorTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
HighLatencyDataTxVectorTag::GetSerializedSize (void) const
{
  return sizeof (WifiTxVector);
}

void
HighLatencyDataTxVectorTag::Serialize (TagBuffer i) const
{
  i.Write ((uint8_t *)&m_dataTxVector, sizeof (WifiTxVector));
}

void
HighLatencyDataTxVectorTag::Deserialize (TagBuffer i)
{
  i.Read ((uint8_t *)&m_dataTxVector, sizeof (WifiTxVector));
}

void
HighLatencyDataTxVectorTag::Print (std::ostream &os) const
{
  os << "Data=" << m_dataTxVector;
}

class HighLatencyRtsTxVectorTag : public Tag
{
public:
  HighLatencyRtsTxVectorTag ();
  HighLatencyRtsTxVectorTag (WifiTxVector rtsTxVector);
  /**
   * \returns the transmission mode to use to send the RTS prior to the
   *          transmission of the data packet itself.
   */
  WifiTxVector GetRtsTxVector (void) const;

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;
private:
  WifiTxVector m_rtsTxVector;
};

HighLatencyRtsTxVectorTag::HighLatencyRtsTxVectorTag ()
{
}

HighLatencyRtsTxVectorTag::HighLatencyRtsTxVectorTag (WifiTxVector rtsTxVector)
  : m_rtsTxVector (rtsTxVector)
{
}

WifiTxVector
HighLatencyRtsTxVectorTag::GetRtsTxVector (void) const
{
  return m_rtsTxVector;
}

TypeId
HighLatencyRtsTxVectorTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::HighLatencyRtsTxVectorTag")
    .SetParent<Tag> ()
    .SetGroupName ("Wifi")
    .AddConstructor<HighLatencyRtsTxVectorTag> ()
  ;
  return tid;
}

TypeId
HighLatencyRtsTxVectorTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
HighLatencyRtsTxVectorTag::GetSerializedSize (void) const
{
  return sizeof (WifiTxVector);
}

void
HighLatencyRtsTxVectorTag::Serialize (TagBuffer i) const
{
  i.Write ((uint8_t *)&m_rtsTxVector, sizeof (WifiTxVector));
}

void
HighLatencyRtsTxVectorTag::Deserialize (TagBuffer i)
{
  i.Read ((uint8_t *)&m_rtsTxVector, sizeof (WifiTxVector));
}

void
HighLatencyRtsTxVectorTag::Print (std::ostream &os) const
{
  os << "Rts=" << m_rtsTxVector;
}

class HighLatencyCtsToSelfTxVectorTag : public Tag
{
public:
  HighLatencyCtsToSelfTxVectorTag ();
  HighLatencyCtsToSelfTxVectorTag (WifiTxVector ctsToSelfTxVector);
  /**
   * \returns the transmission mode to use for the CTS-to-self.
   */
  WifiTxVector GetCtsToSelfTxVector (void) const;

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;
private:
  WifiTxVector m_ctsToSelfTxVector;
};

HighLatencyCtsToSelfTxVectorTag::HighLatencyCtsToSelfTxVectorTag ()
{
}

HighLatencyCtsToSelfTxVectorTag::HighLatencyCtsToSelfTxVectorTag (WifiTxVector ctsToSelfTxVector)
  : m_ctsToSelfTxVector (ctsToSelfTxVector)
{
}

WifiTxVector
HighLatencyCtsToSelfTxVectorTag::GetCtsToSelfTxVector (void) const
{
  return m_ctsToSelfTxVector;
}

TypeId
HighLatencyCtsToSelfTxVectorTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::HighLatencyCtsToSelfTxVectorTag")
    .SetParent<Tag> ()
    .SetGroupName ("Wifi")
    .AddConstructor<HighLatencyCtsToSelfTxVectorTag> ()
  ;
  return tid;
}

TypeId
HighLatencyCtsToSelfTxVectorTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
HighLatencyCtsToSelfTxVectorTag::GetSerializedSize (void) const
{
  return sizeof (WifiTxVector);
}

void
HighLatencyCtsToSelfTxVectorTag::Serialize (TagBuffer i) const
{
  i.Write ((uint8_t *)&m_ctsToSelfTxVector, sizeof (WifiTxVector));
}

void
HighLatencyCtsToSelfTxVectorTag::Deserialize (TagBuffer i)
{
  i.Read ((uint8_t *)&m_ctsToSelfTxVector, sizeof (WifiTxVector));
}

void
HighLatencyCtsToSelfTxVectorTag::Print (std::ostream &os) const
{
  os << "Cts To Self=" << m_ctsToSelfTxVector;
}

} //namespace ns3

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (WifiRemoteStationManager);

TypeId
WifiRemoteStationManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WifiRemoteStationManager")
    .SetParent<Object> ()
    .SetGroupName ("Wifi")
    .AddAttribute ("IsLowLatency",
                   "If true, we attempt to modelize a so-called low-latency device: "
                   "a device where decisions about tx parameters can be made on a per-packet basis and "
                   "feedback about the transmission of each packet is obtained before sending the next. "
                   "Otherwise, we modelize a high-latency device, that is a device where we cannot update "
                   "our decision about tx parameters after every packet transmission.",
                   TypeId::ATTR_GET,
                   BooleanValue (true), //this value is ignored because there is no setter
                   MakeBooleanAccessor (&WifiRemoteStationManager::IsLowLatency),
                   MakeBooleanChecker ())
    .AddAttribute ("MaxSsrc",
                   "The maximum number of retransmission attempts for an RTS. "
                   " This value will not have any effect on some rate control algorithms.",
                   UintegerValue (7),
                   MakeUintegerAccessor (&WifiRemoteStationManager::m_maxSsrc),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MaxSlrc",
                   "The maximum number of retransmission attempts for a DATA packet. "
                   "This value will not have any effect on some rate control algorithms.",
                   UintegerValue (7),
                   MakeUintegerAccessor (&WifiRemoteStationManager::m_maxSlrc),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("RtsCtsThreshold",
                   "If the size of the PSDU is bigger than this value, we use an RTS/CTS handshake before sending the data frame."
                   "This value will not have any effect on some rate control algorithms.",
                   UintegerValue (65535),
                   MakeUintegerAccessor (&WifiRemoteStationManager::m_rtsCtsThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("FragmentationThreshold",
                   "If the size of the PSDU is bigger than this value, we fragment it such that the size of the fragments are equal or smaller. "
                   "This value does not apply when it is carried in an A-MPDU. "
                   "This value will not have any effect on some rate control algorithms.",
                   UintegerValue (2346),
                   MakeUintegerAccessor (&WifiRemoteStationManager::DoSetFragmentationThreshold,
                                         &WifiRemoteStationManager::DoGetFragmentationThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("NonUnicastMode",
                   "Wifi mode used for non-unicast transmissions.",
                   WifiModeValue (),
                   MakeWifiModeAccessor (&WifiRemoteStationManager::m_nonUnicastMode),
                   MakeWifiModeChecker ())
    .AddAttribute ("DefaultTxPowerLevel",
                   "Default power level to be used for transmissions. "
                   "This is the power level that is used by all those WifiManagers that do not implement TX power control.",
                   UintegerValue (0),
                   MakeUintegerAccessor (&WifiRemoteStationManager::m_defaultTxPowerLevel),
                   MakeUintegerChecker<uint8_t> ())
    .AddAttribute ("ProtectionMode",
                   "Protection mode used when non-ERP STAs are connected to an ERP AP: Rts-Cts or Cts-To-Self",
                   EnumValue (WifiRemoteStationManager::CTS_TO_SELF),
                   MakeEnumAccessor (&WifiRemoteStationManager::SetProtectionMode,
                                     &WifiRemoteStationManager::GetProtectionMode),
                   MakeEnumChecker (WifiRemoteStationManager::RTS_CTS, "Rts-Cts",
                                    WifiRemoteStationManager::CTS_TO_SELF, "Cts-To-Self"))
    .AddTraceSource ("MacTxRtsFailed",
                     "The transmission of a RTS by the MAC layer has failed",
                     MakeTraceSourceAccessor (&WifiRemoteStationManager::m_macTxRtsFailed),
                     "ns3::Mac48Address::TracedCallback")
    .AddTraceSource ("MacTxDataFailed",
                     "The transmission of a data packet by the MAC layer has failed",
                     MakeTraceSourceAccessor (&WifiRemoteStationManager::m_macTxDataFailed),
                     "ns3::Mac48Address::TracedCallback")
    .AddTraceSource ("MacTxFinalRtsFailed",
                     "The transmission of a RTS has exceeded the maximum number of attempts",
                     MakeTraceSourceAccessor (&WifiRemoteStationManager::m_macTxFinalRtsFailed),
                     "ns3::Mac48Address::TracedCallback")
    .AddTraceSource ("MacTxFinalDataFailed",
                     "The transmission of a data packet has exceeded the maximum number of attempts",
                     MakeTraceSourceAccessor (&WifiRemoteStationManager::m_macTxFinalDataFailed),
                     "ns3::Mac48Address::TracedCallback")
  ;
  return tid;
}

WifiRemoteStationManager::WifiRemoteStationManager ()
  : m_htSupported (false),
    m_vhtSupported (false),
    m_useNonErpProtection (false),
    m_shortPreambleEnabled (false),
    m_shortSlotTimeEnabled (false)
{
}

WifiRemoteStationManager::~WifiRemoteStationManager ()
{
}

void
WifiRemoteStationManager::DoDispose (void)
{
  for (StationStates::const_iterator i = m_states.begin (); i != m_states.end (); i++)
    {
      delete (*i);
    }
  m_states.clear ();
  for (Stations::const_iterator i = m_stations.begin (); i != m_stations.end (); i++)
    {
      delete (*i);
    }
  m_stations.clear ();
}

void
WifiRemoteStationManager::SetupPhy (Ptr<WifiPhy> phy)
{
  //We need to track our PHY because it is the object that knows the
  //full set of transmit rates that are supported. We need to know
  //this in order to find the relevant mandatory rates when chosing a
  //transmit rate for automatic control responses like
  //acknowledgements.
  m_wifiPhy = phy;
  m_defaultTxMode = phy->GetMode (0);
  if (HasHtSupported () || HasVhtSupported ())
    {
      m_defaultTxMcs = phy->GetMcs (0);
    }
  Reset ();
}

void
WifiRemoteStationManager::SetupMac (Ptr<WifiMac> mac)
{
  //We need to track our MAC because it is the object that knows the
  //full set of interframe spaces.
  m_wifiMac = mac;
  Reset ();
}

void
WifiRemoteStationManager::SetHtSupported (bool enable)
{
  m_htSupported = enable;
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
  DoSetFragmentationThreshold (threshold);
}

void
WifiRemoteStationManager::SetProtectionMode (WifiRemoteStationManager::ProtectionMode mode)
{
  m_protectionMode = mode;
}

void
WifiRemoteStationManager::SetShortPreambleEnabled (bool enable)
{
  m_shortPreambleEnabled = enable;
}

void
WifiRemoteStationManager::SetShortSlotTimeEnabled (bool enable)
{
  m_shortSlotTimeEnabled = enable;
}

bool
WifiRemoteStationManager::GetShortSlotTimeEnabled (void) const
{
  return m_shortSlotTimeEnabled;
}

bool
WifiRemoteStationManager::GetShortPreambleEnabled (void) const
{
  return m_shortPreambleEnabled;
}

WifiRemoteStationManager::ProtectionMode
WifiRemoteStationManager::GetProtectionMode (void) const
{
  return m_protectionMode;
}

bool
WifiRemoteStationManager::HasHtSupported (void) const
{
  return m_htSupported;
}

void
WifiRemoteStationManager::SetVhtSupported (bool enable)
{
  m_vhtSupported = enable;
}

bool
WifiRemoteStationManager::HasVhtSupported (void) const
{
  return m_vhtSupported;
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
  return DoGetFragmentationThreshold ();
}

void
WifiRemoteStationManager::Reset (Mac48Address address)
{
  NS_LOG_FUNCTION (this << address);
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStationState *state = LookupState (address);
  state->m_operationalRateSet.clear ();
  state->m_operationalMcsSet.clear ();
  AddSupportedMode (address, GetDefaultMode ());
  AddSupportedMcs (address, GetDefaultMcs ());
}

void
WifiRemoteStationManager::AddSupportedPlcpPreamble (Mac48Address address, bool isShortPreambleSupported)
{
  NS_LOG_FUNCTION (this << address << isShortPreambleSupported);
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStationState *state = LookupState (address);
  state->m_shortPreamble = isShortPreambleSupported;
}

void
WifiRemoteStationManager::AddSupportedErpSlotTime (Mac48Address address, bool isShortSlotTimeSupported)
{
  NS_LOG_FUNCTION (this << address << isShortSlotTimeSupported);
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStationState *state = LookupState (address);
  state->m_shortSlotTime = isShortSlotTimeSupported;
}

void
WifiRemoteStationManager::AddSupportedMode (Mac48Address address, WifiMode mode)
{
  NS_LOG_FUNCTION (this << address << mode);
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStationState *state = LookupState (address);
  for (WifiModeListIterator i = state->m_operationalRateSet.begin (); i != state->m_operationalRateSet.end (); i++)
    {
      if ((*i) == mode)
        {
          //already in.
          return;
        }
    }
  state->m_operationalRateSet.push_back (mode);
}

void
WifiRemoteStationManager::AddAllSupportedModes (Mac48Address address)
{
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStationState *state = LookupState (address);
  state->m_operationalRateSet.clear ();
  for (uint32_t i = 0; i < m_wifiPhy->GetNModes (); i++)
    {
      state->m_operationalRateSet.push_back (m_wifiPhy->GetMode (i));
    }
}

void
WifiRemoteStationManager::AddSupportedMcs (Mac48Address address, WifiMode mcs)
{
  NS_LOG_FUNCTION (this << address << mcs);
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStationState *state = LookupState (address);
  for (WifiModeListIterator i = state->m_operationalMcsSet.begin (); i != state->m_operationalMcsSet.end (); i++)
    {
      if ((*i) == mcs)
        {
          //already in.
          return;
        }
    }
  state->m_operationalMcsSet.push_back (mcs);
}

bool
WifiRemoteStationManager::GetShortPreambleSupported (Mac48Address address) const
{
  return LookupState (address)->m_shortPreamble;
}

bool
WifiRemoteStationManager::GetShortSlotTimeSupported (Mac48Address address) const
{
  return LookupState (address)->m_shortSlotTime;
}

bool
WifiRemoteStationManager::IsBrandNew (Mac48Address address) const
{
  if (address.IsGroup ())
    {
      return false;
    }
  return LookupState (address)->m_state == WifiRemoteStationState::BRAND_NEW;
}

bool
WifiRemoteStationManager::IsAssociated (Mac48Address address) const
{
  if (address.IsGroup ())
    {
      return true;
    }
  return LookupState (address)->m_state == WifiRemoteStationState::GOT_ASSOC_TX_OK;
}

bool
WifiRemoteStationManager::IsWaitAssocTxOk (Mac48Address address) const
{
  if (address.IsGroup ())
    {
      return false;
    }
  return LookupState (address)->m_state == WifiRemoteStationState::WAIT_ASSOC_TX_OK;
}

void
WifiRemoteStationManager::RecordWaitAssocTxOk (Mac48Address address)
{
  NS_ASSERT (!address.IsGroup ());
  LookupState (address)->m_state = WifiRemoteStationState::WAIT_ASSOC_TX_OK;
}

void
WifiRemoteStationManager::RecordGotAssocTxOk (Mac48Address address)
{
  NS_ASSERT (!address.IsGroup ());
  LookupState (address)->m_state = WifiRemoteStationState::GOT_ASSOC_TX_OK;
}

void
WifiRemoteStationManager::RecordGotAssocTxFailed (Mac48Address address)
{
  NS_ASSERT (!address.IsGroup ());
  LookupState (address)->m_state = WifiRemoteStationState::DISASSOC;
}

void
WifiRemoteStationManager::RecordDisassociated (Mac48Address address)
{
  NS_ASSERT (!address.IsGroup ());
  LookupState (address)->m_state = WifiRemoteStationState::DISASSOC;
}

void
WifiRemoteStationManager::PrepareForQueue (Mac48Address address, const WifiMacHeader *header,
                                           Ptr<const Packet> packet, uint32_t fullPacketSize)
{
  NS_LOG_FUNCTION (this << address << *header << packet << fullPacketSize);
  if (IsLowLatency () || address.IsGroup ())
    {
      return;
    }
  WifiRemoteStation *station = Lookup (address, header);
  WifiTxVector rts = DoGetRtsTxVector (station);
  WifiTxVector data = DoGetDataTxVector (station, fullPacketSize);
  WifiTxVector ctstoself = DoGetCtsToSelfTxVector ();
  HighLatencyDataTxVectorTag datatag;
  HighLatencyRtsTxVectorTag rtstag;
  HighLatencyCtsToSelfTxVectorTag ctstoselftag;
  //first, make sure that the tag is not here anymore.
  ConstCast<Packet> (packet)->RemovePacketTag (datatag);
  ConstCast<Packet> (packet)->RemovePacketTag (rtstag);
  ConstCast<Packet> (packet)->RemovePacketTag (ctstoselftag);
  datatag = HighLatencyDataTxVectorTag (data);
  rtstag = HighLatencyRtsTxVectorTag (rts);
  ctstoselftag = HighLatencyCtsToSelfTxVectorTag (ctstoself);
  //and then, add it back
  packet->AddPacketTag (datatag);
  packet->AddPacketTag (rtstag);
  packet->AddPacketTag (ctstoselftag);
}

WifiTxVector
WifiRemoteStationManager::GetDataTxVector (Mac48Address address, const WifiMacHeader *header,
                                           Ptr<const Packet> packet, uint32_t fullPacketSize)
{
  NS_LOG_FUNCTION (this << address << *header << packet << fullPacketSize);
  if (address.IsGroup ())
    {
      WifiTxVector v;
      v.SetMode (GetNonUnicastMode ());
      v.SetTxPowerLevel (m_defaultTxPowerLevel);
      v.SetChannelWidth (m_wifiPhy->GetChannelWidth ());
      v.SetShortGuardInterval (m_wifiPhy->GetGuardInterval ());
      v.SetNss (1);
      v.SetNess (0);
      v.SetStbc (false);
      return v;
    }
  if (!IsLowLatency ())
    {
      HighLatencyDataTxVectorTag datatag;
      bool found;
      found = ConstCast<Packet> (packet)->PeekPacketTag (datatag);
      NS_ASSERT (found);
      //cast found to void, to suppress 'found' set but not used
      //compiler warning
      (void) found;
      return datatag.GetDataTxVector ();
    }
  return DoGetDataTxVector (Lookup (address, header), fullPacketSize);
}

WifiTxVector
WifiRemoteStationManager::GetCtsToSelfTxVector (const WifiMacHeader *header,
                                                Ptr<const Packet> packet)
{
  NS_LOG_FUNCTION (this << *header << packet);
  if (!IsLowLatency ())
    {
      HighLatencyCtsToSelfTxVectorTag ctstoselftag;
      bool found;
      found = ConstCast<Packet> (packet)->PeekPacketTag (ctstoselftag);
      NS_ASSERT (found);
      //cast found to void, to suppress 'found' set but not used
      //compiler warning
      (void) found;
      return ctstoselftag.GetCtsToSelfTxVector ();
    }
  return DoGetCtsToSelfTxVector ();
}

WifiTxVector
WifiRemoteStationManager::DoGetCtsToSelfTxVector (void)
{
  return WifiTxVector (GetDefaultMode (),
                       GetDefaultTxPowerLevel (),
                       0,
                       m_wifiPhy->GetChannelWidth (),
                       m_wifiPhy->GetGuardInterval (),
                       GetNumberOfTransmitAntennas (),
                       GetNumberOfTransmitAntennas (),
                       false,
                       false);
}

WifiTxVector
WifiRemoteStationManager::GetRtsTxVector (Mac48Address address, const WifiMacHeader *header,
                                          Ptr<const Packet> packet)
{
  NS_LOG_FUNCTION (this << address << *header << packet);
  NS_ASSERT (!address.IsGroup ());
  if (!IsLowLatency ())
    {
      HighLatencyRtsTxVectorTag rtstag;
      bool found;
      found = ConstCast<Packet> (packet)->PeekPacketTag (rtstag);
      NS_ASSERT (found);
      //cast found to void, to suppress 'found' set but not used
      //compiler warning
      (void) found;
      return rtstag.GetRtsTxVector ();
    }
  return DoGetRtsTxVector (Lookup (address, header));
}

void
WifiRemoteStationManager::ReportRtsFailed (Mac48Address address, const WifiMacHeader *header)
{
  NS_LOG_FUNCTION (this << address << *header);
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStation *station = Lookup (address, header);
  station->m_ssrc++;
  m_macTxRtsFailed (address);
  DoReportRtsFailed (station);
}

void
WifiRemoteStationManager::ReportDataFailed (Mac48Address address, const WifiMacHeader *header)
{
  NS_LOG_FUNCTION (this << address << *header);
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStation *station = Lookup (address, header);
  station->m_slrc++;
  m_macTxDataFailed (address);
  DoReportDataFailed (station);
}

void
WifiRemoteStationManager::ReportRtsOk (Mac48Address address, const WifiMacHeader *header,
                                       double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  NS_LOG_FUNCTION (this << address << *header << ctsSnr << ctsMode << rtsSnr);
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStation *station = Lookup (address, header);
  station->m_state->m_info.NotifyTxSuccess (station->m_ssrc);
  station->m_ssrc = 0;
  DoReportRtsOk (station, ctsSnr, ctsMode, rtsSnr);
}

void
WifiRemoteStationManager::ReportDataOk (Mac48Address address, const WifiMacHeader *header,
                                        double ackSnr, WifiMode ackMode, double dataSnr)
{
  NS_LOG_FUNCTION (this << address << *header << ackSnr << ackMode << dataSnr);
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStation *station = Lookup (address, header);
  station->m_state->m_info.NotifyTxSuccess (station->m_slrc);
  station->m_slrc = 0;
  DoReportDataOk (station, ackSnr, ackMode, dataSnr);
}

void
WifiRemoteStationManager::ReportFinalRtsFailed (Mac48Address address, const WifiMacHeader *header)
{
  NS_LOG_FUNCTION (this << address << *header);
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStation *station = Lookup (address, header);
  station->m_state->m_info.NotifyTxFailed ();
  station->m_ssrc = 0;
  m_macTxFinalRtsFailed (address);
  DoReportFinalRtsFailed (station);
}

void
WifiRemoteStationManager::ReportFinalDataFailed (Mac48Address address, const WifiMacHeader *header)
{
  NS_LOG_FUNCTION (this << address << *header);
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStation *station = Lookup (address, header);
  station->m_state->m_info.NotifyTxFailed ();
  station->m_slrc = 0;
  m_macTxFinalDataFailed (address);
  DoReportFinalDataFailed (station);
}

void
WifiRemoteStationManager::ReportRxOk (Mac48Address address, const WifiMacHeader *header,
                                      double rxSnr, WifiMode txMode)
{
  NS_LOG_FUNCTION (this << address << *header << rxSnr << txMode);
  if (address.IsGroup ())
    {
      return;
    }
  WifiRemoteStation *station = Lookup (address, header);
  DoReportRxOk (station, rxSnr, txMode);
}

bool
WifiRemoteStationManager::NeedRts (Mac48Address address, const WifiMacHeader *header,
                                   Ptr<const Packet> packet, WifiTxVector txVector)
{
  WifiMode mode = txVector.GetMode ();
  NS_LOG_FUNCTION (this << address << *header << packet << mode);
  if (m_protectionMode == RTS_CTS
      && ((mode.GetModulationClass () == WIFI_MOD_CLASS_ERP_OFDM)
      || (mode.GetModulationClass () == WIFI_MOD_CLASS_HT)
      || (mode.GetModulationClass () == WIFI_MOD_CLASS_VHT))
      && m_useNonErpProtection)
    {
      NS_LOG_DEBUG ("WifiRemoteStationManager::NeedRTS returning true to protect non-ERP stations");
      return true;
    }
  if (address.IsGroup ())
    {
      return false;
    }
  bool normally = (packet->GetSize () + header->GetSize () + WIFI_MAC_FCS_LENGTH) > GetRtsCtsThreshold ();
  return DoNeedRts (Lookup (address, header), packet, normally);
}

bool
WifiRemoteStationManager::NeedCtsToSelf (WifiTxVector txVector)
{
  WifiMode mode = txVector.GetMode ();
  NS_LOG_FUNCTION (this << mode);
  if (m_protectionMode == CTS_TO_SELF
      && ((mode.GetModulationClass () == WIFI_MOD_CLASS_ERP_OFDM)
      || (mode.GetModulationClass () == WIFI_MOD_CLASS_HT)
      || (mode.GetModulationClass () == WIFI_MOD_CLASS_VHT))
      && m_useNonErpProtection)
    {
      NS_LOG_DEBUG ("WifiRemoteStationManager::NeedCtsToSelf returning true to protect non-ERP stations");
      return true;
    }
  else if (!m_useNonErpProtection)
    {
    //search for the BSS Basic Rate set, if the used mode is in the basic set then there is no need for Cts To Self
      for (WifiModeListIterator i = m_bssBasicRateSet.begin (); i != m_bssBasicRateSet.end (); i++)
        {
          if (mode == *i)
            {
              NS_LOG_DEBUG ("WifiRemoteStationManager::NeedCtsToSelf returning false");
              return false;
            }
        }
      if (HasHtSupported ())
        {
          //search for the BSS Basic MCS set, if the used mode is in the basic set then there is no need for Cts To Self
          for (WifiModeListIterator i = m_bssBasicMcsSet.begin (); i != m_bssBasicMcsSet.end (); i++)
            {
              if (mode == *i)
                {
                  NS_LOG_DEBUG ("WifiRemoteStationManager::NeedCtsToSelf returning false");
                  return false;
                }
            }
        }
      NS_LOG_DEBUG ("WifiRemoteStationManager::NeedCtsToSelf returning true");
      return true;
    }
  return false;
}

void
WifiRemoteStationManager::SetUseNonErpProtection (bool enable)
{
  m_useNonErpProtection = enable;
}

bool
WifiRemoteStationManager::GetUseNonErpProtection (void) const
{
  return m_useNonErpProtection;
}

bool
WifiRemoteStationManager::NeedRtsRetransmission (Mac48Address address, const WifiMacHeader *header,
                                                 Ptr<const Packet> packet)
{
  NS_LOG_FUNCTION (this << address << packet << *header);
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStation *station = Lookup (address, header);
  bool normally = station->m_ssrc < GetMaxSsrc ();
  NS_LOG_DEBUG ("WifiRemoteStationManager::NeedDataRetransmission count: " << station->m_ssrc << " result: " << std::boolalpha << normally);
  return DoNeedRtsRetransmission (station, packet, normally);
}

bool
WifiRemoteStationManager::NeedDataRetransmission (Mac48Address address, const WifiMacHeader *header,
                                                  Ptr<const Packet> packet)
{
  NS_LOG_FUNCTION (this << address << packet << *header);
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStation *station = Lookup (address, header);
  bool normally = station->m_slrc < GetMaxSlrc ();
  NS_LOG_DEBUG ("WifiRemoteStationManager::NeedDataRetransmission count: " << station->m_slrc << " result: " << std::boolalpha << normally);
  return DoNeedDataRetransmission (station, packet, normally);
}

bool
WifiRemoteStationManager::NeedFragmentation (Mac48Address address, const WifiMacHeader *header,
                                             Ptr<const Packet> packet)
{
  NS_LOG_FUNCTION (this << address << packet << *header);
  if (address.IsGroup ())
    {
      return false;
    }
  WifiRemoteStation *station = Lookup (address, header);
  bool normally = (packet->GetSize () + header->GetSize () + WIFI_MAC_FCS_LENGTH) > GetFragmentationThreshold ();
  NS_LOG_DEBUG ("WifiRemoteStationManager::NeedFragmentation result: " << std::boolalpha << normally);
  return DoNeedFragmentation (station, packet, normally);
}

void
WifiRemoteStationManager::DoSetFragmentationThreshold (uint32_t threshold)
{
  NS_LOG_FUNCTION (this << threshold);
  if (threshold < 256)
    {
      /*
       * ASN.1 encoding of the MAC and PHY MIB (256 ... 8000)
       */
      NS_LOG_WARN ("Fragmentation threshold should be larger than 256. Setting to 256.");
      m_nextFragmentationThreshold = 256;
    }
  else
    {
      /*
       * The length of each fragment shall be an even number of octets, except for the last fragment if an MSDU or
       * MMPDU, which may be either an even or an odd number of octets.
       */
      if (threshold % 2 != 0)
        {
          NS_LOG_WARN ("Fragmentation threshold should be an even number. Setting to " << threshold - 1);
          m_nextFragmentationThreshold = threshold - 1;
        }
      else
        {
          m_nextFragmentationThreshold = threshold;
        }
    }
}

void
WifiRemoteStationManager::UpdateFragmentationThreshold (void)
{
  m_fragmentationThreshold = m_nextFragmentationThreshold;
}

uint32_t
WifiRemoteStationManager::DoGetFragmentationThreshold (void) const
{
  return m_fragmentationThreshold;
}

uint32_t
WifiRemoteStationManager::GetNFragments (const WifiMacHeader *header, Ptr<const Packet> packet)
{
  NS_LOG_FUNCTION (this << *header << packet);
  //The number of bytes a fragment can support is (Threshold - WIFI_HEADER_SIZE - WIFI_FCS).
  uint32_t nFragments = (packet->GetSize () / (GetFragmentationThreshold () - header->GetSize () - WIFI_MAC_FCS_LENGTH));

  //If the size of the last fragment is not 0.
  if ((packet->GetSize () % (GetFragmentationThreshold () - header->GetSize () - WIFI_MAC_FCS_LENGTH)) > 0)
    {
      nFragments++;
    }
  NS_LOG_DEBUG ("WifiRemoteStationManager::GetNFragments returning " << nFragments);
  return nFragments;
}

uint32_t
WifiRemoteStationManager::GetFragmentSize (Mac48Address address, const WifiMacHeader *header,
                                           Ptr<const Packet> packet, uint32_t fragmentNumber)
{
  NS_LOG_FUNCTION (this << address << *header << packet << fragmentNumber);
  NS_ASSERT (!address.IsGroup ());
  uint32_t nFragment = GetNFragments (header, packet);
  if (fragmentNumber >= nFragment)
    {
      NS_LOG_DEBUG ("WifiRemoteStationManager::GetFragmentSize returning 0");
      return 0;
    }
  //Last fragment
  if (fragmentNumber == nFragment - 1)
    {
      uint32_t lastFragmentSize = packet->GetSize () - (fragmentNumber * (GetFragmentationThreshold () - header->GetSize () - WIFI_MAC_FCS_LENGTH));
      NS_LOG_DEBUG ("WifiRemoteStationManager::GetFragmentSize returning " << lastFragmentSize);
      return lastFragmentSize;
    }
  //All fragments but the last, the number of bytes is (Threshold - WIFI_HEADER_SIZE - WIFI_FCS).
  else
    {
      uint32_t fragmentSize = GetFragmentationThreshold () - header->GetSize () - WIFI_MAC_FCS_LENGTH;
      NS_LOG_DEBUG ("WifiRemoteStationManager::GetFragmentSize returning " << fragmentSize);
      return fragmentSize;
    }
}

uint32_t
WifiRemoteStationManager::GetFragmentOffset (Mac48Address address, const WifiMacHeader *header,
                                             Ptr<const Packet> packet, uint32_t fragmentNumber)
{
  NS_LOG_FUNCTION (this << address << *header << packet << fragmentNumber);
  NS_ASSERT (!address.IsGroup ());
  NS_ASSERT (fragmentNumber < GetNFragments (header, packet));
  uint32_t fragmentOffset = fragmentNumber * (GetFragmentationThreshold () - header->GetSize () - WIFI_MAC_FCS_LENGTH);
  NS_LOG_DEBUG ("WifiRemoteStationManager::GetFragmentOffset returning " << fragmentOffset);
  return fragmentOffset;
}

bool
WifiRemoteStationManager::IsLastFragment (Mac48Address address, const WifiMacHeader *header,
                                          Ptr<const Packet> packet, uint32_t fragmentNumber)
{
  NS_LOG_FUNCTION (this << address << *header << packet << fragmentNumber);
  NS_ASSERT (!address.IsGroup ());
  bool isLast = fragmentNumber == (GetNFragments (header, packet) - 1);
  NS_LOG_DEBUG ("WifiRemoteStationManager::IsLastFragment returning " << std::boolalpha << isLast);
  return isLast;
}

WifiMode
WifiRemoteStationManager::GetControlAnswerMode (Mac48Address address, WifiMode reqMode)
{
  /**
   * The standard has relatively unambiguous rules for selecting a
   * control response rate (the below is quoted from IEEE 802.11-2012,
   * Section 9.7):
   *
   * To allow the transmitting STA to calculate the contents of the
   * Duration/ID field, a STA responding to a received frame shall
   * transmit its Control Response frame (either CTS or ACK), other
   * than the BlockAck control frame, at the highest rate in the
   * BSSBasicRateSet parameter that is less than or equal to the
   * rate of the immediately previous frame in the frame exchange
   * sequence (as defined in Annex G) and that is of the same
   * modulation class (see Section 9.7.8) as the received frame...
   */
  NS_LOG_FUNCTION (this << address << reqMode);
  WifiMode mode = GetDefaultMode ();
  bool found = false;
  uint8_t nss = 1;  // Use one spatial stream for control response
  //First, search the BSS Basic Rate set
  for (WifiModeListIterator i = m_bssBasicRateSet.begin (); i != m_bssBasicRateSet.end (); i++)
    {
      if ((!found || i->GetPhyRate (m_wifiPhy->GetChannelWidth (), 0, nss) > mode.GetPhyRate (m_wifiPhy->GetChannelWidth (), 0, nss))
          && (i->GetPhyRate (m_wifiPhy->GetChannelWidth (), 0, nss) <= reqMode.GetPhyRate (m_wifiPhy->GetChannelWidth (), 0, nss))
          && (i->GetConstellationSize (nss) <= reqMode.GetConstellationSize (nss))
          && ((i->GetModulationClass () == reqMode.GetModulationClass ())
              || (reqMode.GetModulationClass () == WIFI_MOD_CLASS_HT)
              || (reqMode.GetModulationClass () == WIFI_MOD_CLASS_VHT)))

        {
          mode = *i;
          //We've found a potentially-suitable transmit rate, but we
          //need to continue and consider all the basic rates before
          //we can be sure we've got the right one.
          found = true;
        }
    }
  nss = 1;  // Continue the assumption that MIMO not used for control response
  if (HasHtSupported () || HasVhtSupported ())
    {
      if (!found)
        {
          mode = GetDefaultMcs ();
          for (WifiModeListIterator i = m_bssBasicMcsSet.begin (); i != m_bssBasicMcsSet.end (); i++)
            {
              if ((!found || i->GetPhyRate (m_wifiPhy->GetChannelWidth (), 0, nss) > mode.GetPhyRate (m_wifiPhy->GetChannelWidth (), 0, nss))
                  && i->GetPhyRate (m_wifiPhy->GetChannelWidth (), 0, nss) <= reqMode.GetPhyRate (m_wifiPhy->GetChannelWidth (), 0, nss))
              //&& thismode.GetModulationClass () == reqMode.GetModulationClass ()) //TODO: check standard
                {
                  mode = *i;
                  //We've found a potentially-suitable transmit rate, but we
                  //need to continue and consider all the basic rates before
                  //we can be sure we've got the right one.
                  found = true;
                }
            }
        }
    }
  //If we found a suitable rate in the BSSBasicRateSet, then we are
  //done and can return that mode.
  if (found)
    {
      NS_LOG_DEBUG ("WifiRemoteStationManager::GetControlAnswerMode returning " << mode);
      return mode;
    }

  /**
   * If no suitable basic rate was found, we search the mandatory
   * rates. The standard (IEEE 802.11-2007, Section 9.6) says:
   *
   *   ...If no rate contained in the BSSBasicRateSet parameter meets
   *   these conditions, then the control frame sent in response to a
   *   received frame shall be transmitted at the highest mandatory
   *   rate of the PHY that is less than or equal to the rate of the
   *   received frame, and that is of the same modulation class as the
   *   received frame. In addition, the Control Response frame shall
   *   be sent using the same PHY options as the received frame,
   *   unless they conflict with the requirement to use the
   *   BSSBasicRateSet parameter.
   *
   * \todo Note that we're ignoring the last sentence for now, because
   * there is not yet any manipulation here of PHY options.
   */
  nss = 1;  // Continue the assumption that MIMO not used for control response
  for (uint32_t idx = 0; idx < m_wifiPhy->GetNModes (); idx++)
    {
      WifiMode thismode = m_wifiPhy->GetMode (idx);

      /* If the rate:
       *
       *  - is a mandatory rate for the PHY, and
       *  - is equal to or faster than our current best choice, and
       *  - is less than or equal to the rate of the received frame, and
       *  - is of the same modulation class as the received frame
       *
       * ...then it's our best choice so far.
       */
      if (thismode.IsMandatory ()
          && (!found || thismode.GetPhyRate (m_wifiPhy->GetChannelWidth (), 0, nss) > mode.GetPhyRate (m_wifiPhy->GetChannelWidth (), 0, nss))
          && (thismode.GetPhyRate (m_wifiPhy->GetChannelWidth (), 0, nss) <= reqMode.GetPhyRate (m_wifiPhy->GetChannelWidth (), 0, nss))
          && (thismode.GetConstellationSize (nss) <= reqMode.GetConstellationSize (nss))
          && ((thismode.GetModulationClass () == reqMode.GetModulationClass ())
              || (reqMode.GetModulationClass () == WIFI_MOD_CLASS_HT)
              || (reqMode.GetModulationClass () == WIFI_MOD_CLASS_HT)))

        {
          mode = thismode;
          //As above; we've found a potentially-suitable transmit
          //rate, but we need to continue and consider all the
          //mandatory rates before we can be sure we've got the right one.
          found = true;
        }
    }
  nss = 1;  // Continue the assumption that MIMO not used for control response
  if (HasHtSupported () || HasVhtSupported ())
    {
      for (uint32_t idx = 0; idx < m_wifiPhy->GetNMcs (); idx++)
        {
          WifiMode thismode = m_wifiPhy->GetMcs (idx);
          if (thismode.IsMandatory ()
              && (!found || thismode.GetPhyRate (m_wifiPhy->GetChannelWidth (), 0, nss) > mode.GetPhyRate (m_wifiPhy->GetChannelWidth (), 0, nss))
              && thismode.GetPhyRate (m_wifiPhy->GetChannelWidth (), 0, nss) <= reqMode.GetPhyRate (m_wifiPhy->GetChannelWidth (), 0, nss))
              //&& thismode.GetModulationClass () == reqMode.GetModulationClass ()) //TODO: check standard
            {
              mode = thismode;
              //As above; we've found a potentially-suitable transmit
              //rate, but we need to continue and consider all the
              //mandatory rates before we can be sure we've got the right one.
              found = true;
            }

        }
    }

  /**
   * If we still haven't found a suitable rate for the response then
   * someone has messed up the simulation config. This probably means
   * that the WifiPhyStandard is not set correctly, or that a rate that
   * is not supported by the PHY has been explicitly requested in a
   * WifiRemoteStationManager (or descendant) configuration.
   *
   * Either way, it is serious - we can either disobey the standard or
   * fail, and I have chosen to do the latter...
   */
  if (!found)
    {
      NS_FATAL_ERROR ("Can't find response rate for " << reqMode);
    }

  NS_LOG_DEBUG ("WifiRemoteStationManager::GetControlAnswerMode returning " << mode);
  return mode;
}

WifiTxVector
WifiRemoteStationManager::GetCtsTxVector (Mac48Address address, WifiMode rtsMode)
{
  NS_ASSERT (!address.IsGroup ());
  WifiTxVector v;
  v.SetMode (GetControlAnswerMode (address, rtsMode));
  v.SetTxPowerLevel (DoGetCtsTxPowerLevel (address, v.GetMode ()));
  v.SetChannelWidth (DoGetCtsTxChannelWidth (address, v.GetMode ()));
  v.SetShortGuardInterval (DoGetCtsTxGuardInterval (address, v.GetMode ()));
  v.SetNss (DoGetCtsTxNss (address, v.GetMode ()));
  v.SetNess (DoGetCtsTxNess (address, v.GetMode ()));
  v.SetStbc (DoGetCtsTxStbc (address, v.GetMode ()));
  return v;
}

WifiTxVector
WifiRemoteStationManager::GetAckTxVector (Mac48Address address, WifiMode dataMode)
{
  NS_ASSERT (!address.IsGroup ());
  WifiTxVector v;
  v.SetMode (GetControlAnswerMode (address, dataMode));
  v.SetTxPowerLevel (DoGetAckTxPowerLevel (address, v.GetMode ()));
  v.SetChannelWidth (DoGetAckTxChannelWidth (address, v.GetMode ()));
  v.SetShortGuardInterval (DoGetAckTxGuardInterval (address, v.GetMode ()));
  v.SetNss (DoGetAckTxNss (address, v.GetMode ()));
  v.SetNess (DoGetAckTxNess (address, v.GetMode ()));
  v.SetStbc (DoGetAckTxStbc (address, v.GetMode ()));
  return v;
}

WifiTxVector
WifiRemoteStationManager::GetBlockAckTxVector (Mac48Address address, WifiMode blockAckReqMode)
{
  NS_ASSERT (!address.IsGroup ());
  WifiTxVector v;
  v.SetMode (GetControlAnswerMode (address, blockAckReqMode));
  v.SetTxPowerLevel (DoGetBlockAckTxPowerLevel (address, v.GetMode ()));
  v.SetChannelWidth (DoGetBlockAckTxChannelWidth (address, v.GetMode ()));
  v.SetShortGuardInterval (DoGetBlockAckTxGuardInterval (address, v.GetMode ()));
  v.SetNss (DoGetBlockAckTxNss (address, v.GetMode ()));
  v.SetNess (DoGetBlockAckTxNess (address, v.GetMode ()));
  v.SetStbc (DoGetBlockAckTxStbc (address, v.GetMode ()));
  return v;
}

uint8_t
WifiRemoteStationManager::DoGetCtsTxPowerLevel (Mac48Address address, WifiMode ctsMode)
{
  return m_defaultTxPowerLevel;
}

uint32_t
WifiRemoteStationManager::DoGetCtsTxChannelWidth (Mac48Address address, WifiMode ctsMode)
{
  return m_wifiPhy->GetChannelWidth ();
}

bool
WifiRemoteStationManager::DoGetCtsTxGuardInterval (Mac48Address address, WifiMode ctsMode)
{
  return m_wifiPhy->GetGuardInterval ();
}

uint8_t
WifiRemoteStationManager::DoGetCtsTxNss (Mac48Address address, WifiMode ctsMode)
{
  return 1;
}

uint8_t
WifiRemoteStationManager::DoGetCtsTxNess (Mac48Address address, WifiMode ctsMode)
{
  return 0;
}

bool
WifiRemoteStationManager::DoGetCtsTxStbc (Mac48Address address, WifiMode ctsMode)
{
  return m_wifiPhy->GetStbc ();
}

uint8_t
WifiRemoteStationManager::DoGetAckTxPowerLevel (Mac48Address address, WifiMode ackMode)
{
  return m_defaultTxPowerLevel;
}

uint32_t
WifiRemoteStationManager::DoGetAckTxChannelWidth (Mac48Address address, WifiMode ctsMode)
{
  return m_wifiPhy->GetChannelWidth ();
}

bool
WifiRemoteStationManager::DoGetAckTxGuardInterval (Mac48Address address, WifiMode ackMode)
{
  return m_wifiPhy->GetGuardInterval ();
}

uint8_t
WifiRemoteStationManager::DoGetAckTxNss (Mac48Address address, WifiMode ackMode)
{
  return 1;
}

uint8_t
WifiRemoteStationManager::DoGetAckTxNess (Mac48Address address, WifiMode ackMode)
{
  return 0;
}

bool
WifiRemoteStationManager::DoGetAckTxStbc (Mac48Address address, WifiMode ackMode)
{
  return m_wifiPhy->GetStbc ();
}

uint8_t
WifiRemoteStationManager::DoGetBlockAckTxPowerLevel (Mac48Address address, WifiMode blockAckMode)
{
  return m_defaultTxPowerLevel;
}

uint32_t
WifiRemoteStationManager::DoGetBlockAckTxChannelWidth (Mac48Address address, WifiMode ctsMode)
{
  return m_wifiPhy->GetChannelWidth ();
}

bool
WifiRemoteStationManager::DoGetBlockAckTxGuardInterval (Mac48Address address, WifiMode blockAckMode)
{
  return m_wifiPhy->GetGuardInterval ();
}

uint8_t
WifiRemoteStationManager::DoGetBlockAckTxNss (Mac48Address address, WifiMode blockAckMode)
{
  return 1;
}

uint8_t
WifiRemoteStationManager::DoGetBlockAckTxNess (Mac48Address address, WifiMode blockAckMode)
{
  return 0;
}

bool
WifiRemoteStationManager::DoGetBlockAckTxStbc (Mac48Address address, WifiMode blockAckMode)
{
  return m_wifiPhy->GetStbc ();
}

uint8_t
WifiRemoteStationManager::GetDefaultTxPowerLevel (void) const
{
  return m_defaultTxPowerLevel;
}

WifiRemoteStationInfo
WifiRemoteStationManager::GetInfo (Mac48Address address)
{
  WifiRemoteStationState *state = LookupState (address);
  return state->m_info;
}

WifiRemoteStationState *
WifiRemoteStationManager::LookupState (Mac48Address address) const
{
  NS_LOG_FUNCTION (this << address);
  for (StationStates::const_iterator i = m_states.begin (); i != m_states.end (); i++)
    {
      if ((*i)->m_address == address)
        {
          NS_LOG_DEBUG ("WifiRemoteStationManager::LookupState returning existing state");
          return (*i);
        }
    }
  WifiRemoteStationState *state = new WifiRemoteStationState ();
  state->m_state = WifiRemoteStationState::BRAND_NEW;
  state->m_address = address;
  state->m_operationalRateSet.push_back (GetDefaultMode ());
  state->m_operationalMcsSet.push_back (GetDefaultMcs ());
  state->m_channelWidth = m_wifiPhy->GetChannelWidth ();
  state->m_shortGuardInterval = m_wifiPhy->GetGuardInterval ();
  state->m_greenfield = m_wifiPhy->GetGreenfield ();
  state->m_ness = 0;
  state->m_aggregation = false;
  state->m_stbc = false;
  const_cast<WifiRemoteStationManager *> (this)->m_states.push_back (state);
  NS_LOG_DEBUG ("WifiRemoteStationManager::LookupState returning new state");
  return state;
}

WifiRemoteStation *
WifiRemoteStationManager::Lookup (Mac48Address address, const WifiMacHeader *header) const
{
  uint8_t tid;
  if (header->IsQosData ())
    {
      tid = header->GetQosTid ();
    }
  else
    {
      tid = 0;
    }
  return Lookup (address, tid);
}

WifiRemoteStation *
WifiRemoteStationManager::Lookup (Mac48Address address, uint8_t tid) const
{
  NS_LOG_FUNCTION (this << address << (uint16_t)tid);
  for (Stations::const_iterator i = m_stations.begin (); i != m_stations.end (); i++)
    {
      if ((*i)->m_tid == tid
          && (*i)->m_state->m_address == address)
        {
          return (*i);
        }
    }
  WifiRemoteStationState *state = LookupState (address);

  WifiRemoteStation *station = DoCreateStation ();
  station->m_state = state;
  station->m_tid = tid;
  station->m_ssrc = 0;
  station->m_slrc = 0;
  const_cast<WifiRemoteStationManager *> (this)->m_stations.push_back (station);
  return station;
}

void
WifiRemoteStationManager::AddStationHtCapabilities (Mac48Address from, HtCapabilities htCapabilities)
{
  //Used by all stations to record HT capabilities of remote stations
  NS_LOG_FUNCTION (this << from << htCapabilities);
  WifiRemoteStationState *state;
  state = LookupState (from);
  state->m_shortGuardInterval = htCapabilities.GetShortGuardInterval20 ();
  if (htCapabilities.GetSupportedChannelWidth () == 1)
    {
      state->m_channelWidth = 40;
    }
  else
    {
      state->m_channelWidth = 20;
    }
  state->m_greenfield = htCapabilities.GetGreenfield ();
}

void
WifiRemoteStationManager::AddStationVhtCapabilities (Mac48Address from, VhtCapabilities vhtCapabilities)
{
  //Used by all stations to record VHT capabilities of remote stations
  NS_LOG_FUNCTION (this << from << vhtCapabilities);
  WifiRemoteStationState *state;
  state = LookupState (from);
  if (vhtCapabilities.GetSupportedChannelWidthSet () == 1)
    {
      state->m_channelWidth = 160;
    }
  else
    {
      state->m_channelWidth = 80;
    }
  //This is a workaround to enable users to force a 20 or 40 MHz channel for a VHT-compliant device,
  //since IEEE 802.11ac standard says that 20, 40 and 80 MHz channels are mandatory.
  if (m_wifiPhy->GetChannelWidth () < state->m_channelWidth)
    {
      state->m_channelWidth = m_wifiPhy->GetChannelWidth ();
    }
}

bool
WifiRemoteStationManager::GetGreenfieldSupported (Mac48Address address) const
{
  return LookupState (address)->m_greenfield;
}

WifiMode
WifiRemoteStationManager::GetDefaultMode (void) const
{
  return m_defaultTxMode;
}

WifiMode
WifiRemoteStationManager::GetDefaultMcs (void) const
{
  return m_defaultTxMcs;
}

void
WifiRemoteStationManager::Reset (void)
{
  NS_LOG_FUNCTION (this);
  for (Stations::const_iterator i = m_stations.begin (); i != m_stations.end (); i++)
    {
      delete (*i);
    }
  m_stations.clear ();
  m_bssBasicRateSet.clear ();
  m_bssBasicRateSet.push_back (m_defaultTxMode);
  m_bssBasicMcsSet.clear ();
  m_bssBasicMcsSet.push_back (m_defaultTxMcs);
  NS_ASSERT (m_defaultTxMode.IsMandatory ());
}

void
WifiRemoteStationManager::AddBasicMode (WifiMode mode)
{
  NS_LOG_FUNCTION (this << mode);
  if (mode.GetModulationClass () == WIFI_MOD_CLASS_HT || mode.GetModulationClass () == WIFI_MOD_CLASS_VHT)
    {
      NS_FATAL_ERROR ("It is not allowed to add a (V)HT rate in the BSSBasicRateSet!");
    }
  for (uint32_t i = 0; i < GetNBasicModes (); i++)
    {
      if (GetBasicMode (i) == mode)
        {
          return;
        }
    }
  m_bssBasicRateSet.push_back (mode);
}

uint32_t
WifiRemoteStationManager::GetNBasicModes (void) const
{
  return m_bssBasicRateSet.size ();
}

WifiMode
WifiRemoteStationManager::GetBasicMode (uint32_t i) const
{
  NS_ASSERT (i < GetNBasicModes ());
  return m_bssBasicRateSet[i];
}

uint32_t
WifiRemoteStationManager::GetNNonErpBasicModes (void) const
{
  uint32_t size = 0;
  for (WifiModeListIterator i = m_bssBasicRateSet.begin (); i != m_bssBasicRateSet.end (); i++)
  {
    if (i->GetModulationClass () == WIFI_MOD_CLASS_ERP_OFDM)
      {
        continue;
      }
    size++;
  }
  return size;
}

WifiMode
WifiRemoteStationManager::GetNonErpBasicMode (uint32_t i) const
{
  NS_ASSERT (i < GetNNonErpBasicModes ());
  uint32_t index = 0;
  bool found = false;
  for (WifiModeListIterator j = m_bssBasicRateSet.begin (); j != m_bssBasicRateSet.end (); )
  {
    if (i == index)
      {
        found = true;
      }
    if (j->GetModulationClass () != WIFI_MOD_CLASS_ERP_OFDM)
      {
        if (found)
          {
            break;
          }
      }
    index++;
    j++;
  }
  return m_bssBasicRateSet[index];
}

void
WifiRemoteStationManager::AddBasicMcs (WifiMode mcs)
{
  NS_LOG_FUNCTION (this << (uint32_t)mcs.GetMcsValue ());
  for (uint32_t i = 0; i < GetNBasicMcs (); i++)
    {
      if (GetBasicMcs (i) == mcs)
        {
          return;
        }
    }
  m_bssBasicMcsSet.push_back (mcs);
}

uint32_t
WifiRemoteStationManager::GetNBasicMcs (void) const
{
  return m_bssBasicMcsSet.size ();
}

WifiMode
WifiRemoteStationManager::GetBasicMcs (uint32_t i) const
{
  NS_ASSERT (i < GetNBasicMcs ());
  return m_bssBasicMcsSet[i];
}

WifiMode
WifiRemoteStationManager::GetNonUnicastMode (void) const
{
  if (m_nonUnicastMode == WifiMode ())
    {
      return GetBasicMode (0);
    }
  else
    {
      return m_nonUnicastMode;
    }
}

bool
WifiRemoteStationManager::DoNeedRts (WifiRemoteStation *station,
                                     Ptr<const Packet> packet, bool normally)
{
  return normally;
}

bool
WifiRemoteStationManager::DoNeedRtsRetransmission (WifiRemoteStation *station,
                                                   Ptr<const Packet> packet, bool normally)
{
  return normally;
}

bool
WifiRemoteStationManager::DoNeedDataRetransmission (WifiRemoteStation *station,
                                                    Ptr<const Packet> packet, bool normally)
{
  return normally;
}

bool
WifiRemoteStationManager::DoNeedFragmentation (WifiRemoteStation *station,
                                               Ptr<const Packet> packet, bool normally)
{
  return normally;
}

WifiMode
WifiRemoteStationManager::GetSupported (const WifiRemoteStation *station, uint32_t i) const
{
  NS_ASSERT (i < GetNSupported (station));
  return station->m_state->m_operationalRateSet[i];
}

WifiMode
WifiRemoteStationManager::GetMcsSupported (const WifiRemoteStation *station, uint32_t i) const
{
  NS_ASSERT (i < GetNMcsSupported (station));
  return station->m_state->m_operationalMcsSet[i];
}

WifiMode
WifiRemoteStationManager::GetNonErpSupported (const WifiRemoteStation *station, uint32_t i) const
{
  NS_ASSERT (i < GetNNonErpSupported (station));
  //IEEE 802.11g standard defines that if the protection mechanism is enabled, Rts, Cts and Cts-To-Self
  //frames should select a rate in the BSSBasicRateSet that corresponds to an 802.11b basic rate.
  //This is a implemented here to avoid changes in every RAA, but should maybe be moved in case it breaks standard rules.
  uint32_t index = 0;
  bool found = false;
  for (WifiModeListIterator j = station->m_state->m_operationalRateSet.begin (); j != station->m_state->m_operationalRateSet.end (); )
  {
    if (i == index)
      {
        found = true;
      }
    if (j->GetModulationClass () != WIFI_MOD_CLASS_ERP_OFDM)
      {
        if (found)
          {
            break;
          }
      }
    index++;
    j++;
  }
  return station->m_state->m_operationalRateSet[index];
}

uint32_t
WifiRemoteStationManager::GetChannelWidth (const WifiRemoteStation *station) const
{
  return station->m_state->m_channelWidth;
}

bool
WifiRemoteStationManager::GetShortGuardInterval (const WifiRemoteStation *station) const
{
  return station->m_state->m_shortGuardInterval;
}

bool
WifiRemoteStationManager::GetGreenfield (const WifiRemoteStation *station) const
{
  return station->m_state->m_greenfield;
}

bool
WifiRemoteStationManager::GetAggregation (const WifiRemoteStation *station) const
{
  return station->m_state->m_aggregation;
}

bool
WifiRemoteStationManager::GetStbc (const WifiRemoteStation *station) const
{
  return station->m_state->m_stbc;
}

uint32_t
WifiRemoteStationManager::GetNess (const WifiRemoteStation *station) const
{
  return station->m_state->m_ness;
}

uint32_t
WifiRemoteStationManager::GetShortRetryCount (const WifiRemoteStation *station) const
{
  return station->m_ssrc;
}

Ptr<WifiPhy>
WifiRemoteStationManager::GetPhy (void) const
{
  return m_wifiPhy;
}

Ptr<WifiMac>
WifiRemoteStationManager::GetMac (void) const
{
  return m_wifiMac;
}

uint32_t
WifiRemoteStationManager::GetLongRetryCount (const WifiRemoteStation *station) const
{
  return station->m_slrc;
}

uint32_t
WifiRemoteStationManager::GetNSupported (const WifiRemoteStation *station) const
{
  return station->m_state->m_operationalRateSet.size ();
}

uint32_t
WifiRemoteStationManager::GetNMcsSupported (const WifiRemoteStation *station) const
{
  return station->m_state->m_operationalMcsSet.size ();
}

uint32_t
WifiRemoteStationManager::GetNNonErpSupported (const WifiRemoteStation *station) const
{
  uint32_t size = 0;
  for (WifiModeListIterator i = station->m_state->m_operationalRateSet.begin (); i != station->m_state->m_operationalRateSet.end (); i++)
  {
    if (i->GetModulationClass () == WIFI_MOD_CLASS_ERP_OFDM)
      {
        continue;
      }
    size++;
  }
  return size;
}

void
WifiRemoteStationManager::SetDefaultTxPowerLevel (uint8_t txPower)
{
  m_defaultTxPowerLevel = txPower;
}

uint32_t
WifiRemoteStationManager::GetNumberOfTransmitAntennas (void)
{
  return m_wifiPhy->GetNumberOfTransmitAntennas ();
}

WifiRemoteStationInfo::WifiRemoteStationInfo ()
  : m_memoryTime (Seconds (1.0)),
    m_lastUpdate (Seconds (0.0)),
    m_failAvg (0.0)
{
}

double
WifiRemoteStationInfo::CalculateAveragingCoefficient ()
{
  double retval = std::exp ((double)(m_lastUpdate.GetMicroSeconds () - Simulator::Now ().GetMicroSeconds ())
                            / (double)m_memoryTime.GetMicroSeconds ());
  m_lastUpdate = Simulator::Now ();
  return retval;
}

void
WifiRemoteStationInfo::NotifyTxSuccess (uint32_t retryCounter)
{
  double coefficient = CalculateAveragingCoefficient ();
  m_failAvg = (double)retryCounter / (1 + (double)retryCounter) * (1.0 - coefficient) + coefficient * m_failAvg;
}

void
WifiRemoteStationInfo::NotifyTxFailed ()
{
  double coefficient = CalculateAveragingCoefficient ();
  m_failAvg = (1.0 - coefficient) + coefficient * m_failAvg;
}

double
WifiRemoteStationInfo::GetFrameErrorRate () const
{
  return m_failAvg;
}

WifiRemoteStation::~WifiRemoteStation ()
{
  NS_LOG_FUNCTION (this);
}

} //namespace ns3
