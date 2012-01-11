/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

#ifndef FF_MAC_SCHED_SAP_H
#define FF_MAC_SCHED_SAP_H

#include <stdint.h>
#include <vector>

#include "ff-mac-common.h"


namespace ns3 {

/**
 * \ingroup ff-api
 * \defgroup FF-API FfMacSchedSap
 */
/**
 * \ingroup FfMacSchedSap
 * \brief Provides the SCHED SAP
 *
 * This abstract class defines the MAC Scheduler interface specified in the
 * Femto Forum Technical Document:
 *   - LTE MAC Scheduler Interface Specification v1.11
 *
 * The Technical Document contains a detailed description of the API.
 * The documentation of this class refers to sections of this Technical Document.
 *
 * You can found an example of the implementation of this interface
 * in the SampleFfMacSchedSapProvider and SampleFfMacSchedSapUser classes
 */
class FfMacSchedSapProvider
{
public:
  virtual ~FfMacSchedSapProvider ();

  /**
   * Parameters of the API primitives
   */

  /**
   * Parameters of the SCHED_DL_RLC_BUFFER_REQ primitive.
   * See section 4.2.1 for a detailed description of the parameters.
   */
  struct SchedDlRlcBufferReqParameters
  {
    uint16_t  m_rnti;
    uint8_t   m_logicalChannelIdentity;
    uint32_t  m_rlcTransmissionQueueSize;
    uint16_t  m_rlcTransmissionQueueHolDelay;
    uint32_t  m_rlcRetransmissionQueueSize;
    uint16_t  m_rlcRetransmissionHolDelay;
    uint16_t  m_rlcStatusPduSize;

    std::vector <struct VendorSpecificListElement_s> m_vendorSpecificList;
  };

  /**
   * Parameters of the SCHED_DL_PAGING_BUFFER_REQ primitive.
   * See section 4.2.2 for a detailed description of the parameters.
   */
  struct SchedDlPagingBufferReqParameters
  {
    uint16_t  m_rnti;
    std::vector <struct PagingInfoListElement_s> m_pagingInfoList;

    std::vector <struct VendorSpecificListElement_s> m_vendorSpecificList;
  };

  /**
   * Parameters of the SCHED_DL_MAC_BUFFER_REQ primitive.
   * See section 4.2.3 for a detailed description of the parameters.
   */
  struct SchedDlMacBufferReqParameters
  {
    uint16_t  m_rnti;
    enum CeBitmap_e m_ceBitmap;

    std::vector <struct VendorSpecificListElement_s> m_vendorSpecificList;
  };

  /**
   * Parameters of the SCHED_DL_TRIGGER_REQ primitive.
   * See section 4.2.4 for a detailed description of the parameters.
   */
  struct SchedDlTriggerReqParameters
  {
    uint16_t  m_sfnSf;
    std::vector <struct DlInfoListElement_s> m_dlInfoList;

    std::vector <struct VendorSpecificListElement_s> m_vendorSpecificList;
  };

  /**
   * Parameters of the SCHED_DL_RACH_INFO_REQ primitive.
   * See section 4.2.5 for a detailed description of the parameters.
   */
  struct SchedDlRachInfoReqParameters
  {
    uint16_t  m_sfnSf;
    std::vector <struct RachListElement_s> m_rachList;

    std::vector <struct VendorSpecificListElement_s> m_vendorSpecificList;
  };

  /**
   * Parameters of the SCHED_DL_CQI_INFO_REQ primitive.
   * See section 4.2.6 for a detailed description of the parameters.
   */
  struct SchedDlCqiInfoReqParameters
  {
    uint16_t  m_sfnSf;
    std::vector <struct CqiListElement_s> m_cqiList;

    std::vector <struct VendorSpecificListElement_s> m_vendorSpecificList;
  };

  /**
   * Parameters of the SCHED_UL_TRIGGER_REQ primitive.
   * See section 4.2.8 for a detailed description of the parameters.
   */
  struct SchedUlTriggerReqParameters
  {
    uint16_t  m_sfnSf;
    std::vector <struct UlInfoListElement_s> m_ulInfoList;

    std::vector <struct VendorSpecificListElement_s> m_vendorSpecificList;
  };

  /**
   * Parameters of the SCHED_UL_NOISE_INTERFERENCE_REQ primitive.
   * See section 4.2.9 for a detailed description of the parameters.
   */
  struct SchedUlNoiseInterferenceReqParameters
  {
    uint16_t  m_sfnSf;
    uint16_t  m_rip;
    uint16_t  m_tnp;

    std::vector <struct VendorSpecificListElement_s> m_vendorSpecificList;
  };

  /**
   * Parameters of the SCHED_UL_SR_INFO_REQ primitive.
   * See section 4.2.10 for a detailed description of the parameters.
   */
  struct SchedUlSrInfoReqParameters
  {
    uint16_t  m_sfnSf;
    std::vector <struct SrListElement_s> m_srList;

    std::vector <struct VendorSpecificListElement_s> m_vendorSpecificList;
  };

  /**
   * Parameters of the SCHED_UL_MAC_CTRL_INFO_REQ primitive.
   * See section 4.2.11 for a detailed description of the parameters.
   */
  struct SchedUlMacCtrlInfoReqParameters
  {
    uint16_t  m_sfnSf;
    std::vector <struct MacCeListElement_s> m_macCeList;

    std::vector <struct VendorSpecificListElement_s> m_vendorSpecificList;
  };

  /**
   * Parameters of the SCHED_UL_CQI_INFO_REQ primitive.
   * See section 4.2.12 for a detailed description of the parameters.
   */
  struct SchedUlCqiInfoReqParameters
  {
    uint16_t  m_sfnSf;
    struct UlCqi_s m_ulCqi;

    std::vector <struct VendorSpecificListElement_s> m_vendorSpecificList;
  };

  //
  // SCHED - MAC Scheduler SAP primitives
  // (See 4.2 for description of the primitives)
  //

  virtual void SchedDlRlcBufferReq (const struct SchedDlRlcBufferReqParameters& params) = 0;

  virtual void SchedDlPagingBufferReq (const struct SchedDlPagingBufferReqParameters& params) = 0;

  virtual void SchedDlMacBufferReq (const struct SchedDlMacBufferReqParameters& params) = 0;

  virtual void SchedDlTriggerReq (const struct SchedDlTriggerReqParameters& params) = 0;

  virtual void SchedDlRachInfoReq (const struct SchedDlRachInfoReqParameters& params) = 0;

  virtual void SchedDlCqiInfoReq (const struct SchedDlCqiInfoReqParameters& params) = 0;

  virtual void SchedUlTriggerReq (const struct SchedUlTriggerReqParameters& params) = 0;

  virtual void SchedUlNoiseInterferenceReq (const struct SchedUlNoiseInterferenceReqParameters& params) = 0;

  virtual void SchedUlSrInfoReq (const struct SchedUlSrInfoReqParameters& params) = 0;

  virtual void SchedUlMacCtrlInfoReq (const struct SchedUlMacCtrlInfoReqParameters& params) = 0;

  virtual void SchedUlCqiInfoReq (const struct SchedUlCqiInfoReqParameters& params) = 0;

private:
};


class FfMacSchedSapUser
{
public:
  virtual ~FfMacSchedSapUser ();

  /**
   * Parameters of the API primitives
   */

  /**
   * Parameters of the SCHED_DL_CONFIG_IND primitive.
   * See section 4.2.7 for a detailed description of the parameters.
   */
  struct SchedDlConfigIndParameters
  {
    std::vector <struct BuildDataListElement_s>      m_buildDataList;
    std::vector <struct BuildRarListElement_s>       m_buildRarList;
    std::vector <struct BuildBroadcastListElement_s> m_buildBroadcastList;

    uint8_t m_nrOfPdcchOfdmSymbols;

    std::vector <struct VendorSpecificListElement_s> m_vendorSpecificList;
  };

  /**
   * Parameters of the SCHED_UL_CONFIG_IND primitive.
   * See section 4.2.13 for a detailed description of the parameters.
   */
  struct SchedUlConfigIndParameters
  {
    std::vector <struct UlDciListElement_s> m_dciList;
    std::vector <struct PhichListElement_s> m_phichList;

    std::vector <struct VendorSpecificListElement_s> m_vendorSpecificList;
  };

  //
  // SCHED - MAC Scheduler SAP primitives
  // (See 4.2 for description of the primitives)
  //

  virtual void SchedDlConfigInd (const struct SchedDlConfigIndParameters& params) = 0;

  virtual void SchedUlConfigInd (const struct SchedUlConfigIndParameters& params) = 0;

private:
};

} // namespace ns3

#endif /* FF_MAC_SCHED_SAP_H */
