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
#include <ns3/ideal-control-messages.h>
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
  virtual void ConfigureUe (uint16_t rnti);
  virtual void AddLc (uint8_t lcId, LteMacSapUser* msu);
  virtual void RemoveLc (uint8_t lcId);
  virtual void RrcUpdateConfigurationReq (LteUeConfig_t params);

private:
  LteUeMac* m_mac;
};


UeMemberLteUeCmacSapProvider::UeMemberLteUeCmacSapProvider (LteUeMac* mac)
  : m_mac (mac)
{
}

void
UeMemberLteUeCmacSapProvider::ConfigureUe (uint16_t rnti)
{
  m_mac->DoConfigureUe (rnti);
}

void
UeMemberLteUeCmacSapProvider::AddLc (uint8_t lcId, LteMacSapUser* msu)
{
  m_mac->DoAddLc (lcId, msu);
}

void
UeMemberLteUeCmacSapProvider::RemoveLc (uint8_t lcid)
{
  m_mac->DoRemoveLc (lcid);
}

void
UeMemberLteUeCmacSapProvider::RrcUpdateConfigurationReq (LteUeConfig_t params)
{
  m_mac->DoRrcUpdateConfigurationReq (params);
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
  virtual void ReceiveIdealControlMessage (Ptr<IdealControlMessage> msg);

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
UeMemberLteUePhySapUser::ReceiveIdealControlMessage (Ptr<IdealControlMessage> msg)
{
  m_mac->DoReceiveIdealControlMessage (msg);
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
  m_freshUlBsr (false)
  
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
  if (m_ulBsrReceived.size () == 0)
    {
      return;  // No BSR report to transmit
    }
  MacCeListElement_s bsr;
  bsr.m_rnti = m_rnti;
  bsr.m_macCeType = MacCeListElement_s::BSR;

  // BSR is reported for each LCG. As a simplification, we consider that all LCs belong to the first LCG.
  std::map <uint8_t, uint64_t>::iterator it;  
  int queue = 0;
  for (it = m_ulBsrReceived.begin (); it != m_ulBsrReceived.end (); it++)
    {
      queue += (*it).second;
  
    }
  int index = BufferSizeLevelBsr::BufferSize2BsrId (queue);
  bsr.m_macCeValue.m_bufferStatus.push_back (index);
  // FF API says that all 4 LCGs are always present
  // we do so but reporting a 0 size for all other LCGs
  bsr.m_macCeValue.m_bufferStatus.push_back (BufferSizeLevelBsr::BufferSize2BsrId (0));
  bsr.m_macCeValue.m_bufferStatus.push_back (BufferSizeLevelBsr::BufferSize2BsrId (0));
  bsr.m_macCeValue.m_bufferStatus.push_back (BufferSizeLevelBsr::BufferSize2BsrId (0));

  // create the feedback to eNB
  Ptr<BsrIdealControlMessage> msg = Create<BsrIdealControlMessage> ();
  msg->SetBsr (bsr);
  m_uePhySapProvider->SendIdealControlMessage (msg);

}

void
LteUeMac::DoConfigureUe (uint16_t rnti)
{
  NS_LOG_FUNCTION (this << " rnti" << rnti);
  m_rnti = rnti;
}

void
LteUeMac::DoAddLc (uint8_t lcId, LteMacSapUser* msu)
{
  NS_LOG_FUNCTION (this << " lcId" << (uint16_t) lcId);
  NS_ASSERT_MSG (m_macSapUserMap.find (lcId) == m_macSapUserMap.end (), "cannot add channel because LCID " << lcId << " is already present");
  m_macSapUserMap[lcId] = msu;
}

void
LteUeMac::DoRemoveLc (uint8_t lcId)
{
  NS_LOG_FUNCTION (this << " lcId" << lcId);
  NS_ASSERT_MSG (m_macSapUserMap.find (lcId) == m_macSapUserMap.end (), "could not find LCID " << lcId);
  m_macSapUserMap.erase (lcId);
}

void
LteUeMac::DoRrcUpdateConfigurationReq (LteUeConfig_t params)
{
  NS_LOG_FUNCTION (this << " txMode " << (uint8_t) params.m_transmissionMode);
  // forward info to PHY layer
  m_uePhySapProvider->SetTransmissionMode (params.m_transmissionMode);
}


void
LteUeMac::DoReceivePhyPdu (Ptr<Packet> p)
{
  LteRadioBearerTag tag;
  p->RemovePacketTag (tag);
  if (tag.GetRnti () == m_rnti)
    {
      // packet is for the current user
      std::map <uint8_t, LteMacSapUser*>::const_iterator it = m_macSapUserMap.find (tag.GetLcid ());
      NS_ASSERT_MSG (it != m_macSapUserMap.end (), "received packet with unknown lcid");
      it->second->ReceivePdu (p);
    }
}


void
LteUeMac::DoReceiveIdealControlMessage (Ptr<IdealControlMessage> msg)
{
  NS_LOG_FUNCTION (this);
  if (msg->GetMessageType () == IdealControlMessage::UL_DCI)
    {
      Ptr<UlDciIdealControlMessage> msg2 = DynamicCast<UlDciIdealControlMessage> (msg);
      UlDciListElement_s dci = msg2->GetDci ();
      std::map <uint8_t, uint64_t>::iterator itBsr;
      NS_ASSERT_MSG (m_ulBsrReceived.size () <=4, " Too many LCs (max is 4)");
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
      std::map <uint8_t, LteMacSapUser*>::iterator it;
      NS_LOG_FUNCTION (this << " UE: UL-CQI notified TxOpportunity of " << dci.m_tbSize);
      for (it = m_macSapUserMap.begin (); it!=m_macSapUserMap.end (); it++)
        {
          itBsr = m_ulBsrReceived.find ((*it).first);
          if (itBsr!=m_ulBsrReceived.end ())
            {
              NS_LOG_FUNCTION (this << "\t" << dci.m_tbSize / m_macSapUserMap.size () << " bytes to LC " << (uint16_t)(*it).first << " queue " << (*itBsr).second);
              (*it).second->NotifyTxOpportunity (dci.m_tbSize / activeLcs, 0);
              if ((*itBsr).second >=  static_cast<uint64_t> (dci.m_tbSize / activeLcs))
                {
                  (*itBsr).second -= dci.m_tbSize / activeLcs;
                }
              else
                {
                  (*itBsr).second = 0;
                }
            }
        }

    }
  else
    {
      NS_LOG_FUNCTION (this << " IdealControlMessage not recognized");
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
