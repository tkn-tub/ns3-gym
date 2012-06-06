/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include <ns3/attribute-accessor-helper.h>
#include <ns3/double.h>


#include "lte-enb-phy.h"
#include "lte-net-device.h"
#include "lte-spectrum-value-helper.h"
#include "ideal-control-messages.h"
#include "lte-enb-net-device.h"
#include "lte-enb-mac.h"
#include <ns3/lte-common.h>


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
  virtual uint8_t GetMacChTtiDelay ();
  virtual void SetTransmissionMode (uint16_t  rnti, uint8_t txMode);
  

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

uint8_t
EnbMemberLteEnbPhySapProvider::GetMacChTtiDelay ()
{
  return (m_phy->DoGetMacChTtiDelay ());
}

void
EnbMemberLteEnbPhySapProvider::SetTransmissionMode (uint16_t  rnti, uint8_t txMode)
{
  m_phy->DoSetTransmissionMode (rnti, txMode);
}


////////////////////////////////////////
// generic LteEnbPhy methods
////////////////////////////////////////



NS_OBJECT_ENSURE_REGISTERED (LteEnbPhy);


LteEnbPhy::LteEnbPhy ()
{
  NS_LOG_FUNCTION (this);
  NS_FATAL_ERROR ("This constructor should not be called");
}

LteEnbPhy::LteEnbPhy (Ptr<LteSpectrumPhy> dlPhy, Ptr<LteSpectrumPhy> ulPhy)
  : LtePhy (dlPhy, ulPhy),
    m_nrFrames (0),
    m_nrSubFrames (0)
{
  m_enbPhySapProvider = new EnbMemberLteEnbPhySapProvider (this);
  Simulator::ScheduleNow (&LteEnbPhy::StartFrame, this);
}

TypeId
LteEnbPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteEnbPhy")
    .SetParent<LtePhy> ()
    .AddConstructor<LteEnbPhy> ()
    .AddAttribute ("TxPower",
                   "Transmission power in dBm",
                   DoubleValue (30.0),
                   MakeDoubleAccessor (&LteEnbPhy::SetTxPower, 
                                       &LteEnbPhy::GetTxPower),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("NoiseFigure",
                   "Loss (dB) in the Signal-to-Noise-Ratio due to non-idealities in the receiver."
                   " According to Wikipedia (http://en.wikipedia.org/wiki/Noise_figure), this is "
                   "\"the difference in decibels (dB) between"
                   " the noise output of the actual receiver to the noise output of an "
                   " ideal receiver with the same overall gain and bandwidth when the receivers "
                   " are connected to sources at the standard noise temperature T0.\" "
                   "In this model, we consider T0 = 290K.",
                   DoubleValue (5.0),
                   MakeDoubleAccessor (&LteEnbPhy::SetNoiseFigure, 
                                       &LteEnbPhy::GetNoiseFigure),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MacToChannelDelay",
                   "The delay in TTI units that occurs between a scheduling decision in the MAC and the actual start of the transmission by the PHY. This is intended to be used to model the latency of real PHY and MAC implementations.",
                   UintegerValue (2),
                   MakeUintegerAccessor (&LteEnbPhy::SetMacChDelay, 
                                         &LteEnbPhy::GetMacChDelay),
                   MakeUintegerChecker<uint8_t> ())
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
  delete m_enbPhySapProvider;
  LtePhy::DoDispose ();
}

void
LteEnbPhy::DoStart ()
{
  NS_LOG_FUNCTION (this);
  Ptr<SpectrumValue> noisePsd = LteSpectrumValueHelper::CreateNoisePowerSpectralDensity (m_ulEarfcn, m_ulBandwidth, m_noiseFigure);
  m_uplinkSpectrumPhy->SetNoisePowerSpectralDensity (noisePsd);
  LtePhy::DoStart ();
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

void
LteEnbPhy::SetTxPower (double pow)
{
  NS_LOG_FUNCTION (this << pow);
  m_txPower = pow;
}

double
LteEnbPhy::GetTxPower () const
{
  NS_LOG_FUNCTION (this);
  return m_txPower;
}

void
LteEnbPhy::SetNoiseFigure (double nf)
{
  NS_LOG_FUNCTION (this << nf);
  m_noiseFigure = nf;
}

double
LteEnbPhy::GetNoiseFigure () const
{
  NS_LOG_FUNCTION (this);
  return m_noiseFigure;
}

void
LteEnbPhy::SetMacChDelay (uint8_t delay)
{
  m_macChTtiDelay = delay;
  for (int i = 0; i < m_macChTtiDelay; i++)
    {
      Ptr<PacketBurst> pb = CreateObject <PacketBurst> ();
      m_packetBurstQueue.push_back (pb);
      std::list<Ptr<IdealControlMessage> > l;
      m_controlMessagesQueue.push_back (l);
      std::list<UlDciIdealControlMessage> l1;
      m_ulDciQueue.push_back (l1);
    }
  for (int i = 0; i < UL_PUSCH_TTIS_DELAY; i++)
    {
      std::list<UlDciIdealControlMessage> l1;
      m_ulDciQueue.push_back (l1);
    }
}

uint8_t
LteEnbPhy::GetMacChDelay (void) const
{
  return (m_macChTtiDelay);
}

bool
LteEnbPhy::AddUePhy (uint16_t rnti, Ptr<LteUePhy> phy)
{
  std::map <uint16_t, Ptr<LteUePhy> >::iterator it;
  it = m_ueAttached.find (rnti);
  if (it == m_ueAttached.end ())
    {
      m_ueAttached.insert (std::pair<uint16_t, Ptr<LteUePhy> > (rnti, phy));
      return (true);
    }
  else
    {
      NS_LOG_ERROR ("UE already attached");
      return (false);
    }
}

bool
LteEnbPhy::DeleteUePhy (uint16_t rnti)
{
  std::map <uint16_t, Ptr<LteUePhy> >::iterator it;
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
  NS_LOG_FUNCTION (this);
  SetMacPdu (p);
}

uint8_t
LteEnbPhy::DoGetMacChTtiDelay ()
{
  return (m_macChTtiDelay);
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

  Ptr<SpectrumValue> psd = LteSpectrumValueHelper::CreateTxPowerSpectralDensity (m_dlEarfcn, m_dlBandwidth, m_txPower, GetDownlinkSubChannels ());

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
  
  // update info on TB to be received
  std::list<UlDciIdealControlMessage> uldcilist = DequeueUlDci ();
  std::list<UlDciIdealControlMessage>::iterator dciIt = uldcilist.begin ();
  for (dciIt = uldcilist.begin (); dciIt!=uldcilist.end (); dciIt++)
  {
    std::map <uint16_t, Ptr<LteUePhy> >::iterator it2;
    it2 = m_ueAttached.find ((*dciIt).GetDci ().m_rnti);
    
    if (it2 == m_ueAttached.end ())
    {
      NS_LOG_ERROR ("UE not attached");
    }
    else
    {
      // send info of TB to LteSpectrumPhy 
      // translate to allocation map
      std::vector <int> rbMap;
      for (int i = (*dciIt).GetDci ().m_rbStart; i < (*dciIt).GetDci ().m_rbStart + (*dciIt).GetDci ().m_rbLen; i++)
      {
        rbMap.push_back (i);
      }
      m_uplinkSpectrumPhy->AddExpectedTb ((*dciIt).GetDci ().m_rnti, (*dciIt).GetDci ().m_tbSize, (*dciIt).GetDci ().m_mcs, rbMap, 0 /* always SISO*/);
    }
  }

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
              std::map <uint16_t, Ptr<LteUePhy> >::iterator it2;
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
              std::map <uint16_t, Ptr<LteUePhy> >::iterator it2;
              Ptr<UlDciIdealControlMessage> dci = DynamicCast<UlDciIdealControlMessage> (msg);
//               QueueUlDci (*dci);
              it2 = m_ueAttached.find (dci->GetDci ().m_rnti);

              if (it2 == m_ueAttached.end ())
                {
                  NS_LOG_ERROR ("UE not attached");
                }
              else
                {
                  (*it2).second->ReceiveIdealControlMessage (msg);
                  QueueUlDci (*dci);
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
      NS_LOG_LOGIC (this << " eNB start TX");
      m_downlinkSpectrumPhy->StartTx (pb);
    }

  // trigger the MAC
  m_enbPhySapUser->SubframeIndication (m_nrFrames, m_nrSubFrames);


  // trigger the UE(s)
  std::map <uint16_t, Ptr<LteUePhy> >::iterator it;
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
LteEnbPhy::GenerateCqiReport (const SpectrumValue& sinr)
{
  NS_LOG_FUNCTION (this << sinr);
  m_enbPhySapUser->UlCqiReport (CreateUlCqiReport (sinr));
}


UlCqi_s
LteEnbPhy::CreateUlCqiReport (const SpectrumValue& sinr)
{
  NS_LOG_FUNCTION (this << sinr);
  Values::const_iterator it;
  UlCqi_s ulcqi;
  ulcqi.m_type = UlCqi_s::PUSCH;
  int i = 0;
  for (it = sinr.ConstValuesBegin (); it != sinr.ConstValuesEnd (); it++)
    {
      double sinrdb = 10 * log10 ((*it));
//       NS_LOG_DEBUG ("ULCQI RB " << i << " value " << sinrdb);
      // convert from double to fixed point notation Sxxxxxxxxxxx.xxx
      int16_t sinrFp = LteFfConverter::double2fpS11dot3 (sinrdb);
      ulcqi.m_sinr.push_back (sinrFp);
      i++;
    }
  return (ulcqi);
	
}

void
LteEnbPhy::DoSetTransmissionMode (uint16_t  rnti, uint8_t txMode)
{
  NS_LOG_FUNCTION (this << rnti << (uint16_t)txMode);
  // UL supports only SISO MODE
}

void
LteEnbPhy::QueueUlDci (UlDciIdealControlMessage m)
{
  NS_LOG_FUNCTION (this);
  m_ulDciQueue.at (UL_PUSCH_TTIS_DELAY - 1).push_back (m);
}

std::list<UlDciIdealControlMessage>
LteEnbPhy::DequeueUlDci (void)
{
  NS_LOG_FUNCTION (this);
  if (m_ulDciQueue.at (0).size ()>0)
    {
      std::list<UlDciIdealControlMessage> ret = m_ulDciQueue.at (0);
      m_ulDciQueue.erase (m_ulDciQueue.begin ());
      std::list<UlDciIdealControlMessage> l;
      m_ulDciQueue.push_back (l);
      return (ret);
    }
  else
    {
      m_ulDciQueue.erase (m_ulDciQueue.begin ());
      std::list<UlDciIdealControlMessage> l;
      m_ulDciQueue.push_back (l);
      std::list<UlDciIdealControlMessage> emptylist;
      return (emptylist);
    }
}


};
