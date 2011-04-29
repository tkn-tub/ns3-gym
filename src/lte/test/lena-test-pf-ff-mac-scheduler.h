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

#ifndef LENA_TEST_PF_FF_MAC_SCHEDULER_H
#define LENA_TEST_PF_FF_MAC_SCHEDULER_H

#include "ns3/simulator.h"
#include "ns3/test.h"


using namespace ns3;


/**
 * Test 1.5 Proportional Fair MAC Scheduler
 */
class LenaPfFfMacSchedulerTestCase : public TestCase
{
  public:
    LenaPfFfMacSchedulerTestCase ();
    virtual ~LenaPfFfMacSchedulerTestCase ();

  private:
    virtual void DoRun (void);
};



class LenaTest1_5Suite : public TestSuite
{
  public:
    LenaTest1_5Suite ();
};




#endif /* LENA_TEST_RR_FF_MAC_SCHEDULER_H */
