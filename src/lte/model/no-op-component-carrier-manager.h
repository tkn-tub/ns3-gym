/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Danilo Abrignani
 * Copyright (c) 2016 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Authors: Danilo Abrignani <danilo.abrignani@unibo.it>
 *          Biljana Bojovic <biljana.bojovic@cttc.es>
 */

#ifndef NO_OP_COMPONENT_CARRIER_MANAGER_H
#define NO_OP_COMPONENT_CARRIER_MANAGER_H

#include <ns3/lte-enb-component-carrier-manager.h>
#include <ns3/lte-ccm-rrc-sap.h>
#include <ns3/lte-rrc-sap.h>
#include <map>

namespace ns3 {

class UeManager;
class LteCcmRrcSapProvider;

/**
 * \brief The default component carrier manager that forwards all traffic, the uplink and the downlink,
 *  over the primary carrier, and will not use secondary carriers. To enable carrier aggregation
 *  feature, select another component carrier manager class, i.e., some of child classes of
 *  LteEnbComponentCarrierManager of NoOpComponentCarrierManager.
 */

class NoOpComponentCarrierManager : public LteEnbComponentCarrierManager
{
  friend class EnbMacMemberLteMacSapProvider<NoOpComponentCarrierManager>;
  friend class MemberLteCcmRrcSapProvider<NoOpComponentCarrierManager>;
  friend class MemberLteCcmRrcSapUser<NoOpComponentCarrierManager>;
  friend class MemberLteCcmMacSapUser<NoOpComponentCarrierManager>;

public:

  NoOpComponentCarrierManager ();
  virtual ~NoOpComponentCarrierManager ();
  // Inherited method
  static TypeId GetTypeId ();

protected:
  // Inherited methods
  virtual void DoInitialize ();
  virtual void DoDispose ();
  virtual void DoReportUeMeas (uint16_t rnti, LteRrcSap::MeasResults measResults);
  virtual void DoAddUe (uint16_t rnti, uint8_t state);
  virtual void DoAddLc (LteEnbCmacSapProvider::LcInfo lcInfo, LteMacSapUser* msu);
  virtual std::vector<LteCcmRrcSapProvider::LcsConfig> DoSetupDataRadioBearer (EpsBearer bearer, uint8_t bearerId, uint16_t rnti, uint8_t lcid, uint8_t lcGroup, LteMacSapUser* msu);
  virtual void DoTransmitPdu (LteMacSapProvider::TransmitPduParameters params);
  virtual void DoReportBufferStatus (LteMacSapProvider::ReportBufferStatusParameters params);
  virtual void DoNotifyTxOpportunity (uint32_t bytes, uint8_t layer, uint8_t harqId, uint8_t componentCarrierId, uint16_t rnti, uint8_t lcid);
  virtual void DoReceivePdu (Ptr<Packet> p, uint16_t rnti, uint8_t lcid);
  virtual void DoNotifyHarqDeliveryFailure ();
  virtual void DoRemoveUe (uint16_t rnti);
  virtual std::vector<uint8_t> DoReleaseDataRadioBearer (uint16_t rnti, uint8_t lcid);
  virtual LteMacSapUser* DoConfigureSignalBearer(LteEnbCmacSapProvider::LcInfo lcinfo,  LteMacSapUser* msu);
  /*
   * \brief Forwards uplink BSR to CCM, called by MAC through CCM SAP interface..
   */
  virtual void DoUlReceiveMacCe (MacCeListElement_s bsr, uint8_t componentCarrierId);
  /*
   * \brief Function implements the function of the SAP interface of CCM instance which is used by MAC
   * to notify the PRB occupancy reported by scheduler.
   */
  virtual void DoNotifyPrbOccupancy (double prbOccupancy, uint8_t componentCarrierId);

protected:

  std::map <uint8_t, double > m_ccPrbOccupancy;//!< The physical resource block occupancy per carrier.

}; // end of class NoOpComponentCarrierManager


/*
 * \brief Component carrier manager implementation that splits traffic equally among carriers.
 */
class RrComponentCarrierManager : public NoOpComponentCarrierManager
{
public:

  RrComponentCarrierManager ();
  virtual ~RrComponentCarrierManager ();
  // Inherited method
  static TypeId GetTypeId ();

protected:

  // Inherited methods
  virtual void DoReportBufferStatus (LteMacSapProvider::ReportBufferStatusParameters params);
  virtual void DoUlReceiveMacCe (MacCeListElement_s bsr, uint8_t componentCarrierId);

}; // end of class RrComponentCarrierManager

} // end of namespace ns3


#endif /* NO_OP_COMPONENT_CARRIER_MANAGER_H */
