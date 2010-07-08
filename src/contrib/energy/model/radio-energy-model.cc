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

#include "radio-energy-model.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("RadioEnergyModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (RadioEnergyModel);

TypeId 
RadioEnergyModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RadioEnergyModel")
    .SetParent<DeviceEnergyModel> ()
  ;
  return tid;
}

RadioEnergyModel::RadioEnergyModel ()
{
}

RadioEnergyModel::~RadioEnergyModel ()
{
}

double
RadioEnergyModel::GetTxPowerW (void) const
{
  return DoGetTxPowerW ();
}
double
RadioEnergyModel::GetRxPowerW (void) const
{
  return DoGetRxPowerW ();
}

double
RadioEnergyModel::GetIdlePowerW (void) const
{
  return DoGetIdlePowerW ();
}

double
RadioEnergyModel::GetSleepPowerW (void) const
{
  return DoGetSleepPowerW ();
}

void 
RadioEnergyModel::SetTxPowerW (double txPowerW)
{
  DoSetTxPowerW (txPowerW);
}

void 
RadioEnergyModel::SetRxPowerW (double rxPowerW)
{
  DoSetRxPowerW (rxPowerW);
}

void 
RadioEnergyModel::SetIdlePowerW (double idlePowerW)
{
  DoSetIdlePowerW (idlePowerW);
}

void 
RadioEnergyModel::SetSleepPowerW (double sleepPowerW)
{
  DoSetSleepPowerW (sleepPowerW);
}

void
RadioEnergyModel::UpdateRemainingEnergy (const RadioState destState)
{
  DoUpdateRemainingEnergy (destState);
}

} // namespace ns3
