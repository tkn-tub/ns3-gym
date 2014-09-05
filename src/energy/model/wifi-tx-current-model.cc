/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Universita' degli Studi di Napoli "Federico II"
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
 * Author: Stefano Avallone <stefano.avallone@unina.it>
 */

#include "wifi-tx-current-model.h"
#include "ns3/log.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/pointer.h"
#include <cmath>

NS_LOG_COMPONENT_DEFINE ("WifiTxCurrentModel");

namespace ns3 {

// ------------------------------------------------------------------------- //

NS_OBJECT_ENSURE_REGISTERED (WifiTxCurrentModel);

TypeId 
WifiTxCurrentModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WifiTxCurrentModel")
    .SetParent<Object> ()
  ;
  return tid;
}

WifiTxCurrentModel::WifiTxCurrentModel()
{
}

WifiTxCurrentModel::~WifiTxCurrentModel()
{
}

double
WifiTxCurrentModel::DbmToW (double dbm)
{
  double mW = std::pow (10.0, dbm / 10.0);
  return mW / 1000.0;
}

// ------------------------------------------------------------------------- //

NS_OBJECT_ENSURE_REGISTERED (LinearWifiTxCurrentModel);

TypeId 
LinearWifiTxCurrentModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LinearWifiTxCurrentModel")
    .SetParent<WifiTxCurrentModel> ()
    .AddConstructor<LinearWifiTxCurrentModel> ()
    .AddAttribute ("Eta", "The efficiency of the power amplifier.",
                   DoubleValue (0.10),
                   MakeDoubleAccessor (&LinearWifiTxCurrentModel::SetEta,
                                       &LinearWifiTxCurrentModel::GetEta),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Voltage", "The supply voltage (in Volts).",
                   DoubleValue (3.0),
                   MakeDoubleAccessor (&LinearWifiTxCurrentModel::SetVoltage,
                                       &LinearWifiTxCurrentModel::GetVoltage),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("IdleCurrent", "The current in the IDLE state (in Watts).",
                   DoubleValue (0.273333),
                   MakeDoubleAccessor (&LinearWifiTxCurrentModel::SetIdleCurrent,
                                       &LinearWifiTxCurrentModel::GetIdleCurrent),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

LinearWifiTxCurrentModel::LinearWifiTxCurrentModel ()
{
}

LinearWifiTxCurrentModel::~LinearWifiTxCurrentModel()
{
}

void
LinearWifiTxCurrentModel::SetEta (double eta)
{
  m_eta = eta;
}

void
LinearWifiTxCurrentModel::SetVoltage (double voltage)
{
  m_voltage = voltage;
}

void
LinearWifiTxCurrentModel::SetIdleCurrent (double idleCurrent)
{
  m_idleCurrent = idleCurrent;
}

double
LinearWifiTxCurrentModel::GetEta (void) const
{
  return m_eta;
}

double
LinearWifiTxCurrentModel::GetVoltage (void) const
{
  return m_voltage;
}

double
LinearWifiTxCurrentModel::GetIdleCurrent (void) const
{
  return m_idleCurrent;
}

double
LinearWifiTxCurrentModel::CalcTxCurrent (double txPowerDbm) const
{
  return DbmToW (txPowerDbm) / (m_voltage * m_eta) + m_idleCurrent;
}

// ------------------------------------------------------------------------- //

} // namespace ns3
