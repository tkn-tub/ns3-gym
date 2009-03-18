/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Authors: Aleksey Kovalenko <kovalenko@iitp.ru>
 *          Kirill Andreev <andreev@iitp.ru>
 */


#ifndef DOT11S_PARAMETERS_H
#define DOT11S_PARAMETERS_H
#include "ns3/uinteger.h"
#include "ns3/nstime.h"

namespace ns3 {

#define TU_TO_TIME (x) MicroSeconds(x*1024)
#define TIME_TO_TU (x) x.GetMicroSeconds()/1024

/**
  * \ingroup mesh
*/
struct dot11sParameters
{
  /** Peer Link  */
  static uint8_t dot11MeshMaxRetries;
  static Time dot11MeshRetryTimeout;
  static Time dot11MeshHoldingTimeout;
  static Time dot11MeshConfirmTimeout;
  /** HWMP */
  static uint8_t dot11MeshHWMPmaxPREQretries;
  static Time dot11MeshHWMPnetDiameterTraversalTime;
  static Time dot11MeshHWMPpreqMinInterval;
  static Time dot11MeshHWMPperrMinInterval;
  static Time dot11MeshHWMPactiveRootTimeout;
  static Time dot11MeshHWMPactivePathTimeout;
  static Time dot11MeshHWMPpathToRootInterval;
  static Time dot11MeshHWMPrannInterval;
};

};
#endif
