#ifndef ERROR_RATE_MODEL_H
#define ERROR_RATE_MODEL_H

#include <stdint.h>
#include "wifi-mode.h"
#include "ns3/object.h"

namespace ns3 {

class ErrorRateModel : public Object
{
public:
  static TypeId GetTypeId (void);

  ErrorRateModel ();

  double GetChunkSuccessRate (WifiMode mode, double snr, uint32_t nbits) const;

private:
  double Log2 (double val) const;
  double GetBpskBer (double snr, uint32_t signalSpread, uint32_t phyRate) const;
  double GetQamBer (double snr, unsigned int m, uint32_t signalSpread, uint32_t phyRate) const;
  uint32_t Factorial (uint32_t k) const;
  double Binomial (uint32_t k, double p, uint32_t n) const;
  double CalculatePdOdd (double ber, unsigned int d) const;
  double CalculatePdEven (double ber, unsigned int d) const;
  double CalculatePd (double ber, unsigned int d) const;
  double GetFecBpskBer (double snr, double nbits, 
                        uint32_t signalSpread, uint32_t phyRate,
                        uint32_t dFree, uint32_t adFree) const;
  double GetFecQamBer (double snr, uint32_t nbits, 
                       uint32_t signalSpread,
                       uint32_t phyRate,
                       uint32_t m, uint32_t dfree,
                       uint32_t adFree, uint32_t adFreePlusOne) const;
};


} // namespace ns3

#endif /* ERROR_RATE_MODEL_H */
