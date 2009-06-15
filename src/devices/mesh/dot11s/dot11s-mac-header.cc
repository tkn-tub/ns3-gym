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
#include "dot11s-mac-header.h"
#include "ns3/packet.h"
#include "ns3/test.h"

namespace ns3 {
namespace dot11s {
/***********************************************************
 *  Here Mesh Mac Header functionality is defined.
 ***********************************************************/
TypeId
MeshHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Dot11sMacHeader")
    .SetParent<Header> ()
    .AddConstructor<MeshHeader> ()
    ;
  return tid;
}
MeshHeader::MeshHeader ():
  m_meshFlags (0),
  m_meshTtl (0),
  m_meshSeqno (0),
  m_addr4 (Mac48Address ()),
  m_addr5 (Mac48Address ()),
  m_addr6 (Mac48Address ())
{
}
MeshHeader::~MeshHeader ()
{
}
TypeId
MeshHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}
void
MeshHeader::SetAddr4 (Mac48Address address)
{
  m_addr4 = address;
}
void
MeshHeader::SetAddr5 (Mac48Address address)
{
  m_addr5 = address;
}
void
MeshHeader::SetAddr6 (Mac48Address address)
{
  m_addr6 = address;
}
Mac48Address
MeshHeader::GetAddr4 () const
{
  return m_addr4;
}
Mac48Address
MeshHeader::GetAddr5 () const
{
  return m_addr5;
}
Mac48Address
MeshHeader::GetAddr6 () const
{
  return m_addr6;
}
void
MeshHeader::SetMeshSeqno (uint32_t seqno)
{
  m_meshSeqno = seqno;
}
uint32_t
MeshHeader::GetMeshSeqno () const
{
  return m_meshSeqno;
}
void
MeshHeader::SetMeshTtl (uint8_t TTL)
{
  m_meshTtl = TTL;
}
uint8_t
MeshHeader::GetMeshTtl () const
{
  return m_meshTtl;
}
void
MeshHeader::SetAddressExt (uint8_t num_of_addresses)
{
  NS_ASSERT (num_of_addresses <= 3);
  m_meshFlags |= 0x03 & num_of_addresses;
}
uint8_t
MeshHeader::GetAddressExt () const
{
  return (0x03 & m_meshFlags);
}
uint32_t
MeshHeader::GetSerializedSize () const
{
  return 6 + GetAddressExt () * 6;
}
void
MeshHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteU8 (m_meshFlags);
  i.WriteU8 (m_meshTtl);
  i.WriteHtolsbU32 (m_meshSeqno);
  uint8_t addresses_to_add = GetAddressExt ();
  //Writing Address extensions:
  if ((addresses_to_add == 1) || (addresses_to_add == 3))
    WriteTo (i, m_addr4);
  if (addresses_to_add > 1)
    WriteTo (i, m_addr5);
  if (addresses_to_add > 1)
    WriteTo (i, m_addr6);
}
uint32_t
MeshHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint8_t addresses_to_read = 0;
  m_meshFlags = i.ReadU8 ();
  m_meshTtl = i.ReadU8 ();
  m_meshSeqno = i.ReadLsbtohU32 ();
  addresses_to_read = m_meshFlags & 0x03;
  if ((addresses_to_read == 1) || (addresses_to_read == 3))
    ReadFrom (i, m_addr4);
  if (addresses_to_read > 1)
    ReadFrom (i, m_addr5);
  if (addresses_to_read > 1)
    ReadFrom (i, m_addr6);
  return i.GetDistanceFrom (start);
}
void
MeshHeader::Print (std::ostream &os) const
{
  os << "flags = " << (uint16_t)m_meshFlags
  << "\nttl = " << (uint16_t)m_meshTtl
  << "\nseqno = " << m_meshSeqno
  << "\naddr4 = " << m_addr4
  << "\naddr5 = " << m_addr5
  << "\naddr6 = " << m_addr6;
}
bool operator== (const MeshHeader & a, const MeshHeader & b)
{
  return (
      (a.m_meshFlags == b.m_meshFlags) &&
      (a.m_meshTtl == b.m_meshTtl) &&
      (a.m_meshSeqno == b.m_meshSeqno) &&
      (a.m_addr4 == b.m_addr4) &&
      (a.m_addr5 == b.m_addr5) &&
      (a.m_addr6 == b.m_addr6)
      );
}
/**********************************************************
 *   ActionFrame
 **********************************************************/
WifiMeshActionHeader::WifiMeshActionHeader ()
{
}
WifiMeshActionHeader::~WifiMeshActionHeader ()
{
}
void
WifiMeshActionHeader::SetAction (
  WifiMeshActionHeader::CategoryValue type,
  WifiMeshActionHeader::ActionValue action)
{
  m_category = type;
  
  switch (type)
    {
    case MESH_PEERING_MGT:
      {
        m_actionValue = action.peerLink;
        break;
      }
    case MESH_PATH_SELECTION:
      {
        m_actionValue = action.pathSelection;
        break;
      }
    case MESH_LINK_METRIC:
    case MESH_INTERWORKING:
    case MESH_RESOURCE_COORDINATION:
    case MESH_PROXY_FORWARDING:
      break;
    };
}
WifiMeshActionHeader::CategoryValue
WifiMeshActionHeader::GetCategory ()
{
  switch (m_category)
    {
    case MESH_PEERING_MGT:
      return MESH_PEERING_MGT;
    case MESH_LINK_METRIC:
      return MESH_LINK_METRIC;
    case MESH_PATH_SELECTION:
      return MESH_PATH_SELECTION;
    case MESH_INTERWORKING:
      return MESH_INTERWORKING;
    case MESH_RESOURCE_COORDINATION:
      return MESH_RESOURCE_COORDINATION;
    case MESH_PROXY_FORWARDING:
      return MESH_PROXY_FORWARDING;
    default:
      NS_FATAL_ERROR ("Unknown action value");
      return MESH_PEERING_MGT;
    }
}
WifiMeshActionHeader::ActionValue
WifiMeshActionHeader::GetAction ()
{
  ActionValue retval;
  switch (m_category)
    {
    case MESH_PEERING_MGT:
      switch (m_actionValue)
        {
        case PEER_LINK_OPEN:
          retval.peerLink = PEER_LINK_OPEN;
          return retval;
        case PEER_LINK_CONFIRM:
          retval.peerLink = PEER_LINK_CONFIRM;
          return retval;
        case PEER_LINK_CLOSE:
          retval.peerLink = PEER_LINK_CLOSE;
          return retval;
        default:
          NS_FATAL_ERROR ("Unknown mesh peering management action code");
          return retval;
        }
    case MESH_PATH_SELECTION:
      switch (m_actionValue)
        {
        case PATH_REQUEST:
          retval.pathSelection = PATH_REQUEST;
          return retval;
        case PATH_REPLY:
          retval.pathSelection = PATH_REPLY;
          return retval;
        case PATH_ERROR:
          retval.pathSelection = PATH_ERROR;
          return retval;
        case ROOT_ANNOUNCEMENT:
          retval.pathSelection = ROOT_ANNOUNCEMENT;
          return retval;
        default:
          NS_FATAL_ERROR ("Unknown mesh path selection action code");
          return retval;
        }
    case MESH_LINK_METRIC:
      // not yet supported 
    case MESH_INTERWORKING:
      // not yet supported
    case MESH_RESOURCE_COORDINATION:
      // not yet supported
    default:
      NS_FATAL_ERROR ("Unsupported mesh action");
      return retval;
    }
}
TypeId
WifiMeshActionHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::WifiMeshActionHeader")
                      .SetParent<Header> ()
                      .AddConstructor<WifiMeshActionHeader> ()
                      ;
  return tid;
}
TypeId
WifiMeshActionHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}
void
WifiMeshActionHeader::Print (std::ostream &os) const
{
}
uint32_t
WifiMeshActionHeader::GetSerializedSize () const
{
  return 2;
}
void
WifiMeshActionHeader::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (m_category);
  start.WriteU8 (m_actionValue);
}
uint32_t
WifiMeshActionHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_category = i.ReadU8 ();
  m_actionValue = i.ReadU8 ();
  return i.GetDistanceFrom (start);
}
#ifdef RUN_SELF_TESTS

/// Built-in self test for Dot11sMacHeader
struct Dot11sMacHeaderBist : public Test 
{
  Dot11sMacHeaderBist () : Test ("Mesh/802.11s/MeshHeader") {}
  virtual bool RunTests(); 
};

/// Test instance
static Dot11sMacHeaderBist g_Dot11sMacHeaderBist;

bool Dot11sMacHeaderBist::RunTests ()
{
  bool result (true);
  {
    MeshHeader a;
    a.SetAddressExt(3);
    a.SetAddr4(Mac48Address ("11:22:33:44:55:66"));
    a.SetAddr5(Mac48Address ("11:00:33:00:55:00"));
    a.SetAddr6(Mac48Address ("00:22:00:44:00:66"));
    a.SetMeshTtl (122);
    a.SetMeshSeqno (321);
    Ptr<Packet> packet = Create<Packet> ();
    packet->AddHeader (a);
    MeshHeader b;
    packet->RemoveHeader (b);
    NS_TEST_ASSERT_EQUAL (a, b);
  } 
  {
    MeshHeader a;
    a.SetAddressExt(2);
    a.SetAddr5(Mac48Address ("11:00:33:00:55:00"));
    a.SetAddr6(Mac48Address ("00:22:00:44:00:66"));
    a.SetMeshTtl (122);
    a.SetMeshSeqno (321);
    Ptr<Packet> packet = Create<Packet> ();
    packet->AddHeader (a);
    MeshHeader b;
    packet->RemoveHeader (b);
    NS_TEST_ASSERT_EQUAL (a, b);
  }
  {
    MeshHeader a;
    a.SetAddressExt(1);
    a.SetAddr4(Mac48Address ("11:22:33:44:55:66"));
    a.SetMeshTtl (122);
    a.SetMeshSeqno (321);
    Ptr<Packet> packet = Create<Packet> ();
    packet->AddHeader (a);
    MeshHeader b;
    packet->RemoveHeader (b);
    NS_TEST_ASSERT_EQUAL (a, b);
  }
  return result;
}
#endif
} //namespace dot11s
} // namespace ns3
