/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018 Piotr Gawlowicz
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
 * Author: Piotr Gawlowicz <gawlowicz.p@gmail.com>
 *
 */

#include <algorithm>
#include "ns3/log.h"
#include "ns3/type-name.h"
#include "container.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("OpenGymDataContainer");

NS_OBJECT_ENSURE_REGISTERED (OpenGymDataContainer);


TypeId
OpenGymDataContainer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::OpenGymDataContainer")
    .SetParent<Object> ()
    .SetGroupName ("OpenGym")
    .AddConstructor<OpenGymDataContainer> ()
    ;
  return tid;
}

OpenGymDataContainer::OpenGymDataContainer()
{
  //NS_LOG_FUNCTION (this);
}

OpenGymDataContainer::~OpenGymDataContainer ()
{
  //NS_LOG_FUNCTION (this);
}

void
OpenGymDataContainer::DoDispose (void)
{
  //NS_LOG_FUNCTION (this);
}

void
OpenGymDataContainer::DoInitialize (void)
{
  //NS_LOG_FUNCTION (this);
}

void
OpenGymDataContainer::FillDataContainerPbMsg(ns3opengym::DataContainer &dataContainerPbMsg)
{
}

Ptr<OpenGymDataContainer>
OpenGymDataContainer::CreateFromDataContainerPbMsg(ns3opengym::DataContainer &dataContainerPbMsg)
{
  Ptr<OpenGymDataContainer> actDataContainer;

  if (dataContainerPbMsg.type() == ns3opengym::Discrete)
  {
    ns3opengym::DiscreteDataContainer discreteContainerPbMsg;
    dataContainerPbMsg.data().UnpackTo(&discreteContainerPbMsg);

    Ptr<OpenGymDiscreteContainer> discrete = CreateObject<OpenGymDiscreteContainer>();
    discrete->SetValue(discreteContainerPbMsg.data());
    actDataContainer = discrete;
  }
  else if (dataContainerPbMsg.type() == ns3opengym::Box)
  {
    ns3opengym::BoxDataContainer boxContainerPbMsg;
    dataContainerPbMsg.data().UnpackTo(&boxContainerPbMsg);

    if (boxContainerPbMsg.dtype() == ns3opengym::INT) {
      Ptr<OpenGymBoxContainer<int32_t> > box = CreateObject<OpenGymBoxContainer<int32_t> >();
      std::vector<int32_t> myData;
      myData.assign(boxContainerPbMsg.intdata().begin(), boxContainerPbMsg.intdata().end());
      box->SetData(myData);
      actDataContainer = box;

    } else if (boxContainerPbMsg.dtype() == ns3opengym::UINT) {
      Ptr<OpenGymBoxContainer<uint32_t> > box = CreateObject<OpenGymBoxContainer<uint32_t> >();
      std::vector<uint32_t> myData;
      myData.assign(boxContainerPbMsg.uintdata().begin(), boxContainerPbMsg.uintdata().end());
      box->SetData(myData);
      actDataContainer = box;

    } else if (boxContainerPbMsg.dtype() == ns3opengym::FLOAT) {
      Ptr<OpenGymBoxContainer<float> > box = CreateObject<OpenGymBoxContainer<float> >();
      std::vector<float> myData;
      myData.assign(boxContainerPbMsg.floatdata().begin(), boxContainerPbMsg.floatdata().end());
      box->SetData(myData);
      actDataContainer = box;

    } else if (boxContainerPbMsg.dtype() == ns3opengym::DOUBLE) {
      Ptr<OpenGymBoxContainer<double> > box = CreateObject<OpenGymBoxContainer<double> >();
      std::vector<double> myData;
      myData.assign(boxContainerPbMsg.doubledata().begin(), boxContainerPbMsg.doubledata().end());
      box->SetData(myData);
      actDataContainer = box;

    } else {
      Ptr<OpenGymBoxContainer<float> > box = CreateObject<OpenGymBoxContainer<float> >();
      std::vector<float> myData;
      myData.assign(boxContainerPbMsg.floatdata().begin(), boxContainerPbMsg.floatdata().end());
      box->SetData(myData);
      actDataContainer = box;
    }
  }
  return actDataContainer;
}


TypeId
OpenGymDiscreteContainer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::OpenGymDiscreteContainer")
    .SetParent<OpenGymDataContainer> ()
    .SetGroupName ("OpenGym")
    .AddConstructor<OpenGymDiscreteContainer> ()
    ;
  return tid;
}

OpenGymDiscreteContainer::OpenGymDiscreteContainer()
{
  //NS_LOG_FUNCTION (this);
  m_n = 0;
}

OpenGymDiscreteContainer::OpenGymDiscreteContainer(uint32_t n)
{
  //NS_LOG_FUNCTION (this);
  m_n = n;
}

OpenGymDiscreteContainer::~OpenGymDiscreteContainer ()
{
  //NS_LOG_FUNCTION (this);
}

void
OpenGymDiscreteContainer::DoDispose (void)
{
  //NS_LOG_FUNCTION (this);
}

void
OpenGymDiscreteContainer::DoInitialize (void)
{
  //NS_LOG_FUNCTION (this);
}

void
OpenGymDiscreteContainer::FillDataContainerPbMsg(ns3opengym::DataContainer &dataContainerPbMsg)
{
  ns3opengym::DiscreteDataContainer discreteContainerPbMsg;
  discreteContainerPbMsg.set_data(GetValue());

  dataContainerPbMsg.set_type(ns3opengym::Discrete);
  dataContainerPbMsg.mutable_data()->PackFrom(discreteContainerPbMsg);
}

bool
OpenGymDiscreteContainer::SetValue(uint32_t value)
{
  m_value = value;
  return true;
}

uint32_t
OpenGymDiscreteContainer::GetValue()
{
  return m_value;
}

}