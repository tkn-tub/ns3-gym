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
    m_timeWindow (0.5)
{
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
  static TypeId tid = TypeId ("PfFfMacScheduler")
    .SetParent<FfMacScheduler> ()
    .AddConstructor<PfFfMacScheduler> ();
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
  NS_LOG_FUNCTION (this);
  // Not used at this stage
  return;
}

void
PfFfMacScheduler::DoCschedLcConfigReq (const struct FfMacCschedSapProvider::CschedLcConfigReqParameters& params)
{
  NS_LOG_FUNCTION (this << " New LC, rnti: "  << params.m_rnti);
  
  std::map <pfsFlowId_t, pfsFlowPerf_t>::iterator it;
  for (uint16_t i = 0; i < params.m_logicalChannelConfigList.size (); i++)
    {
      pfsFlowId_t flow;
      flow.m_rnti = params.m_rnti;
      flow.m_lcId = params.m_logicalChannelConfigList.at (i).m_logicalChannelIdentity;
      
      it = m_flowStats.find (flow);
      
      if (it == m_flowStats.end ())
        {
          pfsFlowPerf_t flowStats;
          flowStats.flowStart = Simulator::Now ();
          flowStats.totalBytesTransmitted = 0;
          flowStats.lastTtiBytesTrasmitted = 0;
          flowStats.lastAveragedThroughput = 0.0;
          m_flowStats.insert (std::pair<pfsFlowId_t, pfsFlowPerf_t> (flow, flowStats));
        }
      else
        {
          NS_LOG_ERROR ("LC already exists");
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
  
  
  std::map <pfsFlowId_t, pfsFlowPerf_t>::iterator it;
  
  pfsFlowId_t flow;
  flow.m_rnti = params.m_rnti;
  flow.m_lcId = params.m_logicalChannelIdentity;
  
  it = m_flowStats.find (flow);
  
  if (it == m_flowStats.end ())
    {
      NS_LOG_ERROR (this << " RlcBufferReq from an unregistered LC");
    }
  else
    {
      (*it).second.rlcBufferReq = params;
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


void
PfFfMacScheduler::DoSchedDlTriggerReq (const struct FfMacSchedSapProvider::SchedDlTriggerReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // API generated by RLC for triggering the scheduling of a DL subframe
  
  
  // evaluate the relative channel quality indicator for each UE per each RBG 
  // (since we are using allocation type 0 the small unit of allocation is RBG)
  // Resource allocation type 0 (see sec 7.1.6.1 of 36.213)
  int rbgSize = GetRbgSize (m_cschedCellConfig.m_dlBandwidth);
  int rbgNum = m_cschedCellConfig.m_dlBandwidth / rbgSize;
  std::vector <pfsFlowId_t> rbgAllocationMap;
  for (int i = 0; i < rbgNum; i++)
    {
      //NS_LOG_DEBUG (this << " ALLOCATION for RBG " << i << " of " << rbgNum);
      std::map <pfsFlowId_t, pfsFlowPerf_t>::iterator it;
      std::map <pfsFlowId_t, pfsFlowPerf_t>::iterator itMax = m_flowStats.begin ();
      double rcqiMax = 0.0;
      for (it = m_flowStats.begin (); it != m_flowStats.end (); it++)
        {
          if ( ((*it).second.rlcBufferReq.m_rlcTransmissionQueueSize > 0)
            || ((*it).second.rlcBufferReq.m_rlcRetransmissionQueueSize > 0)
            || ((*it).second.rlcBufferReq.m_rlcStatusPduSize > 0) )
            {
              // this UE-LC has data to transmit
              std::map <uint16_t,SbMeasResult_s>::iterator itCqi;
              itCqi = m_a30CqiRxed.find ((*it).first.m_rnti);
              //NS_LOG_DEBUG (this << " ue-lc " << (*it).first.m_rnti);
              uint8_t cqi = 0;
              if (itCqi == m_a30CqiRxed.end ())
                {
                  //NS_LOG_DEBUG (this << " No DL-CQI for this UE-LC " << (*it).first.m_rnti);
                  cqi = 0;
                }
              else
                {
                  cqi = (*itCqi).second.m_higherLayerSelected.at (i).m_sbCqi.at (0);
                }
              uint8_t mcs = LteAmc::GetMcsFromCqi (cqi);
              //NS_LOG_DEBUG (this << " MCS " << (uint32_t)mcs);
              double achievableRate = ((LteAmc::GetTbSizeFromMcs (mcs, 1) / 8)/0.001); // = TB size / TTI
              double rcqi = achievableRate / (*it).second.lastAveragedThroughput;
              //NS_LOG_DEBUG (this << " achievaleRate " << achievableRate << " avg thr " << (*it).second.lastAveragedThroughput);
              //NS_LOG_DEBUG (this << " RCQI " << rcqi);
              
              if (rcqi > rcqiMax)
                {
                  rcqiMax = rcqi;
                  itMax = it;
                }

            }
        } // end for m_rlcBufferReq
      
      if (itMax == m_flowStats.end ())
        {
          // no UE available for this RB
          //NS_LOG_DEBUG (this << " no UE found");
          pfsFlowId_t nullFlow;
          nullFlow.m_rnti = 0;
          rbgAllocationMap.push_back (nullFlow);
        }
      else
        {
          rbgAllocationMap.push_back ((*itMax).first);
          //NS_LOG_DEBUG (this << " UE assigned " << (*itMax).first.m_rnti);
        }
    } // end for RBGs
    
    // generate the transmission opportunities by grouping the adjacent RBGs and
    // creating the correspondent DCIs
    FfMacSchedSapUser::SchedDlConfigIndParameters ret;
    std::vector <pfsFlowId_t>:: iterator flowIt = rbgAllocationMap.begin ();
    int rbgAllocated = 0;
    while (flowIt != rbgAllocationMap.end ())
      {
        if ((*flowIt).m_rnti == 0)
          {
            // skip this RBG
            //NS_LOG_DEBUG (this << " RBG empty " << rbgAllocated);
            rbgAllocated++;
            flowIt++;
            continue;
          }
        int lcRbgNum = 1;
        while (((flowIt+1) != rbgAllocationMap.end ()) && ((*flowIt).m_rnti == (*(flowIt+1)).m_rnti))
          {
            lcRbgNum++;
            flowIt++;
          }
        //NS_LOG_DEBUG (this << " Tx Opp for user " << (*flowIt).m_rnti);
        //NS_LOG_DEBUG (this << " Tx Opp from " << rbgAllocated << " to " << rbgAllocated + lcRbgNum);
        // create new BuildDataListElement_s for this LC
        BuildDataListElement_s newEl;
        newEl.m_rnti = (*flowIt).m_rnti;
        // NS_LOG_DEBUG (this << "Allocate user " << newEl.m_rnti << " rbg " << rbgPerFlow);
        // create the DlDciListElement_s
        DlDciListElement_s newDci;
        newDci.m_rnti = (*flowIt).m_rnti;
        newDci.m_resAlloc = 0;
        newDci.m_rbBitmap = 0; // TBD (32 bit bitmap see 7.1.6 of 36.213)
        uint32_t rbgMask = 0;
        for (int i = 0; i < lcRbgNum; i++)
          {
            rbgMask = rbgMask + (0x1 << rbgAllocated);
            // NS_LOG_DEBUG (this << " Allocated PRB " << rbgAllocated);
            rbgAllocated++;
          }
        newDci.m_rbBitmap = rbgMask; // (32 bit bitmap see 7.1.6 of 36.213)
        
        int nbOfTbsInNewDci = 1;  // SISO -> only one TB
        int rlcPduSize = 0;
        for (int i = 0; i < nbOfTbsInNewDci; i++)
          {
            std::map <uint16_t,SbMeasResult_s>::iterator itCqi;
            itCqi = m_a30CqiRxed.find (newDci.m_rnti);
            if (itCqi == m_a30CqiRxed.end ())
              {
                newDci.m_mcs.push_back (1); // no info on this user -> lowest MCS
                //NS_LOG_DEBUG (this << " No DL-CQI for this allocated UE-LC");
              }
            else
              {
                newDci.m_mcs.push_back ( LteAmc::GetMcsFromCqi ((*itCqi).second.m_higherLayerSelected.at (i).m_sbCqi.at (0)) );
                //NS_LOG_DEBUG (this << " MCS " << (uint32_t)newDci.m_mcs.at(0));
              }
            int nPRB = rbgSize * lcRbgNum;
            newDci.m_tbsSize.push_back ( (LteAmc::GetTbSizeFromMcs (newDci.m_mcs.at (i), nPRB) / 8) ); // (size of TB in bytes according to table 7.1.7.2.1-1 of 36.213)
            newDci.m_ndi.push_back (1); // TBD (new data indicator)
            newDci.m_rv.push_back (0); // TBD (redundancy version)
            //NS_LOG_DEBUG (this << " nPRB " << nPRB << " tbSize " << newDci.m_tbsSize.at (0));
            
            rlcPduSize += newDci.m_tbsSize.at (i);
          }
        newEl.m_dci = newDci;
        // ...more parameters -> ingored in this version
        
        RlcPduListElement_s newRlcEl;
        newRlcEl.m_logicalChannelIdentity = (*flowIt).m_lcId;
        // NS_LOG_DEBUG (this << "LCID " << (uint32_t) newRlcEl.m_logicalChannelIdentity);
        newRlcEl.m_size = rlcPduSize; // TBD (max length of RLC-PDU in bytes)
        
        std::vector <struct RlcPduListElement_s> newRlcPduLe;
        newRlcPduLe.push_back (newRlcEl);
        
        newEl.m_rlcPduList.push_back (newRlcPduLe);
        ret.m_buildDataList.push_back (newEl);
        
        // update UE-LC stats
        std::map <pfsFlowId_t, pfsFlowPerf_t>::iterator it;
        it = m_flowStats.find ((*flowIt));
        if (it != m_flowStats.end())
          {
            (*it).second.lastTtiBytesTrasmitted += rlcPduSize;
            //NS_LOG_DEBUG (this << " UE bytes txed " << (*it).second.lastTtiBytesTrasmitted);
          }
//         else
//           {
//             NS_LOG_DEBUG (this << " No Stats for this allocated UE-LC");
//           }
        if (rbgAllocated == rbgNum)
          {
            break;                       // no more RGB to be allocated
          }
          
        if ((flowIt+1) == rbgAllocationMap.end ())
          {
            // no more flows to allocated
            break;
          }
        else
          {
            flowIt++;
          }
         
      } // end while allocation
      ret.m_nrOfPdcchOfdmSymbols = 1;   // TODO: check correct value according the DCIs txed
      
      // update the UE-LC stats
      std::map <pfsFlowId_t, pfsFlowPerf_t>::iterator it;
      for (it = m_flowStats.begin (); it != m_flowStats.end (); it++)
        {
          (*it).second.totalBytesTransmitted += (*it).second.lastTtiBytesTrasmitted;
          // update average throughput (see eq. 12.3 of Sec 12.3.1.2 of LTE – The UMTS Long Term Evolution, Ed Wiley)
          (*it).second.lastAveragedThroughput = ((1 - (1 / m_timeWindow)) * (*it).second.lastAveragedThroughput) + ((1 / m_timeWindow) * ((*it).second.lastTtiBytesTrasmitted / 0.001));
          (*it).second.lastTtiBytesTrasmitted = 0;
          //NS_LOG_DEBUG (this << " UE tot bytes " << (*it).second.totalBytesTransmitted);
//           NS_LOG_DEBUG (this << " UE avg thr " << (*it).second.lastAveragedThroughput);
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
            }
          else
            {
              // update the CQI value
              (*it).second = params.m_cqiList.at (i).m_wbCqi.at (0);
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
            }
          else
            {
              // update the CQI value
              (*it).second = params.m_cqiList.at (i).m_sbMeasResult;
            }
        }
      else
        {
          NS_LOG_ERROR (this << " CQI type unknown");
        }
    }

  return;
}

void
PfFfMacScheduler::DoSchedUlTriggerReq (const struct FfMacSchedSapProvider::SchedUlTriggerReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // TODO: Implementation of the API
  
  std::map <uint16_t,uint8_t>::iterator it; 
  int nflows = 0;
  
  for (it = m_ceBsrRxed.begin (); it != m_ceBsrRxed.end (); it++)
    {
      // remove old entries of this UE-LC
      if ((*it).second > 0)
      {
        nflows++;
      }
    }
  
  if (nflows==0)
  {
    return ;  // no flows to be scheduled
  } 
   
  
  // Divide the resource equally among the active users
  int rbPerFlow = m_cschedCellConfig.m_ulBandwidth / nflows;
  if (rbPerFlow == 0)
  {
    rbPerFlow = 1;                // at least 1 rbg per flow (till available resource)
  }
  int rbAllocated = 0;
  
  FfMacSchedSapUser::SchedUlConfigIndParameters ret;

  for (it = m_ceBsrRxed.begin (); it != m_ceBsrRxed.end (); it++)
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
      rbAllocated += rbPerFlow;
      uldci.m_mcs = 0; // MCS 0 -> UL-AMC TBD
      uldci.m_tbSize = (LteAmc::GetTbSizeFromMcs (uldci.m_mcs, rbPerFlow) / 8); // MCS 0 -> UL-AMC TBD
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
    }
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
  
  std::map <uint16_t,uint8_t>::iterator it;
  
  for (unsigned int i = 0; i < params.m_macCeList.size (); i++)
  {
    if ( params.m_macCeList.at (i).m_macCeType == MacCeListElement_s::BSR )
    {
      // buffer status report
      uint16_t rnti = params.m_macCeList.at (i).m_rnti;
      it = m_ceBsrRxed.find(rnti);
      if (it==m_ceBsrRxed.end())
      {
        // create the new entry
        uint8_t bsr = params.m_macCeList.at (i).m_macCeValue.m_bufferStatus.at (0);
        m_ceBsrRxed.insert( std::pair<uint16_t, uint8_t > (rnti, bsr)); // only 1 buffer status is working now
      }
      else
      {
        // update the CQI value
        (*it).second = params.m_macCeList.at (i).m_macCeValue.m_bufferStatus.at (0);
      }
    }
  }
  
  return;
}

void
PfFfMacScheduler::DoSchedUlCqiInfoReq (const struct FfMacSchedSapProvider::SchedUlCqiInfoReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // TODO: Implementation of the API
  return;
}

}
