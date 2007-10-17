#ifndef WIFI_MODE_H
#define WIFI_MODE_H

#include <stdint.h>
#include <string>
#include <vector>
#include <ostream>

namespace ns3 {

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

  // create an invalid WifiMode.
  WifiMode ();
private:
  friend class WifiModeFactory;
  WifiMode (uint32_t uid);
  uint32_t m_uid;
};

bool operator == (const WifiMode &a, const WifiMode &b);
std::ostream & operator << (std::ostream & os, const WifiMode &mode);

class WifiModeFactory
{
public:
  static WifiMode CreateBpsk (std::string uniqueName,
			      bool isMandatory,
			      uint32_t bandwidth,
			      uint32_t dataRate,
			      uint32_t phyRate);
  static WifiMode CreateQam (std::string uniqueName,
			      bool isMandatory,
			     uint32_t bandwidth,
			     uint32_t dataRate,
			     uint32_t phyRate,
			     uint8_t constellationSize);
private:
  friend class WifiMode;  
  static WifiModeFactory *GetFactory ();
  WifiModeFactory ();

  struct WifiModeItem {
    std::string uniqueUid;
    uint32_t bandwidth;
    uint32_t dataRate;
    uint32_t phyRate;
    enum WifiMode::ModulationType modulation;
    uint8_t constellationSize;
    bool isMandatory;
  };

  uint32_t AllocateUid (std::string uniqueName);
  WifiModeItem *Get (uint32_t uid);

  typedef std::vector<struct WifiModeItem> WifiModeItemList;
  WifiModeItemList m_itemList;
};

} // namespace ns3

#endif /* WIFI_MODE_H */
