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
#include "ns3/wifi-phy-standard.h"

namespace ns3 {

/**
 * This enumeration defines the modulation classes per IEEE
 * 802.11-2007, Section 9.6.1, Table 9-2.
 */
enum WifiModulationClass
{
  /** Modulation class unknown or unspecified. A WifiMode with this
  WifiModulationClass has not been properly initialised. */
  WIFI_MOD_CLASS_UNKNOWN = 0,
  /** Infrared (IR) (Clause 16) */
  WIFI_MOD_CLASS_IR,
  /** Frequency-hopping spread spectrum (FHSS) PHY (Clause 14) */
  WIFI_MOD_CLASS_FHSS,
  /** DSSS PHY (Clause 15) and HR/DSSS PHY (Clause 18) */
  WIFI_MOD_CLASS_DSSS,
  /** ERP-PBCC PHY (19.6) */
  WIFI_MOD_CLASS_ERP_PBCC,
  /** DSSS-OFDM PHY (19.7) */
  WIFI_MOD_CLASS_DSSS_OFDM,
  /** ERP-OFDM PHY (19.5) */
  WIFI_MOD_CLASS_ERP_OFDM,
  /** OFDM PHY (Clause 17) */
  WIFI_MOD_CLASS_OFDM,
  /** HT PHY (Clause 20) */
  WIFI_MOD_CLASS_HT
};


/**
 * This enumeration defines the various convolutional coding rates
 * used for the OFDM transmission modes in the IEEE 802.11
 * standard. DSSS (for example) rates which do not have an explicit
 * coding stage in their generation should have this parameter set to
 * WIFI_CODE_RATE_UNDEFINED.
 */
enum WifiCodeRate
{
  /** No explicit coding (e.g., DSSS rates) */
  WIFI_CODE_RATE_UNDEFINED,
  /** Rate 3/4 */
  WIFI_CODE_RATE_3_4,
  /** Rate 2/3 */
  WIFI_CODE_RATE_2_3,
  /** Rate 1/2 */
  WIFI_CODE_RATE_1_2
};

/**
 * \brief represent a single transmission mode
 * \ingroup wifi
 *
 * A WifiMode is implemented by a single integer which is used
 * to lookup in a global array the characteristics of the
 * associated transmission mode. It is thus extremely cheap to
 * keep a WifiMode variable around.
 */
class WifiMode
{
public:
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
   * \returns the coding rate of this transmission mode
   */
  enum WifiCodeRate GetCodeRate (void) const;
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
   *
   * \returns the Modulation Class (see IEEE 802.11-2007 Section
   * 9.6.1) to which this WifiMode belongs.
   */
  enum WifiModulationClass GetModulationClass () const;

  /**
   * Create an invalid WifiMode. Calling any method on the
   * instance created will trigger an assert. This is useful
   * to separate the declaration of a WifiMode variable from
   * its initialization.
   */
  WifiMode ();
  WifiMode (std::string name);
private:
  friend class WifiModeFactory;
  WifiMode (uint32_t uid);
  uint32_t m_uid;
};

bool operator == (const WifiMode &a, const WifiMode &b);
std::ostream & operator << (std::ostream & os, const WifiMode &mode);
std::istream & operator >> (std::istream &is, WifiMode &mode);

/**
 * \class ns3::WifiModeValue
 * \brief hold objects of type ns3::WifiMode
 */

ATTRIBUTE_HELPER_HEADER (WifiMode);

/**
 * In various parts of the code, folk are interested in maintaining a
 * list of transmission modes. The vector class provides a good basis
 * for this, but we here add some syntactic sugar by defining a
 * WifiModeList type, and a corresponding iterator.
 */
typedef std::vector<WifiMode> WifiModeList;
typedef WifiModeList::const_iterator WifiModeListIterator;

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
   * \param modClass the class of modulation
   * \param isMandatory true if this WifiMode is mandatory, false otherwise.
   * \param bandwidth the bandwidth (Hz) of the signal generated when the
   *        associated WifiMode is used.
   * \param dataRate the rate (bits/second) at which the user data is transmitted
   * \param codingRate if convolutional coding is used for this rate
   * then this parameter specifies the convolutional coding rate
   * used. If there is no explicit convolutional coding step (e.g.,
   * for DSSS rates) then the caller should set this parameter to
   * WIFI_CODE_RATE_UNCODED.
   * \param constellationSize the order of the constellation used.
   *
   * Create a WifiMode.
   */
  static WifiMode CreateWifiMode (std::string uniqueName,
                                  enum WifiModulationClass modClass,
                                  bool isMandatory,
                                  uint32_t bandwidth,
                                  uint32_t dataRate,
                                  enum WifiCodeRate codingRate,
                                  uint8_t constellationSize);

private:
  friend class WifiMode;
  friend std::istream & operator >> (std::istream &is, WifiMode &mode);
  static WifiModeFactory* GetFactory ();
  WifiModeFactory ();

  /**
   * This is the data associated to a unique WifiMode.
   * The integer stored in a WifiMode is in fact an index
   * in an array of WifiModeItem objects.
   */
  struct WifiModeItem
  {
    std::string uniqueUid;
    uint32_t bandwidth;
    uint32_t dataRate;
    uint32_t phyRate;
    enum WifiModulationClass modClass;
    uint8_t constellationSize;
    enum WifiCodeRate codingRate;
    bool isMandatory;
  };

  WifiMode Search (std::string name);
  uint32_t AllocateUid (std::string uniqueName);
  WifiModeItem* Get (uint32_t uid);

  typedef std::vector<struct WifiModeItem> WifiModeItemList;
  WifiModeItemList m_itemList;
};

} // namespace ns3

#endif /* WIFI_MODE_H */
