/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "yans-error-rate-model.h"
#include "wifi-phy.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("YansErrorRateModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (YansErrorRateModel);

#ifndef ENABLE_GSL
const double YansErrorRateModel::WLAN_SIR_PERFECT = 10.0; 
const double YansErrorRateModel::WLAN_SIR_IMPOSSIBLE = 0.1; 
#endif

TypeId 
YansErrorRateModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::YansErrorRateModel")
    .SetParent<ErrorRateModel> ()
    .AddConstructor<YansErrorRateModel> ()
    ;
  return tid;
}

YansErrorRateModel::YansErrorRateModel ()
{}

double 
YansErrorRateModel::Log2 (double val) const
{
  return log(val) / log(2.0);
}
double 
YansErrorRateModel::GetBpskBer (double snr, uint32_t signalSpread, uint32_t phyRate) const
{
  double EbNo = snr * signalSpread / phyRate;
  double z = sqrt(EbNo);
  double ber = 0.5 * erfc(z);
  NS_LOG_INFO ("bpsk snr="<<snr<<" ber="<<ber);
  return ber;
}
double 
YansErrorRateModel::GetQamBer (double snr, unsigned int m, uint32_t signalSpread, uint32_t phyRate) const
{
  double EbNo = snr * signalSpread / phyRate;
  double z = sqrt ((1.5 * Log2 (m) * EbNo) / (m - 1.0));
  double z1 = ((1.0 - 1.0 / sqrt (m)) * erfc (z)) ;
  double z2 = 1 - pow ((1-z1), 2.0);
  double ber = z2 / Log2 (m);
  NS_LOG_INFO ("Qam m="<<m<<" rate=" << phyRate << " snr="<<snr<<" ber="<<ber);
  return ber;
}
uint32_t
YansErrorRateModel::Factorial (uint32_t k) const
{
  uint32_t fact = 1;
  while (k > 0) 
    {
      fact *= k;
      k--;
    }
  return fact;
}
double 
YansErrorRateModel::Binomial (uint32_t k, double p, uint32_t n) const
{
  double retval = Factorial (n) / (Factorial (k) * Factorial (n-k)) * pow (p, k) * pow (1-p, n-k);
  return retval;
}
double 
YansErrorRateModel::CalculatePdOdd (double ber, unsigned int d) const
{
  NS_ASSERT ((d % 2) == 1);
  unsigned int dstart = (d + 1) / 2;
  unsigned int dend = d;
  double pd = 0;

  for (unsigned int i = dstart; i < dend; i++) 
    {
      pd += Binomial (i, ber, d);
    }
  return pd;
}
double 
YansErrorRateModel::CalculatePdEven (double ber, unsigned int d) const
{
  NS_ASSERT ((d % 2) == 0);
  unsigned int dstart = d / 2 + 1;
  unsigned int dend = d;
  double pd = 0;

  for (unsigned int i = dstart; i < dend; i++)
    {
      pd +=  Binomial (i, ber, d);
    }
  pd += 0.5 * Binomial (d / 2, ber, d);

  return pd;
}

double 
YansErrorRateModel::CalculatePd (double ber, unsigned int d) const
{
  double pd;
  if ((d % 2) == 0) 
    {
      pd = CalculatePdEven (ber, d);
    } 
  else 
    {
      pd = CalculatePdOdd (ber, d);
    }
  return pd;
}

double
YansErrorRateModel::GetFecBpskBer (double snr, double nbits, 
                         uint32_t signalSpread, uint32_t phyRate,
                         uint32_t dFree, uint32_t adFree) const
{
  double ber = GetBpskBer (snr, signalSpread, phyRate);
  if (ber == 0.0) 
    {
      return 1.0;
    }
  double pd = CalculatePd (ber, dFree);
  double pmu = adFree * pd;
  pmu = std::min (pmu, 1.0);
  double pms = pow (1 - pmu, nbits);
  return pms;
}

double
YansErrorRateModel::GetFecQamBer (double snr, uint32_t nbits, 
                       uint32_t signalSpread,
                       uint32_t phyRate,
                       uint32_t m, uint32_t dFree,
                       uint32_t adFree, uint32_t adFreePlusOne) const
{
  double ber = GetQamBer (snr, m, signalSpread, phyRate);
  if (ber == 0.0) 
    {
      return 1.0;
    }
  /* first term */
  double pd = CalculatePd (ber, dFree);
  double pmu = adFree * pd;
  /* second term */
  pd = CalculatePd (ber, dFree + 1);
  pmu += adFreePlusOne * pd;
  pmu = std::min (pmu, 1.0);
  double pms = pow (1 - pmu, nbits);
  return pms;
}

double 
YansErrorRateModel::GetChunkSuccessRate (WifiMode mode, double snr, uint32_t nbits) const
{
  if (mode == WifiPhy::Get6mba ())
    {
      return GetFecBpskBer (snr, 
                            nbits,
                            mode.GetBandwidth (), // signal spread
                            mode.GetPhyRate (), // phy rate
                            10, // dFree
                            11 // adFree
                            );      
    }
  else if (mode == WifiPhy::Get9mba ())
    {
      return GetFecBpskBer (snr, 
                            nbits,
                            mode.GetBandwidth (), // signal spread
                            mode.GetPhyRate (), // phy rate
                            5, // dFree
                            8 // adFree
                            );
    }
  else if (mode == WifiPhy::Get12mba ())
    {
      return GetFecQamBer (snr, 
                           nbits,
                           mode.GetBandwidth (), // signal spread
                           mode.GetPhyRate (), // phy rate
                           4,  // m 
                           10, // dFree
                           11, // adFree
                           0   // adFreePlusOne
                           );
    }
  else if (mode == WifiPhy::Get18mba ())
    {
      return GetFecQamBer (snr, 
                           nbits,
                           mode.GetBandwidth (), // signal spread
                           mode.GetPhyRate (), // phy rate
                           4, // m
                           5, // dFree
                           8, // adFree
                           31 // adFreePlusOne
                           );
    }
  else if (mode == WifiPhy::Get24mba ())
    {
      return GetFecQamBer (snr, 
                           nbits,
                           mode.GetBandwidth (), // signal spread
                           mode.GetPhyRate (), // phy rate
                           16, // m
                           10, // dFree
                           11, // adFree
                           0   // adFreePlusOne
                           );
    }
  else if (mode == WifiPhy::Get36mba ())
    {
      return GetFecQamBer (snr, 
                           nbits,
                           mode.GetBandwidth (), // signal spread
                           mode.GetPhyRate (), // phy rate
                           16, // m
                           5,  // dFree
                           8,  // adFree
                           31  // adFreePlusOne
                           );
    }
  else if (mode == WifiPhy::Get48mba ())
    {
      return GetFecQamBer (snr, 
                           nbits,
                           mode.GetBandwidth (), // signal spread
                           mode.GetPhyRate (), // phy rate
                           64, // m
                           6,  // dFree
                           1,  // adFree
                           16  // adFreePlusOne
                           );
    }
  else if (mode == WifiPhy::Get54mba ())
    {
      return GetFecQamBer (snr, 
                           nbits,
                           mode.GetBandwidth (), // signal spread
                           mode.GetPhyRate (), // phy rate
                           64, // m
                           5,  // dFree
                           8,  // adFree
                           31  // adFreePlusOne
                           );
    }
  else if (mode == WifiPhy::Get1mbb ())
    {
      return Get80211bDsssDbpskSuccessRate (snr,nbits);
    }
  else if (mode == WifiPhy::Get2mbb ())
    {
      return Get80211bDsssDqpskSuccessRate (snr,nbits);
    }
  else if (mode == WifiPhy::Get5_5mbb ())
    {
      return Get80211bDsssDqpskCck5_5SuccessRate (snr,nbits);
    }
  else if (mode == WifiPhy::Get11mbb ())
    {
      return Get80211bDsssDqpskCck11SuccessRate (snr,nbits);
    }
  return 0;
}

// 802.11b ber based on "Wireless Network Coexistence: Wireless
// LAN in the 21st Century" by Robert Morrow page 187
double 
YansErrorRateModel::DqpskFunction (double x) const
{
  return ((sqrt (2.0) + 1.0) / sqrt (8.0*3.1415926*sqrt (2.0)))
         *(1.0/sqrt (x))
         *exp ( - (2.0 - sqrt (2.0)) * x) ;
}

double 
YansErrorRateModel::Get80211bDsssDbpskSuccessRate (double sinr, uint32_t nbits) const
{
  double EbN0 = sinr * 22000000.0 / 1000000.0; // 1 bit per symbol with 1 MSPS
  double ber = 0.5 * exp (-EbN0);
  return pow ((1.0 - ber), nbits);
}

double 
YansErrorRateModel::Get80211bDsssDqpskSuccessRate (double sinr,uint32_t nbits) const
{
  double EbN0 = sinr * 22000000.0 / 1000000.0 / 2.0; // 2 bits per symbol, 1 MSPS
  double ber = DqpskFunction (EbN0);
  return pow ((1.0 - ber), nbits);
}

double 
YansErrorRateModel::Get80211bDsssDqpskCck5_5SuccessRate (double sinr,uint32_t nbits) const
{
#ifdef ENABLE_GSL
  // symbol error probability
  double EbN0 = sinr * 22000000.0 / 1375000.0 / 4.0;
  double sep = SymbolErrorProb16Cck (4.0*EbN0/2.0);
  return pow (1.0-sep,nbits/4.0);
#else
  NS_LOG_WARN ("Running a 802.11b CCK Matlab model less accurate than GSL model"); 
  // The matlab model
  double ber; 
  if (sinr > WLAN_SIR_PERFECT)
    {
       ber = 0.0 ;
    }
  else if (sinr < WLAN_SIR_IMPOSSIBLE)
    {
       ber = 0.5;
    }
  else
    { // fitprops.coeff from matlab berfit
       double a1 =  5.3681634344056195e-001;
       double a2 =  3.3092430025608586e-003;
       double a3 =  4.1654372361004000e-001;
       double a4 =  1.0288981434358866e+000;
       ber = a1 * exp (-(pow ((sinr-a2)/a3,a4)));
     }
  return pow ((1.0 - ber), nbits);
#endif
}

double 
YansErrorRateModel::Get80211bDsssDqpskCck11SuccessRate (double sinr,uint32_t nbits) const
{
#ifdef ENABLE_GSL
 // symbol error probability
  double EbN0 = sinr * 22000000.0 / 1375000.0 / 8.0;
  double sep = SymbolErrorProb256Cck (8.0*EbN0/2.0);
  return pow (1.0-sep,nbits/8.0);
#else
  NS_LOG_WARN ("Running a 802.11b CCK Matlab model less accurate than GSL model"); 
  // The matlab model
  double ber; 
  if (sinr > WLAN_SIR_PERFECT)
    {
       ber = 0.0 ;
    }
  else if (sinr < WLAN_SIR_IMPOSSIBLE)
    {
       ber = 0.5;
    }
  else
    { // fitprops.coeff from matlab berfit
       double a1 =  7.9056742265333456e-003;
       double a2 = -1.8397449399176360e-001;
       double a3 =  1.0740689468707241e+000;
       double a4 =  1.0523316904502553e+000;
       double a5 =  3.0552298746496687e-001;
       double a6 =  2.2032715128698435e+000;
       ber =  (a1*sinr*sinr+a2*sinr+a3)/(sinr*sinr*sinr+a4*sinr*sinr+a5*sinr+a6);
     }
  return pow ((1.0 - ber), nbits);
#endif
}

#ifdef ENABLE_GSL
double 
IntegralFunction (double x, void *params)
{
  double beta = ((FunctionParameters *) params)->beta;
  double n = ((FunctionParameters *) params)->n;
  double IntegralFunction = pow (2*gsl_cdf_ugaussian_P (x+ beta) - 1, n-1) 
                            * exp (-x*x/2.0) / sqrt (2.0 * M_PI);
  return IntegralFunction;
}

double 
YansErrorRateModel::SymbolErrorProb16Cck (double e2) const
{
  double sep;
  double error;
 
  FunctionParameters params;
  params.beta = sqrt (2.0*e2);
  params.n = 8.0;

  gsl_integration_workspace * w = gsl_integration_workspace_alloc (1000); 
 
  gsl_function F;
  F.function = &IntegralFunction;
  F.params = &params;

  gsl_integration_qagiu (&F,-params.beta, 0, 1e-7, 1000, w, &sep, &error);
  gsl_integration_workspace_free (w);
  if (error == 0.0) 
    {
       sep = 1.0;
    }

  return 1.0 - sep;
}

double YansErrorRateModel::SymbolErrorProb256Cck (double e1) const
{
  return 1.0 - pow (1.0 - SymbolErrorProb16Cck (e1/2.0), 2.0);
}

#endif

} // namespace ns3
