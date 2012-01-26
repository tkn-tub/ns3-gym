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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */

#include "lte-radio-bearer-info.h"
#include "lte-ue-rrc.h"
#include "lte-rlc.h"
#include "lte-pdcp.h"

#include <ns3/log.h>



namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LteRadioBearerInfo);

LteRadioBearerInfo::LteRadioBearerInfo (void)
{
}

LteRadioBearerInfo::~LteRadioBearerInfo (void)
{
}

TypeId LteRadioBearerInfo::GetTypeId (void)
{
  static TypeId
  tid =
    TypeId ("ns3::LteRadioBearerInfo")
    .SetParent<Object> ()
    .AddConstructor<LteRadioBearerInfo> ()
    .AddAttribute ("LteRlc", "RLC instance of the radio bearer.",
                   PointerValue (),
                   MakePointerAccessor (&LteRadioBearerInfo::m_rlc),
                   MakePointerChecker<LteRlc> ())
    .AddAttribute ("LtePdcp", "PDCP instance of the radio bearer.",
                   PointerValue (),
                   MakePointerAccessor (&LteRadioBearerInfo::m_pdcp),
                   MakePointerChecker<LtePdcp> ())
  ;
  return tid;
}




} // namespace ns3
