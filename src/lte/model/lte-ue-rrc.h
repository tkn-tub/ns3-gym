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

#ifndef LTE_UE_RRC_H
#define LTE_UE_RRC_H

#include <ns3/object.h>
#include <ns3/packet.h>
#include <ns3/lte-ue-cmac-sap.h>
#include <ns3/lte-pdcp-sap.h>
#include <ns3/lte-as-sap.h>
#include <ns3/lte-ue-cphy-sap.h>

#include <map>

namespace ns3 {

class LteRlc;
class LteMacSapProvider;
class LteUeCmacSapUser;
class LteUeCmacSapProvider;
class LteRadioBearerInfo;
class LteEnbRrc;

/**
 *
 *
 */
class LteUeRrc : public Object
{

  friend class UeMemberLteUeCmacSapUser;
  friend class UeRrcMemberLteEnbCmacSapUser;
  friend class LtePdcpSpecificLtePdcpSapUser<LteUeRrc>;
  friend class MemberLteAsSapProvider<LteUeRrc>;
  friend class MemberLteUeCphySapUser<LteUeRrc>;

public:
  /**
   * create an RRC instance for use within an ue
   *
   */
  LteUeRrc ();


  /**
   * Destructor
   */
  virtual ~LteUeRrc ();


  // inherited from Object
  virtual void DoDispose (void);
  static TypeId GetTypeId (void);


  /**
   * set the CPHY SAP this RRC should use to interact with the PHY
   *
   * \param s the CPHY SAP Provider
   */
  void SetLteUeCphySapProvider (LteUeCphySapProvider * s);

  /**
   *
   *
   * \return s the CPHY SAP User interface offered to the PHY by this RRC
   */
  LteUeCphySapUser* GetLteUeCphySapUser ();

  /**
   * set the CMAC SAP this RRC should interact with
   *
   * \param s the CMAC SAP Provider to be used by this RRC
   */
  void SetLteUeCmacSapProvider (LteUeCmacSapProvider * s);

  /**
   *
   *
   * \return s the CMAC SAP User interface offered to the MAC by this RRC
   */
  LteUeCmacSapUser* GetLteUeCmacSapUser ();

  /**
   * set the MAC SAP provider. The ue RRC does not use this
   * directly, but it needs to provide it to newly created RLC instances.
   *
   * \param s the MAC SAP provider that will be used by all
   * newly created RLC instances
   */
  void SetLteMacSapProvider (LteMacSapProvider* s);

  /** 
   * Set the AS SAP user to interact with the NAS entity
   * 
   * \param s the AS SAP user
   */
  void SetAsSapUser (LteAsSapUser* s);

  /** 
   * 
   * 
   * \return the AS SAP provider exported by this RRC
   */
  LteAsSapProvider* GetAsSapProvider ();

  /** 
   * 
   * 
   * \param imsi the unique UE identifier
   */
  void SetImsi (uint64_t imsi);

  /**
   * Set UE RRC parameters
   *
   * \param rnti  C-RNTI of the UE
   * \param cellId Serving cell identifier
   */
  void ConfigureUe (uint16_t rnti, uint16_t cellId);



  /**
   * Setup a new radio bearer for the given user
   *
   * \param bearer the characteristics of the bearer to be activated
   * \param rlcTypeId the TypeId identifying the type of RLC to be used for this bearer.
   * \param lcid the logical channel id allocated for this bearer by the eNB
   * \param tft the Traffic Flow Template identifying this bearer
   *
   */
  void SetupRadioBearer (EpsBearer bearer, TypeId rlcTypeId, uint8_t lcid);


  /**
   *
   * Release the given radio bearer
   *
   * \param lcId the logical channel id of the bearer to be released
   */
  void ReleaseRadioBearer (uint8_t lcId);


  /**
   *
   * \return the C-RNTI of the user
   */
  uint16_t GetRnti () const;


  /**
   *
   * \return the CellId of the attached Enb
   */
  uint16_t GetCellId () const;

  /**
   *
   * @return a vector with the allocated LCID
   */
  std::vector<uint8_t> GetLcIdVector ();


  /** 
   * reception of the RRC ConnectionReconfiguration message
   *
   */
  void DoRecvConnectionSetup (LteUeConfig_t params);

  /** 
   * reception of the RRC ConnectionReconfiguration message
   *
   */
  void DoRecvConnectionReconfiguration (LteUeConfig_t params);


  /** 
   * Execute a handover
   * 
   * \param targetCellId 
   * \param newRnti 
   */
  void DoRecvConnectionReconfigurationWithMobilityControlInfo (uint16_t targetCellId, uint16_t newRnti);
  
private:

  // PDCP SAP methods
  void DoReceiveRrcPdu (LtePdcpSapUser::ReceiveRrcPduParameters params);

  // CMAC SAP methods
  void DoLcConfigCompleted ();

  // LTE AS SAP methods
  void DoForceCampedOnEnb (Ptr<LteEnbNetDevice> enbDevice, uint16_t cellId);
  void DoConnect ();
  void DoSendData (Ptr<Packet> packet, uint8_t bid);

  LteUeCphySapUser* m_cphySapUser;
  LteUeCphySapProvider* m_cphySapProvider;

  LteUeCmacSapUser* m_cmacSapUser;
  LteUeCmacSapProvider* m_cmacSapProvider;

  LteMacSapProvider* m_macSapProvider;
  LtePdcpSapUser* m_pdcpSapUser;
  
  LteAsSapProvider* m_asSapProvider;
  LteAsSapUser* m_asSapUser;

  uint64_t m_imsi;
  uint16_t m_rnti;
  uint16_t m_cellId;

  std::map <uint8_t, Ptr<LteRadioBearerInfo> > m_rbMap;  

  Ptr<LteEnbRrc> m_enbRrc; // wild hack, might go away in future versions
  
};


} // namespace ns3

#endif // LTE_UE_RRC_H
