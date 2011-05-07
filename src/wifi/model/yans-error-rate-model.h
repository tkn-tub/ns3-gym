/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
#ifndef YANS_ERROR_RATE_MODEL_H
#define YANS_ERROR_RATE_MODEL_H

#include <stdint.h>
#include "wifi-mode.h"
#include "error-rate-model.h"
#include "dsss-error-rate-model.h"

namespace ns3 {

/**
 * \brief Model the error rate for different modulations.
 * \ingroup wifi
 *
 * A packet of interest (e.g., a packet can potentially be received by the MAC)
 * is divided into chunks. Each chunk is related to an start/end receiving event.
 * For each chunk, it calculates the ratio (SINR) between received power of packet
 * of interest and summation of noise and interfering power of all the other incoming
 * packets. Then, it will calculate the success rate of the chunk based on
 * BER of the modulation. The success reception rate of the packet is derived from
 * the success rate of all chunks.
 *
 * The 802.11b modulations:
 *    - 1 Mbps mode is based on DBPSK. BER is from equation 5.2-69 from John G. Proakis
 *      Digitial Communications, 2001 edition
 *    - 2 Mbps model is based on DQPSK. Equation 8 from "Tight bounds and accurate
 *      approximations for dqpsk transmission bit error rate", G. Ferrari and G.E. Corazza
 *      ELECTRONICS LETTERS, 40(20):1284-1285, September 2004
 *    - 5.5 Mbps and 11 Mbps are based on equations (18) and (17) from "Properties and
 *      performance of the ieee 802.11b complementarycode-key signal sets",
 *      Michael B. Pursley and Thomas C. Royster. IEEE TRANSACTIONS ON COMMUNICATIONS,
 *      57(2):440-449, February 2009.
 *    - More detailed description and validation can be found in
 *      http://www.nsnam.org/~pei/80211b.pdf
 */
class YansErrorRateModel : public ErrorRateModel
{
public:
  static TypeId GetTypeId (void);

  YansErrorRateModel ();

  virtual double GetChunkSuccessRate (WifiMode mode, double snr, uint32_t nbits) const;

private:
  double Log2 (double val) const;
  double GetBpskBer (double snr, uint32_t signalSpread, uint32_t phyRate) const;
  double GetQamBer (double snr, unsigned int m, uint32_t signalSpread, uint32_t phyRate) const;
  uint32_t Factorial (uint32_t k) const;
  double Binomial (uint32_t k, double p, uint32_t n) const;
  double CalculatePdOdd (double ber, unsigned int d) const;
  double CalculatePdEven (double ber, unsigned int d) const;
  double CalculatePd (double ber, unsigned int d) const;
  double GetFecBpskBer (double snr, double nbits,
                        uint32_t signalSpread, uint32_t phyRate,
                        uint32_t dFree, uint32_t adFree) const;
  double GetFecQamBer (double snr, uint32_t nbits,
                       uint32_t signalSpread,
                       uint32_t phyRate,
                       uint32_t m, uint32_t dfree,
                       uint32_t adFree, uint32_t adFreePlusOne) const;
};


} // namespace ns3

#endif /* YANS_ERROR_RATE_MODEL_H */
