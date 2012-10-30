/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Nicola Baldo  <nbaldo@cttc.es>
 * Author: Marco Miozzo <mmiozzo@cttc.es>
 */



#include <ns3/log.h>
#include <ns3/pointer.h>
#include <ns3/packet.h>
#include <ns3/packet-burst.h>

#include "lte-ue-mac.h"
#include "lte-ue-net-device.h"
#include "lte-radio-bearer-tag.h"
#include <ns3/ff-mac-common.h>
#include <ns3/lte-control-messages.h>
#include <ns3/simulator.h>
#include <ns3/lte-common.h>


NS_LOG_COMPONENT_DEFINE ("LteUeMac");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LteUeMac);


///////////////////////////////////////////////////////////
// SAP forwarders
///////////////////////////////////////////////////////////


class UeMemberLteUeCmacSapProvider : public LteUeCmacSapProvider
{
public:
  UeMemberLteUeCmacSapProvider (LteUeMac* mac);

  // inherited from LteUeCmacSapProvider
  virtual void StartContentionBasedRandomAccessProcedure ();
  virtual void StartNonContentionBasedRandomAccessProcedure (uint16_t rnti, uint8_t preambleId, uint8_t prachMask);
  virtual void AddLc (uint8_t lcId, LteUeCmacSapProvider::LogicalChannelConfig lcConfig, LteMacSapUser* msu);
  virtual void RemoveLc (uint8_t lcId);
  virtual void Reset ();

private:
  LteUeMac* m_mac;
};


UeMemberLteUeCmacSapProvider::UeMemberLteUeCmacSapProvider (LteUeMac* mac)
  : m_mac (mac)
{
}

  void 
UeMemberLteUeCmacSapProvider::StartContentionBasedRandomAccessProcedure ()
{
  m_mac->DoStartContentionBasedRandomAccessProcedure ();
}

 void 
UeMemberLteUeCmacSapProvider::StartNonContentionBasedRandomAccessProcedure (uint16_t rnti, uint8_t preambleId, uint8_t prachMask)
{
  m_mac->DoStartNonContentionBasedRandomAccessProcedure (rnti, preambleId, prachMask);
}


void
UeMemberLteUeCmacSapProvider::AddLc (uint8_t lcId, LogicalChannelConfig lcConfig, LteMacSapUser* msu)
{
  m_mac->DoAddLc (lcId, lcConfig, msu);
}

void
UeMemberLteUeCmacSapProvider::RemoveLc (uint8_t lcid)
{
  m_mac->DoRemoveLc (lcid);
}

void
UeMemberLteUeCmacSapProvider::Reset ()
{
  m_mac->DoReset ();
}

class UeMemberLteMacSapProvider : public LteMacSapProvider
{
public:
  UeMemberLteMacSapProvider (LteUeMac* mac);

  // inherited from LteMacSapProvider
  virtual void TransmitPdu (TransmitPduParameters params);
  virtual void ReportBufferStatus (ReportBufferStatusParameters params);

private:
  LteUeMac* m_mac;
};


UeMemberLteMacSapProvider::UeMemberLteMacSapProvider (LteUeMac* mac)
  : m_mac (mac)
{
}

void
UeMemberLteMacSapProvider::TransmitPdu (TransmitPduParameters params)
{
  m_mac->DoTransmitPdu (params);
}


void
UeMemberLteMacSapProvider::ReportBufferStatus (ReportBufferStatusParameters params)
{
  m_mac->DoReportBufferStatus (params);
}




class UeMemberLteUePhySapUser : public LteUePhySapUser
{
public:
  UeMemberLteUePhySapUser (LteUeMac* mac);

  // inherited from LtePhySapUser
  virtual void ReceivePhyPdu (Ptr<Packet> p);
  virtual void SubframeIndication (uint32_t frameNo, uint32_t subframeNo);
  virtual void ReceiveLteControlMessage (Ptr<LteControlMessage> msg);

private:
  LteUeMac* m_mac;
};

UeMemberLteUePhySapUser::UeMemberLteUePhySapUser (LteUeMac* mac) : m_mac (mac)
{

}

void
UeMemberLteUePhySapUser::ReceivePhyPdu (Ptr<Packet> p)
{
  m_mac->DoReceivePhyPdu (p);
}


void
UeMemberLteUePhySapUser::SubframeIndication (uint32_t frameNo, uint32_t subframeNo)
{
  m_mac->DoSubframeIndication (frameNo, subframeNo);
}

void
UeMemberLteUePhySapUser::ReceiveLteControlMessage (Ptr<LteControlMessage> msg)
{
  m_mac->DoReceiveLteControlMessage (msg);
}




//////////////////////////////////////////////////////////
// LteUeMac methods
///////////////////////////////////////////////////////////


TypeId
LteUeMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteUeMac")
    .SetParent<Object> ()
    .AddConstructor<LteUeMac> ();
  return tid;
}


LteUeMac::LteUeMac ()
  :  m_bsrPeriodicity (MilliSeconds (1)), // ideal behavior
     m_bsrLast (MilliSeconds (0)),
     m_freshUlBsr (false),
     m_rnti (0)
  
{
  NS_LOG_FUNCTION (this);
  m_macSapProvider = new UeMemberLteMacSapProvider (this);
  m_cmacSapProvider = new UeMemberLteUeCmacSapProvider (this);
  m_uePhySapUser = new UeMemberLteUePhySapUser (this);
}


LteUeMac::~LteUeMac ()
{
  NS_LOG_FUNCTION (this);
}

void
LteUeMac::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_macSapProvider;
  delete m_cmacSapProvider;
  delete m_uePhySapUser;
  Object::DoDispose ();
}


LteUePhySapUser*
LteUeMac::GetLteUePhySapUser (void)
{
  return m_uePhySapUser;
}

void
LteUeMac::SetLteUePhySapProvider (LteUePhySapProvider* s)
{
  m_uePhySapProvider = s;
}


LteMacSapProvider*
LteUeMac::GetLteMacSapProvider (void)
{
  return m_macSapProvider;
}

void
LteUeMac::SetLteUeCmacSapUser (LteUeCmacSapUser* s)
{
  m_cmacSapUser = s;
}

LteUeCmacSapProvider*
LteUeMac::GetLteUeCmacSapProvider (void)
{
  return m_cmacSapProvider;
}


void
LteUeMac::DoTransmitPdu (LteMacSapProvider::TransmitPduParameters params)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG (m_rnti == params.rnti, "RNTI mismatch between RLC and MAC");
  LteRadioBearerTag tag (params.rnti, params.lcid, 0 /* UE works in SISO mode*/);
  params.pdu->AddPacketTag (tag);
//   Ptr<PacketBurst> pb = CreateObject<PacketBurst> ();
//   pb->AddPacket (params.pdu);
  m_uePhySapProvider->SendMacPdu (params.pdu);
  // Uplink not implemented yet, so we wait can wait for the PHY SAP
  // to be defined before we implement the transmission method.
}

void
LteUeMac::DoReportBufferStatus (LteMacSapProvider::ReportBufferStatusParameters params)
{
  NS_LOG_FUNCTION (this);
  
  std::map <uint8_t, uint64_t>::iterator it;
  
  
  it = m_ulBsrReceived.find (params.lcid);
  if (it!=m_ulBsrReceived.end ())
    {
      // update entry
      (*it).second = params.txQueueSize + params.retxQueueSize + params.statusPduSize;
    }
  else
    {
      m_ulBsrReceived.insert (std::pair<uint8_t, uint64_t> (params.lcid, params.txQueueSize + params.retxQueueSize + params.statusPduSize));
    }
  m_freshUlBsr = true;
}


void
LteUeMac::SendReportBufferStatus (void)
{
  NS_LOG_FUNCTION (this);

  if (m_rnti == 0)
    {
      NS_LOG_INFO ("MAC not initialized, BSR deferred");
      return; 
    }

  if (m_ulBsrReceived.size () == 0)
    {
      NS_LOG_INFO ("No BSR report to transmit");
      return; 
    }
  MacCeListElement_s bsr;
  bsr.m_rnti = m_rnti;
  bsr.m_macCeType = MacCeListElement_s::BSR;

  // BSR is reported for each LCG
  std::map <uint8_t, uint64_t>::iterator it;  
  std::vector<uint32_t> queue (4, 0); // one value per each of the 4 LCGs, initialized to 0
  for (it = m_ulBsrReceived.begin (); it != m_ulBsrReceived.end (); it++)
    {
      uint8_t lcid = it->first;
      std::map <uint8_t, LcInfo>::iterator lcInfoMapIt;
      lcInfoMapIt = m_lcInfoMap.find (lcid);
      NS_ASSERT (lcInfoMapIt !=  m_lcInfoMap.end ());
      uint8_t lcg = lcInfoMapIt->second.lcConfig.logicalChannelGroup;
      queue.at (lcg) += (*it).second;
    }

  // FF API says that all 4 LCGs are always present
  bsr.m_macCeValue.m_bufferStatus.push_back (BufferSizeLevelBsr::BufferSize2BsrId (queue.at (0)));
  bsr.m_macCeValue.m_bufferStatus.push_back (BufferSizeLevelBsr::BufferSize2BsrId (queue.at (1)));
  bsr.m_macCeValue.m_bufferStatus.push_back (BufferSizeLevelBsr::BufferSize2BsrId (queue.at (2)));
  bsr.m_macCeValue.m_bufferStatus.push_back (BufferSizeLevelBsr::BufferSize2BsrId (queue.at (3)));

  // create the feedback to eNB
  Ptr<BsrLteControlMessage> msg = Create<BsrLteControlMessage> ();
  msg->SetBsr (bsr);
  m_uePhySapProvider->SendLteControlMessage (msg);

}

void 
LteUeMac::DoStartContentionBasedRandomAccessProcedure ()
{
  NS_LOG_FUNCTION (this);
  static uint32_t prachIdCounter = 256;
  prachIdCounter += 2;
  m_prachId = prachIdCounter;
  NS_LOG_INFO ("sending RACH preamble " << m_prachId);


  // Since regular UL LteControlMessages need m_ulConfigured = true in
  // order to be sent by the UE, the rach preamble needs to be sent
  // with a dedicated primitive (not
  // m_uePhySapProvider->SendLteControlMessage (msg)) so that it can
  // bypass the m_ulConfigured flag. This is reasonable, since In fact
  // the RACH preamble is sent on 6RB bandwidth so the uplink
  // bandwidth does not need to be configured. 

  Ptr<RachPreambleLteControlMessage> msg = Create<RachPreambleLteControlMessage> ();
  msg->SetPrachId (m_prachId);
  m_uePhySapProvider->SendRachPreamble (m_prachId);
}

void 
LteUeMac::DoStartNonContentionBasedRandomAccessProcedure (uint16_t rnti, uint8_t preambleId, uint8_t prachMask)
{
  NS_LOG_FUNCTION (this << " rnti" << rnti);
  m_rnti = rnti;
}

void
LteUeMac::DoAddLc (uint8_t lcId,  LteUeCmacSapProvider::LogicalChannelConfig lcConfig, LteMacSapUser* msu)
{
  NS_LOG_FUNCTION (this << " lcId" << (uint16_t) lcId);
  NS_ASSERT_MSG (m_lcInfoMap.find (lcId) == m_lcInfoMap.end (), "cannot add channel because LCID " << lcId << " is already present");
  
  LcInfo lcInfo;
  lcInfo.lcConfig = lcConfig;
  lcInfo.macSapUser = msu;
  m_lcInfoMap[lcId] = lcInfo;
}

void
LteUeMac::DoRemoveLc (uint8_t lcId)
{
  NS_LOG_FUNCTION (this << " lcId" << lcId);
  NS_ASSERT_MSG (m_lcInfoMap.find (lcId) == m_lcInfoMap.end (), "could not find LCID " << lcId);
  m_lcInfoMap.erase (lcId);
}

void
LteUeMac::DoReset ()
{
  NS_LOG_FUNCTION (this);
  std::map <uint8_t, LcInfo>::iterator it = m_lcInfoMap.begin ();
  while (it != m_lcInfoMap.end ())
    {
      // don't delete CCCH)
      if (it->first == 0)
        {          
          ++it;
        }
      else
        {
          // note: use of postfix operator preserves validity of iterator
          m_lcInfoMap.erase (it++);
        }
    }
}

void
LteUeMac::DoReceivePhyPdu (Ptr<Packet> p)
{
  LteRadioBearerTag tag;
  p->RemovePacketTag (tag);
  if (tag.GetRnti () == m_rnti)
    {
      // packet is for the current user
      std::map <uint8_t, LcInfo>::const_iterator it = m_lcInfoMap.find (tag.GetLcid ());
      NS_ASSERT_MSG (it != m_lcInfoMap.end (), "received packet with unknown lcid");
      it->second.macSapUser->ReceivePdu (p);
    }
}


void
LteUeMac::DoReceiveLteControlMessage (Ptr<LteControlMessage> msg)
{
  NS_LOG_FUNCTION (this);
  if (msg->GetMessageType () == LteControlMessage::UL_DCI)
    {
      Ptr<UlDciLteControlMessage> msg2 = DynamicCast<UlDciLteControlMessage> (msg);
      UlDciListElement_s dci = msg2->GetDci ();
      std::map <uint8_t, uint64_t>::iterator itBsr;
      uint16_t activeLcs = 0;
      for (itBsr = m_ulBsrReceived.begin (); itBsr != m_ulBsrReceived.end (); itBsr++)
        {
          if ((*itBsr).second > 0)
            {
              activeLcs++;
            }
        }
      if (activeLcs == 0)
        {
          NS_LOG_ERROR (this << " No active flows for this UL-DCI");
          return;
        }
      std::map <uint8_t, LcInfo>::iterator it;
      uint32_t bytesPerActiveLc = dci.m_tbSize / activeLcs;
      NS_LOG_LOGIC (this << " UE: UL-CQI notified TxOpportunity of " << dci.m_tbSize << " => " << bytesPerActiveLc << " bytes per active LC");
      for (it = m_lcInfoMap.begin (); it!=m_lcInfoMap.end (); it++)
        {
          itBsr = m_ulBsrReceived.find ((*it).first);
          if (((itBsr!=m_ulBsrReceived.end ()) && ((*itBsr).second > 0)))
            {
              NS_LOG_LOGIC (this << "\t" << bytesPerActiveLc << " bytes to LC " << (uint16_t)(*it).first << " queue " << (*itBsr).second);
              (*it).second.macSapUser->NotifyTxOpportunity (bytesPerActiveLc, 0);
              if ((*itBsr).second >=  static_cast<uint64_t> (bytesPerActiveLc))
                {
                  (*itBsr).second -= bytesPerActiveLc;
                }
              else
                {
                  (*itBsr).second = 0;
                }
            }
        }

    }
  else if (msg->GetMessageType () == LteControlMessage::RAR)
    {
      Ptr<RarLteControlMessage> msg2 = DynamicCast<RarLteControlMessage> (msg);
      
      if (msg2->GetPrachId () == m_prachId)
        {
          m_rnti = msg2->GetRar ().m_rnti;
          NS_LOG_INFO ("got RAR for PRACH ID " << m_prachId << ", setting T-C-RNTI = " << m_rnti);
          m_cmacSapUser->SetTemporaryCellRnti (m_rnti);
            
          // in principle we should wait for contention resolution,
          // but, since we don't model RACH PREAMBLE collisions, we
          // just stop here
          m_cmacSapUser->NotifyRandomAccessSuccessful ();
        }
      // else the RAR is not for me
    }
  else
    {
      NS_LOG_WARN (this << " LteControlMessage not recognized");
    }
}


void
LteUeMac::DoSubframeIndication (uint32_t frameNo, uint32_t subframeNo)
{
  NS_LOG_FUNCTION (this);
  if ((Simulator::Now () >= m_bsrLast + m_bsrPeriodicity) && (m_freshUlBsr==true))
    {
      SendReportBufferStatus ();
      m_bsrLast = Simulator::Now ();
      m_freshUlBsr = false;
    }
}


} // namespace ns3
