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

#include "lte-amc.h"
#include "rr-ff-mac-scheduler.h"

NS_LOG_COMPONENT_DEFINE ("RrFfMacScheduler");

namespace ns3 {

int Type0AllocationRbg[4] = {
  10,       // RGB size 1
  26,       // RGB size 2
  63,       // RGB size 3
  110       // RGB size 4
};  // see table 7.1.6.1-1 of 36.213


NS_OBJECT_ENSURE_REGISTERED (RrFfMacScheduler);


class RrSchedulerMemberCschedSapProvider : public FfMacCschedSapProvider
{
public:
  RrSchedulerMemberCschedSapProvider (RrFfMacScheduler* scheduler);

  // inherited from FfMacCschedSapProvider
  virtual void CschedCellConfigReq (const struct CschedCellConfigReqParameters& params);
  virtual void CschedUeConfigReq (const struct CschedUeConfigReqParameters& params);
  virtual void CschedLcConfigReq (const struct CschedLcConfigReqParameters& params);
  virtual void CschedLcReleaseReq (const struct CschedLcReleaseReqParameters& params);
  virtual void CschedUeReleaseReq (const struct CschedUeReleaseReqParameters& params);

private:
  RrSchedulerMemberCschedSapProvider ();
  RrFfMacScheduler* m_scheduler;
};

RrSchedulerMemberCschedSapProvider::RrSchedulerMemberCschedSapProvider ()
{
}

RrSchedulerMemberCschedSapProvider::RrSchedulerMemberCschedSapProvider (RrFfMacScheduler* scheduler) : m_scheduler (scheduler)
{
}


void
RrSchedulerMemberCschedSapProvider::CschedCellConfigReq (const struct CschedCellConfigReqParameters& params)
{
  m_scheduler->DoCschedCellConfigReq (params);
}

void
RrSchedulerMemberCschedSapProvider::CschedUeConfigReq (const struct CschedUeConfigReqParameters& params)
{
  m_scheduler->DoCschedUeConfigReq (params);
}


void
RrSchedulerMemberCschedSapProvider::CschedLcConfigReq (const struct CschedLcConfigReqParameters& params)
{
  m_scheduler->DoCschedLcConfigReq (params);
}

void
RrSchedulerMemberCschedSapProvider::CschedLcReleaseReq (const struct CschedLcReleaseReqParameters& params)
{
  m_scheduler->DoCschedLcReleaseReq (params);
}

void
RrSchedulerMemberCschedSapProvider::CschedUeReleaseReq (const struct CschedUeReleaseReqParameters& params)
{
  m_scheduler->DoCschedUeReleaseReq (params);
}




class RrSchedulerMemberSchedSapProvider : public FfMacSchedSapProvider
{
public:
  RrSchedulerMemberSchedSapProvider (RrFfMacScheduler* scheduler);

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
  RrSchedulerMemberSchedSapProvider ();
  RrFfMacScheduler* m_scheduler;
};



RrSchedulerMemberSchedSapProvider::RrSchedulerMemberSchedSapProvider ()
{
}


RrSchedulerMemberSchedSapProvider::RrSchedulerMemberSchedSapProvider (RrFfMacScheduler* scheduler)
  : m_scheduler (scheduler)
{
}

void
RrSchedulerMemberSchedSapProvider::SchedDlRlcBufferReq (const struct SchedDlRlcBufferReqParameters& params)
{
  m_scheduler->DoSchedDlRlcBufferReq (params);
}

void
RrSchedulerMemberSchedSapProvider::SchedDlPagingBufferReq (const struct SchedDlPagingBufferReqParameters& params)
{
  m_scheduler->DoSchedDlPagingBufferReq (params);
}

void
RrSchedulerMemberSchedSapProvider::SchedDlMacBufferReq (const struct SchedDlMacBufferReqParameters& params)
{
  m_scheduler->DoSchedDlMacBufferReq (params);
}

void
RrSchedulerMemberSchedSapProvider::SchedDlTriggerReq (const struct SchedDlTriggerReqParameters& params)
{
  m_scheduler->DoSchedDlTriggerReq (params);
}

void
RrSchedulerMemberSchedSapProvider::SchedDlRachInfoReq (const struct SchedDlRachInfoReqParameters& params)
{
  m_scheduler->DoSchedDlRachInfoReq (params);
}

void
RrSchedulerMemberSchedSapProvider::SchedDlCqiInfoReq (const struct SchedDlCqiInfoReqParameters& params)
{
  m_scheduler->DoSchedDlCqiInfoReq (params);
}

void
RrSchedulerMemberSchedSapProvider::SchedUlTriggerReq (const struct SchedUlTriggerReqParameters& params)
{
  m_scheduler->DoSchedUlTriggerReq (params);
}

void
RrSchedulerMemberSchedSapProvider::SchedUlNoiseInterferenceReq (const struct SchedUlNoiseInterferenceReqParameters& params)
{
  m_scheduler->DoSchedUlNoiseInterferenceReq (params);
}

void
RrSchedulerMemberSchedSapProvider::SchedUlSrInfoReq (const struct SchedUlSrInfoReqParameters& params)
{
  m_scheduler->DoSchedUlSrInfoReq (params);
}

void
RrSchedulerMemberSchedSapProvider::SchedUlMacCtrlInfoReq (const struct SchedUlMacCtrlInfoReqParameters& params)
{
  m_scheduler->DoSchedUlMacCtrlInfoReq (params);
}

void
RrSchedulerMemberSchedSapProvider::SchedUlCqiInfoReq (const struct SchedUlCqiInfoReqParameters& params)
{
  m_scheduler->DoSchedUlCqiInfoReq (params);
}





RrFfMacScheduler::RrFfMacScheduler ()
  :   m_cschedSapUser (0),
    m_schedSapUser (0)
{
  m_cschedSapProvider = new RrSchedulerMemberCschedSapProvider (this);
  m_schedSapProvider = new RrSchedulerMemberSchedSapProvider (this);
}

RrFfMacScheduler::~RrFfMacScheduler ()
{
  NS_LOG_FUNCTION (this);
}

void
RrFfMacScheduler::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_cschedSapProvider;
  delete m_schedSapProvider;
}

TypeId
RrFfMacScheduler::GetTypeId (void)
{
  static TypeId tid = TypeId ("RrFfMacScheduler")
    .SetParent<FfMacScheduler> ()
    .AddConstructor<RrFfMacScheduler> ();
  return tid;
}



void
RrFfMacScheduler::SetFfMacCschedSapUser (FfMacCschedSapUser* s)
{
  m_cschedSapUser = s;
}

void
RrFfMacScheduler::SetFfMacSchedSapUser (FfMacSchedSapUser* s)
{
  m_schedSapUser = s;
}

FfMacCschedSapProvider*
RrFfMacScheduler::GetFfMacCschedSapProvider ()
{
  return m_cschedSapProvider;
}

FfMacSchedSapProvider*
RrFfMacScheduler::GetFfMacSchedSapProvider ()
{
  return m_schedSapProvider;
}

void
RrFfMacScheduler::DoCschedCellConfigReq (const struct FfMacCschedSapProvider::CschedCellConfigReqParameters& params)
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
RrFfMacScheduler::DoCschedUeConfigReq (const struct FfMacCschedSapProvider::CschedUeConfigReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // Not used at this stage
  return;
}

void
RrFfMacScheduler::DoCschedLcConfigReq (const struct FfMacCschedSapProvider::CschedLcConfigReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // Not used at this stage
  return;
}

void
RrFfMacScheduler::DoCschedLcReleaseReq (const struct FfMacCschedSapProvider::CschedLcReleaseReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // TODO: Implementation of the API
  return;
}

void
RrFfMacScheduler::DoCschedUeReleaseReq (const struct FfMacCschedSapProvider::CschedUeReleaseReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // TODO: Implementation of the API
  return;
}


void
RrFfMacScheduler::DoSchedDlRlcBufferReq (const struct FfMacSchedSapProvider::SchedDlRlcBufferReqParameters& params)
{
  NS_LOG_FUNCTION (this << params.m_rnti << (uint32_t) params.m_logicalChannelIdentity);
  // API generated by RLC for updating RLC parameters on a LC (tx and retx queues)
  std::vector<FfMacSchedSapProvider::SchedDlRlcBufferReqParameters>::iterator it;

  for (it = m_rlcBufferReq.begin (); it != m_rlcBufferReq.end (); it++)
    {
      // remove old entries of this UE-LC
      if (((*it).m_rnti == params.m_rnti)&&((*it).m_logicalChannelIdentity == params.m_logicalChannelIdentity))
        {
          m_rlcBufferReq.erase (it);
        }
    }
  // add the new paramters
  m_rlcBufferReq.insert (it, params);

  return;
}

void
RrFfMacScheduler::DoSchedDlPagingBufferReq (const struct FfMacSchedSapProvider::SchedDlPagingBufferReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // TODO: Implementation of the API
  return;
}

void
RrFfMacScheduler::DoSchedDlMacBufferReq (const struct FfMacSchedSapProvider::SchedDlMacBufferReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // TODO: Implementation of the API
  return;
}

int
RrFfMacScheduler::GetRbgSize (int dlbandwidth)
{
  for (int i = 0; i < 4; i++)
    {
      if (dlbandwidth < Type0AllocationRbg[i])
        {
          return (i + 1);
        }
    }

  return (-1);
}


void
RrFfMacScheduler::DoSchedDlTriggerReq (const struct FfMacSchedSapProvider::SchedDlTriggerReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // API generated by RLC for triggering the scheduling of a DL subframe

  // Get the actual active flows (queue!=0)
  std::vector<FfMacSchedSapProvider::SchedDlRlcBufferReqParameters>::iterator it;
  int nflows = 0;

  for (it = m_rlcBufferReq.begin (); it != m_rlcBufferReq.end (); it++)
    {
      // remove old entries of this UE-LC
      if ( ((*it).m_rlcTransmissionQueueSize > 0)
           || ((*it).m_rlcRetransmissionQueueSize > 0)
           || ((*it).m_rlcStatusPduSize > 0) )
        {
          nflows++;
        }
    }

  if (nflows == 0)
    {
      return;
    }

  // Divide the resource equally among the active users according to
  // Resource allocation type 0 (see sec 7.1.6.1 of 36.213)
  int rbgSize = GetRbgSize (m_cschedCellConfig.m_dlBandwidth);
  int rbgNum = m_cschedCellConfig.m_dlBandwidth / rbgSize;
  int rbgPerFlow = rbgNum / nflows;
  if (rbgPerFlow == 0)
    {
      rbgPerFlow = 1;                // at least 1 rbg per flow (till available resource)
    }
  int rbgAllocated = 0;

  FfMacSchedSapUser::SchedDlConfigIndParameters ret;

  // round robin assignment to all UE-LC registered
  // std::vector<FfMacSchedSapProvider::SchedDlRlcBufferReqParameters>::iterator it;
  for (it = m_rlcBufferReq.begin (); it != m_rlcBufferReq.end (); it++)
    {
      // create new BuildDataListElement_s for this LC
      BuildDataListElement_s newEl;
      newEl.m_rnti = (*it).m_rnti;
      // NS_LOG_DEBUG (this << "Allocate user " << newEl.m_rnti << " rbg " << rbgPerFlow);
      // create the DlDciListElement_s
      DlDciListElement_s newDci;
      newDci.m_rnti = (*it).m_rnti;
      newDci.m_resAlloc = 0;
      newDci.m_rbBitmap = 0; // TBD (32 bit bitmap see 7.1.6 of 36.213)
      uint32_t rbgMask = 0;
      for (int i = 0; i < rbgPerFlow; i++)
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
          std::map <uint16_t,uint8_t>::iterator it = m_p10CqiRxed.find (newDci.m_rnti);
          if (it == m_p10CqiRxed.end ())
            {
              newDci.m_mcs.push_back (1); // no info on this user -> lowest MCS
            }
          else
            {
              newDci.m_mcs.push_back ( LteAmc::GetMcsFromCqi ((*it).second) );
            }
          int nPRB = rbgSize * rbgPerFlow;
          newDci.m_tbsSize.push_back ( (LteAmc::GetTbSizeFromMcs (newDci.m_mcs.at (i), nPRB) / 8) ); // (size of TB in bytes according to table 7.1.7.2.1-1 of 36.213)
          newDci.m_ndi.push_back (1); // TBD (new data indicator)
          newDci.m_rv.push_back (0); // TBD (redundancy version)

          rlcPduSize += newDci.m_tbsSize.at (i);
        }
      newEl.m_dci = newDci;
      // ...more parameters -> ignored in this version

      RlcPduListElement_s newRlcEl;
      newRlcEl.m_logicalChannelIdentity = (*it).m_logicalChannelIdentity;
      // NS_LOG_DEBUG (this << "LCID " << (uint32_t) newRlcEl.m_logicalChannelIdentity);
      newRlcEl.m_size = rlcPduSize; // TBD (max length of RLC-PDU in bytes)

      std::vector <struct RlcPduListElement_s> newRlcPduLe;
      newRlcPduLe.push_back (newRlcEl);

      newEl.m_rlcPduList.push_back (newRlcPduLe);
      ret.m_buildDataList.push_back (newEl);
      if (rbgAllocated == rbgNum)
        {
          break;                       // no more RGB to be allocated
        }
    }

  ret.m_nrOfPdcchOfdmSymbols = 1;   // TODO: check correct value according the DCIs txed

  m_schedSapUser->SchedDlConfigInd (ret);
  return;
}

void
RrFfMacScheduler::DoSchedDlRachInfoReq (const struct FfMacSchedSapProvider::SchedDlRachInfoReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // TODO: Implementation of the API
  return;
}

void
RrFfMacScheduler::DoSchedDlCqiInfoReq (const struct FfMacSchedSapProvider::SchedDlCqiInfoReqParameters& params)
{
  NS_LOG_FUNCTION (this);

  std::map <uint16_t,uint8_t>::iterator it;
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
          // Not used by RR Scheduler
        }
      else
        {
          NS_LOG_ERROR (this << " CQI type unknown");
        }
    }

  return;
}

void
RrFfMacScheduler::DoSchedUlTriggerReq (const struct FfMacSchedSapProvider::SchedUlTriggerReqParameters& params)
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
RrFfMacScheduler::DoSchedUlNoiseInterferenceReq (const struct FfMacSchedSapProvider::SchedUlNoiseInterferenceReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // TODO: Implementation of the API
  return;
}

void
RrFfMacScheduler::DoSchedUlSrInfoReq (const struct FfMacSchedSapProvider::SchedUlSrInfoReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // TODO: Implementation of the API
  return;
}

void
RrFfMacScheduler::DoSchedUlMacCtrlInfoReq (const struct FfMacSchedSapProvider::SchedUlMacCtrlInfoReqParameters& params)
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
RrFfMacScheduler::DoSchedUlCqiInfoReq (const struct FfMacSchedSapProvider::SchedUlCqiInfoReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  // TODO: Implementation of the API
  return;
}

}
