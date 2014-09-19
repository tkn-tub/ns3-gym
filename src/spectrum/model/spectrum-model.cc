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

#include <cmath>
#include <cstddef>
#include <ns3/spectrum-model.h>
#include <ns3/log.h>
#include <ns3/assert.h>



NS_LOG_COMPONENT_DEFINE ("SpectrumModel");



namespace ns3 {


bool operator== (const SpectrumModel& lhs, const SpectrumModel& rhs)
{
  return (lhs.m_uid == rhs.m_uid);
}

SpectrumModelUid_t SpectrumModel::m_uidCount = 0;

SpectrumModel::SpectrumModel (std::vector<double> centerFreqs)
{
  NS_ASSERT (centerFreqs.size () > 1);
  m_uid = ++m_uidCount;

  for (std::vector<double>::const_iterator it = centerFreqs.begin ();
       it != centerFreqs.end ();
       ++it)
    {
      BandInfo e;
      e.fc = *it;
      if (it ==  centerFreqs.begin ())
        {
          double delta = ((*(it + 1)) - (*it)) / 2;
          e.fl =  *it - delta;
          e.fh =  *it + delta;
        }
      else if (it == centerFreqs.end () - 1 )
        {
          double delta = ((*it) - (*(it - 1))) / 2;
          e.fl  =  *it - delta;
          e.fh =  *it + delta;
        }
      else
        {
          e.fl  = ((*it) + (*(it - 1))) / 2;
          e.fh = ((*(it + 1)) + (*it)) / 2;
        }
      m_bands.push_back (e);
    }
}

SpectrumModel::SpectrumModel (Bands bands)
{
  m_uid = ++m_uidCount;
  NS_LOG_INFO ("creating new SpectrumModel, m_uid=" << m_uid);
  m_bands = bands;
}

Bands::const_iterator
SpectrumModel::Begin () const
{
  return m_bands.begin ();
}

Bands::const_iterator
SpectrumModel::End () const
{
  return m_bands.end ();
}

size_t
SpectrumModel::GetNumBands () const
{
  return m_bands.size ();
}

SpectrumModelUid_t
SpectrumModel::GetUid () const
{
  return m_uid;
}



} // namespace ns3
