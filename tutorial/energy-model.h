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

#ifndef ENERGY_MODEL_H
#define ENERGY_MODEL_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/nstime.h"

namespace ns3 {

class EnergyModel : public Object
{
public:
  static const InterfaceId iid;

  EnergyModel (
    double ampHours, 
    double volts,
    double idlePower,
    double receivePower,
    double transmitPower);

  virtual ~EnergyModel ();

  double GetCapacity (Time t);

  double GetTotalIdlePower (Time t);
  double GetTotalTransmitPower (void);
  double GetTotalReceivePower (void);

  bool DrawTransmitPower (Time t);
  bool DrawReceivePower (Time t);

private:
  double m_capacity;
  double m_idlePower;
  double m_receivePower;
  double m_transmitPower;
  double m_totalTransmitPower;
  double m_totalReceivePower;
};

}; // namespace ns3

#endif /* ENERGY_MODEL_H */
