/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 CTTC
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */

#ifndef SPECTRUM_MODEL_H
#define SPECTRUM_MODEL_H

#include <ns3/simple-ref-count.h>
#include <vector>

namespace ns3 {

/**
 * \defgroup spectrum Spectrum Models
 *
 */

/**
 * \ingroup spectrum
 *
 * The building block of a SpectrumModel. This struct models
 * a frequency band defined by the frequency interval [fl, fc] and
 * with center frequency fc. Typically, the center frequency will be
 * used for stuff such as propagation modeling, while the interval
 * boundaries will be used for bandwidth calculation and for
 * conversion between different SpectrumRepresentations.
 *
 */
struct BandInfo
{
  double fl; ///< lower limit of subband
  double fc; ///< center frequency
  double fh; ///< upper limit of subband
};


typedef std::vector<BandInfo> Bands;
typedef uint32_t SpectrumModelUid_t;

/**
 * Set of frequency values implementing the domain of the functions in
 * the Function Space defined by SpectrumValue. Frequency values are in
 * Hz. It is intended that frequency values are non-negative, though
 * this is not enforced.
 *
 */
class SpectrumModel : public SimpleRefCount<SpectrumModel>
{
public:
  friend bool operator== (const SpectrumModel& lhs, const SpectrumModel& rhs);

  /**
   * This constructs a SpectrumModel based on a given set of frequencies,
   * which is assumed to be sorted by increasing frequency. The lower
   * (resp. upper) frequency band limit is determined as the mean value
   * between the center frequency of the considered band and the
   * center frequency of the adjacent lower (resp. upper) band.
   *
   * @param centerFreqs the vector of center frequencies.
   *
   * @return
   */
  SpectrumModel (std::vector<double> centerFreqs);


  /**
   * This construct a SpectrumModel based on the explicit values of
   * center frequencies and boundaries of each subband.
   *
   * @param bands
   *
   * @return
   */
  SpectrumModel (Bands bands);

  /**
   *
   * @return the number of frequencies in this SpectrumModel
   */
  size_t GetNumBands () const;


  /**
   *
   * @return the unique id of this SpectrumModel
   */
  SpectrumModelUid_t GetUid () const;


  Bands::const_iterator Begin () const;
  Bands::const_iterator End () const;

private:
  Bands m_bands;         ///< actual definition of frequency bands
                         /// within this SpectrumModel
  SpectrumModelUid_t m_uid;        ///< unique id for a given set of frequencies
  static SpectrumModelUid_t m_uidCount;    ///< counter to assign m_uids
};


} // namespace ns3

#endif /* SPECTRUM_MODEL_H */
