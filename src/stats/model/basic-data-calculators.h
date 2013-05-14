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

#ifndef BASIC_DATA_CALCULATORS_H
#define BASIC_DATA_CALCULATORS_H

#include "data-calculator.h"
#include "data-output-interface.h"

namespace ns3 {

/**
 * \defgroup stats Statistics
 *
 */

//------------------------------------------------------------
//--------------------------------------------
template <typename T  = uint32_t>
class MinMaxAvgTotalCalculator : public DataCalculator,
                                 public StatisticalSummary {
public:
  MinMaxAvgTotalCalculator();
  virtual ~MinMaxAvgTotalCalculator();

  void Update (const T i);
  void Reset ();

  virtual void Output (DataOutputCallback &callback) const;

  long getCount () const { return m_count; }
  double getSum () const { return m_total; }
  double getMin () const { return m_min; }
  double getMax () const { return m_max; }
  double getMean () const { return m_meanCurr; }
  double getStddev () const { return std::sqrt (m_varianceCurr); }
  double getVariance () const { return m_varianceCurr; }
  double getSqrSum () const { return m_squareTotal; }

protected:
  virtual void DoDispose (void);

  uint32_t m_count;

  T m_total;
  T m_squareTotal;
  T m_min;
  T m_max;

  double m_meanCurr;
  double m_sCurr;
  double m_varianceCurr;

  double m_meanPrev;
  double m_sPrev;

  // end MinMaxAvgTotalCalculator
};

//----------------------------------------------
template <typename T>
MinMaxAvgTotalCalculator<T>::MinMaxAvgTotalCalculator()
{
  m_count = 0;

  m_total       = 0;
  m_squareTotal = 0;

  m_meanCurr     = NaN;
  m_sCurr        = NaN;
  m_varianceCurr = NaN;

  m_meanPrev     = NaN;
  m_sPrev        = NaN;
}

template <typename T>
MinMaxAvgTotalCalculator<T>::~MinMaxAvgTotalCalculator()
{
}
template <typename T>
void
MinMaxAvgTotalCalculator<T>::DoDispose (void)
{
  DataCalculator::DoDispose ();
  // MinMaxAvgTotalCalculator::DoDispose
}

template <typename T>
void
MinMaxAvgTotalCalculator<T>::Update (const T i)
{
  if (m_enabled) {
      m_count++;

      m_total       += i;
      m_squareTotal += i*i;

      if (m_count == 1)
        {
          m_min = i;
          m_max = i;
        }
      else
        {
          if (i < m_min)
            {
              m_min = i;
            }
          if (i > m_max)
            {
              m_max = i;
            }
        }

      // Calculate the variance based on equations (15) and (16) on
      // page 216 of "The Art of Computer Programming, Volume 2",
      // Second Edition. Donald E. Knuth.  Addison-Wesley
      // Publishing Company, 1973.
      //
      // The relationships between the variance, standard deviation,
      // and s are as follows
      //
      //                      s
      //     variance  =  -----------
      //                   count - 1
      //
      //                                -------------
      //                               /
      //     standard_deviation  =    /  variance
      //                            \/
      //
      if (m_count == 1)
        {
          // Set the very first values.
          m_meanCurr     = i;
          m_sCurr        = 0;
          m_varianceCurr = m_sCurr;
        }
      else
        {
          // Save the previous values.
          m_meanPrev     = m_meanCurr;
          m_sPrev        = m_sCurr;

          // Update the current values.
          m_meanCurr     = m_meanPrev + (i - m_meanPrev) / m_count;
          m_sCurr        = m_sPrev    + (i - m_meanPrev) * (i - m_meanCurr);
          m_varianceCurr = m_sCurr / (m_count - 1);
        }
    }
  // end MinMaxAvgTotalCalculator::Update
}

template <typename T>
void
MinMaxAvgTotalCalculator<T>::Reset ()
{
  m_count = 0;

  m_total       = 0;
  m_squareTotal = 0;

  m_meanCurr     = NaN;
  m_sCurr        = NaN;
  m_varianceCurr = NaN;

  m_meanPrev     = NaN;
  m_sPrev        = NaN;
  // end MinMaxAvgTotalCalculator::Reset
}

template <typename T>
void
MinMaxAvgTotalCalculator<T>::Output (DataOutputCallback &callback) const
{
  callback.OutputStatistic (m_context, m_key, this);
}


/**
 * \ingroup stats
 *
 */
//------------------------------------------------------------
//--------------------------------------------
template <typename T  = uint32_t>
class CounterCalculator : public DataCalculator {
public:
  CounterCalculator();
  virtual ~CounterCalculator();

  void Update ();
  void Update (const T i);

  T GetCount () const;

  virtual void Output (DataOutputCallback &callback) const;

protected:
  virtual void DoDispose (void);

  T m_count;

  // end CounterCalculator
};


//--------------------------------------------
template <typename T>
CounterCalculator<T>::CounterCalculator() :
  m_count (0)
{
}

template <typename T>
CounterCalculator<T>::~CounterCalculator()
{
}
template <typename T>
void
CounterCalculator<T>::DoDispose (void)
{
  DataCalculator::DoDispose ();
  // CounterCalculator::DoDispose
}

template <typename T>
void
CounterCalculator<T>::Update ()
{
  if (m_enabled) {
      m_count++;
    }
  // end CounterCalculator::Update
}

template <typename T>
void
CounterCalculator<T>::Update (const T i)
{
  if (m_enabled) {
      m_count += i;
    }
  // end CounterCalculator::Update
}

template <typename T>
T
CounterCalculator<T>::GetCount () const
{
  return m_count;
  // end CounterCalculator::GetCount
}

template <typename T>
void
CounterCalculator<T>::Output (DataOutputCallback &callback) const
{
  callback.OutputSingleton (m_context, m_key, m_count);
  // end CounterCalculator::Output
}

// end namespace ns3
};


#endif /* BASIC_DATA_CALCULATORS_H */
