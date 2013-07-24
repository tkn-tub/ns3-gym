/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2009 INESC Porto
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: Pedro Fortuna  <pedro.fortuna@inescporto.pt> <pedro.fortuna@gmail.com>
//

#ifndef NS3_HISTOGRAM_H
#define NS3_HISTOGRAM_H

#include <vector>
#include <stdint.h>
#include <ostream>

namespace ns3 {

class Histogram
{
public:

  // --- basic methods ---
  Histogram (double binWidth);
  Histogram ();

  // Methods for Getting the Histogram Results
  uint32_t GetNBins () const;
  double GetBinStart (uint32_t index);
  double GetBinEnd (uint32_t index);
  double GetBinWidth (uint32_t index) const;
  void SetDefaultBinWidth (double binWidth);
  uint32_t GetBinCount (uint32_t index);

  // Method for adding values
  void AddValue (double value);


  void SerializeToXmlStream (std::ostream &os, int indent, std::string elementName) const;

  /// \todo add method(s) to estimate N, µ, and s² from the histogram,
  /// see http://www.dspguide.com/ch2/4.htm

private:
  std::vector<uint32_t> m_histogram;
  double m_binWidth;
};


} // namespace ns3

#endif /* NS3_HISTOGRAM_H */
