#ifndef WIFI_DEFAULT_PARAMETERS_H
#define WIFI_DEFAULT_PARAMETERS_H

#include <stdint.h>
#include "ns3/nstime.h"
#include "ssid.h"

namespace ns3 {

namespace WifiDefaultParameters {

enum PhyStandard {
  PHY_STANDARD_80211a,
};
enum RateControlAlgorithm {
  CONSTANT_RATE,
  ARF,
  AARF,
  IDEAL
};
enum PhyModeParameter {
  MODE_6MB = 6000000,
  MODE_9MB = 9000000,
  MODE_12MB = 12000000,
  MODE_18MB = 18000000,
  MODE_24MB = 24000000,
  MODE_36MB = 36000000,
  MODE_48MB = 48000000,
  MODE_54MB = 54000000
};

uint32_t GetMaxSsrc (void);
uint32_t GetMaxSlrc (void);
uint32_t GetRtsCtsThreshold (void);
uint32_t GetFragmentationThreshold (void);
Time GetApBeaconInterval (void);
enum PhyStandard GetPhyStandard (void);
enum RateControlAlgorithm GetRateControlAlgorithm (void);
enum PhyModeParameter GetConstantDataRate (void);
enum PhyModeParameter GetConstantCtlRate (void);
double GetIdealRateControlBer (void);
uint32_t GetArfRateControlSuccessThreshold (void);
uint32_t GetArfRateControlTimerThreshold (void);
uint32_t GetAarfRateControlMinSuccessThreshold (void);
uint32_t GetAarfRateControlMinTimerThreshold (void);
double GetAarfRateControlSuccessK (void);
uint32_t GetAarfRateControlMaxSuccessThreshold (void);
double GetAarfRateControlTimerK (void);

double GetPhyEnergyDetectionThresholdDbm (void);
double GetPhyRxNoiseDb (void);
double GetPhyTxPowerBaseDbm (void);
double GetPhyTxPowerEndDbm (void);
uint32_t GetPhyTxPowerLevels (void);
double GetPhyTxGainDbm (void);
double GetPhyRxGainDbm (void);

Ssid GetSsid (void);

} // namespace WifiDefaultParameters

} // namespace ns3

#endif /* WIFI_DEFAULT_PARAMETERS_H */
