/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INESC Porto
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
 * Author: Gustavo J. A. M. Carneiro  <gjc@inescporto.pt>
 */

#ifndef OLSR_H
#define OLSR_H

/**
 * \ingroup routing
 * \defgroup olsr OLSR
 *
 * \section model Model
 *
 * This model implements the base specification of the Optimized
 * Link State Routing (OLSR) protocol, which is a dynamic mobile ad hoc
 * unicast routing protocol.  It has been developed at the 
 * University of Murcia (Spain) by Francisco J. Ros for NS-2, and was
 * ported to NS-3 by Gustavo Carneiro at INESC Porto (Portugal).
 *
 * Here is a summary of software's main features:
 * - Mostly compliant with OLSR as documented in RFC 3626 (http://www.ietf.org/rfc/rfc3626.txt), with the following differences:
 *  - The use of multiple interfaces was not supported by the NS-2 version, but is supported in NS-3;
 *  - Unlike the NS-2 version, does not yet support MAC layer feedback as described in RFC 3626;
 *  - HNA (Host/Network Association) messages are almost-but-not-quite supported in this version.
 *
 * \section api API and Usage
 * 
 * A helper class for OLSR has been written.  After an IPv4 topology
 * has been created and unique IP addresses assigned to each node, the
 * simulation script writer can call one of three overloaded functions
 * with different scope to enable OLSR: ns3::OlsrHelper::Install
 * (NodeContainer container); ns3::OlsrHelper::Install (Ptr<Node>
 * node); or ns3::OlsrHelper::InstallAll (void);
 *
 * In addition, the behavior of OLSR can be modified by changing certain
 * attributes.  The method ns3::OlsrHelper::SetAgent () can be used
 * to set OLSR attributes.  These include HelloInterval, TcInterval,
 * MidInterval, Willingness.  Other parameters are defined as macros
 * in olsr-agent-impl.cc.
 */


#endif /* OLSR_H */
