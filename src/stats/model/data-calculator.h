/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 Drexel University
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
 * Author: Joe Kopena (tjkopena@cs.drexel.edu)
 */

#ifndef DATA_CALCULATOR_H
#define DATA_CALCULATOR_H

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/simulator.h"

namespace ns3 {
extern const double NaN;
inline bool isNaN (double x) { return x != x; }

class DataOutputCallback;

class StatisticalSummary {
public:
  /**
   * Destructor
   */
  virtual ~StatisticalSummary ()
  {
  }
  /**
   * Returns the number of the observations.
   */
  virtual long getCount () const = 0;

  /**
   * Returns the sum of the values.
   * @see getWeightedSum()
   */
  virtual double getSum () const = 0;

  /**
   * Returns the sum of the squared values.
   * @see getWeightedSqrSum()
   */
  virtual double getSqrSum () const = 0;

  /**
   * Returns the minimum of the values.
   */
  virtual double getMin () const = 0;

  /**
   * Returns the maximum of the values.
   */
  virtual double getMax () const = 0;

  /**
   * Returns the mean of the (weighted) observations.
   */
  virtual double getMean () const = 0;

  /**
   * Returns the standard deviation of the (weighted) observations.
   */
  virtual double getStddev () const = 0;

  /**
   * Returns the variance of the (weighted) observations.
   */
  virtual double getVariance () const = 0;
};

//------------------------------------------------------------
//--------------------------------------------
class DataCalculator : public Object {
public:
  DataCalculator();
  virtual ~DataCalculator();

  bool GetEnabled () const;
  void Enable ();
  void Disable ();

  void SetKey (const std::string key);
  std::string GetKey () const;

  void SetContext (const std::string context);
  std::string GetContext () const;

  virtual void Start (const Time& startTime);
  virtual void Stop (const Time& stopTime);

  virtual void Output (DataOutputCallback &callback) const = 0;

protected:
  bool m_enabled;    // Descendant classes *must* check & respect m_enabled!

  std::string m_key;
  std::string m_context;

  virtual void DoDispose (void);

private:
  EventId m_startEvent;
  EventId m_stopEvent;

  // end class DataCalculator
};


// end namespace ns3
};


#endif /* DATA_CALCULATOR_H */
