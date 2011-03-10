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
#include "enb-phy.h"
#include "lte-net-device.h"
#include "lte-spectrum-value-helper.h"
#include "ideal-control-messages.h"
#include "enb-net-device.h"
#include "lte-enb-mac.h"


NS_LOG_COMPONENT_DEFINE ("EnbLtePhy");

namespace ns3 {


////////////////////////////////////////
// member SAP forwarders
////////////////////////////////////////


class EnbMemberLteEnbPhySapProvider : public LteEnbPhySapProvider
{
public:
  EnbMemberLteEnbPhySapProvider (EnbLtePhy* phy);

  // inherited from LteEnbPhySapProvider
  virtual void SendMacPdu (Ptr<Packet> p);
  virtual void SetBandwidth (uint8_t ulBandwidth, uint8_t dlBandwidth);
  virtual void SetCellId (uint16_t cellId);
  virtual void SendIdealControlMessage (Ptr<IdealControlMessage> msg);

private:
  EnbLtePhy* m_phy;
};

EnbMemberLteEnbPhySapProvider::EnbMemberLteEnbPhySapProvider (EnbLtePhy* phy) : m_phy (phy)
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
// generic EnbLtePhy methods
////////////////////////////////////////



NS_OBJECT_ENSURE_REGISTERED (EnbLtePhy);


EnbLtePhy::EnbLtePhy ()
  : m_nrFrames (0),
    m_nrSubFrames (0)
{
  m_enbPhySapProvider = new EnbMemberLteEnbPhySapProvider (this);
}


TypeId
EnbLtePhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EnbLtePhy")
    .SetParent<LtePhy> ()
    .AddConstructor<EnbLtePhy> ()
  ;
  return tid;
}


EnbLtePhy::~EnbLtePhy ()
{
}

void
EnbLtePhy::SetLteEnbPhySapUser (LteEnbPhySapUser* s)
{
  m_enbPhySapUser = s;
}

LteEnbPhySapProvider*
EnbLtePhy::GetLteEnbPhySapProvider ()
{
  return (m_enbPhySapProvider);
}



bool
EnbLtePhy::AddUePhy (uint8_t rnti, Ptr<UeLtePhy> phy)
{
  std::map <uint8_t, Ptr<UeLtePhy> >::iterator it;
  it = m_ueAttached.find (rnti);
  if (it == m_ueAttached.end ())
    {
      m_ueAttached.insert (std::pair<uint8_t, Ptr<UeLtePhy> > (rnti, phy));
      return (true);
    }
  else
    {
      NS_LOG_ERROR ("UE already attached");
      return (false);
    }
}

bool
EnbLtePhy::DeleteUePhy (uint8_t rnti)
{
  std::map <uint8_t, Ptr<UeLtePhy> >::iterator it;
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
EnbLtePhy::DoSendMacPdu (Ptr<Packet> p)
{
//   NS_LOG_FUNCTION (this << pb->GetNPackets () << pb->GetSize ());
//   return GetDownlinkSpectrumPhy ()->StartTx (pb);

  NS_LOG_FUNCTION (this);
  SetMacPdu (p);
}


void
EnbLtePhy::PhyPduReceived (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
  m_enbPhySapUser->ReceivePhyPdu (p);
}

void
EnbLtePhy::DoSetDownlinkSubChannels ()
{
  NS_LOG_FUNCTION (this);
  Ptr<SpectrumValue> txPsd = CreateTxPowerSpectralDensity ();
  GetDownlinkSpectrumPhy ()->SetTxPowerSpectralDensity (txPsd);
}


Ptr<SpectrumValue>
EnbLtePhy::CreateTxPowerSpectralDensity ()
{
  NS_LOG_FUNCTION (this);

  LteSpectrumValueHelper psdHelper;
  Ptr<SpectrumValue> psd = psdHelper.CreateDownlinkTxPowerSpectralDensity (GetTxPower (), GetDownlinkSubChannels ());

  return psd;
}


void
EnbLtePhy::CalcChannelQualityForUe (std::vector <double> sinr, Ptr<LteSpectrumPhy> ue)
{
  NS_LOG_FUNCTION (this);
}


void
EnbLtePhy::DoSendIdealControlMessage (Ptr<IdealControlMessage> msg)
{
  NS_LOG_FUNCTION (this << msg);
  // queues the message (wait for MAC-PHY delay)
  SetControlMessages (msg);
}



void
EnbLtePhy::ReceiveIdealControlMessage (Ptr<IdealControlMessage> msg)
{
  NS_LOG_FUNCTION (this << msg);
  m_enbPhySapUser->ReceiveIdealControlMessage (msg);
}



void
EnbLtePhy::StartFrame (void)
{
  NS_LOG_FUNCTION (this);

  ++m_nrFrames;
  NS_LOG_INFO ("-----frame " << m_nrFrames << "-----");
  m_nrSubFrames = 0;
  StartSubFrame ();
}


void
EnbLtePhy::StartSubFrame (void)
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
              std::map <uint8_t, Ptr<UeLtePhy> >::iterator it2;
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
              std::map <uint8_t, Ptr<UeLtePhy> >::iterator it2;
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
      GetDownlinkSpectrumPhy ()->StartTx (pb);
    }

  // trigger the MAC
  Ptr<LteEnbMac> macEntity = GetDevice ()->GetObject<EnbNetDevice> ()->GetMac ();

  m_enbPhySapUser->SubframeIndication (m_nrFrames, m_nrSubFrames);
  
  
  // trigger the UE(s)
  std::map <uint8_t, Ptr<UeLtePhy> >::iterator it;
  for (it = m_ueAttached.begin (); it != m_ueAttached.end (); it++)
    {
      (*it).second->SubframeIndication (m_nrFrames, m_nrSubFrames);
    }

  Simulator::Schedule (Seconds (GetTti ()),
                       &EnbLtePhy::EndSubFrame,
                       this);

}


void
EnbLtePhy::EndSubFrame (void)
{
  NS_LOG_FUNCTION (this << Simulator::Now ().GetSeconds ());
  if (m_nrSubFrames == 10)
    {
      Simulator::ScheduleNow (&EnbLtePhy::EndFrame, this);
    }
  else
    {
      Simulator::ScheduleNow (&EnbLtePhy::StartSubFrame, this);
    }
}


void
EnbLtePhy::EndFrame (void)
{
  NS_LOG_FUNCTION (this << Simulator::Now ().GetSeconds ());
  Simulator::ScheduleNow (&EnbLtePhy::StartFrame, this);
}


void 
EnbLtePhy::GenerateCqiFeedback (const SpectrumValue& sinr)
{
  NS_LOG_FUNCTION (this << sinr);
}

};
