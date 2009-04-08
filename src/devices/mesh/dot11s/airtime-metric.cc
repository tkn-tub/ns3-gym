#include "airtime-metric.h"
#include "ns3/wifi-remote-station-manager.h"
#include "ns3/wifi-mode.h"
namespace ns3 {
namespace dot11s {
uint32_t
AirtimeLinkMetricCalculator::CalculateMetric(Mac48Address peerAddress, Ptr<MeshWifiInterfaceMac> mac)
{
  WifiRemoteStation * station = mac->GetStationManager ()->Lookup(peerAddress);
  Ptr<Packet> test_frame = Create<Packet> (1024);
  uint32_t rate = station->GetDataMode(test_frame, 1024+36).GetDataRate ();
  uint32_t payload_nanosec = (uint32_t)((double)(1024*8) / ((double)rate)*1e9);
  uint32_t metric = (uint32_t)(((double)(payload_nanosec + overhead*1000))/102.4);
  return metric;
}
} //namespace dot11s
} //namespace ns3
