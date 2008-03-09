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
#ifndef WIFI_MODE_H
#define WIFI_MODE_H

#include <stdint.h>
#include <string>
#include <vector>
#include <ostream>
#include "ns3/attribute-helper.h"

namespace ns3 {

/**
 * \brief represent a single transmission mode
 *
 * A WifiMode is implemented by a single integer which is used
 * to lookup in a global array the characteristics of the
 * associated transmission mode. It is thus extremely cheap to
 * keep a WifiMode variable around.
 */
class WifiMode
{
 public:
  enum ModulationType {
    BPSK,
    QAM
  };
  
  /**
   * \returns the number of Hz used by this signal
   */
  uint32_t GetBandwidth (void) const;
  /**
   * \returns the physical bit rate of this signal.
   *
   * If a transmission mode uses 1/2 FEC, and if its
   * data rate is 3Mbs, the phy rate is 6Mbs
   */
  uint32_t GetPhyRate (void) const;
  /**
   * \returns the data bit rate of this signal.
   */
  uint32_t GetDataRate (void) const;
  /**
   * \returns true if this mode uses a bpsk modulation, false
   *          otherwise.
   */
  bool IsModulationBpsk (void) const;
  /**
   * \returns true if this mode uses a qam modulation, false
   *          otherwise.
   */
  bool IsModulationQam (void) const;
  /**
   * \returns the type of modulation used by this mode.
   */
  enum ModulationType GetModulationType (void) const;
  /**
   * \returns the size of the modulation constellation.
   */
  uint8_t GetConstellationSize (void) const;

  /**
   * \returns a human-readable representation of this WifiMode
   * instance.
   */
  std::string GetUniqueName (void) const;

  /**
   * \returns true if this mode is a mandatory mode, false
   *          otherwise.
   */
  bool IsMandatory (void) const;

  /**
   * \returns the uid associated to this wireless mode.
   *
   * Each specific wireless mode should have a different uid.
   * For example, the 802.11b 1Mbs and the 802.11b 2Mbs modes
   * should have different uids.
   */
  uint32_t GetUid (void) const;

  /**
   * Create an invalid WifiMode. Calling any method on the
   * instance created will trigger an assert. This is useful
   * to separate the declaration of a WifiMode variable from
   * its initialization.
   */
  WifiMode ();

  ATTRIBUTE_HELPER_HEADER_1 (WifiMode);
private:
  friend class WifiModeFactory;
  WifiMode (uint32_t uid);
  uint32_t m_uid;
};

bool operator == (const WifiMode &a, const WifiMode &b);
std::ostream & operator << (std::ostream & os, const WifiMode &mode);
std::istream & operator >> (std::istream &is, WifiMode &mode);

ATTRIBUTE_HELPER_HEADER_2 (WifiMode);

/**
 * \brief create WifiMode class instances and keep track of them.
 *
 * This factory ensures that each WifiMode created has a unique name
 * and assigns to each of them a unique integer.
 */
class WifiModeFactory
{
public:
  /**
   * \param uniqueName the name of the associated WifiMode. This name
   *        must be unique accross _all_ instances.
   * \param isMandatory true if this WifiMode is mandatory, false otherwise.
   * \param bandwidth the bandwidth (Hz) of the signal generated when the
   *        associated WifiMode is used.
   * \param dataRate the rate (bits/second) at which the user data is transmitted
   * \param phyRate the rate (bits/second) at which the encoded user data is transmitted
   *        The phyRate includes FEC so, is typically higher than the dataRate.
   *
   * Create a BPSK WifiMode.
   */
  static WifiMode CreateBpsk (std::string uniqueName,
			      bool isMandatory,
			      uint32_t bandwidth,
			      uint32_t dataRate,
			      uint32_t phyRate);
  /**
   * \param uniqueName the name of the associated WifiMode. This name
   *        must be unique accross _all_ instances.
   * \param isMandatory true if this WifiMode is mandatory, false otherwise.
   * \param bandwidth the bandwidth (Hz) of the signal generated when the
   *        associated WifiMode is used.
   * \param dataRate the rate (bits/second) at which the user data is transmitted
   * \param phyRate the rate (bits/second) at which the encoded user data is transmitted
   *        The phyRate includes FEC so, is typically higher than the dataRate.
   * \param constellationSize the number of elements included in the QAM constellation. 
   *
   * Create a QAM WifiMode.
   */
  static WifiMode CreateQam (std::string uniqueName,
                             bool isMandatory,
			     uint32_t bandwidth,
			     uint32_t dataRate,
			     uint32_t phyRate,
			     uint8_t constellationSize);

private:
  friend class WifiMode;  
  friend std::istream & operator >> (std::istream &is, WifiMode &mode);
  static WifiModeFactory *GetFactory ();
  WifiModeFactory ();

  /**
   * This is the data associated to a unique WifiMode.
   * The integer stored in a WifiMode is in fact an index
   * in an array of WifiModeItem objects.
   */
  struct WifiModeItem {
    std::string uniqueUid;
    uint32_t bandwidth;
    uint32_t dataRate;
    uint32_t phyRate;
    enum WifiMode::ModulationType modulation;
    uint8_t constellationSize;
    bool isMandatory;
  };

  bool Search (std::string name, WifiMode *mode);
  uint32_t AllocateUid (std::string uniqueName);
  WifiModeItem *Get (uint32_t uid);

  typedef std::vector<struct WifiModeItem> WifiModeItemList;
  WifiModeItemList m_itemList;
};

} // namespace ns3

#endif /* WIFI_MODE_H */
