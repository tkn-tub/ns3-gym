/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

class JakesPropagationLossModel::PathCoefficients 
{
public:
  PathCoefficients (Ptr<const JakesPropagationLossModel> jakes,
                    Ptr<MobilityModel> receiver, 
                    uint8_t nRays, 
                    uint8_t nOscillators);
  ~PathCoefficients ();
  double GetLoss (void);
  Ptr<MobilityModel> GetReceiver (void);
private:
  void DoConstruct (void);
  Ptr<MobilityModel> m_receiver;
  uint8_t m_nOscillators;
  uint8_t m_nRays;
  double **m_phases;
  Time m_lastUpdate;
  Ptr<const JakesPropagationLossModel> m_jakes;
};


JakesPropagationLossModel::PathCoefficients::PathCoefficients (Ptr<const JakesPropagationLossModel> jakes, 
                                                           Ptr<MobilityModel> receiver, 
                                                           uint8_t nRays, 
							   uint8_t nOscillators)
  : m_receiver (receiver),
    m_nOscillators (nOscillators),
    m_nRays (nRays),
    m_jakes(jakes)
{
  DoConstruct ();
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
JakesPropagationLossModel::PathCoefficients::DoConstruct ()
{
  m_phases = new double*[m_nRays];
  for (uint8_t i = 0; i < m_nRays; i++) 
    {
      m_phases[i] = new double[m_nOscillators + 1];
      for (uint8_t j = 0; j <= m_nOscillators; j++) 
        {
          m_phases[i][j] = 2.0 * JakesPropagationLossModel::PI * m_jakes->m_variable.GetValue ();
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
JakesPropagationLossModel::PathCoefficients::GetLoss (void)
{
  uint16_t N = 4 * m_nOscillators + 2;
  Time interval = Simulator::Now () - m_lastUpdate;
  ComplexNumber coef= {0.0, 0.0};
  ComplexNumber fading;
  double norm = 0.0;
  for (uint8_t i = 0; i < m_nRays; i++) 
    {
      fading.real = 0.0;
      fading.imag = 0.0;
      for (uint8_t j = 0; j <= m_nOscillators; j++) 
        {
          m_phases[i][j] += 2.0 * JakesPropagationLossModel::PI * 
            cos (2.0 * JakesPropagationLossModel::PI * j / N) * m_jakes->m_fd * interval.GetSeconds ();
          m_phases[i][j] -= 2.0 * JakesPropagationLossModel::PI * 
            floor (m_phases[i][j] / 2.0 / JakesPropagationLossModel::PI);
          fading.real += m_jakes->m_amp[j].real * cos (m_phases[i][j]);
          fading.imag += m_jakes->m_amp[j].imag * cos (m_phases[i][j]);
          norm += sqrt(pow (m_jakes->m_amp[j].real, 2) + pow(m_jakes->m_amp[j].imag, 2));
        }
    coef.real += fading.real;
    coef.imag += fading.imag;
    }
  m_lastUpdate = Simulator::Now ();
  double k = sqrt (pow (coef.real, 2) + pow (coef.imag, 2)) / norm;
  NS_LOG_DEBUG ("Jakes coef "<< k << " (" << 10 * log10 (k) << "dB)");
  return 10 * log10 (k);
}

NS_OBJECT_ENSURE_REGISTERED (JakesPropagationLossModel);

const double JakesPropagationLossModel::PI = 3.1415;

TypeId
JakesPropagationLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::JakesPropagationLossModel")
    .SetParent<PropagationLossModel> ()
    .AddConstructor<JakesPropagationLossModel> ()
    .AddAttribute ("NumberOfRaysPerPath",
                   "The number of rays to use by default for compute the fading coeficent for a given path (default is 1)",
                   UintegerValue (1),
		   MakeUintegerAccessor (&JakesPropagationLossModel::m_nRays),
		   MakeUintegerChecker<uint8_t> ())
    .AddAttribute ("NumberOfOscillatorsPerRay",
                   "The number of oscillators to use by default for compute the coeficent for a given ray of a given "
                   "path (default is 4)",
                   UintegerValue (4),
		   MakeUintegerAccessor (&JakesPropagationLossModel::m_nOscillators),
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
{
  DoConstruct ();
}

JakesPropagationLossModel::~JakesPropagationLossModel ()
{
  delete [] m_amp;
  for (PathsList::iterator i = m_paths.end (); i != m_paths.begin (); i--) 
    {
      PathsSet *ps = *i;
      for (DestinationList::iterator r = ps->receivers.begin (); r != ps->receivers.end (); r++) 
        {
          PathCoefficients *pc = *r;
          delete pc;
        }
      delete ps;
    }
}

void
JakesPropagationLossModel::DoConstruct ()
{
  uint16_t N = 4 * m_nOscillators + 2;
  m_amp = new ComplexNumber[m_nOscillators + 1];
  m_amp[0].real = 2.0 * sqrt(2.0 / N) * cos (PI / 4.0);
  m_amp[0].imag = 2.0 * sqrt(2.0 / N) * sin (PI / 4.0);
  for (uint8_t i = 1; i <= m_nOscillators; i++) 
    {
      double beta = PI * (double)i / m_nOscillators;
      m_amp[i].real = 4.0 * cos (beta) / sqrt(N);
      m_amp[i].imag = 4.0 * sin (beta) / sqrt(N);
    }
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
                  return txPowerDbm + pc->GetLoss ();
                }
            }
          PathCoefficients *pc = new PathCoefficients (this, b, m_nRays, m_nOscillators);
          ps->receivers.push_back (pc);
          return txPowerDbm + pc->GetLoss ();
        }
    }
  PathsSet *ps = new PathsSet;
  ps->sender = a;
  PathCoefficients *pc = new PathCoefficients (this, b, m_nRays, m_nOscillators);
  ps->receivers.push_back (pc);
  m_paths.push_back (ps);
  return txPowerDbm + pc->GetLoss ();
}

} // namespace ns3

