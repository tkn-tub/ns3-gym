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
 *         Marco Miozzo <mmiozzo@cttc.es>
 */

#include <ns3/object-factory.h>
#include <ns3/log.h>
#include <math.h>
#include <ns3/simulator.h>
#include "ns3/spectrum-error-model.h"
#include "lte-enb-phy.h"
#include "lte-net-device.h"
#include "lte-spectrum-value-helper.h"
#include "ideal-control-messages.h"
#include "lte-enb-net-device.h"
#include "lte-enb-mac.h"


NS_LOG_COMPONENT_DEFINE ("LteEnbPhy");

namespace ns3 {


////////////////////////////////////////
// member SAP forwarders
////////////////////////////////////////


class EnbMemberLteEnbPhySapProvider : public LteEnbPhySapProvider
{
public:
  EnbMemberLteEnbPhySapProvider (LteEnbPhy* phy);

  // inherited from LteEnbPhySapProvider
  virtual void SendMacPdu (Ptr<Packet> p);
  virtual void SetBandwidth (uint8_t ulBandwidth, uint8_t dlBandwidth);
  virtual void SetCellId (uint16_t cellId);
  virtual void SendIdealControlMessage (Ptr<IdealControlMessage> msg);

private:
  LteEnbPhy* m_phy;
};

EnbMemberLteEnbPhySapProvider::EnbMemberLteEnbPhySapProvider (LteEnbPhy* phy) : m_phy (phy)
{

}


void
EnbMemberLteEnbPhySapProvider::SendMacPdu (Ptr<Packet> p)
{
  m_phy->DoSendMacPdu (p);
}

void
EnbMemberLteEnbPhySapProvider::SetBandwidth (uint8_t ulBandwidth, uint8_t dlBandwidth)
{
  m_phy->DoSetBandwidth (ulBandwidth, dlBandwidth);
}

void
EnbMemberLteEnbPhySapProvider::SetCellId (uint16_t cellId)
{
  m_phy->DoSetCellId (cellId);
}

void
EnbMemberLteEnbPhySapProvider::SendIdealControlMessage (Ptr<IdealControlMessage> msg)
{
  m_phy->DoSendIdealControlMessage (msg);
}


////////////////////////////////////////
// generic LteEnbPhy methods
////////////////////////////////////////



NS_OBJECT_ENSURE_REGISTERED (LteEnbPhy);


LteEnbPhy::LteEnbPhy ()
  : m_nrFrames (0),
    m_nrSubFrames (0)
{
  m_enbPhySapProvider = new EnbMemberLteEnbPhySapProvider (this);
}


TypeId
LteEnbPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteEnbPhy")
    .SetParent<LtePhy> ()
    .AddConstructor<LteEnbPhy> ()
  ;
  return tid;
}


LteEnbPhy::~LteEnbPhy ()
{
}

void
LteEnbPhy::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_ueAttached.clear ();
  LtePhy::DoDispose ();
}

void
LteEnbPhy::SetLteEnbPhySapUser (LteEnbPhySapUser* s)
{
  m_enbPhySapUser = s;
}

LteEnbPhySapProvider*
LteEnbPhy::GetLteEnbPhySapProvider ()
{
  return (m_enbPhySapProvider);
}



bool
LteEnbPhy::AddUePhy (uint8_t rnti, Ptr<LteUePhy> phy)
{
  std::map <uint8_t, Ptr<LteUePhy> >::iterator it;
  it = m_ueAttached.find (rnti);
  if (it == m_ueAttached.end ())
    {
      m_ueAttached.insert (std::pair<uint8_t, Ptr<LteUePhy> > (rnti, phy));
      return (true);
    }
  else
    {
      NS_LOG_ERROR ("UE already attached");
      return (false);
    }
}

bool
LteEnbPhy::DeleteUePhy (uint8_t rnti)
{
  std::map <uint8_t, Ptr<LteUePhy> >::iterator it;
  it = m_ueAttached.find (rnti);
  if (it == m_ueAttached.end ())
    {
      NS_LOG_ERROR ("UE not attached");
      return (false);
    }
  else
    {
      m_ueAttached.erase (it);
      return (true);
    }
}



void
LteEnbPhy::DoSendMacPdu (Ptr<Packet> p)
{
//   NS_LOG_FUNCTION (this << pb->GetNPackets () << pb->GetSize ());
//   return GetDownlinkSpectrumPhy ()->StartTx (pb);

  NS_LOG_FUNCTION (this);
  SetMacPdu (p);
}


void
LteEnbPhy::PhyPduReceived (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
  m_enbPhySapUser->ReceivePhyPdu (p);
}

void
LteEnbPhy::DoSetDownlinkSubChannels ()
{
  NS_LOG_FUNCTION (this);
  Ptr<SpectrumValue> txPsd = CreateTxPowerSpectralDensity ();
  m_downlinkSpectrumPhy->SetTxPowerSpectralDensity (txPsd);
}


Ptr<SpectrumValue>
LteEnbPhy::CreateTxPowerSpectralDensity ()
{
  NS_LOG_FUNCTION (this);

  LteSpectrumValueHelper psdHelper;
  Ptr<SpectrumValue> psd = psdHelper.CreateDownlinkTxPowerSpectralDensity (GetTxPower (), GetDownlinkSubChannels ());

  return psd;
}


void
LteEnbPhy::CalcChannelQualityForUe (std::vector <double> sinr, Ptr<LteSpectrumPhy> ue)
{
  NS_LOG_FUNCTION (this);
}


void
LteEnbPhy::DoSendIdealControlMessage (Ptr<IdealControlMessage> msg)
{
  NS_LOG_FUNCTION (this << msg);
  // queues the message (wait for MAC-PHY delay)
  SetControlMessages (msg);
}



void
LteEnbPhy::ReceiveIdealControlMessage (Ptr<IdealControlMessage> msg)
{
  NS_LOG_FUNCTION (this << msg);
  m_enbPhySapUser->ReceiveIdealControlMessage (msg);
}



void
LteEnbPhy::StartFrame (void)
{
  NS_LOG_FUNCTION (this);

  ++m_nrFrames;
  NS_LOG_INFO ("-----frame " << m_nrFrames << "-----");
  m_nrSubFrames = 0;
  StartSubFrame ();
}


void
LteEnbPhy::StartSubFrame (void)
{
  NS_LOG_FUNCTION (this);

  ++m_nrSubFrames;
  NS_LOG_INFO ("-----sub frame " << m_nrSubFrames << "-----");
  
  // send the current burst of control messages
  std::list<Ptr<IdealControlMessage> > ctrlMsg = GetControlMessages ();
  std::vector <int> dlRb;
  if (ctrlMsg.size () > 0)
    {
      std::list<Ptr<IdealControlMessage> >::iterator it;
      it = ctrlMsg.begin ();
      while (it != ctrlMsg.end ())
        {
          Ptr<IdealControlMessage> msg = (*it);
          if (msg->GetMessageType () == IdealControlMessage::DL_DCI)
            {
              std::map <uint8_t, Ptr<LteUePhy> >::iterator it2;
              Ptr<DlDciIdealControlMessage> dci = DynamicCast<DlDciIdealControlMessage> (msg);
              it2 = m_ueAttached.find (dci->GetDci ().m_rnti);
              
              if (it2 == m_ueAttached.end ())
                {
                  NS_LOG_ERROR ("UE not attached");
                }
              else
                {
                  // get the tx power spectral density according to DL-DCI(s)
                  // translate the DCI to Spectrum framework
                  uint32_t mask = 0x1;
                  for (int i = 0; i < 32; i++)
                    {
                      if (((dci->GetDci ().m_rbBitmap & mask) >> i) == 1)
                        {
                          for (int k = 0; k < GetRbgSize (); k++)
                            {
                              dlRb.push_back ((i * GetRbgSize ()) + k);
                              //NS_LOG_DEBUG(this << " [enb]DL-DCI allocated PRB " << (i*GetRbgSize()) + k);
                            }
                        }
                      mask = (mask << 1);
                    }
                  (*it2).second->ReceiveIdealControlMessage (msg);
                }
            }
          else if (msg->GetMessageType () == IdealControlMessage::UL_DCI)
            {
              std::map <uint8_t, Ptr<LteUePhy> >::iterator it2;
              Ptr<UlDciIdealControlMessage> dci = DynamicCast<UlDciIdealControlMessage> (msg);
              it2 = m_ueAttached.find (dci->GetDci ().m_rnti);
             
              if (it2 == m_ueAttached.end ())
              {
                NS_LOG_ERROR ("UE not attached");
              }
              else
              {
                (*it2).second->ReceiveIdealControlMessage (msg);
              }
            }
          ctrlMsg.pop_front ();
          it = ctrlMsg.begin ();
        }
    }
  // set the current tx power spectral density
  SetDownlinkSubChannels (dlRb);
  // send the current burts of packets
  Ptr<PacketBurst> pb = GetPacketBurst ();
  if (pb)
    {
      m_downlinkSpectrumPhy->StartTx (pb);
    }

  // trigger the MAC
  Ptr<LteEnbMac> macEntity = GetDevice ()->GetObject<LteEnbNetDevice> ()->GetMac ();

  m_enbPhySapUser->SubframeIndication (m_nrFrames, m_nrSubFrames);
  
  
  // trigger the UE(s)
  std::map <uint8_t, Ptr<LteUePhy> >::iterator it;
  for (it = m_ueAttached.begin (); it != m_ueAttached.end (); it++)
    {
      (*it).second->SubframeIndication (m_nrFrames, m_nrSubFrames);
    }

  Simulator::Schedule (Seconds (GetTti ()),
                       &LteEnbPhy::EndSubFrame,
                       this);

}


void
LteEnbPhy::EndSubFrame (void)
{
  NS_LOG_FUNCTION (this << Simulator::Now ().GetSeconds ());
  if (m_nrSubFrames == 10)
    {
      Simulator::ScheduleNow (&LteEnbPhy::EndFrame, this);
    }
  else
    {
      Simulator::ScheduleNow (&LteEnbPhy::StartSubFrame, this);
    }
}


void
LteEnbPhy::EndFrame (void)
{
  NS_LOG_FUNCTION (this << Simulator::Now ().GetSeconds ());
  Simulator::ScheduleNow (&LteEnbPhy::StartFrame, this);
}


void 
LteEnbPhy::GenerateCqiFeedback (const SpectrumValue& sinr)
{
  NS_LOG_FUNCTION (this << sinr);
    Ptr<LteEnbNetDevice> thisDevice = GetDevice ()->GetObject<LteEnbNetDevice> ();
    Ptr<UlCqiIdealControlMessage> msg = CreateUlCqiFeedbackMessage (sinr);
    m_enbPhySapUser->ReceiveIdealControlMessage (msg);
  
  
}


Ptr<UlCqiIdealControlMessage> 
LteEnbPhy::CreateUlCqiFeedbackMessage (const SpectrumValue& sinr)
{
	NS_LOG_FUNCTION (this << sinr);
  Values::const_iterator it;
  UlCqi_s ulcqi;
  ulcqi.m_type = UlCqi_s::PUSCH;
  int i = 0;
  for (it = sinr.ConstValuesBegin (); it != sinr.ConstValuesEnd (); it++)
  {
    ulcqi.m_sinr.push_back ((*it));
    NS_LOG_DEBUG(this << " RB " << i << " SINR " << (*it));
    i++;
  }
  Ptr<UlCqiIdealControlMessage> msg = Create<UlCqiIdealControlMessage> ();
  msg->SetUlCqi (ulcqi);
  return (msg);
	
}

};
