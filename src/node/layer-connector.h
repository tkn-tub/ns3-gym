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
 *      Wed Feb 14 16:05:46 PST 2007 craigdo:  created
 */

#include "ns3/packet.h"

#ifndef LAYER_CONNECTOR_H
#define LAYER_CONNECTOR_H

namespace ns3 {

class LayerConnectorLower;

class LayerConnectorUpper {
public:
  LayerConnectorUpper ();
  virtual ~LayerConnectorUpper ();

  // called by the layer connector to introduce the lower interface to us here
  bool ConnectToLower (LayerConnectorLower &lower);

  // called by the lower layer to send a received packet on up the protocol
  // stack to us here.
  bool UpperSendUp (Packet &p);

  // Call this function to tell a lower layer that there is data available
  // here.  It is expected that the lower layer will call pull as soon as
  // possible to get the data.
  bool UpperNotify ();

  // called by the lower layer to get available packet data that was implied
  // by a previous Notify() call.
  bool UpperPull(Packet &p);

protected:
  virtual bool DoConnectToLower (LayerConnectorLower &lower);
  virtual bool UpperDoSendUp (Packet &p) = 0;
  virtual bool UpperDoPull (Packet &p) = 0;

  LayerConnectorLower *m_lowerPartner;

private:
};

class LayerConnectorLower
{
public:
  LayerConnectorLower ();
  virtual ~LayerConnectorLower ();

  // This function is called by the layer connector to introduce the upper
  // layer interface to us here
  bool ConnectToUpper (LayerConnectorUpper &upper);

  // Notify is called by the upper layer connector to tell us that there
  // is data available.  It is expected that we will call pull (below) as
  // soon as possible to get the data.
  bool LowerNotify (LayerConnectorUpper *upper);

  // The lower connector calls this method to send a received packet on up 
  // the protocol stack
  bool LowerSendUp (Packet &p);

  // Call this function to get available packet data from the upper connector
  // that was implied by a previous Notify() call.
  bool LowerPull(Packet &p);

protected:
  virtual bool DoConnectToUpper (LayerConnectorUpper &upper);
  virtual bool LowerDoNotify (LayerConnectorUpper *upper) = 0;
  LayerConnectorUpper *m_upperPartner;

private:
};

}; // namespace ns3

#endif /* LAYER_CONNECTOR_H */
