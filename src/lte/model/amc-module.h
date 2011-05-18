/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */

#ifndef AMCMODULE_H
#define AMCMODULE_H

#include "ns3/object.h"
#include <vector>

namespace ns3 {


/**
 * \ingroup lte
 *
 * \brief The AMC module attached to the LTE networ device
 * AmcModule class implements the Adaptive Modulation And Coding Scheme
 * as proposed in 3GPP TSG-RAN WG1 - R1-081483
 * http://www.3gpp.org/ftp/tsg_ran/WG1_RL1/TSGR1_52b/Docs/R1-081483.zip
 */
class AmcModule : public Object
{

public:
  static TypeId GetTypeId (void);

  AmcModule ();
  virtual ~AmcModule ();

  /**
   * \brief Initialize CQI, MCS, SpectralEfficiency e TBs values
   */
  void Initialize ();

  /**
   * \brief Get the Modulation anc Coding Scheme for
   * a CQI value
   * \param cqi the cqi value
   * \return the MCS  value
   */
  int GetMcsFromCqi (int cqi);

  /**
   * \brief Get the Transport Block Size for a selected MCS
   * \param mcs the mcs index
   * \return the TBs value
   */
  int GetTbSizeFromMcs (int mcs);

  /**
   * \brief Get the spectral efficiency value associated
   * to the received CQI
   * \param cqi the cqi value
   * \return the spectral efficiency value
   */
  double GetSpectralEfficiencyFromCqi (int cqi);

  /**
   * \brief Create a message with CQI feedbaks
   */
  std::vector<int> CreateCqiFeedbacks (std::vector<double> sinr);

private:
  /**
   * \brief Get a proper CQI for the spectrale efficiency value.
   * In order to assure a fewer block error rate, the AMC chooses the lower CQI value
   * for a given spectral efficiency
   * \param s the spectral efficiency
   * \return the CQI value
   */
  int GetCqiFromSpectralEfficiency (double s);

};


}

#endif /* AMCMODULE_H */
