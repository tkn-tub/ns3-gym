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

#include <ns3/lte-amc.h>
#include <ns3/rr-ff-mac-scheduler.h>
#include <ns3/simulator.h>
#include <ns3/lte-common.h>

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
    m_schedSapUser (0),
    m_nextRntiDl (0),
    m_nextRntiUl (0)
{
  m_amc = CreateObject <LteAmc> ();
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
  static TypeId tid = TypeId ("ns3::RrFfMacScheduler")
    .SetParent<FfMacScheduler> ()
    .AddConstructor<RrFfMacScheduler> ()
    .AddAttribute ("CqiTimerThreshold",
                   "The number of TTIs a CQI is valid (default 1000 - 1 sec.)",
                   UintegerValue (1000),
                   MakeUintegerAccessor (&RrFfMacScheduler::m_cqiTimersThreshold),
                   MakeUintegerChecker<uint32_t> ())
    ;
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
  std::list<FfMacSchedSapProvider::SchedDlRlcBufferReqParameters>::iterator it = m_rlcBufferReq.begin ();
  bool newLc = true;
  while (it != m_rlcBufferReq.end ())
    {
      // remove old entries of this UE-LC
      if (((*it).m_rnti == params.m_rnti)&&((*it).m_logicalChannelIdentity == params.m_logicalChannelIdentity))
        {
          it = m_rlcBufferReq.erase (it);
          newLc = false;
        }
      else
        {
          ++it;
        }
    }
  // add the new parameters
  m_rlcBufferReq.insert (it, params);
  // initialize statistics of the flow in case of new flows
  if (newLc == true)
    {
      m_p10CqiRxed.insert ( std::pair<uint16_t, uint8_t > (params.m_rnti, 1)); // only codeword 0 at this stage (SISO)
      // initialized to 1 (i.e., the lowest value for transmitting a signal)
      m_p10CqiTimers.insert ( std::pair<uint16_t, uint32_t > (params.m_rnti, m_cqiTimersThreshold));
    }

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

bool
RrFfMacScheduler::SortRlcBufferReq (FfMacSchedSapProvider::SchedDlRlcBufferReqParameters i,FfMacSchedSapProvider::SchedDlRlcBufferReqParameters j)
{
  return (i.m_rnti<j.m_rnti);
}


void
RrFfMacScheduler::DoSchedDlTriggerReq (const struct FfMacSchedSapProvider::SchedDlTriggerReqParameters& params)
{
  NS_LOG_FUNCTION (this << " DL Frame no. " << (params.m_sfnSf >> 4) << " subframe no. " << (0xF & params.m_sfnSf));
  // API generated by RLC for triggering the scheduling of a DL subframe
  
  RefreshDlCqiMaps ();

  // Get the actual active flows (queue!=0)
  std::list<FfMacSchedSapProvider::SchedDlRlcBufferReqParameters>::iterator it;
  m_rlcBufferReq.sort (SortRlcBufferReq);
  int nflows = 0;
  int nTbs = 0;
  std::map <uint16_t,uint8_t> lcActivesPerRnti;
  std::map <uint16_t,uint8_t>::iterator itLcRnti;
  for (it = m_rlcBufferReq.begin (); it != m_rlcBufferReq.end (); it++)
    {
//       NS_LOG_INFO (this << " User " << (*it).m_rnti << " LC " << (uint16_t)(*it).m_logicalChannelIdentity);
      // remove old entries of this UE-LC
      if ( ((*it).m_rlcTransmissionQueueSize > 0)
           || ((*it).m_rlcRetransmissionQueueSize > 0)
           || ((*it).m_rlcStatusPduSize > 0) )
        {
          std::map <uint16_t,uint8_t>::iterator itCqi = m_p10CqiRxed.find ((*it).m_rnti);
          uint8_t cqi = 0;
          if (itCqi != m_p10CqiRxed.end ())
            {
              cqi = (*itCqi).second;
            }
          else
            {
              cqi = 1; // lowest value fro trying a transmission
            }
          if (cqi != 0)
            {
              // CQI == 0 means "out of range" (see table 7.2.3-1 of 36.213)
              nflows++;
              itLcRnti = lcActivesPerRnti.find ((*it).m_rnti);
              if (itLcRnti != lcActivesPerRnti.end ())
                {
                  (*itLcRnti).second++;
                }
              else
                {
                  lcActivesPerRnti.insert (std::pair<uint16_t, uint8_t > ((*it).m_rnti, 1));
                  nTbs++;
                }
        
            }
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
  int rbgPerTb = rbgNum / nTbs;
  if (rbgPerTb == 0)
    {
      rbgPerTb = 1;                // at least 1 rbg per TB (till available resource)
    }
  int rbgAllocated = 0;

  FfMacSchedSapUser::SchedDlConfigIndParameters ret;
  // round robin assignment to all UE-LC registered starting from the subsequent of the one
  // served last scheduling trigger
  //NS_LOG_DEBUG (this << " next to be served " << m_nextRntiDl << " nflows " << nflows);
  if (m_nextRntiDl != 0)
    {
      for (it = m_rlcBufferReq.begin (); it != m_rlcBufferReq.end (); it++)
        {
          if ((*it).m_rnti == m_nextRntiDl)
            {
              break;
            }
        }

      if (it == m_rlcBufferReq.end ())
        {
          NS_LOG_ERROR (this << " no user found");
        }
    }
  else
    {
      it = m_rlcBufferReq.begin ();
      m_nextRntiDl = (*it).m_rnti;
    }
  std::map <uint16_t,uint8_t>::iterator itTxMode;
  do
    {
      itLcRnti = lcActivesPerRnti.find ((*it).m_rnti);
      if (itLcRnti == lcActivesPerRnti.end ())
        {
          // skip this entry
          it++;
          if (it == m_rlcBufferReq.end ())
            {
              // restart from the first
              it = m_rlcBufferReq.begin ();
            }
          continue;
        }
      itTxMode = m_uesTxMode.find ((*it).m_rnti);
      if (itTxMode == m_uesTxMode.end())
        {
          NS_FATAL_ERROR ("No Transmission Mode info on user " << (*it).m_rnti);
        }
      int nLayer = TransmissionModesLayers::TxMode2LayerNum ((*itTxMode).second);
      int lcNum = (*itLcRnti).second;
      // create new BuildDataListElement_s for this RNTI
      BuildDataListElement_s newEl;
      newEl.m_rnti = (*it).m_rnti;
      // create the DlDciListElement_s
      DlDciListElement_s newDci;
      newDci.m_rnti = (*it).m_rnti;
      newDci.m_resAlloc = 0;
      newDci.m_rbBitmap = 0;
      std::map <uint16_t,uint8_t>::iterator itCqi = m_p10CqiRxed.find (newEl.m_rnti);
      for (uint8_t i = 0; i < nLayer; i++) 
        {
          if (itCqi == m_p10CqiRxed.end ())
            {
              newDci.m_mcs.push_back (0); // no info on this user -> lowest MCS
            }
          else
            {
              newDci.m_mcs.push_back ( m_amc->GetMcsFromCqi ((*itCqi).second) );
            }
        }
      // group the LCs of this RNTI
      std::vector <struct RlcPduListElement_s> newRlcPduLe;
//       int totRbg = lcNum * rbgPerFlow;
//       totRbg = rbgNum / nTbs;
      int tbSize = (m_amc->GetTbSizeFromMcs (newDci.m_mcs.at (0), rbgPerTb * rbgSize) / 8);
      NS_LOG_DEBUG (this << "Allocate user " << newEl.m_rnti << " LCs " << (uint16_t)(*itLcRnti).second << " bytes " << tbSize << " PRBs " <<  rbgAllocated * rbgSize << "..." << (rbgAllocated* rbgSize) + (rbgPerTb * rbgSize) - 1 << " mcs " << (uint16_t) newDci.m_mcs.at (0) << " layers " << nLayer);
      uint16_t rlcPduSize = tbSize / lcNum;
      for (int i = 0; i < lcNum ; i++)
        {
          for (uint8_t j = 0; j < nLayer; j++)
            {
              RlcPduListElement_s newRlcEl;
              newRlcEl.m_logicalChannelIdentity = (*it).m_logicalChannelIdentity;
//               NS_LOG_DEBUG (this << "LCID " << (uint32_t) newRlcEl.m_logicalChannelIdentity << " size " << rlcPduSize << " ID " << (*it).m_rnti << " layer " << (uint16_t)j);
              newRlcEl.m_size = rlcPduSize;
              UpdateDlRlcBufferInfo ((*it).m_rnti, newRlcEl.m_logicalChannelIdentity, rlcPduSize);
              newRlcPduLe.push_back (newRlcEl);
            }
          it++;
          if (it == m_rlcBufferReq.end ())
            {
              // restart from the first
              it = m_rlcBufferReq.begin ();
            }
        }
      uint32_t rbgMask = 0;
      for (int i = 0; i < rbgPerTb; i++)
        {
          rbgMask = rbgMask + (0x1 << rbgAllocated);
          rbgAllocated++;
        }
      newDci.m_rbBitmap = rbgMask; // (32 bit bitmap see 7.1.6 of 36.213)

      for (int i = 0; i < nLayer; i++)
        {
          newDci.m_tbsSize.push_back (tbSize);
          newDci.m_ndi.push_back (1); // TBD (new data indicator)
          newDci.m_rv.push_back (0); // TBD (redundancy version)
        }
      newEl.m_dci = newDci;
      // ...more parameters -> ignored in this version




      newEl.m_rlcPduList.push_back (newRlcPduLe);
      ret.m_buildDataList.push_back (newEl);
      if (rbgAllocated == rbgNum)
        {
          //NS_LOG_DEBUG (this << " FULL " << (*it).m_rnti);
          m_nextRntiDl = (*it).m_rnti; // store last RNTI served
          break;                       // no more RGB to be allocated
        }
    }
  while ((*it).m_rnti != m_nextRntiDl);

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
              // generate correspondent timer
              m_p10CqiTimers.insert ( std::pair<uint16_t, uint32_t > (rnti, m_cqiTimersThreshold));
            }
          else
            {
              // update the CQI value
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
  NS_LOG_FUNCTION (this << " Ul - Frame no. " << (params.m_sfnSf >> 4) << " subframe no. " << (0xF & params.m_sfnSf));


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


  // Divide the resource equally among the active users starting from the subsequent one served last scheduling trigger
  int rbPerFlow = m_cschedCellConfig.m_ulBandwidth / nflows;
  if (rbPerFlow == 0)
    {
      rbPerFlow = 1;              // at least 1 rbg per flow (till available resource)
    }
  int rbAllocated = 0;

  FfMacSchedSapUser::SchedUlConfigIndParameters ret;
  std::vector <uint16_t> rbgAllocationMap;
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
          NS_LOG_DEBUG (this << " UE does not have ULCQI " << (*it).first );
        }
      else
        {
          // take the lowest CQI value (worst RB)
          double minSinr = (*itCqi).second.at (uldci.m_rbStart);
          for (uint16_t i = uldci.m_rbStart; i < uldci.m_rbStart + uldci.m_rbLen; i++)
            {
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
        
      uldci.m_tbSize = (m_amc->GetTbSizeFromMcs (uldci.m_mcs, rbPerFlow) / 8); // MCS 0 -> UL-AMC TBD
      NS_LOG_DEBUG (this << " UL - UE " << (*it).first << " startPRB " << (uint32_t)uldci.m_rbStart << " nPRB " << (uint32_t)uldci.m_rbLen << " CQI " << cqi << " MCS " << (uint32_t)uldci.m_mcs << " TBsize " << uldci.m_tbSize);
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
  m_allocationMaps.insert (std::pair <uint16_t, std::vector <uint16_t> > (params.m_sfnSf, rbgAllocationMap));
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
RrFfMacScheduler::DoSchedUlCqiInfoReq (const struct FfMacSchedSapProvider::SchedUlCqiInfoReqParameters& params)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG (this << " RX SFNID " << params.m_sfnSf);
//     NS_LOG_DEBUG (this << " Actual sfn " << frameNo << " sbfn " << subframeNo << " sfnSf "  << sfnSf);
  // retrieve the allocation for this subframe
  std::map <uint16_t, std::vector <uint16_t> >::iterator itMap;
  std::map <uint16_t, std::vector <double> >::iterator itCqi;
  itMap = m_allocationMaps.find (params.m_sfnSf);
  if (itMap == m_allocationMaps.end ())
    {
      NS_LOG_DEBUG (this << " Does not find info on allocation");
      return;
    }
  for (uint32_t i = 0; i < (*itMap).second.size (); i++)
    {
      // convert from fixed point notation Sxxxxxxxxxxx.xxx to double
      double sinr = LteFfConverter::fpS11dot3toDouble (params.m_ulCqi.m_sinr.at (i));
//       NS_LOG_DEBUG (this << " RB " << i << "UE " << (*itMap).second.at (i) << " SINRfp " << params.m_ulCqi.m_sinr.at (i) << " sinrdb " << sinr);
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
                  // initialize with maximum value according to the fixed point notation
                  newCqi.push_back (30.0);
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
RrFfMacScheduler::RefreshDlCqiMaps(void)
{
  NS_LOG_FUNCTION (this << m_p10CqiTimers.size ());
  // refresh DL CQI P01 Map
  std::map <uint16_t,uint32_t>::iterator itP10 = m_p10CqiTimers.begin ();
  while (itP10!=m_p10CqiTimers.end ())
    {
      NS_LOG_INFO (this << " P10-CQI for user " << (*itP10).first << " is " << (uint32_t)(*itP10).second << " thr " << (uint32_t)m_cqiTimersThreshold);
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
  
  return;
}


void
RrFfMacScheduler::RefreshUlCqiMaps(void)
{
  // refresh UL CQI  Map
  std::map <uint16_t,uint32_t>::iterator itUl = m_ueCqiTimers.begin ();
  while (itUl!=m_ueCqiTimers.end ())
    {
      NS_LOG_INFO (this << " UL-CQI for user " << (*itUl).first << " is " << (uint32_t)(*itUl).second << " thr " << (uint32_t)m_cqiTimersThreshold);
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
RrFfMacScheduler::UpdateDlRlcBufferInfo (uint16_t rnti, uint8_t lcid, uint16_t size)
{
  size = size - 2; // remove the minimum RLC overhead
  std::list<FfMacSchedSapProvider::SchedDlRlcBufferReqParameters>::iterator it;
  for (it = m_rlcBufferReq.begin (); it != m_rlcBufferReq.end (); it++)
    {
      if (((*it).m_rnti == rnti) && ((*it).m_logicalChannelIdentity))
        {
//           NS_LOG_DEBUG (this << " UE " << rnti << " LC " << (uint16_t)lcid << " txqueue " << (*it).m_rlcTransmissionQueueSize << " retxqueue " << (*it).m_rlcRetransmissionQueueSize << " status " << (*it).m_rlcStatusPduSize << " decrease " << size);
          // Update queues: RLC tx order Status, ReTx, Tx
          // Update status queue
          if ((*it).m_rlcStatusPduSize <= size)
            {
              size -= (*it).m_rlcStatusPduSize;
              (*it).m_rlcStatusPduSize = 0;
            }
          else
            {
              (*it).m_rlcStatusPduSize -= size;
              return;
            }
          // update retransmission queue  
          if ((*it).m_rlcRetransmissionQueueSize <= size)
            {
              size -= (*it).m_rlcRetransmissionQueueSize;
              (*it).m_rlcRetransmissionQueueSize = 0;
            }
          else
            {
              (*it).m_rlcRetransmissionQueueSize -= size;
              return;
            }
          // update transmission queue
          if ((*it).m_rlcTransmissionQueueSize <= size)
            {
              size -= (*it).m_rlcTransmissionQueueSize;
              (*it).m_rlcTransmissionQueueSize = 0;
            }
          else
            {
              (*it).m_rlcTransmissionQueueSize -= size;
              return;
            }
          return;
        }
    }
}

void
RrFfMacScheduler::UpdateUlRlcBufferInfo (uint16_t rnti, uint16_t size)
{

  size = size - 2; // remove the minimum RLC overhead
  std::map <uint16_t,uint32_t>::iterator it = m_ceBsrRxed.find (rnti);
  if (it!=m_ceBsrRxed.end ())
    {
//       NS_LOG_DEBUG (this << " Update RLC BSR UE " << rnti << " size " << size << " BSR " << (*it).second);      
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
RrFfMacScheduler::TransmissionModeConfigurationUpdate (uint16_t rnti, uint8_t txMode)
{
  NS_LOG_FUNCTION (this << " RNTI " << rnti << " txMode " << (uint16_t)txMode);
  FfMacCschedSapUser::CschedUeConfigUpdateIndParameters params;
  params.m_rnti = rnti;
  params.m_transmissionMode = txMode;
  m_cschedSapUser->CschedUeConfigUpdateInd (params);
}



}
