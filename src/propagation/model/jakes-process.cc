/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Telum (www.telum.ru)
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
 * Author: Kirill Andreev <andreev@telum.ru>, Alexander Sofronov <sofronov@telum.ru>
 */

#include "jakes-process.h"
#include "ns3/random-variable.h"
#include "ns3/simulator.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"

namespace ns3 {
const double JakesProcess::PI = 3.14159265358979323846;

/// Represents a single oscillator
JakesProcess::Oscillator::Oscillator (std::complex<double> amplitude, double initialPhase, double omega) :
  m_amplitude (amplitude),
  m_phase (initialPhase),
  m_omega (omega)
{}

std::complex<double>
JakesProcess::Oscillator::GetValueAt (Time at) const
{
  return (m_amplitude * cos (at.GetSeconds () * m_omega + m_phase));
}

NS_OBJECT_ENSURE_REGISTERED (JakesProcess);

TypeId
JakesProcess::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::JakesProcess")
    .SetParent<Object> ()
    .AddConstructor<JakesProcess> ()
    .AddAttribute ("DopplerFrequencyHz", "Corresponding doppler frequency[Hz]",
                   DoubleValue (80),
                   MakeDoubleAccessor (&JakesProcess::SetDopplerFrequencyHz),
                   MakeDoubleChecker<double> (0.0, 1e4))
    .AddAttribute ("NumberOfOscillators", "The number of oscillators",
                   UintegerValue (20),
                   MakeUintegerAccessor (&JakesProcess::SetNOscillators),
                   MakeUintegerChecker<unsigned int> (4, 1000))
  ;
  return tid;
}

void
JakesProcess::SetNOscillators (unsigned int nOscillators)
{
  m_nOscillators = nOscillators;
  if (m_omegaDopplerMax != 0)
    {
      ConstructOscillators ();
    }
}

void
JakesProcess::SetDopplerFrequencyHz (double dopplerFrequencyHz)
{
  m_omegaDopplerMax = 2 * dopplerFrequencyHz * PI;
  if (m_nOscillators != 0)
    {
      ConstructOscillators ();
    }
}

void
JakesProcess::ConstructOscillators ()
{
  // Initial phase is common for all oscillators:
  double phi = UniformVariable (-PI, PI).GetValue ();
  // Theta is common for all oscillatoer:
  double theta = UniformVariable (-PI, PI).GetValue ();
  for (unsigned int i = 0; i < m_nOscillators; i++)
    {
      unsigned int n = i + 1;
      /// 1. Rotation speed
      /// 1a. Initiate \f[ \alpha_n = \frac{2\pi n - \pi + \theta}{4M},  n=1,2, \ldots,M\f], n is oscillatorNumber, M is m_nOscillators
      double alpha = (2.0 * PI * n - PI + theta) / (4.0 * m_nOscillators);
      /// 1b. Initiate rotation speed:
      double omega = m_omegaDopplerMax * cos (alpha);
      /// 2. Initiate complex amplitude:
      double psi = UniformVariable (-PI, PI).GetValue ();
      std::complex<double> amplitude = std::complex<double> (cos (psi), sin (psi)) * 2.0 / sqrt (m_nOscillators);
      /// 3. Construct oscillator:
      m_oscillators.push_back (Oscillator (amplitude, phi, omega)); 
    }
}

JakesProcess::JakesProcess () :
  m_omegaDopplerMax (0),
  m_nOscillators (0)
{}

JakesProcess::~JakesProcess()
{
  m_oscillators.clear ();
}

std::complex<double>
JakesProcess::GetComplexGain () const
{
  std::complex<double> sumAplitude = std::complex<double> (0, 0);
  for (unsigned int i = 0; i < m_oscillators.size (); i++)
    {
      sumAplitude += m_oscillators[i].GetValueAt (Now ());
    }
  return sumAplitude;
}

double
JakesProcess::GetChannelGainDb () const
{
  std::complex<double> complexGain = GetComplexGain ();
  return (10 * log10 ((pow (complexGain.real (), 2) + pow (complexGain.imag (), 2)) / 2));
}

} // namespace ns3
