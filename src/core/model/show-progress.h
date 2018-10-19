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


#ifndef SHOW_PROGRESS_H
#define SHOW_PROGRESS_H

/**
 * \file
 * \ingroup core
 * ns3::ShowProgress declaration.
 */

#include <iostream>

#include "nstime.h"
#include "system-wall-clock-ms.h"

namespace ns3 {


/**
 * \ingroup core
 * \ingroup debugging
 *
 * Periodically print a status message indicating simulator progress.
 *
 * Print a status message showing the simulator time and
 * execution speed, relative to wall clock time, as well as the number
 * of events executed each interval.
 *
 * The target update rate (and output stream) can be configured at construction.
 *
 * Example usage:
 *
 * \code
 *     int main (int arg, char ** argv)
 *     {
 *       // Create your model
 *
 *       ShowProgress (Seconds (1), std::cerr);
 *       Simulator::Run ();
 *       Simulator::Destroy ();
 *     }
 * \endcode
 * 
 * Based on a python version by Gustavo Carneiro <gjcarneiro@gmail.com>,
 * as released here:
 * http://mailman.isi.edu/pipermail/ns-developers/2009-January/005201.html
 */
class ShowProgress
{
public:
  /**
   * Constructor.
   * \param [in] interval The target wallclock interval to show progress.
   * \param [in] os The stream to print on.
   */
  ShowProgress (const Time interval = Seconds (1.0),
		std::ostream & os = std::cout);

  /**
   * Set the target update interval, in wallclock time.
   * \param [in] interval The target wallclock interval to show progress.
   */
  void SetInterval (const Time interval);

  /**
   * Set the TimePrinter function to be used
   * to prepend progress messages with the simulation time.
   *
   * The default is DefaultTimePrinter().
   *
   * \param [in] lp The TimePrinter function.
   */
  void SetTimePrinter (TimePrinter lp);

  /**
   * Set the output stream to show progress on.
   * \param [in] os The output stream; defaults to std::cerr.
   */
  void SetStream (std::ostream & os);
  
  /** Start the progress timer. */
  void Start (void);

  /**
   * Set verbose mode to print real and virtual time intervals.
   *
   * \param [in] verbose \c true to turn on verbose mode
   */
  void SetVerbose (bool verbose);

   
private:
  /** Show execution progress. */
  void Feedback (void);

  /**
   * Hysteresis factor.
   * \see Feedback()
   */
  const int64x64_t HYSTERESIS = 1.2;
  /**
   * Maximum growth factor.
   * \see Feedback()
   */
  const int64x64_t MAXGAIN = 2.0;
  
  SystemWallClockMs m_timer;  //!< Wallclock timer
  Time m_interval;            //!< The target update interval, in wallclock time
  Time m_vtime;               //!< The virtual time interval.
  EventId m_event;            //!< The next progress event.
  uint64_t m_eventCount;      //!< Simulator event count

  TimePrinter m_printer;      //!< The TimePrinter to use
  std::ostream *m_os;         //!< The output stream to use.
  bool m_verbose;             //!< Verbose mode flag
  uint64_t m_repCount;        //!< Count of progress lines printed
  
};  // class ShowProgress
  
}  // namespace ns3

#endif  /* SHOW_PROGRESS_H */
