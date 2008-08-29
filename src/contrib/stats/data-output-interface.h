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

#ifndef __DATA_OUTPUT_INTERFACE_H__
#define __DATA_OUTPUT_INTERFACE_H__

#include "ns3/object.h"
#include "ns3/nstime.h"

namespace ns3 {

  class DataCollector;

  //------------------------------------------------------------
  //--------------------------------------------
  class DataOutputInterface : public Object {
  public:
    DataOutputInterface();
    virtual ~DataOutputInterface();

    virtual void Output(DataCollector &dc) = 0;

  protected:
    virtual void DoDispose();

    // end class DataOutputInterface
  };

  class DataOutputCallback {
  public:
    virtual ~DataOutputCallback() {}

    virtual void OutputSingleton(std::string key,
                                 std::string variable,
                                 int val) = 0;

    virtual void OutputSingleton(std::string key,
                                 std::string variable,
                                 uint32_t val) = 0;

    virtual void OutputSingleton(std::string key,
                                 std::string variable,
                                 double val) = 0;

    virtual void OutputSingleton(std::string key,
                                 std::string variable,
                                 std::string val) = 0;

    virtual void OutputSingleton(std::string key,
                                 std::string variable,
                                 Time val) = 0;
    // end class DataOutputCallback
  };

  // end namespace ns3
};


#endif // __DATA_OUTPUT_INTERFACE_H__
