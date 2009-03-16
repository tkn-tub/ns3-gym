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


#include "ns3/dot11s-parameters.h"

namespace ns3 {

uint8_t dot11sParameters::dot11MeshMaxRetries      = 4;
Time    dot11sParameters::dot11MeshRetryTimeout    = TU_TO_TIME(40);
Time    dot11sParameters::dot11MeshHoldingTimeout  = TU_TO_TIME(40);
Time    dot11sParameters::dot11MeshConfirmTimeout  = TU_TO_TIME(40);


uint8_t dot11sParameters::dot11MeshHWMPmaxPREQretries           = 3;
Time    dot11sParameters::dot11MeshHWMPnetDiameterTraversalTime = TU_TO_TIME(10);
Time    dot11sParameters::dot11MeshHWMPpreqMinInterval          = TU_TO_TIME(100);
Time    dot11sParameters::dot11MeshHWMPperrMinInterval          = TU_TO_TIME(100);
Time    dot11sParameters::dot11MeshHWMPactiveRootTimeout        = TU_TO_TIME(5000);
Time    dot11sParameters::dot11MeshHWMPactivePathTimeout        = TU_TO_TIME(5000);
Time    dot11sParameters::dot11MeshHWMPpathToRootInterval       = TU_TO_TIME(5000);
Time    dot11sParameters::dot11MeshHWMPrannInterval             = TU_TO_TIME(1000);

}
