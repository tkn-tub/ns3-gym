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
 * Author: Nicola Baldo  <nbaldo@cttc.es>
 * Author: Marco Miozzo <mmiozzo@cttc.es>
 */

#ifndef LTE_UE_MAC_ENTITY_H
#define LTE_UE_MAC_ENTITY_H



#include <map>

#include <ns3/lte-mac-sap.h>
#include <ns3/lte-ue-cmac-sap.h>
#include <ns3/lte-ue-phy-sap.h>
#include <ns3/nstime.h>


namespace ns3 {


class LteUeMac :   public Object
{
  friend class UeMemberLteUeCmacSapProvider;
  friend class UeMemberLteMacSapProvider;
  friend class UeMemberLteUePhySapUser;

public:
  static TypeId GetTypeId (void);

  LteUeMac ();
  virtual ~LteUeMac ();
  virtual void DoDispose (void);

  LteMacSapProvider*  GetLteMacSapProvider (void);
  void  SetLteUeCmacSapUser (LteUeCmacSapUser* s);
  LteUeCmacSapProvider*  GetLteUeCmacSapProvider (void);

  /**
  * \brief Get the PHY SAP user
  * \return a pointer to the SAP user of the PHY
  */
  LteUePhySapUser* GetLteUePhySapUser ();

  /**
  * \brief Set the PHY SAP Provider
  * \param s a pointer to the PHY SAP Provider
  */
  void SetLteUePhySapProvider (LteUePhySapProvider* s);
  
  /**
  * \brief Forwarded from LteUePhySapUser: trigger the start from a new frame
  *
  * \param frameNo frame number
  * \param subframeNo subframe number
  */
  void DoSubframeIndication (uint32_t frameNo, uint32_t subframeNo);

private:
  // forwarded from MAC SAP
  void DoTransmitPdu (LteMacSapProvider::TransmitPduParameters params);
  void DoReportBufferStatus (LteMacSapProvider::ReportBufferStatusParameters params);

  // forwarded from UE CMAC SAP
  void DoConfigureUe (uint16_t rnti);
  void DoAddLc (uint8_t lcId, LteMacSapUser* msu);
  void DoRemoveLc (uint8_t lcId);
  void DoRrcUpdateConfigurationReq (LteUeConfig_t params);

  // forwarded from PHY SAP
  void DoReceivePhyPdu (Ptr<Packet> p);
  void DoReceiveIdealControlMessage (Ptr<IdealControlMessage> msg);
  
  void SendReportBufferStatus (void);

private:
  // end of temporary hack
  std::map <uint8_t, LteMacSapUser*> m_macSapUserMap;
  LteMacSapProvider* m_macSapProvider;

  LteUeCmacSapUser* m_cmacSapUser;
  LteUeCmacSapProvider* m_cmacSapProvider;

  LteUePhySapProvider* m_uePhySapProvider;
  LteUePhySapUser* m_uePhySapUser;
  
  std::map <uint8_t, uint64_t> m_ulBsrReceived; // BSR received from RLC (BSR up to now)
  
  Time m_bsrPeriodicity;
  Time m_bsrLast;
  
  bool m_freshUlBsr; // true when a BSR has been received in the last TTI


  uint16_t m_rnti;

};

} // namespace ns3

#endif // LTE_UE_MAC_ENTITY
