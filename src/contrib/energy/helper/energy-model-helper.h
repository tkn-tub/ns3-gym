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
 * Authors: Sidharth Nabar <snabar@uw.edu>, He Wu <mdzz@u.washington.edu>
 */

#ifndef ENERGY_MODEL_HELPER
#define ENERGY_MODEL_HELPER

#include "ns3/attribute.h"
#include "ns3/object-factory.h"
#include "ns3/node-container.h"
#include "ns3/ptr.h"
#include "ns3/energy-source.h"
#include "ns3/device-energy-model.h"

namespace ns3 {

/**
 * \brief Creates EnergySource objects.
 *
 * This class creates and installs an energy source onto network nodes. This
 * base class must be implemented by new EnergySource implementation which wish
 * to integrate with the \ref ns3::EnergyModelHelper class.
 */
class EnergySourceHelper
{
public:
  virtual ~EnergySourceHelper ();

  /**
   * \returns A newly created EnergySource object.
   * Subclasses must implement this methods to allow the ns3::EnergySource class
   * to create DeviceEnergyModel object for installation.
   */
  virtual Ptr<EnergySource> Create (void) const = 0;

  /**
   * \param node Pointer to the node where EnergySource will be installed on.
   *
   * This method installs an EnergySource onto a node. Must be implemented by
   * subclasses.
   */
  virtual void Install (Ptr<Node> node) const = 0;

};


/**
 * \brief Creates DeviceEnergyModel objects
 *
 * This class can help to create and install an DeviceEnergyModel onto network
 * nodes. It can be called separately to install extra DeviceEnergyModels onto
 * the same nodes. This base class must be implemented by new DeviceEnergyModel
 * which wishes to integrate with the \ref ns3::EnergyModelHelper class.
 *
 * Note that DeviceEnergyModel objects are *not* aggregated onto the node. They
 * can be accessed through the EnergySource object, which *is* aggregated onto
 * the node.
 */
class DeviceEnergyModelHelper
{
public:
  virtual ~DeviceEnergyModelHelper ();

  /**
   * \returns A newly created DeviceEnergySource object.
   *
   * Subclasses must implement this method to allow the ns3::DeviceEnergyModel
   * class to create DeviceEnergyModel object for installation.
   */
  virtual Ptr<DeviceEnergyModel> Create (void) const = 0;

  /**
   * \param name Name of attribute to set.
   * \param v Value of the attribute.
   *
   * Sets one of the attributes of underlying DeviceEnergyModel.
   */
  virtual void Set (std::string name, const AttributeValue &v) = 0;

  /**
   * \param node The node on which a DeviceEnergyModel object must be created.
   *
   * Installs an DeviceEnergyModel onto a node. Subclasses must implement this
   * method.
   */
  void Install (Ptr<Node> node) const;

  /**
   * \param c The set of nodes on which a jammer object must be created.
   *
   * Installs DeviceEnergyModel onto a list of nodes. Calls Install (Ptr<Node>
   * node).
   */
  void Install (NodeContainer c) const;

  /**
   * \param nodeName The name of node.
   *
   * Calls Install (Ptr<Node> node).
   */
  void Install (std::string nodeName) const;

  /**
   * Install on *ALL* nodes exists in simulation.
   */
  void InstallAll (void) const;


private:
  /**
   * \param node The node on which a DeviceEnergyModel object must be created.
   *
   * Implements Install (Ptr<Node>). Subclasses must implement this method.
   */
  virtual void DoInstall (Ptr<Node> node) const = 0;

};


/**
 * \brief Creates EnergySource and DeviceEnergyModel objects.
 *
 * This class helps to create EnergySource and DeviceEnergyModel objects. Note
 * that only a single EnergySource can exist on the same node where multiple
 * DeviceEnergyModel can coexist on the same node. Extra DeviceEnergyModels can
 * be installed by DeviceEnergyModelHelper.
 */
class EnergyModelHelper
{
public:
  /**
   * Create an EnergyModelHelper in an empty state: all its parameters must be
   * set before calling ns3::EnergyModelHelper::Install.
   */
  EnergyModelHelper ();

  /**
   * \param source The EnergySourceHelper to create EnergySource.
   * \param model The DeviceEnergyModelHelper to create DeviceEnergyModel.
   * \param node The node on which the energy model will be installed on.
   */
  void Install (const EnergySourceHelper &sourceHelper,
                const DeviceEnergyModelHelper &modelHelper, Ptr<Node> node) const;

  /**
   * \param source The EnergySourceHelper to create EnergySource.
   * \param model The DeviceEnergyModelHelper to create DeviceEnergyModel.
   * \param node List of node on which the energy model will be installed on.
   */
  void Install (const EnergySourceHelper &sourceHelper,
                const DeviceEnergyModelHelper &modelHelper, NodeContainer c) const;

  /**
   * \param source The EnergySourceHelper to create EnergySource.
   * \param model The DeviceEnergyModelHelper to create DeviceEnergyModel.
   * \param node name of node on which the energy model will be installed on.
   */
  void Install (const EnergySourceHelper &sourceHelper,
                const DeviceEnergyModelHelper &modelHelper, std::string nodeName) const;

  /**
   * \param source The EnergySourceHelper to create EnergySource.
   * \param model The DeviceEnergyModelHelper to create DeviceEnergyModel.
   * \param node name of node on which the energy model will be installed on.
   */
  void InstallAll (const EnergySourceHelper &sourceHelper,
                   const DeviceEnergyModelHelper &modelHelper) const;

};

} // namespace ns3

#endif  /* ENERGY_MODEL_HELPER */
