/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
 * Copyright (c) 2013 Dalian University of Technology
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
 *         Junling Bu <linlinjavaer@gmail.com>
 */
#include "higher-tx-tag.h"
#include "ns3/tag.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"

NS_LOG_COMPONENT_DEFINE ("HigherDataTxVectorTag");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (HigherDataTxVectorTag);

TypeId
HigherDataTxVectorTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::HigherDataTxVectorTag")
    .SetParent<Tag> ()
    .AddConstructor<HigherDataTxVectorTag> ()
  ;
  return tid;
}
HigherDataTxVectorTag::HigherDataTxVectorTag (void)
  : m_adapter (false)
{
  NS_LOG_FUNCTION (this);
}
HigherDataTxVectorTag::HigherDataTxVectorTag (WifiTxVector dataTxVector, bool adapter)
  : m_dataTxVector (dataTxVector),
    m_adapter (adapter)
{
  NS_LOG_FUNCTION (this);
}
HigherDataTxVectorTag::~HigherDataTxVectorTag (void)
{
  NS_LOG_FUNCTION (this);
}
TypeId
HigherDataTxVectorTag::GetInstanceTypeId (void) const
{
  NS_LOG_FUNCTION (this);
  return GetTypeId ();
}

WifiTxVector
HigherDataTxVectorTag::GetDataTxVector (void) const
{
  NS_LOG_FUNCTION (this);
  return m_dataTxVector;
}
bool
HigherDataTxVectorTag::IsAdapter (void) const
{
  NS_LOG_FUNCTION (this);
  return m_adapter;
}

uint32_t
HigherDataTxVectorTag::GetSerializedSize (void) const
{
  NS_LOG_FUNCTION (this);
  return (sizeof (WifiTxVector) + 1);
}
void
HigherDataTxVectorTag::Serialize (TagBuffer i) const
{
  NS_LOG_FUNCTION (this << &i);
  i.Write ((uint8_t *)&m_dataTxVector, sizeof (WifiTxVector));
  i.WriteU8 (static_cast<uint8_t> (m_adapter));
}
void
HigherDataTxVectorTag::Deserialize (TagBuffer i)
{
  NS_LOG_FUNCTION (this << &i);
  i.Read ((uint8_t *)&m_dataTxVector, sizeof (WifiTxVector));
  m_adapter = i.ReadU8 ();
}
void
HigherDataTxVectorTag::Print (std::ostream &os) const
{
  NS_LOG_FUNCTION (this << &os);
  os << " Data=" << m_dataTxVector << " Adapter=" << m_adapter;
}

} // namespace ns3
