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
 * Author: Marco Miozzo <marco.miozzo@cttc.es>
 *         Nicola Baldo <nbaldo@cttc.es>
 */

#ifndef RR_FF_MAC_SCHEDULER_H
#define RR_FF_MAC_SCHEDULER_H

#include <ns3/ff-mac-csched-sap.h>
#include <ns3/ff-mac-sched-sap.h>
#include <ns3/ff-mac-scheduler.h>
#include <vector>
#include <map>
#include <ns3/lte-common.h>
#include <ns3/lte-amc.h>


namespace ns3 {



/**
 * \ingroup ff-api
 * \defgroup FF-API RrFfMacScheduler
 */
/**
 * \ingroup RrFfMacScheduler
 * \brief Implements the SCHED SAP and CSCHED SAP for a Round Robin scheduler
 *
 * This class implements the interface defined by the FfMacScheduler abstract class
 */

class RrFfMacScheduler : public FfMacScheduler
{
public:
  /**
   * \brief Constructor
   *
   * Creates the MAC Scheduler interface implementation
   */
  RrFfMacScheduler ();

  /**
   * Destructor
   */
  virtual ~RrFfMacScheduler ();

  // inherited from Object
  virtual void DoDispose (void);
  static TypeId GetTypeId (void);

  // inherited from FfMacScheduler
  virtual void SetFfMacCschedSapUser (FfMacCschedSapUser* s);
  virtual void SetFfMacSchedSapUser (FfMacSchedSapUser* s);
  virtual FfMacCschedSapProvider* GetFfMacCschedSapProvider ();
  virtual FfMacSchedSapProvider* GetFfMacSchedSapProvider ();

  friend class RrSchedulerMemberCschedSapProvider;
  friend class RrSchedulerMemberSchedSapProvider;
  
  void TransmissionModeConfigurationUpdate (uint16_t rnti, uint8_t txMode);

private:
  //
  // Implementation of the CSCHED API primitives
  // (See 4.1 for description of the primitives)
  //

  void DoCschedCellConfigReq (const struct FfMacCschedSapProvider::CschedCellConfigReqParameters& params);

  void DoCschedUeConfigReq (const struct FfMacCschedSapProvider::CschedUeConfigReqParameters& params);

  void DoCschedLcConfigReq (const struct FfMacCschedSapProvider::CschedLcConfigReqParameters& params);

  void DoCschedLcReleaseReq (const struct FfMacCschedSapProvider::CschedLcReleaseReqParameters& params);

  void DoCschedUeReleaseReq (const struct FfMacCschedSapProvider::CschedUeReleaseReqParameters& params);

  //
  // Implementation of the SCHED API primitives
  // (See 4.2 for description of the primitives)
  //

  void DoSchedDlRlcBufferReq (const struct FfMacSchedSapProvider::SchedDlRlcBufferReqParameters& params);

  void DoSchedDlPagingBufferReq (const struct FfMacSchedSapProvider::SchedDlPagingBufferReqParameters& params);

  void DoSchedDlMacBufferReq (const struct FfMacSchedSapProvider::SchedDlMacBufferReqParameters& params);

  void DoSchedDlTriggerReq (const struct FfMacSchedSapProvider::SchedDlTriggerReqParameters& params);

  void DoSchedDlRachInfoReq (const struct FfMacSchedSapProvider::SchedDlRachInfoReqParameters& params);

  void DoSchedDlCqiInfoReq (const struct FfMacSchedSapProvider::SchedDlCqiInfoReqParameters& params);

  void DoSchedUlTriggerReq (const struct FfMacSchedSapProvider::SchedUlTriggerReqParameters& params);

  void DoSchedUlNoiseInterferenceReq (const struct FfMacSchedSapProvider::SchedUlNoiseInterferenceReqParameters& params);

  void DoSchedUlSrInfoReq (const struct FfMacSchedSapProvider::SchedUlSrInfoReqParameters& params);

  void DoSchedUlMacCtrlInfoReq (const struct FfMacSchedSapProvider::SchedUlMacCtrlInfoReqParameters& params);

  void DoSchedUlCqiInfoReq (const struct FfMacSchedSapProvider::SchedUlCqiInfoReqParameters& params);


  int GetRbgSize (int dlbandwidth);
  
  static bool SortRlcBufferReq (FfMacSchedSapProvider::SchedDlRlcBufferReqParameters i,FfMacSchedSapProvider::SchedDlRlcBufferReqParameters j);
  
  void RefreshDlCqiMaps(void);
  void RefreshUlCqiMaps(void);
  
  void UpdateDlRlcBufferInfo (uint16_t rnti, uint8_t lcid, uint16_t size);
  void UpdateUlRlcBufferInfo (uint16_t rnti, uint16_t size);
  
  Ptr<LteAmc> m_amc;

  /*
   * Vectors of UE's RLC info
  */
  std::list <FfMacSchedSapProvider::SchedDlRlcBufferReqParameters> m_rlcBufferReq;

  /*
  * Map of UE's DL CQI P01 received
  */
  std::map <uint16_t,uint8_t> m_p10CqiRxed;
  /*
  * Map of UE's timers on DL CQI P01 received
  */
  std::map <uint16_t,uint32_t> m_p10CqiTimers;

  /*
  * Map of previous allocated UE per RBG
  * (used to retrieve info from UL-CQI)
  */
  std::map <uint16_t, std::vector <uint16_t> > m_allocationMaps;

  /*
  * Map of UEs' UL-CQI per RBG
  */
  std::map <uint16_t, std::vector <double> > m_ueCqi;
  /*
  * Map of UEs' timers on UL-CQI per RBG
  */
  std::map <uint16_t, uint32_t> m_ueCqiTimers;



  /*
  * Map of UE's buffer status reports received
  */
  std::map <uint16_t,uint32_t> m_ceBsrRxed;

  // MAC SAPs
  FfMacCschedSapUser* m_cschedSapUser;
  FfMacSchedSapUser* m_schedSapUser;
  FfMacCschedSapProvider* m_cschedSapProvider;
  FfMacSchedSapProvider* m_schedSapProvider;


  // Internal parameters
  FfMacCschedSapProvider::CschedCellConfigReqParameters m_cschedCellConfig;

  uint16_t m_nextRntiDl; // RNTI of the next user to be served next scheduling in DL
  uint16_t m_nextRntiUl; // RNTI of the next user to be served next scheduling in UL
  
  uint32_t m_cqiTimersThreshold; // # of TTIs for which a CQI canbe considered valid
  
  std::map <uint16_t,uint8_t> m_uesTxMode; // txMode of the UEs

};

} // namespace ns3

#endif /* RR_FF_MAC_SCHEDULER_H */
