/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * XXXX
 */

#include "ns2-calendar-scheduler.h"
#include "event-impl.h"
#include <utility>
#include <string>
#include <list>
#include "ns3/assert.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Ns2CalendarScheduler");

NS_OBJECT_ENSURE_REGISTERED (Ns2CalendarScheduler);

TypeId 
Ns2CalendarScheduler::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ns2CalendarScheduler")
    .SetParent<Scheduler> ()
    .AddConstructor<Ns2CalendarScheduler> ()
    ;
  return tid;
}

Ns2CalendarScheduler::Ns2CalendarScheduler ()
{
  NS_LOG_FUNCTION (this);
}
Ns2CalendarScheduler::~Ns2CalendarScheduler ()
{
  NS_LOG_FUNCTION (this);
}
void
Ns2CalendarScheduler::Insert (const Event &ev)
{
  NS_LOG_FUNCTION (this);
}
bool 
Ns2CalendarScheduler::IsEmpty (void) const
{
  return false;
}
Scheduler::Event
Ns2CalendarScheduler::PeekNext (void) const
{
  NS_LOG_FUNCTION (this);
  return Scheduler::Event ();
}


Scheduler::Event
Ns2CalendarScheduler::RemoveNext (void)
{
  NS_LOG_FUNCTION (this);
  return Scheduler::Event ();
}

void
Ns2CalendarScheduler::Remove (const Event &ev)
{
  NS_ASSERT (!IsEmpty ());
  NS_ASSERT (false);
}

} // namespace ns3
