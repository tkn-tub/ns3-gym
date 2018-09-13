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
#include "ns3/node.h"
#include "spaces.h"
#include <boost/algorithm/string.hpp>


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("OpenGymSpace");

NS_OBJECT_ENSURE_REGISTERED (OpenGymSpace);


TypeId
OpenGymSpace::GetTypeId (void)
{
  static TypeId tid = TypeId ("OpenGymSpace")
    .SetParent<Object> ()
    .SetGroupName ("OpenGym")
    ;
  return tid;
}

OpenGymSpace::OpenGymSpace()
{
  NS_LOG_FUNCTION (this);
}

OpenGymSpace::~OpenGymSpace ()
{
  NS_LOG_FUNCTION (this);
}

void
OpenGymSpace::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
}

void
OpenGymSpace::DoInitialize (void)
{
  NS_LOG_FUNCTION (this);
}



TypeId
OpenGymDiscreteSpace::GetTypeId (void)
{
  static TypeId tid = TypeId ("OpenGymDiscreteSpace")
    .SetParent<OpenGymSpace> ()
    .SetGroupName ("OpenGym")
    .AddConstructor<OpenGymDiscreteSpace> ()
    ;
  return tid;
}

OpenGymDiscreteSpace::OpenGymDiscreteSpace()
{
  NS_LOG_FUNCTION (this);
}

OpenGymDiscreteSpace::OpenGymDiscreteSpace(int n):
  m_n(n)
{
  NS_LOG_FUNCTION (this);
}

OpenGymDiscreteSpace::~OpenGymDiscreteSpace ()
{
  NS_LOG_FUNCTION (this);
}

void
OpenGymDiscreteSpace::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
}

void
OpenGymDiscreteSpace::DoInitialize (void)
{
  NS_LOG_FUNCTION (this);
}

int
OpenGymDiscreteSpace::GetN (void)
{
  NS_LOG_FUNCTION (this);
  return m_n;
}

std::string
OpenGymDiscreteSpace::Serialize (void)
{
  NS_LOG_FUNCTION (this);
  std::string serialized = "SpaceType=0;N=" + std::to_string(m_n);
  return serialized;
}

std::ostream& operator<< (std::ostream& os, const OpenGymDiscreteSpace& space)  
{  
  os << " DiscreteSpace N: " << space.m_n;  
  return os;  
}  



TypeId
OpenGymBoxSpace::GetTypeId (void)
{
  static TypeId tid = TypeId ("OpenGymBoxSpace")
    .SetParent<OpenGymSpace> ()
    .SetGroupName ("OpenGym")
    .AddConstructor<OpenGymBoxSpace> ()
    ;
  return tid;
}

OpenGymBoxSpace::OpenGymBoxSpace ()
{
  NS_LOG_FUNCTION (this);
}

OpenGymBoxSpace::OpenGymBoxSpace (float low, float high, std::vector<uint32_t> shape, std::string dtype):
  m_low(low), m_high(high), m_shape(shape), m_dtypeName(dtype)
{
  NS_LOG_FUNCTION (this);
  SetDtype ();
}

OpenGymBoxSpace::OpenGymBoxSpace (std::vector<float> low, std::vector<float> high, std::vector<uint32_t> shape, std::string dtype):
  m_low(0), m_high(0), m_shape(shape), m_dtypeName(dtype), m_lowVec(low), m_highVec(high)

{
  NS_LOG_FUNCTION (this);
  SetDtype ();
} 

OpenGymBoxSpace::~OpenGymBoxSpace ()
{
  NS_LOG_FUNCTION (this);
}

void
OpenGymBoxSpace::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
}

void
OpenGymBoxSpace::DoInitialize (void)
{
  NS_LOG_FUNCTION (this);
}

void
OpenGymBoxSpace::SetDtype ()
{
  std::string name = m_dtypeName;
  if (name == "int8_t" || name == "int16_t" || name == "int32_t" || name == "int64_t")
    m_dtype = Dtype::INT;
  else if (name == "uint8_t" || name == "uint16_t" || name == "uint32_t" || name == "uint64_t")
    m_dtype = Dtype::UINT;
  else if (name == "float" || name == "double")
    m_dtype = Dtype::FLOAT;
  else
    m_dtype = Dtype::FLOAT;
}

float
OpenGymBoxSpace::GetLow()
{
  NS_LOG_FUNCTION (this);
  return m_low;
}

float
OpenGymBoxSpace::GetHigh()
{
  NS_LOG_FUNCTION (this);
  return m_high;
}

std::vector<uint32_t>
OpenGymBoxSpace::GetShape()
{
  NS_LOG_FUNCTION (this);
  return m_shape;
}

Dtype
OpenGymBoxSpace::GetDtype()
{
  NS_LOG_FUNCTION (this);
  return m_dtype;
}

std::string
OpenGymBoxSpace::Serialize (void)
{
  NS_LOG_FUNCTION (this);
  std::string serialized = "SpaceType=1;Low=" + std::to_string(m_low)
                         + ";High=" + std::to_string(m_high)
                         + ";Shape=(";

  for (auto i = m_shape.begin(); i != m_shape.end(); ++i)
  {
    serialized += std::to_string(*i) +",";
  }
  serialized += ") Dtype: " + std::to_string(m_dtype);

  return serialized;
}

std::ostream& operator<< (std::ostream& os, const OpenGymBoxSpace& box)  
{  
  os << " BoxSpace Low: " << box.m_low << " High: " << box.m_high << " Shape: (";

  for (auto i = box.m_shape.begin(); i != box.m_shape.end(); ++i)
  {
    os << *i << ",";
  }
  os << ") Dtype: ";

  if (box.m_dtype == Dtype::INT)
  {
    os << "INT";
  } else if (box.m_dtype == Dtype::UINT) {
    os << "UINT";
  } else if (box.m_dtype == Dtype::DOUBLE) {
    os << "DOUBLE";
  } else {
    os << "FLOAT";
  }

  return os;  
}  

}