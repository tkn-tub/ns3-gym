#ifndef TCP_RL_H
#define TCP_RL_H

#include "ns3/tcp-congestion-ops.h"
#include "ns3/tcp-recovery-ops.h"
#include "ns3/sequence-number.h"
#include "ns3/traced-value.h"
#include "ns3/event-id.h"
#include "ns3/opengym-module.h"

namespace ns3 {

class Packet;
class TcpHeader;
class Time;
class EventId;
class TcpRl;

class TcpGymEnv : public OpenGymEnv
{
public:
  TcpGymEnv ();
  virtual ~TcpGymEnv ();
  static TypeId GetTypeId (void);
  virtual void DoDispose ();

  void SetTcp(Ptr<TcpRl> tcp);
  void SetExtraInfo(std::string info);
  void SetTcpSocketState(std::string info);

  // OpenGym interface
  Ptr<OpenGymSpace> GetActionSpace();
  Ptr<OpenGymSpace> GetObservationSpace();
  bool GetGameOver();
  Ptr<OpenGymDataContainer> GetObservation();
  float GetReward();
  std::string GetExtraInfo();
  bool ExecuteActions(Ptr<OpenGymDataContainer> action);


  // TCP congestion control interface
  uint32_t GetSsThresh (Ptr<const TcpSocketState> tcb, uint32_t bytesInFlight);
  void IncreaseWindow (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked);
  void PktsAcked (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked, const Time& rtt);
  void CongestionStateSet (Ptr<TcpSocketState> tcb, const TcpSocketState::TcpCongState_t newState);
  void CwndEvent (Ptr<TcpSocketState> tcb, const TcpSocketState::TcpCAEvent_t event);

private:
  Ptr<TcpRl> m_tcp;
  std::string m_info;

  // state
  Ptr<const TcpSocketState> m_tcb;
  uint32_t m_bytesInFlight;
  uint32_t m_segmentsAcked;
  Time m_rtt;
  TcpSocketState::TcpCongState_t m_newState;
  TcpSocketState::TcpCAEvent_t m_event;

  // actions
  uint32_t m_new_ssThresh;
  uint32_t m_new_cWnd;
};


class TcpRl : public TcpCongestionOps
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  TcpRl ();

  /**
   * \brief Copy constructor.
   * \param sock object to copy.
   */
  TcpRl (const TcpRl& sock);

  ~TcpRl ();

  std::string GetName () const;

  virtual uint32_t GetSsThresh (Ptr<const TcpSocketState> tcb,
                                uint32_t bytesInFlight);

  virtual void IncreaseWindow (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked);

  virtual void PktsAcked (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked,
                          const Time& rtt);

  virtual void CongestionStateSet (Ptr<TcpSocketState> tcb,
                                   const TcpSocketState::TcpCongState_t newState);

  virtual void CwndEvent (Ptr<TcpSocketState> tcb,
                          const TcpSocketState::TcpCAEvent_t event);

  virtual Ptr<TcpCongestionOps> Fork ();

private:
  // OpenGymEnv interface
  Ptr<TcpGymEnv> m_tcpGymEnv;
};

} // namespace ns3

#endif /* TCP_RL_H */