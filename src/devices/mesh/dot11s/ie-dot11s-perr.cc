/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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


#include "ie-dot11s-perr.h"
#include "ns3/address-utils.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/test.h"
namespace ns3 {
namespace dot11s {
IePerr::~IePerr ()
{
}
void
IePerr::PrintInformation (std::ostream &os) const
{
  // TODO
}
IePerr::IePerr ():
    m_numOfDest (0)
{
}
uint8_t
IePerr::GetNumOfDest ()
{
  return m_numOfDest;
}
void
IePerr::SerializeInformation (Buffer::Iterator i) const
{
  i.WriteU8 (0);
  i.WriteU8 (m_numOfDest);
  NS_ASSERT (m_numOfDest == m_addressUnits.size ());
  for (unsigned int j = 0; j < m_numOfDest; j++)
    {
      WriteTo (i, m_addressUnits[j].destination);
      i.WriteHtolsbU32 (m_addressUnits[j].seqnum);
    }
}
uint8_t
IePerr::DeserializeInformation (Buffer::Iterator start, uint8_t length)
{
  Buffer::Iterator i = start;
  i.Next (1); //Mode flags is not used now
  m_numOfDest = i.ReadU8 ();
  NS_ASSERT ((2+10*m_numOfDest) == length);
  length = 0; //to avoid compiler warning in optimized builds
  for (unsigned int j = 0; j < m_numOfDest; j++)
    {
      FailedDestination unit;
      ReadFrom (i,unit.destination);
      unit.seqnum = i.ReadLsbtohU32 ();
      m_addressUnits.push_back (unit);
    }
  return i.GetDistanceFrom (start);
}

uint8_t
IePerr::GetInformationSize () const
{
  uint8_t retval =
     1 //ModeFlags
    +1 //NumOfDests
    +6*m_numOfDest
    +4*m_numOfDest;
  return retval;
}

void
IePerr::AddAddressUnit (FailedDestination unit)
{
  for (unsigned int i = 0; i < m_addressUnits.size (); i ++)
    if (m_addressUnits[i].destination == unit.destination)
      return;
  m_addressUnits.push_back (unit);
  m_numOfDest++;
}

std::vector<IePerr::FailedDestination>
IePerr::GetAddressUnitVector () const
{
  return m_addressUnits;
}
void
IePerr::DeleteAddressUnit (Mac48Address address)
{
  for (std::vector<FailedDestination>::iterator i = m_addressUnits.begin (); i != m_addressUnits.end(); i ++)
    if (i->destination == address)
      {
        m_numOfDest --;
        m_addressUnits.erase (i);
        break;
      }
}
void
IePerr::Merge(const IePerr perr)
{
  std::vector<FailedDestination> to_merge = perr.GetAddressUnitVector ();
  for (std::vector<FailedDestination>::iterator i = to_merge.begin (); i != to_merge.end(); i ++)
  {
    bool should_add = true;
    for (std::vector<FailedDestination>::iterator j = m_addressUnits.begin (); j != m_addressUnits.end(); j ++)
      if ((i->destination == j->destination) && (i->seqnum <= j->seqnum))
        should_add = false;
    if(should_add)
      AddAddressUnit (*i);
  }
}
void
IePerr::ResetPerr ()
{
  m_numOfDest = 0;
  m_addressUnits.clear ();
}
bool operator== (const IePerr & a, const IePerr & b)
{
  if(a.m_numOfDest != b.m_numOfDest)
    return false;
  for(unsigned int i = 0; i < a.m_addressUnits.size(); i ++)
  {
    if(a.m_addressUnits[i].destination != b.m_addressUnits[i].destination)
      return false;
    if(a.m_addressUnits[i].seqnum != b.m_addressUnits[i].seqnum)
      return false;
  }
  return true;
}
#ifdef RUN_SELF_TESTS

/// Built-in self test for IePreq
struct IePerrBist : public IeTest 
{
  IePerrBist () : IeTest ("Mesh/802.11s/IE/PERR") {}
  virtual bool RunTests(); 
};

/// Test instance
static IePerrBist g_IePerrBist;

bool IePerrBist::RunTests ()
{
  bool result(true);
  // create test information element
  IePerr a;
  IePerr::FailedDestination dest;
  dest.destination = Mac48Address("11:22:33:44:55:66");
  dest.seqnum = 1;
  a.AddAddressUnit(dest);
  dest.destination = Mac48Address("10:20:30:40:50:60");
  dest.seqnum = 2;
  a.AddAddressUnit(dest);
  dest.destination = Mac48Address("01:02:03:04:05:06");
  dest.seqnum = 3;
  a.AddAddressUnit(dest);
  
  IePerr b = a;
  b.Merge(a);
  NS_TEST_ASSERT_EQUAL (a, b);
  
  result = result && TestRoundtripSerialization (a);
  return result;
}

#endif // RUN_SELF_TESTS

} // namespace dot11s
} //namespace ns3


