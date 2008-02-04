/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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
 */

#include "ns3/log.h"
#include "energy-model.h"

NS_LOG_COMPONENT_DEFINE("EnergyModel");

namespace ns3 {

//
// Some dimensional analysis ...
//
// 1 [watt] = 1 [joule] / [second]
// 1 [watt] = 1 [volt] * 1 [amp]
// 1 [amp] = 1 [coulomb] / 1 [second]
// 1 [watt-second] = 1 [joule] / [second] * [second] = 1 [joule]
//
// A watt has dimensions of energy per second.  A watt-second has dimensions
// of energy.
//
// 1 [amp-hour] = 1 [coulomb] / 1 [second] * 1 [hour]
// 1 [amp-hour] = 1 [coulomb] / 1 [second] * 3600 [seconds]
// 1 [amp-hour] = 3600 [amp-seconds]
// 1 [watt-second] = 1 [amp-seconds] * 1 [volt]
//
// To get the energy capacity of your battery in watt-seconds from its 
// amp-hour rating, multiply by 3600 and then the voltage.  For example, your
// Alkaline AAA battery may be rated at 1.5 volts and 900 milli-amp-hours;
// so the energy capacity will be .9 * 3600 * 1.5 = 4860 watt-seconds.
//
// In a very simple battery model, we'll take this naively to mean that this
// battery can supply one watt continuously for 4860 seconds and then will die
// instantaneously.
//
// We'll assume our transmitter is measured in watts.  When it is turned on
// it draws some amount of power, 100 milliwatts for example.  If it transmits
// for one second, it will suck up .1 watt-seconds of our precious energy.
// 

const InterfaceId EnergyModel::iid = 
  MakeInterfaceId ("EnergyModel", Object::iid);


EnergyModel::EnergyModel (
  double ampHours, 
  double volts,
  double idlePower,
  double receivePower,
  double transmitPower)
:
  m_capacity (ampHours * 3600. * volts),
  m_idlePower (idlePower),
  m_receivePower (receivePower),
  m_transmitPower (transmitPower),
  m_totalTransmitPower (0.),
  m_totalReceivePower (0.)
{
  NS_LOG_FUNCTION;
  SetInterfaceId (EnergyModel::iid);
}

EnergyModel::~EnergyModel ()
{
  NS_LOG_FUNCTION;
}

  double
EnergyModel::GetCapacity (Time t)
{
  NS_LOG_FUNCTION;
  double c = m_capacity - m_idlePower * t.GetSeconds ();
  return c >= 0. ? c : 0.;
}

  double
EnergyModel::GetTotalIdlePower (Time t)
{
  NS_LOG_FUNCTION;
  return m_idlePower * t.GetSeconds ();
}

  double
EnergyModel::GetTotalReceivePower (void)
{
  NS_LOG_FUNCTION;
  return m_totalReceivePower;
}

  double
EnergyModel::GetTotalTransmitPower (void)
{
  NS_LOG_FUNCTION;
  return m_totalTransmitPower;
}

  bool
EnergyModel::DrawTransmitPower (Time t)
{
  NS_LOG_FUNCTION;
  double power = m_transmitPower * t.GetSeconds ();
  m_totalTransmitPower += power;
  m_capacity -= power;
  return m_capacity - m_idlePower * t.GetSeconds () >= 0. ? true : false;
}

  bool
EnergyModel::DrawReceivePower (Time t)
{
  NS_LOG_FUNCTION;
  double power = m_receivePower * t.GetSeconds ();
  m_totalReceivePower += power;
  m_capacity -= power;
  return m_capacity - m_idlePower * t.GetSeconds () >= 0. ? true : false;
}

}; // namespace ns3
