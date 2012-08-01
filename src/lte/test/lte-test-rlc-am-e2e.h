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

#ifndef LTE_TEST_RLC_AM_E2E_H
#define LTE_TEST_RLC_AM_E2E_H

#include "ns3/test.h"


namespace ns3 {


/**
 * Test x.x.x RLC AM: End-to-end flow
 */
class LteRlcAmE2eTestSuite : public TestSuite
{
public:
  LteRlcAmE2eTestSuite ();
};


class LteRlcAmE2eTestCase : public TestCase
{
  public:
    LteRlcAmE2eTestCase (std::string name, uint32_t seed, double losses);
    LteRlcAmE2eTestCase ();
    virtual ~LteRlcAmE2eTestCase ();

  private:
    virtual void DoRun (void);

    void DlDropEvent (Ptr<const Packet> p);
    void UlDropEvent (Ptr<const Packet> p);

    uint32_t m_dlDrops;
    uint32_t m_ulDrops;

    uint32_t m_seed;
    double   m_losses;
};


} // namespace ns3


#endif // LTE_TEST_RLC_AM_E2E_H
