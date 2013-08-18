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

#ifndef A3_RSRP_HANDOVER_ALGORITHM_H
#define A3_RSRP_HANDOVER_ALGORITHM_H

#include <ns3/lte-handover-algorithm.h>
#include <ns3/lte-handover-management-sap.h>
#include <ns3/lte-rrc-sap.h>
#include <ns3/nstime.h>

namespace ns3 {


/**
 * \brief A handover algorithm based on RSRP and Event A3.
 *
 * The algorithm utilizes Event A3 (Section 5.5.4.4 of 3GPP TS 36.331) UE
 * measurements and the Reference Signal Reference Power (RSRP). It is defined
 * as the event when the UE perceives that a neighbour cell's RSRP is better
 * than the serving cell's RSRP.
 *
 * Handover margin (a.k.a. hysteresis) and time-to-trigger (TTT) can be
 * configured to delay the event triggering. The values of these parameters
 * apply to all attached UEs. Note that these attributes must be set before the
 * handover algorithm object is instantiated in order for them to take effect,
 * i.e. before calling LteHelper::InstallSingleEnbDevice. Subsequent changes to
 * the attribute values after that will not have any effect.
 */
class A3RsrpHandoverAlgorithm : public LteHandoverAlgorithm
{
public:
  A3RsrpHandoverAlgorithm ();
  virtual ~A3RsrpHandoverAlgorithm ();

  // inherited from Object
  virtual void DoDispose (void);
  static TypeId GetTypeId (void);

  // inherited from LteHandoverAlgorithm
  virtual void SetLteHandoverManagementSapUser (LteHandoverManagementSapUser* s);
  virtual LteHandoverManagementSapProvider* GetLteHandoverManagementSapProvider ();

  friend class MemberLteHandoverManagementSapProvider<A3RsrpHandoverAlgorithm>;

protected:
  // inherited from Object
  virtual void DoInitialize ();

private:

  // Handover Management SAP implementation
  void DoReportUeMeas (uint16_t rnti, LteRrcSap::MeasResults measResults);

  // Internal method
  bool IsValidNeighbour (uint16_t cellId);

  // The expected measurement identity
  uint8_t m_measId;

  // Class attributes
  double m_hysteresisDb;
  Time m_timeToTrigger;

  // Handover Management SAPs
  LteHandoverManagementSapUser* m_handoverManagementSapUser;
  LteHandoverManagementSapProvider* m_handoverManagementSapProvider;

}; // end of class A3RsrpHandoverAlgorithm


} // end of namespace ns3


#endif /* A3_RSRP_HANDOVER_ALGORITHM_H */
