/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 The Boeing Company
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
 * Author:
 *  kwong yin <kwong-sang.yin@boeing.com>
 *  Sascha Alexander Jopen <jopen@cs.uni-bonn.de>
 */

#ifndef LR_WPAN_CSMACA_H
#define LR_WPAN_CSMACA_H

#include <ns3/object.h>
#include <ns3/random-variable.h>
#include <ns3/event-id.h>
#include <ns3/lr-wpan-mac.h>

namespace ns3 {

/* This method informs MAC that channel is idle or busy */
typedef Callback<void, LrWpanMacState> LrWpanMacStateCallback;

/**
 * \ingroup lr-wpan
 *
 * This class is a helper for the LrWpanMac to manage the Csma/CA
 * state machine.
 */
class LrWpanCsmaCa : public Object
{

public:
  static TypeId GetTypeId (void);

  LrWpanCsmaCa (void);
  virtual ~LrWpanCsmaCa (void);

  void SetMac (Ptr<LrWpanMac> mac);
  Ptr<LrWpanMac> GetMac (void) const;
  void SetSlottedCsmaCa (void);
  void SetUnSlottedCsmaCa (void);
  bool IsSlottedCsmaCa (void) const;
  bool IsUnSlottedCsmaCa (void) const;

  void SetMacMinBE (uint8_t macMinBE);
  uint8_t GetMacMinBE (void) const;
  void SetMacMaxBE (uint8_t macMaxBE);
  uint8_t GetMacMaxBE (void) const;
  void SetMacMaxCSMABackoffs (uint8_t macMaxCSMABackoffs);
  uint8_t GetMacMaxCSMABackoffs (void) const;

  void SetUnitBackoffPeriod (uint64_t unitBackoffPeriod);
  uint64_t GetUnitBackoffPeriod (void) const;

  /*
   *  Amount of time from now to the beginning of the next slot
   */
  uint64_t GetTimeToNextSlot (void) const;

  /*
   * Start CSMA-CA algorithm (step 1), initialize NB, BE for both slotted and unslotted
   * CSMA-CA. For the slotted intialize CW plus also start on the backoff boundary
   */
  void Start (void);

  /*
   * cancel CSMA-CA algorithm
   */
  void Cancel (void);

  /*
   * In step 2 of the CSMA-CA, perform a random backoff in the range of 0 to 2^BE -1
   */
  void RandomBackoffDelay (void);

  /*
   * In the slotted CSMA-CA, after random backoff, Determine if the remaining
   * CSMA-CA operation can proceed ie. Can the entire transactions can be transmitted
   * before the end of the CAP. This step is performed between step 2 and 3.
   * This step is NOT performed for the unslotted CSMA-CA. If it can proceed
   * function CCAconfirmed() is called.
   */
  void CanProceed (void);

  /*
   * Request the Phy to perform CCA (Step 3)
   *
   */
  void RequestCCA (void);

  /**
   *  IEEE 802.15.4-2006 section 6.2.2.2
   *  PLME-CCA.confirm status
   *  @param status TRX_OFF, BUSY or IDLE
   *
   * When Phy has completed CCA, it calls back here which in turn execute the final steps
   * of the CSMA-CA algorithm.
   * It checks to see if the Channel is idle, if so check the Contention window  before
   * permitting transmission (step 5). If channel is busy, either backoff and perform CCA again or
   * treat as channel access failure (step 4).
   */
  void PlmeCcaConfirm (LrWpanPhyEnumeration status);

  /**
    * set the callback function. Used at the end of a Channel Assessment, as part of the
    * interconnections between the CSMA-CA and the MAC. The callback
    * lets MAc know a channel is either idle or busy
    */
  void SetLrWpanMacStateCallback (LrWpanMacStateCallback macState);

private:
  virtual void DoDispose (void);
  LrWpanMacStateCallback m_lrWpanMacStateCallback;
  bool m_isSlotted;                     // beacon-enabled slotted or nonbeacon-enabled unslotted CSMA-CA
                                        // beacon order == 15 means nonbeacon-enabled

  Ptr<LrWpanMac> m_mac;
  uint8_t m_NB;                         // number of backoffs for the current transmission
  uint8_t m_CW;                         // contention window length (used in slotted ver only)
  uint8_t m_BE;                         // backoff exponent
  bool m_BLE;                           // Battery Life Extension

  uint8_t m_macMinBE;                   //0-macMaxBE default 3
  uint8_t m_macMaxBE;                   //3-8 default 5
  uint8_t m_macMaxCSMABackoffs;         //0-5 default 4
  uint64_t m_aUnitBackoffPeriod;        // 20 symbols in each backoff periods
  UniformVariable m_random;

  EventId m_randomBackoffEvent;
  EventId m_requestCcaEvent;
  EventId m_canProceedEvent;
  bool m_ccaRequestRunning;
};

}

// namespace ns-3

#endif /* LR_WPAN_CSMACA_H */
