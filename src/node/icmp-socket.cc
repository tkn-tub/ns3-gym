/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 Louis Pasteur University
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
 * Author: Sebastien Vincent <vincent@clarinet.u-strasbg.fr>
 */

#include "ns3/object.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "icmp-socket.h"

NS_LOG_COMPONENT_DEFINE ("IcmpSocket");

namespace ns3 {

  NS_OBJECT_ENSURE_REGISTERED (IcmpSocket);

  TypeId IcmpSocket::GetTypeId (void)
  {
    static TypeId tid = TypeId ("ns3::IcmpSocket")
      .SetParent<Socket> ()
      .AddAttribute ("RcvBufSize",
          "IcmpSocket maximum receive buffer size (bytes)",
          UintegerValue (0xffffffffl),
          MakeUintegerAccessor (&IcmpSocket::GetRcvBufSize,
            &IcmpSocket::SetRcvBufSize),
          MakeUintegerChecker<uint32_t> ())
      ;
    return tid;
  }

  IcmpSocket::IcmpSocket (void)
  {
    NS_LOG_FUNCTION_NOARGS ();
  }

  IcmpSocket::~IcmpSocket (void)
  {
    NS_LOG_FUNCTION_NOARGS ();
  }

}; /* namespace ns3 */

