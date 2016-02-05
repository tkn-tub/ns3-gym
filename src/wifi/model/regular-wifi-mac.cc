/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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

#include "regular-wifi-mac.h"
#include "ns3/log.h"
#include "ns3/boolean.h"
#include "ns3/pointer.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "mac-rx-middle.h"
#include "mac-tx-middle.h"
#include "mac-low.h"
#include "dcf.h"
#include "dcf-manager.h"
#include "wifi-phy.h"
#include "msdu-standard-aggregator.h"
#include "mpdu-standard-aggregator.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RegularWifiMac");

NS_OBJECT_ENSURE_REGISTERED (RegularWifiMac);

RegularWifiMac::RegularWifiMac ()
{
  NS_LOG_FUNCTION (this);
  m_rxMiddle = new MacRxMiddle ();
  m_rxMiddle->SetForwardCallback (MakeCallback (&RegularWifiMac::Receive, this));

  m_txMiddle = new MacTxMiddle ();

  m_low = CreateObject<MacLow> ();
  m_low->SetRxCallback (MakeCallback (&MacRxMiddle::Receive, m_rxMiddle));

  m_dcfManager = new DcfManager ();
  m_dcfManager->SetupLowListener (m_low);

  m_dca = CreateObject<DcaTxop> ();
  m_dca->SetLow (m_low);
  m_dca->SetManager (m_dcfManager);
  m_dca->SetTxMiddle (m_txMiddle);
  m_dca->SetTxOkCallback (MakeCallback (&RegularWifiMac::TxOk, this));
  m_dca->SetTxFailedCallback (MakeCallback (&RegularWifiMac::TxFailed, this));

  //Construct the EDCAFs. The ordering is important - highest
  //priority (Table 9-1 UP-to-AC mapping; IEEE 802.11-2012) must be created
  //first.
  SetupEdcaQueue (AC_VO);
  SetupEdcaQueue (AC_VI);
  SetupEdcaQueue (AC_BE);
  SetupEdcaQueue (AC_BK);
}

RegularWifiMac::~RegularWifiMac ()
{
  NS_LOG_FUNCTION (this);
}

void
RegularWifiMac::DoInitialize ()
{
  NS_LOG_FUNCTION (this);
  m_dca->Initialize ();

  for (EdcaQueues::iterator i = m_edca.begin (); i != m_edca.end (); ++i)
    {
      i->second->Initialize ();
    }
}

void
RegularWifiMac::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_rxMiddle;
  m_rxMiddle = 0;

  delete m_txMiddle;
  m_txMiddle = 0;

  delete m_dcfManager;
  m_dcfManager = 0;

  m_low->Dispose ();
  m_low = 0;

  m_phy = 0;
  m_stationManager = 0;

  m_dca->Dispose ();
  m_dca = 0;

  for (EdcaQueues::iterator i = m_edca.begin (); i != m_edca.end (); ++i)
    {
      i->second = 0;
    }
}

void
RegularWifiMac::SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> stationManager)
{
  NS_LOG_FUNCTION (this << stationManager);
  m_stationManager = stationManager;
  m_stationManager->SetHtSupported (GetHtSupported ());
  m_stationManager->SetVhtSupported (GetVhtSupported ());
  m_low->SetWifiRemoteStationManager (stationManager);

  m_dca->SetWifiRemoteStationManager (stationManager);

  for (EdcaQueues::iterator i = m_edca.begin (); i != m_edca.end (); ++i)
    {
      i->second->SetWifiRemoteStationManager (stationManager);
    }
}

Ptr<WifiRemoteStationManager>
RegularWifiMac::GetWifiRemoteStationManager () const
{
  return m_stationManager;
}

void
RegularWifiMac::SetVoMaxAmsduSize (uint32_t size)
{
  NS_LOG_FUNCTION (this << size);
  m_voMaxAmsduSize = size;
  ConfigureAggregation ();
}

void
RegularWifiMac::SetViMaxAmsduSize (uint32_t size)
{
  NS_LOG_FUNCTION (this << size);
  m_viMaxAmsduSize = size;
  ConfigureAggregation ();
}

void
RegularWifiMac::SetBeMaxAmsduSize (uint32_t size)
{
  NS_LOG_FUNCTION (this << size);
  m_beMaxAmsduSize = size;
  ConfigureAggregation ();
}

void
RegularWifiMac::SetBkMaxAmsduSize (uint32_t size)
{
  NS_LOG_FUNCTION (this << size);
  m_bkMaxAmsduSize = size;
  ConfigureAggregation ();
}

void
RegularWifiMac::SetVoMaxAmpduSize (uint32_t size)
{
  NS_LOG_FUNCTION (this << size);
  m_voMaxAmpduSize = size;
  ConfigureAggregation ();
}

void
RegularWifiMac::SetViMaxAmpduSize (uint32_t size)
{
  NS_LOG_FUNCTION (this << size);
  m_viMaxAmpduSize = size;
  ConfigureAggregation ();
}

void
RegularWifiMac::SetBeMaxAmpduSize (uint32_t size)
{
  NS_LOG_FUNCTION (this << size);
  m_beMaxAmpduSize = size;
  ConfigureAggregation ();
}

void
RegularWifiMac::SetBkMaxAmpduSize (uint32_t size)
{
  NS_LOG_FUNCTION (this << size);
  m_bkMaxAmpduSize = size;
  ConfigureAggregation ();
}

void
RegularWifiMac::SetVoBlockAckThreshold (uint8_t threshold)
{
  NS_LOG_FUNCTION (this << threshold);
  GetVOQueue ()->SetBlockAckThreshold (threshold);
}

void
RegularWifiMac::SetViBlockAckThreshold (uint8_t threshold)
{
  NS_LOG_FUNCTION (this << threshold);
  GetVIQueue ()->SetBlockAckThreshold (threshold);
}

void
RegularWifiMac::SetBeBlockAckThreshold (uint8_t threshold)
{
  NS_LOG_FUNCTION (this << threshold);
  GetBEQueue ()->SetBlockAckThreshold (threshold);
}

void
RegularWifiMac::SetBkBlockAckThreshold (uint8_t threshold)
{
  NS_LOG_FUNCTION (this << threshold);
  GetBKQueue ()->SetBlockAckThreshold (threshold);
}

void
RegularWifiMac::SetVoBlockAckInactivityTimeout (uint16_t timeout)
{
  NS_LOG_FUNCTION (this << timeout);
  GetVOQueue ()->SetBlockAckInactivityTimeout (timeout);
}

void
RegularWifiMac::SetViBlockAckInactivityTimeout (uint16_t timeout)
{
  NS_LOG_FUNCTION (this << timeout);
  GetVIQueue ()->SetBlockAckInactivityTimeout (timeout);
}

void
RegularWifiMac::SetBeBlockAckInactivityTimeout (uint16_t timeout)
{
  NS_LOG_FUNCTION (this << timeout);
  GetBEQueue ()->SetBlockAckInactivityTimeout (timeout);
}

void
RegularWifiMac::SetBkBlockAckInactivityTimeout (uint16_t timeout)
{
  NS_LOG_FUNCTION (this << timeout);
  GetBKQueue ()->SetBlockAckInactivityTimeout (timeout);
}

void
RegularWifiMac::SetupEdcaQueue (enum AcIndex ac)
{
  NS_LOG_FUNCTION (this << ac);

  //Our caller shouldn't be attempting to setup a queue that is
  //already configured.
  NS_ASSERT (m_edca.find (ac) == m_edca.end ());

  Ptr<EdcaTxopN> edca = CreateObject<EdcaTxopN> ();
  edca->SetLow (m_low);
  edca->SetManager (m_dcfManager);
  edca->SetTxMiddle (m_txMiddle);
  edca->SetTxOkCallback (MakeCallback (&RegularWifiMac::TxOk, this));
  edca->SetTxFailedCallback (MakeCallback (&RegularWifiMac::TxFailed, this));
  edca->SetAccessCategory (ac);
  edca->CompleteConfig ();

  m_edca.insert (std::make_pair (ac, edca));
}

void
RegularWifiMac::SetTypeOfStation (TypeOfStation type)
{
  NS_LOG_FUNCTION (this << type);
  for (EdcaQueues::iterator i = m_edca.begin (); i != m_edca.end (); ++i)
    {
      i->second->SetTypeOfStation (type);
    }
}

Ptr<DcaTxop>
RegularWifiMac::GetDcaTxop () const
{
  return m_dca;
}

Ptr<EdcaTxopN>
RegularWifiMac::GetVOQueue () const
{
  return m_edca.find (AC_VO)->second;
}

Ptr<EdcaTxopN>
RegularWifiMac::GetVIQueue () const
{
  return m_edca.find (AC_VI)->second;
}

Ptr<EdcaTxopN>
RegularWifiMac::GetBEQueue () const
{
  return m_edca.find (AC_BE)->second;
}

Ptr<EdcaTxopN>
RegularWifiMac::GetBKQueue () const
{
  return m_edca.find (AC_BK)->second;
}

void
RegularWifiMac::SetWifiPhy (Ptr<WifiPhy> phy)
{
  NS_LOG_FUNCTION (this << phy);
  m_phy = phy;
  m_dcfManager->SetupPhyListener (phy);
  m_low->SetPhy (phy);
}

Ptr<WifiPhy>
RegularWifiMac::GetWifiPhy (void) const
{
  NS_LOG_FUNCTION (this);
  return m_phy;
}

void
RegularWifiMac::ResetWifiPhy (void)
{
  NS_LOG_FUNCTION (this);
  m_low->ResetPhy ();
  m_dcfManager->RemovePhyListener (m_phy);
  m_phy = 0;
}

void
RegularWifiMac::SetForwardUpCallback (ForwardUpCallback upCallback)
{
  NS_LOG_FUNCTION (this);
  m_forwardUp = upCallback;
}

void
RegularWifiMac::SetLinkUpCallback (Callback<void> linkUp)
{
  NS_LOG_FUNCTION (this);
  m_linkUp = linkUp;
}

void
RegularWifiMac::SetLinkDownCallback (Callback<void> linkDown)
{
  NS_LOG_FUNCTION (this);
  m_linkDown = linkDown;
}

void
RegularWifiMac::SetQosSupported (bool enable)
{
  NS_LOG_FUNCTION (this << enable);
  m_qosSupported = enable;
}

bool
RegularWifiMac::GetQosSupported () const
{
  return m_qosSupported;
}

void
RegularWifiMac::SetHtSupported (bool enable)
{
  NS_LOG_FUNCTION (this << enable);
  m_htSupported = enable;
  if (enable)
    {
      SetQosSupported (true);
    }
  if (!enable && !m_vhtSupported)
    {
      DisableAggregation ();
    }
  else
    {
      EnableAggregation ();
    }
}

bool
RegularWifiMac::GetVhtSupported () const
{
  return m_vhtSupported;
}

void
RegularWifiMac::SetVhtSupported (bool enable)
{
  NS_LOG_FUNCTION (this << enable);
  m_vhtSupported = enable;
  if (enable)
    {
      SetQosSupported (true);
    }
  if (!enable && !m_htSupported)
    {
      DisableAggregation ();
    }
  else
    {
      EnableAggregation ();
    }
}

bool
RegularWifiMac::GetHtSupported () const
{
  return m_htSupported;
}

void
RegularWifiMac::SetCtsToSelfSupported (bool enable)
{
  NS_LOG_FUNCTION (this);
  m_low->SetCtsToSelfSupported (enable);
}

bool
RegularWifiMac::GetCtsToSelfSupported () const
{
  return m_low->GetCtsToSelfSupported ();
}

void
RegularWifiMac::SetSlot (Time slotTime)
{
  NS_LOG_FUNCTION (this << slotTime);
  m_dcfManager->SetSlot (slotTime);
  m_low->SetSlotTime (slotTime);
}

Time
RegularWifiMac::GetSlot (void) const
{
  return m_low->GetSlotTime ();
}

void
RegularWifiMac::SetSifs (Time sifs)
{
  NS_LOG_FUNCTION (this << sifs);
  m_dcfManager->SetSifs (sifs);
  m_low->SetSifs (sifs);
}

Time
RegularWifiMac::GetSifs (void) const
{
  return m_low->GetSifs ();
}

void
RegularWifiMac::SetEifsNoDifs (Time eifsNoDifs)
{
  NS_LOG_FUNCTION (this << eifsNoDifs);
  m_dcfManager->SetEifsNoDifs (eifsNoDifs);
}

Time
RegularWifiMac::GetEifsNoDifs (void) const
{
  return m_dcfManager->GetEifsNoDifs ();
}

void
RegularWifiMac::SetRifs (Time rifs)
{
  NS_LOG_FUNCTION (this << rifs);
  m_low->SetRifs (rifs);
}

Time
RegularWifiMac::GetRifs (void) const
{
  return m_low->GetRifs ();
}

void
RegularWifiMac::SetPifs (Time pifs)
{
  NS_LOG_FUNCTION (this << pifs);
  m_low->SetPifs (pifs);
}

Time
RegularWifiMac::GetPifs (void) const
{
  return m_low->GetPifs ();
}

void
RegularWifiMac::SetAckTimeout (Time ackTimeout)
{
  NS_LOG_FUNCTION (this << ackTimeout);
  m_low->SetAckTimeout (ackTimeout);
}

Time
RegularWifiMac::GetAckTimeout (void) const
{
  return m_low->GetAckTimeout ();
}

void
RegularWifiMac::SetCtsTimeout (Time ctsTimeout)
{
  NS_LOG_FUNCTION (this << ctsTimeout);
  m_low->SetCtsTimeout (ctsTimeout);
}

Time
RegularWifiMac::GetCtsTimeout (void) const
{
  return m_low->GetCtsTimeout ();
}

void
RegularWifiMac::SetBasicBlockAckTimeout (Time blockAckTimeout)
{
  NS_LOG_FUNCTION (this << blockAckTimeout);
  m_low->SetBasicBlockAckTimeout (blockAckTimeout);
}

Time
RegularWifiMac::GetBasicBlockAckTimeout (void) const
{
  return m_low->GetBasicBlockAckTimeout ();
}

void
RegularWifiMac::SetCompressedBlockAckTimeout (Time blockAckTimeout)
{
  NS_LOG_FUNCTION (this << blockAckTimeout);
  m_low->SetCompressedBlockAckTimeout (blockAckTimeout);
}

Time
RegularWifiMac::GetCompressedBlockAckTimeout (void) const
{
  return m_low->GetCompressedBlockAckTimeout ();
}

void
RegularWifiMac::SetAddress (Mac48Address address)
{
  NS_LOG_FUNCTION (this << address);
  m_low->SetAddress (address);
}

Mac48Address
RegularWifiMac::GetAddress (void) const
{
  return m_low->GetAddress ();
}

void
RegularWifiMac::SetSsid (Ssid ssid)
{
  NS_LOG_FUNCTION (this << ssid);
  m_ssid = ssid;
}

Ssid
RegularWifiMac::GetSsid (void) const
{
  return m_ssid;
}

void
RegularWifiMac::SetBssid (Mac48Address bssid)
{
  NS_LOG_FUNCTION (this << bssid);
  m_low->SetBssid (bssid);
}

Mac48Address
RegularWifiMac::GetBssid (void) const
{
  return m_low->GetBssid ();
}

void
RegularWifiMac::SetPromisc (void)
{
  m_low->SetPromisc ();
}

void
RegularWifiMac::Enqueue (Ptr<const Packet> packet,
                         Mac48Address to, Mac48Address from)
{
  //We expect RegularWifiMac subclasses which do support forwarding (e.g.,
  //AP) to override this method. Therefore, we throw a fatal error if
  //someone tries to invoke this method on a class which has not done
  //this.
  NS_FATAL_ERROR ("This MAC entity (" << this << ", " << GetAddress ()
                                      << ") does not support Enqueue() with from address");
}

bool
RegularWifiMac::SupportsSendFrom (void) const
{
  return false;
}

void
RegularWifiMac::ForwardUp (Ptr<Packet> packet, Mac48Address from, Mac48Address to)
{
  NS_LOG_FUNCTION (this << packet << from);
  m_forwardUp (packet, from, to);
}

void
RegularWifiMac::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr)
{
  NS_LOG_FUNCTION (this << packet << hdr);

  Mac48Address to = hdr->GetAddr1 ();
  Mac48Address from = hdr->GetAddr2 ();

  //We don't know how to deal with any frame that is not addressed to
  //us (and odds are there is nothing sensible we could do anyway),
  //so we ignore such frames.
  //
  //The derived class may also do some such filtering, but it doesn't
  //hurt to have it here too as a backstop.
  if (to != GetAddress ())
    {
      return;
    }

  if (hdr->IsMgt () && hdr->IsAction ())
    {
      //There is currently only any reason for Management Action
      //frames to be flying about if we are a QoS STA.
      NS_ASSERT (m_qosSupported);

      WifiActionHeader actionHdr;
      packet->RemoveHeader (actionHdr);

      switch (actionHdr.GetCategory ())
        {
        case WifiActionHeader::BLOCK_ACK:

          switch (actionHdr.GetAction ().blockAck)
            {
            case WifiActionHeader::BLOCK_ACK_ADDBA_REQUEST:
              {
                MgtAddBaRequestHeader reqHdr;
                packet->RemoveHeader (reqHdr);

                //We've received an ADDBA Request. Our policy here is
                //to automatically accept it, so we get the ADDBA
                //Response on it's way immediately.
                SendAddBaResponse (&reqHdr, from);
                //This frame is now completely dealt with, so we're done.
                return;
              }
            case WifiActionHeader::BLOCK_ACK_ADDBA_RESPONSE:
              {
                MgtAddBaResponseHeader respHdr;
                packet->RemoveHeader (respHdr);

                //We've received an ADDBA Response. We assume that it
                //indicates success after an ADDBA Request we have
                //sent (we could, in principle, check this, but it
                //seems a waste given the level of the current model)
                //and act by locally establishing the agreement on
                //the appropriate queue.
                AcIndex ac = QosUtilsMapTidToAc (respHdr.GetTid ());
                m_edca[ac]->GotAddBaResponse (&respHdr, from);
                //This frame is now completely dealt with, so we're done.
                return;
              }
            case WifiActionHeader::BLOCK_ACK_DELBA:
              {
                MgtDelBaHeader delBaHdr;
                packet->RemoveHeader (delBaHdr);

                if (delBaHdr.IsByOriginator ())
                  {
                    //This DELBA frame was sent by the originator, so
                    //this means that an ingoing established
                    //agreement exists in MacLow and we need to
                    //destroy it.
                    m_low->DestroyBlockAckAgreement (from, delBaHdr.GetTid ());
                  }
                else
                  {
                    //We must have been the originator. We need to
                    //tell the correct queue that the agreement has
                    //been torn down
                    AcIndex ac = QosUtilsMapTidToAc (delBaHdr.GetTid ());
                    m_edca[ac]->GotDelBaFrame (&delBaHdr, from);
                  }
                //This frame is now completely dealt with, so we're done.
                return;
              }
            default:
              NS_FATAL_ERROR ("Unsupported Action field in Block Ack Action frame");
              return;
            }
        default:
          NS_FATAL_ERROR ("Unsupported Action frame received");
          return;
        }
    }
  NS_FATAL_ERROR ("Don't know how to handle frame (type=" << hdr->GetType ());
}

void
RegularWifiMac::DeaggregateAmsduAndForward (Ptr<Packet> aggregatedPacket,
                                            const WifiMacHeader *hdr)
{
  MsduAggregator::DeaggregatedMsdus packets =
    MsduAggregator::Deaggregate (aggregatedPacket);

  for (MsduAggregator::DeaggregatedMsdusCI i = packets.begin ();
       i != packets.end (); ++i)
    {
      ForwardUp ((*i).first, (*i).second.GetSourceAddr (),
                 (*i).second.GetDestinationAddr ());
    }
}

void
RegularWifiMac::SendAddBaResponse (const MgtAddBaRequestHeader *reqHdr,
                                   Mac48Address originator)
{
  NS_LOG_FUNCTION (this);
  WifiMacHeader hdr;
  hdr.SetAction ();
  hdr.SetAddr1 (originator);
  hdr.SetAddr2 (GetAddress ());
  hdr.SetAddr3 (GetAddress ());
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();

  MgtAddBaResponseHeader respHdr;
  StatusCode code;
  code.SetSuccess ();
  respHdr.SetStatusCode (code);
  //Here a control about queues type?
  respHdr.SetAmsduSupport (reqHdr->IsAmsduSupported ());

  if (reqHdr->IsImmediateBlockAck ())
    {
      respHdr.SetImmediateBlockAck ();
    }
  else
    {
      respHdr.SetDelayedBlockAck ();
    }
  respHdr.SetTid (reqHdr->GetTid ());
  //For now there's not no control about limit of reception. We
  //assume that receiver has no limit on reception. However we assume
  //that a receiver sets a bufferSize in order to satisfy next
  //equation: (bufferSize + 1) % 16 = 0 So if a recipient is able to
  //buffer a packet, it should be also able to buffer all possible
  //packet's fragments. See section 7.3.1.14 in IEEE802.11e for more details.
  respHdr.SetBufferSize (1023);
  respHdr.SetTimeout (reqHdr->GetTimeout ());

  WifiActionHeader actionHdr;
  WifiActionHeader::ActionValue action;
  action.blockAck = WifiActionHeader::BLOCK_ACK_ADDBA_RESPONSE;
  actionHdr.SetAction (WifiActionHeader::BLOCK_ACK, action);

  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (respHdr);
  packet->AddHeader (actionHdr);

  //We need to notify our MacLow object as it will have to buffer all
  //correctly received packets for this Block Ack session
  m_low->CreateBlockAckAgreement (&respHdr, originator,
                                  reqHdr->GetStartingSequence ());

  //It is unclear which queue this frame should go into. For now we
  //bung it into the queue corresponding to the TID for which we are
  //establishing an agreement, and push it to the head.
  m_edca[QosUtilsMapTidToAc (reqHdr->GetTid ())]->PushFront (packet, hdr);
}

TypeId
RegularWifiMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RegularWifiMac")
    .SetParent<WifiMac> ()
    .SetGroupName ("Wifi")
    .AddAttribute ("QosSupported",
                   "This Boolean attribute is set to enable 802.11e/WMM-style QoS support at this STA",
                   BooleanValue (false),
                   MakeBooleanAccessor (&RegularWifiMac::SetQosSupported,
                                        &RegularWifiMac::GetQosSupported),
                   MakeBooleanChecker ())
    .AddAttribute ("HtSupported",
                   "This Boolean attribute is set to enable 802.11n support at this STA",
                   BooleanValue (false),
                   MakeBooleanAccessor (&RegularWifiMac::SetHtSupported,
                                        &RegularWifiMac::GetHtSupported),
                   MakeBooleanChecker ())
    .AddAttribute ("VhtSupported",
                   "This Boolean attribute is set to enable 802.11ac support at this STA",
                   BooleanValue (false),
                   MakeBooleanAccessor (&RegularWifiMac::SetVhtSupported,
                                        &RegularWifiMac::GetVhtSupported),
                   MakeBooleanChecker ())
    .AddAttribute ("CtsToSelfSupported",
                   "Use CTS to Self when using a rate that is not in the basic set rate",
                   BooleanValue (false),
                   MakeBooleanAccessor (&RegularWifiMac::SetCtsToSelfSupported,
                                        &RegularWifiMac::GetCtsToSelfSupported),
                   MakeBooleanChecker ())
    .AddAttribute ("VO_MaxAmsduSize",
                   "Maximum length in bytes of an A-MSDU for AC_VO access class",
                   UintegerValue (0),
                   MakeUintegerAccessor (&RegularWifiMac::SetVoMaxAmsduSize),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("VI_MaxAmsduSize",
                   "Maximum length in bytes of an A-MSDU for AC_VI access class",
                   UintegerValue (0),
                   MakeUintegerAccessor (&RegularWifiMac::SetViMaxAmsduSize),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("BE_MaxAmsduSize",
                   "Maximum length in bytes of an A-MSDU for AC_BE access class",
                   UintegerValue (0),
                   MakeUintegerAccessor (&RegularWifiMac::SetBeMaxAmsduSize),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("BK_MaxAmsduSize",
                   "Maximum length in bytes of an A-MSDU for AC_BK access class",
                   UintegerValue (0),
                   MakeUintegerAccessor (&RegularWifiMac::SetBkMaxAmsduSize),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("VO_MaxAmpduSize",
                   "Maximum length in bytes of an A-MPDU for AC_VO access class",
                   UintegerValue (0),
                   MakeUintegerAccessor (&RegularWifiMac::SetVoMaxAmpduSize),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("VI_MaxAmpduSize",
                   "Maximum length in bytes of an A-MPDU for AC_VI access class",
                   UintegerValue (65535),
                   MakeUintegerAccessor (&RegularWifiMac::SetViMaxAmpduSize),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("BE_MaxAmpduSize",
                   "Maximum length in bytes of an A-MPDU for AC_BE access class",
                   UintegerValue (65535),
                   MakeUintegerAccessor (&RegularWifiMac::SetBeMaxAmpduSize),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("BK_MaxAmpduSize",
                   "Maximum length in bytes of an A-MPDU for AC_BK access class",
                   UintegerValue (0),
                   MakeUintegerAccessor (&RegularWifiMac::SetBkMaxAmpduSize),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("VO_BlockAckThreshold",
                   "If number of packets in VO queue reaches this value, "
                   "block ack mechanism is used. If this value is 0, block ack is never used.",
                   UintegerValue (0),
                   MakeUintegerAccessor (&RegularWifiMac::SetVoBlockAckThreshold),
                   MakeUintegerChecker<uint8_t> (0, 64))
    .AddAttribute ("VI_BlockAckThreshold",
                   "If number of packets in VI queue reaches this value, "
                   "block ack mechanism is used. If this value is 0, block ack is never used.",
                   UintegerValue (0),
                   MakeUintegerAccessor (&RegularWifiMac::SetViBlockAckThreshold),
                   MakeUintegerChecker<uint8_t> (0, 64))
    .AddAttribute ("BE_BlockAckThreshold",
                   "If number of packets in BE queue reaches this value, "
                   "block ack mechanism is used. If this value is 0, block ack is never used.",
                   UintegerValue (0),
                   MakeUintegerAccessor (&RegularWifiMac::SetBeBlockAckThreshold),
                   MakeUintegerChecker<uint8_t> (0, 64))
    .AddAttribute ("BK_BlockAckThreshold",
                   "If number of packets in BK queue reaches this value, "
                   "block ack mechanism is used. If this value is 0, block ack is never used.",
                   UintegerValue (0),
                   MakeUintegerAccessor (&RegularWifiMac::SetBkBlockAckThreshold),
                   MakeUintegerChecker<uint8_t> (0, 64))
    .AddAttribute ("VO_BlockAckInactivityTimeout",
                   "Represents max time (blocks of 1024 micro seconds) allowed for block ack"
                   "inactivity for AC_VO. If this value isn't equal to 0 a timer start after that a"
                   "block ack setup is completed and will be reset every time that a block ack"
                   "frame is received. If this value is 0, block ack inactivity timeout won't be used.",
                   UintegerValue (0),
                   MakeUintegerAccessor (&RegularWifiMac::SetVoBlockAckInactivityTimeout),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("VI_BlockAckInactivityTimeout",
                   "Represents max time (blocks of 1024 micro seconds) allowed for block ack"
                   "inactivity for AC_VI. If this value isn't equal to 0 a timer start after that a"
                   "block ack setup is completed and will be reset every time that a block ack"
                   "frame is received. If this value is 0, block ack inactivity timeout won't be used.",
                   UintegerValue (0),
                   MakeUintegerAccessor (&RegularWifiMac::SetViBlockAckInactivityTimeout),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("BE_BlockAckInactivityTimeout",
                   "Represents max time (blocks of 1024 micro seconds) allowed for block ack"
                   "inactivity for AC_BE. If this value isn't equal to 0 a timer start after that a"
                   "block ack setup is completed and will be reset every time that a block ack"
                   "frame is received. If this value is 0, block ack inactivity timeout won't be used.",
                   UintegerValue (0),
                   MakeUintegerAccessor (&RegularWifiMac::SetBeBlockAckInactivityTimeout),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("BK_BlockAckInactivityTimeout",
                   "Represents max time (blocks of 1024 micro seconds) allowed for block ack"
                   "inactivity for AC_BK. If this value isn't equal to 0 a timer start after that a"
                   "block ack setup is completed and will be reset every time that a block ack"
                   "frame is received. If this value is 0, block ack inactivity timeout won't be used.",
                   UintegerValue (0),
                   MakeUintegerAccessor (&RegularWifiMac::SetBkBlockAckInactivityTimeout),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("DcaTxop", "The DcaTxop object",
                   PointerValue (),
                   MakePointerAccessor (&RegularWifiMac::GetDcaTxop),
                   MakePointerChecker<DcaTxop> ())
    .AddAttribute ("VO_EdcaTxopN",
                   "Queue that manages packets belonging to AC_VO access class",
                   PointerValue (),
                   MakePointerAccessor (&RegularWifiMac::GetVOQueue),
                   MakePointerChecker<EdcaTxopN> ())
    .AddAttribute ("VI_EdcaTxopN",
                   "Queue that manages packets belonging to AC_VI access class",
                   PointerValue (),
                   MakePointerAccessor (&RegularWifiMac::GetVIQueue),
                   MakePointerChecker<EdcaTxopN> ())
    .AddAttribute ("BE_EdcaTxopN",
                   "Queue that manages packets belonging to AC_BE access class",
                   PointerValue (),
                   MakePointerAccessor (&RegularWifiMac::GetBEQueue),
                   MakePointerChecker<EdcaTxopN> ())
    .AddAttribute ("BK_EdcaTxopN",
                   "Queue that manages packets belonging to AC_BK access class",
                   PointerValue (),
                   MakePointerAccessor (&RegularWifiMac::GetBKQueue),
                   MakePointerChecker<EdcaTxopN> ())
    .AddTraceSource ("TxOkHeader",
                     "The header of successfully transmitted packet",
                     MakeTraceSourceAccessor (&RegularWifiMac::m_txOkCallback),
                     "ns3::WifiMacHeader::TracedCallback")
    .AddTraceSource ("TxErrHeader",
                     "The header of unsuccessfully transmitted packet",
                     MakeTraceSourceAccessor (&RegularWifiMac::m_txErrCallback),
                     "ns3::WifiMacHeader::TracedCallback")
  ;
  return tid;
}

void
RegularWifiMac::FinishConfigureStandard (enum WifiPhyStandard standard)
{
  NS_LOG_FUNCTION (this << standard);
  uint32_t cwmin = 0;
  uint32_t cwmax = 0;
  switch (standard)
    {
    case WIFI_PHY_STANDARD_80211ac:
      SetVhtSupported (true);
    case WIFI_PHY_STANDARD_80211n_5GHZ:
    case WIFI_PHY_STANDARD_80211n_2_4GHZ:
      SetHtSupported (true);
    case WIFI_PHY_STANDARD_holland:
    case WIFI_PHY_STANDARD_80211a:
    case WIFI_PHY_STANDARD_80211g:
    case WIFI_PHY_STANDARD_80211_10MHZ:
    case WIFI_PHY_STANDARD_80211_5MHZ:
      cwmin = 15;
      cwmax = 1023;
      break;
    case WIFI_PHY_STANDARD_80211b:
      cwmin = 31;
      cwmax = 1023;
      break;
    default:
      NS_FATAL_ERROR ("Unsupported WifiPhyStandard in RegularWifiMac::FinishConfigureStandard ()");
    }

  //The special value of AC_BE_NQOS which exists in the Access
  //Category enumeration allows us to configure plain old DCF.
  ConfigureDcf (m_dca, cwmin, cwmax, AC_BE_NQOS);

  //Now we configure the EDCA functions
  for (EdcaQueues::iterator i = m_edca.begin (); i != m_edca.end (); ++i)
    {
      ConfigureDcf (i->second, cwmin, cwmax, i->first);
    }
}

void
RegularWifiMac::TxOk (const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this << hdr);
  m_txOkCallback (hdr);
}

void
RegularWifiMac::TxFailed (const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this << hdr);
  m_txErrCallback (hdr);
}

void
RegularWifiMac::ConfigureAggregation (void)
{
  NS_LOG_FUNCTION (this);
  if (GetVOQueue ()->GetMsduAggregator () != 0)
    {
      GetVOQueue ()->GetMsduAggregator ()->SetMaxAmsduSize (m_voMaxAmsduSize);
    }
  if (GetVIQueue ()->GetMsduAggregator () != 0)
    {
      GetVIQueue ()->GetMsduAggregator ()->SetMaxAmsduSize (m_viMaxAmsduSize);
    }
  if (GetBEQueue ()->GetMsduAggregator () != 0)
    {
      GetBEQueue ()->GetMsduAggregator ()->SetMaxAmsduSize (m_beMaxAmsduSize);
    }
  if (GetBKQueue ()->GetMsduAggregator () != 0)
    {
      GetBKQueue ()->GetMsduAggregator ()->SetMaxAmsduSize (m_bkMaxAmsduSize);
    }
  if (GetVOQueue ()->GetMpduAggregator () != 0)
    {
      GetVOQueue ()->GetMpduAggregator ()->SetMaxAmpduSize (m_voMaxAmpduSize);
    }
  if (GetVIQueue ()->GetMpduAggregator () != 0)
    {
      GetVIQueue ()->GetMpduAggregator ()->SetMaxAmpduSize (m_viMaxAmpduSize);
    }
  if (GetBEQueue ()->GetMpduAggregator () != 0)
    {
      GetBEQueue ()->GetMpduAggregator ()->SetMaxAmpduSize (m_beMaxAmpduSize);
    }
  if (GetBKQueue ()->GetMpduAggregator () != 0)
    {
      GetBKQueue ()->GetMpduAggregator ()->SetMaxAmpduSize (m_bkMaxAmpduSize);
    }
}

void
RegularWifiMac::EnableAggregation (void)
{
  NS_LOG_FUNCTION (this);
  for (EdcaQueues::iterator i = m_edca.begin (); i != m_edca.end (); ++i)
  {
    if (i->second->GetMsduAggregator () == 0)
      {
        Ptr<MsduStandardAggregator> msduAggregator = CreateObject<MsduStandardAggregator> ();
        i->second->SetMsduAggregator (msduAggregator);
      }
    if (i->second->GetMpduAggregator () == 0)
      {
        Ptr<MpduStandardAggregator> mpduAggregator = CreateObject<MpduStandardAggregator> ();
        i->second->SetMpduAggregator (mpduAggregator);
      }
  }
  ConfigureAggregation ();
}

void
RegularWifiMac::DisableAggregation (void)
{
  NS_LOG_FUNCTION (this);
  for (EdcaQueues::iterator i = m_edca.begin (); i != m_edca.end (); ++i)
  {
    i->second->SetMsduAggregator (0);
    i->second->SetMpduAggregator (0);
  }
}

} //namespace ns3
