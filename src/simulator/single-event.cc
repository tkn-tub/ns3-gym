/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INESC Porto
 * All rights reserved.
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
 * Author: Gustavo J. A. M. Carneiro  <gjc@inescporto.pt>
 */

#ifdef RUN_SELF_TESTS

#include "single-event.h"
#include "ns3/test.h"

namespace ns3 {

class SingleEventTests : public Test
{
  int m_counter;
  SingleEvent *m_events;

  void SingleEventCallback ();

public:

  SingleEventTests ();
  virtual ~SingleEventTests ();
  virtual bool RunTests (void);
};

SingleEventTests::SingleEventTests ()
  : Test ("SingleEvent"), m_counter (0), m_events (0)
{}

SingleEventTests::~SingleEventTests ()
{}

void
SingleEventTests::SingleEventCallback ()
{
  m_counter++;
}

bool SingleEventTests::RunTests (void)
{
  bool result = true;


  {
    SingleEvent event;

    event = Simulator::Schedule (Simulator::Now (), &SingleEventTests::SingleEventCallback, this);
    Simulator::Run ();
    NS_TEST_ASSERT_EQUAL (m_counter, 1);
  }

  {
    SingleEvent event;

    event = Simulator::Schedule (Simulator::Now (), &SingleEventTests::SingleEventCallback, this);
  } // event is destroyed => the eventid cancelled

  Simulator::Run ();
  NS_TEST_ASSERT_EQUAL (m_counter, 1);

  {
    SingleEvent event;

    event = Simulator::Schedule (Simulator::Now (), &SingleEventTests::SingleEventCallback, this);
    // the second event cancels the first one
    event = Simulator::Schedule (Simulator::Now (), &SingleEventTests::SingleEventCallback, this);
    Simulator::Run ();
    NS_TEST_ASSERT_EQUAL (m_counter, 2);
  }

  return result;
}

static SingleEventTests g_singleEventTests;
    
};

#endif /* RUN_SELF_TESTS */
