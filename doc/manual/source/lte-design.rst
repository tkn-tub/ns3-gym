.. include:: replace.txt


++++++++++++++++++++++++++
 LTE Design Documentation
++++++++++++++++++++++++++


An overview of the LTE module
*****************************


Design Criteria
~~~~~~~~~~~~~~~

The LTE module has the long term objective of supporting the evaluation of the following aspects of LTE systems:

* LTE Radio Resource Management
* QoS-aware Packet Scheduling
* Call Admission Control
* Inter-cell Interference Coordination
* Load Balancing 
* Mobility 


In order to model LTE systems to a level of detail that is sufficient to allow a correct evaluation of the above mentioned aspects, the following assumptions have been made:

#. At the radio level, the granularity of the model should be at least that of the Resource Block. In fact, this is the fundamental unit being used for resource allocation. Without this minimum level of granularity, it is not possible to model accurately, for example, packet scheduling and inter-cell-interference solutions. Note that this design choice rules out system level simulator, which work at the granularity of call / bearer establishment.
#. The simulator should scale up to tens of eNBs and hundreds of UEs. This rules out the use of a link level simulator, i.e., a simulator whose radio interface is modeled with a granularity up to the symbol level. This is becaise a symbol layer model needs to implement all the PHY layer signal processing, whose huge complexity severely limits scalability in terms of number of eNBs and UEs. In fact, link-level simulators are normally limited to a single eNB and one or a few UEs.
#. MAC-level KPIs (e.g., per-UE and per-bearer throughput, delay and loss rate measured at the RLC PDU level) are probably the most straightforward KPIs that can be extracted by the simulator. Still, these KPIs cannot be mapped directly to the Quality of Experience (QoE) perceived by the end user, at least without some gross approximation. Hence, to allow a proper evaluation of the QoE, the LTE user plane protocol stack should be modeled accurately. In particular, the RLC protocol should be modeled: in fact, since the RLC takes care of fragmentation, concatenation & fragment retransmission of user-plane IP packets, it  
#. While it is acceptable to idealize some aspects of the control plane for ease of simulations, there are some other aspects that need to be modeled in order to obtain accurate simulation results. For example, control signaling consumes radio resources and can be the cause of a limitation in the user-perceived performance; furthermore, the correct or erroneous reception of the control signalling affects important aspects of LTE such as cell selection by the UEs and the neighbor relation function of the eNBs.


Module Architecture
~~~~~~~~~~~~~~~~~~~


The overall architecture of the LTE module is represented in the following figures.

.. figure:: figures/lte-enb-architecture.png
   :align: right

   The architecture of the LTE eNB

.. figure:: figures/lte-ue-architecture.png
   :align: right

   The architecture of the LTE UE



Detailed description of the components
**************************************

LTE Spectrum Model
~~~~~~~~~~~~~~~~~~

Based on the Spectrum




Radio Resource Management and Packet Scheduling
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For how the ns-3 LTE module handles Radio Resource Management and Packet Scheduling at the eNB, please see the :ref:`ff-mac-sched-api`.


Physical layer
~~~~~~~~~~~~~~

The `ns3::LtePhy` class models the LTE PHY layer. 

Basic functionalities of the PHY layer are: (i) transmit packets coming from the device to the channel; (ii) receive packets from the channel; (ii) evaluate the quality of the channel from the Signal To Noise ratio of the received signal; and (iii) forward received packets to the device.

Both the PHY and channel have been developed extending `ns3::SpectrumPhy` and 
`ns3::SpectrumChannel` classes, respectively, which are provided by the Spectrum Framework [1]_.
 
The module implements an FDD channel access. In FDD channel access, downlink and uplink 
transmissions work together in the time but using a different set of frequencies.  
Since DL and UL are indipendent between them, the PHY is composed by couple of 
`ns3::LteSpectrumPhy` object, one for the downlink and one for the uplink.
The `ns3::LtePhy` stores and manages both downlink and uplink 
`ns3::LteSpectrumPhy` elements. 

In order to customize all physical functionalities for both UE and eNB devices, dedicated 
classes have been inherited from ones described before. In particular, 
`ns3::LteUePhy` and `ns3::LteEnbPhy` classes, inherited from 
the `ns3::LtePhy` class, implement the PHY layer for the UE and the 
eNB, respectively. 

The figure below shows how UE and eNB can exchange packets through the considered PHY layer.


For the downlink, when the eNB whants to send packets, it calls the ``StartTx`` function to 
send them into the downlink channel. Then, the downlink channel delivers the burst 
of packets to all the `ns3::UeLteSpectrumPhy` attached to it, handling the 
``StartRx`` function. 
When the UE receives packets, it executes the following tasks:

* compute the SINR for all the sub channel used in the downlink

* create and send CQI feedbacks

* forward all the received packets to the MAC layer through the PHY SAP.

The uplink works similary.

Propagation Loss Models
~~~~~~~~~~~~~~~~~~~~~~~

A proper propagation loss model has been developed for the LTE E-UTRAN interface (see [2]_ and [3]_).
It is used by the PHY layer to compute the loss due to the propagation. 

The LTE propagation loss model is composed by 4 different models (shadowing, multipath, 
penetration loss and path loss) [2]_:

* Pathloss: :math:`PL = 128.1 + (37.6 * log10 (R))`, where R is the distance between the 
  UE and the eNB in Km.

* Multipath: Jakes model

* PenetrationLoss: 10 dB

* Shadowing: log-normal distribution (mean=0dB, standard deviation=8dB)

Every time that the ``LteSpectrumPHY::StartRx ()`` function is called, the 
``SpectrumInterferenceModel`` is used to computed the SINR, as proposed in [3]_. Then, 
the network device uses the AMC module to map the SINR to a proper CQI and to send it 
to the eNB using the ideal control channel.


References
----------

.. [1] N. Baldo and M. Miozzo, Spectrum-aware Channel and PHY layer modeling for ns3, Proceedings 
       of ICST NSTools 2009, Pisa, Italy. The framework is designed to simulate only data 
       transmissions. For the transmission of control messages (such as CQI feedback, PDCCH, 
       etc..) will be used an ideal control channel). 

.. [2] 3GPP TS 25.814 ( http://www.3gpp.org/ftp/specs/html-INFO/25814.htm ) 

.. [3] Giuseppe Piro, Luigi Alfredo Grieco, Gennaro Boggia, and Pietro Camarda", A Two-level 
       Scheduling Algorithm for QoS Support in the Downlink of LTE Cellular Networks", Proc. of 
       European Wireless, EW2010, Lucca, Italy, Apr., 2010 ( draft version is available on 
       http://telematics.poliba.it/index.php?option=com_jombib&task=showbib&id=330 )

.. [4] 3GPP R1-081483 (available on 
       http://www.3gpp.org/ftp/tsg_ran/WG1_RL1/TSGR1_52b/Docs/R1-081483.zip )
