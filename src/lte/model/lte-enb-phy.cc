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
#include <cmath>
#include <ns3/simulator.h>
#include <ns3/attribute-accessor-helper.h>
#include <ns3/double.h>


#include "lte-enb-phy.h"
#include "lte-net-device.h"
#include "lte-spectrum-value-helper.h"
#include "lte-control-messages.h"
#include "lte-enb-net-device.h"
#include "lte-enb-mac.h"
#include <ns3/lte-common.h>
#include <ns3/lte-vendor-specific-parameters.h>


NS_LOG_COMPONENT_DEFINE ("LteEnbPhy");

namespace ns3 {


// duration of the data part of a subframe in DL
// = 0.001 / 14 * 11 (fixed to 11 symbols) -1ns as margin to avoid overlapping simulator events
static const Time DL_DATA_DURATION = NanoSeconds (785714 -1);

//  delay from subframe start to transmission of the data in DL 
// = 0.001 / 14 * 3 (ctrl fixed to 3 symbols)
static const Time DL_CTRL_DELAY_FROM_SUBFRAME_START = NanoSeconds (214286);

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
  virtual void SendLteControlMessage (Ptr<LteControlMessage> msg);
  virtual uint8_t GetMacChTtiDelay ();
  virtual void SetTransmissionMode (uint16_t  rnti, uint8_t txMode);
  virtual void SetSrsConfigurationIndex (uint16_t  rnti, uint16_t srcCi);
  

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
EnbMemberLteEnbPhySapProvider::SendLteControlMessage (Ptr<LteControlMessage> msg)
{
  m_phy->DoSendLteControlMessage (msg);
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

void
EnbMemberLteEnbPhySapProvider::SetSrsConfigurationIndex (uint16_t  rnti, uint16_t srcCi)
{
  m_phy->DoSetSrsConfigurationIndex (rnti, srcCi);
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
    m_nrSubFrames (0),
    m_srsPeriodicity (0),
    m_currentSrsOffset (0)
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
  m_srsUeOffset.clear ();
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
      std::list<Ptr<LteControlMessage> > l;
      m_controlMessagesQueue.push_back (l);
      std::list<UlDciLteControlMessage> l1;
      m_ulDciQueue.push_back (l1);
    }
  for (int i = 0; i < UL_PUSCH_TTIS_DELAY; i++)
    {
      std::list<UlDciLteControlMessage> l1;
      m_ulDciQueue.push_back (l1);
    }
}

uint8_t
LteEnbPhy::GetMacChDelay (void) const
{
  return (m_macChTtiDelay);
}

bool
LteEnbPhy::AddUePhy (uint16_t rnti)
{
  std::set <uint16_t>::iterator it;
  it = m_ueAttached.find (rnti);
  if (it == m_ueAttached.end ())
    {
      m_ueAttached.insert (rnti);
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
  std::set <uint16_t>::iterator it;
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
LteEnbPhy::SetDownlinkSubChannels (std::vector<int> mask)
{
  NS_LOG_FUNCTION (this);
  m_listOfDownlinkSubchannel = mask;
  Ptr<SpectrumValue> txPsd = CreateTxPowerSpectralDensity ();
  m_downlinkSpectrumPhy->SetTxPowerSpectralDensity (txPsd);
}

std::vector<int>
LteEnbPhy::GetDownlinkSubChannels (void)
{
  NS_LOG_FUNCTION (this);
  return m_listOfDownlinkSubchannel;
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
LteEnbPhy::DoSendLteControlMessage (Ptr<LteControlMessage> msg)
{
  NS_LOG_FUNCTION (this << msg);
  // queues the message (wait for MAC-PHY delay)
  SetControlMessages (msg);
}



void
LteEnbPhy::ReceiveLteControlMessage (Ptr<LteControlMessage> msg)
{
  NS_FATAL_ERROR ("Obsolete function");
  NS_LOG_FUNCTION (this << msg);
  m_enbPhySapUser->ReceiveLteControlMessage (msg);
}

void
LteEnbPhy::ReceiveLteControlMessageList (std::list<Ptr<LteControlMessage> > msgList)
{
  NS_LOG_FUNCTION (this);
  std::list<Ptr<LteControlMessage> >::iterator it;
  for (it = msgList.begin (); it != msgList.end(); it++)
    {
      m_enbPhySapUser->ReceiveLteControlMessage (*it);
    }
    
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
  if (m_srsPeriodicity>0)
    { 
      // might be 0 in case the eNB has no UEs attached
      m_currentSrsOffset = (m_currentSrsOffset + 1) % m_srsPeriodicity;
    }
  NS_LOG_INFO ("-----sub frame " << m_nrSubFrames << "-----");
  
  
  // update info on TB to be received
  std::list<UlDciLteControlMessage> uldcilist = DequeueUlDci ();
  std::list<UlDciLteControlMessage>::iterator dciIt = uldcilist.begin ();
  m_ulRntiRxed.clear ();
  NS_LOG_DEBUG (this << " eNB Expected TBs " << uldcilist.size ());
  for (dciIt = uldcilist.begin (); dciIt!=uldcilist.end (); dciIt++)
    {
      std::set <uint16_t>::iterator it2;
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
          m_ulRntiRxed.push_back ((*dciIt).GetDci ().m_rnti);
        }
    }

  // process the current burst of control messages
  std::list<Ptr<LteControlMessage> > ctrlMsg = GetControlMessages ();
  std::list<DlDciListElement_s> dlDci;
  std::list<UlDciListElement_s> ulDci;
//   std::vector <int> dlRb;
  m_dlDataRbMap.clear ();
  if (ctrlMsg.size () > 0)
    {
      std::list<Ptr<LteControlMessage> >::iterator it;
      it = ctrlMsg.begin ();
      while (it != ctrlMsg.end ())
        {
          Ptr<LteControlMessage> msg = (*it);
          if (msg->GetMessageType () == LteControlMessage::DL_DCI)
            {
              Ptr<DlDciLteControlMessage> dci = DynamicCast<DlDciLteControlMessage> (msg);
              dlDci.push_back (dci->GetDci ());
                  // get the tx power spectral density according to DL-DCI(s)
                  // translate the DCI to Spectrum framework
                  uint32_t mask = 0x1;
                  for (int i = 0; i < 32; i++)
                    {
                      if (((dci->GetDci ().m_rbBitmap & mask) >> i) == 1)
                        {
                          for (int k = 0; k < GetRbgSize (); k++)
                            {
                              m_dlDataRbMap.push_back ((i * GetRbgSize ()) + k);
                              //NS_LOG_DEBUG(this << " [enb]DL-DCI allocated PRB " << (i*GetRbgSize()) + k);
                            }
                        }
                      mask = (mask << 1);
                    }
            }
          else if (msg->GetMessageType () == LteControlMessage::UL_DCI)
            {
              Ptr<UlDciLteControlMessage> dci = DynamicCast<UlDciLteControlMessage> (msg);
              QueueUlDci (*dci);
              ulDci.push_back (dci->GetDci ());
            }
          it++;

        }
    }
    
  SendControlChannels (ctrlMsg);
  
  // send data frame
  Ptr<PacketBurst> pb = GetPacketBurst ();
  if (pb)
    {
      Simulator::Schedule (DL_CTRL_DELAY_FROM_SUBFRAME_START, // ctrl frame fixed to 3 symbols
                       &LteEnbPhy::SendDataChannels,
                       this,pb);
    }

  // trigger the MAC
  m_enbPhySapUser->SubframeIndication (m_nrFrames, m_nrSubFrames);

  Simulator::Schedule (Seconds (GetTti ()),
                       &LteEnbPhy::EndSubFrame,
                       this);

}

void
LteEnbPhy::SendControlChannels (std::list<Ptr<LteControlMessage> > ctrlMsgList)
{
  NS_LOG_FUNCTION (this << " eNB " << m_cellId << " start tx ctrl frame");
  // set the current tx power spectral density (full bandwidth)
  std::vector <int> dlRb;
  for (uint8_t i = 0; i < m_dlBandwidth; i++)
    {
      dlRb.push_back (i);
    }
  SetDownlinkSubChannels (dlRb);
  NS_LOG_LOGIC (this << " eNB start TX CTRL");
  m_downlinkSpectrumPhy->StartTxDlCtrlFrame (ctrlMsgList);
  
}

void
LteEnbPhy::SendDataChannels (Ptr<PacketBurst> pb)
{
  // set the current tx power spectral density
  SetDownlinkSubChannels (m_dlDataRbMap);
  // send the current burts of packets
  NS_LOG_LOGIC (this << " eNB start TX DATA");
  std::list<Ptr<LteControlMessage> > ctrlMsgList;
  ctrlMsgList.clear ();
  m_downlinkSpectrumPhy->StartTxDataFrame (pb, ctrlMsgList, DL_DATA_DURATION);
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
LteEnbPhy::GenerateCtrlCqiReport (const SpectrumValue& sinr)
{
  NS_LOG_FUNCTION (this << sinr);
  FfMacSchedSapProvider::SchedUlCqiInfoReqParameters ulcqi = CreateSrsCqiReport (sinr);
  m_enbPhySapUser->UlCqiReport (ulcqi);
}

void
LteEnbPhy::GenerateDataCqiReport (const SpectrumValue& sinr)
{
  NS_LOG_FUNCTION (this << sinr);
  FfMacSchedSapProvider::SchedUlCqiInfoReqParameters ulcqi = CreatePuschCqiReport (sinr);
  m_enbPhySapUser->UlCqiReport (ulcqi);
}



FfMacSchedSapProvider::SchedUlCqiInfoReqParameters
LteEnbPhy::CreatePuschCqiReport (const SpectrumValue& sinr)
{
  NS_LOG_FUNCTION (this << sinr);
  Values::const_iterator it;
  FfMacSchedSapProvider::SchedUlCqiInfoReqParameters ulcqi;
  ulcqi.m_ulCqi.m_type = UlCqi_s::PUSCH;
  int i = 0;
  for (it = sinr.ConstValuesBegin (); it != sinr.ConstValuesEnd (); it++)
    {
      double sinrdb = 10 * std::log10 ((*it));
//       NS_LOG_DEBUG ("ULCQI RB " << i << " value " << sinrdb);
      // convert from double to fixed point notation Sxxxxxxxxxxx.xxx
      int16_t sinrFp = LteFfConverter::double2fpS11dot3 (sinrdb);
      ulcqi.m_ulCqi.m_sinr.push_back (sinrFp);
      i++;
    }
  return (ulcqi);
	
}


FfMacSchedSapProvider::SchedUlCqiInfoReqParameters
LteEnbPhy::CreateSrsCqiReport (const SpectrumValue& sinr)
{
  NS_LOG_FUNCTION (this << sinr);
  Values::const_iterator it;
  FfMacSchedSapProvider::SchedUlCqiInfoReqParameters ulcqi;
  ulcqi.m_ulCqi.m_type = UlCqi_s::SRS;
  int i = 0;
  for (it = sinr.ConstValuesBegin (); it != sinr.ConstValuesEnd (); it++)
  {
    double sinrdb = 10 * log10 ((*it));
    //       NS_LOG_DEBUG ("ULCQI RB " << i << " value " << sinrdb);
    // convert from double to fixed point notation Sxxxxxxxxxxx.xxx
    int16_t sinrFp = LteFfConverter::double2fpS11dot3 (sinrdb);
    ulcqi.m_ulCqi.m_sinr.push_back (sinrFp);
    i++;
  }
  // Insert the user generated the srs as a vendor specific parameter
  NS_LOG_DEBUG (this << " ENB RX UL-CQI of " << m_srsUeOffset.at (m_currentSrsOffset));
  VendorSpecificListElement_s vsp;
  vsp.m_type = SRS_CQI_RNTI_VSP;
  vsp.m_length = sizeof(SrsCqiRntiVsp);
  Ptr<SrsCqiRntiVsp> rnti  = Create <SrsCqiRntiVsp> (m_srsUeOffset.at (m_currentSrsOffset));
  vsp.m_value = rnti;
  ulcqi.m_vendorSpecificList.push_back (vsp);
  return (ulcqi);
  
}

void
LteEnbPhy::DoSetTransmissionMode (uint16_t  rnti, uint8_t txMode)
{
  NS_LOG_FUNCTION (this << rnti << (uint16_t)txMode);
  // UL supports only SISO MODE
}

void
LteEnbPhy::QueueUlDci (UlDciLteControlMessage m)
{
  NS_LOG_FUNCTION (this);
  m_ulDciQueue.at (UL_PUSCH_TTIS_DELAY - 1).push_back (m);
}

std::list<UlDciLteControlMessage>
LteEnbPhy::DequeueUlDci (void)
{
  NS_LOG_FUNCTION (this);
  if (m_ulDciQueue.at (0).size ()>0)
    {
      std::list<UlDciLteControlMessage> ret = m_ulDciQueue.at (0);
      m_ulDciQueue.erase (m_ulDciQueue.begin ());
      std::list<UlDciLteControlMessage> l;
      m_ulDciQueue.push_back (l);
      return (ret);
    }
  else
    {
      m_ulDciQueue.erase (m_ulDciQueue.begin ());
      std::list<UlDciLteControlMessage> l;
      m_ulDciQueue.push_back (l);
      std::list<UlDciLteControlMessage> emptylist;
      return (emptylist);
    }
}

void
LteEnbPhy::DoSetSrsConfigurationIndex (uint16_t  rnti, uint16_t srcCi)
{
  NS_LOG_FUNCTION (this);
  uint16_t p = GetSrsPeriodicity (srcCi);
  if (p!=m_srsPeriodicity)
    {
      // resize the array of offset -> re-initialize variables
      m_srsUeOffset.clear ();
      m_srsUeOffset.resize (p, 0);
      m_srsPeriodicity = p;
      m_currentSrsOffset = p - 1; // for starting from 0 next subframe
    }
    
  NS_LOG_DEBUG (this << " ENB SRS P " << m_srsPeriodicity << " RNTI " << rnti << " offset " << GetSrsSubframeOffset (srcCi) << " CI " << srcCi);
  std::map <uint16_t,uint16_t>::iterator it = m_srsCounter.find (rnti);
  if (it != m_srsCounter.end ())
    {
      (*it).second = GetSrsSubframeOffset (srcCi) + 1;
    }
  else
    {
      m_srsCounter.insert (std::pair<uint16_t, uint16_t> (rnti, GetSrsSubframeOffset (srcCi) + 1));
    }
  m_srsUeOffset.at (GetSrsSubframeOffset (srcCi)) = rnti;
  
}


};
