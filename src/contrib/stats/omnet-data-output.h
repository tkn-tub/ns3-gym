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

#ifndef __OMNET_DATA_OUTPUT_H__
#define __OMNET_DATA_OUTPUT_H__

#include "ns3/nstime.h"

#include "data-output-interface.h"

namespace ns3 {


  //------------------------------------------------------------
  //--------------------------------------------
  class OmnetDataOutput : public DataOutputInterface {
  public:
    OmnetDataOutput();
    virtual ~OmnetDataOutput();

    virtual void Output(DataCollector &dc);

    void SetFilePrefix(const std::string prefix);
    std::string GetFilePrefix() const;

  protected:
    virtual void DoDispose();

  private:
    class OmnetOutputCallback : public DataOutputCallback {
    public:
      OmnetOutputCallback(std::ostream *scalar);

      void OutputSingleton(std::string key,
                           std::string variable,
                           int val);

      void OutputSingleton(std::string key,
                           std::string variable,
                           uint32_t val);

      void OutputSingleton(std::string key,
                           std::string variable,
                           double val);

      void OutputSingleton(std::string key,
                           std::string variable,
                           std::string val);

      void OutputSingleton(std::string key,
                           std::string variable,
                           Time val);

    private:
      std::ostream *m_scalar;
      // end class OmnetOutputCallback
    };

    std::string m_filePrefix;
    // end class OmnetDataOutput
  };

  // end namespace ns3
};


#endif // __OMNET_DATA_OUTPUT_H__
