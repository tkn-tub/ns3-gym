#ifndef DELAY_JITTER_ESTIMATION_H
#define DELAY_JITTER_ESTIMATION_H

#include "ns3/nstime.h"
#include "ns3/packet.h"

namespace ns3 {

class DelayJitterEstimation
{
public:
  DelayJitterEstimation ();
  static void PrepareTx (Ptr<const Packet> packet);
  void RecordRx (Ptr<const Packet> packet);

  Time GetLastDelay (void) const;
  Time GetLastJitter (void) const;

private:
  Time m_previousRx;
  Time m_previousRxTx;
  Time m_jitter;
  Time m_delay;
};

} // namespace ns3

#endif /* DELAY_JITTER_ESTIMATION_H */
