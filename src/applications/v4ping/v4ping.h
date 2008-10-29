#ifndef V4PING_H
#define V4PING_H

#include "ns3/application.h"
#include "ns3/traced-callback.h"
#include "ns3/nstime.h"

namespace ns3 {

class Socket;

class V4Ping : public Application
{
public:
  static TypeId GetTypeId (void);

  V4Ping ();
  virtual ~V4Ping ();

private:
  // inherited from Application base class.
  virtual void StartApplication (void);
  virtual void StopApplication (void);
  virtual void DoDispose (void);
  uint32_t GetApplicationId (void) const;
  void Receive (Ptr<Socket> socket);

  Ipv4Address m_remote;
  Ptr<Socket> m_socket;
  uint16_t m_seq;
  TracedCallback<Time> m_traceRtt;
};

} // namespace ns3

#endif /* V4PING_H */
