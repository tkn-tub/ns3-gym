#ifndef AIRTIME_METRIC_H
#define AIRTIME_METRIC_H
#include "ns3/mesh-wifi-interface-mac.h"
namespace ns3 {
namespace dot11s {
class AirtimeLinkMetricCalculator : public RefCountBase
{
  public:
    friend class MeshWifiInterfaceMac;
    uint32_t CalculateMetric(Mac48Address peerAddress, Ptr<MeshWifiInterfaceMac> mac);
  private:
    ///\Microseconds of overhead:
    static const uint32_t overhead = 
      34 //DIFS
      +9*2 //SIFS
      +16*2//Preamble
      +24;  //Ack duration
};
} //namespace dot11s
} //namespace ns3
#endif
