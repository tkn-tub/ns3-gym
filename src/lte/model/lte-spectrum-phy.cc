/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 CTTC
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


#include <ns3/waveform-generator.h>
#include <ns3/object-factory.h>
#include <ns3/log.h>
#include <math.h>
#include <ns3/simulator.h>
#include <ns3/trace-source-accessor.h>
#include "ns3/spectrum-error-model.h"
#include "lte-spectrum-phy.h"
#include "lte-net-device.h"

NS_LOG_COMPONENT_DEFINE ("LteSpectrumPhy");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (LteSpectrumPhy);

LteSpectrumPhy::LteSpectrumPhy ()
  : m_mobility (0),
    m_device (0),
    m_channel (0),
    m_txPsd (0),
    m_state (IDLE)
{
  //m_interference = CreateObject<SpectrumInterference> ();
  //m_interference->SetErrorModel (CreateObject<LteSpectrumErrorModel> ());
}


LteSpectrumPhy::~LteSpectrumPhy ()
{

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


Ptr<SpectrumChannel>
LteSpectrumPhy::GetChannel (void)
{
  return m_channel;
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
  static SpectrumType st = SpectrumTypeFactory::Create ("IdealOfdm");
  return st;
}


void
LteSpectrumPhy::SetTxPowerSpectralDensity (Ptr<SpectrumValue> txPsd)
{
  NS_LOG_FUNCTION (this << txPsd);
  NS_ASSERT (txPsd);
  m_txPsd = txPsd;
  NS_LOG_INFO ("\t computed tx_psd: " << *txPsd << "\t stored tx_psd: " << *m_txPsd);
}


void
LteSpectrumPhy::SetNoisePowerSpectralDensity (Ptr<const SpectrumValue> noisePsd)
{
  NS_LOG_FUNCTION (this << noisePsd);
  NS_LOG_INFO ("\t computed noise_psd: " << *noisePsd );
  NS_ASSERT (noisePsd);
  m_noise = noisePsd;
}

Ptr<const SpectrumValue> 
LteSpectrumPhy::GetNoisePowerSpectralDensity (void)
{ 
  NS_LOG_FUNCTION (this);
  return m_noise;
}


void
LteSpectrumPhy::SetGenericPhyTxEndCallback (GenericPhyTxEndCallback c)
{
  NS_LOG_FUNCTION (this);
  m_phyMacTxEndCallback = c;
}


void
LteSpectrumPhy::SetGenericPhyRxStartCallback (GenericPhyRxStartCallback c)
{
  NS_LOG_FUNCTION (this);
  m_phyMacRxStartCallback = c;
}


void
LteSpectrumPhy::SetGenericPhyRxEndErrorCallback (GenericPhyRxEndErrorCallback c)
{
  NS_LOG_FUNCTION (this);
  m_phyMacRxEndErrorCallback = c;
}


void
LteSpectrumPhy::SetGenericPhyRxEndOkCallback (GenericPhyRxEndOkCallback c)
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
  NS_LOG_LOGIC (this << "state: " << m_state);


  for (std::list<Ptr<Packet> >::const_iterator iter = pb->Begin (); iter
       != pb->End (); ++iter)
    {
      Ptr<Packet> packet = (*iter)->Copy ();
      m_phyTxStartTrace (packet);
    }


  if (m_state == LteSpectrumPhy::RX)
    {
      /*
       * NS FATAL ERROR: according to FDD channel acces,
       * the physical layer for transmission cannot be used for reception.
       */
      NS_FATAL_ERROR ("FDD ERROR: R State while sending packet");
    }

  if (m_state == LteSpectrumPhy::IDLE)
    {

      /*
       m_txPsd must be setted by the device, according to
       (i) the available subchannel for transmission
       (ii) the power transmission
       */
      NS_ASSERT (m_txPsd);

      m_txPacket = pb;
      ChangeState (TX);
      NS_ASSERT (m_channel);
      double tti = 0.001;
      m_channel->StartTx (pb, m_txPsd, GetSpectrumType (), Seconds (tti), GetObject<SpectrumPhy> ());
      Simulator::Schedule (Seconds (tti), &LteSpectrumPhy::EndTx, this);
      return false;
    }
  else
    {
      // The device have already started the transmission.
      return true;
    }
}


void
LteSpectrumPhy::EndTx ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC (this << "state: " << m_state);

  NS_ASSERT (m_state == TX);

  for (std::list<Ptr<Packet> >::const_iterator iter = m_txPacket->Begin (); iter
       != m_txPacket->End (); ++iter)
    {
      Ptr<Packet> packet = (*iter)->Copy ();
      m_phyTxEndTrace (packet);
    }

  if (!m_phyMacTxEndCallback.IsNull ())
    {
      for (std::list<Ptr<Packet> >::const_iterator iter = m_txPacket->Begin (); iter
           != m_txPacket->End (); ++iter)
        {
          Ptr<Packet> packet = (*iter)->Copy ();
          m_phyMacTxEndCallback (packet);
        }
    }

  m_txPacket = 0;
  ChangeState (IDLE);
}


void
LteSpectrumPhy::StartRx (Ptr<PacketBurst> pb, Ptr <const SpectrumValue> rxPsd, SpectrumType st, Time duration)
{
  NS_LOG_FUNCTION (this << pb << rxPsd << st << duration);
  NS_LOG_LOGIC (this << "state: " << m_state);


  // interference will happen regardless of the state of the receiver
  // m_interference->AddSignal (rxPsd, duration);

  // the device might start RX only if the signal is of a type understood by this device
  // this corresponds in real device to preamble detection
  if (st == GetSpectrumType ())
    {
      switch (m_state)
        {
        case TX:
          /*
           * NS FATAL ERROR: according to FDD channel acces,
           * the physical layer for reception cannot be used for transmission.
           */
          NS_FATAL_ERROR ("FDD ERROR: TX State while receiving packet");

          break;

        case RX:
          break;

        case IDLE:
          // preamble detection and synchronization is supposed to be always successful.
          NS_LOG_LOGIC (this << " receiving new packet");

          for (std::list<Ptr<Packet> >::const_iterator iter = pb->Begin (); iter
               != pb->End (); ++iter)
            {
              Ptr<Packet> packet = (*iter)->Copy ();
              m_phyRxStartTrace (packet);
            }


          m_rxPacket = pb;
          m_rxPsd = rxPsd;

          ChangeState (RX);

          if (!m_phyMacRxStartCallback.IsNull ())
            {
              NS_LOG_LOGIC (this << " calling m_phyMacRxStartCallback");
              m_phyMacRxStartCallback ();
            }
          else
            {
              NS_LOG_LOGIC (this << " m_phyMacRxStartCallback is NULL");
            }

          // XXX: modify SpectrumInterference in order to compute
          // the correct/erroneus reception of PacketBurst!!!
          /* 
          for (std::list<Ptr<Packet> >::const_iterator iter = pb->Begin (); iter
               != pb->End (); ++iter)
            {
              Ptr<Packet> packet = (*iter)->Copy ();
              m_interference->StartRx (packet, rxPsd);
            }
          */


          NS_LOG_LOGIC (this << " scheduling EndRx with delay " << duration);
          m_endRxEventId = Simulator::Schedule (duration, &LteSpectrumPhy::EndRx, this);

          break;

        }
    }

  NS_LOG_LOGIC (this << "state: " << m_state);
}


void
LteSpectrumPhy::AbortRx ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC (this << "state: " << m_state);

  NS_ASSERT (m_state == RX);

  for (std::list<Ptr<Packet> >::const_iterator iter = m_rxPacket->Begin (); iter
       != m_rxPacket->End (); ++iter)
    {
      Ptr<Packet> packet = (*iter)->Copy ();
      m_phyRxAbortTrace (packet);
    }

  m_endRxEventId.Cancel ();
  m_rxPacket = 0;
  ChangeState (IDLE);
}


void
LteSpectrumPhy::EndRx ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC (this << "state: " << m_state);

  NS_ASSERT (m_state == RX);

  CalcSinrValues (m_rxPsd, GetNoisePowerSpectralDensity ());

  bool rxOk = true; //m_interference->EndRx ();

  NS_LOG_FUNCTION (rxOk);
  if (rxOk)
    {

      for (std::list<Ptr<Packet> >::const_iterator iter = m_rxPacket->Begin (); iter
           != m_rxPacket->End (); ++iter)
        {
          Ptr<Packet> packet = (*iter)->Copy ();
          m_phyRxEndOkTrace (packet);
        }

      if (!m_phyMacRxEndOkCallback.IsNull ())
        {
          NS_LOG_LOGIC (this << " calling m_phyMacRxEndOkCallback");

          for (std::list<Ptr<Packet> >::const_iterator iter = m_rxPacket->Begin (); iter
               != m_rxPacket->End (); ++iter)
            {
              Ptr<Packet> packet = (*iter)->Copy ();
              m_phyMacRxEndOkCallback (packet);
            }
        }
      else
        {
          NS_LOG_LOGIC (this << " m_phyMacRxEndOkCallback is NULL");
        }
    }
  else
    {
      for (std::list<Ptr<Packet> >::const_iterator iter = m_rxPacket->Begin (); iter
           != m_rxPacket->End (); ++iter)
        {
          Ptr<Packet> packet = (*iter)->Copy ();
          m_phyRxEndErrorTrace (packet);
        }

      if (!m_phyMacRxEndErrorCallback.IsNull ())
        {
          NS_LOG_LOGIC (this << " calling m_phyMacRxEndErrorCallback");

          for (std::list<Ptr<Packet> >::const_iterator iter = m_rxPacket->Begin (); iter
               != m_rxPacket->End (); ++iter)
            {
              Ptr<Packet> packet = (*iter)->Copy ();
              m_phyMacRxEndOkCallback (packet);
            }

        }
      else
        {
          NS_LOG_LOGIC (this << " m_phyMacRxEndErrorCallback is NULL");
        }
    }

  ChangeState (IDLE);
  m_rxPacket = 0;
  m_rxPsd = 0;
}


} // namespace ns3
