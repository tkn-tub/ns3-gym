#ifndef V4PING_H
#define V4PING_H

#include "ns3/application.h"
#include "ns3/traced-callback.h"
#include "ns3/nstime.h"
#include "ns3/average.h"
#include "ns3/simulator.h"

namespace ns3 {

class Socket;

class V4Ping : public Application
{
public:
  static TypeId GetTypeId (void);

  V4Ping ();
  virtual ~V4Ping ();

private:
  void Write32 (uint8_t *buffer, uint32_t data);

  // inherited from Application base class.
  virtual void StartApplication (void);
  virtual void StopApplication (void);
  virtual void DoDispose (void);
  uint32_t GetApplicationId (void) const;
  void Receive (Ptr<Socket> socket);
  void Send ();

  /// Remote address
  Ipv4Address m_remote;
  /// Wait  interval  seconds between sending each packet
  Time m_interval;
  /** 
   * Specifies  the number of data bytes to be sent. 
   * The default is 56, which translates into 64 ICMP data bytes when combined with the 8 bytes of ICMP header data.
   */
  uint32_t m_size;
  Ptr<Socket> m_socket;
  uint16_t m_seq;
  TracedCallback<Time> m_traceRtt;
  /// produce ping-style output if true
  bool m_verbose;
  /// received packets counter
  uint32_t m_recv;
  /// Start time to report total ping time
  Time m_started;
  /// Average rtt is ms
  Average<double> m_avgRtt;
  /// Next packet will be sent
  EventId m_next;
};

} // namespace ns3

#endif /* V4PING_H */
