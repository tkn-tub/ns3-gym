/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017 Lawrence Livermore National Laboratory
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Gustavo Carneiro <gjc@inescporto.pt>
 * Author: Peter D. Barnes, Jr. <pdbarnes@llnl.gov>
 */

/**
 * \file
 * \ingroup core
 * ns3::ShowProgress implementation.
 */

#include <iomanip>

#include "event-id.h"
#include "log.h"
#include "nstime.h"
#include "simulator.h"
#include "singleton.h"

#include "show-progress.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ShowProgress");  

ShowProgress::ShowProgress (const Time interval /* = Seconds (1.0) */,
                            std::ostream & os /* = std::cout */)
  : m_timer (),
    m_interval (interval),
    m_vtime (Seconds (1.0)),
    m_event (),
    m_eventCount (0),
    m_printer (DefaultTimePrinter),
    m_os (&os),
    m_verbose (false),
    m_repCount (0)
{
  NS_LOG_FUNCTION (this << interval);
  Start ();
}

void
ShowProgress::SetInterval (const Time interval)
{
  NS_LOG_FUNCTION (this << interval);
  const int64x64_t ratio = interval  / m_interval;
  m_interval = interval;
  m_vtime = m_vtime * ratio;
  Simulator::Cancel (m_event);
  Start ();

}  // ShowProgress::SetInterval

void
ShowProgress::SetTimePrinter (TimePrinter lp)
{
  NS_LOG_FUNCTION (this << lp);
  m_printer = lp;
}

void
ShowProgress::SetVerbose (bool verbose)
{
  NS_LOG_FUNCTION (this << verbose);
  m_verbose = verbose;
}
  
void
ShowProgress::SetStream (std::ostream & os)
{
  m_os = &os;
}
  
void
ShowProgress::Start (void)
{
  NS_LOG_FUNCTION (this);
  m_event = Simulator::Schedule (m_vtime, &ShowProgress::Feedback, this);
  m_timer.Start ();
  
}  // ShowProgress::Start

void
ShowProgress::Feedback (void)
{
  // Get elapsed wall clock time
  Time elapsed = MilliSeconds (m_timer.End ());
  NS_LOG_FUNCTION (this << elapsed);

  // Don't do anything unless the elapsed time is positive.
  if (elapsed <= Time (0))
    {
      m_vtime = m_vtime * MAXGAIN;
      Start ();
      return;
    }

  // Speed: how fast are we compared to real time
  const int64x64_t speed = m_vtime / elapsed;
  
  // Ratio: how much real time did we use,
  // compared to reporting interval target
  const int64x64_t ratio = elapsed / m_interval;

  // Elapsed event count
  uint64_t events = Simulator::GetEventCount ();
  uint64_t nEvents = events - m_eventCount;
  m_eventCount = events;
  
  /**
   * \internal Update algorithm
   *
   * We steer \c m_vtime to obtain updates approximately every
   * \c m_interval in wall clock time.  To smooth things out a little
   * we impose a hysteresis band around \c m_interval where we
   * don't change \c m_vtime.  To avoid too rapid movements
   * chasing spikes or dips in execution rate, we bound the
   * change in \c m_vtime to a maximum factor.
   *
   * In mathematical terms, we compute the ratio of elapsed wall clock time
   * compared to the target reporting interval:
   * \f[ ratio = \frac{elapsed}{target interval)} \f]
   *
   * Graphically, the windows in ratio value and the corresponding
   * updates to \c m_vtime are sketched in this figure:
   * \verbatim
                      ^
                      |   
              ratio   |   vtime update
                      |
                      |   
                      |   *= MAXGAIN
                      |   
            MAXGAIN  -|--------------    *= min (ratio, MAXGAIN)
                      |   
                      |   *= ratio
                      |   
         HYSTERESIS  -|=============================================
                      |   
                      |   
                      |   
                   1 -|   No change
                      |   
                      |   
                      |
      1/ HYSTERESIS  -|==============================================
                      |   
                      |   *= ratio
                      |   
         1/ MAXGAIN  -|---------------   *=  max (ratio, 1/ MAXGAIN)   
                      |   
                      |   *= 1/MAXGAIN
                      |   
     \endverbatim
   *
   * As indicated, when ratio is outside the hysteresis band
   * it amounts to multiplying \c m_vtime by the min/max of the ratio
   * with the appropriate MAXGAIN factor.
   */
  if (ratio > HYSTERESIS)
    {
      m_vtime = m_vtime / std::min (ratio, MAXGAIN);
    }
  else if (ratio < 1.0 / HYSTERESIS)
    {
      m_vtime = m_vtime / std::max (ratio, 1.0 / MAXGAIN);
    }

  // Save stream state
  auto precision = m_os->precision ();
  auto flags = m_os->flags ();
  
  m_os->setf (std::ios::fixed, std:: ios::floatfield);

  if (m_verbose)
    {
      (*m_os) << m_repCount
              << " [del: " << elapsed.As(Time::S)
              << "/ int: " << m_interval.As(Time::S)
              << " = rat: " << ratio 
              << (ratio > HYSTERESIS ? " up" :
                  (ratio < 1.0 / HYSTERESIS ? " dn" : " --"))
              << ", vt: " << m_vtime.As(Time::S) << "] ";
    }
  m_repCount++;
  
  // Print the current time
  (*m_printer)(*m_os);
  
  (*m_os) << " ("
          << std::setprecision (3) << std::setw (8) << speed.GetDouble () << "x real time) "
          << nEvents << " events processed"
          << std::endl
          << std::flush;

  // Restore stream state
  m_os->precision (precision);
  m_os->flags (flags);

  // And do it again
  Start ();

}  // ShowProgress::Feedback

  
}  // namespace ns3
