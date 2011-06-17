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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */



#include "ns3/test.h"
#include "ns3/log.h"

#include "ns3/lte-spectrum-value-helper.h"

NS_LOG_COMPONENT_DEFINE ("LteTestEarfcn");

namespace ns3 {

class LteEarfcnTestCase : public TestCase
{
public:
  LteEarfcnTestCase (const char* str, uint16_t earfcn, double f);
  virtual ~LteEarfcnTestCase ();

protected:
  uint16_t m_earfcn;
  double m_f;

private:
  virtual void DoRun (void);
};

LteEarfcnTestCase::LteEarfcnTestCase (const char* str, uint16_t earfcn, double f)
  :   TestCase (str),
    m_earfcn (earfcn),
    m_f (f)
{
  NS_LOG_FUNCTION (this << str << earfcn << f);
}

LteEarfcnTestCase::~LteEarfcnTestCase ()
{
}

void 
LteEarfcnTestCase::DoRun (void)
{
  double f = LteSpectrumValueHelper::GetCarrierFrequency (m_earfcn);
  NS_TEST_ASSERT_MSG_EQ_TOL (f, m_f, 0.0000001, "wrong frequency");
}

class LteEarfcnDlTestCase : public LteEarfcnTestCase
{
public:
  LteEarfcnDlTestCase (const char* str, uint16_t earfcn, double f);

private:
  virtual void DoRun (void);
};

LteEarfcnDlTestCase::LteEarfcnDlTestCase (const char* str, uint16_t earfcn, double f)
  : LteEarfcnTestCase (str, earfcn, f)
{
}

void 
LteEarfcnDlTestCase::DoRun (void)
{
//   LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);
//   LogComponentEnable ("LteSpectrumValueHelper", logLevel);
//   LogComponentEnable ("LteTestEarfcn", logLevel);

  double f = LteSpectrumValueHelper::GetDownlinkCarrierFrequency (m_earfcn);
  NS_TEST_ASSERT_MSG_EQ_TOL (f, m_f, 0.0000001, "wrong frequency");
}


class LteEarfcnUlTestCase : public LteEarfcnTestCase
{
public:
  LteEarfcnUlTestCase (const char* str, uint16_t earfcn, double f);

private:
  virtual void DoRun (void);
};

LteEarfcnUlTestCase::LteEarfcnUlTestCase (const char* str, uint16_t earfcn, double f)
  : LteEarfcnTestCase (str, earfcn, f)
{
}

void 
LteEarfcnUlTestCase::DoRun (void)
{
  double f = LteSpectrumValueHelper::GetUplinkCarrierFrequency (m_earfcn);
  NS_TEST_ASSERT_MSG_EQ_TOL (f, m_f, 0.0000001, "wrong frequency");
}


/**
 * Test the calculation of carrier frequency based on EARFCN
 */
class LteEarfcnTestSuite : public TestSuite
{
public:
  LteEarfcnTestSuite ();
};

static LteEarfcnTestSuite g_lteEarfcnTestSuite;

LteEarfcnTestSuite::LteEarfcnTestSuite ()
  : TestSuite ("lte-earfcn", UNIT)
{
  NS_LOG_FUNCTION (this);

  AddTestCase (new LteEarfcnDlTestCase ("DL EARFCN=500", 500, 2160e6));
  AddTestCase (new LteEarfcnDlTestCase ("DL EARFCN=1000", 1000, 1970e6));
  AddTestCase (new LteEarfcnDlTestCase ("DL EARFCN=1301", 1301, 1815.1e6));
  AddTestCase (new LteEarfcnDlTestCase ("DL EARFCN=7000", 7000, 0.0));
  AddTestCase (new LteEarfcnDlTestCase ("DL EARFCN=20000", 20000, 0.0));
  AddTestCase (new LteEarfcnDlTestCase ("DL EARFCN=50000", 50000, 0.0));

  AddTestCase (new LteEarfcnUlTestCase ("UL EARFCN=18100", 18100, 1930e6));
  AddTestCase (new LteEarfcnUlTestCase ("UL EARFCN=19000", 19000, 1890e6));
  AddTestCase (new LteEarfcnUlTestCase ("UL EARFCN=19400", 19400, 1730e6));
  AddTestCase (new LteEarfcnUlTestCase ("UL EARFCN=10", 10, 0.0));
  AddTestCase (new LteEarfcnUlTestCase ("UL EARFCN=1000", 1000, 0.0));
  AddTestCase (new LteEarfcnUlTestCase ("UL EARFCN=50000", 50000, 0.0));

  AddTestCase (new LteEarfcnTestCase ("EARFCN=500", 500, 2160e6));
  AddTestCase (new LteEarfcnTestCase ("EARFCN=1000", 1000, 1970e6));
  AddTestCase (new LteEarfcnTestCase ("EARFCN=1301", 1301, 1815.1e6));
  AddTestCase (new LteEarfcnTestCase ("EARFCN=8000", 8000, 0.0));
  AddTestCase (new LteEarfcnTestCase ("EARFCN=50000", 50000, 0.0));
  AddTestCase (new LteEarfcnTestCase ("EARFCN=18100", 18100, 1930e6));
  AddTestCase (new LteEarfcnTestCase ("EARFCN=19000", 19000, 1890e6));
  AddTestCase (new LteEarfcnTestCase ("EARFCN=19400", 19400, 1730e6));
  AddTestCase (new LteEarfcnTestCase ("EARFCN=50000", 50000, 0.0));
}


} // namespace ns3
