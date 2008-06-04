#ifndef TEST_CHANNEL_H
#define TEST_CHANNEL_H

#include "channel.h"
#include "mac48-address.h"
#include <vector>

namespace ns3 {

class SimpleNetDevice;
class Packet;

/**
 * \ingroup channel
 * \brief A simple channel, for simple things and testing
 */
class SimpleChannel : public Channel
{
public:
  static TypeId GetTypeId (void);
  SimpleChannel ();

  void Send (Ptr<Packet> p, uint16_t protocol, Mac48Address to, Mac48Address from,
	     Ptr<SimpleNetDevice> sender);

  void Add (Ptr<SimpleNetDevice> device);

  virtual uint32_t GetNDevices (void) const;
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const;

private:

  std::vector<Ptr<SimpleNetDevice> > m_devices;
};

} // namespace ns3

#endif /* TEST_CHANNEL_H */
