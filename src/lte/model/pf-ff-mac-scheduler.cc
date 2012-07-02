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
 */

#ifdef __FreeBSD__
#define log2(x) (log(x) / M_LN2)
#endif /* __FreeBSD__ */

#include <ns3/log.h>
#include <ns3/pointer.h>

#include <ns3/simulator.h>
#include <ns3/lte-amc.h>
#include <ns3/pf-ff-mac-scheduler.h>

NS_LOG_COMPONENT_DEFINE ("PfFfMacScheduler");

namespace ns3 {

int PfType0AllocationRbg[4] = {
  10,       // RGB size 1
  26,       // RGB size 2
  63,       // RGB size 3
  110       // RGB size 4
};  // see table 7.1.6.1-1 of 36.213


NS_OBJECT_ENSURE_REGISTERED (PfFfMacScheduler);



class PfSchedulerMemberCschedSapProvider : public FfMacCschedSapProvider
{
public:
  PfSchedulerMemberCschedSapProvider (PfFfMacScheduler* scheduler);

  // inherited from FfMacCschedSapProvider
  virtual void CschedCellConfigReq (const struct CschedCellConfigReqParameters& params);
  virtual void CschedUeConfigReq (const struct CschedUeConfigReqParameters& params);
  virtual void CschedLcConfigReq (const struct CschedLcConfigReqParameters& params);
  virtual void CschedLcReleaseReq (const struct CschedLcReleaseReqParameters& params);
  virtual void CschedUeReleaseReq (const struct CschedUeReleaseReqParameters& params);

private:
  PfSchedulerMemberCschedSapProvider ();
  PfFfMacScheduler* m_scheduler;
};

PfSchedulerMemberCschedSapProvider::PfSchedulerMemberCschedSapProvider ()
{
}

PfSchedulerMemberCschedSapProvider::PfSchedulerMemberCschedSapProvider (PfFfMacScheduler* scheduler) : m_scheduler (scheduler)
{
}


void
PfSchedulerMemberCschedSapProvider::CschedCellConfigReq (const struct CschedCellConfigReqParameters& params)
{
  m_scheduler->DoCschedCellConfigReq (params);
}

void
PfSchedulerMemberCschedSapProvider::CschedUeConfigReq (const struct CschedUeConfigReqParameters& params)
{
  m_scheduler->DoCschedUeConfigReq (params);
}


void
PfSchedulerMemberCschedSapProvider::CschedLcConfigReq (const struct CschedLcConfigReqParameters& params)
{
  m_scheduler->DoCschedLcConfigReq (params);
}

void
PfSchedulerMemberCschedSapProvider::CschedLcReleaseReq (const struct CschedLcReleaseReqParameters& params)
{
  m_scheduler->DoCschedLcReleaseReq (params);
}

void
PfSchedulerMemberCschedSapProvider::CschedUeReleaseReq (const struct CschedUeReleaseReqParameters& params)
{
  m_scheduler->DoCschedUeReleaseReq (params);
}




class PfSchedulerMemberSchedSapProvider : public FfMacSchedSapProvider
{
public:
  PfSchedulerMemberSchedSapProvider (PfFfMacScheduler* scheduler);

  // inherited from FfMacSchedSapProvider
  virtual void SchedDlRlcBufferReq (const struct SchedDlRlcBufferReqParameters& params);
  virtual void SchedDlPagingBufferReq (const struct SchedDlPagingBufferReqParameters& params);
  virtual void SchedDlMacBufferReq (const struct SchedDlMacBufferReqParameters& params);
  virtual void SchedDlTriggerReq (const struct SchedDlTriggerReqParameters& params);
  virtual void SchedDlRachInfoReq (const struct SchedDlRachInfoReqParameters& params);
  virtual void SchedDlCqiInfoReq (const struct SchedDlCqiInfoReqParameters& params);
  virtual void SchedUlTriggerReq (const struct SchedUlTriggerReqParameters& params);
  virtual void SchedUlNoiseInterferenceReq (const struct SchedUlNoiseInterferenceReqParameters& params);
  virtual void SchedUlSrInfoReq (const struct SchedUlSrInfoReqParameters& params);
  virtual void SchedUlMacCtrlInfoReq (const struct SchedUlMacCtrlInfoReqParameters& params);
  virtual void SchedUlCqiInfoReq (const struct SchedUlCqiInfoReqParameters& params);


private:
  PfSchedulerMemberSchedSapProvider ();
  PfFfMacScheduler* m_scheduler;
};



PfSchedulerMemberSchedSapProvider::PfSchedulerMemberSchedSapProvider ()
{
}


PfSchedulerMemberSchedSapProvider::PfSchedulerMemberSchedSapProvider (PfFfMacScheduler* scheduler)
  : m_scheduler (scheduler)
{
}

void
PfSchedulerMemberSchedSapProvider::SchedDlRlcBufferReq (const struct SchedDlRlcBufferReqParameters& params)
{
  m_scheduler->DoSchedDlRlcBufferReq (params);
}

void
PfSchedulerMemberSchedSapProvider::SchedDlPagingBufferReq (const struct SchedDlPagingBufferReqParameters& params)
{
  m_scheduler->DoSchedDlPagingBufferReq (params);
}

void
PfSchedulerMemberSchedSapProvider::SchedDlMacBufferReq (const struct SchedDlMacBufferReqParameters& params)
{
  m_scheduler->DoSchedDlMacBufferReq (params);
}

void
PfSchedulerMemberSchedSapProvider::SchedDlTriggerReq (const struct SchedDlTriggerReqParameters& params)
{
  m_scheduler->DoSchedDlTriggerReq (params);
}

void
PfSchedulerMemberSchedSapProvider::SchedDlRachInfoReq (const struct SchedDlRachInfoReqParameters& params)
{
  m_scheduler->DoSchedDlRachInfoReq (params);
}

void
PfSchedulerMemberSchedSapProvider::SchedDlCqiInfoReq (const struct SchedDlCqiInfoReqParameters& params)
{
  m_scheduler->DoSchedDlCqiInfoReq (params);
}

void
PfSchedulerMemberSchedSapProvider::SchedUlTriggerReq (const struct SchedUlTriggerReqParameters& params)
{
  m_scheduler->DoSchedUlTriggerReq (params);
}

void
PfSchedulerMemberSchedSapProvider::SchedUlNoiseInterferenceReq (const struct SchedUlNoiseInterferenceReqParameters& params)
{
  m_scheduler->DoSchedUlNoiseInterferenceReq (params);
}

void
PfSchedulerMemberSchedSapProvider::SchedUlSrInfoReq (const struct SchedUlSrInfoReqParameters& params)
{
  m_scheduler->DoSchedUlSrInfoReq (params);
}

void
PfSchedulerMemberSchedSapProvider::SchedUlMacCtrlInfoReq (const struct SchedUlMacCtrlInfoReqParameters& params)
{
  m_scheduler->DoSchedUlMacCtrlInfoReq (params);
}

void
PfSchedulerMemberSchedSapProvider::SchedUlCqiInfoReq (const struct SchedUlCqiInfoReqParameters& params)
{
  m_scheduler->DoSchedUlCqiInfoReq (params);
}





PfFfMacScheduler::PfFfMacScheduler ()
  :   m_cschedSapUser (0),
    m_schedSapUser (0),
    m_timeWindow (99.0),
    m_nextRntiUl (0)
{
  m_amc = CreateObject <LteAmc> ();
  m_cschedSapProvider = new PfSchedulerMemberCschedSapProvider (this);
  m_schedSapProvider = new PfSchedulerMemberSchedSapProvider (this);
}

PfFfMacScheduler::~PfFfMacScheduler ()
{
  NS_LOG_FUNCTION (this);
}

void
PfFfMacScheduler::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_cschedSapProvider;
  delete m_schedSapProvider;
}

TypeId
PfFfMacScheduler::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PfFfMacScheduler")
    .SetParent<FfMacScheduler> ()
    .AddConstructor<PfFfMacScheduler> ()
    .AddAttribute ("CqiTimerThreshold",
                   "The number of TTIs a CQI is valid (default 1000 - 1 sec.)",
                   UintegerValue (1000),
                   MakeUintegerAccessor (&PfFfMacScheduler::m_cqiTimersThreshold),
                   MakeUintegerChecker<uint32_t> ())
    ;
  return tid;
}



void
PfFfMacScheduler::SetFfMacCschedSapUser (FfMacCschedSapUser* s)
{
  m_cschedSapUser = s;
}

void
PfFfMacScheduler::SetFfMacSchedSapUser (FfMacSchedSapUser* s)
{
  m_schedSapUser = s;
}

FfMacCschedSapProvider*
PfFfMacScheduler::GetFfMacCschedSapProvider ()
{
  return m_cschedSapProvider;
}

FfMacSchedSapProvider*
PfFfMacScheduler::GetFfMacSchedSapProvider ()
{
  return m_schedSapProvider;
}

void
PfFfMacScheduler::DoCschedCellConfigReq (const struct FfMacCschedSapProvider::CschedCellConfigReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // Read the subset of parameters used
  m_cschedCellConfig = params;
  FfMacCschedSapUser::CschedUeConfigCnfParameters cnf;
  cnf.m_result = SUCCESS;
  m_cschedSapUser->CschedUeConfigCnf (cnf);
  return;
}

void
PfFfMacScheduler::DoCschedUeConfigReq (const struct FfMacCschedSapProvider::CschedUeConfigReqParameters& params)
{
  NS_LOG_FUNCTION (this << " RNTI " << params.m_rnti << " txMode " << (uint16_t)params.m_transmissionMode);
  std::map <uint16_t,uint8_t>::iterator it = m_uesTxMode.find (params.m_rnti);
  if (it==m_uesTxMode.end ())
  {
    m_uesTxMode.insert (std::pair <uint16_t, double> (params.m_rnti, params.m_transmissionMode));
  }
  else
  {
    (*it).second = params.m_transmissionMode;
  }
  return;
  return;
}

void
PfFfMacScheduler::DoCschedLcConfigReq (const struct FfMacCschedSapProvider::CschedLcConfigReqParameters& params)
{
  NS_LOG_FUNCTION (this << " New LC, rnti: "  << params.m_rnti);

  std::map <uint16_t, pfsFlowPerf_t>::iterator it;
  for (uint16_t i = 0; i < params.m_logicalChannelConfigList.size (); i++)
    {
      it = m_flowStatsDl.find (params.m_rnti);

      if (it == m_flowStatsDl.end ())
        {
          pfsFlowPerf_t flowStatsDl;
          flowStatsDl.flowStart = Simulator::Now ();
          flowStatsDl.totalBytesTransmitted = 0;
          flowStatsDl.lastTtiBytesTrasmitted = 0;
          flowStatsDl.lastAveragedThroughput = 1;
          m_flowStatsDl.insert (std::pair<uint16_t, pfsFlowPerf_t> (params.m_rnti, flowStatsDl));
          pfsFlowPerf_t flowStatsUl;
          flowStatsUl.flowStart = Simulator::Now ();
          flowStatsUl.totalBytesTransmitted = 0;
          flowStatsUl.lastTtiBytesTrasmitted = 0;
          flowStatsUl.lastAveragedThroughput = 1;
          m_flowStatsUl.insert (std::pair<uint16_t, pfsFlowPerf_t> (params.m_rnti, flowStatsUl));
        }
      else
        {
          NS_LOG_ERROR ("RNTI already exists");
        }
    }

  return;
}

void
PfFfMacScheduler::DoCschedLcReleaseReq (const struct FfMacCschedSapProvider::CschedLcReleaseReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // TODO: Implementation of the API
  return;
}

void
PfFfMacScheduler::DoCschedUeReleaseReq (const struct FfMacCschedSapProvider::CschedUeReleaseReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // TODO: Implementation of the API
  return;
}


void
PfFfMacScheduler::DoSchedDlRlcBufferReq (const struct FfMacSchedSapProvider::SchedDlRlcBufferReqParameters& params)
{
  NS_LOG_FUNCTION (this << params.m_rnti << (uint32_t) params.m_logicalChannelIdentity);
  // API generated by RLC for updating RLC parameters on a LC (tx and retx queues)

  std::map <LteFlowId_t, FfMacSchedSapProvider::SchedDlRlcBufferReqParameters>::iterator it;

  LteFlowId_t flow (params.m_rnti, params.m_logicalChannelIdentity);

  it =  m_rlcBufferReq.find (flow);

  if (it == m_rlcBufferReq.end ())
    {
      m_rlcBufferReq.insert (std::pair <LteFlowId_t, FfMacSchedSapProvider::SchedDlRlcBufferReqParameters> (flow, params));
    }
  else
    {
      (*it).second = params;
    }

  return;
}

void
PfFfMacScheduler::DoSchedDlPagingBufferReq (const struct FfMacSchedSapProvider::SchedDlPagingBufferReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // TODO: Implementation of the API
  return;
}

void
PfFfMacScheduler::DoSchedDlMacBufferReq (const struct FfMacSchedSapProvider::SchedDlMacBufferReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // TODO: Implementation of the API
  return;
}

int
PfFfMacScheduler::GetRbgSize (int dlbandwidth)
{
  for (int i = 0; i < 4; i++)
    {
      if (dlbandwidth < PfType0AllocationRbg[i])
        {
          return (i + 1);
        }
    }

  return (-1);
}


int 
PfFfMacScheduler::LcActivePerFlow (uint16_t rnti)
{
  std::map <LteFlowId_t, FfMacSchedSapProvider::SchedDlRlcBufferReqParameters>::iterator it;
  int lcActive = 0;
  for (it = m_rlcBufferReq.begin (); it != m_rlcBufferReq.end (); it++)
    {
      if (((*it).first.m_rnti == rnti) && (((*it).second.m_rlcTransmissionQueueSize > 0)
                                           || ((*it).second.m_rlcRetransmissionQueueSize > 0)
                                           || ((*it).second.m_rlcStatusPduSize > 0) ))
        {
          lcActive++;
        }
      if ((*it).first.m_rnti > rnti)
        {
          break;
        }
    }
  return (lcActive);

}


void
PfFfMacScheduler::DoSchedDlTriggerReq (const struct FfMacSchedSapProvider::SchedDlTriggerReqParameters& params)
{
  NS_LOG_FUNCTION (this << " Frame no. " << (params.m_sfnSf >> 4) << " subframe no. " << (0xF & params.m_sfnSf));
  // API generated by RLC for triggering the scheduling of a DL subframe


  // evaluate the relative channel quality indicator for each UE per each RBG 
  // (since we are using allocation type 0 the small unit of allocation is RBG)
  // Resource allocation type 0 (see sec 7.1.6.1 of 36.213)
  
  RefreshDlCqiMaps ();
  
  int rbgSize = GetRbgSize (m_cschedCellConfig.m_dlBandwidth);
  int rbgNum = m_cschedCellConfig.m_dlBandwidth / rbgSize;
  std::map <uint16_t, std::vector <uint16_t> > allocationMap;
  for (int i = 0; i < rbgNum; i++)
    {
//       NS_LOG_DEBUG (this << " ALLOCATION for RBG " << i << " of " << rbgNum);
      std::map <uint16_t, pfsFlowPerf_t>::iterator it;
      std::map <uint16_t, pfsFlowPerf_t>::iterator itMax = m_flowStatsDl.end ();
      double rcqiMax = 0.0;
      for (it = m_flowStatsDl.begin (); it != m_flowStatsDl.end (); it++)
        {
          std::map <uint16_t,SbMeasResult_s>::iterator itCqi;
          itCqi = m_a30CqiRxed.find ((*it).first);
          std::map <uint16_t,uint8_t>::iterator itTxMode;
          itTxMode = m_uesTxMode.find ((*it).first);
          if (itTxMode == m_uesTxMode.end())
            {
              NS_FATAL_ERROR ("No Transmission Mode info on user " << (*it).first);
            }
          int nLayer = TransmissionModesLayers::TxMode2LayerNum ((*itTxMode).second);
          std::vector <uint8_t> sbCqi;
          if (itCqi == m_a30CqiRxed.end ())
            {
//               NS_LOG_DEBUG (this << " No DL-CQI for this UE " << (*it).first);
              for (uint8_t k = 0; k < nLayer; k++)
                {
                  sbCqi.push_back (1);  // start with lowest value
                }
            }
          else
            {
              sbCqi = (*itCqi).second.m_higherLayerSelected.at (i).m_sbCqi;
//               NS_LOG_INFO (this << " CQI " << (uint32_t)cqi);
            }
          uint8_t cqi1 = sbCqi.at(0);
          uint8_t cqi2 = 1;
          if (sbCqi.size () > 1)
            {
              cqi2 = sbCqi.at(1);
            }
          
          if ((cqi1 > 0)||(cqi2 > 0)) // CQI == 0 means "out of range" (see table 7.2.3-1 of 36.213)
            {
//               NS_LOG_DEBUG (this << " LC active " << LcActivePerFlow ((*it).first));
              if (LcActivePerFlow ((*it).first) > 0)
                {
                  // this UE has data to transmit
                  double achievableRate = 0.0;
                  for (uint8_t k = 0; k < nLayer; k++) 
                    {
                      uint8_t mcs = 0; 
                      if (sbCqi.size () > k)
                        {                       
                          mcs = m_amc->GetMcsFromCqi (sbCqi.at (k));
                        }
                      else
                        {
                          // no info on this subband -> worst MCS
                          mcs = 0;
                        }
                        achievableRate += ((m_amc->GetTbSizeFromMcs (mcs, rbgSize) / 8) / 0.001); // = TB size / TTI
                    }
                  
                  double rcqi = achievableRate / (*it).second.lastAveragedThroughput;
//                   NS_LOG_DEBUG (this << " RNTI " << (*it).first << " MCS " << (uint32_t)mcs << " achievableRate " << achievableRate << " avgThr " << (*it).second.lastAveragedThroughput << " RCQI " << rcqi);

                  if (rcqi > rcqiMax)
                    {
                      rcqiMax = rcqi;
                      itMax = it;
                    }
                }
            }   // end if cqi
        } // end for m_rlcBufferReq

      if (itMax == m_flowStatsDl.end ())
        {
          // no UE available for this RB
          NS_LOG_DEBUG (this << " no UE found");
        }
      else
        {
          std::map <uint16_t, std::vector <uint16_t> >::iterator itMap;
          itMap = allocationMap.find ((*itMax).first);
          if (itMap == allocationMap.end ())
            {
              // insert new element
              std::vector <uint16_t> tempMap;
              tempMap.push_back (i);
              allocationMap.insert (std::pair <uint16_t, std::vector <uint16_t> > ((*itMax).first, tempMap));
            }
          else
            {
              (*itMap).second.push_back (i);
            }
//           NS_LOG_DEBUG (this << " UE assigned " << (*itMax).first);
        }
    } // end for RBGs

  // reset TTI stats of users
  std::map <uint16_t, pfsFlowPerf_t>::iterator itStats;
  for (itStats = m_flowStatsDl.begin (); itStats != m_flowStatsDl.end (); itStats++)
    {
      (*itStats).second.lastTtiBytesTrasmitted = 0;
    }

  // generate the transmission opportunities by grouping the RBGs of the same RNTI and
  // creating the correspondent DCIs
  FfMacSchedSapUser::SchedDlConfigIndParameters ret;
  std::map <uint16_t, std::vector <uint16_t> >::iterator itMap = allocationMap.begin ();
  while (itMap != allocationMap.end ())
    {
      // create new BuildDataListElement_s for this LC
      BuildDataListElement_s newEl;
      newEl.m_rnti = (*itMap).first;
      // create the DlDciListElement_s
      DlDciListElement_s newDci;
      std::vector <struct RlcPduListElement_s> newRlcPduLe;
      newDci.m_rnti = (*itMap).first;

      uint16_t lcActives = LcActivePerFlow ((*itMap).first);
//       NS_LOG_DEBUG (this << "Allocate user " << newEl.m_rnti << " rbg " << lcActives);
      uint16_t RgbPerRnti = (*itMap).second.size ();
      std::map <uint16_t,SbMeasResult_s>::iterator itCqi;
      itCqi = m_a30CqiRxed.find ((*itMap).first);
      std::map <uint16_t,uint8_t>::iterator itTxMode;
      itTxMode = m_uesTxMode.find ((*itMap).first);
      if (itTxMode == m_uesTxMode.end())
        {
          NS_FATAL_ERROR ("No Transmission Mode info on user " << (*itMap).first);
        }
      int nLayer = TransmissionModesLayers::TxMode2LayerNum ((*itTxMode).second);
      std::vector <uint8_t> worstCqi (2, 15);
      if (itCqi != m_a30CqiRxed.end ())
        {
          for (uint16_t k = 0; k < (*itMap).second.size (); k++)
            {
              if ((*itCqi).second.m_higherLayerSelected.size () > (*itMap).second.at (k))
                {
//                 NS_LOG_DEBUG (this << " RBG " << (*itMap).second.at (k) << " CQI " << (uint16_t)((*itCqi).second.m_higherLayerSelected.at ((*itMap).second.at (k)).m_sbCqi.at (0)) );
                  for (uint8_t j = 0; j < nLayer; j++) 
                    {
                      if ((*itCqi).second.m_higherLayerSelected.at ((*itMap).second.at (k)).m_sbCqi.size ()> j)
                        {
                          if (((*itCqi).second.m_higherLayerSelected.at ((*itMap).second.at (k)).m_sbCqi.at (j)) < worstCqi.at (j))
                            {
                              worstCqi.at (j) = ((*itCqi).second.m_higherLayerSelected.at ((*itMap).second.at (k)).m_sbCqi.at (j));
                            }
                        }
                      else
                        {
                          // no CQI for this layer of this suband -> worst one
                          worstCqi.at (j) = 1;
                        }
                    }
                }
              else
                {
                  for (uint8_t j = 0; j < nLayer; j++)
                    {
                      worstCqi.at (j) = 1; // try with lowest MCS in RBG with no info on channel
                    }
                }
            }
        }
      else
        {
          for (uint8_t j = 0; j < nLayer; j++)
            {
              worstCqi.at (j) = 1; // try with lowest MCS in RBG with no info on channel
            }
        }
//       NS_LOG_DEBUG (this << " CQI " << (uint16_t)worstCqi);
      uint32_t bytesTxed = 0;
      for (uint8_t j = 0; j < nLayer; j++)
        {
          newDci.m_mcs.push_back (m_amc->GetMcsFromCqi (worstCqi.at (j)));
          int tbSize = (m_amc->GetTbSizeFromMcs (newDci.m_mcs.at (j), RgbPerRnti * rbgSize) / 8); // (size of TB in bytes according to table 7.1.7.2.1-1 of 36.213)
          newDci.m_tbsSize.push_back (tbSize);
          NS_LOG_DEBUG (this << " MCS " << m_amc->GetMcsFromCqi (worstCqi.at (j)));
          bytesTxed += tbSize;
        }

      newDci.m_resAlloc = 0;  // only allocation type 0 at this stage
      newDci.m_rbBitmap = 0; // TBD (32 bit bitmap see 7.1.6 of 36.213)
      uint32_t rbgMask = 0;
      for (uint16_t k = 0; k < (*itMap).second.size (); k++)
        {
          rbgMask = rbgMask + (0x1 << (*itMap).second.at (k));
//           NS_LOG_DEBUG (this << " Allocated PRB " << (*itMap).second.at (k));
        }
      newDci.m_rbBitmap = rbgMask; // (32 bit bitmap see 7.1.6 of 36.213)

      // create the rlc PDUs -> equally divide resources among actives LCs
      std::map <LteFlowId_t, FfMacSchedSapProvider::SchedDlRlcBufferReqParameters>::iterator itBufReq;
      for (itBufReq = m_rlcBufferReq.begin (); itBufReq != m_rlcBufferReq.end (); itBufReq++)
        {
          if (((*itBufReq).first.m_rnti == (*itMap).first) &&
            (((*itBufReq).second.m_rlcTransmissionQueueSize > 0)
              || ((*itBufReq).second.m_rlcRetransmissionQueueSize > 0)
              || ((*itBufReq).second.m_rlcStatusPduSize > 0) ))
            {
              for (uint8_t j = 0; j < nLayer; j++)
                {
                  RlcPduListElement_s newRlcEl;
                  newRlcEl.m_logicalChannelIdentity = (*itBufReq).first.m_lcId;
                  newRlcEl.m_size = newDci.m_tbsSize.at (j) / lcActives;
                  NS_LOG_DEBUG (this << " LCID " << (uint32_t) newRlcEl.m_logicalChannelIdentity << " size " << newRlcEl.m_size << " layer " << (uint16_t)j);
                  newRlcPduLe.push_back (newRlcEl);
                  UpdateDlRlcBufferInfo (newDci.m_rnti, newRlcEl.m_logicalChannelIdentity, newRlcEl.m_size);
                }
            }
          if ((*itBufReq).first.m_rnti > (*itMap).first)
            {
              break;
            }
        }
      newDci.m_ndi.push_back (1); // TBD (new data indicator)
      newDci.m_rv.push_back (0); // TBD (redundancy version)

      newEl.m_dci = newDci;
      // ...more parameters -> ingored in this version

      newEl.m_rlcPduList.push_back (newRlcPduLe);
      ret.m_buildDataList.push_back (newEl);

      // update UE stats
      std::map <uint16_t, pfsFlowPerf_t>::iterator it;
      it = m_flowStatsDl.find ((*itMap).first);
      if (it != m_flowStatsDl.end ())
        {
          (*it).second.lastTtiBytesTrasmitted = bytesTxed;
//           NS_LOG_DEBUG (this << " UE bytes txed " << (*it).second.lastTtiBytesTrasmitted);


        }
      else
        {
          NS_LOG_DEBUG (this << " No Stats for this allocated UE");
        }

      itMap++;
    } // end while allocation
  ret.m_nrOfPdcchOfdmSymbols = 1;   // TODO: check correct value according the DCIs txed


  // update UEs stats
  for (itStats = m_flowStatsDl.begin (); itStats != m_flowStatsDl.end (); itStats++)
    {
      (*itStats).second.totalBytesTransmitted += (*itStats).second.lastTtiBytesTrasmitted;
      // update average throughput (see eq. 12.3 of Sec 12.3.1.2 of LTE – The UMTS Long Term Evolution, Ed Wiley)
      (*itStats).second.lastAveragedThroughput = ((1.0 - (1.0 / m_timeWindow)) * (*itStats).second.lastAveragedThroughput) + ((1.0 / m_timeWindow) * (double)((*itStats).second.lastTtiBytesTrasmitted / 0.001));
//       NS_LOG_DEBUG (this << " UE tot bytes " << (*itStats).second.totalBytesTransmitted);
//       NS_LOG_DEBUG (this << " UE avg thr " << (*itStats).second.lastAveragedThroughput);
      (*itStats).second.lastTtiBytesTrasmitted = 0;
    }

  m_schedSapUser->SchedDlConfigInd (ret);

 
  return;
}

void
PfFfMacScheduler::DoSchedDlRachInfoReq (const struct FfMacSchedSapProvider::SchedDlRachInfoReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // TODO: Implementation of the API
  return;
}

void
PfFfMacScheduler::DoSchedDlCqiInfoReq (const struct FfMacSchedSapProvider::SchedDlCqiInfoReqParameters& params)
{
  NS_LOG_FUNCTION (this);

  for (unsigned int i = 0; i < params.m_cqiList.size (); i++)
    {
      if ( params.m_cqiList.at (i).m_cqiType == CqiListElement_s::P10 )
        {
          // wideband CQI reporting
          std::map <uint16_t,uint8_t>::iterator it;
          uint16_t rnti = params.m_cqiList.at (i).m_rnti;
          it = m_p10CqiRxed.find (rnti);
          if (it == m_p10CqiRxed.end ())
            {
              // create the new entry
              m_p10CqiRxed.insert ( std::pair<uint16_t, uint8_t > (rnti, params.m_cqiList.at (i).m_wbCqi.at (0)) ); // only codeword 0 at this stage (SISO)
              // generate correspondent timer
              m_p10CqiTimers.insert ( std::pair<uint16_t, uint32_t > (rnti, m_cqiTimersThreshold));
            }
          else
            {
              // update the CQI value and refresh correspondent timer
              (*it).second = params.m_cqiList.at (i).m_wbCqi.at (0);
              // update correspondent timer
              std::map <uint16_t,uint32_t>::iterator itTimers;
              itTimers = m_p10CqiTimers.find (rnti);
              (*itTimers).second = m_cqiTimersThreshold;
            }
        }
      else if ( params.m_cqiList.at (i).m_cqiType == CqiListElement_s::A30 )
        {
          // subband CQI reporting high layer configured
          std::map <uint16_t,SbMeasResult_s>::iterator it;
          uint16_t rnti = params.m_cqiList.at (i).m_rnti;
          it = m_a30CqiRxed.find (rnti);
          if (it == m_a30CqiRxed.end ())
            {
              // create the new entry
              m_a30CqiRxed.insert ( std::pair<uint16_t, SbMeasResult_s > (rnti, params.m_cqiList.at (i).m_sbMeasResult) );
              m_a30CqiTimers.insert ( std::pair<uint16_t, uint32_t > (rnti, m_cqiTimersThreshold));
            }
          else
            {
              // update the CQI value and refresh correspondent timer
              (*it).second = params.m_cqiList.at (i).m_sbMeasResult;
              std::map <uint16_t,uint32_t>::iterator itTimers;
              itTimers = m_a30CqiTimers.find (rnti);
              (*itTimers).second = m_cqiTimersThreshold;
            }
        }
      else
        {
          NS_LOG_ERROR (this << " CQI type unknown");
        }
    }

  return;
}


double
PfFfMacScheduler::EstimateUlSinr (uint16_t rnti, uint16_t rb)
{
  std::map <uint16_t, std::vector <double> >::iterator itCqi = m_ueCqi.find (rnti);
  if (itCqi == m_ueCqi.end ())
    {
      // no cqi info about this UE
      return (NO_SINR);

    }
  else
    {
      // take the average SINR value among the available
      double sinrSum = 0;
      int sinrNum = 0;
      for (uint32_t i = 0; i < m_cschedCellConfig.m_ulBandwidth; i++)
        {
          double sinr = (*itCqi).second.at (i);
          if (sinr != NO_SINR)
            {
              sinrSum += sinr;
              sinrNum++;
            }
        }
      double estimatedSinr = sinrSum / (double)sinrNum;
      // store the value
      (*itCqi).second.at (rb) = estimatedSinr;
      return (estimatedSinr);
    }
}

void
PfFfMacScheduler::DoSchedUlTriggerReq (const struct FfMacSchedSapProvider::SchedUlTriggerReqParameters& params)
{
  NS_LOG_FUNCTION (this << " UL - Frame no. " << (params.m_sfnSf >> 4) << " subframe no. " << (0xF & params.m_sfnSf));
 
  RefreshUlCqiMaps ();

  std::map <uint16_t,uint32_t>::iterator it; 
  int nflows = 0;

  for (it = m_ceBsrRxed.begin (); it != m_ceBsrRxed.end (); it++)
    {
      // remove old entries of this UE-LC
      if ((*it).second > 0)
        {
          nflows++;
        }
    }

  if (nflows == 0)
    {
      return ; // no flows to be scheduled
    }


  // Divide the resource equally among the active users
  int rbPerFlow = m_cschedCellConfig.m_ulBandwidth / nflows;
  if (rbPerFlow == 0)
    {
      rbPerFlow = 1;              // at least 1 rbg per flow (till available resource)
    }
  int rbAllocated = 0;

  FfMacSchedSapUser::SchedUlConfigIndParameters ret;
  std::vector <uint16_t> rbgAllocationMap;
  std::map <uint16_t, pfsFlowPerf_t>::iterator itStats;
  if (m_nextRntiUl != 0)
    {
      for (it = m_ceBsrRxed.begin (); it != m_ceBsrRxed.end (); it++)
        {
          if ((*it).first == m_nextRntiUl)
            {
              break;
            }
        }
      if (it == m_ceBsrRxed.end ())
        {
          NS_LOG_ERROR (this << " no user found");
        }
    }
  else
    {
      it = m_ceBsrRxed.begin ();
      m_nextRntiUl = (*it).first;
    }
  do
    {
      if (rbAllocated + rbPerFlow > m_cschedCellConfig.m_ulBandwidth)
        {
          // limit to physical resources last resource assignment
          rbPerFlow = m_cschedCellConfig.m_ulBandwidth - rbAllocated;
        }
     
      UlDciListElement_s uldci;
      uldci.m_rnti = (*it).first;
      uldci.m_rbStart = rbAllocated;
      uldci.m_rbLen = rbPerFlow;
      std::map <uint16_t, std::vector <double> >::iterator itCqi = m_ueCqi.find ((*it).first);
      int cqi = 0;
      if (itCqi == m_ueCqi.end ())
        {
          // no cqi info about this UE
          uldci.m_mcs = 0; // MCS 0 -> UL-AMC TBD
//           NS_LOG_DEBUG (this << " UE does not have ULCQI " << (*it).first );
        }
      else
        {
          // take the lowest CQI value (worst RB)
          double minSinr = (*itCqi).second.at (uldci.m_rbStart);
          if (minSinr == NO_SINR)
            {
              minSinr = EstimateUlSinr ((*it).first, uldci.m_rbStart);
            }
          for (uint16_t i = uldci.m_rbStart; i < uldci.m_rbStart + uldci.m_rbLen; i++)
            {
//               NS_LOG_DEBUG (this << " UE " << (*it).first << " has SINR " << (*itCqi).second.at(i));
              double sinr = (*itCqi).second.at (i);
              if (sinr == NO_SINR)
                {
                  sinr = EstimateUlSinr ((*it).first, i);
                }
              if ((*itCqi).second.at (i) < minSinr)
                {
                  minSinr = (*itCqi).second.at (i);
                }
            }

          // translate SINR -> cqi: WILD ACK: same as DL
          double s = log2 ( 1 + (
                              pow (10, minSinr / 10 )  /
                              ( (-log (5.0 * 0.00005 )) / 1.5) ));
          cqi = m_amc->GetCqiFromSpectralEfficiency (s);
          if (cqi == 0)
            {
              it++;
              if (it == m_ceBsrRxed.end ())
                {
                  // restart from the first
                  it = m_ceBsrRxed.begin ();
                }
              continue; // CQI == 0 means "out of range" (see table 7.2.3-1 of 36.213)
            }
          uldci.m_mcs = m_amc->GetMcsFromCqi (cqi);
//           NS_LOG_DEBUG (this << " UE " <<  (*it).first << " minsinr " << minSinr << " -> mcs " << (uint16_t)uldci.m_mcs);

        }
      
      rbAllocated += rbPerFlow;
      // store info on allocation for managing ul-cqi interpretation
      for (int i = 0; i < rbPerFlow; i++)
        {
          rbgAllocationMap.push_back ((*it).first);
        }
      uldci.m_tbSize = (m_amc->GetTbSizeFromMcs (uldci.m_mcs, rbPerFlow) / 8);
//       NS_LOG_DEBUG (this << " UE " << (*it).first << " startPRB " << (uint32_t)uldci.m_rbStart << " nPRB " << (uint32_t)uldci.m_rbLen << " CQI " << cqi << " MCS " << (uint32_t)uldci.m_mcs << " TBsize " << uldci.m_tbSize << " RbAlloc " << rbAllocated);
      UpdateUlRlcBufferInfo (uldci.m_rnti, uldci.m_tbSize);
      uldci.m_ndi = 1;
      uldci.m_cceIndex = 0;
      uldci.m_aggrLevel = 1;
      uldci.m_ueTxAntennaSelection = 3; // antenna selection OFF
      uldci.m_hopping = false;
      uldci.m_n2Dmrs = 0;
      uldci.m_tpc = 0; // no power control
      uldci.m_cqiRequest = false; // only period CQI at this stage
      uldci.m_ulIndex = 0; // TDD parameter
      uldci.m_dai = 1; // TDD parameter
      uldci.m_freqHopping = 0;
      uldci.m_pdcchPowerOffset = 0; // not used
      ret.m_dciList.push_back (uldci);

      // update TTI  UE stats
      itStats = m_flowStatsUl.find ((*it).first);
      if (itStats != m_flowStatsUl.end ())
        {
          (*itStats).second.lastTtiBytesTrasmitted =  uldci.m_tbSize;
//                     NS_LOG_DEBUG (this << " UE bytes txed " << (*it).second.lastTtiBytesTrasmitted);


        }
      else
        {
          NS_LOG_DEBUG (this << " No Stats for this allocated UE");
        }


      it++;
      if (it == m_ceBsrRxed.end ())
        {
          // restart from the first
          it = m_ceBsrRxed.begin ();
        }
      if (rbAllocated == m_cschedCellConfig.m_ulBandwidth)
        {
          // Stop allocation: no more PRBs
          m_nextRntiUl = (*it).first;
          break;
        }
    }
  while ((*it).first != m_nextRntiUl);


  // Update global UE stats
  // update UEs stats
  for (itStats = m_flowStatsUl.begin (); itStats != m_flowStatsUl.end (); itStats++)
    {
      (*itStats).second.totalBytesTransmitted += (*itStats).second.lastTtiBytesTrasmitted;
      // update average throughput (see eq. 12.3 of Sec 12.3.1.2 of LTE – The UMTS Long Term Evolution, Ed Wiley)
      (*itStats).second.lastAveragedThroughput = ((1.0 - (1.0 / m_timeWindow)) * (*itStats).second.lastAveragedThroughput) + ((1.0 / m_timeWindow) * (double)((*itStats).second.lastTtiBytesTrasmitted / 0.001));
      //       NS_LOG_DEBUG (this << " UE tot bytes " << (*itStats).second.totalBytesTransmitted);
      //       NS_LOG_DEBUG (this << " UE avg thr " << (*itStats).second.lastAveragedThroughput);
      (*itStats).second.lastTtiBytesTrasmitted = 0;
    }
  m_allocationMaps.insert (std::pair <uint16_t, std::vector <uint16_t> > (params.m_sfnSf, rbgAllocationMap));
  m_schedSapUser->SchedUlConfigInd (ret);
  return;
}

void
PfFfMacScheduler::DoSchedUlNoiseInterferenceReq (const struct FfMacSchedSapProvider::SchedUlNoiseInterferenceReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // TODO: Implementation of the API
  return;
}

void
PfFfMacScheduler::DoSchedUlSrInfoReq (const struct FfMacSchedSapProvider::SchedUlSrInfoReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // TODO: Implementation of the API
  return;
}

void
PfFfMacScheduler::DoSchedUlMacCtrlInfoReq (const struct FfMacSchedSapProvider::SchedUlMacCtrlInfoReqParameters& params)
{
  NS_LOG_FUNCTION (this);

  std::map <uint16_t,uint32_t>::iterator it;
  
  for (unsigned int i = 0; i < params.m_macCeList.size (); i++)
  {
    if ( params.m_macCeList.at (i).m_macCeType == MacCeListElement_s::BSR )
    {
      // buffer status report
      // note that we only consider LCG 0, the other three LCGs are neglected
      // this is consistent with the assumption in LteUeMac that the first LCG gathers all LCs
      uint16_t rnti = params.m_macCeList.at (i).m_rnti;
      it = m_ceBsrRxed.find (rnti);
      if (it == m_ceBsrRxed.end ())
      {
        // create the new entry
        uint8_t bsrId = params.m_macCeList.at (i).m_macCeValue.m_bufferStatus.at (0);
        int buffer = BufferSizeLevelBsr::BsrId2BufferSize (bsrId);
        m_ceBsrRxed.insert ( std::pair<uint16_t, uint32_t > (rnti, buffer)); 
      }
      else
      {
        // update the buffer size value
        (*it).second = BufferSizeLevelBsr::BsrId2BufferSize (params.m_macCeList.at (i).m_macCeValue.m_bufferStatus.at (0));
      }
    }
  }
  
  return;
}

void
PfFfMacScheduler::DoSchedUlCqiInfoReq (const struct FfMacSchedSapProvider::SchedUlCqiInfoReqParameters& params)
{
  NS_LOG_FUNCTION (this);
//   NS_LOG_DEBUG (this << " RX SFNID " << params.m_sfnSf);
  // retrieve the allocation for this subframe
  std::map <uint16_t, std::vector <uint16_t> >::iterator itMap;
  std::map <uint16_t, std::vector <double> >::iterator itCqi;
  itMap = m_allocationMaps.find (params.m_sfnSf);
  if (itMap == m_allocationMaps.end ())
    {
      NS_LOG_DEBUG (this << " Does not find info on allocation, size : " << m_allocationMaps.size ());
      return;
    }
  for (uint32_t i = 0; i < (*itMap).second.size (); i++)
    {
      // convert from fixed point notation Sxxxxxxxxxxx.xxx to double
//       NS_LOG_INFO (this << " i " << i << " size " << params.m_ulCqi.m_sinr.size () << " mapSIze " << (*itMap).second.size ());
      double sinr = LteFfConverter::fpS11dot3toDouble (params.m_ulCqi.m_sinr.at (i));
      //NS_LOG_DEBUG (this << " UE " << (*itMap).second.at (i) << " SINRfp " << params.m_ulCqi.m_sinr.at (i) << " sinrdb " << sinr);
      itCqi = m_ueCqi.find ((*itMap).second.at (i));
      if (itCqi == m_ueCqi.end ())
        {
          // create a new entry
          std::vector <double> newCqi;
          for (uint32_t j = 0; j < m_cschedCellConfig.m_ulBandwidth; j++)
            {
              if (i == j)
                {
                  newCqi.push_back (sinr);
                }
              else
                {
                  // initialize with NO_SINR value.
                  newCqi.push_back (NO_SINR);
                }

            }
          m_ueCqi.insert (std::pair <uint16_t, std::vector <double> > ((*itMap).second.at (i), newCqi));
          // generate correspondent timer
          m_ueCqiTimers.insert (std::pair <uint16_t, uint32_t > ((*itMap).second.at (i), m_cqiTimersThreshold));
        }
      else
        {
          // update the value
          (*itCqi).second.at (i) = sinr;
          // update correspondent timer
          std::map <uint16_t, uint32_t>::iterator itTimers;
          itTimers = m_ueCqiTimers.find ((*itMap).second.at (i));
          (*itTimers).second = m_cqiTimersThreshold;
          
        }

    }
  // remove obsolete info on allocation
  m_allocationMaps.erase (itMap);

  return;
}

void
PfFfMacScheduler::RefreshDlCqiMaps(void)
{
  // refresh DL CQI P01 Map
  std::map <uint16_t,uint32_t>::iterator itP10 = m_p10CqiTimers.begin ();
  while (itP10!=m_p10CqiTimers.end ())
    {
//       NS_LOG_INFO (this << " P10-CQI for user " << (*itP10).first << " is " << (uint32_t)(*itP10).second << " thr " << (uint32_t)m_cqiTimersThreshold);
      if ((*itP10).second == 0)
        {
          // delete correspondent entries
          std::map <uint16_t,uint8_t>::iterator itMap = m_p10CqiRxed.find ((*itP10).first);
          NS_ASSERT_MSG (itMap != m_p10CqiRxed.end (), " Does not find CQI report for user " << (*itP10).first);
          NS_LOG_INFO (this << " P10-CQI exired for user " << (*itP10).first);
          m_p10CqiRxed.erase (itMap);
          std::map <uint16_t,uint32_t>::iterator temp = itP10;
          itP10++;
          m_p10CqiTimers.erase (temp);
        }
      else
        {
          (*itP10).second--;
          itP10++;
        }
    }
  
  // refresh DL CQI A30 Map
  std::map <uint16_t,uint32_t>::iterator itA30 = m_a30CqiTimers.begin ();
  while (itA30!=m_a30CqiTimers.end ())
    {
//       NS_LOG_INFO (this << " A30-CQI for user " << (*itA30).first << " is " << (uint32_t)(*itA30).second << " thr " << (uint32_t)m_cqiTimersThreshold);
      if ((*itA30).second == 0)
        {
          // delete correspondent entries
          std::map <uint16_t,SbMeasResult_s>::iterator itMap = m_a30CqiRxed.find ((*itA30).first);
          NS_ASSERT_MSG (itMap != m_a30CqiRxed.end (), " Does not find CQI report for user " << (*itA30).first);
          NS_LOG_INFO (this << " A30-CQI exired for user " << (*itA30).first);
          m_a30CqiRxed.erase (itMap);
          std::map <uint16_t,uint32_t>::iterator temp = itA30;
          itA30++;
          m_a30CqiTimers.erase (temp);
        }
      else
        {
          (*itA30).second--;
          itA30++;
        }
    }
    
    return;
}


void
PfFfMacScheduler::RefreshUlCqiMaps(void)
{
  // refresh UL CQI  Map
  std::map <uint16_t,uint32_t>::iterator itUl = m_ueCqiTimers.begin ();
  while (itUl!=m_ueCqiTimers.end ())
    {
//       NS_LOG_INFO (this << " UL-CQI for user " << (*itUl).first << " is " << (uint32_t)(*itUl).second << " thr " << (uint32_t)m_cqiTimersThreshold);
      if ((*itUl).second == 0)
        {
          // delete correspondent entries
          std::map <uint16_t, std::vector <double> >::iterator itMap = m_ueCqi.find ((*itUl).first);
          NS_ASSERT_MSG (itMap != m_ueCqi.end (), " Does not find CQI report for user " << (*itUl).first);
          NS_LOG_INFO (this << " UL-CQI exired for user " << (*itUl).first);
          (*itMap).second.clear ();
          m_ueCqi.erase (itMap);
          std::map <uint16_t,uint32_t>::iterator temp = itUl;
          itUl++;
          m_ueCqiTimers.erase (temp);
        }
      else
        {
          (*itUl).second--;
          itUl++;
        }
    }
    
    return;
}

void
PfFfMacScheduler::UpdateDlRlcBufferInfo (uint16_t rnti, uint8_t lcid, uint16_t size)
{
  size = size - 2; // remove the minimum RLC overhead
  std::map<LteFlowId_t, FfMacSchedSapProvider::SchedDlRlcBufferReqParameters>::iterator it;
  LteFlowId_t flow (rnti, lcid);
  it = m_rlcBufferReq.find (flow);
  if (it!=m_rlcBufferReq.end ())
    {
//       NS_LOG_DEBUG (this << " UE " << rnti << " LC " << (uint16_t)lcid << " txqueue " << (*it).second.m_rlcTransmissionQueueSize << " retxqueue " << (*it).second.m_rlcRetransmissionQueueSize << " status " << (*it).second.m_rlcStatusPduSize << " decrease " << size);
      // Update queues: RLC tx order Status, ReTx, Tx
      // Update status queue
      if ((*it).second.m_rlcStatusPduSize <= size)
        {
          size -= (*it).second.m_rlcStatusPduSize;
          (*it).second.m_rlcStatusPduSize = 0;
        }
      else
        {
          (*it).second.m_rlcStatusPduSize -= size;
          return;
        }
      // update retransmission queue  
      if ((*it).second.m_rlcRetransmissionQueueSize <= size)
        {
          size -= (*it).second.m_rlcRetransmissionQueueSize;
          (*it).second.m_rlcRetransmissionQueueSize = 0;
        }
      else
        {
          (*it).second.m_rlcRetransmissionQueueSize -= size;
          return;
        }
      // update transmission queue
      if ((*it).second.m_rlcTransmissionQueueSize <= size)
        {
          size -= (*it).second.m_rlcTransmissionQueueSize;
          (*it).second.m_rlcTransmissionQueueSize = 0;
        }
      else
        {
          (*it).second.m_rlcTransmissionQueueSize -= size;
          return;
        }
    }
  else
    {
      NS_LOG_ERROR (this << " Does not find DL RLC Buffer Report of UE " << rnti);
    }
}

void
PfFfMacScheduler::UpdateUlRlcBufferInfo (uint16_t rnti, uint16_t size)
{
  
  size = size - 2; // remove the minimum RLC overhead
  std::map <uint16_t,uint32_t>::iterator it = m_ceBsrRxed.find (rnti);
  if (it!=m_ceBsrRxed.end ())
    {
//       NS_LOG_DEBUG (this << " UE " << rnti << " size " << size << " BSR " << (*it).second);      
      if ((*it).second >= size)
        {
          (*it).second -= size;
        }
      else
        {
          (*it).second = 0;
        }
    }
  else
    {
      NS_LOG_ERROR (this << " Does not find BSR report info of UE " << rnti);
    }
  
}

void
PfFfMacScheduler::TransmissionModeConfigurationUpdate (uint16_t rnti, uint8_t txMode)
{
  NS_LOG_FUNCTION (this << " RNTI " << rnti << " txMode " << (uint16_t)txMode);
  FfMacCschedSapUser::CschedUeConfigUpdateIndParameters params;
  params.m_rnti = rnti;
  params.m_transmissionMode = txMode;
  m_cschedSapUser->CschedUeConfigUpdateInd (params);
}


}
