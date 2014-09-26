/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

#include <ns3/spectrum-converter.h>
#include <ns3/assert.h>
#include <ns3/log.h>
#include <algorithm>



namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("SpectrumConverter");

SpectrumConverter::SpectrumConverter ()
{
}

SpectrumConverter::SpectrumConverter (Ptr<const SpectrumModel> fromSpectrumModel, Ptr<const SpectrumModel> toSpectrumModel)
{
  NS_LOG_FUNCTION (this);
  m_fromSpectrumModel = fromSpectrumModel;
  m_toSpectrumModel = toSpectrumModel;

  for (Bands::const_iterator toit = toSpectrumModel->Begin (); toit != toSpectrumModel->End (); ++toit)
    {
      std::vector<double> coeffs;

      for (Bands::const_iterator fromit = fromSpectrumModel->Begin (); fromit != fromSpectrumModel->End (); ++fromit)
        {
          double c = GetCoefficient (*fromit, *toit);
          NS_LOG_LOGIC ("(" << fromit->fl << ","  << fromit->fh << ")"
                            << " --> " <<
                        "(" << toit->fl << "," << toit->fh << ")"
                            << " = " << c);
          coeffs.push_back (c);
        }

      m_conversionMatrix.push_back (coeffs);
    }

}


double SpectrumConverter::GetCoefficient (const BandInfo& from, const BandInfo& to) const
{
  NS_LOG_FUNCTION (this);
  double coeff = std::min (from.fh, to.fh) - std::max (from.fl, to.fl);
  coeff = std::max (0.0, coeff);
  coeff = std::min (1.0, coeff / (to.fh - to.fl));
  return coeff;
}



Ptr<SpectrumValue>
SpectrumConverter::Convert (Ptr<const SpectrumValue> fvvf) const
{
  NS_ASSERT ( *(fvvf->GetSpectrumModel ()) == *m_fromSpectrumModel);

  Ptr<SpectrumValue> tvvf = Create<SpectrumValue> (m_toSpectrumModel);

  Values::iterator tvit = tvvf->ValuesBegin ();


  for (std::vector<std::vector<double> >::const_iterator toit = m_conversionMatrix.begin ();
       toit != m_conversionMatrix.end ();
       ++toit)
    {
      NS_ASSERT (tvit != tvvf->ValuesEnd ());
      Values::const_iterator fvit = fvvf->ConstValuesBegin ();

      double sum = 0;
      for (std::vector<double>::const_iterator fromit = toit->begin ();
           fromit != toit->end ();
           ++fromit)
        {
          NS_ASSERT (fvit != fvvf->ConstValuesEnd ());
          sum += (*fvit) * (*fromit);
          ++fvit;
        }
      *tvit = sum;
      ++tvit;
    }

  return tvvf;
}





} // namespace ns3
