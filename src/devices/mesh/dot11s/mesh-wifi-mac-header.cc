/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */


#include "ns3/assert.h"
#include "ns3/address-utils.h"
#include "mesh-wifi-mac-header.h"

namespace ns3 {
namespace dot11s {
/***********************************************************
 *  Here Mesh Mac Header functionality is defined.
 ***********************************************************/
TypeId
WifiMeshHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::WifiMeshHeader")
    .SetParent<Header> ()
    .AddConstructor<WifiMeshHeader> ()
    ;
  return tid;
}
WifiMeshHeader::WifiMeshHeader ():
  m_meshFlags (0),
  m_meshTtl (0),
  m_meshSeqno (0),
  m_addr4 (Mac48Address ()),
  m_addr5 (Mac48Address ()),
  m_addr6 (Mac48Address ())
{
}
WifiMeshHeader::~WifiMeshHeader ()
{
}
TypeId
WifiMeshHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}
void
WifiMeshHeader::SetAddr4 (Mac48Address address)
{
  m_addr4 = address;
}
void
WifiMeshHeader::SetAddr5 (Mac48Address address)
{
  m_addr5 = address;
}
void
WifiMeshHeader::SetAddr6 (Mac48Address address)
{
  m_addr6 = address;
}
Mac48Address
WifiMeshHeader::GetAddr4 () const
{
  return m_addr4;
}
Mac48Address
WifiMeshHeader::GetAddr5 () const
{
  return m_addr5;
}
Mac48Address
WifiMeshHeader::GetAddr6 () const
{
  return m_addr6;
}
void
WifiMeshHeader::SetMeshSeqno (uint32_t seqno)
{
  m_meshSeqno = seqno;
}
uint32_t
WifiMeshHeader::GetMeshSeqno () const
{
  return m_meshSeqno;
}
void
WifiMeshHeader::SetMeshTtl (uint8_t TTL)
{
  m_meshTtl = TTL;
}
uint8_t
WifiMeshHeader::GetMeshTtl () const
{
  return m_meshTtl;
}
void
WifiMeshHeader::SetAddressExt (uint8_t num_of_addresses)
{
  if (num_of_addresses > 3)
    return;
  m_meshFlags |= 0xc0 & (num_of_addresses << 6);
}
uint8_t
WifiMeshHeader::GetAddressExt () const
{
  return ((0xc0 & m_meshFlags) >> 6);
}
uint32_t
WifiMeshHeader::GetSerializedSize () const
{
  return 6 + GetAddressExt () * 6;
}
void
WifiMeshHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteU8 (m_meshFlags);
  i.WriteU8 (m_meshTtl);
  i.WriteU32 (m_meshSeqno);
  uint8_t addresses_to_add = GetAddressExt ();
  //Writing Address extensions:
  if (addresses_to_add > 0)
    WriteTo (i, m_addr4);
  if (addresses_to_add > 1)
    WriteTo (i, m_addr5);
  if (addresses_to_add > 2)
    WriteTo (i, m_addr6);
}
uint32_t
WifiMeshHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint8_t addresses_to_read = 0;
  m_meshFlags = i.ReadU8 ();
  m_meshTtl = i.ReadU8 ();
  m_meshSeqno = i.ReadU32 ();
  addresses_to_read = (m_meshFlags & 0xc0) >> 6;
  if (addresses_to_read > 0)
    ReadFrom (i, m_addr4);
  if (addresses_to_read > 1)
    ReadFrom (i, m_addr5);
  if (addresses_to_read > 2)
    ReadFrom (i, m_addr6);
  return i.GetDistanceFrom (start);
}
void
WifiMeshHeader::Print (std::ostream &os) const
{
  os << "flags" << m_meshFlags
  << "ttl" << m_meshTtl
  << "seqno" << m_meshSeqno;
}
/**********************************************************
 *   MultihopActionFrame
 **********************************************************/
WifiMeshMultihopActionHeader::WifiMeshMultihopActionHeader ()
{
}
WifiMeshMultihopActionHeader::~WifiMeshMultihopActionHeader ()
{
}
void
WifiMeshMultihopActionHeader::SetAction (
  WifiMeshMultihopActionHeader::CategoryValue type,
  WifiMeshMultihopActionHeader::ACTION_VALUE action)
{
  switch (type)
    {
    case MESH_PEER_LINK_MGT:
      m_category = 4;
      switch (action.peerLink)
        {
        case PEER_LINK_OPEN:
          m_actionValue = 0;
          break;
        case PEER_LINK_CONFIRM:
          m_actionValue = 1;
          break;
        case PEER_LINK_CLOSE:
          m_actionValue = 2;
          break;
        };
      break;
    case MESH_LINK_METRIC:
      m_category = 5;
      break;
    case MESH_PATH_SELECTION:
      m_category = 6;
      switch (action.pathSelection)
        {
        case PATH_REQUEST:
          m_actionValue = 0;
          break;
        case PATH_REPLY:
          m_actionValue = 1;
          break;
        case PATH_ERROR:
          m_actionValue = 2;
          break;
        case ROOT_ANNOUNCEMENT:
          m_actionValue = 3;
          break;
        };
      break;
    case MESH_INTERWORK_ACTION:
      m_category = 7;
      break;
    case MESH_RESOURCE_COORDINATION:
      m_category = 8;
      break;
    };
}
WifiMeshMultihopActionHeader::CategoryValue
WifiMeshMultihopActionHeader::GetCategory ()
{
  switch (m_category)
    {
    case 4:
      return MESH_PEER_LINK_MGT;
    case 5:
      return MESH_LINK_METRIC;
    case 6:
      return MESH_PATH_SELECTION;
    case 7:
      return MESH_INTERWORK_ACTION;
    case 8:
      return MESH_RESOURCE_COORDINATION;
    default:
      NS_ASSERT (false);
      return MESH_PEER_LINK_MGT;
    }
}
WifiMeshMultihopActionHeader::ACTION_VALUE
WifiMeshMultihopActionHeader::GetAction ()
{
  ACTION_VALUE retval;
  switch (m_category)
    {
    case 4:
      //MESH_PEER_LINK_MGT;
      switch (m_actionValue)
        {
        case 0:
          retval.peerLink = PEER_LINK_OPEN;
          return retval;
        case 1:
          retval.peerLink = PEER_LINK_CONFIRM;
          return retval;
        case 2:
          retval.peerLink = PEER_LINK_CLOSE;
          return retval;
        default:
          NS_ASSERT (false);
          return retval;

        }
    case 5:
      //MESH_LINK_METRIC;
    case 6:
      //MESH_PATH_SELECTION;
      switch (m_actionValue)
        {
        case 0:
          retval.pathSelection = PATH_REQUEST;
          return retval;
        case 1:
          retval.pathSelection = PATH_REPLY;
          return retval;
        case 2:
          retval.pathSelection = PATH_ERROR;
          return retval;
        case 3:
          retval.pathSelection = ROOT_ANNOUNCEMENT;
          return retval;
        default:
          NS_ASSERT (false);
          return retval;
        }

    case 7:
      //MESH_INTERWORK_ACTION;
    case 8:
      //MESH_RESOURCE_COORDINATION;
    default:
      NS_ASSERT (false);
      return retval;
    }
}
TypeId
WifiMeshMultihopActionHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::WifiMeshMultihopActionHeader")
                      .SetParent<Header> ()
                      .AddConstructor<WifiMeshMultihopActionHeader> ()
                      ;
  return tid;
}
TypeId
WifiMeshMultihopActionHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}
void
WifiMeshMultihopActionHeader::Print (std::ostream &os) const
{
}
uint32_t
WifiMeshMultihopActionHeader::GetSerializedSize () const
{
  return 2;
}
void
WifiMeshMultihopActionHeader::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (m_category);
  start.WriteU8 (m_actionValue);
}
uint32_t
WifiMeshMultihopActionHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_category = i.ReadU8 ();
  m_actionValue = i.ReadU8 ();
  return i.GetDistanceFrom (start);
}
} //namespace dot11s
} // namespace ns3
