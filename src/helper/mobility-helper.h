/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef MOBILITY_HELPER_H
#define MOBILITY_HELPER_H

#include <vector>
#include "ns3/object-factory.h"
#include "ns3/attribute.h"
#include "ns3/position-allocator.h"
#include "node-container.h"

namespace ns3 {

class PositionAllocator;
class MobilityModel;

/**
 * \brief assign positions and mobility models to nodes.
 *
 * MobilityHelper::Layout is the most important method here.
 */
class MobilityHelper
{
public:
  MobilityHelper ();

  /**
   * After this method is called, every call to MobilityHelper::Layout
   * will also attach to the new ns3::MobilityModel an ns3::MobilityModelNotifier
   * which can be used to listen to CourseChange events.
   */
  void EnableNotifier (void);
  /**
   * After this method is called, no ns3::MobilityModelNotifier object will
   * be associated to any new ns3::MobilityModel created by MobilityHelper::Layout.
   * This will make it impossible to listen to "CourseChange" events from these
   * new ns3::MobilityModel instances.
   */
  void DisableNotifier (void);

  /**
   * \param allocator allocate initial node positions
   *
   * Set the position allocator which will be used to allocate
   * the initial position of every node in MobilityModel::Layout.
   */
  void SetPositionAllocator (Ptr<PositionAllocator> allocator);

  /**
   * \param type the type of mobility model to use.
   * \param n1 the name of the attribute to set in the mobility model.
   * \param v1 the value of the attribute to set in the mobility model.
   * \param n2 the name of the attribute to set in the mobility model.
   * \param v2 the value of the attribute to set in the mobility model.
   * \param n3 the name of the attribute to set in the mobility model.
   * \param v3 the value of the attribute to set in the mobility model.
   * \param n4 the name of the attribute to set in the mobility model.
   * \param v4 the value of the attribute to set in the mobility model.
   * \param n5 the name of the attribute to set in the mobility model.
   * \param v5 the value of the attribute to set in the mobility model.
   * \param n6 the name of the attribute to set in the mobility model.
   * \param v6 the value of the attribute to set in the mobility model.
   * \param n7 the name of the attribute to set in the mobility model.
   * \param v7 the value of the attribute to set in the mobility model.
   * \param n8 the name of the attribute to set in the mobility model.
   * \param v8 the value of the attribute to set in the mobility model.
   * \param n9 the name of the attribute to set in the mobility model.
   * \param v9 the value of the attribute to set in the mobility model.
   */
  void SetPositionAllocator (std::string type,
			     std::string n1 = "", Attribute v1 = Attribute (),
			     std::string n2 = "", Attribute v2 = Attribute (),
			     std::string n3 = "", Attribute v3 = Attribute (),
			     std::string n4 = "", Attribute v4 = Attribute (),
			     std::string n5 = "", Attribute v5 = Attribute (),
			     std::string n6 = "", Attribute v6 = Attribute (),
			     std::string n7 = "", Attribute v7 = Attribute (),
			     std::string n8 = "", Attribute v8 = Attribute (),
			     std::string n9 = "", Attribute v9 = Attribute ());

  /**
   * \param type the type of mobility model to use.
   * \param n1 the name of the attribute to set in the mobility model.
   * \param v1 the value of the attribute to set in the mobility model.
   * \param n2 the name of the attribute to set in the mobility model.
   * \param v2 the value of the attribute to set in the mobility model.
   * \param n3 the name of the attribute to set in the mobility model.
   * \param v3 the value of the attribute to set in the mobility model.
   * \param n4 the name of the attribute to set in the mobility model.
   * \param v4 the value of the attribute to set in the mobility model.
   * \param n5 the name of the attribute to set in the mobility model.
   * \param v5 the value of the attribute to set in the mobility model.
   * \param n6 the name of the attribute to set in the mobility model.
   * \param v6 the value of the attribute to set in the mobility model.
   * \param n7 the name of the attribute to set in the mobility model.
   * \param v7 the value of the attribute to set in the mobility model.
   * \param n8 the name of the attribute to set in the mobility model.
   * \param v8 the value of the attribute to set in the mobility model.
   * \param n9 the name of the attribute to set in the mobility model.
   * \param v9 the value of the attribute to set in the mobility model.
   *
   * Calls to MobilityHelper::Layout will create an instance of a matching 
   * mobility model for each node.
   */
  void SetMobilityModel (std::string type,
			 std::string n1 = "", Attribute v1 = Attribute (),
			 std::string n2 = "", Attribute v2 = Attribute (),
			 std::string n3 = "", Attribute v3 = Attribute (),
			 std::string n4 = "", Attribute v4 = Attribute (),
			 std::string n5 = "", Attribute v5 = Attribute (),
			 std::string n6 = "", Attribute v6 = Attribute (),
			 std::string n7 = "", Attribute v7 = Attribute (),
			 std::string n8 = "", Attribute v8 = Attribute (),
			 std::string n9 = "", Attribute v9 = Attribute ());

  /**
   * \param reference item to push.
   *
   * Push an item on the top of the stack of "reference mobility models".
   * The input item should be a node instance to which a mobility model
   * has already been aggregated (usually by a call to Layout).
   *
   * If this this stack is not empty when MobilityHelper::Layout
   * is called, the model from the top of the stack is used
   * to create a ns3::HierarchicalMobilityModel to make the
   * newly-created models define their positions relative to that
   * of the parent mobility model.
   *
   * This method is typically used to create hierarchical mobility
   * patterns and positions by starting with the large-scale mobility
   * features, and, then, defining the smaller-scale movements relative
   * to a few reference points in the large-scale model.
   */
  void PushReferenceMobilityModel (Ptr<Object> reference);
  /**
   * Remove the top item from the top of the stack of
   * "reference mobility models".
   */
  void PopReferenceMobilityModel (void);

  /**
   * \returns a string which contains the TypeId of the currently-selected
   *          mobility model.
   */
  std::string GetMobilityModelType (void) const;

  /**
   * \param container the set of nodes to layout.
   *
   * For each input node, this method creates an instance of a ns3::MobilityModel
   * subclass (the type of which was set with MobilityHelper::SetMobilityModel), 
   * aggregates it to the mode, and sets an initial position based on the current 
   * position allocator (set through MobilityHelper::SetPositionAllocator). 
   * Optionally, this method will also create and aggregate a
   * ns3::MobilityModelNotifier to generate 'CourseChange' events based on the 
   * boolean flag set by MobilityHelper::EnableNotifier and MobilityHelper::DisableNotifier.
   */
  void Layout (NodeContainer container);

  /**
   * Perform the work of MobilityHelper::Layout on _all_ nodes which
   * exist in the simulation.
   */
  void LayoutAll (void);
private:

  std::vector<Ptr<MobilityModel> > m_mobilityStack;
  bool m_notifierEnabled;
  ObjectFactory m_mobility;
  Ptr<PositionAllocator> m_position;
};

} // namespace ns3

#endif /* MOBILITY_HELPER_H */
