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
 * Author: Marco Miozzo <mmiozzo@cttc.es> : Update to FF API Architecture
 * Author: Nicola Baldo <nbaldo@cttc.es>  : Integrated with new RRC and MAC architecture
 */

#include <ns3/llc-snap-header.h>
#include <ns3/simulator.h>
#include <ns3/callback.h>
#include <ns3/node.h>
#include <ns3/packet.h>
#include <ns3/lte-net-device.h>
#include <ns3/packet-burst.h>
#include <ns3/uinteger.h>
#include <ns3/trace-source-accessor.h>
#include <ns3/pointer.h>
#include <ns3/enum.h>
#include <ns3/lte-amc.h>
#include <ns3/lte-enb-mac.h>
#include <ns3/lte-enb-net-device.h>
#include <ns3/lte-enb-rrc.h>
#include <ns3/lte-ue-net-device.h>
#include <ns3/lte-enb-phy.h>
#include <ns3/ff-mac-scheduler.h>


NS_LOG_COMPONENT_DEFINE ("LteEnbNetDevice");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED ( LteEnbNetDevice);

uint16_t LteEnbNetDevice::m_cellIdCounter = 0;

TypeId LteEnbNetDevice::GetTypeId (void)
{
  static TypeId
  tid =
    TypeId ("ns3::LteEnbNetDevice")
    .SetParent<LteNetDevice> ()
    .AddConstructor<LteEnbNetDevice> ()
    .AddAttribute ("LteEnbRrc",
                   "The RRC associated to this EnbNetDevice",               
                   PointerValue (),
                   MakePointerAccessor (&LteEnbNetDevice::m_rrc),
                   MakePointerChecker <LteEnbRrc> ())
    ;
  return tid;
}

LteEnbNetDevice::LteEnbNetDevice (void)
{
  NS_LOG_FUNCTION (this);
  NS_FATAL_ERROR ("This constructor should not be called");
  InitLteEnbNetDevice ();
}

LteEnbNetDevice::LteEnbNetDevice (Ptr<Node> node, Ptr<LteEnbPhy> phy, Ptr<LteEnbMac> mac, Ptr<FfMacScheduler> sched, Ptr<LteEnbRrc> rrc)
{
  NS_LOG_FUNCTION (this);
  m_phy = phy;
  m_mac = mac;
  m_scheduler = sched;
  m_rrc = rrc;
  InitLteEnbNetDevice ();
  SetNode (node);
}

LteEnbNetDevice::~LteEnbNetDevice (void)
{
  NS_LOG_FUNCTION (this);
}

void
LteEnbNetDevice::DoDispose ()
{
  NS_LOG_FUNCTION (this);

  m_mac->Dispose ();
  m_mac = 0;

  m_scheduler->Dispose ();
  m_scheduler = 0;

  m_rrc->Dispose ();
  m_rrc = 0;

  m_phy->Dispose ();
  m_phy = 0;

  LteNetDevice::DoDispose ();
}


void
LteEnbNetDevice::InitLteEnbNetDevice (void)
{
  NS_LOG_FUNCTION (this);

  SetNode (0);
  if (GetPhy () == 0)
    {
      NS_LOG_DEBUG (this << "PHY NULL");
    }
  else
    {
      NS_LOG_DEBUG (this << "PHY ! NULL");
    }
  
  m_rrc->SetLteEnbCmacSapProvider (m_mac->GetLteEnbCmacSapProvider ());
  m_mac->SetLteEnbCmacSapUser (m_rrc->GetLteEnbCmacSapUser ());
  m_rrc->SetLteMacSapProvider (m_mac->GetLteMacSapProvider ());

  m_mac->SetFfMacSchedSapProvider (m_scheduler->GetFfMacSchedSapProvider ());
  m_mac->SetFfMacCschedSapProvider (m_scheduler->GetFfMacCschedSapProvider ());

  m_scheduler->SetFfMacSchedSapUser (m_mac->GetFfMacSchedSapUser ());
  m_scheduler->SetFfMacCschedSapUser (m_mac->GetFfMacCschedSapUser ());

  GetPhy ()->GetObject<LteEnbPhy> ()->SetLteEnbPhySapUser (m_mac->GetLteEnbPhySapUser ());
  m_mac->SetLteEnbPhySapProvider (GetPhy ()->GetObject<LteEnbPhy> ()->GetLteEnbPhySapProvider ());

  m_rrc->ConfigureCell (25, 25);
  NS_ASSERT_MSG (m_cellIdCounter < 65535, "max num eNBs exceeded");
  m_cellId = ++m_cellIdCounter;

  // WILD HACK -  should use the PHY SAP instead. Probably should handle this through the RRC
  GetPhy ()->GetObject<LteEnbPhy> ()->DoSetBandwidth (25,25);
  GetPhy ()->GetObject<LteEnbPhy> ()->DoSetCellId (m_cellId);
  
  Simulator::ScheduleNow (&LteEnbPhy::StartFrame, GetPhy ()->GetObject<LteEnbPhy> ());
}


Ptr<LteEnbMac>
LteEnbNetDevice::GetMac (void)
{
  NS_LOG_FUNCTION (this);
  return m_mac;
}


Ptr<LteEnbPhy>
LteEnbNetDevice::GetPhy (void) const
{
  NS_LOG_FUNCTION (this);
  return m_phy;
}


bool
LteEnbNetDevice::DoSend (Ptr<Packet> packet, const Mac48Address& source,
                      const Mac48Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << source << dest << protocolNumber);

  NS_FATAL_ERROR ("IP connectivity not implemented yet");

  /*
   * The classification of traffic in DL is done by the PGW (not
   * by the eNB).
   * Hovever, the core network is not implemented yet.
   * For now the classification is managed by the eNB.
   */

  // if (protocolNumber == 2048)
  //   {
  //     // it is an IP packet
  //   }

  // if (protocolNumber != 2048 || bearer == 0)
  //   {

  //   }


  return true;
}


void
LteEnbNetDevice::DoReceive (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);
  ForwardUp (p->Copy ());
}


void
LteEnbNetDevice::SendIdealPdcchMessage (void)
{
  NS_LOG_FUNCTION (this);
  /*
   * Get both PDCCH ideal message for UL and DL and
   * set assigned resources to UEs using
   * SendAssignedDLResources and SendAssignedULResources
   */
}

Ptr<LteEnbRrc>
LteEnbNetDevice::GetRrc ()
{
  return m_rrc;
}

uint16_t
LteEnbNetDevice::GetCellId ()
{
  return m_cellId;
}

} // namespace ns3
