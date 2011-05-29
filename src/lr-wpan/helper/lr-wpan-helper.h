/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 The Boeing Company
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
 * Authors:
 *  Gary Pei <guangyu.pei@boeing.com>
 */
#ifndef LR_WPAN_HELPER_H
#define LR_WPAN_HELPER_H

#include "ns3/node-container.h"
#include "ns3/net-device-container.h"
#include "ns3/lr-wpan-phy.h"
#include "ns3/lr-wpan-mac.h"
#include "ns3/lr-wpan-csmaca.h"
#include "ns3/lr-wpan-error-rate-model.h"
#include "ns3/wpan-spectrum-propagation-loss-model.h"
#include "ns3/single-model-spectrum-channel.h"

namespace ns3 {

/**
 * \brief helps to manage and create ieee.15.4 NetDevice objects
 *
 * This class can help to create a ieee.15.4 NetDevice objects
 * and to configure their attributes during creation.
 */

class LrWpanHelper
{
public:
  /**
   * \brief Create a LrWpan helper in an empty state.
   */
  LrWpanHelper (void);
  ~LrWpanHelper (void);

  /**
  * \brief Add mobility model to a physical device
  * \param phy the physical device
  * \param m the mobility model
  */
  void AddMobility (Ptr<LrWpanPhy> phy, Ptr<MobilityModel>m);

  /**
   * \param c a set of nodes
   * \param type device type to create
   */
  NetDeviceContainer Install (NodeContainer c);

  /**
   * Helper to enable all LrWpan log components with one statement
   */
  void EnableLogComponents (void);

private:
  Ptr<SingleModelSpectrumChannel> m_channel;

};

}

#endif /* LR_WPAN_HELPER_H */
