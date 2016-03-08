/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Universita' degli Studi di Napoli Federico II
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
 * Author: Stefano Avallone <stavallo@unina.it>
 */

#include "ns3/log.h"
#include "ns3/abort.h"
#include "ns3/queue-disc.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"
#include "ns3/traffic-control-layer.h"
#include "traffic-control-helper.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TrafficControlHelper");

QueueDiscFactory::QueueDiscFactory (ObjectFactory factory)
  : m_queueDiscFactory (factory)
{
}

void
QueueDiscFactory::AddInternalQueue (ObjectFactory factory)
{
  m_internalQueuesFactory.push_back (factory);
}

void
QueueDiscFactory::AddPacketFilter (ObjectFactory factory)
{
  m_packetFiltersFactory.push_back (factory);
}

uint16_t
QueueDiscFactory::AddQueueDiscClass (ObjectFactory factory)
{
  m_queueDiscClassesFactory.push_back (factory);
  return m_queueDiscClassesFactory.size () - 1;
}

void
QueueDiscFactory::SetChildQueueDisc (uint16_t classId, uint16_t handle)
{
  NS_ABORT_MSG_IF (classId >= m_queueDiscClassesFactory.size (),
                   "Cannot attach a queue disc to a non existing class");
  m_classIdChildHandleMap[classId] = handle;
}

Ptr<QueueDisc>
QueueDiscFactory::CreateQueueDisc (const std::vector<Ptr<QueueDisc> > & queueDiscs)
{
  // create the queue disc
  Ptr<QueueDisc> qd = m_queueDiscFactory.Create<QueueDisc> ();

  // create and add the internal queues
  for (std::vector<ObjectFactory>::iterator i = m_internalQueuesFactory.begin ();
       i != m_internalQueuesFactory.end (); i++ )
    {
      qd->AddInternalQueue (i->Create<Queue> ());
    }

  // create and add the packet filters
  for (std::vector<ObjectFactory>::iterator i = m_packetFiltersFactory.begin ();
       i != m_packetFiltersFactory.end (); i++ )
    {
      qd->AddPacketFilter (i->Create<PacketFilter> ());
    }

  // create and add the queue disc classes
  for (uint32_t i = 0; i < m_queueDiscClassesFactory.size (); i++)
    {
      // the class ID is given by the index i of the vector
      NS_ABORT_MSG_IF (m_classIdChildHandleMap.find (i) == m_classIdChildHandleMap.end (),
                       "Cannot create a queue disc class with no attached queue disc");

      uint16_t handle = m_classIdChildHandleMap[i];
      NS_ABORT_MSG_IF (handle >= queueDiscs.size () || queueDiscs[handle] == 0, 
                       "A queue disc with handle " << handle << " has not been created yet");

      m_queueDiscClassesFactory[i].Set ("QueueDisc", PointerValue (queueDiscs[handle]));
      qd->AddQueueDiscClass (m_queueDiscClassesFactory[i].Create<QueueDiscClass> ());
    }

  return qd;
}


TrafficControlHelper::TrafficControlHelper ()
{
}

TrafficControlHelper
TrafficControlHelper::Default (void)
{
  TrafficControlHelper helper;
  uint16_t handle = helper.SetRootQueueDisc ("ns3::PfifoFastQueueDisc");
  helper.AddInternalQueues (handle, 3, "ns3::DropTailQueue", "MaxPackets", UintegerValue (1000));
  helper.AddPacketFilter (handle, "ns3::PfifoFastIpv4PacketFilter");
  helper.AddPacketFilter (handle, "ns3::PfifoFastIpv6PacketFilter");
  return helper;
}

uint16_t
TrafficControlHelper::SetRootQueueDisc (std::string type,
                                        std::string n01, const AttributeValue& v01,
                                        std::string n02, const AttributeValue& v02,
                                        std::string n03, const AttributeValue& v03,
                                        std::string n04, const AttributeValue& v04,
                                        std::string n05, const AttributeValue& v05,
                                        std::string n06, const AttributeValue& v06,
                                        std::string n07, const AttributeValue& v07,
                                        std::string n08, const AttributeValue& v08,
                                        std::string n09, const AttributeValue& v09,
                                        std::string n10, const AttributeValue& v10,
                                        std::string n11, const AttributeValue& v11,
                                        std::string n12, const AttributeValue& v12,
                                        std::string n13, const AttributeValue& v13,
                                        std::string n14, const AttributeValue& v14,
                                        std::string n15, const AttributeValue& v15)
{
  NS_ABORT_MSG_UNLESS (m_queueDiscFactory.empty (), "A root queue disc has been already added to this factory");

  ObjectFactory factory;
  factory.SetTypeId (type);
  factory.Set (n01, v01);
  factory.Set (n02, v02);
  factory.Set (n03, v03);
  factory.Set (n04, v04);
  factory.Set (n05, v05);
  factory.Set (n06, v06);
  factory.Set (n07, v07);
  factory.Set (n08, v08);
  factory.Set (n09, v09);
  factory.Set (n10, v10);
  factory.Set (n11, v11);
  factory.Set (n12, v12);
  factory.Set (n13, v13);
  factory.Set (n14, v14);
  factory.Set (n15, v15);

  m_queueDiscFactory.push_back (QueueDiscFactory (factory));
  return 0;
}

void
TrafficControlHelper::AddInternalQueues (uint16_t handle, uint16_t count, std::string type,
                                         std::string n01, const AttributeValue& v01,
                                         std::string n02, const AttributeValue& v02,
                                         std::string n03, const AttributeValue& v03,
                                         std::string n04, const AttributeValue& v04,
                                         std::string n05, const AttributeValue& v05,
                                         std::string n06, const AttributeValue& v06,
                                         std::string n07, const AttributeValue& v07,
                                         std::string n08, const AttributeValue& v08)
{
  NS_ABORT_MSG_IF (handle >= m_queueDiscFactory.size (), "A queue disc with handle "
                   << handle << " does not exist");

  ObjectFactory factory;
  factory.SetTypeId (type);
  factory.Set (n01, v01);
  factory.Set (n02, v02);
  factory.Set (n03, v03);
  factory.Set (n04, v04);
  factory.Set (n05, v05);
  factory.Set (n06, v06);
  factory.Set (n07, v07);
  factory.Set (n08, v08);

  for (int i = 0; i < count; i++)
    {
      m_queueDiscFactory[handle].AddInternalQueue (factory);
    }
}

void
TrafficControlHelper::AddPacketFilter (uint16_t handle, std::string type,
                                       std::string n01, const AttributeValue& v01,
                                       std::string n02, const AttributeValue& v02,
                                       std::string n03, const AttributeValue& v03,
                                       std::string n04, const AttributeValue& v04,
                                       std::string n05, const AttributeValue& v05,
                                       std::string n06, const AttributeValue& v06,
                                       std::string n07, const AttributeValue& v07,
                                       std::string n08, const AttributeValue& v08)
{
  NS_ABORT_MSG_IF (handle >= m_queueDiscFactory.size (), "A queue disc with handle "
                   << handle << " does not exist");

  ObjectFactory factory;
  factory.SetTypeId (type);
  factory.Set (n01, v01);
  factory.Set (n02, v02);
  factory.Set (n03, v03);
  factory.Set (n04, v04);
  factory.Set (n05, v05);
  factory.Set (n06, v06);
  factory.Set (n07, v07);
  factory.Set (n08, v08);

  m_queueDiscFactory[handle].AddPacketFilter (factory);
}

TrafficControlHelper::ClassIdList
TrafficControlHelper::AddQueueDiscClasses (uint16_t handle, uint16_t count, std::string type,
                                           std::string n01, const AttributeValue& v01,
                                           std::string n02, const AttributeValue& v02,
                                           std::string n03, const AttributeValue& v03,
                                           std::string n04, const AttributeValue& v04,
                                           std::string n05, const AttributeValue& v05,
                                           std::string n06, const AttributeValue& v06,
                                           std::string n07, const AttributeValue& v07,
                                           std::string n08, const AttributeValue& v08)
{
  NS_ABORT_MSG_IF (handle >= m_queueDiscFactory.size (), "A queue disc with handle "
                   << handle << " does not exist");

  ObjectFactory factory;
  factory.SetTypeId (type);
  factory.Set (n01, v01);
  factory.Set (n02, v02);
  factory.Set (n03, v03);
  factory.Set (n04, v04);
  factory.Set (n05, v05);
  factory.Set (n06, v06);
  factory.Set (n07, v07);
  factory.Set (n08, v08);

  ClassIdList list;
  uint16_t classId;

  for (int i = 0; i < count; i++)
    {
      classId = m_queueDiscFactory[handle].AddQueueDiscClass (factory);
      list.push_back (classId);
    }
  return list;
}

uint16_t
TrafficControlHelper::AddChildQueueDisc (uint16_t handle, uint16_t classId, std::string type,
                                         std::string n01, const AttributeValue& v01,
                                         std::string n02, const AttributeValue& v02,
                                         std::string n03, const AttributeValue& v03,
                                         std::string n04, const AttributeValue& v04,
                                         std::string n05, const AttributeValue& v05,
                                         std::string n06, const AttributeValue& v06,
                                         std::string n07, const AttributeValue& v07,
                                         std::string n08, const AttributeValue& v08,
                                         std::string n09, const AttributeValue& v09,
                                         std::string n10, const AttributeValue& v10,
                                         std::string n11, const AttributeValue& v11,
                                         std::string n12, const AttributeValue& v12,
                                         std::string n13, const AttributeValue& v13,
                                         std::string n14, const AttributeValue& v14,
                                         std::string n15, const AttributeValue& v15)
{
  NS_ABORT_MSG_IF (handle >= m_queueDiscFactory.size (), "A queue disc with handle "
                   << handle << " does not exist");

  ObjectFactory factory;
  factory.SetTypeId (type);
  factory.Set (n01, v01);
  factory.Set (n02, v02);
  factory.Set (n03, v03);
  factory.Set (n04, v04);
  factory.Set (n05, v05);
  factory.Set (n06, v06);
  factory.Set (n07, v07);
  factory.Set (n08, v08);
  factory.Set (n09, v09);
  factory.Set (n10, v10);
  factory.Set (n11, v11);
  factory.Set (n12, v12);
  factory.Set (n13, v13);
  factory.Set (n14, v14);
  factory.Set (n15, v15);

  uint16_t childHandle = m_queueDiscFactory.size ();
  m_queueDiscFactory.push_back (QueueDiscFactory (factory));
  m_queueDiscFactory[handle].SetChildQueueDisc (classId, childHandle);

  return childHandle;
}

TrafficControlHelper::HandleList
TrafficControlHelper::AddChildQueueDiscs (uint16_t handle, const TrafficControlHelper::ClassIdList &classes,
                                          std::string type,
                                          std::string n01, const AttributeValue& v01,
                                          std::string n02, const AttributeValue& v02,
                                          std::string n03, const AttributeValue& v03,
                                          std::string n04, const AttributeValue& v04,
                                          std::string n05, const AttributeValue& v05,
                                          std::string n06, const AttributeValue& v06,
                                          std::string n07, const AttributeValue& v07,
                                          std::string n08, const AttributeValue& v08,
                                          std::string n09, const AttributeValue& v09,
                                          std::string n10, const AttributeValue& v10,
                                          std::string n11, const AttributeValue& v11,
                                          std::string n12, const AttributeValue& v12,
                                          std::string n13, const AttributeValue& v13,
                                          std::string n14, const AttributeValue& v14,
                                          std::string n15, const AttributeValue& v15)
{
  HandleList list;
  for (ClassIdList::const_iterator c = classes.begin (); c != classes.end (); c++)
    {
      uint16_t childHandle = AddChildQueueDisc (handle, *c, type, n01, v01, n02, v02, n03, v03,
                                                n04, v04, n05, v05, n06, v06, n07, v07, n08, v08, n09, v09,
                                                n10, v10, n11, v11, n12, v12, n13, v13, n14, v14, n15, v15);
      list.push_back (childHandle);
    }
  return list;
}

QueueDiscContainer
TrafficControlHelper::Install (Ptr<NetDevice> d)
{
  QueueDiscContainer container;

  // A TrafficControlLayer object is aggregated by the InternetStackHelper, but check
  // anyway because a queue disc has no effect without a TrafficControlLayer object
  Ptr<TrafficControlLayer> tc = d->GetNode ()->GetObject<TrafficControlLayer> ();
  NS_ASSERT (tc != 0);

  // Start from an empty vector of queue discs
  m_queueDiscs.clear ();
  m_queueDiscs.resize (m_queueDiscFactory.size ());

  // Create queue discs (from leaves to root)
  for (int i = m_queueDiscFactory.size () - 1; i >= 0; i--)
    {
      Ptr<QueueDisc> q = m_queueDiscFactory[i].CreateQueueDisc (m_queueDiscs);
      q->SetNetDevice (d);
      m_queueDiscs[i] = q;
      container.Add (q);
    }

  // Set the root queue disc on the device
  tc->SetRootQueueDiscOnDevice (d, m_queueDiscs[0]);

  return container;
}

QueueDiscContainer
TrafficControlHelper::Install (NetDeviceContainer c)
{
  QueueDiscContainer container;

  for (NetDeviceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      container.Add (Install (*i));
    }

  return container;
}

void
TrafficControlHelper::Uninstall (Ptr<NetDevice> d)
{
  Ptr<TrafficControlLayer> tc = d->GetNode ()->GetObject<TrafficControlLayer> ();
  NS_ASSERT (tc != 0);

  tc->DeleteRootQueueDiscOnDevice (d);
}

void
TrafficControlHelper::Uninstall (NetDeviceContainer c)
{
  for (NetDeviceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Uninstall (*i);
    }
}


} // namespace ns3
