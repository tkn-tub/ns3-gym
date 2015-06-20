/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#ifndef WIFI_PHY_STANDARD_H
#define WIFI_PHY_STANDARD_H

namespace ns3 {


/**
 * \ingroup wifi
 * Identifies the PHY specification that a Wifi device is configured to use.
 */
enum WifiPhyStandard
{
  /** OFDM PHY for the 5 GHz band (Clause 17) */
  WIFI_PHY_STANDARD_80211a,
  /** DSSS PHY (Clause 15) and HR/DSSS PHY (Clause 18) */
  WIFI_PHY_STANDARD_80211b,
  /** ERP-OFDM PHY (Clause 19, Section 19.5) */
  WIFI_PHY_STANDARD_80211g,
  /** OFDM PHY for the 5 GHz band (Clause 17 with 10 MHz channel bandwidth) */
  WIFI_PHY_STANDARD_80211_10MHZ,
  /** OFDM PHY for the 5 GHz band (Clause 17 with 5 MHz channel bandwidth) */
  WIFI_PHY_STANDARD_80211_5MHZ,
  /** This is intended to be the configuration used in this paper:
   *  Gavin Holland, Nitin Vaidya and Paramvir Bahl, "A Rate-Adaptive
   *  MAC Protocol for Multi-Hop Wireless Networks", in Proc. of
   *  ACM MOBICOM, 2001.
   */
  WIFI_PHY_STANDARD_holland,
  /** HT OFDM PHY for the 2.4 GHz band (clause 20) */
  WIFI_PHY_STANDARD_80211n_2_4GHZ,
  /** HT OFDM PHY for the 5 GHz band (clause 20) */
  WIFI_PHY_STANDARD_80211n_5GHZ
};

} //namespace ns3

#endif /* WIFI_PHY_STANDARD_H */
