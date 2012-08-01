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
 */

#ifndef LTE_TEST_RLC_UM_TRANSMITTER_H
#define LTE_TEST_RLC_UM_TRANSMITTER_H

#include "ns3/test.h"


namespace ns3 {

class LteTestRrc;
class LteTestMac;
class LteTestPdcp;

/**
 * TestSuite 4.1.1 RLC UM: Only transmitter
 */
class LteRlcUmTransmitterTestSuite : public TestSuite
{
  public:
    LteRlcUmTransmitterTestSuite ();
};

class LteRlcUmTransmitterTestCase : public TestCase
{
  public:
    LteRlcUmTransmitterTestCase (std::string name);
    LteRlcUmTransmitterTestCase ();
    virtual ~LteRlcUmTransmitterTestCase ();

    void CheckDataReceived (Time time, std::string shouldReceived, std::string assertMsg);

  protected:
    virtual void DoRun (void);

    Ptr<LteTestPdcp> txPdcp;
    Ptr<LteRlc> txRlc;
    Ptr<LteTestMac> txMac;

  private:
    void DoCheckDataReceived (std::string shouldReceived, std::string assertMsg);

};

/**
 * Test 4.1.1.1 One SDU, One PDU
 */
class LteRlcUmTransmitterOneSduTestCase : public LteRlcUmTransmitterTestCase
{
  public:
    LteRlcUmTransmitterOneSduTestCase (std::string name);
    LteRlcUmTransmitterOneSduTestCase ();
    virtual ~LteRlcUmTransmitterOneSduTestCase ();

  private:
    virtual void DoRun (void);

};

/**
 * Test 4.1.1.2 Segmentation (One SDU => n PDUs)
 */
class LteRlcUmTransmitterSegmentationTestCase : public LteRlcUmTransmitterTestCase
{
  public:
    LteRlcUmTransmitterSegmentationTestCase (std::string name);
    LteRlcUmTransmitterSegmentationTestCase ();
    virtual ~LteRlcUmTransmitterSegmentationTestCase ();

  private:
    virtual void DoRun (void);

};

/**
 * Test 4.1.1.3 Concatenation (n SDUs => One PDU)
 */
class LteRlcUmTransmitterConcatenationTestCase : public LteRlcUmTransmitterTestCase
{
  public:
    LteRlcUmTransmitterConcatenationTestCase (std::string name);
    LteRlcUmTransmitterConcatenationTestCase ();
    virtual ~LteRlcUmTransmitterConcatenationTestCase ();

  private:
    virtual void DoRun (void);

};

/**
 * Test 4.1.1.4 Report Buffer Status (test primitive parameters)
 */
class LteRlcUmTransmitterReportBufferStatusTestCase : public LteRlcUmTransmitterTestCase
{
  public:
    LteRlcUmTransmitterReportBufferStatusTestCase (std::string name);
    LteRlcUmTransmitterReportBufferStatusTestCase ();
    virtual ~LteRlcUmTransmitterReportBufferStatusTestCase ();

  private:
    virtual void DoRun (void);

};

} // namespace ns3

#endif /* LTE_TEST_RLC_UM_TRANSMITTER_H */
