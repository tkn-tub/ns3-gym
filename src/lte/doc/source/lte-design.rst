.. include:: replace.txt


++++++++++++++++++++++++++
 LTE Design Documentation
++++++++++++++++++++++++++


An overview of the LTE module
*****************************


Design Criteria
~~~~~~~~~~~~~~~


The LTE module has been designed to support the evaluation of the following aspects of LTE systems:  

 * Radio Resource Management
 * QoS-aware Packet Scheduling
 * Inter-cell Interference Coordination
 * Dynamic Spectrum Access

In order to model LTE systems to a level of detail that is sufficient to allow a
correct evaluation of the above mentioned aspects, the following requirements
have been considered:

 #. At the radio level, the granularity of the model should be at least that
    of the Resource Block (RB). In fact, this is the fundamental unit being used for
    resource allocation. Without this minimum level of granularity, it is not
    possible to model accurately packet scheduling and
    inter-cell-interference.
    The reason is that, since packet scheduling is done on
    a per-RB basis, an eNB might transmit on a subset only of all the available
    RBs, hence interfering with other eNBs only on those RBs where it is
    trasmitting.
    Note that this requirement rules out the adoption of a system lever simulation
    approach, which evaluates resource allocation only at the granularity of
    call/bearer establishment.
 #. The simulator should scale up to tens of eNBs and hundreds of User
    Equipments (UEs). This
    rules out the use of a link level simulator, i.e., a simulator whose radio
    interface is modeled with a granularity up to the symbol level. This is because
    to have a symbol level model it is necessary to implement all the PHY
    layer signal processing, whose huge computational complexity severely limits
    simulation. In fact, link-level simulators are normally limited to a single eNB
    and one or a few UEs.
 #. It should be possible within the simulation to configure different cells
    so that they use different carrier frequencies and system bandwidths. The
    bandwidth used by different cells should be allowed to overlap, in order to
    support dynamic spectrum licensing solutions such as those described in
    in~\cite{ofcom2.6GHz, RealWireless}. The calculation of interference should
    handle appropriately this case.
 #. To be more representative of the LTE standard, as well as to be as
    close as possible to real-world implementations, the simulator
    should support the MAC Scheduler API published by the FemtoForum
    [FFAPI]_. This interface is expected to be used by femtocell manufacturers
    for the implementation of scheduling and Radio Resource Management
    (RRM) algorithms. By introducing support for this interface in the
    simulator, we make it possible for LTE equipment vendors and
    operators to test in a simulative environment exactly the same
    algorithms that would be deployed in a real system.  
#.  The LTE simulation module should contain its own implementation of
    the API defined in [FFAPI]_. Neither
    binary nor data structure compatibility with vendor-specific implementations
    of the same interface are expected; hence, a compatibility layer should be
    interposed whenever a vendor-specific MAC scheduler is to be used with the
    simulator. This requirement is necessary to allow the
    simulator to be independent from vendor-specific implementations of this
    interface specification. We note that [FFAPI]_ is a logical
    specification only, and its implementation (e.g., translation to some specific
    programming language) is left to the vendors. 



Module Architecture
~~~~~~~~~~~~~~~~~~~

The overall architecture of the LTE module is represented in the
figures :ref:`fig-lte-enb-architecture` and :ref:`fig-lte-ue-architecture`, which
deal respectively with the eNB and the UE. A detailed description of the most
important components are provided in the following sections.



.. _fig-lte-enb-architecture:
   
.. figure:: figures/lte-enb-architecture.*

   Architecture of the LTE eNB



.. _fig-lte-ue-architecture:

.. figure:: figures/lte-ue-architecture.*

   Architecture of the LTE UE




.. _ff-mac-sched-api:

The FemtoForum MAC Scheduler Interface
++++++++++++++++++++++++++++++++++++++

This section describes the ns-3 specific version of the LTE MAC
Scheduler Interface Specification published by the FemtoForum [FFAPI]_.

We implemented the ns-3 specific version of the FemtoForum MAC Scheduler
Interface [FFAPI]_ as a set of C++ abstract
classes; in particular, each primitive is translated to a C++ method of a
given class. The term *implemented* here is used with the same
meaning adopted in [FFAPI]_, and hence refers to the process of translating
the logical interface specification to a particular programming language.
The primitives in [FFAPI]_ are grouped in two groups: the CSCHED
primitives, which deal with scheduler configuration, and the SCHED primitives,
which deal with the execution of the scheduler. Furthermore, [FFAPI]_
defines primitives of two different kinds: those of type REQ go from the MAC to
the Scheduler, and those of type IND/CNF go from the scheduler to the MAC. To
translate these characteristics into C++, we define the following abstract
classes that implement Service Access Points (SAPs) to be used to issue the
primitives:

 *  the ``FfMacSchedSapProvider`` class defines all the C++ methods that
    correspond to SCHED primitives of type REQ;
 *  the ``FfMacSchedSapUser`` class defines all the C++ methods that
    correspond to SCHED primitives of type CNF/IND;
 *  the ``FfMacCschedSapProvider`` class defines all the C++ methods that
    correspond to CSCHED primitives of type REQ;
 *  the ``FfMacCschedSapUser`` class defines all the C++ methods that
    correspond to CSCHED primitives of type CNF/IND;


There are 3 blocks involved in the MAC Scheduler interface: Control block,
Subframe block and Scheduler block. Each of these blocks provide one part of the
MAC Scheduler interface. The figure below shows the relationship
between the blocks and the SAPs defined in our implementation of the MAC
Scheduler Interface. 

.. figure:: figures/ff-mac-saps.*

In addition to the above principles, the following design choices have been
taken:

 *  The definition of the MAC Scheduler interface classes follows the naming
    conventions of the |ns3| Coding Style. In particular, we follow the
    CamelCase convention for the primitive names. For example, the primitive
    ``CSCHED_CELL_CONFIG_REG`` is translated to ``CschedCellConfigReg``
    in the |ns3| code.
 *  The same naming conventions are followed for the primitive parameters. As
    the primitive parameters are member variables of classes, they are also prefixed
    with a ``m_``.
 *  regarding the use of vectors and lists in data structures, we note
    that [FFAPI]_ is a pretty much C-oriented API. However, considered that
    C++ is used in ns-3, and that the use of C arrays is discouraged, we used STL
    vectors (``std::vector``) for the implementation of the MAC Scheduler
    Interface, instead of using C arrays as implicitly suggested by the
    way [FFAPI]_ is written.
 *  In C++, members with constructors and destructors are not allow in
    ``unions``. Hence all those data structures that are said to be
    ``unions`` in [FFAPI]_ have been defined as ``structs`` in our code.

The figure below shows how the MAC Scheduler Interface is
used within the eNB. 

.. figure:: figures/ff-example.*

The User side of both the CSCHED SAP and the SCHED SAP are
implemented within the eNB MAC, i.e., in the file ``lte-enb-mac.cc``.
The eNB MAC can be used with different scheduler implementations without
modifications. The same figure also shows, as an example, how the Round Robin
Scheduler is implemented: to interact with the MAC of the eNB, the Round Robin
scheduler implements the Provider side of the SCHED SAP and CSCHED
SAP interfaces. A similar approach can be used to implement other schedulers as
well. A description of all the scheduler implementations that we provide as
part of our LTE simulation module will be given in
the following.


Resource Allocation Model
+++++++++++++++++++++++++


We now briefly describe how resource allocation is handled in LTE,
clarifying how it is implemented in the simulator. The scheduler is in
charge of generating specific structures calles Data Control Indication (DCI)
which are then transmitted by the PHY of the eNB to the connected UEs, in order
to inform them of the resource allocation on a per subframe basis. In doing this
in the downlink direction, the scheduler has to fill some specific fields of the
DCI structure with all the information, such as: the Modulation and Coding
Scheme (MCS) to be used, the MAC Transport Block (TB) size, and the allocation
bitmap which identifies which RBs will contain the data
tranmitted by the eNB to each user. For the mapping of resources to
physical RBs, we adopt a *localized mapping* approach
(see [Sesia2009]_, Section 9.2.2.1);
hence in a given subframe each RB is always allocated to the same user in both
slots.
The allocation bitmap can be coded in
different formats; in this implementation, we considered the *Allocation
Type 0* defined in [TS36.213]_, according to which the RBs are grouped in
Resource Block Groups (RBG) of different size determined as a function of the
Transmission Bandwidth Configuration in use. For certain bandwidth
values not all the RBs are usable, since the 
group size is not a common divisor of the group. This is for instance the case
when the bandwith is equal to 25 RBs, which results in a RBG size of 2 RBs, and
therefore 1 RB will result not addressable. 
In uplink the format of the DCIs is different, since only adjacent RBs
can be used because of the SC-FDMA modulation. As a consequence, all
RBs can be allocated by the eNB regardless of the bandwidth
configuration. 



MAC Scheduler Implementations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In this section we describe the features of the MAC Scheduler Implementations that are included with the simulator. Both these models are well-known from the literature. The reason for their inclusion is twofold: first, they can be used as starting code base for the development of more advanced schedulers; second, they can be used as reference algorithm when doing performance evaluation. With this latter respect, we stress that the use of a publicly available scheduler implementation as the reference for a performance evaluation study is beneficial to the authoritativeness of the study itself.


Round Robin (RR) Scheduler
++++++++++++++++++++++++++

Here we describe the RR scheduler that we implement, providing references from the literature.


Proportional Fair (PF) Scheduler
++++++++++++++++++++++++++++++++

Here we describe the PF scheduler that we implement, providing references from the literature.

Spectrum Model


Physical layer
~~~~~~~~~~~~~~
The usage of the radio spectrum by eNBs and UEs in LTE is described in [TS36.101]_. 

In the simulator, we model it as follow. for communications is modeled as follows. 
Let :math:`f_c` denote the  LTE Absolute Radio Frequency Channel Number, which identifies the carrier frequency on a 100 kHz raster; furthermore, let :math:`B` be the Transmission Bandwidth Configuration in number of Resource Blocks. For every pair :math:`(f_c,B)` used in the simulation we create a corresponding spectrum model using the `ns3::Spectrum`` framework of [Baldo2009]_. All these LTE-specific spectrum models are used to simulate different spectrum usage policies in the same simulation, for example allowing the simulation of cognitive radio / dynamic spectrum access strategies for LTE. 

The physical layer model provided in this LTE simulator model supports Frequency Division Duplex (FDD) only.



Propagation Loss Models
~~~~~~~~~~~~~~~~~~~~~~~
NOTE: this information refers to the GSoC model, which as of this writing is not working anymore. 

A proper propagation loss model has been developed for the LTE E-UTRAN interface (see [TS25.814]_ and [Piro2010]_).
It is used by the PHY layer to compute the loss due to the propagation. 

The LTE propagation loss model is composed by 4 different models (shadowing, multipath, 
penetration loss and path loss) [TS25.814]_:

* Pathloss: :math:`PL = 128.1 + (37.6 * log10 (R))`, where R is the distance between the 
  UE and the eNB in Km.

* Multipath: Jakes model

* PenetrationLoss: 10 dB

* Shadowing: log-normal distribution (mean=0dB, standard deviation=8dB)

Every time that the ``LteSpectrumPHY::StartRx ()`` function is called, the 
``SpectrumInterferenceModel`` is used to computed the SINR, as proposed in [Piro2010]_. Then, 
the network device uses the AMC module to map the SINR to a proper CQI and to send it 
to the eNB using the ideal control channel.

