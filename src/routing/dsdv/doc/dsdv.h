/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Hemanth Narra, Yufei Cheng
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
 * Author: Hemanth Narra <hemanth@ittc.ku.com>
 * Author: Yufei Cheng   <yfcheng@ittc.ku.edu>
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  http://wiki.ittc.ku.edu/resilinets
 * Information and Telecommunication Technology Center (ITTC)
 * and Department of Electrical Engineering and Computer Science
 * The University of Kansas Lawrence, KS USA.
 *
 * Work supported in part by NSF FIND (Future Internet Design) Program
 * under grant CNS-0626918 (Postmodern Internet Architecture),
 * NSF grant CNS-1050226 (Multilayer Network Resilience Analysis and Experimentation on GENI),
 * US Department of Defense (DoD), and ITTC at The University of Kansas.
 */
#ifndef DSDV_H_
#define DSDV_H_

/**
 * \ingroup routing
 * \defgroup dsdv
 * 
 * \brief Destination-Sequenced Distance Vector (DSDV) routing protocol is a pro-active, table-driven routing protocol
 *  for MANETs developed by Charles E. Perkins and Pravin Bhagwat in 1994. It uses the hop count as metric in route
 *  selection.
 *
 *  DSDV Routing Table: Every node will maintain a table listing all the other nodes it has known either directly
 *  or through some neighbors. Every node has a single entry in the routing table. The entry will have information
 *  about the node's IP address, last known sequence number and the hop count to reach that node. Along with these
 *  details the table also keeps track of the nexthop neighbor to reach the destination node, the timestamp of the last
 *  update received for that node.
 *
 *  The DSDV update message consists of three fields, Destination Address, Sequence Number and Hop Count.
 *
 *  Each node uses 2 mechanisms to send out the DSDV updates. They are,
 *  1. Periodic Updates
 *      Periodic updates are sent out after every m_periodicUpdateInterval(default:15s). In this update the node broadcasts
 *      out its entire routing table.
 *  2. Trigger Updates
 *      Trigger Updates are small updates in-between the periodic updates. These updates are sent out whenever a node
 *      receives a DSDV packet that caused a change in its routing table. The original paper did not clearly mention
 *      when for what change in the table should a DSDV update be sent out. The current implemntation sends out an update
 *      irrespective of the change in the routing table.
 *
 *  The updates are accepted based on the metric for a particular node. The first factor determinig the acceptance of
 *  an update is the sequence number. It has to accept the update if the sequence number of the update message is higher
 *  irrespective of the metric. If the update with same sequence number is received, then the update with least metric
 *  (hopCount) is given precedence.
 *
 *  In highly mobile scenarios, there is a high chance of route fluctuations, thus we have the concept of weighted
 *  settling time where an update with change in metric will not be advertised to neighbors. The node waits for
 *  the settling time to make sure that it did not receive the update from its old neighbor before sending out
 *  that update.
 *
 *  The current implementation covers all the above features of DSDV. The current implementation also has a request queue
 *  to buffer packets that have no routes to destination. The default is set to buffer up to 5 packets per destination.
 *
 *  Link to the Paper: http://portal.acm.org/citation.cfm?doid=190314.190336
 *
 */

#endif /* DSDV_H_ */
