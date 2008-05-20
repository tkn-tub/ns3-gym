
#include "delay-jitter-estimation.h"
#include "ns3/tag.h"
#include "ns3/simulator.h"
#include "ns3/string.h"

namespace ns3 {

class DelayJitterEstimationTimestampTag : public Tag
{
public:
  DelayJitterEstimationTimestampTag ();
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);


  Time GetTxTime (void) const;
private:
  uint64_t m_creationTime;
};

DelayJitterEstimationTimestampTag::DelayJitterEstimationTimestampTag ()
  : m_creationTime (Simulator::Now ().GetTimeStep ())
{}

TypeId 
DelayJitterEstimationTimestampTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("anon::DelayJitterEstimationTimestampTag")
    .SetParent<Tag> ()
    .AddConstructor<DelayJitterEstimationTimestampTag> ()
    .AddAttribute ("CreationTime",
		   "The time at which the timestamp was created",
		   StringValue ("0.0s"),
		   MakeTimeAccessor (&DelayJitterEstimationTimestampTag::GetTxTime),
		   MakeTimeChecker ())
    ;
  return tid;
}
TypeId 
DelayJitterEstimationTimestampTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t 
DelayJitterEstimationTimestampTag::GetSerializedSize (void) const
{
  return 8;
}
void 
DelayJitterEstimationTimestampTag::Serialize (TagBuffer i) const
{
  i.WriteU64 (m_creationTime);
}
void 
DelayJitterEstimationTimestampTag::Deserialize (TagBuffer i)
{
  m_creationTime = i.ReadU64 ();
}
Time 
DelayJitterEstimationTimestampTag::GetTxTime (void) const
{
  return TimeStep (m_creationTime);
}

DelayJitterEstimation::DelayJitterEstimation ()
  : m_previousRx (Simulator::Now ()),
    m_previousRxTx (Simulator::Now ()),
    m_jitter (Seconds (0.0)),
    m_delay (Seconds (0.0))
{}
void 
DelayJitterEstimation::PrepareTx (Ptr<const Packet> packet)
{
  DelayJitterEstimationTimestampTag tag;
  packet->AddTag (tag);
}
void 
DelayJitterEstimation::RecordRx (Ptr<const Packet> packet)
{
  DelayJitterEstimationTimestampTag tag;
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
