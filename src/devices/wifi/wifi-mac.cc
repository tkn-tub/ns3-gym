#include "wifi-mac.h"
#include "ns3/uinteger.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (WifiMac);

Time
WifiMac::GetDefaultMaxPropagationDelay (void)
{
  // 1000m 
  return Seconds (1000.0 / 300000000.0);
}
Time
WifiMac::GetDefaultSlot (void)
{
  // 802.11-a specific
  return MicroSeconds (9);
}
Time
WifiMac::GetDefaultSifs (void)
{
  // 802.11-a specific
  return MicroSeconds (16);
}
Time
WifiMac::GetDefaultEifsNoDifs (void)
{
  return GetDefaultSifs () + GetDefaultCtsAckDelay ();
}
Time
WifiMac::GetDefaultCtsAckDelay (void)
{
  // 802.11-a specific: 6mb/s
  return MicroSeconds (44);
}
Time
WifiMac::GetDefaultCtsAckTimeout (void)
{
  /* Cts_Timeout and Ack_Timeout are specified in the Annex C 
     (Formal description of MAC operation, see details on the 
     Trsp timer setting at page 346)
  */
  Time ctsTimeout = GetDefaultSifs ();
  ctsTimeout += GetDefaultCtsAckDelay ();
  ctsTimeout += GetDefaultMaxPropagationDelay () * Scalar (2);
  ctsTimeout += GetDefaultSlot ();
  return ctsTimeout;
}


TypeId 
WifiMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("WifiMac")
    .SetParent<Object> ()
    .AddAttribute ("CtsTimeout", "XXX",
                   GetDefaultCtsAckTimeout (),
                   MakeTimeAccessor (&WifiMac::m_ctsTimeout),
                   MakeTimeChecker ())
    .AddAttribute ("AckTimeout", "XXX",
                   GetDefaultCtsAckTimeout (),
                   MakeTimeAccessor (&WifiMac::m_ackTimeout),
                   MakeTimeChecker ())
    .AddAttribute ("Sifs", "XXX",
                   GetDefaultSifs (),
                   MakeTimeAccessor (&WifiMac::SetSifs,
				     &WifiMac::GetSifs),
                   MakeTimeChecker ())
    .AddAttribute ("EifsNoDifs", "XXX",
		   GetDefaultEifsNoDifs (),
		   MakeTimeAccessor (&WifiMac::SetEifsNoDifs,
				     &WifiMac::GetEifsNoDifs),
		   MakeTimeChecker ())
    .AddAttribute ("Slot", "XXX",
                   GetDefaultSlot (),
                   MakeTimeAccessor (&WifiMac::SetSlot,
				     &WifiMac::GetSlot),
                   MakeTimeChecker ())
    .AddAttribute ("Pifs", "XXX",
                   GetDefaultSifs () + GetDefaultSlot (),
                   MakeTimeAccessor (&WifiMac::m_pifs),
                   MakeTimeChecker ())
    .AddAttribute ("MaxPropagationDelay", "XXX",
                   GetDefaultMaxPropagationDelay (),
                   MakeTimeAccessor (&WifiMac::m_maxPropagationDelay),
                   MakeTimeChecker ())
    .AddAttribute ("MaxMsduSize", "XXX",
		   Uinteger (2304),
		   MakeUintegerAccessor (&WifiMac::m_maxMsduSize),
		   MakeUintegerChecker (1,2304))
    .AddAttribute ("Ssid", "XXX",
		   Ssid ("default"),
		   MakeSsidAccessor (&WifiMac::GetSsid,
				     &WifiMac::SetSsid),
		   MakeSsidChecker ())
    ;
  return tid;
}

void 
WifiMac::SetSlot (Time slotTime)
{
  m_slot = slotTime;
}
void
WifiMac::SetEifsNoDifs (Time eifsNoDifs)
{
  m_eifsNoDifs = eifsNoDifs;
}
void
WifiMac::SetSifs (Time sifs)
{
  m_sifs = sifs;
}
void
WifiMac::SetPifs (Time pifs)
{
  m_pifs = pifs;
}
void 
WifiMac::SetCtsTimeout (Time ctsTimeout)
{
  m_ctsTimeout = ctsTimeout;
}
void 
WifiMac::SetAckTimeout (Time ackTimeout)
{
  m_ackTimeout = ackTimeout;
}
void 
WifiMac::SetMaxPropagationDelay (Time delay)
{
  m_maxPropagationDelay = delay;
}
Time
WifiMac::GetPifs (void) const
{
  return m_pifs;
}
Time
WifiMac::GetSifs (void) const
{
  return m_sifs;
}
Time
WifiMac::GetSlot (void) const
{
  return m_slot;
}
Time 
WifiMac::GetEifsNoDifs (void) const
{
  return m_eifsNoDifs;
}
Time
WifiMac::GetCtsTimeout (void) const
{
  return m_ctsTimeout;
}
Time
WifiMac::GetAckTimeout (void) const
{
  return m_ackTimeout;
}

Time
WifiMac::GetMsduLifetime (void) const
{
  return Seconds (10);
}
Time
WifiMac::GetMaxPropagationDelay (void) const
{
  return m_maxPropagationDelay;
}

uint32_t 
WifiMac::GetMaxMsduSize (void) const
{
  return m_maxMsduSize;
}


} // namespace ns3
