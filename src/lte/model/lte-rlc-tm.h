/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011,2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 *         Nicola Baldo <nbaldo@cttc.es>
 */

#ifndef LTE_RLC_TM_H
#define LTE_RLC_TM_H

#include "ns3/lte-rlc.h"

#include <ns3/event-id.h>
#include <map>

namespace ns3 {

/**
 * LTE RLC Transparent Mode (TM), see 3GPP TS 36.322
 */
class LteRlcTm : public LteRlc
{
public:
  LteRlcTm ();
  virtual ~LteRlcTm ();
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  virtual void DoDispose ();

  /**
   * RLC SAP
   *
   * \param p packet
   */
  virtual void DoTransmitPdcpPdu (Ptr<Packet> p);

  /**
   * MAC SAP
   * 
   * \param txOpParams the LteMacSapUser::TxOpportunityParameters
   */
  virtual void DoNotifyTxOpportunity (LteMacSapUser::TxOpportunityParameters txOpParams);
  /**
   * Notify HARQ deliver failure
   */
  virtual void DoNotifyHarqDeliveryFailure ();
  virtual void DoReceivePdu (LteMacSapUser::ReceivePduParameters rxPduParams);

private:
  /// Expire RBS timer function
  void ExpireRbsTimer (void);
  /// Report buffer status
  void DoReportBufferStatus ();

private:
  uint32_t m_maxTxBufferSize; ///< maximum transmit buffer size
  uint32_t m_txBufferSize; ///< transmit buffer size
  std::vector < Ptr<Packet> > m_txBuffer; ///< Transmission buffer

  EventId m_rbsTimer; ///< RBS timer

};


} // namespace ns3

#endif // LTE_RLC_TM_H
