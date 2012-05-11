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
#include <ns3/epc-tft-classifier.h>
#include <ns3/lte-pdcp-sap.h>

#include <map>

namespace ns3 {

class LteRlc;
class LteMacSapProvider;
class LteUeCmacSapUser;
class LteUeCmacSapProvider;
class LteRadioBearerInfo;
class LtePdcpSapUser;
class LtePdcpSapProvider;


/**
 *
 *
 */
class LteUeRrc : public Object
{

  friend class UeMemberLteUeCmacSapUser;
  friend class UeRrcMemberLteEnbCmacSapUser;
  friend class LtePdcpSpecificLtePdcpSapUser<LteUeRrc>;

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
   * Set UE RRC parameters
   *
   * \param rnti  C-RNTI of the UE
   * \param cellId Serving cell identifier
   */
  void ConfigureUe (uint16_t rnti, uint16_t cellId);

  /**
   * Setup a new radio bearer for the given user
   *
   * \param rnti the RNTI of the user
   * \param bearer the characteristics of the bearer to be activated
   * \param rlcTypeId the TypeId identifying the type of RLC to be used for this bearer.
   * \param lcid the logical channel id allocated for this bearer by the eNB
   * \param tft the Traffic Flow Template identifying this bearer
   *
   */
  void SetupRadioBearer (uint16_t rnti, EpsBearer bearer, TypeId rlcTypeId, uint8_t lcid, Ptr<EpcTft> tft);


  /**
   *
   * Release the given radio bearer
   *
   * \param rnti the C-RNTI  of the user owning the bearer
   * \param lcId the logical channel id of the bearer to be released
   */
  void ReleaseRadioBearer (uint16_t rnti, uint8_t lcId);


  /**
   *
   * \return the C-RNTI of the user
   */
  uint16_t GetRnti ();


  /**
   *
   * \return the CellId of the attached Enb
   */
  uint16_t GetCellId ();

  /**
   *
   * @return a vector with the allocated LCID
   */
  std::vector<uint8_t> GetLcIdVector ();


  /** 
   * Enqueue an IP packet on the proper bearer for uplink transmission
   * 
   * \param p the packet
   * 
   * \return true if successful, false if an error occurred
   */
  bool Send (Ptr<Packet> p);

  /** 
   * set the callback used to forward data packets up the stack
   * 
   * \param void 
   * \param cb 
   */
  void SetForwardUpCallback (Callback <void, Ptr<Packet> > cb);
  
  /** 
  * message from eNB-RRC for changing UE's configuration
  * (up to now TxMode)
  *
  */
  void DoRrcConfigurationUpdateInd (LteUeConfig_t params);
  
private:

  void DoReceiveRrcPdu (LtePdcpSapUser::ReceiveRrcPduParameters params);

  // forwarded from CMAC SAP user
  void DoLcConfigCompleted ();

  Callback <void, Ptr<Packet> > m_forwardUpCallback;

  LteUeCmacSapUser* m_cmacSapUser;
  LteUeCmacSapProvider* m_cmacSapProvider;

  LteMacSapProvider* m_macSapProvider;
  LtePdcpSapUser* m_pdcpSapUser;

  uint16_t m_rnti;
  uint16_t m_cellId;

  std::map <uint8_t, Ptr<LteRadioBearerInfo> > m_rbMap;
  
  EpcTftClassifier m_tftClassifier;
  
};


} // namespace ns3

#endif // LTE_UE_RRC_H
