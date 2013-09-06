/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Budiarto Herman
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
 * Author: Budiarto Herman <budiarto.herman@magister.fi>
 *
 */

#ifndef A2_A4_RSRQ_HANDOVER_ALGORITHM_H
#define A2_A4_RSRQ_HANDOVER_ALGORITHM_H

#include <ns3/lte-handover-algorithm.h>
#include <ns3/lte-handover-management-sap.h>
#include <ns3/lte-rrc-sap.h>
#include <map>

namespace ns3 {


/**
 * \brief Implements the Handover Management SAP for a handover algorithm based
 *        on RSRQ, Event A2 and Event A4.
 *
 * Handover decision is primarily based on Event A2 measurements (serving cell's
 * RSRQ becomes worse than threshold). The threshold used can be configured in
 * the `ServingCellThreshold` attribute. When the event is triggered, the first
 * condition of handover is fulfilled.
 *
 * Event A4 measurements (neighbour cell's RSRQ becomes better than threshold)
 * are used to detect neighbouring cells and their respective RSRQ. When a
 * neighbouring cell's RSRQ is higher than the serving cell's RSRQ by a certain
 * offset (configurable in the `NeighbourCellOffset` attribute), then the second
 * condition of handover is fulfilled.
 *
 * When the first and second conditions above are fulfilled, the algorithm
 * triggers a handover.
 *
 * Note that the attributes must be set before the handover algorithm object is
 * instantiated in order for them to take effect, i.e. before calling
 * LteHelper::InstallSingleEnbDevice. Subsequent changes to the attribute values
 * after that will not have any effect.
 */
class A2A4RsrqHandoverAlgorithm : public LteHandoverAlgorithm
{
public:
  A2A4RsrqHandoverAlgorithm ();
  virtual ~A2A4RsrqHandoverAlgorithm ();

  // inherited from Object
  virtual void DoDispose (void);
  static TypeId GetTypeId (void);

  // inherited from HandoverAlgorithm
  virtual void SetLteHandoverManagementSapUser (LteHandoverManagementSapUser* s);
  virtual LteHandoverManagementSapProvider* GetLteHandoverManagementSapProvider ();

  friend class MemberLteHandoverManagementSapProvider<A2A4RsrqHandoverAlgorithm>;

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

}; // end of class A2A4RsrqHandoverAlgorithm


} // end of namespace ns3


#endif /* A2_A4_RSRQ_HANDOVER_ALGORITHM_H */
