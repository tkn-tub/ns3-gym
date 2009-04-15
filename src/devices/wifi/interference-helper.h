#ifndef INTERFERENCE_HELPER_H
#define INTERFERENCE_HELPER_H

#include <stdint.h>
#include <vector>
#include <list>
#include "wifi-mode.h"
#include "wifi-preamble.h"
#include "ns3/nstime.h"
#include "ns3/ref-count-base.h"

namespace ns3 {

class ErrorRateModel;

class InterferenceHelper
{
public:
  class Event : public RefCountBase
  {
  public:
    Event (uint32_t size, WifiMode payloadMode, 
	     enum WifiPreamble preamble,
	     Time duration, double rxPower);
    virtual ~Event ();
  
    Time GetDuration (void) const;
    Time GetStartTime (void) const;
    Time GetEndTime (void) const;
    bool Overlaps (Time time) const;
    double GetRxPowerW (void) const;
    uint32_t GetSize (void) const;
    WifiMode GetPayloadMode (void) const;
    enum WifiPreamble GetPreambleType (void) const;
  private:
    uint32_t m_size;
    WifiMode m_payloadMode;
    enum WifiPreamble m_preamble;
    Time m_startTime;
    Time m_endTime;
    double m_rxPowerW;
  };
  struct SnrPer 
  {
    double snr;
    double per;
  };

  InterferenceHelper ();
  ~InterferenceHelper ();

  void Configure80211aParameters (void);
  void SetNoiseFigure (double value);
  void SetErrorRateModel (Ptr<ErrorRateModel> rate);

  double GetNoiseFigure (void) const;
  Ptr<ErrorRateModel> GetErrorRateModel (void) const;


  /**
   * \param energyW the minimum energy (W) requested
   * \returns the expected amount of time the observed 
   *          energy on the medium will be higher than
   *          the requested threshold.
   */
  Time GetEnergyDuration (double energyW);
  Time CalculateTxDuration (uint32_t size, WifiMode payloadMode, WifiPreamble preamble) const;
  Ptr<InterferenceHelper::Event> Add (uint32_t size, WifiMode payloadMode, 
				      enum WifiPreamble preamble,
				      Time duration, double rxPower);

  struct InterferenceHelper::SnrPer CalculateSnrPer (Ptr<InterferenceHelper::Event> event);
private:
  class NiChange {
  public:
    NiChange (Time time, double delta);
    Time GetTime (void) const;
    double GetDelta (void) const;
    bool operator < (NiChange const &o) const;
  private:
    Time m_time;
    double m_delta;
  };
  typedef std::vector <NiChange> NiChanges;
  typedef std::list<Ptr<Event> > Events;

  InterferenceHelper (const InterferenceHelper &o);
  InterferenceHelper &operator = (const InterferenceHelper &o);
  void AppendEvent (Ptr<Event> event);
  double CalculateNoiseInterferenceW (Ptr<Event> event, NiChanges *ni) const;
  double CalculateSnr (double signal, double noiseInterference, WifiMode mode) const;
  double CalculateChunkSuccessRate (double snir, Time delay, WifiMode mode) const;
  double CalculatePer (Ptr<const Event> event, NiChanges *ni) const;
  Time GetMaxPacketDuration (void) const;

  uint64_t m_plcpLongPreambleDelayUs;
  uint64_t m_plcpShortPreambleDelayUs;
  WifiMode m_longPlcpHeaderMode;
  WifiMode m_shortPlcpHeaderMode;
  uint32_t m_plcpHeaderLength;
  Time m_maxPacketDuration;
  double m_noiseFigure; /**< noise figure (linear) */
  Events m_events;
  bool m_80211a;
  Ptr<ErrorRateModel> m_errorRateModel;
};

} // namespace ns3

#endif /* INTERFERENCE_HELPER_H */
