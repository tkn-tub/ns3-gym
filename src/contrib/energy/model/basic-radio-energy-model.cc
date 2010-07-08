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
 * Author: Sidharth Nabar <snabar@uw.edu>, He Wu <mdzz@u.washington.edu>
 */

#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/simulator.h"
#include "ns3/trace-source-accessor.h"
#include "basic-radio-energy-model.h"

NS_LOG_COMPONENT_DEFINE ("BasicRadioEnergyModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (BasicRadioEnergyModel);

TypeId
BasicRadioEnergyModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BasicRadioEnergyModel")
    .SetParent<RadioEnergyModel> ()
    .AddConstructor<BasicRadioEnergyModel> ()
    .AddAttribute ("TxPowerW",
                   "The radio Tx power in Watts.",
                   DoubleValue (0.0435),    // typical CC2420 value
                   MakeDoubleAccessor (&BasicRadioEnergyModel::SetTxPowerW,
                                       &BasicRadioEnergyModel::GetTxPowerW),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("RxPowerW",
                   "The radio Rx power in Watts.",
                   DoubleValue (0.047),     // typical CC2420 value
                   MakeDoubleAccessor (&BasicRadioEnergyModel::SetRxPowerW,
                                       &BasicRadioEnergyModel::GetRxPowerW),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("IdlePowerW",
                   "The default radio Idle power in Watts.",
                   DoubleValue (0.001065),  // typical CC2420 value
                   MakeDoubleAccessor (&BasicRadioEnergyModel::SetIdlePowerW,
                                       &BasicRadioEnergyModel::GetIdlePowerW),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("SleepPowerW",
                   "The default radio Sleep power in Watts.",
                   DoubleValue (0.00005),   // typical CC2420 value
                   MakeDoubleAccessor (&BasicRadioEnergyModel::SetSleepPowerW,
                                       &BasicRadioEnergyModel::GetSleepPowerW),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("PeriodicEnergyUpdateInterval",
                   "Time between two consecutive periodic energy updates.",
                   TimeValue (Seconds (1.0)),
                   MakeTimeAccessor (&BasicRadioEnergyModel::SetEnergyUpdateInterval,
                                     &BasicRadioEnergyModel::GetEnergyUpdateInterval),
                   MakeTimeChecker ())
    .AddTraceSource ("TotalEnergyConsumption",
                     "Total energy consumption of the radio device.",
                     MakeTraceSourceAccessor (&BasicRadioEnergyModel::m_totalEnergyConsumption))
  ; 
  return tid;
}

BasicRadioEnergyModel::BasicRadioEnergyModel ()
{
  NS_LOG_FUNCTION (this);
  m_currentState = IDLE;
  m_lastUpdateTime = Seconds (0.0);
  m_energyDepletionCallback.Nullify ();
  /*
   * Start scheduling the periodic energy update event. BasicRadioEnergyModel
   * is *not* aggregated to the node. Hence we have to schedule this in the
   * constructor instead of calling it in DoStart.
   */
  m_periodicEnergyUpdateEvent = Simulator::Schedule (Seconds (0.0),
                                                     &BasicRadioEnergyModel::DoUpdateRemainingEnergy, this, m_currentState);
}

BasicRadioEnergyModel::~BasicRadioEnergyModel ()
{
}

BasicRadioEnergyModel::RadioState
BasicRadioEnergyModel::GetCurrentState (void) const
{
  NS_LOG_FUNCTION (this);
  return m_currentState;
}

void
BasicRadioEnergyModel::SetCurrentState (const RadioState currentState)
{
  NS_LOG_FUNCTION (this);
  if (currentState != m_currentState)
    {
      m_currentState = currentState;
      std::string stateName;
      switch (currentState)
        {
        case TX:
          stateName = "TX";
          break;
        case RX:
          stateName = "RX";
          break;
        case IDLE:
          stateName = "IDLE";
          break;
        case SLEEP:
          stateName = "SLEEP";
          break;
        }
      NS_LOG_DEBUG ("BasicRadioEnergyModel: Switching to state: "
                    << stateName << " at time = " << Simulator::Now ());
    }
}

Time
BasicRadioEnergyModel::GetEnergyUpdateInterval (void) const
{
  NS_LOG_FUNCTION (this);
  return m_energyUpdateInterval;
}

void
BasicRadioEnergyModel::SetEnergyUpdateInterval (const Time interval)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (interval.GetSeconds () > 0);
  m_energyUpdateInterval = interval;
}

void
BasicRadioEnergyModel::SetEnergyDepletionCallback (
  BasicEnergyDepletionCallback callback)
{
  NS_LOG_FUNCTION (this);
  if (callback.IsNull ())
    {
      NS_LOG_DEBUG ("BasicRadioEnergyModel:Setting NULL energy depletion callback!");
    }
  m_energyDepletionCallback = callback;
}

bool
BasicRadioEnergyModel::IsStateTransitionValid (const RadioState destState)
{
  NS_LOG_FUNCTION (this << destState);
  bool returnValue = true;

  /*
   * This is a placeholder function to specify if some of the radio state
   * transitions are prohibited. For example, if transition TX -> RX is not
   * allowed, and must go through IDLE as TX -> IDLE -> RX, then this method
   * can be used to raise an ns3 error. Initial state is m_currentState.
   */

  return returnValue;
}

/*
 * Private functions start here.
 */

void
BasicRadioEnergyModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  BreakSourceRefCycle (); // break reference cycle to energy source
  m_energyDepletionCallback.Nullify ();
}

void
BasicRadioEnergyModel::DoHandleEnergyDepletion (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("BasicRadioEnergyModel:Energy is depleted!");
  // invoke energy depletion callback, if set.
  if (!m_energyDepletionCallback.IsNull ())
    {
      m_energyDepletionCallback ();
    }
}

double
BasicRadioEnergyModel::DoGetTotalEnergyConsumption (void) const
{
  NS_LOG_FUNCTION (this);
  return m_totalEnergyConsumption;
}

double
BasicRadioEnergyModel::DoGetTxPowerW (void) const
{
  NS_LOG_FUNCTION (this);
  return m_txPower;
}

void 
BasicRadioEnergyModel::DoSetTxPowerW (double txPowerW)
{
  NS_LOG_FUNCTION (this << txPowerW);
  m_txPower = txPowerW;
}

double
BasicRadioEnergyModel::DoGetRxPowerW (void) const
{
  NS_LOG_FUNCTION (this);
  return m_rxPower;
}

void 
BasicRadioEnergyModel::DoSetRxPowerW (double rxPowerW)
{
  NS_LOG_FUNCTION (this << rxPowerW);
  m_rxPower = rxPowerW;
}

double
BasicRadioEnergyModel::DoGetIdlePowerW (void) const
{
  NS_LOG_FUNCTION (this);
  return m_idlePower;
}

void 
BasicRadioEnergyModel::DoSetIdlePowerW (double idlePowerW)
{
  NS_LOG_FUNCTION (this << idlePowerW);
  m_idlePower = idlePowerW;
}

double
BasicRadioEnergyModel::DoGetSleepPowerW (void) const
{
  NS_LOG_FUNCTION (this);
  return m_sleepPower;
}

void 
BasicRadioEnergyModel::DoSetSleepPowerW (double sleepPowerW)
{
  NS_LOG_FUNCTION (this << sleepPowerW);
  m_sleepPower = sleepPowerW;
}

void
BasicRadioEnergyModel::DoUpdateRemainingEnergy (const RadioState destState)
{
  NS_LOG_FUNCTION (this << destState);
  NS_ASSERT (IsStateTransitionValid (destState));

  // Cancel the currently scheduled periodic event, if any
  m_periodicEnergyUpdateEvent.Cancel ();

  Time duration = Simulator::Now () - m_lastUpdateTime;
  NS_ASSERT (duration.GetNanoSeconds () >= 0); // check if duration is valid

  // update remaining energy accordingly
  double energyToDecrease = 0.0;
  switch (m_currentState)
    {
    case TX:
      energyToDecrease = duration.GetSeconds () * m_txPower;
      DecreaseRemainingEnergy (energyToDecrease);
      break;
    case RX:
      energyToDecrease = duration.GetSeconds () * m_rxPower;
      DecreaseRemainingEnergy (energyToDecrease);
      break;
    case IDLE:
      energyToDecrease = duration.GetSeconds () * m_idlePower;
      DecreaseRemainingEnergy (energyToDecrease);
      break;
    case SLEEP:
      energyToDecrease = duration.GetSeconds () * m_sleepPower;
      DecreaseRemainingEnergy (energyToDecrease);
      break;
    }

  // update total energy consumption
  m_totalEnergyConsumption += energyToDecrease;

  // update last update time stamp
  m_lastUpdateTime = Simulator::Now ();

  // update current state & last update time stamp
  SetCurrentState (destState);

  // Schedule the next periodic energy update event
  m_periodicEnergyUpdateEvent = Simulator::Schedule (m_energyUpdateInterval,
                                                     &BasicRadioEnergyModel::DoUpdateRemainingEnergy, this, m_currentState);

  // some debug message
  NS_LOG_DEBUG ("BasicRadioEnergyModel:Total energy consumption is "
                << m_totalEnergyConsumption << "J");
}

} // namespace ns3
