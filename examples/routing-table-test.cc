/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */


#include <iostream>
#include <fstream>
#include "ns3/core-module.h"
#include <vector>
#include "ns3/mac48-address.h"
#include "ns3/simulator-module.h"
#include <string.h>
#include "ns3/hwmp-rtable.h"



using namespace ns3;

NS_LOG_COMPONENT_DEFINE("RoutingTableTest");


void
aux_function(Ptr<HwmpRtable> ptr,Mac48Address addr,std::vector<bool> *results, std::vector<Time> *seconds)
{
	if (ptr->LookupReactive(addr).retransmitter.IsBroadcast() && ptr->LookupReactive(addr).metric == 0xffffffff)
	results->push_back(false);
	else results->push_back(true);
	seconds->push_back(Simulator::Now());
	NS_LOG_UNCOND("Reactive retransmitter is "<<ptr->LookupReactive(addr).retransmitter << "\nport is "<<ptr->LookupReactive(addr).ifIndex << "\nmetric is " << ptr->LookupReactive(addr).metric << "\nseqnum is " << ptr->LookupReactive(addr).seqnum);
	NS_LOG_UNCOND("Checked at "<<Simulator::Now().GetSeconds()<<" seconds\n\n");
};


void
aux_function1(Ptr<HwmpRtable> ptr,uint32_t port,std::vector<bool> *results,std::vector<Time> *seconds)
{
	if (ptr->LookupProactive(port).retransmitter.IsBroadcast() && ptr->LookupProactive(port).metric == 0xffffffff)
	results->push_back(false);
	else results->push_back(true);
	seconds->push_back(Simulator::Now());
	NS_LOG_UNCOND("Proactive retransmitter is "<<ptr->LookupProactive(port).retransmitter<< "\nport is "<<ptr->LookupProactive(port).ifIndex << "\nmetric is " << ptr->LookupProactive(port).metric << "\nseqnum is " << ptr->LookupProactive(port).seqnum);
	NS_LOG_UNCOND("Checked at "<<Simulator::Now().GetSeconds()<<" seconds\n\n");
};

void
print_results(std::vector<bool> *results,std::vector<Time> *seconds)
{
NS_LOG_UNCOND("Fulfilled "<<results->size()<<" tests\n");
for (unsigned int i = 0;i!=results->size();i++)
	{
	NS_LOG_UNCOND("Result: "<<results->at(i));
	NS_LOG_UNCOND("Tested at "<<seconds->at(i).GetSeconds()<<" seconds\n");
	}
}

void 
get_conclusion(std::vector<bool> *results)
{
	if (
		results->at(0) 
		&& results->at(1) 
		&& results->at(2)
		&& results->at(3)
		&& results->at(4)
		&& results->at(5)
		&& results->at(6)
		&& results->at(7)
		&& !results->at(8)
		&& !results->at(9)
		&& !results->at(10)
		&& !results->at(11)
		&& !results->at(12)
		&& !results->at(13)
		&& !results->at(14))
		NS_LOG_UNCOND("\n\n\nPASSED\n\n\n");
	else		
		NS_LOG_UNCOND("\n\n\nNOT PASSED\n\n\n");

}

int main(int argc,char *argv[])
{	
	// Create Table
	Ptr<HwmpRtable> table = CreateObject<HwmpRtable>();
	std::vector<bool>  test_results = std::vector<bool>();
	std::vector<Time>  test_time = std::vector<Time>();
	//Add reactive paths
	table->AddReactivePath(Mac48Address("01:00:00:01:00:01"),Mac48Address("01:00:00:01:00:03"),8010,10,Seconds(20),1);
	table->AddReactivePath(Mac48Address("01:00:00:02:00:01"),Mac48Address("01:00:00:02:00:03"),8020,20,Seconds(30),2);
//	table->AddReactivePath(Mac48Address("01:00:00:03:00:01"),Mac48Address("01:00:00:03:00:03"),8030,30,Seconds(30),3);
//	table->AddReactivePath(Mac48Address("01:00:00:04:00:01"),Mac48Address("01:00:00:04:00:03"),8040,40,Seconds(35),4);
	//Add proactive paths
	table->AddProactivePath(10,Mac48Address("01:00:00:01:00:01"),Mac48Address("01:00:00:01:00:03"),8010,Seconds(20),1);
	table->AddProactivePath(20,Mac48Address("01:00:00:02:00:01"),Mac48Address("01:00:00:02:00:03"),8020,Seconds(30),2);
	table->AddProactivePath(30,Mac48Address("01:00:00:03:00:01"),Mac48Address("01:00:00:03:00:03"),8030,Seconds(20),3);
	table->AddProactivePath(40,Mac48Address("01:00:00:04:00:01"),Mac48Address("01:00:00:04:00:03"),8040,Seconds(30),4);
	Simulator::Schedule(Seconds(0.0),
			    &aux_function,
			    table,
			    Mac48Address("01:00:00:01:00:01"),&test_results,&test_time);
	Simulator::Schedule(Seconds(0.0),
			    &aux_function1,
			    table,
			    8010,&test_results,&test_time);
	Simulator::Schedule(Seconds(0.0),
			    &aux_function,
			    table,
			    Mac48Address("01:00:00:02:00:01"),&test_results,&test_time);
	Simulator::Schedule(Seconds(0.0),
			    &aux_function1,
			    table,
			    8020,&test_results,&test_time);
	Simulator::Schedule(Seconds(1.0),
			    &aux_function,
			    table,
			    Mac48Address("01:00:00:01:00:01"),&test_results,&test_time);
	Simulator::Schedule(Seconds(1.0),
			    &aux_function1,
			    table,
			    8010,&test_results,&test_time);
	Simulator::Schedule(Seconds(1.0),
			    &aux_function,
			    table,
			    Mac48Address("01:00:00:02:00:01"),&test_results,&test_time);
	Simulator::Schedule(Seconds(1.0),
			    &aux_function1,
			    table,
			    8020,&test_results,&test_time);
	//Print routing info
	NS_LOG_UNCOND("Added reactive path number "<<table->LookupReactive(Mac48Address("01:00:00:01:00:01")).seqnum << " at dest " << table->LookupReactive(Mac48Address("01:00:00:01:00:01")).retransmitter << " with metric "<< table->LookupReactive(Mac48Address("01:00:00:01:00:01")).metric <<"\n");
	NS_LOG_UNCOND("Added reactive path number "<<table->LookupReactive(Mac48Address("01:00:00:02:00:01")).seqnum << " at dest " << table->LookupReactive(Mac48Address("01:00:00:02:00:01")).retransmitter << " with metric "<< table->LookupReactive(Mac48Address("01:00:00:02:00:01")).metric <<"\n");
	NS_LOG_UNCOND("Added reactive path number "<<table->LookupReactive(Mac48Address("01:00:00:03:00:01")).seqnum << " at dest " << table->LookupReactive(Mac48Address("01:00:00:03:00:01")).retransmitter << " with metric "<< table->LookupReactive(Mac48Address("01:00:00:03:00:01")).metric <<"\n");
	NS_LOG_UNCOND("Added reactive path number "<<table->LookupReactive(Mac48Address("01:00:00:04:00:01")).seqnum << " at dest " << table->LookupReactive(Mac48Address("01:00:00:04:00:01")).retransmitter << " with metric "<< table->LookupReactive(Mac48Address("01:00:00:04:00:01")).metric <<"\n");

	NS_LOG_UNCOND("Added proactive path number " << table->LookupProactive(8010).seqnum <<" with retr "<< table->LookupProactive(8010).retransmitter<< " at port " <<table->LookupProactive(8010).ifIndex<< " with metric " <<table->LookupProactive(8010).metric<<"\n");
	NS_LOG_UNCOND("Added proactive path number " << table->LookupProactive(8020).seqnum <<" with retr "<< table->LookupProactive(8020).retransmitter<< " at port " <<table->LookupProactive(8020).ifIndex<< " with metric " <<table->LookupProactive(8020).metric<<"\n");
	NS_LOG_UNCOND("Added proactive path number " << table->LookupProactive(8030).seqnum <<" with retr "<< table->LookupProactive(8030).retransmitter<< " at port " <<table->LookupProactive(8030).ifIndex<< " with metric " <<table->LookupProactive(8030).metric<<"\n");
	NS_LOG_UNCOND("Added proactive path number " << table->LookupProactive(8040).seqnum <<" with retr "<< table->LookupProactive(8040).retransmitter<< " at port " <<table->LookupProactive(8040).ifIndex<< " with metric " <<table->LookupProactive(8040).metric<<"\n");

	table->DeleteProactivePath(8030);
	table->DeleteProactivePath(Mac48Address("01:00:00:04:00:01"),8040);
	Simulator::Schedule(Seconds(1.0),
			    &aux_function1,
			    table,
			    8030,&test_results,&test_time);
	Simulator::Schedule(Seconds(1.0),
			    &aux_function1,
			    table,
			    8040,&test_results,&test_time);

	Simulator::Schedule(Seconds(10.0),
			    &HwmpRtable::DeleteReactivePath,
			    table,
			    Mac48Address("01:00:00:02:00:01"),8020);

	Simulator::Schedule(Seconds(15.0),
			    &aux_function,
			    table,
			    Mac48Address("01:00:00:02:00:01"),&test_results,&test_time);
        Simulator::Schedule(Seconds(25.0),
			    &aux_function,
			    table,
			    Mac48Address("01:00:00:01:00:01"),&test_results,&test_time);
	Simulator::Schedule(Seconds(25.0),
			    &aux_function,
			    table,
			    Mac48Address("01:00:00:02:00:01"),&test_results,&test_time);
	Simulator::Schedule(Seconds(25.0),
			    &aux_function1,
			    table,
			    8010,&test_results,&test_time);
	Simulator::Schedule(Seconds(30.0),
			    &aux_function,
			    table,
			    Mac48Address("01:00:00:01:00:01"),&test_results,&test_time);
	
	Simulator::Schedule(Seconds(40.0),
			    &print_results,
                            &test_results,&test_time);
	Simulator::Schedule(Seconds(41.0),
			    &get_conclusion,
		            &test_results);
	
	Simulator::Run();
	Simulator::Destroy();
	return 0;
}

