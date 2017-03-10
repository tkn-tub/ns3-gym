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
* Author: Marco Miozzo <marco.miozzo@cttc.es>
*/

#ifndef LTE_TEST_FADING_H
#define LTE_TEST_FADING_H

#include "ns3/spectrum-value.h"

#include "ns3/test.h"

#include <ns3/buildings-mobility-model.h>
#include <ns3/buildings-propagation-loss-model.h>
#include <ns3/spectrum-value.h>

#include <ns3/trace-fading-loss-model.h>

using namespace ns3;


/**
 * \ingroup lte-test
 * \ingroup tests
 *
 * \brief Test 1.1 fading model
 */

class LteFadingTestSuite : public TestSuite
{
  public:
    LteFadingTestSuite ();
};


/**
 * \ingroup lte-test
 * \ingroup tests
 *
 * \brief Fading test case is checking if the pathloss between macro and UE 
 * is equal to the theoretical value when using the Okumura Hata Model 
 * (150 < freq < 1500 MHz).
 */
class LteFadingTestCase : public TestCase
{
  public:
    /**
     * Lte Fading Test Case function
     * \param m1 building mobility model #1
     * \param m2 building modility model #2
     * \param refValue reference value
     * \param name the reference name
     */
    LteFadingTestCase (Ptr<BuildingsMobilityModel> m1, Ptr<BuildingsMobilityModel> m2, double refValue, std::string name);
    virtual ~LteFadingTestCase ();
    
  private:
    virtual void DoRun (void);
    
    void GetFadingSample ();
    
    Ptr<BuildingsMobilityModel> m_node1; ///< building mobility model #1
    Ptr<BuildingsMobilityModel> m_node2; ///< building mobility model #2
    Ptr<TraceFadingLossModel> m_fadingModule; ///< fading loss model
    double m_lossRef; ///< loss reference
    std::vector<SpectrumValue> m_fadingSamples; ///< fading samples
     
    
};

/**
 * \ingroup lte-test
 * \ingroup tests
 *
 * \brief Lte Fading System Test Case
 */
class LteFadingSystemTestCase : public TestCase
{
  public:
    /**
     * Lte Fading System Test Case function
     * \param name the reference name
     * \param snrDb SNR in DB
     * \param dist the distance
     * \param mcsIndex the MCS index
     */
    LteFadingSystemTestCase (std::string name, double snrDb, double dist, uint16_t mcsIndex);
    LteFadingSystemTestCase ();
    virtual ~LteFadingSystemTestCase ();
    
    /**
     * DL scheduling function
     * \param dlInfo DL scheduling info
     */
    void DlScheduling (DlSchedulingCallbackInfo dlInfo);
                       
  private:
    virtual void DoRun (void);
    
    double m_snrDb; ///< SNR in DB
    double m_distance; ///< distance
    uint16_t m_mcsIndex; ///< MCS index
};

#endif /*LTE_TEST_FADING_H*/
