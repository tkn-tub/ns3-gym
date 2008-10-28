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

#ifndef __DATA_CALCULATOR_H__
#define __DATA_CALCULATOR_H__

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/simulator.h"

namespace ns3 {

  class DataOutputCallback;

  //------------------------------------------------------------
  //--------------------------------------------
  class DataCalculator : public Object {
  public:
    DataCalculator();
    virtual ~DataCalculator();

    bool GetEnabled() const;
    void Enable();
    void Disable();

    void SetKey(const std::string key);
    std::string GetKey() const;

    virtual void Start(const Time& startTime);
    virtual void Stop(const Time& stopTime);

    virtual void Output(DataOutputCallback &callback) const = 0;

  protected:
    bool m_enabled;  // Descendant classes *must* check & respect m_enabled!

    std::string m_key;

    virtual void DoDispose(void);

  private:
    EventId m_startEvent;
    EventId m_stopEvent;

    // end class DataCalculator
  };


  // end namespace ns3
};


#endif // __DATA_CALCULATOR_H__
