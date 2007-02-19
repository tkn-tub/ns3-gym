/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
 * All rights reserved.
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
 * Author: Craig Dowell <craigdo@ee.washingon.edu>
 *
 *      Fri Feb 16 12:18:11 PST 2007 craigdo:  created
 */

#include "ns3/debug.h"
#include "ns3/assert.h"
#include "layer-connector.h"

NS_DEBUG_COMPONENT_DEFINE ("LayerConnector");

namespace ns3 {

LayerConnectorUpper::LayerConnectorUpper ()
{
  NS_DEBUG("LayerConnectorUpper::LayerConnectorUpper ()")
}

LayerConnectorUpper::~LayerConnectorUpper ()
{
  NS_DEBUG("LayerConnectorUpper::~LayerConnectorUpper ()")
}

  bool
LayerConnectorUpper::ConnectToLower (LayerConnectorLower &partner)
{
  NS_DEBUG("LayerConnectorUpper::ConnectToLower (" << &partner << ")")

  return DoConnectToLower(partner);
}

  bool
LayerConnectorUpper::DoConnectToLower (LayerConnectorLower &partner)
{
  NS_DEBUG("LayerConnectorUpper::DoConnectToLower (" << &partner << ")")

  m_lowerPartner = &partner;
  NS_ASSERT (m_lowerPartner);
  return true;
}

  bool
LayerConnectorUpper::UpperSendUp (Packet &p)
{
  NS_DEBUG("LayerConnectorUpper::UpperSendUp (" << &p << ")")

  return UpperDoSendUp(p);
}

  bool
LayerConnectorUpper::UpperPull (Packet &p)
{
  NS_DEBUG("LayerConnectorUpper::UpperPull (" << &p << ")")

  return UpperDoPull(p);
}

  bool
LayerConnectorUpper::UpperNotify ()
{
  NS_DEBUG("LayerConnectorUpper::UpperNotify ()")

  NS_ASSERT (m_lowerPartner);
  return m_lowerPartner->LowerNotify(this);
}

LayerConnectorLower::LayerConnectorLower ()
{
  NS_DEBUG("LayerConnectorLower::LayerConnectorLower ()")
}

LayerConnectorLower::~LayerConnectorLower ()
{
  NS_DEBUG("LayerConnectorLower::~LayerConnectorLower ()")
}

  bool
LayerConnectorLower::ConnectToUpper (LayerConnectorUpper &partner)
{
  NS_DEBUG("LayerConnectorLower::ConnectToUpper (" << &partner << ")")

  return DoConnectToUpper(partner);
}

  bool
LayerConnectorLower::DoConnectToUpper (LayerConnectorUpper &partner)
{
  NS_DEBUG("LayerConnectorLower::DoConnectToUpper (" << &partner << ")")

  m_upperPartner = &partner;
  NS_ASSERT (m_upperPartner);
  return true;
}

  bool
LayerConnectorLower::LowerSendUp (Packet &p)
{
  NS_DEBUG("LayerConnectorLower::LowerSendUp (" << &p << ")")

  NS_ASSERT (m_upperPartner);
  return m_upperPartner->UpperSendUp(p);
}

  bool
LayerConnectorLower::LowerPull (Packet &p)
{
  NS_DEBUG("LayerConnectorLower::LowerPull (" << &p << ")")

  NS_ASSERT (m_upperPartner);
  return m_upperPartner->UpperPull(p);
}

  bool
LayerConnectorLower::LowerNotify (LayerConnectorUpper *upper)
{
  NS_DEBUG("LayerConnectorLower::LowerNotify ()")
  return LowerDoNotify(upper);
}

} // namespace ns3
