/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 The Boeing Company
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
 */

#include "ns3/core-module.h"
#include "ns3/lr-wpan-module.h"

#include <iostream>

using namespace ns3;

int main (int argc, char *argv[])
{
  Packet::EnablePrinting ();
  Packet::EnableChecking ();
  LrWpanMacHeader macHdr (LrWpanMacHeader::LRWPAN_MAC_BEACON, 0);        //sequence number set to 0
  macHdr.SetSrcAddrMode (2);                                             // short addr
  // ... other setters


  uint16_t shortAddr = 66;
  //uint64_t extAddr = 77;
  uint16_t srcPanId = 100;
  WpanAddress srcWpanAddr;
  /*uint16_t dstPanId = 200;
  WpanAddress dstWpanAddr;*/

  srcWpanAddr.SetAddress (shortAddr);
  //srcWpanAddr.SetAddress(extAddr);
  macHdr.SetSrcAddrFields (srcPanId,srcWpanAddr );
  /*dstWpanAddr.SetAddress(extAddr);
  macHdr.SetDstAddrFields(dstPanId,dstWpanAddr );*/

  LrWpanMacTrailer macTrailer;

  Ptr<Packet> p = Create<Packet> (20);  // 20 bytes of dummy data

  p->AddHeader (macHdr);
  p->AddTrailer (macTrailer);

  p->Print (std::cout);
  std::cout << std::endl;
  return 0;

}
