/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 IITP RAS
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
 * Authors: Pavel Boyko <boyko@iitp.ru>
 */

#ifndef LOOPBACK_H_
#define LOOPBACK_H_
#include "ns3/test.h"
#include <string>

namespace ns3
{
namespace aodv
{
/**
 * \ingroup aodv
 * 
 * \brief AODV ping 127.0.0.1 test case
 */
struct LoopbackTestCase : public TestCase
{
  LoopbackTestCase () : TestCase ("Ping 127.0.0.1 test") {}
  /// Run test
  void DoRun ();
};

}
}

#endif /* LOOPBACK_H_ */
