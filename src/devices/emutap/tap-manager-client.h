#ifndef TAP_MANAGER_CLIENT_H
#define TAP_MANAGER_CLIENT_H

#include "ns3/mac48-address.h"
#include "ns3/ipv4-address.h"
#include <string>

namespace ns3 {

class TapManagerClient
{
public:
  int AllocateTap (Mac48Address host, Ipv4Address ad, Ipv4Mask mask, Ipv4Address gateway);
private:
  std::string FindManager (void) const;
  bool Exists (std::string filename) const;
};

} // namespace ns3

#endif /* TAP_MANAGER_CLIENT_H */
