/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Lalith Suresh 
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
 * Authors: Lalith Suresh <suresh.lalith@gmail.com>
 */

/**
* \ingroup routing
* \defgroup click Click
*
* \section model Model
*
* This model implements the interface to the Click Modular Router and
* provides the Ipv4ClickRouting class to allow a node to use Click 
* for external routing. Unlike normal Ipv4RoutingProtocol sub types,
* Ipv4ClickRouting doesn't use a RouteInput() method, but instead, 
* receives a packet on the appropriate interface and processes it 
* accordingly. Note that you need to have a routing table type element 
* in your Click graph to use Click for external routing. This is needed
* by the RouteOutput() function inherited from Ipv4RoutingProtocol.
* Furthermore, a Click based node uses a different kind of L3 in the
* form of Ipv4L3ClickProtocol, which is a trimmed down version of
* Ipv4L3Protocol. Ipv4L3ClickProtocol passes on packets passing through
* the stack to Ipv4ClickRouting for processing.
*
* \section build Build Instructions
*
* The first step is to build Click. At the top of your Click source directory:
*
*   $: ./configure --enable-userlevel --disable-linuxmodule --enable-nsclick --enable-wifi
*   $: make
* 
* The --enable wifi flag can be skipped in case you don't intend on using
* Click with Wifi.
* *Note: You don't need to do a 'make install'.
*
* Once Click has been built successfully, we proceed to configure ns-3 with 
* Click Integration support:
*
*   $: ./waf configure --with-nsclick=/path/to/click/source
*
* If it says 'enabled' beside 'NS-3 Click Integration Support', then you're 
* good to go.
*
* Next, try running one of the examples:
*
*   $: ./waf --run nsclick-simple-lan
*
* You will find a lot of output being generated. This is because of the 
* IPPrint and Print elements present in the nsclick-simple-lan.click 
* configuration file that the example script uses. 
*
* \section clickgraph Click Graph Instructions
*
* The following should be kept in mind when making your Click graph:
*   - Only userlevel elements can be used.
*   - You will need to replace FromDevice and ToDevice elements 
*     with FromSimDevice and ToSimDevice elements.
*   - Packets to the kernel are sent up using ToSimDevice(tap0,IP).
*   - For any node, the 0th device will be tap0. The remaining devices 
*     should be eth0, eth1 and so forth (even if you're using wifi). 
*     Please note that the device numbering should begin from 0.
*   - A routing table element is a mandatory. The OUTports of the routing 
*     table element should correspond to the interface number of the device 
*     through which the packet will ultimately be sent out. Violating this 
*     rule will lead to really weird packet traces. This routing table element's 
*     name should then be passed to the Ipv4ClickRouting protocol object as 
*     a simulation parameter. See the Click examples for details.
*   - When using Wifi with ns-3-click, do not use wifi specific elements like 
*     WifiEncap, ExtraEncap, MadWifiRate etc. for outgoing packets. Incoming 
*     packets should have their Wifi headers removed using WifiDecap + ExtraDecap 
*     or Strip elements. See the nsclick-raw-wlan.click file for an idea of the same.
*   - The current implementation leaves Click with mainly L3 functionality, 
*     with ns-3 handling L2. We will soon begin working to support the use of 
*     MAC protocols on Click as well.
*
* \section usage Usage
*
* To have a node run Click, the easiest way would be to use the ClickInternetStackHelper 
* class in your simulation script. For instance:
*
*   ClickInternetStackHelper click;
*   click.SetClickFile (myNodeContainer, "nsclick-simple-lan.click");
*   click.SetRoutingTableElement (myNodeContainer, "u/rt");
*   click.Install (myNodeContainer);
* 
* The example scripts inside examples/click/ demonstrate the use of Click based 
* nodes in different scenarios.
*
*
*/

