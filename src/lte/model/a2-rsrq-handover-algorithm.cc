/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 University of Jyvaskyla
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
 * Author: Budiarto Herman <buherman@student.jyu.fi>
 *
 */


#include "a2-rsrq-handover-algorithm.h"
#include <ns3/handover-management-sap.h>
#include <ns3/log.h>
#include <ns3/uinteger.h>

NS_LOG_COMPONENT_DEFINE ("A2RsrqHandoverAlgorithm");


namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (A2RsrqHandoverAlgorithm);


///////////////////////////////////////////
// Handover Management SAP forwarder
///////////////////////////////////////////

/**
 * \brief Class for forwarding Handover Management SAP Provider functions.
 */
class A2RsrqMemberHandoverManagementSapProvider : public HandoverManagementSapProvider
{
public:
  A2RsrqMemberHandoverManagementSapProvider (A2RsrqHandoverAlgorithm* handoverAlgorithm);

  // methods inherited from HandoverManagementSapProvider go here
  virtual void ReportUeMeas (uint16_t rnti, LteRrcSap::MeasResults measResults);

private:
  A2RsrqHandoverAlgorithm* m_handoverAlgorithm;
};

A2RsrqMemberHandoverManagementSapProvider::A2RsrqMemberHandoverManagementSapProvider (A2RsrqHandoverAlgorithm* handoverAlgorithm)
  : m_handoverAlgorithm (handoverAlgorithm)
{
}

void
A2RsrqMemberHandoverManagementSapProvider::ReportUeMeas (uint16_t rnti,
                                                         LteRrcSap::MeasResults measResults)
{
  m_handoverAlgorithm->DoReportUeMeas (rnti, measResults);
}



///////////////////////////////////////////
// A2 Based Handover Algorithm
///////////////////////////////////////////


A2RsrqHandoverAlgorithm::A2RsrqHandoverAlgorithm ()
  : m_handoverManagementSapUser (0)
{
  NS_LOG_FUNCTION (this);
  m_handoverManagementSapProvider = new A2RsrqMemberHandoverManagementSapProvider (this);
}


A2RsrqHandoverAlgorithm::~A2RsrqHandoverAlgorithm ()
{
  NS_LOG_FUNCTION (this);
}


void
A2RsrqHandoverAlgorithm::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_handoverManagementSapProvider;
}


TypeId
A2RsrqHandoverAlgorithm::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::A2RsrqHandoverAlgorithm")
    .SetParent<HandoverAlgorithm> ()
    .AddConstructor<A2RsrqHandoverAlgorithm> ()
    .AddAttribute ("ServingCellThreshold",
                   "If serving cell is worse than this threshold, neighbour cells are consider for Handover",
                   UintegerValue (30),
                   MakeUintegerAccessor (&A2RsrqHandoverAlgorithm::m_servingCellThreshold),
                   MakeUintegerChecker<uint8_t> ())
    .AddAttribute ("NeighbourCellOffset",
                   "Minimum offset between serving and best neighbour cell to trigger the Handover",
                   UintegerValue (1),
                   MakeUintegerAccessor (&A2RsrqHandoverAlgorithm::m_neighbourCellOffset),
                   MakeUintegerChecker<uint8_t> ())
  ;
  return tid;
}


void
A2RsrqHandoverAlgorithm::SetHandoverManagementSapUser (HandoverManagementSapUser* s)
{
  NS_LOG_FUNCTION (this << s);
  m_handoverManagementSapUser = s;
}


HandoverManagementSapProvider*
A2RsrqHandoverAlgorithm::GetHandoverManagementSapProvider ()
{
  NS_LOG_FUNCTION (this);
  return m_handoverManagementSapProvider;
}


void
A2RsrqHandoverAlgorithm::DoInitialize ()
{
  NS_LOG_FUNCTION (this);

  LteRrcSap::ReportConfigEutra reportConfigA2;
  reportConfigA2.eventId = LteRrcSap::ReportConfigEutra::EVENT_A2;
  reportConfigA2.threshold1.choice = LteRrcSap::ThresholdEutra::THRESHOLD_RSRQ;
  reportConfigA2.threshold1.range = m_servingCellThreshold;
  reportConfigA2.triggerQuantity = LteRrcSap::ReportConfigEutra::RSRQ;
  reportConfigA2.reportInterval = LteRrcSap::ReportConfigEutra::MS240;
  m_a2measId = m_handoverManagementSapUser->AddUeMeasReportConfig (reportConfigA2);

  LteRrcSap::ReportConfigEutra reportConfigA4;
  reportConfigA4.eventId = LteRrcSap::ReportConfigEutra::EVENT_A4;
  reportConfigA4.threshold1.choice = LteRrcSap::ThresholdEutra::THRESHOLD_RSRQ;
  reportConfigA4.threshold1.range = 0; // intentionally very low threshold
  reportConfigA4.triggerQuantity = LteRrcSap::ReportConfigEutra::RSRQ;
  reportConfigA4.reportInterval = LteRrcSap::ReportConfigEutra::MS480;
  m_a4measId = m_handoverManagementSapUser->AddUeMeasReportConfig (reportConfigA4);

  HandoverAlgorithm::DoInitialize ();
}


void
A2RsrqHandoverAlgorithm::DoReportUeMeas (uint16_t rnti,
                                         LteRrcSap::MeasResults measResults)
{
  NS_LOG_FUNCTION (this << rnti << (uint16_t) measResults.measId);

  if (measResults.measId == m_a2measId)
    {
      NS_ASSERT_MSG (measResults.rsrqResult <= m_servingCellThreshold,
                     "Invalid UE measurement report");
      EvaluateHandover (rnti, measResults.rsrqResult);
    }
  else if (measResults.measId == m_a4measId)
    {
      if (measResults.haveMeasResultNeighCells
          && !measResults.measResultListEutra.empty ())
        {
          for (std::list <LteRrcSap::MeasResultEutra>::iterator it = measResults.measResultListEutra.begin ();
               it != measResults.measResultListEutra.end ();
               ++it)
            {
              NS_ASSERT_MSG (it->haveRsrqResult == true,
                             "RSRQ measure missing for cellId " << it->physCellId);
              UpdateNeighbourMeasurements (rnti, it->physCellId, it->rsrqResult);
            }
        }
      else
        {
          NS_LOG_LOGIC ("WARNING");
          // NS_ASSERT_MSG ("Event A4 received without measure results for neighbour cells");
          // TODO Remove neighbours in the neighbourCellMeasures table
        }
    }
  else
    {
      NS_LOG_WARN ("Ignoring measId " << (uint16_t) measResults.measId);
    }

} // end of DoReportUeMeas


void
A2RsrqHandoverAlgorithm::EvaluateHandover (uint16_t rnti,
                                           uint8_t servingCellRsrq)
{
  NS_LOG_FUNCTION (this << rnti << (uint16_t) servingCellRsrq);

  MeasurementTable_t::iterator it1;
  it1 = m_neighbourCellMeasures.find (rnti);

  if (it1 == m_neighbourCellMeasures.end ())
    {
      NS_LOG_WARN ("Skipping handover evaluation for RNTI " << rnti << " because neighbour cells information is not found");
    }
  else
    {
      // Find the best neighbour cell (eNB)
      NS_LOG_LOGIC ("Number of neighbour cells = " << it1->second.size ());
      uint16_t bestNeighbourCellId = 0;
      uint8_t bestNeighbourRsrq = 0;
      MeasurementRow_t::iterator it2;
      for (it2 = it1->second.begin (); it2 != it1->second.end (); ++it2)
        {
          if ((it2->second->m_rsrq > bestNeighbourRsrq)
              && IsValidNeighbour (it2->first))
            {
              bestNeighbourCellId = it2->first;
              bestNeighbourRsrq = it2->second->m_rsrq;
            }
        }

      // Trigger Handover, if needed
      if (bestNeighbourCellId > 0)
        {
          NS_LOG_LOGIC ("Best neighbour cellId " << bestNeighbourCellId);

          if ((bestNeighbourRsrq - servingCellRsrq) >= m_neighbourCellOffset)
            {
              NS_LOG_LOGIC ("Trigger Handover to cellId " << bestNeighbourCellId);
              NS_LOG_LOGIC ("target cell RSRQ " << (uint16_t) bestNeighbourRsrq);
              NS_LOG_LOGIC ("serving cell RSRQ " << (uint16_t) servingCellRsrq);

              // Inform eNodeB RRC about handover
              m_handoverManagementSapUser->TriggerHandover (rnti,
                                                            bestNeighbourCellId);
            }
        }

    } // end of else of if (it1 == m_neighbourCellMeasures.end ())

} // end of EvaluateMeasurementReport


bool
A2RsrqHandoverAlgorithm::IsValidNeighbour (uint16_t cellId)
{
  NS_LOG_FUNCTION (this << cellId);

  // Ptr<NeighbourRelation> neighbourRelation = m_rrc->m_neighbourRelationTable[it->second->m_cellId];
  // if ((neighbourRelation->m_noHo == false) &&
  //     (neighbourRelation->m_noX2 == false))
  //   {
  //     bestNeighbour = it->second;
  //     bestNeighbourRsrq = it->second->m_rsrq;
  //   }
  // TODO

  return true;
}


void
A2RsrqHandoverAlgorithm::UpdateNeighbourMeasurements (uint16_t rnti,
                                                      uint16_t cellId,
                                                      uint8_t rsrq)
{
  NS_LOG_FUNCTION (this << rnti << cellId << (uint16_t) rsrq);
  MeasurementTable_t::iterator it1;
  it1 = m_neighbourCellMeasures.find (rnti);

  if (it1 == m_neighbourCellMeasures.end ())
    {
      // insert a new UE entry
      MeasurementRow_t row;
      std::pair<MeasurementTable_t::iterator, bool> ret;
      ret = m_neighbourCellMeasures.insert (std::pair<uint16_t, MeasurementRow_t> (rnti, row));
      NS_ASSERT (ret.second);
      it1 = ret.first;
    }

  NS_ASSERT (it1 != m_neighbourCellMeasures.end ());
  Ptr<UeMeasure> neighbourCellMeasures;
  std::map<uint16_t, Ptr<UeMeasure> >::iterator it2;
  it2 = it1->second.find (cellId);

  if (it2 != it1->second.end ())
    {
      neighbourCellMeasures = it2->second;
      neighbourCellMeasures->m_cellId = cellId;
      neighbourCellMeasures->m_rsrp = 0;
      neighbourCellMeasures->m_rsrq = rsrq;
    }
  else
    {
      // insert a new cell entry
      neighbourCellMeasures = CreateObject<UeMeasure> ();
      neighbourCellMeasures->m_cellId = cellId;
      neighbourCellMeasures->m_rsrp = 0;
      neighbourCellMeasures->m_rsrq = rsrq;
      it1->second[cellId] = neighbourCellMeasures;
    }

} // end of UpdateNeighbourMeasurements


} // end of namespace ns3
