
#include "delay-jitter-estimation.h"
#include "ns3/tag.h"
#include "ns3/simulator.h"

namespace {

class TimestampTag : public ns3::Tag
{
public:
  TimestampTag ();
  static uint32_t GetUid (void);
  void Print (std::ostream &os) const;
  void Serialize (ns3::Buffer::Iterator start) const;
  uint32_t Deserialize (ns3::Buffer::Iterator start);
  uint32_t GetSerializedSize (void) const;

  ns3::Time GetTxTime (void) const;
private:
  uint64_t m_creationTime;
};

TimestampTag::TimestampTag ()
  : m_creationTime (ns3::Simulator::Now ().GetTimeStep ())
{}
uint32_t 
TimestampTag::GetUid (void)
{
  static uint32_t uid = ns3::Tag::AllocateUid<TimestampTag> ("mathieu.paper.TimestampTag");
  return uid;
}
void 
TimestampTag::Print (std::ostream &os) const
{
  os << ns3::TimeStep (m_creationTime);
}
void 
TimestampTag::Serialize (ns3::Buffer::Iterator start) const
{}
uint32_t 
TimestampTag::Deserialize (ns3::Buffer::Iterator start)
{
  return 0;
}
uint32_t 
TimestampTag::GetSerializedSize (void) const
{
  return 0;
}
ns3::Time 
TimestampTag::GetTxTime (void) const
{
  return ns3::TimeStep (m_creationTime);
}

}

namespace ns3 {

DelayJitterEstimation::DelayJitterEstimation ()
  : m_previousRx (Simulator::Now ()),
    m_previousRxTx (Simulator::Now ()),
    m_jitter (Seconds (0.0)),
    m_delay (Seconds (0.0))
{}
void 
DelayJitterEstimation::PrepareTx (Ptr<const Packet> packet)
{
  TimestampTag tag;
  packet->AddTag (tag);
}
void 
DelayJitterEstimation::RecordRx (Ptr<const Packet> packet)
{
  TimestampTag tag;
  bool found;
  found = packet->PeekTag (tag);
  NS_ASSERT (found);
  tag.GetTxTime ();

  Time delta = (Simulator::Now () - m_previousRx) - (tag.GetTxTime () - m_previousRxTx);
  m_jitter += (Abs (delta) - m_jitter ) / Scalar (16.0);
  m_previousRx = Simulator::Now ();
  m_previousRxTx = tag.GetTxTime ();
  m_delay = Simulator::Now () - tag.GetTxTime ();
}

Time 
DelayJitterEstimation::GetLastDelay (void) const
{
  return m_delay;
}
Time
DelayJitterEstimation::GetLastJitter (void) const
{
  return m_jitter;
}

} // namespace ns3
