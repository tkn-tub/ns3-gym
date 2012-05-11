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
 *         Marco Miozzo <mmiozzo@cttc.es>
 */

#ifndef LTE_ENB_CMAC_SAP_H
#define LTE_ENB_CMAC_SAP_H

#include <ns3/packet.h>
#include <ns3/ff-mac-common.h>
#include <ns3/eps-bearer.h>
#include <ns3/lte-common.h>
#include <ns3/ff-mac-csched-sap.h>

namespace ns3 {


class LteMacSapUser;

/**
 * Service Access Point (SAP) offered by the eNB MAC to the eNB RRC
 * See Femto Forum MAC Scheduler Interface Specification v 1.11, Figure 1
 *
 * This is the MAC SAP Provider, i.e., the part of the SAP that contains the MAC methods called by the RRC
 */
class LteEnbCmacSapProvider
{
public:
  virtual ~LteEnbCmacSapProvider ();
  /**
   *
   *
   * @param ulBandwidth
   * @param dlBandwidth
   */
  virtual void ConfigureMac (uint8_t ulBandwidth,
                             uint8_t dlBandwidth) = 0;


  virtual void AddUe (uint16_t rnti) = 0;


  /**
   * Logical Channel information to be passed to CmacSapProvider::ConfigureLc
   *
   */
  struct LcInfo
  {
    uint16_t rnti;   /**< C-RNTI identifying the UE */
    uint8_t  lcId;    /**< logical channel identifier */
    uint8_t  lcGroup; /**< logical channel group */
    uint8_t  qci;     /**< QoS Class Identifier */
    bool     isGbr;   /**< true if the bearer is GBR, false if the bearer is NON-GBR */
    uint64_t mbrUl;   /**< maximum bitrate in uplink */
    uint64_t mbrDl;   /**< maximum bitrate in downlink */
    uint64_t gbrUl;   /**< guaranteed bitrate in uplink */
    uint64_t gbrDl;   /**< guaranteed bitrate in downlink */
  };

  /**
   * Add a new logical channel
   *
   * \param lcinfo
   * \param msu
   */
  virtual void AddLc (LcInfo lcinfo, LteMacSapUser* msu) = 0;


  /**
   * Reconfigure an existing logical channel
   *
   * \param lcinfo
   */
  virtual void ReconfigureLc (LcInfo lcinfo) = 0;


  /**
   * release an existing logical channel
   *
   * \param rnti
   * \param lcid
   */
  virtual void ReleaseLc (uint16_t rnti, uint8_t lcid) = 0;
  
  virtual void RrcUpdateConfigurationReq (FfMacCschedSapProvider::CschedUeConfigReqParameters params) = 0;


};



/**
 * Service Access Point (SAP) offered by the MAC to the RRC
 * See Femto Forum MAC Scheduler Interface Specification v 1.11, Figure 1
 *
 * This is the MAC SAP User, i.e., the part of the SAP that contains the RRC methods called by the MAC
 */
class LteEnbCmacSapUser
{
public:
  virtual ~LteEnbCmacSapUser ();
  /**
   * notify the result of the last LC config operation
   *
   * \param rnti the rnti of the user
   * \param lcid the logical channel id
   * \param success true if the operation was successful, false otherwise
   */
  virtual void NotifyLcConfigResult (uint16_t rnti, uint8_t lcid, bool success) = 0;
  virtual void RrcConfigurationUpdateInd (LteUeConfig_t params) = 0;
};







} // namespace ns3


#endif // MAC_SAP_H
