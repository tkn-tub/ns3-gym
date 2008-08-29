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

#ifndef __BASIC_DATA_CALCULATORS_H__
#define __BASIC_DATA_CALCULATORS_H__

#include "data-calculator.h"
#include "data-output-interface.h"

namespace ns3 {

  //------------------------------------------------------------
  //--------------------------------------------
  template <typename T  = uint32_t>
  class MinMaxAvgTotalCalculator : public DataCalculator {
  public:
    MinMaxAvgTotalCalculator();
    virtual ~MinMaxAvgTotalCalculator();

    void Update(const T i);

    virtual void Output(DataOutputCallback &callback) const;

  protected:
    virtual void DoDispose(void);

    uint32_t m_count;
    T m_total, m_min, m_max;

    // end MinMaxAvgTotalCalculator
  };

  //----------------------------------------------
  template <typename T>
  MinMaxAvgTotalCalculator<T>::MinMaxAvgTotalCalculator()
  {
    m_count = 0;
    m_total = 0;
    m_min = ~0;
    m_max = 0;
  }

  template <typename T>
  MinMaxAvgTotalCalculator<T>::~MinMaxAvgTotalCalculator()
  {
  }
  template <typename T>
  void
  MinMaxAvgTotalCalculator<T>::DoDispose(void)
  {
    DataCalculator::DoDispose();
    // MinMaxAvgTotalCalculator::DoDispose
  }

  template <typename T>
  void
  MinMaxAvgTotalCalculator<T>::Update(const T i)
  {
    if (m_enabled) {
      m_total += i;

      if (i < m_min)
        m_min = i;

      if (i > m_max)
        m_max = i;

      m_count++;
    }
    // end MinMaxAvgTotalCalculator::Update
  }
  template <typename T>
  void
  MinMaxAvgTotalCalculator<T>::Output(DataOutputCallback &callback) const
  {
    callback.OutputSingleton(m_key, "count", m_count);
    if (m_count > 0) {
      callback.OutputSingleton(m_key, "total", m_total);
      callback.OutputSingleton(m_key, "average", m_total/m_count);
      callback.OutputSingleton(m_key, "max", m_max);
      callback.OutputSingleton(m_key, "min", m_min);
    }
    // end MinMaxAvgTotalCalculator::Output
  }




  //------------------------------------------------------------
  //--------------------------------------------
  template <typename T  = uint32_t>
  class CounterCalculator : public DataCalculator {
  public:
    CounterCalculator();
    virtual ~CounterCalculator();

    void Update();
    void Update(const T i);

    T GetCount() const;

    virtual void Output(DataOutputCallback &callback) const;

  protected:
    virtual void DoDispose(void);

    T m_count;

    // end CounterCalculator
  };


  //--------------------------------------------
  template <typename T>
  CounterCalculator<T>::CounterCalculator() :
    m_count(0)
  {
  }

  template <typename T>
  CounterCalculator<T>::~CounterCalculator()
  {
  }
  template <typename T>
  void
  CounterCalculator<T>::DoDispose(void)
  {
    DataCalculator::DoDispose();
    // CounterCalculator::DoDispose
  }

  template <typename T>
  void
  CounterCalculator<T>::Update()
  {
    if (m_enabled) {
      m_count++;
    }
    // end CounterCalculator::Update
  }

  template <typename T>
  void
  CounterCalculator<T>::Update(const T i)
  {
    if (m_enabled) {
      m_count += i;
    }
    // end CounterCalculator::Update
  }

  template <typename T>
  T
  CounterCalculator<T>::GetCount() const
  {
    return m_count;
    // end CounterCalculator::GetCount
  }

  template <typename T>
  void
  CounterCalculator<T>::Output(DataOutputCallback &callback) const
  {
    callback.OutputSingleton(m_key, "count", m_count);
    // end CounterCalculator::Output
  }

  // end namespace ns3
};


#endif // __BASIC_DATA_CALCULATORS_H__
