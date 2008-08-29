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

#ifndef __DATA_COLLECTOR_H__
#define __DATA_COLLECTOR_H__

#include <list>
#include <string>

#include "ns3/object.h"

namespace ns3 {

  class DataCalculator;

  //------------------------------------------------------------
  //--------------------------------------------
  typedef std::list<Ptr<DataCalculator> > DataCalculatorList;
  typedef std::list<std::pair<std::string, std::string> > MetadataList;

  class DataCollector : public Object {
  public:
    DataCollector();
    virtual ~DataCollector();

    void DescribeRun(std::string experiment,
                     std::string strategy,
                     std::string input,
                     std::string runID,
                     std::string description = "");

    std::string GetExperimentLabel() const { return m_experimentLabel; }
    std::string GetStrategyLabel() const { return m_strategyLabel; }
    std::string GetInputLabel() const { return m_inputLabel; }
    std::string GetRunLabel() const { return m_runLabel; }
    std::string GetDescription() const { return m_description; }

    void AddMetadata(std::string key, std::string value);
    void AddMetadata(std::string key, double value);
    void AddMetadata(std::string key, uint32_t value);
    MetadataList::iterator MetadataBegin();
    MetadataList::iterator MetadataEnd();

    void AddDataCalculator(Ptr<DataCalculator> datac);
    DataCalculatorList::iterator DataCalculatorBegin();
    DataCalculatorList::iterator DataCalculatorEnd();

  protected:
    virtual void DoDispose();

  private:
    std::string m_experimentLabel;
    std::string m_strategyLabel;
    std::string m_inputLabel;
    std::string m_runLabel;
    std::string m_description;

    MetadataList m_metadata;
    DataCalculatorList m_calcList;

    // end class DataCollector
  };

  // end namespace ns3
};

#endif // __DATA_COLLECTOR_H__
