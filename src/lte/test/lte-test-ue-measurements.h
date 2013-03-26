/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 *         Nicola Baldo <nbaldo@cttc.es>
 *         Marco Miozzo <mmiozzo@cttc.es> 
 *              adapt lte-test-interference.cc to lte-ue-measurements.cc
 */

#ifndef LTE_TEST_UE_MEASUREMENTS_H
#define LTE_TEST_UE_MEASUREMENTS_H

#include <ns3/test.h>
#include <ns3/lte-rrc-sap.h>


namespace ns3 {


/**
 * Test that UE Measurements (see 36.214) calculation works fine in a 
 * multi-cell interference scenario.
 */
class LteUeMeasurementsTestSuite : public TestSuite
{
public:
  LteUeMeasurementsTestSuite ();
};


class LteUeMeasurementsTestCase : public TestCase
{
public:
  LteUeMeasurementsTestCase (std::string name, double d1, double d2, double rsrpDbmUe1, double rsrpDbmUe2, double rsrqDbUe1, double rsrqDbUe2);
  virtual ~LteUeMeasurementsTestCase ();

  void ReportUeMeasurements (uint16_t rnti, uint16_t cellId, double rsrp, double rsrq, bool servingCell);

  void RecvMeasurementReport (uint64_t imsi, uint16_t cellId, uint16_t rnti, LteRrcSap::MeasurementReport meas);

private:
  virtual void DoRun (void);


  double m_d1;
  double m_d2;
  double m_rsrpDbmUeServingCell;
  double m_rsrpDbmUeNeighborCell;
  double m_rsrqDbUeServingCell;
  double m_rsrqDbUeNeighborCell;
  
};


} // namespace ns3


#endif /* LTE_TEST_UE_MEASUREMENTS_H */
