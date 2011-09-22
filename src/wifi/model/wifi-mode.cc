/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include "ns3/simulator.h"
#include "ns3/assert.h"
#include "ns3/log.h"

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
  mode = WifiModeFactory::GetFactory ()->Search (str);
  return is;
}

uint32_t
WifiMode::GetBandwidth (void) const
{
  struct WifiModeFactory::WifiModeItem *item = WifiModeFactory::GetFactory ()->Get (m_uid);
  return item->bandwidth;
}
uint64_t
WifiMode::GetPhyRate (void) const
{
  struct WifiModeFactory::WifiModeItem *item = WifiModeFactory::GetFactory ()->Get (m_uid);
  return item->phyRate;
}
uint64_t
WifiMode::GetDataRate (void) const
{
  struct WifiModeFactory::WifiModeItem *item = WifiModeFactory::GetFactory ()->Get (m_uid);
  return item->dataRate;
}
enum WifiCodeRate
WifiMode::GetCodeRate (void) const
{
  struct WifiModeFactory::WifiModeItem *item = WifiModeFactory::GetFactory ()->Get (m_uid);
  return item->codingRate;
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
enum WifiModulationClass
WifiMode::GetModulationClass () const
{
  struct WifiModeFactory::WifiModeItem *item = WifiModeFactory::GetFactory ()->Get (m_uid);
  return item->modClass;
}
WifiMode::WifiMode ()
  : m_uid (0)
{
}
WifiMode::WifiMode (uint32_t uid)
  : m_uid (uid)
{
}
WifiMode::WifiMode (std::string name)
{
  *this = WifiModeFactory::GetFactory ()->Search (name);
}

ATTRIBUTE_HELPER_CPP (WifiMode);

WifiModeFactory::WifiModeFactory ()
{
}


WifiMode
WifiModeFactory::CreateWifiMode (std::string uniqueName,
                                 enum WifiModulationClass modClass,
                                 bool isMandatory,
                                 uint32_t bandwidth,
                                 uint32_t dataRate,
                                 enum WifiCodeRate codingRate,
                                 uint8_t constellationSize)
{
  WifiModeFactory *factory = GetFactory ();
  uint32_t uid = factory->AllocateUid (uniqueName);
  WifiModeItem *item = factory->Get (uid);
  item->uniqueUid = uniqueName;
  item->modClass = modClass;
  // The modulation class for this WifiMode must be valid.
  NS_ASSERT (modClass != WIFI_MOD_CLASS_UNKNOWN);

  item->bandwidth = bandwidth;
  item->dataRate = dataRate;

  item->codingRate = codingRate;

  switch (codingRate)
    {
    case WIFI_CODE_RATE_3_4:
      item->phyRate = dataRate * 4 / 3;
      break;
    case WIFI_CODE_RATE_2_3:
      item->phyRate = dataRate * 3 / 2;
      break;
    case WIFI_CODE_RATE_1_2:
      item->phyRate = dataRate * 2 / 1;
      break;
    case WIFI_CODE_RATE_UNDEFINED:
    default:
      item->phyRate = dataRate;
      break;
    }

  // Check for compatibility between modulation class and coding
  // rate. If modulation class is DSSS then coding rate must be
  // undefined, and vice versa. I could have done this with an
  // assertion, but it seems better to always give the error (i.e.,
  // not only in non-optimised builds) and the cycles that extra test
  // here costs are only suffered at simulation setup.
  if ((codingRate == WIFI_CODE_RATE_UNDEFINED) != (modClass == WIFI_MOD_CLASS_DSSS))
    {
      NS_FATAL_ERROR ("Error in creation of WifiMode named " << uniqueName << std::endl
                                                             << "Code rate must be WIFI_CODE_RATE_UNDEFINED iff Modulation Class is WIFI_MOD_CLASS_DSSS");
    }

  item->constellationSize = constellationSize;
  item->isMandatory = isMandatory;

  return WifiMode (uid);
}

WifiMode
WifiModeFactory::Search (std::string name)
{
  WifiModeItemList::const_iterator i;
  uint32_t j = 0;
  for (i = m_itemList.begin (); i != m_itemList.end (); i++)
    {
      if (i->uniqueUid == name)
        {
          return WifiMode (j);
        }
      j++;
    }

  // If we get here then a matching WifiMode was not found above. This
  // is a fatal problem, but we try to be helpful by displaying the
  // list of WifiModes that are supported.
  NS_LOG_UNCOND ("Could not find match for WifiMode named \""
                 << name << "\". Valid options are:");
  for (i = m_itemList.begin (); i != m_itemList.end (); i++)
    {
      NS_LOG_UNCOND ("  " << i->uniqueUid);
    }
  // Empty fatal error to die. We've already unconditionally logged
  // the helpful information.
  NS_FATAL_ERROR ("");

  // This next line is unreachable because of the fatal error
  // immediately above, and that is fortunate, because we have no idea
  // what is in WifiMode (0), but we do know it is not what our caller
  // has requested by name. It's here only because it's the safest
  // thing that'll give valid code.
  return WifiMode (0);
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
      item->modClass = WIFI_MOD_CLASS_UNKNOWN;
      item->constellationSize = 0;
      item->codingRate = WIFI_CODE_RATE_UNDEFINED;
      item->isMandatory = false;
      isFirstTime = false;
    }
  return &factory;
}

} // namespace ns3
