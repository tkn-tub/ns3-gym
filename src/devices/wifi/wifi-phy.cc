/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "wifi-phy.h"
#include "wifi-mode.h"
#include "wifi-channel.h"
#include "wifi-preamble.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/random-variable.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/enum.h"
#include "ns3/trace-source-accessor.h"
#include <math.h>

NS_LOG_COMPONENT_DEFINE ("WifiPhy");

namespace ns3 {

/****************************************************************
 *       This destructor is needed.
 ****************************************************************/

WifiPhyListener::~WifiPhyListener ()
{}

/****************************************************************
 *       The actual WifiPhy class
 ****************************************************************/

NS_OBJECT_ENSURE_REGISTERED (WifiPhy);

TypeId 
WifiPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WifiPhy")
    .SetParent<Object> ()
    ;
  return tid;
}

WifiPhy::WifiPhy ()
{
  NS_LOG_FUNCTION (this);
}

WifiPhy::~WifiPhy ()
{
  NS_LOG_FUNCTION (this);
}

WifiMode 
WifiPhy::Get6mba (void)
{
  static WifiMode mode = WifiModeFactory::CreateBpsk ("wifia-6mbs",
                                                      true,
                                                      20000000, 6000000, 12000000);
  return mode;
}
WifiMode 
WifiPhy::Get9mba (void)
{
  static WifiMode mode = WifiModeFactory::CreateBpsk ("wifia-9mbs",
                                               false,
                                               20000000, 9000000, 12000000);
  return mode;
}
WifiMode 
WifiPhy::Get12mba (void)
{
  static WifiMode mode = WifiModeFactory::CreateBpsk ("wifia-12mbs",
                                                      true,
                                                      20000000, 12000000, 24000000);
  return mode;
}
WifiMode 
WifiPhy::Get18mba (void)
{
  static WifiMode mode = WifiModeFactory::CreateBpsk ("wifia-18mbs",
                                                      false,
                                                      20000000, 18000000, 24000000);
  return mode;
}
WifiMode 
WifiPhy::Get24mba (void)
{
  static WifiMode mode = WifiModeFactory::CreateBpsk ("wifia-24mbs",
                                                      true,
                                                      20000000, 24000000, 48000000);
  return mode;
}
WifiMode 
WifiPhy::Get36mba (void)
{
  static WifiMode mode = WifiModeFactory::CreateBpsk ("wifia-36mbs",
                                                      false,
                                                      20000000, 36000000, 48000000);
  return mode;
}

WifiMode 
WifiPhy::Get48mba (void)
{
  static WifiMode mode = WifiModeFactory::CreateBpsk ("wifia-48mbs",
                                                      false,
                                                      20000000, 48000000, 72000000);
  return mode;
}

WifiMode 
WifiPhy::Get54mba (void)
{
  static WifiMode mode = WifiModeFactory::CreateBpsk ("wifia-54mbs",
                                                      false,
                                                      20000000, 54000000, 72000000);
  return mode;
}

} // namespace ns3
