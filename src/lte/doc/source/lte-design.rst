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

.. figure:: figures/lte-enb-architecture.*
   :align: right

   The architecture of the LTE eNB

.. figure:: figures/lte-ue-architecture.*
   :align: right

   The architecture of the LTE UE



Detailed description of the components
**************************************

.. _ff-mac-sched-api:

The FemtoForum MAC Scheduler Interface
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This section describes the ns-3 specific version of the `FemtoForum LTE MAC Scheduler Interface Specification v1.11 <http://www.femtoforum.org/femto/technical.php>`_

The goals of the definition of this MAC scheduler API in the |ns3| network simulator are:

* to let MAC's developers to implement its own MAC schedulers allowing these developers to provide the standard MAC scheduler API.
* to let MAC's users to use different MAC schedulers using always the same standard MAC scheduler API.

Design
++++++

The MAC Scheduler interface is **specified** or defined as **abstract classes**. The MAC Scheduler interface is **implemented** as **derived classes** from the abstract classes. We have splitted the MAC Scheduler interface into 4 abstract classes:

* SCHED SAP API

  * Provider Side is specified in the ``FfMacSchedSapProvider`` class
  * User Side is specified in the ``FfMacSchedSapUser`` class
  
* CSCHED SAP API

  * Provider Side is specified in the ``FfMacCschedSapProvider`` class
  * User Side is specified in the ``FfMacCschedSapUser`` class


There are 3 blocks involved in the MAC Scheduler interface: Control block, Subframe block and Scheduler block. Each of these blocks provide one part of the MAC Scheduler interface. The following figure shows the relationship between the blocks and the Service Access Points they provide.

.. figure:: figures/ff-mac-saps.*

Implementation details
++++++++++++++++++++++

This subsection details the criteria adopted during the development of the FF MAC API:

* The definition of the MAC Scheduler interface follows the naming conventions of the |ns3| Coding Style. In particular, we follow the CamelCase convention for the primitive names. For example, the primitive **CSCHED_CELL_CONFIG_REG** is translated to ``CschedCellConfigReg`` in the |ns3| code.

* The same naming conventions are followed for the primitive parameters. As the primitive parameters are member variables of classes, they are also prefixed with a ``m_``.

* FF MAC API is a C-based oriented API but |ns3| network simulator is written in C++. So, STL vectors (``std::vector``) are used, instead of using C arrays as the FF MAC API document suggests.

* In C++, members with constructors and destructors are not allow in ``unions``. These ``unions`` have been converted to ``structs``.



Usage in the ns-3 LTE module
++++++++++++++++++++++++++++

To clarify how the MAC Scheduler Interface is used within the eNB, we consider the example of the Round Robin Scheduler which is depicted in the figure below. To interact with the MAC of the eNB, the Round Robin scheduler implements the Provider side of the SCHED SAP and CSCHED SAP interfaces. The MAC of the eNB implements the provider side of the SCHED SAP and CSCHED SAP interfaces. A similar approach is   If you plan to develop your own scheduler, we advise to create your own class taking inspiration from the Round Robin scheduler. 


The User side of both the CSCHED SAP and the SCHED SAP are implemented in the file ``lte-enb-mac.cc``. You are normally not expected to modify these files in order to implement your own scheduler. 


.. figure:: figures/ff-example.*


MAC Scheduler Implementations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In this section we describe the features of the MAC Scheduler Implementations that are included with the simulator. Both these models are well-known from the literature. The reason for their inclusion is twofold: first, they can be used as starting code base for the development of more advanced schedulers; second, they can be used as reference algorithm when doing performance evaluation. With this latter respect, we stress that the use of a publicly available scheduler implementation as the reference for a performance evaluation study is beneficial to the authoritativeness of the study itself.

Before describing the schedulers, we would like introduce a bit the problem of resource allocation in LTE. The scheduler is in charge of generating specific structures calles Data Control Indication (DCI) by which communicates to the users connected the resource allocation per subframe basis. In doing this in the downlink direction, the scheduler has to fill some specific fields of the DCI structure with all the information, such as: MCS, TB size and the allocation bitmap. The latter bitmap can be coded in different manner, in this implementation we considered the "allocation Type 0" [36.213], which implies the aggregation of the RB in group of different size as function of the bandwidth in use. This implies that for certain bandwidth values not all the RBs are usable, since the group size is not a common divisor of the group. This is for instance the case when system bandwith is equal to 25 RBs, which results in a RBG size of 2 RBs and therefore 1 RB will result not addressable. 
In uplink the structure of the DCIs changes and since it uses only adjacent RBs allows the use of all RBs in all the configurations.

On this matter, another constaint of LTE is represented by the assumption that the scheduler can allocate only one transport channel to a user. This implies that the control of the scheduler is only per users basis and the different LCs have to be multiplxed in the same transport block.


Round Robin (RR) Scheduler
++++++++++++++++++++++++++

The RR scheduler is one of the simpliest solution in literature. It divides the available resources among the active flows, which means all the logical channels that has something in the queues. In case the number of active flows is greater then the number of RBGs available, the RR scheduler allocates the users in a round robin fashion within multiples subframes (i.e., users that cannot be allocated in a subframe will be allocated in the subsequents ones). The evaluation of the MCS to be adopted for each user is done according to the received wideband CQIs.


Proportional Fair (PF) Scheduler
++++++++++++++++++++++++++++++++

The PF scheduler[5]_ philosophy is to schedule a user when its instantaneous channel quality is high relative to its own average channel condition over time. In formula this implies that a user :math:`k` is allocated to a rb :math:`m` in a subframe :math:`f` when it is the one that maximizes the formula
:math:`\hat{k_{m}} = \underset{k`=1,...,k}{\operatorname{argmax}}\frac{ R_\mathrm{k`}(m,f) }{ T_\mathrm{k`}(f) }\end{split}\notag`
where :math:`T_{k}(f)` is the long-term average througput perceived by the user :math:`k` during the subframe :math:`f` and :math:`R_{k}(m,f)` is the achievable rate evaluated by the PFS during the subframe :math:`f`. The latter is evaluated by estimating the MCS to be used according to the the inband CQIs of the user :math:`k` in RB :math:`m` and therefore computing the TB size as function of the MCS considering the transmission of a single RB. The long-term average user throughputs are computed each subframe by

:math:`T_\mathrm{k}(f) = (1-\frac{1}{t_\mathrm{c}})T_{k}(f-1)+\frac{1}{t_\mathrm{c}}\sum{i}^{M}_{m = 1}R_\mathrm{k}(m,f)\Phi(\hat{k_{m}}=k\end{split})\notag
%\begin{split}T_\mathrm{k}(f) = (1-\frac{1}{t_\mathrm{c}})T_{k}(f-1)+\frac{1}{t_\mathrm{c}\sum{i}^{M}_{m = 1}R_\mathrm{k}(m,f)\I\{\hat{k_\matrm{m}}=k\}`

where :math:`t_c` is the time window over which the fairness is imposed and :math:`\Phi(\cdot)` is the indicator function equal to one if :math:`\hat{k_m}=k` and zero otherwise. According to this allocation scheme a user can be allocated to different RBGs either undjacents as function of the actual condition of the channel and its past history in terms of throughput.


LTE Spectrum Model
~~~~~~~~~~~~~~~~~~

Here we describe how the LTE spectrum model works and what it allows to do.



Physical layer
~~~~~~~~~~~~~~
The physical layer model provided in this LTE simulator model supports Frequency Division Duplex (FDD) only.



Propagation Loss Models
~~~~~~~~~~~~~~~~~~~~~~~
NOTE: this information refers to the GSoC model, which as of this writing is not working anymore. 

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
**********

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

.. [5] S. Sesia, I. Toufik and M. Baker, “LTE – The UMTS Long Term Evolution”, Ed Wiley, 2009.
