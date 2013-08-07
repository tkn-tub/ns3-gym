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

#ifndef HANDOVER_MANAGEMENT_SAP_H
#define HANDOVER_MANAGEMENT_SAP_H

#include <ns3/lte-rrc-sap.h>

namespace ns3 {


/**
 * \brief Service Access Point (SAP) used by the eNodeB RRC instance to send
 *        messages to the handover algorithm instance.
 */
class HandoverManagementSapProvider
{
public:
  virtual ~HandoverManagementSapProvider ();

  /**
   * \brief Send a UE measurement report to handover algorithm.
   * \param rnti Radio Network Temporary Identity, an integer identifying the UE
   *             where the report originates from
   * \param measResults a single report of one measurement identity
   *
   * The received measurement report may be stored and utilized for the purpose
   * of making handover decision.
   */
  virtual void ReportUeMeas (uint16_t rnti,
                             LteRrcSap::MeasResults measResults) = 0;

}; // end of class HandoverManagementSapProvider


/**
 * \brief Service Access Point (SAP) used by the handover algorithm instance to
 *        send messages to the eNodeB RRC instance.
 */
class HandoverManagementSapUser
{
public:
  virtual ~HandoverManagementSapUser ();

  /**
   * \brief Request a certain reporting configuration to be fulfilled by the UEs
   *        attached to the eNodeB entity.
   * \param reportConfig the UE measurement reporting configuration
   * \return the measurement identity associated with this newly added
   *         reporting configuration
   *
   * The eNodeB RRC entity is expected to configure the same reporting
   * configuration in each of the attached UEs. When later in the simulation a
   * UE measurement report is received from a UE as a result of this
   * configuration, the eNodeB RRC entity shall forward this report to the
   * handover algorithm through the HandoverManagementSapProvider::ReportUeMeas
   * SAP function.
   *
   * This function is only valid before the simulation begins.
   */
  virtual uint8_t AddUeMeasReportConfig (LteRrcSap::ReportConfigEutra reportConfig) = 0;

  /**
   * \brief Instruct the eNodeB RRC entity to prepare a handover.
   * \param rnti Radio Network Temporary Identity, an integer identifying the
   *             UE which shall perform the handover
   * \param targetCellId the cell ID of the target eNodeB
   *
   * This function is used by the handover algorithm entity when a handover
   * decision has been reached.
   *
   * The process to produce the decision is up to the implementation of handover
   * algorithm. It is typically based on the reported UE measurements, which are
   * received through the HandoverManagementSapProvider::ReportUeMeas function.
   */
  virtual void TriggerHandover (uint16_t rnti, uint16_t targetCellId) = 0;

}; // end of class HandoverManagementSapUser


} // end of namespace ns3


#endif /* HANDOVER_MANAGEMENT_SAP_H */
