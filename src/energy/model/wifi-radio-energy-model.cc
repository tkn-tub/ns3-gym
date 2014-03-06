/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
    .AddAttribute ("IdleCurrentA",
                   "The default radio Idle current in Ampere.",
                   DoubleValue (0.000426),  // idle mode = 426uA
                   MakeDoubleAccessor (&WifiRadioEnergyModel::SetIdleCurrentA,
                                       &WifiRadioEnergyModel::GetIdleCurrentA),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("CcaBusyCurrentA",
                   "The default radio CCA Busy State current in Ampere.",
                   DoubleValue (0.000426),  // default to be the same as idle mode
                   MakeDoubleAccessor (&WifiRadioEnergyModel::SetCcaBusyCurrentA,
                                       &WifiRadioEnergyModel::GetCcaBusyCurrentA),
                   MakeDoubleChecker<double> ())
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
    .AddAttribute ("SwitchingCurrentA",
                   "The default radio Channel Switch current in Ampere.",
                   DoubleValue (0.000426),  // default to be the same as idle mode
                   MakeDoubleAccessor (&WifiRadioEnergyModel::SetSwitchingCurrentA,
                                       &WifiRadioEnergyModel::GetSwitchingCurrentA),
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
  m_currentState = WifiPhy::IDLE;  // initially IDLE
  m_lastUpdateTime = Seconds (0.0);
  m_energyDepletionCallback.Nullify ();
  m_source = NULL;
  // set callback for WifiPhy listener
  m_listener = new WifiRadioEnergyModelPhyListener;
  m_listener->SetChangeStateCallback (MakeCallback (&DeviceEnergyModel::ChangeState, this));
}

WifiRadioEnergyModel::~WifiRadioEnergyModel ()
{
  NS_LOG_FUNCTION (this);
  delete m_listener;
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
WifiRadioEnergyModel::GetCcaBusyCurrentA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_ccaBusyCurrentA;
}

void
WifiRadioEnergyModel::SetCcaBusyCurrentA (double CcaBusyCurrentA)
{
  NS_LOG_FUNCTION (this << CcaBusyCurrentA);
  m_ccaBusyCurrentA = CcaBusyCurrentA;
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
WifiRadioEnergyModel::GetSwitchingCurrentA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_switchingCurrentA;
}

void
WifiRadioEnergyModel::SetSwitchingCurrentA (double switchingCurrentA)
{
  NS_LOG_FUNCTION (this << switchingCurrentA);
  m_switchingCurrentA = switchingCurrentA;
}


WifiPhy::State
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

  Time duration = Simulator::Now () - m_lastUpdateTime;
  NS_ASSERT (duration.GetNanoSeconds () >= 0); // check if duration is valid

  // energy to decrease = current * voltage * time
  double energyToDecrease = 0.0;
  double supplyVoltage = m_source->GetSupplyVoltage ();
  switch (m_currentState)
    {
    case WifiPhy::IDLE:
      energyToDecrease = duration.GetSeconds () * m_idleCurrentA * supplyVoltage;
      break;
    case WifiPhy::CCA_BUSY:
      energyToDecrease = duration.GetSeconds () * m_ccaBusyCurrentA * supplyVoltage;
      break;
    case WifiPhy::TX:
      energyToDecrease = duration.GetSeconds () * m_txCurrentA * supplyVoltage;
      break;
    case WifiPhy::RX:
      energyToDecrease = duration.GetSeconds () * m_rxCurrentA * supplyVoltage;
      break;
    case WifiPhy::SWITCHING:
      energyToDecrease = duration.GetSeconds () * m_switchingCurrentA * supplyVoltage;
      break;
    default:
      NS_FATAL_ERROR ("WifiRadioEnergyModel:Undefined radio state: " << m_currentState);
    }

  // update total energy consumption
  m_totalEnergyConsumption += energyToDecrease;

  // update last update time stamp
  m_lastUpdateTime = Simulator::Now ();

  // notify energy source
  m_source->UpdateEnergySource ();

  // update current state & last update time stamp
  SetWifiRadioState ((WifiPhy::State) newState);

  // some debug message
  NS_LOG_DEBUG ("WifiRadioEnergyModel:Total energy consumption is " <<
                m_totalEnergyConsumption << "J");
}

void
WifiRadioEnergyModel::HandleEnergyDepletion (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("WifiRadioEnergyModel:Energy is depleted!");
  // invoke energy depletion callback, if set.
  if (!m_energyDepletionCallback.IsNull ())
    {
      m_energyDepletionCallback ();
    }
}

WifiRadioEnergyModelPhyListener *
WifiRadioEnergyModel::GetPhyListener (void)
{
  NS_LOG_FUNCTION (this);
  return m_listener;
}

/*
 * Private functions start here.
 */

void
WifiRadioEnergyModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_source = NULL;
  m_energyDepletionCallback.Nullify ();
}

double
WifiRadioEnergyModel::DoGetCurrentA (void) const
{
  NS_LOG_FUNCTION (this);
  switch (m_currentState)
    {
    case WifiPhy::IDLE:
      return m_idleCurrentA;
    case WifiPhy::CCA_BUSY:
      return m_ccaBusyCurrentA;
    case WifiPhy::TX:
      return m_txCurrentA;
    case WifiPhy::RX:
      return m_rxCurrentA;
    case WifiPhy::SWITCHING:
      return m_switchingCurrentA;
    default:
      NS_FATAL_ERROR ("WifiRadioEnergyModel:Undefined radio state:" << m_currentState);
    }
}

void
WifiRadioEnergyModel::SetWifiRadioState (const WifiPhy::State state)
{
  NS_LOG_FUNCTION (this << state);
  m_currentState = state;
  std::string stateName;
  switch (state)
    {
    case WifiPhy::IDLE:
      stateName = "IDLE";
      break;
    case WifiPhy::CCA_BUSY:
      stateName = "CCA_BUSY";
      break;
    case WifiPhy::TX:
      stateName = "TX";
      break;
    case WifiPhy::RX:
      stateName = "RX";
      break;
    case WifiPhy::SWITCHING:
      stateName = "SWITCHING";
      break;
    }
  NS_LOG_DEBUG ("WifiRadioEnergyModel:Switching to state: " << stateName <<
                " at time = " << Simulator::Now ());
}

// -------------------------------------------------------------------------- //

WifiRadioEnergyModelPhyListener::WifiRadioEnergyModelPhyListener ()
{
  NS_LOG_FUNCTION (this);
  m_changeStateCallback.Nullify ();
}

WifiRadioEnergyModelPhyListener::~WifiRadioEnergyModelPhyListener ()
{
  NS_LOG_FUNCTION (this);
}

void
WifiRadioEnergyModelPhyListener::SetChangeStateCallback (DeviceEnergyModel::ChangeStateCallback callback)
{
  NS_LOG_FUNCTION (this << &callback);
  NS_ASSERT (!callback.IsNull ());
  m_changeStateCallback = callback;
}

void
WifiRadioEnergyModelPhyListener::NotifyRxStart (Time duration)
{
  NS_LOG_FUNCTION (this << duration);
  if (m_changeStateCallback.IsNull ())
    {
      NS_FATAL_ERROR ("WifiRadioEnergyModelPhyListener:Change state callback not set!");
    }
  m_changeStateCallback (WifiPhy::RX);
  m_switchToIdleEvent.Cancel ();
}

void
WifiRadioEnergyModelPhyListener::NotifyRxEndOk (void)
{
  NS_LOG_FUNCTION (this);
  if (m_changeStateCallback.IsNull ())
    {
      NS_FATAL_ERROR ("WifiRadioEnergyModelPhyListener:Change state callback not set!");
    }
  m_changeStateCallback (WifiPhy::IDLE);
}

void
WifiRadioEnergyModelPhyListener::NotifyRxEndError (void)
{
  NS_LOG_FUNCTION (this);
  if (m_changeStateCallback.IsNull ())
    {
      NS_FATAL_ERROR ("WifiRadioEnergyModelPhyListener:Change state callback not set!");
    }
  m_changeStateCallback (WifiPhy::IDLE);
}

void
WifiRadioEnergyModelPhyListener::NotifyTxStart (Time duration)
{
  NS_LOG_FUNCTION (this << duration);
  if (m_changeStateCallback.IsNull ())
    {
      NS_FATAL_ERROR ("WifiRadioEnergyModelPhyListener:Change state callback not set!");
    }
  m_changeStateCallback (WifiPhy::TX);
  // schedule changing state back to IDLE after TX duration
  m_switchToIdleEvent.Cancel ();
  m_switchToIdleEvent = Simulator::Schedule (duration, &WifiRadioEnergyModelPhyListener::SwitchToIdle, this);
}

void
WifiRadioEnergyModelPhyListener::NotifyMaybeCcaBusyStart (Time duration)
{
  NS_LOG_FUNCTION (this << duration);
  if (m_changeStateCallback.IsNull ())
    {
      NS_FATAL_ERROR ("WifiRadioEnergyModelPhyListener:Change state callback not set!");
    }
  m_changeStateCallback (WifiPhy::CCA_BUSY);
  // schedule changing state back to IDLE after CCA_BUSY duration
  m_switchToIdleEvent.Cancel ();
  m_switchToIdleEvent = Simulator::Schedule (duration, &WifiRadioEnergyModelPhyListener::SwitchToIdle, this);
}

void
WifiRadioEnergyModelPhyListener::NotifySwitchingStart (Time duration)
{
  NS_LOG_FUNCTION (this << duration);
  if (m_changeStateCallback.IsNull ())
    {
      NS_FATAL_ERROR ("WifiRadioEnergyModelPhyListener:Change state callback not set!");
    }
  m_changeStateCallback (WifiPhy::SWITCHING);
  // schedule changing state back to IDLE after CCA_BUSY duration
  m_switchToIdleEvent.Cancel ();
  m_switchToIdleEvent = Simulator::Schedule (duration, &WifiRadioEnergyModelPhyListener::SwitchToIdle, this);
}

/*
 * Private function state here.
 */

void
WifiRadioEnergyModelPhyListener::SwitchToIdle (void)
{
  NS_LOG_FUNCTION (this);
  if (m_changeStateCallback.IsNull ())
    {
      NS_FATAL_ERROR ("WifiRadioEnergyModelPhyListener:Change state callback not set!");
    }
  m_changeStateCallback (WifiPhy::IDLE);
}

} // namespace ns3
