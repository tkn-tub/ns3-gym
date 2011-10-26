/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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
 * Author: Federico Maguolo <maguolof@dei.unipd.it>
 */

#include "ns3/simulator.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/random-variable.h"
#include "ns3/mobility-model.h"
#include "ns3/log.h"
#include "jakes-propagation-loss-model.h"
#include <math.h>

NS_LOG_COMPONENT_DEFINE ("Jakes");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (JakesPropagationLossModel);

class JakesPropagationLossModel::PathCoefficients 
{
public:
  PathCoefficients (Ptr<const JakesPropagationLossModel> jakes,
                    Ptr<MobilityModel> receiver, 
                    uint8_t nRays, 
                    uint8_t nOscillators);
  ~PathCoefficients ();
  double GetLoss (Ptr<const JakesPropagationLossModel> jakes);
  Ptr<MobilityModel> GetReceiver (void);
private:
  void DoConstruct (Ptr<const JakesPropagationLossModel> jakes);
  Ptr<MobilityModel> m_receiver;
  uint8_t m_nOscillators;
  uint8_t m_nRays;
  double **m_phases;
  Time m_lastUpdate;
};


JakesPropagationLossModel::PathCoefficients::PathCoefficients (Ptr<const JakesPropagationLossModel> jakes, 
                                                               Ptr<MobilityModel> receiver,
                                                               uint8_t nRays,
                                                               uint8_t nOscillators)
  : m_receiver (receiver),
    m_nOscillators (nOscillators),
    m_nRays (nRays)
{
  DoConstruct (jakes);
}

JakesPropagationLossModel::PathCoefficients::~PathCoefficients ()
{
  for (uint8_t i = 0; i < m_nRays; i++) 
    {
      delete [] m_phases[i];
    }
  delete [] m_phases;
}

void
JakesPropagationLossModel::PathCoefficients::DoConstruct (Ptr<const JakesPropagationLossModel> jakes)
{
  m_phases = new double*[m_nRays];
  for (uint8_t i = 0; i < m_nRays; i++) 
    {
      m_phases[i] = new double[m_nOscillators + 1];
      for (uint8_t j = 0; j <= m_nOscillators; j++) 
        {
          m_phases[i][j] = 2.0 * JakesPropagationLossModel::PI * jakes->m_variable.GetValue ();
        }
    }
  m_lastUpdate = Simulator::Now ();
}

Ptr<MobilityModel>
JakesPropagationLossModel::PathCoefficients::GetReceiver ()
{
  return m_receiver;
}

double
JakesPropagationLossModel::PathCoefficients::GetLoss (Ptr<const JakesPropagationLossModel> jakes)
{
  uint16_t N = 4 * m_nOscillators + 2;
  Time interval = Simulator::Now () - m_lastUpdate;
  ComplexNumber coef= { 0.0, 0.0};
  ComplexNumber fading;
  double norm = 0.0;
  for (uint8_t i = 0; i < m_nRays; i++) 
    {
      fading.real = 0.0;
      fading.imag = 0.0;
      for (uint8_t j = 0; j <= m_nOscillators; j++) 
        {
          m_phases[i][j] += 2.0 * JakesPropagationLossModel::PI * 
            cos (2.0 * JakesPropagationLossModel::PI * j / N) * jakes->m_fd * interval.GetSeconds ();
          m_phases[i][j] -= 2.0 * JakesPropagationLossModel::PI * 
            floor (m_phases[i][j] / 2.0 / JakesPropagationLossModel::PI);
          fading.real += jakes->m_amp[j].real * cos (m_phases[i][j]);
          fading.imag += jakes->m_amp[j].imag * cos (m_phases[i][j]);
          norm += sqrt (pow (jakes->m_amp[j].real, 2) + pow (jakes->m_amp[j].imag, 2));
        }
      coef.real += fading.real;
      coef.imag += fading.imag;
    }
  m_lastUpdate = Simulator::Now ();
  double k = sqrt (pow (coef.real, 2) + pow (coef.imag, 2)) / norm;
  NS_LOG_DEBUG ("Jakes coef "<< k << " (" << 10 * log10 (k) << "dB)");
  return 10 * log10 (k);
}

const double JakesPropagationLossModel::PI = 3.14159265358979323846;

TypeId
JakesPropagationLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::JakesPropagationLossModel")
    .SetParent<PropagationLossModel> ()
    .AddConstructor<JakesPropagationLossModel> ()
    .AddAttribute ("NumberOfRaysPerPath",
                   "The number of rays to use by default for compute the fading coeficent for a given path (default is 1)",
                   UintegerValue (1),
                   MakeUintegerAccessor (&JakesPropagationLossModel::SetNRays,
                                         &JakesPropagationLossModel::GetNRays),
                   MakeUintegerChecker<uint8_t> ())
    .AddAttribute ("NumberOfOscillatorsPerRay",
                   "The number of oscillators to use by default for compute the coeficent for a given ray of a given "
                   "path (default is 4)",
                   UintegerValue (4),
                   MakeUintegerAccessor (&JakesPropagationLossModel::SetNOscillators,
                                         &JakesPropagationLossModel::GetNOscillators),
                   MakeUintegerChecker<uint8_t> ())
    .AddAttribute ("DopplerFreq",
                   "The doppler frequency in Hz (f_d = v / lambda = v * f / c), the default is 0)",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&JakesPropagationLossModel::m_fd),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Distribution",
                   "The distribution to choose the initial phases.",
                   RandomVariableValue (ConstantVariable (1.0)),
                   MakeRandomVariableAccessor (&JakesPropagationLossModel::m_variable),
                   MakeRandomVariableChecker ())
  ;
  return tid;
}

JakesPropagationLossModel::JakesPropagationLossModel ()
  : m_amp (0),
    m_nRays (0),
    m_nOscillators (0)
{
}

JakesPropagationLossModel::~JakesPropagationLossModel ()
{
  delete [] m_amp;
  for (PathsList::reverse_iterator i = m_paths.rbegin (); i != m_paths.rend (); i++)
    {
      PathsSet *ps = *i;
      for (DestinationList::iterator r = ps->receivers.begin (); r != ps->receivers.end (); r++)
        {
          PathCoefficients *pc = *r;
          delete pc;
        }
      ps->sender = 0;
      ps->receivers.clear ();
      delete ps;
    }
  m_paths.clear ();
}

void
JakesPropagationLossModel::SetNRays (uint8_t nRays)
{
  m_nRays = nRays;
}

void
JakesPropagationLossModel::SetNOscillators (uint8_t nOscillators)
{
  m_nOscillators = nOscillators;
  delete [] m_amp;
  uint16_t N = 4 * m_nOscillators + 2;
  m_amp = new ComplexNumber[m_nOscillators + 1];
  m_amp[0].real = 2.0 * sqrt (2.0 / N) * cos (PI / 4.0);
  m_amp[0].imag = 2.0 * sqrt (2.0 / N) * sin (PI / 4.0);
  for (uint8_t i = 1; i <= m_nOscillators; i++) 
    {
      double beta = PI * (double)i / m_nOscillators;
      m_amp[i].real = 4.0 * cos (beta) / sqrt (N);
      m_amp[i].imag = 4.0 * sin (beta) / sqrt (N);
    }
}

uint8_t 
JakesPropagationLossModel::GetNRays (void) const
{
  return m_nRays;
}
uint8_t 
JakesPropagationLossModel::GetNOscillators (void) const
{
  return m_nOscillators;
}


double 
JakesPropagationLossModel::DoCalcRxPower (double txPowerDbm,
                                          Ptr<MobilityModel> a,
                                          Ptr<MobilityModel> b) const
{
  PathsList::iterator i = m_paths.end ();
  while (i != m_paths.begin ()) 
    {
      i--;
      PathsSet *ps = *i;
      if (ps->sender == a) 
        {
          m_paths.erase (i);
          m_paths.push_back (ps);
          for (DestinationList::iterator r = ps->receivers.begin (); r != ps->receivers.end (); r++) 
            {
              PathCoefficients *pc = *r;
              if (pc->GetReceiver () == b) 
                {
                  ps->receivers.erase (r);
                  ps->receivers.push_back (pc);
                  return txPowerDbm + pc->GetLoss (this);
                }
            }
          PathCoefficients *pc = new PathCoefficients (this, b, m_nRays, m_nOscillators);
          ps->receivers.push_back (pc);
          return txPowerDbm + pc->GetLoss (this);
        }
    }
  PathsSet *ps = new PathsSet;
  ps->sender = a;
  PathCoefficients *pc = new PathCoefficients (this, b, m_nRays, m_nOscillators);
  ps->receivers.push_back (pc);
  m_paths.push_back (ps);
  return txPowerDbm + pc->GetLoss (this);
}

} // namespace ns3

