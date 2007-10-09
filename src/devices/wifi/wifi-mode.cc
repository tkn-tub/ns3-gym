#include "wifi-mode.h"
#include "ns3/assert.h"

namespace ns3 {

bool operator == (const WifiMode &a, const WifiMode &b)
{
  return a.GetUid () == b.GetUid ();
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
  NS_ASSERT (uid > 1);
  return &m_itemList[uid - 1];
}

WifiModeFactory *
WifiModeFactory::GetFactory (void)
{
  static WifiModeFactory factory;
  return &factory;
}

} // namespace ns3
