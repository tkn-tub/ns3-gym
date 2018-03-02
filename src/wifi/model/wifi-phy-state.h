/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#ifndef WIFI_PHY_STATE_H
#define WIFI_PHY_STATE_H

/**
  * The state of the PHY layer.
  */
/// State enumeration
enum WifiPhyState
{
  /**
   * The PHY layer is IDLE.
    */
  IDLE,
  /**
    * The PHY layer has sense the medium busy through the CCA mechanism
    */
  CCA_BUSY,
  /**
    * The PHY layer is sending a packet.
    */
  TX,
  /**
    * The PHY layer is receiving a packet.
    */
  RX,
  /**
    * The PHY layer is switching to other channel.
    */
  SWITCHING,
  /**
    * The PHY layer is sleeping.
    */
  SLEEP,
  /**
    * The PHY layer is switched off.
    */
  OFF
};

#endif /* WIFI_PHY_STATE_H */
