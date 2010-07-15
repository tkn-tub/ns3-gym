/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 * Author: Leonard Tracy <lentracy@gmail.com>
 */

#ifndef UANTXMODE_H
#define UANTXMODE_H

#include "ns3/object.h"
#include <map>

namespace ns3 {

class UanTxModeFactory;
class UanTxMode;

/**
 * \class UanTxMode
 * \brief Abstraction of packet modulation information
 */
class UanTxMode
{
public:
  UanTxMode ();
  ~UanTxMode ();

  /**
   * Modulation type
   */
  typedef enum {
    PSK, QAM, FSK, OTHER
  } ModulationType;
  /**
   * \returns The modulation type of the mode
   */
  ModulationType GetModType (void) const;
  /**
   * \returns Data rate of the TX mode in bits per second
   */
  uint32_t GetDataRateBps (void) const;
  /**
   * \returns PHY rate in symbols per second
   */
  uint32_t GetPhyRateSps (void) const;
  /**
   * \returns Center frequency of transmitted signal in Hz
   */
  uint32_t GetCenterFreqHz (void) const;
  /**
   * \returns Bandwidth of transmitted signal in Hz
   */
  uint32_t GetBandwidthHz (void) const;
  /**
   * \returns Number of constellation points in modulation
   */
  uint32_t GetConstellationSize (void) const;
  /**
   * \returns Name
   */
  std::string GetName (void) const;
  /**
   * \returns Unique ID
   */
  uint32_t GetUid (void) const;

private:
  friend class UanTxModeFactory;
  friend std::ostream &operator<< (std::ostream & os, const UanTxMode &mode);
  friend std::istream &operator>> (std::istream & is, UanTxMode &mode);


  uint32_t m_uid;

};
/**
 * \brief Writes tx mode entry to stream os
 */
std::ostream & operator << (std::ostream & os, const UanTxMode &mode);
/**
 * \brief Reads tx mode entry from stream is
 */
std::istream & operator >> (std::istream & is, const UanTxMode &mode);

class UanTxModeFactory
{
public:
  UanTxModeFactory ();
  ~UanTxModeFactory ();

  /**
   *
   * \param type modulation type
   * \param dataRateBps Data rate in BPS
   * \param phyRateSps  Symbol rate in symbols per second
   * \param cfHz Center frequency in Hz
   * \param bwHz Bandwidth in Hz
   * \param constSize Modulation constellation size (2 for BPSK, 4 for QPSK)
   * \param name Unique string name for this transmission mode
   */
  static UanTxMode CreateMode (UanTxMode::ModulationType type,
                               uint32_t dataRateBps,
                               uint32_t phyRateSps,
                               uint32_t cfHz,
                               uint32_t bwHz,
                               uint32_t constSize,
                               std::string name);

  /**
   * \param name String name of mode
   * \returns Mode with given name
   */
  static UanTxMode GetMode (std::string name);
  /**
   * \param uid Unique ID of mode
   * \returns The mode with given uid
   */
  static UanTxMode GetMode (uint32_t uid);
private:
  friend class UanTxMode;
  uint32_t m_nextUid;

  struct UanTxModeItem
  {
    UanTxMode::ModulationType m_type;
    uint32_t m_cfHz;
    uint32_t m_bwHz;
    uint32_t m_dataRateBps;
    uint32_t m_phyRateSps;
    uint32_t m_constSize;
    uint32_t m_uid;
    std::string m_name;
  };

  std::map<uint32_t, UanTxModeItem> m_modes;
  bool NameUsed (std::string name);
  static UanTxModeFactory &GetFactory (void);
  UanTxModeItem &GetModeItem (uint32_t uid);
  UanTxModeItem &GetModeItem (std::string name);
  UanTxMode MakeModeFromItem (const UanTxModeItem &item);

};

/**
 * \class UanModesList
 * \brief Container for UanTxModes
 */
class UanModesList 
{
public:
  UanModesList ();
  virtual ~UanModesList ();

  /**
   * \param mode Add mode to list
   */
  void AppendMode (UanTxMode mode);
  /**
   * \brief delete mode at given index
   * \param num Index of mode to delete
   */
  void DeleteMode (uint32_t num);
  /**
   * \param index Mode index
   * \returns mode at given index
   */
  UanTxMode operator[] (uint32_t index) const;
  /**
   * \returns Number of modes in list
   */
  uint32_t GetNModes (void) const;


private:
  std::vector<UanTxMode> m_modes;
  friend std::ostream &operator << (std::ostream &os, const UanModesList &ml);
  friend std::istream &operator >> (std::istream &is, UanModesList &ml);
};

/**
 * \brief Write UanModesList to stream os
 */
std::ostream &operator << (std::ostream &os, const UanModesList &ml);
/**
 * \brief Read UanModesList from stream is
 */
std::istream &operator >> (std::istream &is, UanModesList &ml);

///UanModesList is attribute value
ATTRIBUTE_HELPER_HEADER (UanModesList);

} // namespace ns3

#endif // UANTXMODE_H
