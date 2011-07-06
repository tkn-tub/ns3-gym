/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008 INRIA
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
 * Author: Jahanzeb Farooq <jahanzeb.farooq@sophia.inria.fr>
 */

#ifndef BURST_PROFILE_MANAGER_H
#define BURST_PROFILE_MANAGER_H

#include <stdint.h>
#include "cid.h"
#include "wimax-phy.h"
#include "wimax-net-device.h"

namespace ns3 {

class SSRecord;
class RngReq;

/**
 * \ingroup wimax
 */
class BurstProfileManager : public Object
{
public:
  BurstProfileManager (Ptr<WimaxNetDevice> device);
  ~BurstProfileManager (void);
  void DoDispose (void);
  /*
   * \returns the number of available burst profile
   */
  uint16_t GetNrBurstProfilesToDefine (void);

  /*
   * \brief returns the modulation type of a given iuc
   * \param direction should be uplink or downlink
   * \param iuc the iuc
   * \returns the modulation type of the selected iuc
   */
  WimaxPhy::ModulationType GetModulationType (uint8_t iuc,
                                              WimaxNetDevice::Direction direction) const;

  uint8_t GetBurstProfile (WimaxPhy::ModulationType modulationType,
                           WimaxNetDevice::Direction direction) const;

  /*
   * \brief during initial ranging or periodic ranging (or when RNG-REQ is used instead of
   * DBPC) calculates the least robust burst profile for SS, e.g., based on distance,
   * power, signal etc
   *
   */
  uint8_t GetBurstProfileForSS (const SSRecord *ssRecord, const RngReq *rngreq,
                                WimaxPhy::ModulationType &modulationType);
  WimaxPhy::ModulationType GetModulationTypeForSS (const SSRecord *ssRecord,
                                                   const RngReq *rngreq);
  uint8_t GetBurstProfileToRequest (void);
private:
  BurstProfileManager (const BurstProfileManager &);
  BurstProfileManager& operator= (const BurstProfileManager &);

  Ptr<WimaxNetDevice> m_device;
};

} // namespace ns3

#endif /* BURST_PROFILE_MANAGER_H */
