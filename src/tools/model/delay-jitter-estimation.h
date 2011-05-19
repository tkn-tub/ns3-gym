#ifndef DELAY_JITTER_ESTIMATION_H
#define DELAY_JITTER_ESTIMATION_H

#include "ns3/nstime.h"
#include "ns3/packet.h"

namespace ns3 {

/**
 * \ingroup tools
 *
 * \brief quick and dirty delay and jitter estimation
 *
 */
class DelayJitterEstimation
{
public:
  DelayJitterEstimation ();

  /**
   * \param packet the packet to send over a wire
   *
   * This method should be invoked once on each packet to
   * record within the packet the tx time which is used upon
   * packet reception to calculate the delay and jitter. The
   * tx time is stored in the packet as an ns3::Tag which means
   * that it does not use any network resources and is not
   * taken into account in transmission delay calculations.
   */
  static void PrepareTx (Ptr<const Packet> packet);
  /**
   * \param packet the packet received
   *
   * Invoke this method to update the delay and jitter calculations
   * After a call to this method, \ref GetLastDelay and \ref GetLastJitter
   * will return an updated delay and jitter.
   */
  void RecordRx (Ptr<const Packet> packet);

  /**
   * \returns the updated delay.
   */
  Time GetLastDelay (void) const;
  /**
   * The jitter is calculated using the RFC 1889 (RTP) jitter
   * definition.
   *
   * \returns the updated jitter.
   */
  uint64_t GetLastJitter (void) const;

private:
  Time m_previousRx;
  Time m_previousRxTx;
  int64x64_t m_jitter;
  Time m_delay;
};

} // namespace ns3

#endif /* DELAY_JITTER_ESTIMATION_H */
