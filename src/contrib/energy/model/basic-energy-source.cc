/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Network Security Lab, University of Washington, Seattle.
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
 * Authors: Sidharth Nabar <snabar@uw.edu>, He Wu <mdzz@u.washington.edu>
 */

#include "basic-energy-source.h"
#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/double.h"
#include "ns3/trace-source-accessor.h"

NS_LOG_COMPONENT_DEFINE ("BasicEnergySource");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (BasicEnergySource);

TypeId
BasicEnergySource::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BasicEnergySource")
    .SetParent<EnergySource> ()
    .AddConstructor<BasicEnergySource> ()
    .AddAttribute ("BasicEnergySourceInitialEnergyJ",
                   "Initial energy stored in basic energy source.",
                   DoubleValue (10),  // in Joules
                   MakeDoubleAccessor (&BasicEnergySource::SetInitialEnergy,
                                       &BasicEnergySource::DoGetInitialEnergy),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("RemainingEnergy",
                     "Remaining energy at BasicEnergySource.",
                     MakeTraceSourceAccessor (&BasicEnergySource::m_remainingEnergyJ))
  ;
  return tid;
}

BasicEnergySource::BasicEnergySource ()
{
}

BasicEnergySource::~BasicEnergySource ()
{
}

void
BasicEnergySource::SetInitialEnergy (double initialEnergyJ)
{
  NS_LOG_FUNCTION (this << initialEnergyJ);
  NS_ASSERT (initialEnergyJ >= 0);
  m_initialEnergyJ = initialEnergyJ;
  // set remaining energy to be initial energy
  m_remainingEnergyJ = m_initialEnergyJ;
}

/*
 * Private functions start here.
 */

void
BasicEnergySource::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  BreakDeviceEnergyModelRefCycle ();  // break reference cycle
}

double
BasicEnergySource::DoGetInitialEnergy (void) const
{
  NS_LOG_FUNCTION (this);
  return m_initialEnergyJ;
}

double
BasicEnergySource::DoGetRemainingEnergy (void) const
{
  NS_LOG_FUNCTION (this);
  return m_remainingEnergyJ;
}

void
BasicEnergySource::DoDecreaseRemainingEnergy (double energyJ)
{
  NS_LOG_FUNCTION (this << energyJ);
  NS_ASSERT (energyJ >= 0);
  m_remainingEnergyJ -= energyJ;
  // check if remaining energy is 0
  if (m_remainingEnergyJ <= 0)
    {
      HandleEnergyDrainedEvent ();
    }
}

void
BasicEnergySource::HandleEnergyDrainedEvent (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("BasicEnergySource:Energy depleted!");
  NotifyEnergyDrained (); // notify DeviceEnergyModel objects
  // energy never goes below 0
  m_remainingEnergyJ = 0;
}

void
BasicEnergySource::DoIncreaseRemainingEnergy (double energyJ)
{
  NS_LOG_FUNCTION (this << energyJ);
  NS_ASSERT (energyJ >= 0);
  m_remainingEnergyJ += energyJ;
}

double
BasicEnergySource::DoGetEnergyFraction (void) const
{
  NS_LOG_FUNCTION (this);
  return m_remainingEnergyJ / m_initialEnergyJ;
}

} // namespace ns3
