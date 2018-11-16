#include "tcp-rl.h"
#include "ns3/object.h"
#include "ns3/core-module.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/rtt-estimator.h"
#include "ns3/tcp-socket-base.h"


NS_LOG_COMPONENT_DEFINE ("TcpRl");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TcpGymEnv);

TcpGymEnv::TcpGymEnv ()
{
  NS_LOG_FUNCTION (this);
  m_tcp = 0;
  SetOpenGymInterface(OpenGymInterface::Get());
}

TcpGymEnv::~TcpGymEnv ()
{
  NS_LOG_FUNCTION (this);
}

TypeId
TcpGymEnv::GetTypeId (void)
{
  static TypeId tid = TypeId ("TcpGymEnv")
    .SetParent<OpenGymEnv> ()
    .SetGroupName ("OpenGym")
    .AddConstructor<TcpGymEnv> ()
  ;
  return tid;
}

void
TcpGymEnv::DoDispose ()
{
  NS_LOG_FUNCTION (this);
}

void
TcpGymEnv::SetTcp(Ptr<TcpRl> tcp)
{
  NS_LOG_FUNCTION (this);
  m_tcp = tcp;
}

void
TcpGymEnv::SetExtraInfo(std::string info)
{
  NS_LOG_FUNCTION (this);
  m_info = info;
}

/*
Define observation space
*/
Ptr<OpenGymSpace>
TcpGymEnv::GetObservationSpace()
{
  //ssThresh
  //cWnd
  //segmentSize
  //segmentsAcked
  //bytesInFlight
  //sim time in ms
  uint32_t parameterNum = 6;
  float low = 0.0;
  float high = 1000000000.0;
  std::vector<uint32_t> shape = {parameterNum,};
  std::string dtype = TypeNameGet<uint64_t> ();

  Ptr<OpenGymBoxSpace> box = CreateObject<OpenGymBoxSpace> (low, high, shape, dtype);
  NS_LOG_UNCOND ("MyGetObservationSpace: " << box);
  return box;
}

/*
Define action space
*/
Ptr<OpenGymSpace>
TcpGymEnv::GetActionSpace()
{
  // new_ssThresh
  // new_cWnd
  uint32_t parameterNum = 2;
  float low = 0.0;
  float high = 1000000000.0;
  std::vector<uint32_t> shape = {parameterNum,};
  std::string dtype = TypeNameGet<uint32_t> ();

  Ptr<OpenGymBoxSpace> box = CreateObject<OpenGymBoxSpace> (low, high, shape, dtype);
  NS_LOG_UNCOND ("MyGetActionSpace: " << box);
  return box;
}

/*
Define game over condition
*/
bool
TcpGymEnv::GetGameOver()
{
  bool isGameOver = false;
  bool test = false;
  static float stepCounter = 0.0;
  stepCounter += 1;
  if (stepCounter == 10 && test) {
      isGameOver = true;
  }
  NS_LOG_UNCOND ("MyGetGameOver: " << isGameOver);
  return isGameOver;
}

/*
Collect observations
*/
Ptr<OpenGymDataContainer>
TcpGymEnv::GetObservation()
{
  //ssThresh
  //cWnd
  //segmentSize
  //segmentsAcked
  //bytesInFlight
  //sim time in ms
  uint32_t parameterNum = 6;
  std::vector<uint32_t> shape = {parameterNum,};

  Ptr<OpenGymBoxContainer<uint64_t> > box = CreateObject<OpenGymBoxContainer<uint64_t> >(shape);

  box->AddValue(m_tcb->m_ssThresh);
  box->AddValue(m_tcb->m_cWnd);
  box->AddValue(m_tcb->m_segmentSize);
  box->AddValue(m_segmentsAcked);
  box->AddValue(m_bytesInFlight);
  box->AddValue(Simulator::Now ().GetMilliSeconds ());

  // Print data
  NS_LOG_UNCOND ("MyGetObservation: " << box);
  //NS_LOG_UNCOND ("---ssThresh: " << box->GetValue(0));
  //NS_LOG_UNCOND ("---cWnd: " << box->GetValue(1));

  return box;
}

/*
Define reward function
*/
float
TcpGymEnv::GetReward()
{
  float reward = 0.0;
  return reward;
}

/*
Define extra info. Optional
*/
std::string
TcpGymEnv::GetExtraInfo()
{
  NS_LOG_UNCOND("MyGetExtraInfo: " << m_info);
  return m_info;
}

/*
Execute received actions
*/
bool
TcpGymEnv::ExecuteActions(Ptr<OpenGymDataContainer> action)
{
  Ptr<OpenGymBoxContainer<uint32_t> > box = DynamicCast<OpenGymBoxContainer<uint32_t> >(action);
  m_new_ssThresh = box->GetValue(0);
  m_new_cWnd = box->GetValue(1);

  // Print data
  NS_LOG_UNCOND ("MyExecuteActions: " << action);
  //NS_LOG_UNCOND ("---new_ssThresh: " << new_ssThresh);
  //NS_LOG_UNCOND ("---new_cWnd: " << new_cWnd);
  return true;
}


uint32_t
TcpGymEnv::GetSsThresh (Ptr<const TcpSocketState> tcb, uint32_t bytesInFlight)
{
  NS_LOG_FUNCTION (this);
  m_info = "GetSsThresh";
  m_tcb = tcb;
  m_bytesInFlight = bytesInFlight;
  Notify();
  return m_new_ssThresh;
}

void
TcpGymEnv::IncreaseWindow (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
{
  NS_LOG_FUNCTION (this);
  m_info = "IncreaseWindow";
  m_tcb = tcb;
  m_segmentsAcked = segmentsAcked;
  Notify();
  tcb->m_cWnd = m_new_cWnd;
}

void
TcpGymEnv::PktsAcked (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked, const Time& rtt)
{
  NS_LOG_FUNCTION (this);
  m_info = "PktsAcked";
  m_tcb = tcb;
  m_segmentsAcked = segmentsAcked;
  m_rtt = rtt;
  Notify();
}

void
TcpGymEnv::CongestionStateSet (Ptr<TcpSocketState> tcb, const TcpSocketState::TcpCongState_t newState)
{
  NS_LOG_FUNCTION (this);
  m_info = "CongestionStateSet";
  m_tcb = tcb;
  m_newState = newState;
  Notify();
}

void
TcpGymEnv::CwndEvent (Ptr<TcpSocketState> tcb, const TcpSocketState::TcpCAEvent_t event)
{
  NS_LOG_FUNCTION (this);
  m_info = "CwndEvent";
  m_tcb = tcb;
  m_event = event;
  Notify();
}


NS_OBJECT_ENSURE_REGISTERED (TcpRl);

TypeId
TcpRl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpRl")
    .SetParent<TcpCongestionOps> ()
    .SetGroupName ("Internet")
    .AddConstructor<TcpRl> ()
  ;
  return tid;
}

TcpRl::TcpRl (void) : TcpCongestionOps ()
{
  NS_LOG_FUNCTION (this);
  m_tcpGymEnv = CreateObject<TcpGymEnv>();
  m_tcpGymEnv->SetTcp(this);
}

TcpRl::TcpRl (const TcpRl& sock)
  : TcpCongestionOps (sock)
{
  NS_LOG_FUNCTION (this);
  m_tcpGymEnv = CreateObject<TcpGymEnv>();
  m_tcpGymEnv->SetTcp(this);
}

TcpRl::~TcpRl (void)
{
}

std::string
TcpRl::GetName () const
{
  return "TcpRl";
}

uint32_t
TcpRl::GetSsThresh (Ptr<const TcpSocketState> state,
                         uint32_t bytesInFlight)
{
  NS_LOG_FUNCTION (this << state << bytesInFlight);
  uint32_t newSsThresh = m_tcpGymEnv->GetSsThresh(state, bytesInFlight);
  return newSsThresh;
}

void
TcpRl::IncreaseWindow (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
{
  NS_LOG_FUNCTION (this << tcb << segmentsAcked);
  m_tcpGymEnv->IncreaseWindow(tcb, segmentsAcked);
}

void
TcpRl::PktsAcked (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked, const Time& rtt)
{
  NS_LOG_FUNCTION (this);
  m_tcpGymEnv->PktsAcked(tcb, segmentsAcked, rtt);
}

void
TcpRl::CongestionStateSet (Ptr<TcpSocketState> tcb, const TcpSocketState::TcpCongState_t newState)
{
  NS_LOG_FUNCTION (this);
  m_tcpGymEnv->CongestionStateSet(tcb, newState);
}

void
TcpRl::CwndEvent (Ptr<TcpSocketState> tcb, const TcpSocketState::TcpCAEvent_t event)
{
  NS_LOG_FUNCTION (this);
  m_tcpGymEnv->CwndEvent(tcb, event);
}

Ptr<TcpCongestionOps>
TcpRl::Fork ()
{
  return CopyObject<TcpRl> (this);
}

} // namespace ns3
