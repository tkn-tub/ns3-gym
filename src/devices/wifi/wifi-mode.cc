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
std::istream & operator >> (std::istream &is, WifiMode &mode)
{
  std::string str;
  is >> str;
  if (!WifiModeFactory::GetFactory ()->Search (str, &mode))
    {
      is.setstate (std::ios_base::badbit);
    }
  return is;
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
  // needed for ostream printing of the invalid mode
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
enum WifiPhyStandard 
WifiMode::GetStandard () const
{
  struct WifiModeFactory::WifiModeItem *item = WifiModeFactory::GetFactory ()->Get (m_uid);
  return item->standard;
}
WifiMode::WifiMode ()
  : m_uid (0)
{}
WifiMode::WifiMode (uint32_t uid)
  : m_uid (uid)
{}
WifiMode::WifiMode (std::string name)
{
  if (!WifiModeFactory::GetFactory ()->Search (name, this))
    {
      NS_FATAL_ERROR ("Invalid requested wifi mode: " << name);
    }
}

ATTRIBUTE_HELPER_CPP (WifiMode);

WifiModeFactory::WifiModeFactory ()
{}

WifiMode 
WifiModeFactory::CreateBpsk (std::string uniqueName,
			     bool isMandatory,
			     uint32_t bandwidth,
			     uint32_t dataRate,
			     uint32_t phyRate,
                             enum WifiPhyStandard standard)
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
  item->standard = standard;
  return WifiMode (uid);
}

WifiMode 
WifiModeFactory::CreateQpsk (std::string uniqueName,
			     bool isMandatory,
			     uint32_t bandwidth,
			     uint32_t dataRate,
			     uint32_t phyRate,
                             enum WifiPhyStandard standard)
{
  WifiModeFactory *factory = GetFactory ();
  uint32_t uid = factory->AllocateUid (uniqueName);
  WifiModeItem *item = factory->Get (uid);
  item->uniqueUid = uniqueName;
  item->bandwidth = bandwidth;
  item->dataRate = dataRate;
  item->phyRate = phyRate;
  item->modulation = WifiMode::QPSK;
  item->constellationSize = 4;
  item->isMandatory = isMandatory;
  item->standard = standard;
  return WifiMode (uid);
}

WifiMode 
WifiModeFactory::CreateQam (std::string uniqueName,
                            bool isMandatory,
			    uint32_t bandwidth,
			    uint32_t dataRate,
			    uint32_t phyRate,
			    uint8_t constellationSize,
                            enum WifiPhyStandard standard)
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
  item->standard = standard;
  return WifiMode (uid);
}
WifiMode 
WifiModeFactory::CreateDbpsk (std::string uniqueName,
			     bool isMandatory,
			     uint32_t bandwidth,
			     uint32_t dataRate,
			     uint32_t phyRate,
                             enum WifiPhyStandard standard)
{
  WifiModeFactory *factory = GetFactory ();
  uint32_t uid = factory->AllocateUid (uniqueName);
  WifiModeItem *item = factory->Get (uid);
  item->uniqueUid = uniqueName;
  item->bandwidth = bandwidth;
  item->dataRate = dataRate;
  item->phyRate = phyRate;
  item->modulation = WifiMode::DBPSK;
  item->constellationSize = 2;
  item->isMandatory = isMandatory;
  item->standard = standard;
  return WifiMode (uid);
}
WifiMode 
WifiModeFactory::CreateDqpsk (std::string uniqueName,
			     bool isMandatory,
			     uint32_t bandwidth,
			     uint32_t dataRate,
			     uint32_t phyRate,
                             enum WifiPhyStandard standard)
{
  WifiModeFactory *factory = GetFactory ();
  uint32_t uid = factory->AllocateUid (uniqueName);
  WifiModeItem *item = factory->Get (uid);
  item->uniqueUid = uniqueName;
  item->bandwidth = bandwidth;
  item->dataRate = dataRate;
  item->phyRate = phyRate;
  item->modulation = WifiMode::DQPSK;
  item->constellationSize = 4;
  item->isMandatory = isMandatory;
  item->standard = standard;
  return WifiMode (uid);
}
bool 
WifiModeFactory::Search (std::string name, WifiMode *mode)
{
  uint32_t j = 0;
  for (WifiModeItemList::const_iterator i = m_itemList.begin ();
       i != m_itemList.end (); i++)
    {
      if (i->uniqueUid == name)
	{
          *mode = WifiMode (j);
	  return true;
	}
      j++;
    }
  *mode = WifiMode (0);
  return false;
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
  uint32_t uid = m_itemList.size ();
  m_itemList.push_back (WifiModeItem ());
  return uid;
}

struct WifiModeFactory::WifiModeItem *
WifiModeFactory::Get (uint32_t uid)
{
  NS_ASSERT (uid < m_itemList.size ());
  return &m_itemList[uid];
}

WifiModeFactory *
WifiModeFactory::GetFactory (void)
{
  static bool isFirstTime = true;
  static WifiModeFactory factory;
  if (isFirstTime)
    {
      uint32_t uid = factory.AllocateUid ("Invalid-WifiMode");
      WifiModeItem *item = factory.Get (uid);
      item->uniqueUid = "Invalid-WifiMode";
      item->bandwidth = 0;
      item->dataRate = 0;
      item->phyRate = 0;
      item->modulation = WifiMode::UNKNOWN;
      item->constellationSize = 0;
      item->isMandatory = false;
      item->standard = WIFI_PHY_UNKNOWN;
      isFirstTime = false;
    }
  return &factory;
}

} // namespace ns3
