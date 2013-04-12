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

#ifndef LTE_RADIO_BEARER_INFO_H
#define LTE_RADIO_BEARER_INFO_H

#include <ns3/object.h>
#include <ns3/pointer.h>
#include <ns3/eps-bearer.h>
#include <ns3/lte-rrc-sap.h>
#include <ns3/ipv4-address.h>

namespace ns3 {

class LteRlc;
class LtePdcp;

/**
 * store information on active radio bearer instance
 * 
 */
class LteRadioBearerInfo : public Object
{

public:
  LteRadioBearerInfo (void);
  virtual ~LteRadioBearerInfo (void);
  static TypeId GetTypeId (void);

  Ptr<LteRlc> m_rlc;
  Ptr<LtePdcp> m_pdcp;  
};


/**
 * store information on active signaling radio bearer instance
 * 
 */
class LteSignalingRadioBearerInfo : public LteRadioBearerInfo
{

public:
  static TypeId GetTypeId (void);

  uint8_t m_srbIdentity;   
  LteRrcSap::LogicalChannelConfig m_logicalChannelConfig;  
};


/**
 * store information on active data radio bearer instance
 * 
 */
class LteDataRadioBearerInfo : public LteRadioBearerInfo
{

public:
  static TypeId GetTypeId (void);

  EpsBearer m_epsBearer;
  uint8_t m_epsBearerIdentity;
  uint8_t m_drbIdentity;
  LteRrcSap::RlcConfig m_rlcConfig;
  uint8_t m_logicalChannelIdentity;
  LteRrcSap::LogicalChannelConfig m_logicalChannelConfig;
  uint32_t m_gtpTeid; /**< S1-bearer GTP tunnel endpoint identifier, see 36.423 9.2.1 */
  Ipv4Address m_transportLayerAddress; /**< IP Address of the SGW, see 36.423 9.2.1 */
};





} // namespace ns3


#endif // LTE_RADIO_BEARER_INFO_H
