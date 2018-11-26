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

#include "ns3/log.h"
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
  else if (dataContainerPbMsg.type() == ns3opengym::Tuple)
  {
    Ptr<OpenGymTupleContainer> tupleData = CreateObject<OpenGymTupleContainer> ();

    ns3opengym::TupleDataContainer tupleContainerPbMsg;
    dataContainerPbMsg.data().UnpackTo(&tupleContainerPbMsg);

    std::vector< ns3opengym::DataContainer > elements;
    elements.assign(tupleContainerPbMsg.element().begin(), tupleContainerPbMsg.element().end());

    std::vector< ns3opengym::DataContainer >::iterator it;
    for(it=elements.begin();it!=elements.end();++it)
    {
      Ptr<OpenGymDataContainer> subData = OpenGymDataContainer::CreateFromDataContainerPbMsg(*it);
      tupleData->Add(subData);
    }

    actDataContainer = tupleData;
  }
  else if (dataContainerPbMsg.type() == ns3opengym::Dict)
  {
    Ptr<OpenGymDictContainer> dictData = CreateObject<OpenGymDictContainer> ();

    ns3opengym::DictDataContainer dictContainerPbMsg;
    dataContainerPbMsg.data().UnpackTo(&dictContainerPbMsg);

    std::vector< ns3opengym::DataContainer > elements;
    elements.assign(dictContainerPbMsg.element().begin(), dictContainerPbMsg.element().end());

    std::vector< ns3opengym::DataContainer >::iterator it;
    for(it=elements.begin();it!=elements.end();++it)
    {
      Ptr<OpenGymDataContainer> subSpace = OpenGymDataContainer::CreateFromDataContainerPbMsg(*it);
      dictData->Add((*it).name(), subSpace);
    }

    actDataContainer = dictData;
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

ns3opengym::DataContainer
OpenGymDiscreteContainer::GetDataContainerPbMsg()
{
  ns3opengym::DataContainer dataContainerPbMsg;
  ns3opengym::DiscreteDataContainer discreteContainerPbMsg;
  discreteContainerPbMsg.set_data(GetValue());

  dataContainerPbMsg.set_type(ns3opengym::Discrete);
  dataContainerPbMsg.mutable_data()->PackFrom(discreteContainerPbMsg);
  return dataContainerPbMsg;
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

void
OpenGymDiscreteContainer::Print(std::ostream& where) const
{
  where << std::to_string(m_value);
}

TypeId
OpenGymTupleContainer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::OpenGymTupleContainer")
    .SetParent<OpenGymDataContainer> ()
    .SetGroupName ("OpenGym")
    .AddConstructor<OpenGymTupleContainer> ()
    ;
  return tid;
}

OpenGymTupleContainer::OpenGymTupleContainer()
{
  //NS_LOG_FUNCTION (this);
}

OpenGymTupleContainer::~OpenGymTupleContainer ()
{
  //NS_LOG_FUNCTION (this);
}

void
OpenGymTupleContainer::DoDispose (void)
{
  //NS_LOG_FUNCTION (this);
}

void
OpenGymTupleContainer::DoInitialize (void)
{
  //NS_LOG_FUNCTION (this);
}

ns3opengym::DataContainer
OpenGymTupleContainer::GetDataContainerPbMsg()
{
  ns3opengym::DataContainer dataContainerPbMsg;
  dataContainerPbMsg.set_type(ns3opengym::Tuple);

  ns3opengym::TupleDataContainer tupleContainerPbMsg;

  std::vector< Ptr<OpenGymDataContainer> >::iterator it;
  for (it=m_tuple.begin(); it!=m_tuple.end(); ++it)
  {
    Ptr<OpenGymDataContainer> subSpace = *it;
    ns3opengym::DataContainer subDataContainer = subSpace->GetDataContainerPbMsg();

    tupleContainerPbMsg.add_element()->CopyFrom(subDataContainer);
  }

  dataContainerPbMsg.mutable_data()->PackFrom(tupleContainerPbMsg);
  return dataContainerPbMsg;
}

bool
OpenGymTupleContainer::Add(Ptr<OpenGymDataContainer> space)
{
  NS_LOG_FUNCTION (this);
  m_tuple.push_back(space);
  return true;
}

Ptr<OpenGymDataContainer>
OpenGymTupleContainer::Get(uint32_t idx)
{
  Ptr<OpenGymDataContainer> data;

  if (idx < m_tuple.size())
  {
    data = m_tuple.at(idx);
  }

  return data;
}

void
OpenGymTupleContainer::Print(std::ostream& where) const
{
  where << "Tuple(";

  std::vector< Ptr<OpenGymDataContainer> >::const_iterator it;
  std::vector< Ptr<OpenGymDataContainer> >::const_iterator it2;
  for (it=m_tuple.cbegin(); it!=m_tuple.cend(); ++it)
  {
    Ptr<OpenGymDataContainer> subSpace = *it;
    subSpace->Print(where);

    it2 = it;
    it2++;
    if (it2 != m_tuple.end())
      where << ", ";
  }
  where << ")";
}


TypeId
OpenGymDictContainer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::OpenGymDictContainer")
    .SetParent<OpenGymDataContainer> ()
    .SetGroupName ("OpenGym")
    .AddConstructor<OpenGymDictContainer> ()
    ;
  return tid;
}

OpenGymDictContainer::OpenGymDictContainer()
{
  //NS_LOG_FUNCTION (this);
}

OpenGymDictContainer::~OpenGymDictContainer ()
{
  //NS_LOG_FUNCTION (this);
}

void
OpenGymDictContainer::DoDispose (void)
{
  //NS_LOG_FUNCTION (this);
}

void
OpenGymDictContainer::DoInitialize (void)
{
  //NS_LOG_FUNCTION (this);
}

ns3opengym::DataContainer
OpenGymDictContainer::GetDataContainerPbMsg()
{
  ns3opengym::DataContainer dataContainerPbMsg;
  dataContainerPbMsg.set_type(ns3opengym::Dict);

  ns3opengym::DictDataContainer dictContainerPbMsg;

  std::map< std::string, Ptr<OpenGymDataContainer> >::iterator it;
  for (it=m_dict.begin(); it!=m_dict.end(); ++it)
  {
    std::string name = it->first;
    Ptr<OpenGymDataContainer> subSpace = it->second;

    ns3opengym::DataContainer subDataContainer = subSpace->GetDataContainerPbMsg();
    subDataContainer.set_name(name);

    dictContainerPbMsg.add_element()->CopyFrom(subDataContainer);
  }

  dataContainerPbMsg.mutable_data()->PackFrom(dictContainerPbMsg);
  return dataContainerPbMsg;
}

bool
OpenGymDictContainer::Add(std::string key, Ptr<OpenGymDataContainer> data)
{
  NS_LOG_FUNCTION (this);
  m_dict.insert(std::pair<std::string, Ptr<OpenGymDataContainer> > (key, data));
  return true;
}

Ptr<OpenGymDataContainer>
OpenGymDictContainer::Get(std::string key)
{
  Ptr<OpenGymDataContainer> data;
  std::map< std::string, Ptr<OpenGymDataContainer> >::iterator it = m_dict.find(key);
  if ( it != m_dict.end() ) {
    data = it->second;
  }
  return data;
}

void
OpenGymDictContainer::Print(std::ostream& where) const
{
  where << "Dict(";

  std::map< std::string, Ptr<OpenGymDataContainer> >::const_iterator it;
  std::map< std::string, Ptr<OpenGymDataContainer> >::const_iterator it2;
  for (it=m_dict.cbegin(); it!=m_dict.cend(); ++it)
  {
    std::string name = it->first;
    Ptr<OpenGymDataContainer> subSpace = it->second;

    where << name << "=";
    subSpace->Print(where);

    it2 = it;
    it2++;
    if (it2 != m_dict.end())
      where << ", ";
  }
  where << ")";
}

}