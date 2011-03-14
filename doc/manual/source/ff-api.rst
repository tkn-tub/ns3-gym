.. include:: replace.txt

.. _ff-mac-sched-api:

FemtoForum MAC Scheduler Interface
----------------------------------

This section describes the ns-3 specific version of the `FemtoForum LTE MAC Scheduler Interface Specification v1.11 <http://www.femtoforum.org/femto/technical.php>`_

The goals of the definition of this MAC scheduler API in the |ns3| network simulator are:

* to let MAC's developers to implement its own MAC schedulers allowing these developers to provide the standard MAC scheduler API.
* to let MAC's users to use different MAC schedulers using always the same standard MAC scheduler API.

Design
******

The MAC Scheduler interface is **specified** or defined as **abstract classes**. The MAC Scheduler interface is **implemented** as **derived classes** from the abstract classes. We have splitted the MAC Scheduler interface into 4 abstract classes:

* SCHED SAP API

  * Provider Side is specified in the ``FfMacSchedSapProvider`` class
  * User Side is specified in the ``FfMacSchedSapUser`` class
  
* CSCHED SAP API

  * Provider Side is specified in the ``FfMacCschedSapProvider`` class
  * User Side is specified in the ``FfMacCschedSapUser`` class

Blocks
######

There are 3 blocks involved in the MAC Scheduler interface: Control block, Subframe block and Scheduler block. Each of these blocks provide one part of the MAC Scheduler interface. The following figure shows the relationship between the blocks and the Service Access Points they provide.

.. figure:: figures/ff-mac-saps.png

Implementation details
**********************

This subsection details the criteria adopted during the development of the FF MAC API:

* The definition of the MAC Scheduler interface follows the naming conventions of the |ns3| Coding Style. In particular, we follow the CamelCase convention for the primitive names. For example, the primitive **CSCHED_CELL_CONFIG_REG** is translated to ``CschedCellConfigReg`` in the |ns3| code.

* The same naming conventions are followed for the primitive parameters. As the primitive parameters are member variables of classes, they are also prefixed with a ``m_``.

* FF MAC API is a C-based oriented API but |ns3| network simulator is written in C++. So, STL vectors (``std::vector``) are used, instead of using C arrays as the FF MAC API document suggests.

* In C++, members with constructors and destructors are not allow in ``unions``. These ``unions`` have been converted to ``structs``.



Usage in the ns-3 LTE module
****************************

The files ``rr-ff-mac-scheduler.{cc,h}`` implement a Round Robin MAC scheduler. To interact with the MAC of the eNB, the Round Robin scheduler implements the Provider side of the SCHED SAP and CSCHED SAP interface. If you plan to develop your own scheduler, we advise to create your own class taking inspiration from the Round Robin scheduler. 


The User side of the SCHED SAP interface is implemented in the files ``lte-enb-mac.{cc,.h}``. The User side of the CSCHED SAP interface is implemented in the files ``lte-enb-rrc.{cc,.h}``. You are normally not expected to modify these files in order to implement your own scheduler. 


.. figure:: figures/ff-example.png


