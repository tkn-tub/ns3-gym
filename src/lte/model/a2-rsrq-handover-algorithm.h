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

#ifndef A2_RSRQ_HANDOVER_ALGORITHM_H
#define A2_RSRQ_HANDOVER_ALGORITHM_H

#include <ns3/lte-handover-algorithm.h>
#include <ns3/lte-handover-management-sap.h>
#include <ns3/lte-rrc-sap.h>
#include <map>

namespace ns3 {


/**
 * \brief Implements the Handover Management SAP for a handover algorithm based
 *        on RSRQ and Event A2 (serving cell's RSRQ becomes worse than a
 *        threshold).
 *
 * The threshold used by the algorithm can be configured in the
 * `ServingCellThreshold` attribute.
 */
class A2RsrqHandoverAlgorithm : public LteHandoverAlgorithm
{
public:
  A2RsrqHandoverAlgorithm ();
  virtual ~A2RsrqHandoverAlgorithm ();

  // inherited from Object
  virtual void DoDispose (void);
  static TypeId GetTypeId (void);

  // inherited from HandoverAlgorithm
  virtual void SetLteHandoverManagementSapUser (LteHandoverManagementSapUser* s);
  virtual LteHandoverManagementSapProvider* GetLteHandoverManagementSapProvider ();

  friend class MemberLteHandoverManagementSapProvider<A2RsrqHandoverAlgorithm>;

protected:
  // inherited from Object
  virtual void DoInitialize ();

private:

  // Handover Management SAP implementation
  void DoReportUeMeas (uint16_t rnti, LteRrcSap::MeasResults measResults);

  // Internal methods
  void EvaluateHandover (uint16_t rnti, uint8_t servingCellRsrq);
  bool IsValidNeighbour (uint16_t cellId);
  void UpdateNeighbourMeasurements (uint16_t rnti, uint16_t cellId,
                                    uint8_t rsrq);

  // The expected measurement identities
  uint8_t m_a2MeasId;
  uint8_t m_a4MeasId;

  /**
   * \brief Measurements reported by a UE for a cell ID.
   *
   * The values are quantized according 3GPP TS 36.133 section 9.1.4 and 9.1.7.
   */
  class UeMeasure : public Object
  {
  public:
    uint16_t m_cellId;
    uint8_t m_rsrp;
    uint8_t m_rsrq;
  };
  // TODO instead of class, try using struct or SimpleRefCount

  //               cellId
  typedef std::map<uint16_t, Ptr<UeMeasure> > MeasurementRow_t;
  //               rnti
  typedef std::map<uint16_t, MeasurementRow_t> MeasurementTable_t;
  MeasurementTable_t m_neighbourCellMeasures;

  // Class attributes
  uint8_t m_servingCellThreshold;
  uint8_t m_neighbourCellOffset;

  // Handover Management SAPs
  LteHandoverManagementSapUser* m_handoverManagementSapUser;
  LteHandoverManagementSapProvider* m_handoverManagementSapProvider;

}; // end of class A2RsrqHandoverAlgorithm


} // end of namespace ns3


#endif /* A2_RSRQ_HANDOVER_ALGORITHM_H */
