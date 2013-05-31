/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 * Author: Leonard Tracy <lentracy@gmail.com>
 */

#include "uan-prop-model.h"
#include "ns3/nstime.h"
#include <complex>
#include <vector>


namespace ns3 {

std::ostream &
operator<< (std::ostream &os, UanPdp &pdp)
{
  os << pdp.GetNTaps () << '|';
  os << pdp.GetResolution ().GetSeconds () << '|';

  UanPdp::Iterator it = pdp.m_taps.begin ();
  for (; it != pdp.m_taps.end (); it++)
    {
      os << (*it).GetAmp () << '|';
    }
  return os;

}
std::istream &
operator>> (std::istream &is, UanPdp &pdp)
{
  uint32_t ntaps;
  double resolution;
  char c1;

  is >> ntaps >> c1;
  if (c1 != '|')
    {
      NS_FATAL_ERROR ("UanPdp data corrupted at # of taps");
      return is;
    }
  is >> resolution >> c1;
  if (c1 != '|')
    {
      NS_FATAL_ERROR ("UanPdp data corrupted at resolution");
      return is;
    }
  pdp.m_resolution = Seconds (resolution);


  std::complex<double> amp;
  pdp.m_taps = std::vector<Tap> (ntaps);
  for (uint32_t i = 0; i < ntaps && !is.eof (); i++)
    {
      is >> amp >> c1;
      if (c1 != '|')
        {
          NS_FATAL_ERROR ("UanPdp data corrupted at tap " << i);
          return is;
        }
      pdp.m_taps[i] = Tap (Seconds (resolution * i), amp);
    }
  return is;

}

Tap::Tap ()
  : m_amplitude (0.0),
    m_delay (Seconds (0))
{

}

Tap::Tap (Time delay, std::complex<double> amp)
  : m_amplitude (amp),
    m_delay (delay)
{

}

std::complex<double>
Tap::GetAmp (void) const
{
  return m_amplitude;
}

Time
Tap::GetDelay (void) const
{
  return m_delay;
}


UanPdp::UanPdp ()
{

}

UanPdp::UanPdp (std::vector<Tap> taps, Time resolution)
  : m_taps (taps),
    m_resolution (resolution)
{
}

UanPdp::UanPdp (std::vector<std::complex<double> > amps, Time resolution)
  : m_resolution (resolution)
{
  m_taps.resize (amps.size ());
  Time arrTime = Seconds (0);
  for (uint32_t index = 0; index < amps.size (); index++)
    {
      m_taps[index] = Tap (arrTime, amps[index]);
      arrTime = arrTime + m_resolution;
    }
}

UanPdp::UanPdp (std::vector<double> amps, Time resolution)
  : m_resolution (resolution)
{
  m_taps.resize (amps.size ());
  Time arrTime = Seconds (0);
  for (uint32_t index = 0; index < amps.size (); index++)
    {
      m_taps[index] = Tap (arrTime, amps[index]);
      arrTime = arrTime + m_resolution;
    }
}

UanPdp::~UanPdp ()
{
  m_taps.clear ();
}

void
UanPdp::SetTap (std::complex<double> amp, uint32_t index)
{
  if (m_taps.size () <= index)
    {
      m_taps.resize (index + 1);
    }

  Time delay = Seconds (index * m_resolution.GetSeconds ());
  m_taps[index] = Tap (delay, amp);
}
const Tap &
UanPdp::GetTap (uint32_t i) const
{
  NS_ASSERT_MSG (i < GetNTaps (), "Call to UanPdp::GetTap with requested tap out of range");
  return m_taps[i];
}
void
UanPdp::SetNTaps (uint32_t nTaps)
{
  m_taps.resize (nTaps);
}
void
UanPdp::SetResolution (Time resolution)
{
  m_resolution = resolution;
}
UanPdp::Iterator
UanPdp::GetBegin (void) const
{
  return m_taps.begin ();
}

UanPdp::Iterator
UanPdp::GetEnd (void) const
{
  return m_taps.end ();
}

uint32_t
UanPdp::GetNTaps (void) const
{
  return m_taps.size ();
}

Time
UanPdp::GetResolution (void) const
{
  return m_resolution;
}

std::complex<double>
UanPdp::SumTapsFromMaxC (Time delay, Time duration) const
{
  if (m_resolution <= Seconds (0))
    {
      NS_ASSERT_MSG (GetNTaps () == 1, "Attempted to sum taps over time interval in "
                     "UanPdp with resolution 0 and multiple taps");

      return m_taps[0].GetAmp ();
    }

  uint32_t numTaps =  static_cast<uint32_t> (duration.GetSeconds () / m_resolution.GetSeconds () + 0.5);
  double maxAmp = -1;
  uint32_t maxTapIndex = 0;

  for (uint32_t i = 0; i < GetNTaps (); i++)
    {
      if (std::abs (m_taps[i].GetAmp ()) > maxAmp)
        {
          maxAmp = std::abs (m_taps[i].GetAmp ());
          maxTapIndex = i;
        }
    }
  uint32_t start = maxTapIndex + static_cast<uint32_t> (delay.GetSeconds () / m_resolution.GetSeconds ());
  uint32_t end = std::min (start + numTaps, GetNTaps ());
  std::complex<double> sum = 0;
  for (uint32_t i = start; i < end; i++)
    {
      sum += m_taps[i].GetAmp ();
    }
  return sum;
}
double
UanPdp::SumTapsFromMaxNc (Time delay, Time duration) const
{
  if (m_resolution <= Seconds (0))
    {
      NS_ASSERT_MSG (GetNTaps () == 1, "Attempted to sum taps over time interval in "
                     "UanPdp with resolution 0 and multiple taps");

      return std::abs (m_taps[0].GetAmp ());
    }

  uint32_t numTaps =  static_cast<uint32_t> (duration.GetSeconds () / m_resolution.GetSeconds () + 0.5);
  double maxAmp = -1;
  uint32_t maxTapIndex = 0;

  for (uint32_t i = 0; i < GetNTaps (); i++)
    {
      if (std::abs (m_taps[i].GetAmp ()) > maxAmp)
        {
          maxAmp = std::abs (m_taps[i].GetAmp ());
          maxTapIndex = i;
        }
    }


  uint32_t start = maxTapIndex + static_cast<uint32_t> (delay.GetSeconds () / m_resolution.GetSeconds ());
  uint32_t end = std::min (start + numTaps, GetNTaps ());
  double sum = 0;
  for (uint32_t i = start; i < end; i++)

    {
      sum += std::abs (m_taps[i].GetAmp ());
    }
  return sum;
}
double
UanPdp::SumTapsNc (Time begin, Time end) const
{
  if (m_resolution <= Seconds (0))
    {
      NS_ASSERT_MSG (GetNTaps () == 1, "Attempted to sum taps over time interval in "
                     "UanPdp with resolution 0 and multiple taps");

      if (begin <= Seconds (0.0) && end >= Seconds (0.0))
        {
          return std::abs (m_taps[0].GetAmp ());
        }
      else
        {
          return 0.0;
        }
    }

  uint32_t stIndex = (uint32_t)(begin.GetSeconds () / m_resolution.GetSeconds () + 0.5);
  uint32_t endIndex = (uint32_t)(end.GetSeconds () / m_resolution.GetSeconds () + 0.5);

  endIndex = std::min (endIndex, GetNTaps ());
  double sum = 0;
  for (uint32_t i = stIndex; i < endIndex; i++)
    {
      sum += std::abs (m_taps[i].GetAmp ());
    }
  return sum;

}



std::complex<double>
UanPdp::SumTapsC (Time begin, Time end) const
{
  if (m_resolution <= Seconds (0))
    {
      NS_ASSERT_MSG (GetNTaps () == 1, "Attempted to sum taps over time interval in "
                     "UanPdp with resolution 0 and multiple taps");

      if (begin <= Seconds (0.0) && end >= Seconds (0.0))
        {
          return m_taps[0].GetAmp ();
        }
      else
        {
          return std::complex<double> (0.0);
        }
    }

  uint32_t stIndex = (uint32_t)(begin.GetSeconds () / m_resolution.GetSeconds () + 0.5);
  uint32_t endIndex = (uint32_t)(end.GetSeconds () / m_resolution.GetSeconds () + 0.5);

  endIndex = std::min (endIndex, GetNTaps ());

  std::complex<double> sum = 0;
  for (uint32_t i = stIndex; i < endIndex; i++)
    {
      sum += m_taps[i].GetAmp ();
    }
  return sum;
}

UanPdp
UanPdp::CreateImpulsePdp (void)
{
  UanPdp pdp;
  pdp.SetResolution (Seconds (0));
  pdp.SetTap (1.0,0);
  return pdp;
}

NS_OBJECT_ENSURE_REGISTERED (UanPropModel);

TypeId UanPropModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UanPropModel")
    .SetParent<Object> ();
  return tid;
}

void
UanPropModel::Clear (void)
{
}

void
UanPropModel::DoDispose (void)
{
  Clear ();
  Object::DoDispose ();
}

}
