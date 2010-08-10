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
#include "energy-source.h"
#include "wifi-radio-energy-model.h"

NS_LOG_COMPONENT_DEFINE ("WifiRadioEnergyModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (WifiRadioEnergyModel);

TypeId
WifiRadioEnergyModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WifiRadioEnergyModel")
    .SetParent<DeviceEnergyModel> ()
    .AddConstructor<WifiRadioEnergyModel> ()
    .AddAttribute ("TxCurrentA",
                   "The radio Tx current in Ampere.",
                   DoubleValue (0.0174),    // transmit at 0dBm = 17.4mA
                   MakeDoubleAccessor (&WifiRadioEnergyModel::SetTxCurrentA,
                                       &WifiRadioEnergyModel::GetTxCurrentA),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("RxCurrentA",
                   "The radio Rx current in Ampere.",
                   DoubleValue (0.0197),    // receive mode = 19.7mA
                   MakeDoubleAccessor (&WifiRadioEnergyModel::SetRxCurrentA,
                                       &WifiRadioEnergyModel::GetRxCurrentA),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("IdleCurrentA",
                   "The default radio Idle current in Ampere.",
                   DoubleValue (0.000426),  // idle mode = 426uA
                   MakeDoubleAccessor (&WifiRadioEnergyModel::SetIdleCurrentA,
                                       &WifiRadioEnergyModel::GetIdleCurrentA),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("SleepCurrentA",
                   "The default radio Sleep current in Ampere.",
                   DoubleValue (0.00002),   // power down mode = 20uA
                   MakeDoubleAccessor (&WifiRadioEnergyModel::SetSleepCurrentA,
                                       &WifiRadioEnergyModel::GetSleepCurrentA),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("TotalEnergyConsumption",
                     "Total energy consumption of the radio device.",
                     MakeTraceSourceAccessor (&WifiRadioEnergyModel::m_totalEnergyConsumption))
  ; 
  return tid;
}

WifiRadioEnergyModel::WifiRadioEnergyModel ()
{
  NS_LOG_FUNCTION (this);
  m_currentState = IDLE;  // initially IDLE
  m_lastUpdateTime = Seconds (0.0);
  m_energyDepletionCallback.Nullify ();
  m_node = NULL;
  m_source = NULL;
}

WifiRadioEnergyModel::~WifiRadioEnergyModel ()
{
}

void
WifiRadioEnergyModel::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this << node);
  NS_ASSERT (node != NULL);
  m_node = node;
}

Ptr<Node>
WifiRadioEnergyModel::GetNode (void) const
{
  return m_node;
}

void
WifiRadioEnergyModel::SetEnergySource (Ptr<EnergySource> source)
{
  NS_LOG_FUNCTION (this << source);
  NS_ASSERT (source != NULL);
  m_source = source;
}

double
WifiRadioEnergyModel::GetTotalEnergyConsumption (void) const
{
  NS_LOG_FUNCTION (this);
  return m_totalEnergyConsumption;
}

double
WifiRadioEnergyModel::GetTxCurrentA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_txCurrentA;
}

void
WifiRadioEnergyModel::SetTxCurrentA (double txCurrentA)
{
  NS_LOG_FUNCTION (this << txCurrentA);
  m_txCurrentA = txCurrentA;
}

double
WifiRadioEnergyModel::GetRxCurrentA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_rxCurrentA;
}

void
WifiRadioEnergyModel::SetRxCurrentA (double rxCurrentA)
{
  NS_LOG_FUNCTION (this << rxCurrentA);
  m_rxCurrentA = rxCurrentA;
}

double
WifiRadioEnergyModel::GetIdleCurrentA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_idleCurrentA;
}

void
WifiRadioEnergyModel::SetIdleCurrentA (double idleCurrentA)
{
  NS_LOG_FUNCTION (this << idleCurrentA);
  m_idleCurrentA = idleCurrentA;
}

double
WifiRadioEnergyModel::GetSleepCurrentA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_sleepCurrentA;
}

void
WifiRadioEnergyModel::SetSleepCurrentA (double sleepCurrentA)
{
  NS_LOG_FUNCTION (this << sleepCurrentA);
  m_sleepCurrentA = sleepCurrentA;
}

WifiRadioEnergyModel::WifiRadioState
WifiRadioEnergyModel::GetCurrentState (void) const
{
  NS_LOG_FUNCTION (this);
  return m_currentState;
}

void
WifiRadioEnergyModel::SetEnergyDepletionCallback (
    WifiRadioEnergyDepletionCallback callback)
{
  NS_LOG_FUNCTION (this);
  if (callback.IsNull ())
    {
      NS_LOG_DEBUG ("WifiRadioEnergyModel:Setting NULL energy depletion callback!");
    }
  m_energyDepletionCallback = callback;
}

void
WifiRadioEnergyModel::ChangeState (int newState)
{
  NS_LOG_FUNCTION (this << newState);
  NS_ASSERT (IsStateTransitionValid ((WifiRadioState) newState));

  Time duration = Simulator::Now () - m_lastUpdateTime;
  NS_ASSERT (duration.GetNanoSeconds () >= 0); // check if duration is valid

  // energy to decrease = current * voltage * time
  double energyToDecrease = 0.0;
  double supplyVoltage = m_source->GetSupplyVoltage ();
  switch (m_currentState)
    {
    case TX:
      energyToDecrease = duration.GetSeconds () * m_txCurrentA * supplyVoltage;
      break;
    case RX:
      energyToDecrease = duration.GetSeconds () * m_rxCurrentA * supplyVoltage;
      break;
    case IDLE:
      energyToDecrease = duration.GetSeconds () * m_idleCurrentA * supplyVoltage;
      break;
    case SLEEP:
      energyToDecrease = duration.GetSeconds () * m_sleepCurrentA * supplyVoltage;
      break;
    default:
      NS_FATAL_ERROR ("WifiRadioEnergyModel:Undefined radio state!");
    }

  // update total energy consumption
  m_totalEnergyConsumption += energyToDecrease;

  // update last update time stamp
  m_lastUpdateTime = Simulator::Now ();

  // notify energy source
  m_source->UpdateEnergySource ();

  // update current state & last update time stamp
  SetWifiRadioState ((WifiRadioState) newState);

  // some debug message
  NS_LOG_DEBUG ("WifiRadioEnergyModel:Total energy consumption at node #"
      << m_node->GetId () << " is " << m_totalEnergyConsumption << "J");
}

void
WifiRadioEnergyModel::HandleEnergyDepletion (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("WifiRadioEnergyModel:Energy is depleted at node #" <<
                m_node->GetId ());
  // invoke energy depletion callback, if set.
  if (!m_energyDepletionCallback.IsNull ())
    {
      m_energyDepletionCallback ();
    }
}

/*
 * Private functions start here.
 */

void
WifiRadioEnergyModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_node = NULL;
  m_source = NULL;
  m_energyDepletionCallback.Nullify ();
}

double
WifiRadioEnergyModel::DoGetCurrentA (void) const
{
  NS_LOG_FUNCTION (this);
  switch (m_currentState)
    {
    case TX:
      return m_txCurrentA;
    case RX:
      return m_rxCurrentA;
    case IDLE:
      return m_idleCurrentA;
    case SLEEP:
      return m_sleepCurrentA;
    default:
      NS_FATAL_ERROR ("WifiRadioEnergyModel:Undefined radio state!");
    }
}

bool
WifiRadioEnergyModel::IsStateTransitionValid (const WifiRadioState destState)
{
  NS_LOG_FUNCTION (this << destState);

  /*
   * This is a placeholder function to specify if some of the radio state
   * transitions are prohibited. For example, if transition TX -> RX is not
   * allowed, and must go through IDLE as TX -> IDLE -> RX, then this method
   * can be used to raise an ns3 error.
   */

  return true;
}

void
WifiRadioEnergyModel::SetWifiRadioState (const WifiRadioState state)
{
  NS_LOG_FUNCTION (this);
  if (IsStateTransitionValid (state))
    {
      m_currentState = state;
      std::string stateName;
      switch (state)
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
      NS_LOG_DEBUG ("WifiRadioEnergyModel:Switching to state: " << stateName <<
                    " at time = " << Simulator::Now ());
    }
  else
    {
      NS_FATAL_ERROR ("WifiRadioEnergyModel:Invalid state transition!");
    }
}

} // namespace ns3
