
#include "delay-jitter-estimation.h"
#include "ns3/tag.h"
#include "ns3/simulator.h"
#include "ns3/string.h"

namespace {

class TimestampTag : public ns3::Tag
{
public:
  TimestampTag ();
  static ns3::TypeId GetTypeId (void);
  virtual ns3::TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (ns3::TagBuffer i) const;
  virtual void Deserialize (ns3::TagBuffer i);


  ns3::Time GetTxTime (void) const;
private:
  uint64_t m_creationTime;
};

TimestampTag::TimestampTag ()
  : m_creationTime (ns3::Simulator::Now ().GetTimeStep ())
{}

ns3::TypeId 
TimestampTag::GetTypeId (void)
{
  static ns3::TypeId tid = ns3::TypeId ("anon::TimestampTag")
    .SetParent<Tag> ()
    .AddConstructor<TimestampTag> ()
    .AddAttribute ("CreationTime",
		   "The time at which the timestamp was created",
		   ns3::StringValue ("0.0s"),
		   ns3::MakeTimeAccessor (&TimestampTag::GetTxTime),
		   ns3::MakeTimeChecker ())
    ;
  return tid;
}
ns3::TypeId 
TimestampTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t 
TimestampTag::GetSerializedSize (void) const
{
  return 8;
}
void 
TimestampTag::Serialize (ns3::TagBuffer i) const
{
  i.WriteU64 (m_creationTime);
}
void 
TimestampTag::Deserialize (ns3::TagBuffer i)
{
  m_creationTime = i.ReadU64 ();
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
  found = packet->FindFirstMatchingTag (tag);
  if (!found)
    {
      return;
    }
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
