/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 *         Marco Miozzo <marco.miozzo@cttc.es>
 *         Nicola Baldo <nbaldo@cttc.es>
 */

#include <ns3/object-factory.h>
#include <ns3/log.h>
#include <math.h>
#include <ns3/simulator.h>
#include <ns3/spectrum-error-model.h>
#include "ue-phy.h"
#include "enb-phy.h"
#include "lte-net-device.h"
#include "ue-net-device.h"
#include "enb-net-device.h"
#include "lte-spectrum-value-helper.h"
#include "amc-module.h"
#include "lte-ue-mac.h"
#include "ff-mac-common.h"
#include "lte-sinr-chunk-processor.h"


NS_LOG_COMPONENT_DEFINE ("UeLtePhy");

namespace ns3 {

////////////////////////////////////////
// member SAP forwarders
////////////////////////////////////////


class UeMemberUeLtePhySapProvider : public LteUePhySapProvider
{
public:
  UeMemberUeLtePhySapProvider (UeLtePhy* phy);

  // inherited from LtePhySapProvider
  virtual void SendMacPdu (Ptr<Packet> p);
  virtual void SetBandwidth (uint8_t ulBandwidth, uint8_t dlBandwidth);
  virtual void SendIdealControlMessage (Ptr<IdealControlMessage> msg);

private:
  UeLtePhy* m_phy;
};

UeMemberUeLtePhySapProvider::UeMemberUeLtePhySapProvider (UeLtePhy* phy) : m_phy (phy)
{

}


void
UeMemberUeLtePhySapProvider::SendMacPdu (Ptr<Packet> p)
{
  m_phy->DoSendMacPdu (p);
}

void
UeMemberUeLtePhySapProvider::SetBandwidth (uint8_t ulBandwidth, uint8_t dlBandwidth)
{
  m_phy->DoSetBandwidth (ulBandwidth, dlBandwidth);
}

void
UeMemberUeLtePhySapProvider::SendIdealControlMessage (Ptr<IdealControlMessage> msg)
{
  m_phy->DoSendIdealControlMessage (msg);
}



////////////////////////////////////////
// generic UeLtePhy methods
////////////////////////////////////////


NS_OBJECT_ENSURE_REGISTERED (UeLtePhy);


UeLtePhy::UeLtePhy ()
  : m_p10CqiPeriocity (MilliSeconds (2)),    
    m_p10CqiLast (MilliSeconds (0))
{
  m_uePhySapProvider = new UeMemberUeLtePhySapProvider (this);
}


UeLtePhy::~UeLtePhy ()
{
}

void
UeLtePhy::DoDispose ()
{
}
  


TypeId
UeLtePhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UeLtePhy")
    .SetParent<LtePhy> ()
    .AddConstructor<UeLtePhy> ()
  ;
  return tid;
}


void
UeLtePhy::SetLteUePhySapUser (LteUePhySapUser* s)
{
  m_uePhySapUser = s;
}

LteUePhySapProvider*
UeLtePhy::GetLteUePhySapProvider ()
{
  return (m_uePhySapProvider);
}


void
UeLtePhy::DoSendMacPdu (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);

  SetMacPdu (p);
}


void
UeLtePhy::PhyPduReceived (Ptr<Packet> p)
{
  m_uePhySapUser->ReceivePhyPdu (p);
}

void
UeLtePhy::DoSetUplinkSubChannels ()
{
  NS_LOG_FUNCTION (this);

  /*
   *  XXX: the uplink scheduler is not implemented yet!
   *  Now, all uplink sub channels can be used for uplink transmission
   */
  SetSubChannelsForTransmission (GetUplinkSubChannels ());
}


void
UeLtePhy::SetSubChannelsForTransmission (std::vector <int> mask)
{
  NS_LOG_FUNCTION (this);

  m_subChannelsForTransmission = mask;

  Ptr<SpectrumValue> txPsd = CreateTxPowerSpectralDensity ();
  GetUplinkSpectrumPhy ()->SetTxPowerSpectralDensity (txPsd);
}


void
UeLtePhy::SetSubChannelsForReception (std::vector <int> mask)
{
  NS_LOG_FUNCTION (this);
  m_subChannelsForReception = mask;
}


std::vector <int>
UeLtePhy::GetSubChannelsForTransmission ()
{
  NS_LOG_FUNCTION (this);
  return m_subChannelsForTransmission;
}


std::vector <int>
UeLtePhy::GetSubChannelsForReception ()
{
  NS_LOG_FUNCTION (this);
  return m_subChannelsForReception;
}


Ptr<SpectrumValue>
UeLtePhy::CreateTxPowerSpectralDensity ()
{
  NS_LOG_FUNCTION (this);
  LteSpectrumValueHelper psdHelper;
  Ptr<SpectrumValue> psd = psdHelper.CreateUplinkTxPowerSpectralDensity (GetTxPower (), GetSubChannelsForTransmission ());

  return psd;
}

void
UeLtePhy::GenerateCqiFeedback (const SpectrumValue& sinr)
{
  NS_LOG_FUNCTION (this);
  if (Simulator::Now () > m_p10CqiLast + m_p10CqiPeriocity)
    {
      Ptr<UeNetDevice> thisDevice = GetDevice ()->GetObject<UeNetDevice> ();
      Ptr<DlCqiIdealControlMessage> msg = CreateDlCqiFeedbackMessage (sinr);
      DoSendIdealControlMessage (msg);
      m_p10CqiLast = Simulator::Now ();
    }
}



Ptr<DlCqiIdealControlMessage>
UeLtePhy::CreateDlCqiFeedbackMessage (const SpectrumValue& sinr)
{
  NS_LOG_FUNCTION (this);

  std::vector<int> cqi = AmcModule::CreateCqiFeedbacks (sinr);

  // CREATE CqiIdealControlMessage
  Ptr<DlCqiIdealControlMessage> msg = Create<DlCqiIdealControlMessage> ();

  int nbSubChannels = cqi.size ();
  double cqiSum = 0.0;
  for (int i = 0; i < nbSubChannels; i++)
    {
      cqiSum += cqi.at (i);
    }
  CqiListElement_s dlcqi;
  dlcqi.m_rnti = m_rnti;
  dlcqi.m_ri = 1; // not yet used
  dlcqi.m_cqiType = CqiListElement_s::P10; // Peridic CQI using PUCCH wideband
  dlcqi.m_wbCqi.push_back ((uint16_t) cqiSum / nbSubChannels);
  dlcqi.m_wbPmi = 0; // not yet used
  // dl.cqi.m_sbMeasResult others CQI report modes: not yet implemented

  msg->SetDlCqi (dlcqi);
  return msg;
}



void
UeLtePhy::DoSendIdealControlMessage (Ptr<IdealControlMessage> msg)
{
  NS_LOG_FUNCTION (this << msg);
  Ptr<UeNetDevice> thisDevice = GetDevice ()->GetObject<UeNetDevice> ();
  Ptr<EnbNetDevice> remoteDevice = thisDevice->GetTargetEnb ();
  msg->SetSourceDevice (thisDevice);
  msg->SetDestinationDevice (remoteDevice);
  SetControlMessages (msg);
  Ptr<LtePhy> phy = remoteDevice->GetPhy ();
  phy->ReceiveIdealControlMessage (msg);
}


void
UeLtePhy::ReceiveIdealControlMessage (Ptr<IdealControlMessage> msg)
{
  NS_LOG_FUNCTION (this << msg);

  if (msg->GetMessageType () == IdealControlMessage::DL_DCI)
    {
      Ptr<DlDciIdealControlMessage> msg2 = DynamicCast<DlDciIdealControlMessage> (msg);

      DlDciListElement_s dci = msg2->GetDci ();

      if (dci.m_resAlloc != 0)
        {
          NS_FATAL_ERROR ("Resource Allocation type not implemented");
        }

      std::vector <int> dlRb;

      // translate the DCI to Spectrum framework
      uint32_t mask = 0x1;
      for (int i = 0; i < 32; i++)
        {
          if (((dci.m_rbBitmap & mask) >> i) == 1)
            {
              for (int k = 0; k < GetRbgSize (); k++)
                {
                  dlRb.push_back ((i * GetRbgSize ()) + k);
                  //NS_LOG_DEBUG(this << "DL-DCI allocated PRB " << (i*GetRbgSize()) + k);
                }
            }
          mask = (mask << 1);
        }

      SetSubChannelsForReception (dlRb);


    }
  else if (msg->GetMessageType () == IdealControlMessage::UL_DCI) 
    {
      // set the uplink bandwidht according to the UL-CQI
      Ptr<UlDciIdealControlMessage> msg2 = DynamicCast<UlDciIdealControlMessage> (msg);
      UlDciListElement_s dci = msg2->GetDci ();
      std::vector <int> ulRb;
      for (int i = 0; i < dci.m_rbLen; i++)
        {
          ulRb.push_back (i + dci.m_rbStart);
          //NS_LOG_DEBUG (this << " UE RB " << i + dci.m_rbStart);
        }
      SetSubChannelsForTransmission (ulRb);
      // pass the info to the MAC
      m_uePhySapUser->ReceiveIdealControlMessage (msg);
    }
  else
    {
      // pass the message to UE-MAC
      m_uePhySapUser->ReceiveIdealControlMessage (msg);
    }


}


void
UeLtePhy::SubframeIndication (uint32_t frameNo, uint32_t subframeNo)
{
  // trigger from eNB
  
  // send control messages
  std::list<Ptr<IdealControlMessage> > ctrlMsg = GetControlMessages ();
  if (ctrlMsg.size () > 0)
    {
      Ptr<LtePhy> phy = GetDevice ()->GetObject<UeNetDevice> ()->GetTargetEnb ()->GetPhy ();
      std::list<Ptr<IdealControlMessage> >::iterator it;
      it = ctrlMsg.begin ();
      while (it != ctrlMsg.end ())
        {
          Ptr<IdealControlMessage> msg = (*it);
          phy->ReceiveIdealControlMessage (msg);
          ctrlMsg.pop_front ();
          it = ctrlMsg.begin ();
        }
    }
  
  // send packets in queue
  // send the current burts of packets
  Ptr<PacketBurst> pb = GetPacketBurst ();
  if (pb)
    {
      GetUplinkSpectrumPhy ()->StartTx (pb);
    }
  
}


void
UeLtePhy::SetTargetEnb (Ptr<EnbLtePhy> enbPhy)
{
  m_targetEnbPhy = enbPhy;
}



void
UeLtePhy::SetRnti (uint16_t rnti)
{
  NS_LOG_FUNCTION (this << rnti);
  m_rnti = rnti;
}



} // namespace ns3
