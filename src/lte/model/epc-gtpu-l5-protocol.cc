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
 * Author: Jaume Nin <jnin@cttc.cat>
 */

#include "ns3/epc-gtpu-l5-protocol.h"
#include <ns3/log.h>
#include <ns3/uinteger.h>
#include "ns3/epc-gtpu-header.h"

NS_LOG_COMPONENT_DEFINE ("GtpuL5Protocol");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (GtpuL5Protocol);

TypeId 
GtpuL5Protocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::GtpuL5Protocol")
    .SetParent<Object> ()
    .AddConstructor<GtpuL5Protocol> ()
    /*.AddAttribute ("TEID", "Tunnel Endpoint Identifier of this instance.",
                   UintegerValue (0),
                   MakeUintegerAccessor (&GtpuL5Protocol::SetTeid,
                                         &GtpuL5Protocol::GetTeid),
        MakeUintegerChecker<uint32_t> ())*/
  ;
  return tid;
}

GtpuL5Protocol::GtpuL5Protocol ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

GtpuL5Protocol::GtpuL5Protocol (uint32_t teid)
{
  NS_LOG_FUNCTION (this);
  //this.SetAttribute("TEID", teid);
  m_teid = teid;

}


GtpuL5Protocol::~GtpuL5Protocol ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

Ptr<Packet>
GtpuL5Protocol::AddHeader (Ptr<Packet> p)
{
  GtpuHeader h;
  h.SetTeid (m_teid);
  // From 3GPP TS 29.281 v10.0.0 Section 5.1
  // Length of the payload + the non obligatory GTP-U header
  h.SetLength (p->GetSize () + h.GetSerializedSize () - 8);
  p->AddHeader (h);
  NS_LOG_FUNCTION (this << h);
  return p;
}

void
GtpuL5Protocol::RemoveHeader (Ptr<Packet> p)
{
  GtpuHeader h;
  p->RemoveHeader (h);
  NS_LOG_DEBUG (this << h);
  NS_ASSERT( h.GetTeid () == m_teid);
}

}; // namespace ns3
