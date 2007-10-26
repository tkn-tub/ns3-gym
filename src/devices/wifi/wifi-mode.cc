/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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
#include "wifi-mode.h"
#include "ns3/assert.h"

namespace ns3 {

bool operator == (const WifiMode &a, const WifiMode &b)
{
  return a.GetUid () == b.GetUid ();
}
std::ostream & operator << (std::ostream & os, const WifiMode &mode)
{
  os << mode.GetUniqueName ();
  return os;
}

uint32_t 
WifiMode::GetBandwidth (void) const
{
  struct WifiModeFactory::WifiModeItem *item = WifiModeFactory::GetFactory ()->Get (m_uid);
  return item->bandwidth;
}
uint32_t 
WifiMode::GetPhyRate (void) const
{
  struct WifiModeFactory::WifiModeItem *item = WifiModeFactory::GetFactory ()->Get (m_uid);
  return item->phyRate;
}
uint32_t 
WifiMode::GetDataRate (void) const
{
  struct WifiModeFactory::WifiModeItem *item = WifiModeFactory::GetFactory ()->Get (m_uid);
  return item->dataRate;
}
bool 
WifiMode::IsModulationBpsk (void) const
{
  struct WifiModeFactory::WifiModeItem *item = WifiModeFactory::GetFactory ()->Get (m_uid);
  return item->modulation == WifiMode::BPSK;
}
bool 
WifiMode::IsModulationQam (void) const
{
  struct WifiModeFactory::WifiModeItem *item = WifiModeFactory::GetFactory ()->Get (m_uid);
  return item->modulation == WifiMode::QAM;
}
enum WifiMode::ModulationType 
WifiMode::GetModulationType (void) const
{
  struct WifiModeFactory::WifiModeItem *item = WifiModeFactory::GetFactory ()->Get (m_uid);
  return item->modulation;
}
uint8_t 
WifiMode::GetConstellationSize (void) const
{
  struct WifiModeFactory::WifiModeItem *item = WifiModeFactory::GetFactory ()->Get (m_uid);
  return item->constellationSize;
}
std::string 
WifiMode::GetUniqueName (void) const
{
  struct WifiModeFactory::WifiModeItem *item = WifiModeFactory::GetFactory ()->Get (m_uid);
  return item->uniqueUid;
}
bool
WifiMode::IsMandatory (void) const
{
  struct WifiModeFactory::WifiModeItem *item = WifiModeFactory::GetFactory ()->Get (m_uid);
  return item->isMandatory;
}
uint32_t 
WifiMode::GetUid (void) const
{
  return m_uid;
}
WifiMode::WifiMode ()
  : m_uid (0)
{}
WifiMode::WifiMode (uint32_t uid)
  : m_uid (uid)
{}

WifiModeFactory::WifiModeFactory ()
{}

WifiMode 
WifiModeFactory::CreateBpsk (std::string uniqueName,
			     bool isMandatory,
			     uint32_t bandwidth,
			     uint32_t dataRate,
			     uint32_t phyRate)
{
  WifiModeFactory *factory = GetFactory ();
  uint32_t uid = factory->AllocateUid (uniqueName);
  WifiModeItem *item = factory->Get (uid);
  item->uniqueUid = uniqueName;
  item->bandwidth = bandwidth;
  item->dataRate = dataRate;
  item->phyRate = phyRate;
  item->modulation = WifiMode::BPSK;
  item->constellationSize = 2;
  item->isMandatory = isMandatory;
  return WifiMode (uid);
}
WifiMode 
WifiModeFactory::CreateQam (std::string uniqueName,
			     bool isMandatory,
			    uint32_t bandwidth,
			    uint32_t dataRate,
			    uint32_t phyRate,
			    uint8_t constellationSize)
{
  WifiModeFactory *factory = GetFactory ();
  uint32_t uid = factory->AllocateUid (uniqueName);
  WifiModeItem *item = factory->Get (uid);
  item->uniqueUid = uniqueName;
  item->bandwidth = bandwidth;
  item->dataRate = dataRate;
  item->phyRate = phyRate;
  item->modulation = WifiMode::QAM;
  item->constellationSize = constellationSize;
  item->isMandatory = isMandatory;
  return WifiMode (uid);
}

uint32_t
WifiModeFactory::AllocateUid (std::string uniqueUid)
{
  uint32_t j = 0;
  for (WifiModeItemList::const_iterator i = m_itemList.begin ();
       i != m_itemList.end (); i++)
    {
      if (i->uniqueUid == uniqueUid)
	{
	  return j;
	}
      j++;
    }
  m_itemList.push_back (WifiModeItem ());
  uint32_t uid = m_itemList.size ();
  return uid;
}

struct WifiModeFactory::WifiModeItem *
WifiModeFactory::Get (uint32_t uid)
{
  NS_ASSERT (uid > 0 && uid <= m_itemList.size ());
  return &m_itemList[uid - 1];
}

WifiModeFactory *
WifiModeFactory::GetFactory (void)
{
  static WifiModeFactory factory;
  return &factory;
}

} // namespace ns3
