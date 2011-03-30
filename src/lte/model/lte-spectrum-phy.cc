/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009, 2011 CTTC
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 *         Giuseppe Piro  <g.piro@poliba.it>
 */


#include <ns3/object-factory.h>
#include <ns3/log.h>
#include <math.h>
#include <ns3/simulator.h>
#include <ns3/trace-source-accessor.h>
#include "lte-spectrum-phy.h"
#include "lte-net-device.h"
#include "lte-mac-tag.h"
#include "lte-sinr-chunk-processor.h"
#include "lte-phy-tag.h"

NS_LOG_COMPONENT_DEFINE ("LteSpectrumPhy");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (LteSpectrumPhy);

LteSpectrumPhy::LteSpectrumPhy ()
  : m_state (IDLE)
{
  NS_LOG_FUNCTION (this);
  m_interference = CreateObject<LteInterference> ();
}


LteSpectrumPhy::~LteSpectrumPhy ()
{
  NS_LOG_FUNCTION (this);
}

void LteSpectrumPhy::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_endRxEventId.Cancel ();
  m_channel = 0;
  m_mobility = 0;
  m_device = 0;
  m_interference->Dispose ();
  m_interference = 0;
  SpectrumPhy::DoDispose ();
} 

std::ostream& operator<< (std::ostream& os, LteSpectrumPhy::State s)
{
  switch (s)
    {
    case LteSpectrumPhy::IDLE:
      os << "IDLE";
      break;
    case LteSpectrumPhy::RX:
      os << "RX";
      break;
    case LteSpectrumPhy::TX:
      os << "TX";
      break;
    default:
      os << "UNKNOWN";
      break;
    }
  return os;
}


TypeId
LteSpectrumPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteSpectrumPhy")
    .SetParent<SpectrumPhy> ()
    .AddTraceSource ("TxStart",
                     "Trace fired when a new transmission is started",
                     MakeTraceSourceAccessor (&LteSpectrumPhy::m_phyTxStartTrace))
    .AddTraceSource ("TxEnd",
                     "Trace fired when a previosuly started transmission is finished",
                     MakeTraceSourceAccessor (&LteSpectrumPhy::m_phyTxEndTrace))
    .AddTraceSource ("RxStart",
                     "Trace fired when the start of a signal is detected",
                     MakeTraceSourceAccessor (&LteSpectrumPhy::m_phyRxStartTrace))
    .AddTraceSource ("RxAbort",
                     "Trace fired when a previously started RX is aborted before time",
                     MakeTraceSourceAccessor (&LteSpectrumPhy::m_phyRxAbortTrace))
    .AddTraceSource ("RxEndOk",
                     "Trace fired when a previosuly started RX terminates successfully",
                     MakeTraceSourceAccessor (&LteSpectrumPhy::m_phyRxEndOkTrace))
    .AddTraceSource ("RxEndError",
                     "Trace fired when a previosuly started RX terminates with an error (packet is corrupted)",
                     MakeTraceSourceAccessor (&LteSpectrumPhy::m_phyRxEndErrorTrace))
  ;
  return tid;
}



Ptr<Object>
LteSpectrumPhy::GetDevice ()
{
  NS_LOG_FUNCTION (this);
  return m_device;
}


Ptr<Object>
LteSpectrumPhy::GetMobility ()
{
  NS_LOG_FUNCTION (this);
  return m_mobility;
}


void
LteSpectrumPhy::SetDevice (Ptr<Object> d)
{
  NS_LOG_FUNCTION (this << d);
  m_device = d;
}


void
LteSpectrumPhy::SetMobility (Ptr<Object> m)
{
  NS_LOG_FUNCTION (this << m);
  m_mobility = m;
}


void
LteSpectrumPhy::SetChannel (Ptr<SpectrumChannel> c)
{
  NS_LOG_FUNCTION (this << c);
  m_channel = c;
}

Ptr<const SpectrumModel>
LteSpectrumPhy::GetRxSpectrumModel () const
{
  if (m_txPsd)
    {
      return m_txPsd->GetSpectrumModel ();
    }
  else
    {
      return 0;
    }
}


SpectrumType
LteSpectrumPhy::GetSpectrumType ()
{
  NS_LOG_FUNCTION (this);
  static SpectrumType st = SpectrumTypeFactory::Create ("Lte");
  return st;
}


void
LteSpectrumPhy::SetTxPowerSpectralDensity (Ptr<SpectrumValue> txPsd)
{
  NS_LOG_FUNCTION (this << txPsd);
  NS_ASSERT (txPsd);
  m_txPsd = txPsd;
}


void
LteSpectrumPhy::SetNoisePowerSpectralDensity (Ptr<const SpectrumValue> noisePsd)
{
  NS_LOG_FUNCTION (this << noisePsd);
  NS_ASSERT (noisePsd);
  m_interference->SetNoisePowerSpectralDensity (noisePsd);
}



void
LteSpectrumPhy::SetPhyMacTxEndCallback (PhyMacTxEndCallback c)
{
  NS_LOG_FUNCTION (this);
  m_phyMacTxEndCallback = c;
}


void
LteSpectrumPhy::SetPhyMacRxEndErrorCallback (PhyMacRxEndErrorCallback c)
{
  NS_LOG_FUNCTION (this);
  m_phyMacRxEndErrorCallback = c;
}


void
LteSpectrumPhy::SetPhyMacRxEndOkCallback (PhyMacRxEndOkCallback c)
{
  NS_LOG_FUNCTION (this);
  m_phyMacRxEndOkCallback = c;
}


void
LteSpectrumPhy::SetState (State newState)
{
  ChangeState (newState);
}


void
LteSpectrumPhy::ChangeState (State newState)
{
  NS_LOG_LOGIC (this << " state: " << m_state << " -> " << newState);
  m_state = newState;
}


bool
LteSpectrumPhy::StartTx (Ptr<PacketBurst> pb)
{
  NS_LOG_FUNCTION (this << pb);
  NS_LOG_LOGIC (this << " state: " << m_state);


  for (std::list<Ptr<Packet> >::const_iterator iter = pb->Begin (); iter
         != pb->End (); ++iter)
    {
      Ptr<Packet> packet = (*iter)->Copy ();
      m_phyTxStartTrace (packet);
    }

  switch (m_state)
    {
    case RX:
      NS_FATAL_ERROR ("cannot TX while RX: according to FDD channel acces, the physical layer for transmission cannot be used for reception");
      break;

    case TX:
      NS_FATAL_ERROR ("cannot TX while already TX: the MAC should avoid this");
      break;
      
    case IDLE:
      {
        /*
          m_txPsd must be setted by the device, according to
          (i) the available subchannel for transmission
          (ii) the power transmission
        */
        NS_ASSERT (m_txPsd);
        m_txPacketBurst = pb;
        
        // we need to convey some PHY meta information to the receiver
        // to be used for simulation purposes (e.g., the CellId). This
        // is done by adding an LtePhyTag to the first packet in the
        // burst.
        NS_ASSERT (pb->Begin () != pb->End ());
        LtePhyTag tag (m_cellId);
        Ptr<Packet> firstPacketInBurst = *(pb->Begin ());
        firstPacketInBurst->AddPacketTag (tag);
        
        ChangeState (TX);
        NS_ASSERT (m_channel);
        double tti = 0.001;
        m_channel->StartTx (pb, m_txPsd, GetSpectrumType (), Seconds (tti), GetObject<SpectrumPhy> ());
        NS_LOG_LOGIC (this << " scheduling EndTx ()");
        Simulator::Schedule (Seconds (tti), &LteSpectrumPhy::EndTx, this);
      }
      return true;
      break;
      
    default:
      NS_FATAL_ERROR ("uknown state");
      return true;
      break;
    }
}


void
LteSpectrumPhy::EndTx ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC (this << " state: " << m_state);

  NS_ASSERT (m_state == TX);

  for (std::list<Ptr<Packet> >::const_iterator iter = m_txPacketBurst->Begin (); iter
       != m_txPacketBurst->End (); ++iter)
    {
      Ptr<Packet> packet = (*iter)->Copy ();
      m_phyTxEndTrace (packet);
    }

  if (!m_phyMacTxEndCallback.IsNull ())
    {
      for (std::list<Ptr<Packet> >::const_iterator iter = m_txPacketBurst->Begin (); iter
           != m_txPacketBurst->End (); ++iter)
        {
          Ptr<Packet> packet = (*iter)->Copy ();
          m_phyMacTxEndCallback (packet);
        }
    }

  m_txPacketBurst = 0;
  ChangeState (IDLE);
}


void
LteSpectrumPhy::StartRx (Ptr<PacketBurst> pb, Ptr <const SpectrumValue> rxPsd, SpectrumType st, Time duration)
{
  NS_LOG_FUNCTION (this << pb << rxPsd << st << duration);
  NS_LOG_LOGIC (this << " state: " << m_state);

  m_interference->AddSignal (rxPsd, duration);

  // the device might start RX only if the signal is of a type
  // understood by this device - in this case, an LTE signal.
  if (st == GetSpectrumType ())
    {
      switch (m_state)
        {
        case TX:
          NS_FATAL_ERROR ("cannot RX while TX: according to FDD channel acces, the physical layer for transmission cannot be used for reception");
          break;

        case RX:
          break;

        case IDLE:
          {
            // To check if we're synchronized to this signal, we check
            // for the CellId which is reported in the LtePhyTag
            NS_ASSERT (pb->Begin () != pb->End ());          
            LtePhyTag tag;
            Ptr<Packet> firstPacketInBurst = *(pb->Begin ());
            firstPacketInBurst->RemovePacketTag (tag);
            if (tag.GetCellId () == m_cellId)
              {
                NS_LOG_LOGIC (this << " synchronized with this signal (cellId=" << tag.GetCellId () << ")");
                ChangeState (RX);
              
                m_interference->StartRx (rxPsd);
  
                for (std::list<Ptr<Packet> >::const_iterator iter = pb->Begin (); iter
                       != pb->End (); ++iter)
                  {
                    Ptr<Packet> packet = (*iter)->Copy ();
                    m_phyRxStartTrace (packet);
                  }
              
                m_rxPacketBurst = pb;
                m_rxPsd = rxPsd;              
              
                NS_LOG_LOGIC (this << " scheduling EndRx with delay " << duration);
                m_endRxEventId = Simulator::Schedule (duration, &LteSpectrumPhy::EndRx, this);
              }      
            else
              {
                NS_LOG_LOGIC (this << " not in sync with this signal (cellId=" 
                              << tag.GetCellId () << ", m_cellId=" << m_cellId << ")");
              }
          }
          break;
          
        default:
          NS_FATAL_ERROR ("unknown state");
          break;
        }

      NS_LOG_LOGIC (this << " state: " << m_state);
    }
}

void
LteSpectrumPhy::AbortRx ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC (this << " state: " << m_state);

  NS_ASSERT (m_state == RX);

  for (std::list<Ptr<Packet> >::const_iterator iter = m_rxPacketBurst->Begin (); iter
       != m_rxPacketBurst->End (); ++iter)
    {
      Ptr<Packet> packet = (*iter)->Copy ();
      m_phyRxAbortTrace (packet);
    }

  m_endRxEventId.Cancel ();
  m_rxPacketBurst = 0;
  ChangeState (IDLE);
}


void
LteSpectrumPhy::EndRx ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC (this << " state: " << m_state);

  NS_ASSERT (m_state == RX);

  // this will trigger CQI calculation and Error Model evaluation
  // as a side effect, the error model should update the error status of all PDUs
  m_interference->EndRx ();

  for (std::list<Ptr<Packet> >::const_iterator iter = m_rxPacketBurst->Begin (); iter
         != m_rxPacketBurst->End (); ++iter)
    {
      // here we should determine whether this particular PDU
      // (identified by RNTI and LCID) has been received with errors
      // or not 
      // LteMacTag tag;
      // (*iter)->PeekPacketTag (tag);
      // uint16_t rnti = tag.GetRnti ();
      // uint8_t lcid = tag.GetLcid ();
      // bool pduError = IsPduInError (rnti, lcid);
      bool pduError = false;

      if (pduError)
        {
          m_phyRxEndErrorTrace ((*iter)->Copy ());
          if (!m_phyMacRxEndErrorCallback.IsNull ())
            {
              NS_LOG_LOGIC (this << " calling m_phyMacRxEndErrorCallback");
              m_phyMacRxEndOkCallback ((*iter)->Copy ());
            }
          else
            {
              NS_LOG_LOGIC (this << " m_phyMacRxEndErrorCallback is NULL");
            }
        }        
      else // pdu received successfully
        {
          m_phyRxEndOkTrace ((*iter)->Copy ());          
          if (!m_phyMacRxEndOkCallback.IsNull ())
            {
              NS_LOG_LOGIC (this << " calling m_phyMacRxEndOkCallback"); 
              m_phyMacRxEndOkCallback (*iter);            
            }
          else
            {
              NS_LOG_LOGIC (this << " m_phyMacRxEndOkCallback is NULL");
            }
        }
    }

  ChangeState (IDLE);
  m_rxPacketBurst = 0;
  m_rxPsd = 0;
}

void 
LteSpectrumPhy::SetCellId (uint16_t cellId)
{
  m_cellId = cellId;
}

void
LteSpectrumPhy::AddSinrChunkProcessor (Ptr<LteSinrChunkProcessor> p)
{
  m_interference->AddSinrChunkProcessor (p);
}


} // namespace ns3
