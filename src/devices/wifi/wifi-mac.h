#ifndef WIFI_MAC_H
#define WIFI_MAC_H

#include "ns3/packet.h"
#include "ns3/mac48-address.h"

#include "wifi-phy.h"
#include "wifi-remote-station-manager.h"
#include "ssid.h"

namespace ns3 {

class WifiMac : public Object
{
public:
  static TypeId GetTypeId (void);
  
  virtual void SetSlot (Time slotTime);
  virtual void SetSifs (Time sifs);
  virtual void SetEifsNoDifs (Time eifsNoDifs);
  void SetPifs (Time pifs);
  void SetCtsTimeout (Time ctsTimeout);
  void SetAckTimeout (Time ackTimeout);
  void SetMsduLifetime (Time msduLifetime);
  void SetMaxPropagationDelay (Time delay);

  Time GetPifs (void) const;
  Time GetSifs (void) const;
  Time GetSlot (void) const;
  Time GetEifsNoDifs (void) const;
  Time GetCtsTimeout (void) const;
  Time GetAckTimeout (void) const;
  Time GetMsduLifetime (void) const;
  Time GetMaxPropagationDelay (void) const;
  uint32_t GetMaxMsduSize (void) const;

  virtual Mac48Address GetAddress (void) const = 0;
  virtual Ssid GetSsid (void) const = 0;
  virtual Mac48Address GetBssid (void) const = 0;
  virtual void SetAddress (Mac48Address address) = 0;
  virtual void SetSsid (Ssid ssid) = 0;

private:
  friend class WifiNetDevice;
  virtual void Enqueue (Ptr<const Packet> packet, Mac48Address to) = 0;
  virtual void SetWifiPhy (Ptr<WifiPhy> phy) = 0;
  virtual void SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> stationManager) = 0;
  virtual void SetForwardUpCallback (Callback<void,Ptr<Packet>, const Mac48Address &> upCallback) = 0;
  virtual void SetLinkUpCallback (Callback<void> linkUp) = 0;
  virtual void SetLinkDownCallback (Callback<void> linkDown) = 0;


  static Time GetDefaultMaxPropagationDelay (void);
  static Time GetDefaultSlot (void);
  static Time GetDefaultSifs (void);
  static Time GetDefaultEifsNoDifs (void);
  static Time GetDefaultCtsAckDelay (void);
  static Time GetDefaultCtsAckTimeout (void);

  Time m_slot;
  Time m_pifs;
  Time m_sifs;
  Time m_eifsNoDifs;
  Time m_ctsTimeout;
  Time m_ackTimeout;
  Time m_maxPropagationDelay;
  uint32_t m_maxMsduSize;
};

} // namespace ns3

#endif /* WIFI_MAC_H */
