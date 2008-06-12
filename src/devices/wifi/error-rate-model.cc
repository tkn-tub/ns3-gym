#include "error-rate-model.h"
#include "wifi-phy.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("ErrorRateModel");

namespace ns3 {

TypeId 
ErrorRateModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ErrorRateModel")
    .SetParent<Object> ()
    .AddConstructor<ErrorRateModel> ()
    ;
  return tid;
}

ErrorRateModel::ErrorRateModel ()
{}


double 
ErrorRateModel::Log2 (double val) const
{
  return log(val) / log(2.0);
}
double 
ErrorRateModel::GetBpskBer (double snr, uint32_t signalSpread, uint32_t phyRate) const
{
  double EbNo = snr * signalSpread / phyRate;
  double z = sqrt(EbNo);
  double ber = 0.5 * erfc(z);
  NS_LOG_INFO ("bpsk snr="<<snr<<" ber="<<ber);
  return ber;
}
double 
ErrorRateModel::GetQamBer (double snr, unsigned int m, uint32_t signalSpread, uint32_t phyRate) const
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
ErrorRateModel::Factorial (uint32_t k) const
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
ErrorRateModel::Binomial (uint32_t k, double p, uint32_t n) const
{
  double retval = Factorial (n) / (Factorial (k) * Factorial (n-k)) * pow (p, k) * pow (1-p, n-k);
  return retval;
}
double 
ErrorRateModel::CalculatePdOdd (double ber, unsigned int d) const
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
ErrorRateModel::CalculatePdEven (double ber, unsigned int d) const
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
ErrorRateModel::CalculatePd (double ber, unsigned int d) const
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
ErrorRateModel::GetFecBpskBer (double snr, double nbits, 
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
ErrorRateModel::GetFecQamBer (double snr, uint32_t nbits, 
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
ErrorRateModel::GetChunkSuccessRate (WifiMode mode, double snr, uint32_t nbits) const
{
  if (mode == WifiPhy::g_6mba)
    {
      return GetFecBpskBer (snr, 
                            nbits,
                            mode.GetBandwidth (), // signal spread
                            mode.GetPhyRate (), // phy rate
                            10, // dFree
                            11 // adFree
                            );      
    }
  else if (mode == WifiPhy::g_9mba)
    {
      return GetFecBpskBer (snr, 
                            nbits,
                            mode.GetBandwidth (), // signal spread
                            mode.GetPhyRate (), // phy rate
                            5, // dFree
                            8 // adFree
                            );
    }
  else if (mode == WifiPhy::g_12mba)
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
  else if (mode == WifiPhy::g_18mba)
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
  else if (mode == WifiPhy::g_24mba)
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
  else if (mode == WifiPhy::g_36mba)
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
  else if (mode == WifiPhy::g_48mba)
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
  else if (mode == WifiPhy::g_54mba)
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
  return 0;
}

} // namespace ns3
