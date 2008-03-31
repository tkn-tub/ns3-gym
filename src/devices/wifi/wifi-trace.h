/* -*-	Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
 * Author: Federico Maguolo <maguolof@dei.unipd.it>
 */
#ifndef WIFI_TRACE_H
#define WIFI_TRACE_H

#include <string>
#include <fstream>
#include "ns3/wifi-phy.h"
#include "ns3/mac48-address.h"
//#include "ns3/ptr.h"

namespace ns3 {

class Packet;
class TraceContext;

class WifiTrace 
{
public:
  WifiTrace (std::string filename);
  WifiTrace ();
  virtual ~WifiTrace ();
  void TraceAllNetDeviceRx (void);
  void TraceAllNetDeviceTx (void);
  void TraceAllPhy (void);
  void TraceAllErrors (void);
  void TraceAckTimeouts (void);
  void TraceCtsTimeouts (void);
protected:
  virtual void LogErrors (TraceContext const &context, Ptr<const Packet> p);
  virtual void LogDevRx (TraceContext const &context, Ptr<const Packet> p, Mac48Address addr);
  virtual void LogDevTx (TraceContext const &context, Ptr<const Packet> p, Mac48Address addr);
  virtual void LogPhy (TraceContext const &context, Time s, Time d, WifiPhy::State state);
  virtual void LogAckTimeout (TraceContext const &context, uint32_t a);
  virtual void LogCtsTimeout (TraceContext const &context, uint32_t a);
private:
  std::ofstream m_os;
};

}//namespace ns3

#endif /* ASCII_TRACE_H */
