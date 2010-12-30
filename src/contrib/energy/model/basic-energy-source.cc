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
#include "ns3/simulator.h"

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
                                       &BasicEnergySource::GetInitialEnergy),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("BasicEnergySupplyVoltageV",
                   "Initial supply voltage for basic energy source.",
                   DoubleValue (3.0), // in Volts
                   MakeDoubleAccessor (&BasicEnergySource::SetSupplyVoltage,
                                       &BasicEnergySource::GetSupplyVoltage),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("PeriodicEnergyUpdateInterval",
                   "Time between two consecutive periodic energy updates.",
                   TimeValue (Seconds (1.0)),
                   MakeTimeAccessor (&BasicEnergySource::SetEnergyUpdateInterval,
                                     &BasicEnergySource::GetEnergyUpdateInterval),
                   MakeTimeChecker ())
    .AddTraceSource ("RemainingEnergy",
                     "Remaining energy at BasicEnergySource.",
                     MakeTraceSourceAccessor (&BasicEnergySource::m_remainingEnergyJ))
  ;
  return tid;
}

BasicEnergySource::BasicEnergySource ()
{
  m_lastUpdateTime = Seconds (0.0);
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
  m_remainingEnergyJ = m_initialEnergyJ;
}

void
BasicEnergySource::SetSupplyVoltage (double supplyVoltageV)
{
  NS_LOG_FUNCTION (this << supplyVoltageV);
  m_supplyVoltageV = supplyVoltageV;
}

void
BasicEnergySource::SetEnergyUpdateInterval (Time interval)
{
  NS_LOG_FUNCTION (this << interval);
  m_energyUpdateInterval = interval;
}

Time
BasicEnergySource::GetEnergyUpdateInterval (void) const
{
  return m_energyUpdateInterval;
}

double
BasicEnergySource::GetSupplyVoltage (void) const
{
  return m_supplyVoltageV;
}

double
BasicEnergySource::GetInitialEnergy (void) const
{
  return m_initialEnergyJ;
}

double
BasicEnergySource::GetRemainingEnergy (void)
{
  NS_LOG_FUNCTION (this);
  // update energy source to get the latest remaining energy.
  UpdateEnergySource ();
  return m_remainingEnergyJ;
}

double
BasicEnergySource::GetEnergyFraction (void)
{
  NS_LOG_FUNCTION (this);
  // update energy source to get the latest remaining energy.
  UpdateEnergySource ();
  return m_remainingEnergyJ / m_initialEnergyJ;
}

void
BasicEnergySource::UpdateEnergySource (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("BasicEnergySource:Updating remaining energy.");

  // do not update if simulation has finished
  if (Simulator::IsFinished ())
    {
      return;
    }

  m_energyUpdateEvent.Cancel ();

  CalculateRemainingEnergy ();

  if (m_remainingEnergyJ <= 0)
    {
      HandleEnergyDrainedEvent ();
      return; // stop periodic update
    }

  m_lastUpdateTime = Simulator::Now ();

  m_energyUpdateEvent = Simulator::Schedule (m_energyUpdateInterval,
                                             &BasicEnergySource::UpdateEnergySource,
                                             this);
}

/*
 * Private functions start here.
 */

void
BasicEnergySource::DoStart (void)
{
  NS_LOG_FUNCTION (this);
  UpdateEnergySource ();  // start periodic update
}

void
BasicEnergySource::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  BreakDeviceEnergyModelRefCycle ();  // break reference cycle
}

void
BasicEnergySource::HandleEnergyDrainedEvent (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("BasicEnergySource:Energy depleted!");
  NotifyEnergyDrained (); // notify DeviceEnergyModel objects
  m_remainingEnergyJ = 0; // energy never goes below 0
}

void
BasicEnergySource::CalculateRemainingEnergy (void)
{
  NS_LOG_FUNCTION (this);
  double totalCurrentA = CalculateTotalCurrent ();
  Time duration = Simulator::Now () - m_lastUpdateTime;
  NS_ASSERT (duration.GetSeconds () >= 0);
  // energy = current * voltage * time
  double energyToDecreaseJ = totalCurrentA * m_supplyVoltageV * duration.GetSeconds ();
  m_remainingEnergyJ -= energyToDecreaseJ;
  NS_LOG_DEBUG ("BasicEnergySource:Remaining energy = " << m_remainingEnergyJ);
}

} // namespace ns3
