/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
#ifndef MGT_HEADERS_H
#define MGT_HEADERS_H

#include <stdint.h>

#include "ns3/header.h"
#include "status-code.h"
#include "capability-information.h"
#include "supported-rates.h"
#include "ssid.h"

namespace ns3 {

class MgtAssocRequestHeader : public Header 
{
public:
  MgtAssocRequestHeader ();
  ~MgtAssocRequestHeader ();

  void SetSsid (Ssid ssid);
  void SetSupportedRates (SupportedRates rates);
  void SetListenInterval (uint16_t interval);

  Ssid GetSsid (void) const;
  SupportedRates GetSupportedRates (void) const;
  uint16_t GetListenInterval (void) const;

  static uint32_t GetUid (void);
  std::string GetName (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);

private:
  Ssid m_ssid;
  SupportedRates m_rates;
  CapabilityInformation m_capability;
  uint16_t m_listenInterval;
};

class MgtAssocResponseHeader : public Header {
public:
  MgtAssocResponseHeader ();
  ~MgtAssocResponseHeader ();

  StatusCode GetStatusCode (void);
  void SetStatusCode (StatusCode code);

  static uint32_t GetUid (void);
  std::string GetName (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);

private:
  SupportedRates m_rates;
  CapabilityInformation m_capability;
  StatusCode m_code;
  uint16_t m_aid;
};

class MgtProbeRequestHeader : public Header {
public:
  ~MgtProbeRequestHeader ();

  void SetSsid (Ssid ssid);
  void SetSupportedRates (SupportedRates rates);
  Ssid GetSsid (void) const;
  SupportedRates GetSupportedRates (void) const;

  static uint32_t GetUid (void);
  std::string GetName (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
private:

  Ssid m_ssid;
  SupportedRates m_rates;
};

class MgtProbeResponseHeader : public Header {
public:
  MgtProbeResponseHeader ();
  ~MgtProbeResponseHeader ();

  Ssid GetSsid (void) const;
  uint64_t GetBeaconIntervalUs (void) const;
  SupportedRates GetSupportedRates (void) const;

  void SetSsid (Ssid ssid);
  void SetBeaconIntervalUs (uint64_t us);
  void SetSupportedRates (SupportedRates rates);

  static uint32_t GetUid (void);
  std::string GetName (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);

private:
  Ssid m_ssid;
  uint64_t m_beaconInterval;
  SupportedRates m_rates;
  CapabilityInformation m_capability;
};

class MgtBeaconHeader : public MgtProbeResponseHeader {};


} // namespace ns3

#endif /* MGT_HEADERS_H */
