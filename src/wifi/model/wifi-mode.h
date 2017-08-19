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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#ifndef WIFI_MODE_H
#define WIFI_MODE_H

#include <vector>
#include "ns3/attribute-helper.h"

namespace ns3 {

class WifiTxVector;

/**
 * This enumeration defines the modulation classes per
 * (Table 9-4 "Modulation classes"; IEEE 802.11-2012).
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
  /** DSSS PHY (Clause 15) */
  WIFI_MOD_CLASS_DSSS,
  /** HR/DSSS PHY (Clause 18) */
  WIFI_MOD_CLASS_HR_DSSS,
  /** ERP-PBCC PHY (19.6) */
  WIFI_MOD_CLASS_ERP_PBCC,
  /** DSSS-OFDM PHY (19.7) */
  WIFI_MOD_CLASS_DSSS_OFDM,
  /** ERP-OFDM PHY (19.5) */
  WIFI_MOD_CLASS_ERP_OFDM,
  /** OFDM PHY (Clause 17) */
  WIFI_MOD_CLASS_OFDM,
  /** HT PHY (Clause 20) */
  WIFI_MOD_CLASS_HT,
  /** VHT PHY (Clause 22) */
  WIFI_MOD_CLASS_VHT,
  /** HE PHY (Clause 26) */
  WIFI_MOD_CLASS_HE
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
  WIFI_CODE_RATE_1_2,
  /** Rate 5/6 */
  WIFI_CODE_RATE_5_6
};

/**
 * \brief represent a single transmission mode
 * \ingroup wifi
 *
 * A WifiMode is implemented by a single integer which is used
 * to lookup in a global array the characteristics of the
 * associated transmission mode. It is thus extremely cheap to
 * keep a WifiMode variable around.
 *
 * \see attribute_WifiMode
 */
class WifiMode
{
public:
  /**
   * \returns true if this <MCS, channel width, NSS> combination is allowed, false otherwise.
   *
   * \param channelWidth the considered channel width in MHz
   * \param nss the considered number of streams
   */
  bool IsAllowed (uint8_t channelWidth, uint8_t nss) const;
  /**
   *
   * \param channelWidth the considered channel width in MHz
   * \param guardInterval the considered guard interval duration in nanoseconds
   * \param nss the considered number of streams
   *
   * \returns the physical bit rate of this signal.
   *
   * If a transmission mode uses 1/2 FEC, and if its
   * data rate is 3.25Mbps, the phy rate is 6.5Mbps
   */
  uint64_t GetPhyRate (uint8_t channelWidth, uint16_t guardInterval, uint8_t nss) const;
  /**
   * \param txVector the WifiTxVector of the signal
   *
   * \returns the physical bit rate of this signal.
   *
   * If a transmission mode uses 1/2 FEC, and if its
   * data rate is 3.25Mbps, the phy rate is 6.5Mbps
   */
  uint64_t GetPhyRate (WifiTxVector txVector) const;
  /**
   *
   * \param channelWidth the considered channel width in MHz
   * \param guardInterval the considered guard interval duration in nanoseconds
   * \param nss the considered number of streams
   *
   * \returns the data bit rate of this signal.
   */
  uint64_t GetDataRate (uint8_t channelWidth, uint16_t guardInterval, uint8_t nss) const;
  /**
   * \param txVector the WifiTxVector of the signal
   *
   * \returns the data bit rate of this signal.
   */
  uint64_t GetDataRate (WifiTxVector txVector) const;
  /**
   * \param channelWidth the considered channel width in MHz
   *
   * \returns the data bit rate of this non-HT or non-VHT signal.
  */
  uint64_t GetDataRate (uint8_t channelWidth) const;

  /**
   * \returns the coding rate of this transmission mode
   */
  WifiCodeRate GetCodeRate (void) const;
  /**
   * \returns the size of the modulation constellation.
   */
  uint16_t GetConstellationSize (void) const;
  /**
   * \returns the MCS value.
   */
  uint8_t GetMcsValue (void) const;
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
   * For example, the 802.11b 1Mbps and the 802.11b 2Mbps modes
   * should have different uids.
   */
  uint32_t GetUid (void) const;
  /**
   *
   * \returns the Modulation Class (Section 9.7.8 "Modulation classes"; IEEE 802.11-2012)
   * to which this WifiMode belongs.
   */
  WifiModulationClass GetModulationClass () const;
  /**
   * \returns the rate (in bps) of the non-HT Reference Rate
   * which corresponds to the HT MCS of this WifiMode.
   *
   * To convert an HT MCS to is corresponding non-HT Reference Rate
   * use the modulation and coding rate of the HT MCS
   * and lookup in Table 9-5 of IEEE 802.11-2012.
   */
  uint64_t GetNonHtReferenceRate (void) const;
  /**
   * \param mode wifi mode
   * \returns true if this WifiMode has a
   * a code rate strictly higher than mode.
   */
  bool IsHigherCodeRate (WifiMode mode) const;
  /**
   * \param mode wifi mode
   * \returns true if this WifiMode has a
   * a rate strictly higher than mode.
   */
  bool IsHigherDataRate (WifiMode mode) const;

  /**
   * Create an invalid WifiMode. Calling any method on the
   * instance created will trigger an assert. This is useful
   * to separate the declaration of a WifiMode variable from
   * its initialization.
   */
  WifiMode ();
  /**
   * Create a WifiMode if the given string represents a valid
   * WifiMode name.
   *
   * \param name std::string of a valid WifiMode name
   */
  WifiMode (std::string name);


private:
  /// allow WifiModeFactory class access
  friend class WifiModeFactory;
  /// allow WifiPhyTag class access
  friend class WifiPhyTag; // access the UID-based constructor
  /**
   * Create a WifiMode from a given unique ID.
   *
   * \param uid unique ID
   */
  WifiMode (uint32_t uid);
  uint32_t m_uid; ///< UID
};

/// equality operator
bool operator == (const WifiMode &a, const WifiMode &b);
std::ostream & operator << (std::ostream & os, const WifiMode &mode);
std::istream & operator >> (std::istream &is, WifiMode &mode);

ATTRIBUTE_HELPER_HEADER (WifiMode);

/**
 * In various parts of the code, folk are interested in maintaining a
 * list of transmission modes. The vector class provides a good basis
 * for this, but we here add some syntactic sugar by defining a
 * WifiModeList type, and a corresponding iterator.
 */
typedef std::vector<WifiMode> WifiModeList;
/**
 * An iterator for WifiModeList vector.
 */
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
   * \param codingRate if convolutional coding is used for this rate
   *        then this parameter specifies the convolutional coding rate
   *        used. If there is no explicit convolutional coding step (e.g.,
   *        for DSSS rates) then the caller should set this parameter to
   *        WIFI_CODE_RATE_UNCODED.
   * \param constellationSize the order of the constellation used.
   *
   * \return WifiMode
   *
   * Create a WifiMode (not used for HT or VHT).
   */
  static WifiMode CreateWifiMode (std::string uniqueName,
                                  WifiModulationClass modClass,
                                  bool isMandatory,
                                  WifiCodeRate codingRate,
                                  uint16_t constellationSize);

  /**
   * \param uniqueName the name of the associated WifiMode. This name
   *        must be unique accross _all_ instances.
   * \param mcsValue the mcs value
   * \param modClass the class of modulation
   *
   * \return WifiMode
   *
   * Create a HT or VHT WifiMode.
   */
  static WifiMode CreateWifiMcs (std::string uniqueName,
                                 uint8_t mcsValue,
                                 WifiModulationClass modClass);


private:
  /// allow WifiMode class access
  friend class WifiMode;
  friend std::istream & operator >> (std::istream &is, WifiMode &mode);

  /**
   * Return a WifiModeFactory
   *
   * \return a WifiModeFactory
   */
  static WifiModeFactory* GetFactory ();
  WifiModeFactory ();

  /**
   * This is the data associated to a unique WifiMode.
   * The integer stored in a WifiMode is in fact an index
   * in an array of WifiModeItem objects.
   */
  struct WifiModeItem
  {
    std::string uniqueUid; ///< unique UID
    WifiModulationClass modClass; ///< modulation class
    uint16_t constellationSize; ///< constellation size
    WifiCodeRate codingRate; ///< coding rate
    bool isMandatory; ///< flag to indicate whether this mode is mandatory
    uint8_t mcsValue; ///< MCS value
  };

  /**
   * Search and return WifiMode from a given name.
   *
   * \param name human-readable WifiMode
   *
   * \return WifiMode
   */
  WifiMode Search (std::string name) const;
  /**
   * Allocate a WifiModeItem from a given uniqueUid.
   *
   * \param uniqueUid
   *
   * \return uid
   */
  uint32_t AllocateUid (std::string uniqueUid);
  /**
   * Return a WifiModeItem at the given uid index.
   *
   * \param uid
   *
   * \return WifiModeItem at the given uid
   */
  WifiModeItem* Get (uint32_t uid);

  /**
   * typedef for a vector of WifiModeItem.
   */
  typedef std::vector<WifiModeItem> WifiModeItemList;
  WifiModeItemList m_itemList; ///< item list
};

} //namespace ns3

#endif /* WIFI_MODE_H */
