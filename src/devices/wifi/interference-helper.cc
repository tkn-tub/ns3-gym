#include "interference-helper.h"
#include "wifi-phy.h"
#include "error-rate-model.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include <algorithm>

NS_LOG_COMPONENT_DEFINE ("InterferenceHelper");

namespace ns3 {

/****************************************************************
 *       Phy event class
 ****************************************************************/

InterferenceHelper::Event::Event (uint32_t size, WifiMode payloadMode, 
					       enum WifiPreamble preamble,
					       Time duration, double rxPower)
  : m_size (size),
    m_payloadMode (payloadMode),
    m_preamble (preamble),
    m_startTime (Simulator::Now ()),
    m_endTime (m_startTime + duration),
    m_rxPowerW (rxPower)
{}
InterferenceHelper::Event::~Event ()
{}
  
Time 
InterferenceHelper::Event::GetDuration (void) const 
{
  return m_endTime - m_startTime;
}
Time 
InterferenceHelper::Event::GetStartTime (void) const
{
  return m_startTime;
}
Time 
InterferenceHelper::Event::GetEndTime (void) const
{
  return m_endTime;
}
bool 
InterferenceHelper::Event::Overlaps (Time time) const
{
  if (m_startTime <= time &&
      m_endTime >= time) 
    {
      return true;
    } 
  else 
    {
      return false;
    }
}
double 
InterferenceHelper::Event::GetRxPowerW (void) const
{
  return m_rxPowerW;
}
uint32_t 
InterferenceHelper::Event::GetSize (void) const
{
  return m_size;
}
WifiMode 
InterferenceHelper::Event::GetPayloadMode (void) const
{
  return m_payloadMode;
}
enum WifiPreamble 
InterferenceHelper::Event::GetPreambleType (void) const
{
  return m_preamble;
}

/****************************************************************
 *       Class which records SNIR change events for a 
 *       short period of time.
 ****************************************************************/

InterferenceHelper::NiChange::NiChange (Time time, double delta)
  : m_time (time), m_delta (delta) 
{}
Time
InterferenceHelper::NiChange::GetTime (void) const
{
  return m_time;
}
double 
InterferenceHelper::NiChange::GetDelta (void) const
{
  return m_delta;
}
bool 
InterferenceHelper::NiChange::operator < (InterferenceHelper::NiChange const &o) const
{
  return (m_time < o.m_time)?true:false;
}

/****************************************************************
 *       The actual InterferenceHelper
 ****************************************************************/

InterferenceHelper::InterferenceHelper ()
  : m_80211a (false),
    m_errorRateModel (0)
{}
InterferenceHelper::~InterferenceHelper ()
{
  m_errorRateModel = 0;
}

Ptr<InterferenceHelper::Event> 
InterferenceHelper::Add (uint32_t size, WifiMode payloadMode, 
			 enum WifiPreamble preamble,
			 Time duration, double rxPowerW)
{
  Ptr<InterferenceHelper::Event> event;

  event = Create<InterferenceHelper::Event> 
    (size,
     payloadMode,
     preamble,
     duration,
     rxPowerW);

  AppendEvent (event);
  return event;
}

Time 
InterferenceHelper::GetMaxPacketDuration (void) const
{
  return m_maxPacketDuration;
}

void 
InterferenceHelper::SetNoiseFigure (double value)
{
  m_noiseFigure = value;
}

double 
InterferenceHelper::GetNoiseFigure (void) const
{
  return m_noiseFigure;
}

void 
InterferenceHelper::SetErrorRateModel (Ptr<ErrorRateModel> rate)
{
  m_errorRateModel = rate;
}

Ptr<ErrorRateModel> 
InterferenceHelper::GetErrorRateModel (void) const
{
  return m_errorRateModel;
}

Time 
InterferenceHelper::GetEnergyDuration (double energyW)
{
  Time now = Simulator::Now ();

  // first, we iterate over all events and, each event
  // which contributes energy to the channel now is 
  // appended to the noise interference array.
  Events::const_iterator i = m_events.begin ();
  double noiseInterferenceW = 0.0;
  NiChanges ni;
  while (i != m_events.end ()) 
    {
      Ptr<Event> ev = *i;
      NS_ASSERT (ev->GetStartTime () <= now);
      if (ev->GetEndTime () > now)
	{
          ni.push_back (NiChange (ev->GetEndTime (), -ev->GetRxPowerW ()));
          noiseInterferenceW += ev->GetRxPowerW ();
	}
      i++;
    }
  if (noiseInterferenceW < energyW)
    {
      return MicroSeconds (0);
    }

  /* quicksort vector of NI changes by time. 
   */
  std::sort (ni.begin (), ni.end (), std::less<NiChange> ());

  // Now, we iterate the piecewise linear noise function
  Time end = now;
  for (NiChanges::const_iterator i = ni.begin (); i != ni.end (); i++) 
    {
      noiseInterferenceW += i->GetDelta ();
      end = i->GetTime ();
      if (noiseInterferenceW < energyW) 
	{
	  break;
	}
    }
  return end - now;
}

Time
InterferenceHelper::CalculateTxDuration (uint32_t size, WifiMode payloadMode, WifiPreamble preamble) const
{
  NS_ASSERT (m_80211a);
  uint64_t delay = 0;
  delay += m_plcpLongPreambleDelayUs;
  // symbol duration is 4us
  delay += 4;
  delay += lrint (ceil ((size * 8.0 + 16.0 + 6.0) / payloadMode.GetDataRate () / 4e-6) * 4);
  return MicroSeconds (delay);
}

void
InterferenceHelper::Configure80211aParameters (void)
{
  NS_LOG_FUNCTION (this);
  m_80211a = true;
  m_plcpLongPreambleDelayUs = 16;
  m_plcpShortPreambleDelayUs = 16;
  m_longPlcpHeaderMode = WifiPhy::Get6mba ();
  m_shortPlcpHeaderMode = WifiPhy::Get6mba ();
  m_plcpHeaderLength = 4 + 1 + 12 + 1 + 6;
  /* 4095 bytes at a 6Mb/s rate with a 1/2 coding rate. */
  m_maxPacketDuration = CalculateTxDuration (4095, WifiPhy::Get6mba (), WIFI_PREAMBLE_LONG);
}

void 
InterferenceHelper::AppendEvent (Ptr<InterferenceHelper::Event> event)
{
  /* attempt to remove the events which are 
   * not useful anymore. 
   * i.e.: all events which end _before_
   *       now - m_maxPacketDuration
   */
  
  if (Simulator::Now () > GetMaxPacketDuration ())
    {
      Time end = Simulator::Now () - GetMaxPacketDuration ();
      Events::iterator i = m_events.begin ();
      while (i != m_events.end () &&
             (*i)->GetEndTime () <= end) 
        {
          i++;
        }
      m_events.erase (m_events.begin (), i);
    } 
  m_events.push_back (event);
}


double
InterferenceHelper::CalculateSnr (double signal, double noiseInterference, WifiMode mode) const
{
  // thermal noise at 290K in J/s = W
  static const double BOLTZMANN = 1.3803e-23;
  // Nt is the power of thermal noise in W
  double Nt = BOLTZMANN * 290.0 * mode.GetBandwidth ();
  // receiver noise Floor (W) which accounts for thermal noise and non-idealities of the receiver
  double noiseFloor = m_noiseFigure * Nt;
  double noise = noiseFloor + noiseInterference;
  double snr = signal / noise;
  return snr;
}

double
InterferenceHelper::CalculateNoiseInterferenceW (Ptr<InterferenceHelper::Event> event, NiChanges *ni) const
{
  Events::const_iterator i = m_events.begin ();
  double noiseInterference = 0.0;
  while (i != m_events.end ()) 
    {
      if (event == (*i)) 
        {
          i++;
          continue;
        }
      if (event->Overlaps ((*i)->GetStartTime ())) 
        {
          ni->push_back (NiChange ((*i)->GetStartTime (), (*i)->GetRxPowerW ()));
        }
      if (event->Overlaps ((*i)->GetEndTime ())) 
        {
          ni->push_back (NiChange ((*i)->GetEndTime (), -(*i)->GetRxPowerW ()));
        }
      if ((*i)->Overlaps (event->GetStartTime ())) 
        {
          noiseInterference += (*i)->GetRxPowerW ();
        }
      i++;
    }
  ni->push_back (NiChange (event->GetStartTime (), noiseInterference));
  ni->push_back (NiChange (event->GetEndTime (), 0));

  /* quicksort vector of NI changes by time. */
  std::sort (ni->begin (), ni->end (), std::less<NiChange> ());

  return noiseInterference;
}

double
InterferenceHelper::CalculateChunkSuccessRate (double snir, Time duration, WifiMode mode) const
{
  if (duration == NanoSeconds (0)) {
    return 1.0;
  }
  uint32_t rate = mode.GetPhyRate ();
  uint64_t nbits = (uint64_t)(rate * duration.GetSeconds ());
  double csr = m_errorRateModel->GetChunkSuccessRate (mode, snir, (uint32_t)nbits);
  return csr;
}

double 
InterferenceHelper::CalculatePer (Ptr<const InterferenceHelper::Event> event, NiChanges *ni) const
{  
  double psr = 1.0; /* Packet Success Rate */
  NiChanges::iterator j = ni->begin ();
  Time previous = (*j).GetTime ();
  uint64_t plcpPreambleDelayUs;
  WifiMode payloadMode = event->GetPayloadMode ();
  WifiMode headerMode;
  switch (event->GetPreambleType ()) {
  case WIFI_PREAMBLE_LONG:
    plcpPreambleDelayUs = m_plcpLongPreambleDelayUs;
    headerMode = m_longPlcpHeaderMode;
    break;
  case WIFI_PREAMBLE_SHORT:
    plcpPreambleDelayUs = m_plcpShortPreambleDelayUs;
    headerMode = m_shortPlcpHeaderMode;
    break;
  default:
    NS_ASSERT (false);
    // only to quiet compiler. Really stupid.
    plcpPreambleDelayUs = 0;
    headerMode = m_shortPlcpHeaderMode;
    break;
  }
  Time plcpHeaderStart = (*j).GetTime () + MicroSeconds (plcpPreambleDelayUs);
  Time plcpPayloadStart = plcpHeaderStart + 
    Seconds ((m_plcpHeaderLength + 0.0) / headerMode.GetDataRate ());
  double noiseInterferenceW = (*j).GetDelta ();
  double powerW = event->GetRxPowerW ();

  j++;
  while (ni->end () != j) 
    {
      Time current = (*j).GetTime ();
      NS_ASSERT (current >= previous);
    
      if (previous >= plcpPayloadStart) 
        {
          psr *= CalculateChunkSuccessRate (CalculateSnr (powerW, 
                                                          noiseInterferenceW, 
                                                          payloadMode), 
                                            current - previous,
                                            payloadMode);
        } 
      else if (previous >= plcpHeaderStart) 
        {
          if (current >= plcpPayloadStart) 
            {
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW, 
                                                              noiseInterferenceW, 
                                                              headerMode), 
                                                plcpPayloadStart - previous,
                                                headerMode);
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW, 
                                                              noiseInterferenceW, 
                                                              payloadMode),
                                                current - plcpPayloadStart,
                                                payloadMode);
            } 
          else 
            {
              NS_ASSERT (current >= plcpHeaderStart);
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW, 
                                                              noiseInterferenceW, 
                                                              headerMode), 
                                                current - previous,
                                                headerMode);
            }
        } 
      else 
        {
          if (current >= plcpPayloadStart) 
            {
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW, 
                                                              noiseInterferenceW, 
                                                              headerMode), 
                                                plcpPayloadStart - plcpHeaderStart,
                                                headerMode);
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW, 
                                                              noiseInterferenceW, 
                                                              payloadMode), 
                                                current - plcpPayloadStart,
                                                payloadMode);
            } 
          else if (current >= plcpHeaderStart) 
            {
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW, 
                                                              noiseInterferenceW, 
                                                              headerMode), 
                                                current - plcpHeaderStart,
                                                headerMode);
            }
        }

      noiseInterferenceW += (*j).GetDelta ();
      previous = (*j).GetTime ();
      j++;
    }

  double per = 1 - psr;
  return per;
}


struct InterferenceHelper::SnrPer 
InterferenceHelper::CalculateSnrPer (Ptr<InterferenceHelper::Event> event)
{
  NiChanges ni;
  double noiseInterferenceW = CalculateNoiseInterferenceW (event, &ni);
  double snr = CalculateSnr (event->GetRxPowerW (),
                             noiseInterferenceW,
                             event->GetPayloadMode ());
  
  /* calculate the SNIR at the start of the packet and accumulate
   * all SNIR changes in the snir vector.
   */
  double per = CalculatePer (event, &ni);

  struct SnrPer snrPer;
  snrPer.snr = snr;
  snrPer.per = per;
  return snrPer;
}



} // namespace ns3
