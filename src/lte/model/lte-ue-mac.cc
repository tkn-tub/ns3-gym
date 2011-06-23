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
#include "lte-mac-tag.h"
#include <ns3/ff-mac-common.h>
#include <ns3/ideal-control-messages.h>


NS_LOG_COMPONENT_DEFINE ("LteUeMac");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LteUeMac);



int BufferSizeLevelBsr[64] = {

  0, 10, 12, 14, 17, 19, 22, 26, 31, 36, 42, 49, 57, 67, 78, 91, 107, 125, 146,
  171, 200, 234, 274, 321, 376, 440, 515, 603, 706, 826, 967, 1132, 1326, 1552,
  1817, 2127, 2490, 2915, 3413, 3995, 4677, 5476, 6411, 7505, 8787, 10287,
  12043, 14099, 16507, 19325, 22624, 26487, 31009, 36304, 42502, 49759, 58255,
  68201, 79846, 93749, 109439, 128125, 150000, 150000

};


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
  NS_LOG_LOGIC (this << " UE-MAC does not yet support this primitive");
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
  LteMacTag tag (params.rnti, params.lcid);
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
  MacCeListElement_s bsr;
  bsr.m_rnti = m_rnti;
  bsr.m_macCeType = MacCeListElement_s::BSR;
  // short BSR
  int queue = params.txQueueSize;
  int index = 0;
  if (BufferSizeLevelBsr[63] < queue)
    {
      index = 63;
    }
  else
    {
      while (BufferSizeLevelBsr[index] < queue)
        {
          index++;
        }
    }
  bsr.m_macCeValue.m_bufferStatus.push_back (index);

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
  NS_LOG_FUNCTION (this << " lcId" << lcId);
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
LteUeMac::DoReceivePhyPdu (Ptr<Packet> p)
{
  LteMacTag tag;
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
      std::map <uint8_t, LteMacSapUser*>::iterator it; 
      it = m_macSapUserMap.begin ();  // use only the first LC --> UE-SCHEDULER??
      (*it).second->NotifyTxOpportunity (dci.m_tbSize);

    }
  else
    {
      NS_LOG_FUNCTION (this << " IdealControlMessage not recognized");
    }
}


} // namespace ns3
