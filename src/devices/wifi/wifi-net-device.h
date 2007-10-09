/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
 * All rights reserved.
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

#ifndef WIFI_NET_DEVICE_H
#define WIFI_NET_DEVICE_H

#include "ns3/net-device.h"
#include "ns3/packet.h"
#include "ns3/callback-trace-source.h"
#include "ns3/mac48-address.h"
#include "ssid.h"
#include <string>

namespace ns3 {

class WifiChannel;
class PhyWifi;
class PropagationModel;
class MacStations;
class TraceContainer;
class MacLow;
class MacRxMiddle;
class MacTxMiddle;
class MacHighAdhoc;
class MacParameters;
class Dcf;
class DcaTxop;
class MacQueueWifie;
class MacHighAdhoc;
class MacHighNqsta;
class MacHighNqap;

class WifiNetDevice : public NetDevice {
public:
  virtual ~WifiNetDevice ();

  void ConnectTo (Ptr<WifiChannel> channel);

  Mac48Address GetSelfAddress (void) const;
  virtual Mac48Address GetBssid (void) const = 0;
  virtual Ssid GetSsid (void) const = 0;


protected:
  WifiNetDevice (Mac48Address address);
  void DoForwardUp (const Packet &packet, const Mac48Address &from);
private:
  virtual bool SendTo (const Packet &packet, const Address &to, uint16_t protocolNumber);
  virtual void NotifyConnected (void) = 0;
  virtual bool DoSendTo (const Packet &packet, const Mac48Address &to) = 0;
  void Associated (void);

  friend class WifiNetDeviceFactory;

  PropagationModel *m_propagation;
  PhyWifi *m_phy;
  MacStations *m_stations;
  MacLow *m_low;
  MacRxMiddle *m_rxMiddle;
  MacTxMiddle *m_txMiddle;
  MacParameters *m_parameters;
  CallbackTraceSource<Packet> m_rxTraceSource;
};

class WifiAdhocNetDevice : public WifiNetDevice {
public:
  WifiAdhocNetDevice (Ptr<Node> node, Mac48Address address);
  virtual ~WifiAdhocNetDevice ();

  virtual Mac48Address GetBssid (void) const;
  virtual Ssid GetSsid (void) const;
  void SetSsid (Ssid ssid);

private:
  virtual bool DoSendTo (const Packet &packet, Mac48Address const & to);
  virtual void NotifyConnected (void);
  friend class WifiNetDeviceFactory;
  Ssid m_ssid;
  DcaTxop *m_dca;
  MacHighAdhoc *m_high;
};

class WifiNqstaNetDevice : public WifiNetDevice {
public:
  WifiNqstaNetDevice (Ptr<Node> node, Mac48Address address);
  virtual ~WifiNqstaNetDevice ();

  virtual Mac48Address GetBssid (void) const;
  virtual Ssid GetSsid (void) const;
  void StartActiveAssociation (Ssid ssid);
private:
  void Associated (void);
  void DisAssociated (void);
  virtual bool DoSendTo (const Packet &packet, Mac48Address const & to);
  virtual void NotifyConnected (void);
  friend class WifiNetDeviceFactory;
  Ssid m_ssid;
  DcaTxop *m_dca;
  MacHighNqsta *m_high;
};

class WifiNqapWifiNetDevice : public WifiNetDevice {
public:
  WifiNqapWifiNetDevice (Mac48Address address);
  virtual ~WifiNqapWifiNetDevice ();

  virtual Mac48Address GetBssid (void) const;
  virtual Ssid GetSsid (void) const;
  void SetSsid (Ssid ssid);
private:
  virtual bool DoSendTo (const Packet &packet, Mac48Address const & to);
  virtual void NotifyConnected (void);
  friend class WifiNetDeviceFactory;
  Ssid m_ssid;
  DcaTxop *m_dca;
  MacHighNqap *m_high;
};

} // namespace ns3

#endif /* WIFI_NET_DEVICE_H */
