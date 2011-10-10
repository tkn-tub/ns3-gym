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

#include "wifi-remote-station-manager.h"
#include "ns3/simulator.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/tag.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/wifi-phy.h"
#include "ns3/trace-source-accessor.h"
#include "wifi-mac-header.h"
#include "wifi-mac-trailer.h"

NS_LOG_COMPONENT_DEFINE ("WifiRemoteStationManager");


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
  virtual void Print (std::ostream &os) const;
private:
  WifiMode m_rtsMode;
  WifiMode m_dataMode;
};

TxModeTag::TxModeTag ()
{
}
TxModeTag::TxModeTag (WifiMode rtsMode, WifiMode dataMode)
  : m_rtsMode (rtsMode),
    m_dataMode (dataMode)
{
}
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
void
TxModeTag::Print (std::ostream &os) const
{
  os << "Rts=" << m_rtsMode << ", Data=" << m_dataMode;
}

} // namespace ns3


namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (WifiRemoteStationManager);

TypeId
WifiRemoteStationManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WifiRemoteStationManager")
    .SetParent<Object> ()
    .AddAttribute ("IsLowLatency", "If true, we attempt to modelize a so-called low-latency device: a device"
                   " where decisions about tx parameters can be made on a per-packet basis and feedback about the"
                   " transmission of each packet is obtained before sending the next. Otherwise, we modelize a "
                   " high-latency device, that is a device where we cannot update our decision about tx parameters"
                   " after every packet transmission.",
                   BooleanValue (true), // this value is ignored because there is no setter
                   MakeBooleanAccessor (&WifiRemoteStationManager::IsLowLatency),
                   MakeBooleanChecker ())
    .AddAttribute ("MaxSsrc", "The maximum number of retransmission attempts for an RTS. This value"
                   " will not have any effect on some rate control algorithms.",
                   UintegerValue (7),
                   MakeUintegerAccessor (&WifiRemoteStationManager::m_maxSsrc),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MaxSlrc", "The maximum number of retransmission attempts for a DATA packet. This value"
                   " will not have any effect on some rate control algorithms.",
                   UintegerValue (7),
                   MakeUintegerAccessor (&WifiRemoteStationManager::m_maxSlrc),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("RtsCtsThreshold", "If  the size of the data packet + LLC header + MAC header + FCS trailer is bigger than "
                   "this value, we use an RTS/CTS handshake before sending the data, as per IEEE Std. 802.11-2007, Section 9.2.6. "
                   "This value will not have any effect on some rate control algorithms.",
                   UintegerValue (2346),
                   MakeUintegerAccessor (&WifiRemoteStationManager::m_rtsCtsThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("FragmentationThreshold", "If the size of the data packet + LLC header + MAC header + FCS trailer is bigger"
                   "than this value, we fragment it such that the size of the fragments are equal or smaller "
                   "than this value, as per IEEE Std. 802.11-2007, Section 9.4. "
                   "This value will not have any effect on some rate control algorithms.",
                   UintegerValue (2346),
                   MakeUintegerAccessor (&WifiRemoteStationManager::m_fragmentationThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("NonUnicastMode", "Wifi mode used for non-unicast transmissions.",
                   WifiModeValue (),
                   MakeWifiModeAccessor (&WifiRemoteStationManager::m_nonUnicastMode),
                   MakeWifiModeChecker ())
    .AddTraceSource ("MacTxRtsFailed",
                     "The transmission of a RTS by the MAC layer has failed",
                     MakeTraceSourceAccessor (&WifiRemoteStationManager::m_macTxRtsFailed))
    .AddTraceSource ("MacTxDataFailed",
                     "The transmission of a data packet by the MAC layer has failed",
                     MakeTraceSourceAccessor (&WifiRemoteStationManager::m_macTxDataFailed))
    .AddTraceSource ("MacTxFinalRtsFailed",
                     "The transmission of a RTS has exceeded the maximum number of attempts",
                     MakeTraceSourceAccessor (&WifiRemoteStationManager::m_macTxFinalRtsFailed))
    .AddTraceSource ("MacTxFinalDataFailed",
                     "The transmission of a data packet has exceeded the maximum number of attempts",
                     MakeTraceSourceAccessor (&WifiRemoteStationManager::m_macTxFinalDataFailed))
  ;
  return tid;
}

WifiRemoteStationManager::WifiRemoteStationManager ()
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
  // We need to track our PHY because it is the object that knows the
  // full set of transmit rates that are supported. We need to know
  // this in order to find the relevant mandatory rates when chosing a
  // transmit rate for automatic control responses like
  // acknowledgements.
  m_wifiPhy = phy;
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

void
WifiRemoteStationManager::Reset (Mac48Address address)
{
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStationState *state = LookupState (address);
  state->m_operationalRateSet.clear ();
  AddSupportedMode (address, GetDefaultMode ());
}
void
WifiRemoteStationManager::AddSupportedMode (Mac48Address address, WifiMode mode)
{
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStationState *state = LookupState (address);
  for (WifiModeListIterator i = state->m_operationalRateSet.begin (); i != state->m_operationalRateSet.end (); i++)
    {
      if ((*i) == mode)
        {
          // already in.
          return;
        }
    }
  state->m_operationalRateSet.push_back (mode);
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
  if (IsLowLatency () || address.IsGroup ())
    {
      return;
    }
  WifiRemoteStation *station = Lookup (address, header);
  WifiMode rts = DoGetRtsMode (station);
  WifiMode data = DoGetDataMode (station, fullPacketSize);
  TxModeTag tag;
  // first, make sure that the tag is not here anymore.
  ConstCast<Packet> (packet)->RemovePacketTag (tag);
  tag = TxModeTag (rts, data);
  // and then, add it back
  packet->AddPacketTag (tag);
}
WifiMode
WifiRemoteStationManager::GetDataMode (Mac48Address address, const WifiMacHeader *header,
                                       Ptr<const Packet> packet, uint32_t fullPacketSize)
{
  if (address.IsGroup ())
    {
      return GetNonUnicastMode ();
    }
  if (!IsLowLatency ())
    {
      TxModeTag tag;
      bool found;
      found = ConstCast<Packet> (packet)->PeekPacketTag (tag);
      NS_ASSERT (found);
      // cast found to void, to suppress 'found' set but not used
      // compiler warning
      (void) found;
      return tag.GetDataMode ();
    }
  return DoGetDataMode (Lookup (address, header), fullPacketSize);
}
WifiMode
WifiRemoteStationManager::GetRtsMode (Mac48Address address, const WifiMacHeader *header,
                                      Ptr<const Packet> packet)
{
  NS_ASSERT (!address.IsGroup ());
  if (!IsLowLatency ())
    {
      TxModeTag tag;
      bool found;
      found = ConstCast<Packet> (packet)->PeekPacketTag (tag);
      NS_ASSERT (found);
      // cast found to void, to suppress 'found' set but not used
      // compiler warning
      (void) found;
      return tag.GetRtsMode ();
    }
  return DoGetRtsMode (Lookup (address, header));
}
void
WifiRemoteStationManager::ReportRtsFailed (Mac48Address address, const WifiMacHeader *header)
{
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStation *station = Lookup (address, header);
  station->m_ssrc++;
  m_macTxRtsFailed (address);
  DoReportRtsFailed (station);
}
void
WifiRemoteStationManager::ReportDataFailed (Mac48Address address, const WifiMacHeader *header)
{
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
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStation *station = Lookup (address, header);
  station->m_state->m_info.NotifyTxSuccess (station->m_slrc);
  station->m_slrc = 0;
  DoReportDataOk (station, ackSnr, ackMode, dataSnr);
}
void
WifiRemoteStationManager::ReportFinalRtsFailed (Mac48Address address, const WifiMacHeader *header)
{
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
  if (address.IsGroup ())
    {
      return;
    }
  WifiRemoteStation *station = Lookup (address, header);
  DoReportRxOk (station, rxSnr, txMode);
}
bool
WifiRemoteStationManager::NeedRts (Mac48Address address, const WifiMacHeader *header,
                                   Ptr<const Packet> packet)
{
  if (address.IsGroup ())
    {
      return false;
    }
  bool normally = (packet->GetSize () + header->GetSize () + WIFI_MAC_FCS_LENGTH) > GetRtsCtsThreshold ();
  return DoNeedRts (Lookup (address, header), packet, normally);
}
bool
WifiRemoteStationManager::NeedRtsRetransmission (Mac48Address address, const WifiMacHeader *header,
                                                 Ptr<const Packet> packet)
{
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStation *station = Lookup (address, header);
  bool normally = station->m_ssrc < GetMaxSsrc ();
  return DoNeedRtsRetransmission (station, packet, normally);
}
bool
WifiRemoteStationManager::NeedDataRetransmission (Mac48Address address, const WifiMacHeader *header,
                                                  Ptr<const Packet> packet)
{
  NS_ASSERT (!address.IsGroup ());
  WifiRemoteStation *station = Lookup (address, header);
  bool normally = station->m_slrc < GetMaxSlrc ();
  return DoNeedDataRetransmission (station, packet, normally);
}
bool
WifiRemoteStationManager::NeedFragmentation (Mac48Address address, const WifiMacHeader *header,
                                             Ptr<const Packet> packet)
{
  if (address.IsGroup ())
    {
      return false;
    }
  WifiRemoteStation *station = Lookup (address, header);
  bool normally = (packet->GetSize () + header->GetSize () + WIFI_MAC_FCS_LENGTH) > GetFragmentationThreshold ();
  return DoNeedFragmentation (station, packet, normally);
}
uint32_t
WifiRemoteStationManager::GetNFragments (Ptr<const Packet> packet)
{
  uint32_t nFragments = packet->GetSize () / GetFragmentationThreshold () + 1;
  return nFragments;
}

uint32_t
WifiRemoteStationManager::GetFragmentSize (Mac48Address address, const WifiMacHeader *header,
                                           Ptr<const Packet> packet, uint32_t fragmentNumber)
{
  NS_ASSERT (!address.IsGroup ());
  uint32_t nFragment = GetNFragments (packet);
  if (fragmentNumber >= nFragment)
    {
      return 0;
    }
  if (fragmentNumber == nFragment - 1)
    {
      uint32_t lastFragmentSize = packet->GetSize () % GetFragmentationThreshold ();
      return lastFragmentSize;
    }
  else
    {
      return GetFragmentationThreshold ();
    }
}
uint32_t
WifiRemoteStationManager::GetFragmentOffset (Mac48Address address, const WifiMacHeader *header,
                                             Ptr<const Packet> packet, uint32_t fragmentNumber)
{
  NS_ASSERT (!address.IsGroup ());
  NS_ASSERT (fragmentNumber < GetNFragments (packet));
  uint32_t fragmentOffset = fragmentNumber * GetFragmentationThreshold ();
  return fragmentOffset;
}
bool
WifiRemoteStationManager::IsLastFragment (Mac48Address address, const WifiMacHeader *header,
                                          Ptr<const Packet> packet, uint32_t fragmentNumber)
{
  NS_ASSERT (!address.IsGroup ());
  bool isLast = fragmentNumber == (GetNFragments (packet) - 1);
  return isLast;
}
WifiMode
WifiRemoteStationManager::GetControlAnswerMode (Mac48Address address, WifiMode reqMode)
{
  /**
   * The standard has relatively unambiguous rules for selecting a
   * control response rate (the below is quoted from IEEE 802.11-2007,
   * Section 9.6):
   *
   *   To allow the transmitting STA to calculate the contents of the
   *   Duration/ID field, a STA responding to a received frame shall
   *   transmit its Control Response frame (either CTS or ACK), other
   *   than the BlockAck control frame, at the highest rate in the
   *   BSSBasicRateSet parameter that is less than or equal to the
   *   rate of the immediately previous frame in the frame exchange
   *   sequence (as defined in 9.12) and that is of the same
   *   modulation class (see 9.6.1) as the received frame...
   */
  WifiMode mode = GetDefaultMode ();
  bool found = false;

  // First, search the BSS Basic Rate set
  for (WifiModeListIterator i = m_bssBasicRateSet.begin ();
       i != m_bssBasicRateSet.end (); i++)
    {
      if ((!found || i->GetPhyRate () > mode.GetPhyRate ())
          && i->GetPhyRate () <= reqMode.GetPhyRate ()
          && i->GetModulationClass () == reqMode.GetModulationClass ())
        {
          mode = *i;
          // We've found a potentially-suitable transmit rate, but we
          // need to continue and consider all the basic rates before
          // we can be sure we've got the right one.
          found = true;
        }
    }

  // If we found a suitable rate in the BSSBasicRateSet, then we are
  // done and can return that mode.
  if (found)
    {
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
   * TODO: Note that we're ignoring the last sentence for now, because
   * there is not yet any manipulation here of PHY options.
   */
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
          && (!found || thismode.GetPhyRate () > mode.GetPhyRate ())
          && thismode.GetPhyRate () <= reqMode.GetPhyRate ()
          && thismode.GetModulationClass () == reqMode.GetModulationClass ())
        {
          mode = thismode;
          // As above; we've found a potentially-suitable transmit
          // rate, but we need to continue and consider all the
          // mandatory rates before we can be sure we've got the right
          // one.
          found = true;
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
      NS_FATAL_ERROR ("Can't find response rate for " << reqMode
                                                      << ". Check standard and selected rates match.");
    }

  return mode;
}

WifiMode
WifiRemoteStationManager::GetCtsMode (Mac48Address address, WifiMode rtsMode)
{
  NS_ASSERT (!address.IsGroup ());
  return GetControlAnswerMode (address, rtsMode);
}
WifiMode
WifiRemoteStationManager::GetAckMode (Mac48Address address, WifiMode dataMode)
{
  NS_ASSERT (!address.IsGroup ());
  return GetControlAnswerMode (address, dataMode);
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
  for (StationStates::const_iterator i = m_states.begin (); i != m_states.end (); i++)
    {
      if ((*i)->m_address == address)
        {
          return (*i);
        }
    }
  WifiRemoteStationState *state = new WifiRemoteStationState ();
  state->m_state = WifiRemoteStationState::BRAND_NEW;
  state->m_address = address;
  state->m_operationalRateSet.push_back (GetDefaultMode ());
  const_cast<WifiRemoteStationManager *> (this)->m_states.push_back (state);
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
  // XXX
  const_cast<WifiRemoteStationManager *> (this)->m_stations.push_back (station);
  return station;

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
      delete (*i);
    }
  m_stations.clear ();
  m_bssBasicRateSet.clear ();
  m_bssBasicRateSet.push_back (m_defaultTxMode);
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
  NS_ASSERT (i < m_bssBasicRateSet.size ());
  return m_bssBasicRateSet[i];
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
uint32_t
WifiRemoteStationManager::GetNSupported (const WifiRemoteStation *station) const
{
  return station->m_state->m_operationalRateSet.size ();
}

//WifiRemoteStationInfo constructor
WifiRemoteStationInfo::WifiRemoteStationInfo ()
  : m_memoryTime (Seconds (1.0)),
    m_lastUpdate (Seconds (0.0)),
    m_failAvg (0.0)
{
}

double
WifiRemoteStationInfo::CalculateAveragingCoefficient ()
{
  double retval = exp ((double)(m_lastUpdate.GetMicroSeconds () - Simulator::Now ().GetMicroSeconds ())
                       / (double)m_memoryTime.GetMicroSeconds ());
  m_lastUpdate = Simulator::Now ();
  return retval;
}

void
WifiRemoteStationInfo::NotifyTxSuccess (uint32_t retryCounter)
{
  double coefficient = CalculateAveragingCoefficient ();
  m_failAvg = (double)retryCounter / (1 + (double) retryCounter) * (1.0 - coefficient) + coefficient * m_failAvg;
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
} // namespace ns3
