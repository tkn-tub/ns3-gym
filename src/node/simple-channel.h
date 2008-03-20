#ifndef TEST_CHANNEL_H
#define TEST_CHANNEL_H

#include "channel.h"
#include "mac48-address.h"
#include <vector>

namespace ns3 {

class SimpleNetDevice;
class Packet;

class SimpleChannel : public Channel
{
public:
  static TypeId GetTypeId (void);
  SimpleChannel ();

  void Send (Ptr<Packet> p, uint16_t protocol, Mac48Address to, Mac48Address from,
	     Ptr<SimpleNetDevice> sender);

  void Add (Ptr<SimpleNetDevice> device);

  virtual uint32_t GetNDevices (void) const;

private:
  virtual Ptr<NetDevice> DoGetDevice (uint32_t i) const;

  std::vector<Ptr<SimpleNetDevice> > m_devices;
};

} // namespace ns3

#endif /* TEST_CHANNEL_H */
