/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Mathieu Lacage
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
 */

#include "rng-seed-manager.h"
#include "global-value.h"
#include "attribute-helper.h"
#include "integer.h"
#include "config.h"
#include "log.h"

/**
 * \file
 * \ingroup randomvariable
 * ns3::RngSeedManager implementation.
 */

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RngSeedManager");

/**
 * \relates RngSeedManager
 * The next random number generator stream number to use
 * for automatic assignment.
 */
static uint64_t g_nextStreamIndex = 0;
/**
 * \relates RngSeedManager
 * The random number generator seed number global value.
 *
 * This is accessible as "--RngSeed" from CommandLine.
 */
static ns3::GlobalValue g_rngSeed ("RngSeed", 
                                   "The global seed of all rng streams",
                                   ns3::IntegerValue(1),
                                   ns3::MakeIntegerChecker<uint32_t> ());
/**
 * \relates RngSeedManager
 * The random number generator run number global value.
 *
 * This is accessible as "--RngRun" from CommandLine.
 */
static ns3::GlobalValue g_rngRun ("RngRun", 
                                  "The run number used to modify the global seed",
                                  ns3::IntegerValue (1),
                                  ns3::MakeIntegerChecker<int64_t> ());


uint32_t RngSeedManager::GetSeed (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  IntegerValue seedValue;
  g_rngSeed.GetValue (seedValue);
  return seedValue.Get ();
}
void 
RngSeedManager::SetSeed (uint32_t seed)
{
  NS_LOG_FUNCTION (seed);
  Config::SetGlobal ("RngSeed", IntegerValue(seed));
}

void RngSeedManager::SetRun (uint64_t run)
{
  NS_LOG_FUNCTION (run);
  Config::SetGlobal ("RngRun", IntegerValue (run));
}

uint64_t RngSeedManager::GetRun ()
{
  NS_LOG_FUNCTION_NOARGS ();
  IntegerValue value;
  g_rngRun.GetValue (value);
  int run = value.Get();
  return run;
}

uint64_t RngSeedManager::GetNextStreamIndex (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  uint64_t next = g_nextStreamIndex;
  g_nextStreamIndex++;
  return next;
}

} // namespace ns3
