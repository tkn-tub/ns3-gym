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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */

#ifndef LTE_PDCP_SAP_H
#define LTE_PDCP_SAP_H

#include "ns3/packet.h"

namespace ns3 {

/**
 * Service Access Point (SAP) offered by the PDCP entity to the RRC entity
 * See 3GPP 36.323 Packet Data Convergence Protocol (PDCP) specification
 *
 * This is the PDCP SAP Provider
 * (i.e. the part of the SAP that contains the PDCP methods called by the RRC)
 */
class LtePdcpSapProvider
{
public:
  virtual ~LtePdcpSapProvider ();

  /**
   * Parameters for LtePdcpSapProvider::TransmitRrcPdu
   */
  struct TransmitRrcPduParameters
  {
    Ptr<Packet> rrcPdu;  /**< the RRC PDU */
    uint16_t    rnti; /**< the C-RNTI identifying the UE */
    uint8_t     lcid; /**< the logical channel id corresponding to the sending RLC instance */
  };

  /**
   * Send a RRC PDU to the RDCP for transmission
   * This method is to be called
   * when upper RRC entity has a RRC PDU ready to send   
   * 
   * \param params 
   */
  virtual void TransmitRrcPdu (TransmitRrcPduParameters params) = 0;
};


/**
 * Service Access Point (SAP) offered by the PDCP entity to the RRC entity
 * See 3GPP 36.323 Packet Data Convergence Protocol (PDCP) specification
 *
 * This is the PDCP SAP User
 * (i.e. the part of the SAP that contains the RRC methods called by the PDCP)
 */
class LtePdcpSapUser
{
public:
  virtual ~LtePdcpSapUser ();

  /**
   * Parameters for LtePdcpSapUser::ReceiveRrcPdu
   */
  struct ReceiveRrcPduParameters
  {
    Ptr<Packet> rrcPdu;  /**< the RRC PDU */
    uint16_t    rnti; /**< the C-RNTI identifying the UE */
    uint8_t     lcid; /**< the logical channel id corresponding to the sending RLC instance */
  };

  /**
  * Called by the PDCP entity to notify the RRC entity of the reception of a new RRC PDU
  *
  * \param params
  */
  virtual void ReceiveRrcPdu (ReceiveRrcPduParameters params) = 0;
};

///////////////////////////////////////

template <class C>
class LtePdcpSpecificLtePdcpSapProvider : public LtePdcpSapProvider
{
public:
  LtePdcpSpecificLtePdcpSapProvider (C* pdcp);

  // Interface implemented from LtePdcpSapProvider
  virtual void TransmitRrcPdu (TransmitRrcPduParameters params);

private:
  LtePdcpSpecificLtePdcpSapProvider ();
  C* m_pdcp;
};

template <class C>
LtePdcpSpecificLtePdcpSapProvider<C>::LtePdcpSpecificLtePdcpSapProvider (C* pdcp)
  : m_pdcp (pdcp)
{
}

template <class C>
LtePdcpSpecificLtePdcpSapProvider<C>::LtePdcpSpecificLtePdcpSapProvider ()
{
}

template <class C>
void LtePdcpSpecificLtePdcpSapProvider<C>::TransmitRrcPdu (TransmitRrcPduParameters params)
{
  m_pdcp->DoTransmitRrcPdu (params.rrcPdu);
}

///////////////////////////////////////

template <class C>
class LtePdcpSpecificLtePdcpSapUser : public LtePdcpSapUser
{
public:
  LtePdcpSpecificLtePdcpSapUser (C* rrc);

  // Interface implemented from LtePdcpSapUser
  virtual void ReceiveRrcPdu (ReceiveRrcPduParameters params);

private:
  LtePdcpSpecificLtePdcpSapUser ();
  C* m_rrc;
};

template <class C>
LtePdcpSpecificLtePdcpSapUser<C>::LtePdcpSpecificLtePdcpSapUser (C* rrc)
  : m_rrc (rrc)
{
}

template <class C>
LtePdcpSpecificLtePdcpSapUser<C>::LtePdcpSpecificLtePdcpSapUser ()
{
}

template <class C>
void LtePdcpSpecificLtePdcpSapUser<C>::ReceiveRrcPdu (ReceiveRrcPduParameters params)
{
  m_rrc->DoReceiveRrcPdu (params);
}


} // namespace ns3

#endif // LTE_PDCP_SAP_H
