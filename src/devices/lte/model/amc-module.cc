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


#include "amc-module.h"
#include <ns3/log.h>
#include <math.h>

NS_LOG_COMPONENT_DEFINE ("AmcModule");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (AmcModule);



int CqiIndex[15] = {
  1, 2, 3, 4, 5, 6,                    // QAM
  7, 8, 9,                             // 4-QAM
  10, 11, 12, 13, 14, 15               // 16QAM
};


double SpectralEfficiencyForCqiIndex[15] = {
  0.15, 0.23, 0.38, 0.6, 0.88, 1.18,
  1.48, 1.91, 2.41,
  2.73, 3.32, 3.9, 4.52, 5.12, 5.55
};

int McsIndex[32] = {
  0,                                                        // RESERVED
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,                        // QAM
  12, 13, 14, 15, 16, 17, 18,                               // 4-QAM
  19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,               // 16-QAM
  30,                                                       // QAM, RESERVED
  31                                                        // RESERVED
};

int ModulationSchemeForMcsIndex[32] = {
  0,                                                        // Not defined
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  4, 4, 4, 4, 4, 4, 4,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  2,
  0                                                         // Not defined
};

double SpectralEfficiencyForMcsIndex[32] = {
  0,
  0.15, 0.19, 0.23, 0.31, 0.38, 0.49, 0.6, 0.74, 0.88, 1.03, 1.18,
  1.33, 1.48, 1.7, 1.91, 2.16, 2.41, 2.57,
  2.73, 3.03, 3.32, 3.61, 3.9, 4.21, 4.52, 4.82, 5.12, 5.33, 5.55,
  2.4,
  0
};

int TransportBlockSize[32] = {
  0,
  18, 23, 28, 37, 45, 59, 72, 89, 105, 123, 141,
  159, 177, 203, 230, 259, 289, 288,
  308, 328, 363, 399, 433, 468, 506, 543, 578, 614, 640,
  667,
  0
};


AmcModule::AmcModule ()
{
  Initialize ();
}


TypeId
AmcModule::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::AmcModule")
    .SetParent<Object> ()
    .AddConstructor<AmcModule> ()
  ;
  return tid;
}


AmcModule::~AmcModule ()
{

}

void
AmcModule::Initialize ()
{
  NS_LOG_FUNCTION (this);
}


int
AmcModule::GetCqiFromSpectralEfficiency (double s)
{
  NS_LOG_FUNCTION (this << s);
  int cqi = 1; // == CqiIndex[0]
  while (SpectralEfficiencyForCqiIndex[cqi] < s && cqi <= 14)
    {
      cqi++;
    }
  NS_LOG_FUNCTION (this << s << cqi);
  return cqi;
}


int
AmcModule::GetMcsFromCqi (int cqi)
{
  NS_LOG_FUNCTION (this << cqi);
  double spectralEfficiency = SpectralEfficiencyForCqiIndex[cqi - 1];
  int mcs = 1;
  while (SpectralEfficiencyForMcsIndex[mcs] < spectralEfficiency && mcs < 30)
    {
      mcs++;
    }
  NS_LOG_FUNCTION (this << cqi << mcs);
  return mcs;
}


int
AmcModule::GetTbSizeFromMcs (int mcs)
{
  NS_LOG_FUNCTION (this << mcs);
  NS_LOG_FUNCTION (this << mcs << TransportBlockSize[mcs]);
  return TransportBlockSize[mcs];
}


double
AmcModule::GetSpectralEfficiencyFromCqi (int cqi)
{
  NS_LOG_FUNCTION (this << cqi);
  NS_LOG_FUNCTION (this << cqi << SpectralEfficiencyForCqiIndex[cqi - 1]);
  return SpectralEfficiencyForCqiIndex[cqi - 1];
}


std::vector<int>
AmcModule::CreateCqiFeedbacks (std::vector<double> sinr)
{
  NS_LOG_FUNCTION (this);

  std::vector<int> cqi;
  std::vector<double>::iterator it;

  for (it = sinr.begin (); it != sinr.end (); it++)
    {
      double sinr_ = (*it);

      /*
       * Compute the spectral efficiency from the SINR
       *                                        SINR
       * spectralEfficiency = log2 (1 + -------------------- )
       *                                    -ln(5*BER)/1.5
       * NB: SINR must be expressed in natural unit:
       * (SINR)dB => 10 ^ (SINR/10)
       */

      double s = log2 ( 1 + (
                          pow (10, sinr_ / 10 )  /
                          ( (-log (5.0 * 0.00005 )) / 1.5) ));

      int cqi_ = GetCqiFromSpectralEfficiency (s);

      NS_LOG_FUNCTION (this << "channel_id = " << cqi.size ()
                            << "sinr = " << sinr_
                            << "spectral efficiency =" << s
                            << " ---- CQI = " << cqi_ );

      cqi.push_back (cqi_);
    }

  return cqi;
}

} // namespace ns3
