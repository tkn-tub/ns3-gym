#include "wifi-mode.h"

namespace ns3 {

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
uint8_t 
WifiMode::GetConstellationSize (void) const
{
  struct WifiModeFactory::WifiModeItem *item = WifiModeFactory::GetFactory ()->Get (m_uid);
  return item->constellationSize;
}
uint32_t 
WifiMode::GetUid (void) const
{
  return m_uid;
}
WifiMode::WifiMode ()
{}
WifiMode::WifiMode (uint32_t uid)
  : m_uid (uid)
{}


WifiMode 
WifiModeFactory::CreateBpsk (std::string uniqueName,
			     uint32_t bandwidth,
			     uint32_t dataRate,
			     uint32_t phyRate)
{
  WifiModeFactory *factory = GetFactory ();
  uint32_t uid = factory->AllocateUid (uniqueName);
  WifiModeItem *item = factory->Get (uid);
  item->bandwidth = bandwidth;
  item->dataRate = dataRate;
  item->phyRate = phyRate;
  item->modulation = WifiMode::BPSK;
  item->constellationSize = 2;
  return WifiMode (uid);
}
WifiMode 
WifiModeFactory::CreateQam (std::string uniqueName,
			    uint32_t bandwidth,
			    uint32_t dataRate,
			    uint32_t phyRate,
			    uint8_t constellationSize)
{
  WifiModeFactory *factory = GetFactory ();
  uint32_t uid = factory->AllocateUid (uniqueName);
  WifiModeItem *item = factory->Get (uid);
  item->bandwidth = bandwidth;
  item->dataRate = dataRate;
  item->phyRate = phyRate;
  item->modulation = WifiMode::QAM;
  item->constellationSize = constellationSize;
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

WifiModeFactory *
WifiModeFactory::GetFactory (void)
{
  static WifiModeFactory factory;
  return &factory;
}

} // namespace ns3
