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

#ifndef OPENGYM_CONTAINER_H
#define OPENGYM_CONTAINER_H

#include "ns3/object.h"
#include "ns3/simulator.h"
#include "ns3/type-name.h"
#include <map>

#include "spaces.h"
#include "messages.pb.h"

namespace ns3 {

class OpenGymDataContainer : public Object
{
public:
  OpenGymDataContainer ();
  virtual ~OpenGymDataContainer ();

  static TypeId GetTypeId ();

  virtual void FillDataContainerPbMsg(ns3opengym::DataContainer &dataContainer);
  static Ptr<OpenGymDataContainer> CreateFromDataContainerPbMsg(ns3opengym::DataContainer &dataContainer);

protected:
  // Inherited
  virtual void DoInitialize (void);
  virtual void DoDispose (void);

  Dtype m_dtype;
};


class OpenGymDiscreteContainer : public OpenGymDataContainer
{
public:
  OpenGymDiscreteContainer ();
  OpenGymDiscreteContainer (uint32_t n);
  virtual ~OpenGymDiscreteContainer ();

  static TypeId GetTypeId ();

  virtual void FillDataContainerPbMsg(ns3opengym::DataContainer &dataContainer);

  bool SetValue(uint32_t value);
  uint32_t GetValue();

protected:
  // Inherited
  virtual void DoInitialize (void);
  virtual void DoDispose (void);

  Dtype m_dtype;
  uint32_t m_n;
  uint32_t m_value;
};

template <typename T = float>
class OpenGymBoxContainer : public OpenGymDataContainer
{
public:
  OpenGymBoxContainer ();
  OpenGymBoxContainer (std::vector<uint32_t> shape);
  virtual ~OpenGymBoxContainer ();

  static TypeId GetTypeId ();

  virtual void FillDataContainerPbMsg(ns3opengym::DataContainer &dataContainer);

  bool AddValue(T value);
  bool SetData(std::vector<T> data);
  std::vector<uint32_t> GetShape();
  std::vector<T> GetData();

protected:
  // Inherited
  virtual void DoInitialize (void);
  virtual void DoDispose (void);

private:
  void SetDtype();
	std::vector<uint32_t> m_shape;
	Dtype m_dtype;
	std::vector<T> m_data;
};

template <typename T>
TypeId
OpenGymBoxContainer<T>::GetTypeId (void)
{
  std::string name = TypeNameGet<T> ();
  static TypeId tid = TypeId (("ns3::OpenGymBoxContainer<" + name + ">").c_str ())
    .SetParent<Object> ()
    .SetGroupName ("OpenGym")
    .template AddConstructor<OpenGymBoxContainer<T> > ()
    ;
  return tid;
}

template <typename T>
OpenGymBoxContainer<T>::OpenGymBoxContainer()
{
 SetDtype();
}

template <typename T>
OpenGymBoxContainer<T>::OpenGymBoxContainer(std::vector<uint32_t> shape):
	m_shape(shape)
{
  SetDtype();
}

template <typename T>
OpenGymBoxContainer<T>::~OpenGymBoxContainer ()
{
}

template <typename T>
void
OpenGymBoxContainer<T>::SetDtype ()
{
  std::string name = TypeNameGet<T> ();
  if (name == "int8_t" || name == "int16_t" || name == "int32_t" || name == "int64_t") 
    m_dtype = Dtype::INT;
  else if (name == "uint8_t" || name == "uint16_t" || name == "uint32_t" || name == "uint64_t") 
    m_dtype = Dtype::UINT;
  else if (name == "float" || name == "double") 
    m_dtype = Dtype::FLOAT;
  else
    m_dtype = Dtype::FLOAT;
}

template <typename T>
void
OpenGymBoxContainer<T>::DoDispose (void)
{
}

template <typename T>
void
OpenGymBoxContainer<T>::DoInitialize (void)
{
}

template <typename T>
void
OpenGymBoxContainer<T>::FillDataContainerPbMsg(ns3opengym::DataContainer &dataContainerPbMsg)
{
  ns3opengym::BoxDataContainer boxContainerPbMsg;

  std::vector<uint32_t> shape = GetShape();
  *boxContainerPbMsg.mutable_shape() = {shape.begin(), shape.end()};

  Dtype dtype = m_dtype;
  std::vector<T> data = GetData();

  if (dtype == Dtype::INT) {
    boxContainerPbMsg.set_dtype(ns3opengym::INT);
    *boxContainerPbMsg.mutable_intdata() = {data.begin(), data.end()};
  } else if (dtype == Dtype::UINT) {
    boxContainerPbMsg.set_dtype(ns3opengym::UINT);
    *boxContainerPbMsg.mutable_uintdata() = {data.begin(), data.end()};
  } else if (dtype == Dtype::FLOAT) {
    boxContainerPbMsg.set_dtype(ns3opengym::FLOAT);
    *boxContainerPbMsg.mutable_floatdata() = {data.begin(), data.end()};
  } else {
    boxContainerPbMsg.set_dtype(ns3opengym::FLOAT);
    *boxContainerPbMsg.mutable_floatdata() = {data.begin(), data.end()};
  }

  dataContainerPbMsg.set_type(ns3opengym::Box);
  dataContainerPbMsg.mutable_data()->PackFrom(boxContainerPbMsg);
}

template <typename T>
bool
OpenGymBoxContainer<T>::AddValue(T value)
{
  m_data.push_back(value);
  return true;
}

template <typename T>
bool
OpenGymBoxContainer<T>::SetData(std::vector<T> data)
{
  m_data = data;
  return true;
}

template <typename T>
std::vector<uint32_t>
OpenGymBoxContainer<T>::GetShape()
{
  return m_shape;
}

template <typename T>
std::vector<T>
OpenGymBoxContainer<T>::GetData()
{
  return m_data;
}

} // end of namespace ns3

#endif /* OPENGYM_CONTAINER_H */

