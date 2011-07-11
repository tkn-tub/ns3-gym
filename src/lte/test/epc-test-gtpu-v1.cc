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
 * Author: Jaume Nin <jaume.nin@cttc.cat>
 */


#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/packet.h"

#include "ns3/epc-gtpu-v1.h"
#include "ns3/epc-test-gtpu-v1.h"

NS_LOG_COMPONENT_DEFINE ("EpcGtpuTest");

using namespace ns3;


/**
 * TestSuite
 */

EpsGtpuTestSuite::EpsGtpuTestSuite ()
  : TestSuite ("epc-gtpu-v1", SYSTEM)
{
  AddTestCase (new EpsGtpuHeaderTestCase ());
}

static EpsGtpuTestSuite epsGtpuTestSuite;


/**
 * TestCase
 */

EpsGtpuHeaderTestCase::EpsGtpuHeaderTestCase ()
  : TestCase ("Check header coding and decoding")
{
  NS_LOG_INFO ("Creating EpsGtpuHeaderTestCase");
}

EpsGtpuHeaderTestCase::~EpsGtpuHeaderTestCase ()
{
}

void
EpsGtpuHeaderTestCase::DoRun (void)
{
  LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);

  LogComponentEnable ("EpcGtpuTest", logLevel);
  GtpuHeader header1;
  header1.SetExtensionHeaderFlag (true);
  header1.SetLength (1234);
  header1.SetMessageType (123);
  header1.SetNPduNumber (123);
  header1.SetNPduNumberFlag (true);
  header1.SetNextExtensionType (123);
  header1.SetProtocolType (true);
  header1.SetSequenceNumber (1234);
  header1.SetSequenceNumberFlag (true);
  header1.SetTeid (1234567);
  header1.SetVersion (123);

  Packet p;
  GtpuHeader header2;
  p.AddHeader (header1);
  p.RemoveHeader (header2);

  NS_TEST_ASSERT_MSG_EQ (header1, header2, "Wrong value!");
}
