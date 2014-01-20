/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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

#ifndef LTE_TEST_RLC_UM_E2E_H
#define LTE_TEST_RLC_UM_E2E_H

#include "ns3/test.h"


using namespace ns3;


/**
 * Test x.x.x RLC UM: End-to-end flow
 */
class LteRlcUmE2eTestSuite : public TestSuite
{
public:
  LteRlcUmE2eTestSuite ();
};


class LteRlcUmE2eTestCase : public TestCase
{
  public:
    LteRlcUmE2eTestCase (std::string name, uint32_t seed, double losses);
    LteRlcUmE2eTestCase ();
    virtual ~LteRlcUmE2eTestCase ();

  private:
    virtual void DoRun (void);

    void DlDropEvent (Ptr<const Packet> p);
    void UlDropEvent (Ptr<const Packet> p);

    uint32_t m_dlDrops;
    uint32_t m_ulDrops;

    uint32_t m_seed;
    double   m_losses;
};

#endif // LTE_TEST_RLC_UM_E2E_H
