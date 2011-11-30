.. include:: replace.txt




EPC modeling assumptions
------------------------
 
We focus on the simplified EPC model which is represented in the figure :ref:`fig-epc-topology`. The main modeling assumptions are:

 * Only IPv4 is considered
 * Only the EPC data plane is considered in detail. 
 * The EPC control plane is not modeled explicitly. The control plane funcionality that is needed for the purpose of the simulation (e.g., UE attach and bearer setup procedures) are implemented by direct interaction among simulation objects performed via ns-3 helpers.


.. _fig-epc-topology:
   
.. figure:: figures/epc-topology.*
   :align: center

   The simplified EPC network topology considered


Implementation
--------------




.. _fig-epc-data-flow-dl:
   
.. figure:: figures/epc-data-flow-dl.*
   :align: center

   Data flow in the dowlink between the internet and the UE


.. _fig-epc-data-flow-ul:
   
.. figure:: figures/epc-data-flow-ul.*
   :align: center

   Data flow in the uplink between the UE and the internet


.. _fig-lte-arch-data-rrc-pdcp-rlc:
   
.. figure:: figures/lte-arch-data-rrc-pdcp-rlc.*
   :align: center

   Architecture of the data plane showing the RRC, PDCP and RLC protocols
