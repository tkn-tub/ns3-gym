#ifndef TAP_CHANNEL_H
#define TAP_CHANNEL_H

#include "ns3/channel.h"

namespace ns3 {

class HostTapNetDevice;
class TapNetDevice;
class NetDevice;


class TapChannel : public Channel
{
public:
  TapChannel ();

  void SetHostDevice (Ptr<HostTapNetDevice> device);
  void SetDevice (Ptr<TapNetDevice> device);

  Ptr<HostTapNetDevice> GetHostDevice (void) const;

  // overriden from Channel base class
  virtual uint32_t GetNDevices (void) const;
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const;

private:
  virtual void DoDispose (void);

  Ptr<HostTapNetDevice> m_hostDevice;
  Ptr<TapNetDevice> m_device;
};

} // namespace ns3

#endif /* TAP_CHANNEL_H */
