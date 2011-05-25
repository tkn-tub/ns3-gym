/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright (c) 2007,2008, 2009 INRIA, UDcast
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
 * Author: Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 *                              <amine.ismail@udcast.com>
 */


#ifndef SNRTOBLOCKERRORRATEMANAGER_H_
#define SNRTOBLOCKERRORRATEMANAGER_H_

#include "ns3/snr-to-block-error-rate-record.h"
#include <vector>
#include "ns3/ptr.h"

namespace ns3 {

/**
 * \ingroup wimax
 * \brief This class handles the  SNR to BlcER traces.  A path to a repository containing trace files should be provided.
 * If no repository is provided the traces form default-traces.h will be loaded.
 * A valid repository should contain 7 files, one for each modulation and coding scheme.
 * The names of the files should respect the following format: modulation0.txt for modulation 0, modulation1.txt for
 * modulation 1 and so on...
 * The files format should be as follows
 * SNR_value(1)   BER(1)    Blc_ER(1)    STANDARD_DEVIATION(1)    CONFIDENCE_INTERVAL1(1)    CONFIDENCE_INTERVAL2(1)
 * SNR_value(2)   BER(2)    Blc_ER(2)    STANDARD_DEVIATION(2)    CONFIDENCE_INTERVAL1(2)    CONFIDENCE_INTERVAL2(2)
 *  ...           ...       ...          ...                      ...                        ...
 *  ...           ...       ...          ...                      ...                        ...
 * SNR_value(n)   BER(n)    Blc_ER(n)    STANDARD_DEVIATION(n)    CONFIDENCE_INTERVAL1(n)    CONFIDENCE_INTERVAL2(n)
 */
class SNRToBlockErrorRateManager
{
public:
  SNRToBlockErrorRateManager ();
  ~SNRToBlockErrorRateManager (void);
  /**
   * \brief Set the path of the repository containing the traces
   * \param traceFilePath the path to the repository.
   */
  void SetTraceFilePath (char *traceFilePath);
  /**
   * \return the path to the repository containing the traces.
   */
  std::string GetTraceFilePath (void);
  /**
   * \brief returns the Block Error Rate for a given modulation and SNR value
   * \param SNR the SNR value
   * \param modulation one of the seven MCS
   * \return the Block Error Rate
   */
  double GetBlockErrorRate (double SNR, uint8_t modulation);
  SNRToBlockErrorRateRecord *
  /**
   * \brief returns a record of type SNRToBlockErrorRateRecord corresponding to a given modulation and SNR value
   * \param SNR the SNR value
   * \param modulation one of the seven MCS
   * \return the Block Error Rate
   */
  GetSNRToBlockErrorRateRecord (double SNR, uint8_t modulation);
  /**
   * \brief Loads the traces form the repository specified in the constructor or setted by SetTraceFilePath function. If
   * no repository is provided, default traces will be loaded from default-traces.h file
   */

  void LoadTraces (void);
  /**
   * \brief Loads the default traces from default-traces.h file
   */
  void LoadDefaultTraces (void);
  /**
   * \brief Reloads the trace
   */
  void ReLoadTraces (void);
  /**
   * \brief If activate loss is called with false, all the returned BlcER will be 0 (no losses)
   */
  void ActivateLoss (bool loss);
private:
  void ClearRecords (void);
  double m_speed; // in m/s
  uint8_t m_activateLoss;
  char m_traceFilePath[1024];

  std::vector<SNRToBlockErrorRateRecord *> * m_recordModulation[7];

};
}

#endif /* SNRTOBLOCKERRORRATEMANAGER_H_ */
