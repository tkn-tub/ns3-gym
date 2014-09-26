/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 * Author: Leonard Tracy <lentracy@gmail.com>
 */

#include "uan-mac-cw.h"
#include "ns3/attribute.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/nstime.h"
#include "ns3/uan-header-common.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("UanMacCw");

NS_OBJECT_ENSURE_REGISTERED (UanMacCw);

UanMacCw::UanMacCw ()
  : UanMac (),
    m_phy (0),
    m_pktTx (0),
    m_state (IDLE),
    m_cleared (false)

{
  m_rv = CreateObject<UniformRandomVariable> ();
}

UanMacCw::~UanMacCw ()
{
}

void
UanMacCw::Clear ()
{
  if (m_cleared)
    {
      return;
    }
  m_cleared = true;
  m_pktTx = 0;
  if (m_phy)
    {
      m_phy->Clear ();
      m_phy = 0;
    }
  m_sendEvent.Cancel ();
  m_txEndEvent.Cancel ();
}

void
UanMacCw::DoDispose ()
{
  Clear ();
  UanMac::DoDispose ();
}

TypeId
UanMacCw::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UanMacCw")
    .SetParent<Object> ()
    .AddConstructor<UanMacCw> ()
    .AddAttribute ("CW",
                   "The MAC parameter CW.",
                   UintegerValue (10),
                   MakeUintegerAccessor (&UanMacCw::m_cw),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("SlotTime",
                   "Time slot duration for MAC backoff.",
                   TimeValue (MilliSeconds (20)),
                   MakeTimeAccessor (&UanMacCw::m_slotTime),
                   MakeTimeChecker ())
    .AddTraceSource ("Enqueue",
                     "A packet arrived at the MAC for transmission.",
                     MakeTraceSourceAccessor (&UanMacCw::m_enqueueLogger))
    .AddTraceSource ("Dequeue",
                     "A was passed down to the PHY from the MAC.",
                     MakeTraceSourceAccessor (&UanMacCw::m_dequeueLogger))
    .AddTraceSource ("RX",
                     "A packet was destined for this MAC and was received.",
                     MakeTraceSourceAccessor (&UanMacCw::m_rxLogger))

  ;
  return tid;
}

Address
UanMacCw::GetAddress ()
{
  return this->m_address;
}

void
UanMacCw::SetAddress (UanAddress addr)
{
  m_address = addr;
}

bool
UanMacCw::Enqueue (Ptr<Packet> packet, const Address &dest, uint16_t protocolNumber)
{

  switch (m_state)
    {
    case CCABUSY:
      NS_LOG_DEBUG ("Time " << Simulator::Now ().GetSeconds () << " MAC " << GetAddress () << " Starting enqueue CCABUSY");
      if (m_txEndEvent.IsRunning ())
        {
          NS_LOG_DEBUG ("State is TX");
        }
      else
        {
          NS_LOG_DEBUG ("State is not TX");
        }

      NS_ASSERT (m_phy->GetTransducer ()->GetArrivalList ().size () >= 1 || m_phy->IsStateTx ());
      return false;
    case RUNNING:
      NS_LOG_DEBUG ("MAC " << GetAddress () << " Starting enqueue RUNNING");
      NS_ASSERT (m_phy->GetTransducer ()->GetArrivalList ().size () == 0 && !m_phy->IsStateTx ());
      return false;
    case TX:
    case IDLE:
      {
        NS_ASSERT (!m_pktTx);

        UanHeaderCommon header;
        header.SetDest (UanAddress::ConvertFrom (dest));
        header.SetSrc (m_address);
        header.SetType (0);
        packet->AddHeader (header);

        m_enqueueLogger (packet, protocolNumber);

        if (m_phy->IsStateBusy ())
          {
            m_pktTx = packet;
            m_pktTxProt = protocolNumber;
            m_state = CCABUSY;
            uint32_t cw = (uint32_t) m_rv->GetValue (0,m_cw);
            m_savedDelayS = Seconds ((double)(cw) * m_slotTime.GetSeconds ());
            m_sendTime = Simulator::Now () + m_savedDelayS;
            NS_LOG_DEBUG ("Time " << Simulator::Now ().GetSeconds () << ": Addr " << GetAddress () << ": Enqueuing new packet while busy:  (Chose CW " << cw << ", Sending at " << m_sendTime.GetSeconds () << " Packet size: " << packet->GetSize ());
            NS_ASSERT (m_phy->GetTransducer ()->GetArrivalList ().size () >= 1 || m_phy->IsStateTx ());
          }
        else
          {
            NS_ASSERT (m_state != TX);
            NS_LOG_DEBUG ("Time " << Simulator::Now ().GetSeconds () << ": Addr " << GetAddress () << ": Enqueuing new packet while idle (sending)");
            NS_ASSERT (m_phy->GetTransducer ()->GetArrivalList ().size () == 0 && !m_phy->IsStateTx ());
            m_state = TX;
            m_phy->SendPacket (packet,protocolNumber);

          }
        break;
      }
    default:
      NS_LOG_DEBUG ("MAC " << GetAddress () << " Starting enqueue SOMETHING ELSE");
      return false;
    }

  return true;


}

void
UanMacCw::SetForwardUpCb (Callback<void, Ptr<Packet>, const UanAddress&> cb)
{
  m_forwardUpCb = cb;
}

void
UanMacCw::AttachPhy (Ptr<UanPhy> phy)
{
  m_phy = phy;
  m_phy->SetReceiveOkCallback (MakeCallback (&UanMacCw::PhyRxPacketGood, this));
  m_phy->SetReceiveErrorCallback (MakeCallback (&UanMacCw::PhyRxPacketError, this));
  m_phy->RegisterListener (this);
}

Address
UanMacCw::GetBroadcast (void) const
{
  return UanAddress::GetBroadcast ();
}


void
UanMacCw::NotifyRxStart (void)
{
  if (m_state == RUNNING)
    {

      NS_LOG_DEBUG ("Time " << Simulator::Now ().GetSeconds () << " Addr " << GetAddress () << ": Switching to channel busy");
      SaveTimer ();
      m_state = CCABUSY;
    }

}
void
UanMacCw::NotifyRxEndOk (void)
{
  if (m_state == CCABUSY && !m_phy->IsStateCcaBusy ())
    {
      NS_LOG_DEBUG ("Time " << Simulator::Now ().GetSeconds () << " Addr " << GetAddress () << ": Switching to channel idle");
      m_state = RUNNING;
      StartTimer ();

    }

}
void
UanMacCw::NotifyRxEndError (void)
{
  if (m_state == CCABUSY && !m_phy->IsStateCcaBusy ())
    {
      NS_LOG_DEBUG ("Time " << Simulator::Now ().GetSeconds () << " Addr " << GetAddress () << ": Switching to channel idle");
      m_state = RUNNING;
      StartTimer ();

    }

}
void
UanMacCw::NotifyCcaStart (void)
{
  if (m_state == RUNNING)
    {
      NS_LOG_DEBUG ("Time " << Simulator::Now ().GetSeconds () << " Addr " << GetAddress () << ": Switching to channel busy");
      m_state = CCABUSY;
      SaveTimer ();

    }

}
void
UanMacCw::NotifyCcaEnd (void)
{
  if (m_state == CCABUSY)
    {
      NS_LOG_DEBUG ("Time " << Simulator::Now ().GetSeconds () << " Addr " << GetAddress () << ": Switching to channel idle");
      m_state = RUNNING;
      StartTimer ();

    }

}
void
UanMacCw::NotifyTxStart (Time duration)
{

  if (m_txEndEvent.IsRunning ())
    {
      Simulator::Cancel (m_txEndEvent);
    }

  m_txEndEvent = Simulator::Schedule (duration, &UanMacCw::EndTx, this);
  NS_LOG_DEBUG ("Time " << Simulator::Now ().GetSeconds () << " scheduling TxEndEvent with delay " << duration.GetSeconds ());
  if (m_state == RUNNING)
    {
      NS_ASSERT (0);
      m_state = CCABUSY;
      SaveTimer ();

    }

}

int64_t
UanMacCw::AssignStreams (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  m_rv->SetStream (stream);
  return 1;
}

void
UanMacCw::EndTx (void)
{
  NS_ASSERT (m_state == TX || m_state == CCABUSY);
  if (m_state == TX)
    {
      m_state = IDLE;
    }
  else if (m_state == CCABUSY)
    {
      if (m_phy->IsStateIdle ())
        {
          NS_LOG_DEBUG ("Time " << Simulator::Now ().GetSeconds () << " Addr " << GetAddress () << ": Switching to channel idle (After TX!)");
          m_state = RUNNING;
          StartTimer ();
        }
    }
  else
    {
      NS_FATAL_ERROR ("In strange state at UanMacCw EndTx");
    }
}
void
UanMacCw::SetCw (uint32_t cw)
{
  m_cw = cw;
}
void
UanMacCw::SetSlotTime (Time duration)
{
  m_slotTime = duration;
}
uint32_t
UanMacCw::GetCw (void)
{
  return m_cw;
}
Time
UanMacCw::GetSlotTime (void)
{
  return m_slotTime;
}
void
UanMacCw::PhyRxPacketGood (Ptr<Packet> packet, double sinr, UanTxMode mode)
{
  UanHeaderCommon header;
  packet->RemoveHeader (header);

  if (header.GetDest () == m_address || header.GetDest () == UanAddress::GetBroadcast ())
    {
      m_forwardUpCb (packet, header.GetSrc ());
    }
}
void
UanMacCw::PhyRxPacketError (Ptr<Packet> packet, double sinr)
{

}
void
UanMacCw::SaveTimer (void)
{
  NS_LOG_DEBUG ("Time " << Simulator::Now ().GetSeconds () << " Addr " << GetAddress () << " Saving timer (Delay = " << (m_savedDelayS = m_sendTime - Simulator::Now ()).GetSeconds () << ")");
  NS_ASSERT (m_pktTx);
  NS_ASSERT (m_sendTime >= Simulator::Now ());
  m_savedDelayS = m_sendTime - Simulator::Now ();
  Simulator::Cancel (m_sendEvent);


}
void
UanMacCw::StartTimer (void)
{

  m_sendTime = Simulator::Now () + m_savedDelayS;
  if (m_sendTime == Simulator::Now ())
    {
      SendPacket ();
    }
  else
    {
      m_sendEvent = Simulator::Schedule (m_savedDelayS, &UanMacCw::SendPacket, this);
      NS_LOG_DEBUG ("Time " << Simulator::Now ().GetSeconds () << " Addr " << GetAddress () << " Starting timer (New send time = " << this->m_sendTime.GetSeconds () << ")");
    }
}

void
UanMacCw::SendPacket (void)
{
  NS_LOG_DEBUG ("Time " << Simulator::Now ().GetSeconds () << " Addr " << GetAddress () << " Transmitting ");
  NS_ASSERT (m_state == RUNNING);
  m_state = TX;
  m_phy->SendPacket (m_pktTx,m_pktTxProt);
  m_pktTx = 0;
  m_sendTime = Seconds (0);
  m_savedDelayS = Seconds (0);
}

} // namespace ns3
