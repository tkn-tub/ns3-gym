
#include <vector>
#include "ns3/ptr.h"
#include "ns3/assert.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4.h"
#include "static-multicast-route-helper.h"

namespace ns3 {

StaticMulticastRouteHelper::StaticMulticastRouteHelper ()
{
}

void  
StaticMulticastRouteHelper::AddMulticastRoute (
  Ptr<Node> n,
  Ipv4Address source, 
  Ipv4Address group,  
  Ptr<NetDevice> input, 
  NetDeviceContainer output)
{
  Ptr<Ipv4> ipv4 = n->GetObject<Ipv4> ();

  // We need to convert the NetDeviceContainer to an array of ifIndex
  std::vector<uint32_t> outputInterfaces;
  for (NetDeviceContainer::Iterator i = output.Begin (); i != output.End (); ++i)
    {
      Ptr<NetDevice> nd = *i;
      uint32_t oifIndex = ipv4->FindInterfaceForDevice (nd);
      outputInterfaces.push_back(oifIndex);
    }
  uint32_t iifIndex = ipv4->FindInterfaceForDevice (input);
  ipv4->AddMulticastRoute (source, group, iifIndex, outputInterfaces);
}

void
StaticMulticastRouteHelper::SetDefaultMulticastRoute (
  Ptr<Node> n, 
  Ptr<NetDevice> nd)
{
  Ptr<Ipv4> ipv4 = n->GetObject<Ipv4> ();
  uint32_t ifIndexSrc = ipv4->FindInterfaceForDevice (nd);
  ipv4->SetDefaultMulticastRoute (ifIndexSrc);
}

void
StaticMulticastRouteHelper::JoinMulticastGroup (
  Ptr<Node> n, 
  Ipv4Address source, 
  Ipv4Address group)
{
  Ptr<Ipv4> ipv4 = n->GetObject<Ipv4> ();
  ipv4->JoinMulticastGroup (source, group);
}

} // namespace ns3

