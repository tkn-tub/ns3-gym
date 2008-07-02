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
#ifndef PROPAGATION_JAKES_MODEL_H
#define PROPAGATION_JAKES_MODEL_H

#include "ns3/nstime.h"
#include "propagation-loss-model.h"

namespace ns3 {


/**
 * \brief a Jakes propagation loss model
 *
 * The Jakes propagation loss model implemented here is 
 * described in [1].
 * 
 *
 * We call path the set of rays that depart from a given 
 * transmitter and arrive to a given receiver. For each ray
 * The complex coefficient is compute as follow:
 * \f[ u(t)=u_c(t) + j u_s(t)\f]
 * \f[ u_c(t) = \frac{2}{\sqrt{N}}\sum_{n=0}^{M}a_n\cos(\omega_n t+\phi_n)\f]
 * \f[ u_s(t) = \frac{2}{\sqrt{N}}\sum_{n=0}^{M}b_n\cos(\omega_n t+\phi_n)\f]
 * where
 * \f[ a_n=\left \{ \begin{array}{ll}
 * \sqrt{2}\cos\beta_0 & n=0 \\
 * 2\cos\beta_n & n=1,2,\ldots,M
 * \end{array}
 * \right .\f]
 * \f[ b_n=\left \{ \begin{array}{ll}
 * \sqrt{2}\sin\beta_0 & n=0 \\
 * 2\sin\beta_n & n=1,2,\ldots,M
 * \end{array}
 * \right .\f]
 * \f[ \beta_n=\left \{ \begin{array}{ll}
 * \frac{\pi}{4} & n=0 \\
 * \frac{\pi n}{M} & n=1,2,\ldots,M
 * \end{array}
 * \right .\f]
 * \f[ \omega_n=\left \{ \begin{array}{ll}
 * 2\pi f_d & n=0 \\
 * 2\pi f_d \cos\frac{2\pi n}{N} & n=1,2,\ldots,M
 * \end{array}
 * \right .\f]
 *
 * The parameter \f$f_d\f$ is the doppler frequency and \f$N=4M+2\f$ where
 * \f$M\f$ is the number of oscillators per ray.
 *
 * The attenuation coefficent of the path is the magnitude of the sum of 
 * all the ray coefficients. This attenuation coefficient could be greater than
 * \f$1\f$, hence it is divide by \f$ \frac{2N_r}{\sqrt{N}} \sum_{n+0}^{M}\sqrt{a_n^2 +b_n^2}\f$
 * where \f$N_r\f$ is the number of rays.
 *
 * The initail phases \f$\phi_i\f$ are random and they are choosen according 
 * to a given distribution.
 * 
 * [1] Y. R. Zheng and C. Xiao, "Simulation Models With Correct 
 * Statistical Properties for Rayleigh Fading Channel", IEEE
 * Trans. on Communications, Vol. 51, pp 920-928, June 2003
 */
class JakesPropagationLossModel : public PropagationLossModel
{
public:
  static TypeId GetTypeId (void);
  JakesPropagationLossModel ();
  virtual ~JakesPropagationLossModel ();

  virtual double GetLoss (Ptr<MobilityModel> a,
			  Ptr<MobilityModel> b) const;
  /**
   * \param nRays Number of rays per path
   *
   * Set the number of rays for each path
   */
  void SetNRays (uint8_t nRays);
  /**
   * \param nOscillators Number of oscillators
   *
   * Set the number of oscillators to use to compute the ray coefficient
   */
  void SetNOscillators (uint8_t nOscillators);

private:
  class PathCoefficients;
  struct ComplexNumber {
    double real;
    double imag;
  };
  friend class PathCoefficents;
  
  static const double PI;
  ComplexNumber* m_amp;
  RandomVariable m_variable;
  double m_fd;

  typedef std::vector<PathCoefficients *> DestinationList;
  struct PathsSet {
    Ptr<MobilityModel> sender;
    DestinationList receivers;
  };
  typedef std::vector<PathsSet *> PathsList;
  
  void DoConstruct (void);

  mutable PathsList m_paths;
  uint8_t m_nRays;
  uint8_t m_nOscillators;
};

} // namespace ns3

#endif /* PROPAGATION_JAKES_MODEL_H */


