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

#include <fstream>

#include "ns3/log.h"
#include "ns3/nstime.h"

#include "data-collector.h"
#include "data-calculator.h"
#include "omnet-data-output.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("OmnetDataOutput");


//--------------------------------------------------------------
//----------------------------------------------
OmnetDataOutput::OmnetDataOutput()
{
  m_filePrefix = "data";

  NS_LOG_FUNCTION_NOARGS();
}
OmnetDataOutput::~OmnetDataOutput()
{
  NS_LOG_FUNCTION_NOARGS();
}
void
OmnetDataOutput::DoDispose()
{
  NS_LOG_FUNCTION_NOARGS();

  DataOutputInterface::DoDispose();
  // end OmnetDataOutput::DoDispose
}

//----------------------------------------------
void
OmnetDataOutput::Output(DataCollector &dc)
{

  std::ofstream scalarFile;
  std::string fn = m_filePrefix + ".sca";
  scalarFile.open(fn.c_str(), std::ios_base::app);

  scalarFile << std::endl;
  scalarFile << "run " << dc.GetRunLabel() << std::endl;
  scalarFile << std::endl;
  scalarFile << "attr experiment \"" << dc.GetExperimentLabel()
            << "\"" << std::endl;
  scalarFile << "attr strategy \"" << dc.GetStrategyLabel()
            << "\"" << std::endl;
  scalarFile << "attr input \"" << dc.GetInputLabel()
            << "\"" << std::endl;
  scalarFile << "attr description \"" << dc.GetDescription()
            << "\"" << std::endl;
  scalarFile << std::endl;

  for (MetadataList::iterator i = dc.MetadataBegin();
       i != dc.MetadataEnd(); i++) {
    std::pair<std::string, std::string> blob = (*i);
    scalarFile << "attr \"" << blob.first << "\" \"" << blob.second << "\""
               << std::endl;
  }

  scalarFile << std::endl;

  OmnetOutputCallback callback(&scalarFile);

  for (DataCalculatorList::iterator i = dc.DataCalculatorBegin();
       i != dc.DataCalculatorEnd(); i++) {
    (*i)->Output(callback);
  }

  scalarFile << std::endl << std::endl;
  scalarFile.close();

  // end OmnetDataOutput::Output
}

OmnetDataOutput::OmnetOutputCallback::OmnetOutputCallback
  (std::ostream *scalar) :
  m_scalar(scalar)
{
}

void
OmnetDataOutput::OmnetOutputCallback::OutputSingleton(std::string key,
                                                      std::string variable,
                                                      int val)
{
  (*m_scalar) << "scalar " << key << " " << variable << " " << val << std::endl;
  // end OmnetDataOutput::OmnetOutputCallback::OutputSingleton
}
void
OmnetDataOutput::OmnetOutputCallback::OutputSingleton(std::string key,
                                                      std::string variable,
                                                      uint32_t val)
{
  (*m_scalar) << "scalar " << key << " " << variable << " " << val << std::endl;
  // end OmnetDataOutput::OmnetOutputCallback::OutputSingleton
}
void
OmnetDataOutput::OmnetOutputCallback::OutputSingleton(std::string key,
                                                      std::string variable,
                                                      double val)
{
  (*m_scalar) << "scalar " << key << " " << variable << " " << val << std::endl;
  // end OmnetDataOutput::OmnetOutputCallback::OutputSingleton
}
void
OmnetDataOutput::OmnetOutputCallback::OutputSingleton(std::string key,
                                                      std::string variable,
                                                      std::string val)
{
  (*m_scalar) << "scalar " << key << " " << variable << " " << val << std::endl;
  // end OmnetDataOutput::OmnetOutputCallback::OutputSingleton
}
void
OmnetDataOutput::OmnetOutputCallback::OutputSingleton(std::string key,
                                                      std::string variable,
                                                      Time val)
{
  (*m_scalar) << "scalar " << key << " " << variable << " " << val << std::endl;
  // end OmnetDataOutput::OmnetOutputCallback::OutputSingleton
}
