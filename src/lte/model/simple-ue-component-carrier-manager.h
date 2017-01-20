/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Danilo Abrignani
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
 * Author: Danilo Abrignani <danilo.abrignani@unibo.it>
 *
 */

#ifndef SIMPLE_UE_COMPONENT_CARRIER_MANAGER_H
#define SIMPLE_UE_COMPONENT_CARRIER_MANAGER_H

#include <ns3/lte-ue-component-carrier-manager.h>
#include <ns3/lte-ue-ccm-rrc-sap.h>
#include <ns3/lte-rrc-sap.h>
#include <map>

namespace ns3 {
  class LteUeCcmRrcSapProvider;

/**
 * \brief Component carrier manager implementation which simply does nothing.
 *
 * Selecting this component carrier selection algorithm is equivalent to disabling automatic
 * triggering of component carrier selection. This is the default choice.
 *
 */
class SimpleUeComponentCarrierManager : public LteUeComponentCarrierManager
{
public:
  /// Creates a No-op CCS algorithm instance.
  SimpleUeComponentCarrierManager ();

  virtual ~SimpleUeComponentCarrierManager ();

  // inherited from Object
  static TypeId GetTypeId ();

  // inherited from LteComponentCarrierManager
  virtual void SetLteCcmRrcSapUser (LteUeCcmRrcSapUser* s);
  virtual LteUeCcmRrcSapProvider* GetLteCcmRrcSapProvider ();
  virtual LteMacSapProvider* GetLteMacSapProvider ();



  // let the forwarder class access the protected and private members
  friend class MemberLteUeCcmRrcSapProvider<SimpleUeComponentCarrierManager>;
  //friend class MemberLteUeCcmRrcSapUser<SimpleUeComponentCarrierManager>;
  
  friend class SimpleUeCcmMacSapProvider;
  friend class SimpleUeCcmMacSapUser;

protected:

  // inherited from Object
  virtual void DoInitialize ();
  virtual void DoDispose ();
  // inherited from LteCcsAlgorithm as a Component Carrier Management SAP implementation
  void DoReportUeMeas (uint16_t rnti, LteRrcSap::MeasResults measResults);
  // forwarded from LteMacSapProvider
  void DoTransmitPdu (LteMacSapProvider::TransmitPduParameters params);
  void DoReportBufferStatus (LteMacSapProvider::ReportBufferStatusParameters params);
  void DoNotifyHarqDeliveryFailure ();
  // forwarded from LteMacSapUser
  void DoNotifyTxOpportunity (uint32_t bytes, uint8_t layer, uint8_t harqId, uint8_t componentCarrierId, uint16_t rnti, uint8_t lcid);
  void DoReceivePdu (Ptr<Packet> p, uint16_t rnti, uint8_t lcid);
  //forwarded from LteUeCcmRrcSapProvider
  std::vector<LteUeCcmRrcSapProvider::LcsConfig> DoAddLc (uint8_t lcId,  LteUeCmacSapProvider::LogicalChannelConfig lcConfig, LteMacSapUser* msu);
  std::vector<uint16_t> DoRemoveLc (uint8_t lcid);
  void DoNotifyConnectionReconfigurationMsg ();
  LteMacSapUser* DoConfigureSignalBearer (uint8_t lcId,  LteUeCmacSapProvider::LogicalChannelConfig lcConfig, LteMacSapUser* msu);
  
private:
  
  LteUeCcmRrcSapUser* m_ccmRrcSapUser;//!< Interface to the eNodeB RRC instance.
  LteUeCcmRrcSapProvider* m_ccmRrcSapProvider; //!< Receive API calls from the eNodeB RRC instance.
  LteMacSapUser* m_ccmMacSapUser;//!< Interface to the eNodeB RLC instance.
  LteMacSapProvider* m_ccmMacSapProvider; //!< Receive API calls from the eNodeB RLC instance..

}; // end of class SimpleUeComponentCarrierManager


} // end of namespace ns3


#endif /* SIMPLE_UE_COMPONENT_CARRIER_MANAGER_H */
