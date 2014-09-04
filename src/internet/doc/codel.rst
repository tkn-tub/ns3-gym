CoDel queue implementation in |ns3|
-----------------------------------

This chapter describes CoDel queue implementation in |ns3|. 

Developed by Kathleen Nichols and Van Jacobson as a solution to bufferbloat,
CoDel (Controlled Delay Management) is a queuing discipline that uses packet's 
sojourn time (time in queue) to make decisions on packet drops [1]. 


Model Description
*****************

The source code for CoDel model lives in the directory ``src/internet/model``
that consists of 2 files `codel-queue.h` and `codel-queue.cc` defining a CoDelQueue
class and a helper CoDelTimestampTag class. The code was ported to |ns3| based 
on Linux kernel code implemented by Dave Taht and Eric Dumazet. 

* class :cpp:class:`CoDelQueue`: This class implements the main CoDel algorithm:

** CoDelQueue::DoEnqueue (): This routine tags a packet with the current time before pushing it into the queue.
The timestamp tag is used by CoDelQueue::DoDequeue() to compute the packet's sojourn time. 
If the queue is full upon the packet arrival, this routine will drop the packet and record 
the number of drops due to queue overflow, which is stored in `m_dropOverLimit`.
 
** CoDelQueue::ShouldDrop (): This routine is CoDelQueue::DoDequeue()'s helper routine that determines 
whether a packet should be dropped or not based on its sojourn time. 
If the sojourn time goes above `m_target` and remains above continuously for at least `m_interval`, 
the routine returns TRUE indicating that it is OK to drop the packet. Otherwise, it returns FALSE. 

** CoDelQueue::DoDequeue (): This routine performs the actual packet drop based on 
CoDelQueue::ShouldDrop ()'s return value and schedule the next drop. There are 2 branches: 
*** If we are currently in the dropping state, which means the sojourn time 
has remained above `m_target` for more than `m_interval`, the routine determines if it's OK to leave the dropping state 
or it's time for the next drop. When CoDelQueue::ShouldDrop () returns FALSE, 
we can drop out of the dropping state (set `m_dropping` to FALSE). 
Otherwise, we continuously drop packets and update the time for next drop (`m_dropNext`) until one of the following conditions is met: 
(1) The queue is empty upon which we leave the dropping state and exit CoDelQueue::ShouldDrop () routine; 
(2) CoDelQueue::ShouldDrop () returns FALSE (meaning the sojourn time goes below `m_target`) upon which we leave the dropping state; 
(3) it's not time for next drop yet (`m_dropNext` is less than current time) upon which we wait for the next packet dequeue to check the condition again. 
*** If we are not in the dropping state, the routine enters the dropping state and drop the first packet
if CoDelQueue::ShouldDrop () returns TRUE (meaning the sojourn time has gone above `m_target` 
for at least `m_interval` for the first time or it has gone above again after we leave the dropping state). 


* class :cpp:class:`CoDelTimestampTag`: This class implements the timestamp tagging for a packet. 
This tag is used to compute the packet's sojourn time 
(the difference between the time the packet is dequeued and the time it is pused into the queue). 

References
==========

[1] Controlled Delay Active Queue Management [http://tools.ietf.org/html/draft-nichols-tsvwg-codel-02]
[2] Controlling Queue Delay [http://queue.acm.org/detail.cfm?id=2209336]
[3] Bloat >> Cerowrt [http://www.bufferbloat.net/projects/cerowrt]
[4] Traditional AQM is not enough [http://gettys.wordpress.com/category/bufferbloat/]

Attributes
==========

The key attributes that the CoDelQueue class holds include the following: 
* Mode: CoDel operating mode (BYTES, PACKETS, or ILLEGAL). The default mode is BYTES. 
* MaxPackets: The maximum number of packets the queue can hold. The default value is DEFAULT_CODEL_LIMIT, which is 1000 packets.
* MaxBytes: The maximum number of bytes the queue can hold. The default value is 1500 * DEFAULT_CODEL_LIMIT, which is 1500 * 1000 bytes. 
* MinBytes: The CoDel algorithm minbytes parameter. The default value is 1500 bytes. 
* Interval: The sliding-minimum window. The default value is 100 ms. 
* Target: The CoDel algorithm target queue delay. The default value is 5 ms. 

Examples
========

* First example: codel-vs-droptail-basic-test.cc located in src/internet/examples
** Run the file:
   $ ./waf --run "codel-vs-droptail-basic-test --PrintHelp"
   $ ./waf --run "codel-vs-droptail-basic-test --queueType=CoDel --pcapFileName=codel.pcap --cwndTrFileName=cwndCodel.tr" 
** Expected output from the previous commands: codel.pcap file and cwndCoDel.tr are produced
** The .pcap file can be analyzed using wireshark or tcptrace
   $ tcptrace -l -r -n -W codel.pcap

* Second example: codel-vs-droptail-asymmetric.cc located in src/internet/examples
** Run the file:
   $ ./waf --run "codel-vs-droptail-asymmetric --PrintHelp"
   $ ./waf --run codel-vs-droptail-asymmetric
** Expected output from the previous commands:
*** 6 .pcap files:
**** codel-vs-droptail-asymmetric-CoDel-server-lan.pcap
**** codel-vs-droptail-asymmetric-CoDel-router-wan.pcap
**** codel-vs-droptail-asymmetric-CoDel-router-lan.pcap
**** codel-vs-droptail-asymmetric-CoDel-cmts-wan.pcap
**** codel-vs-droptail-asymmetric-CoDel-cmts-lan.pcap
**** codel-vs-droptail-asymmetric-CoDel-host-lan.pcap
*** 1 .attr file:
**** codel-vs-droptail-asymmetric-CoDel.attr 
*** 5 .tr files: 
**** codel-vs-droptail-asymmetric-CoDel-drop.tr
**** codel-vs-droptail-asymmetric-CoDel-drop-state.tr
**** codel-vs-droptail-asymmetric-CoDel-sojourn.tr
**** codel-vs-droptail-asymmetric-CoDel-length.tr
**** codel-vs-droptail-asymmetric-CoDel-cwnd.tr 

Validation
**********

The CoDel model is tested using CoDelQueueTestSuite class defined in ``src/internet/test/codel-queue-test-suite.cc``.
The suite includes 5 test cases:
* Test 1: The first test checks the enqueue/dequeue with no drops and makes sure that CoDel attributes can be set correctly.
* Test 2: The second test checks the enqueue with drops due to queue overflow.
* Test 3: The third test checks the NewtonStep() arithmetic against explicit port of Linux implementation
* Test 4: The fourth test checks the ControlLaw() against explicit port of Linux implementation
* Test 5: The fifth test checks the enqueue/dequeue with drops according to CoDel algorithm

The test suite can be run using the following commands: 
  $ ./waf configure --enable-examples --enable-tests
  $ ./waf build
  $ ./test.py -s codel-queue
or  
  $ NS_LOG="CoDelQueue" ./waf --run "test-runner --suite=codel-queue"

