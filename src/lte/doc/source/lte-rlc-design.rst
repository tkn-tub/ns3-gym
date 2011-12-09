


Overview
++++++++

The RLC entity is specified in the 3GPP technical specification
[TS36322]_, and comprises three different types of RLC: Transparent
Mode (TM), Unacknowledge Mode (UM) and Acknowledged Mode (AM). We
implement only the UM and the AM RLC entities. 

The RLC entities provide the RLC service interface to the upper PDCP layer and the MAC service interface
to the lower MAC layer. The RLC entities use the PDCP service interface from the upper PDCP layer and
the MAC service interface from the lower MAC layer.

Figure :ref:`fig-lte-rlc-implementation-model` shows the
implementation model of the RLC entities and its relationship   
with all the other entities and services in the protocol stack.


.. _fig-lte-rlc-implementation-model:

.. figure:: figures/lte-rlc-implementation-model.*
   :width: 800px

   Implementation Model of PDCP, RLC and MAC entities and SAPs



Service Interfaces
++++++++++++++++++

PDCP Service Interface
----------------------

The PDCP service interface is divided into two parts:

    * the ``PdcpSapProvider`` part is provided by the PDCP layer and used by the upper layer and
    * the ``PdcpSapUser`` part is provided by the upper layer and used by the PDCP layer.

PDCP Service Primitives
^^^^^^^^^^^^^^^^^^^^^^^

The following list specifies which service primitives are provided by the PDCP service interfaces:

    * ``PdcpSapProvider::TransmitRrcPdu``

        * The RRC entity uses this primitive to send an RRC PDU to the lower PDCP entity
          in the transmitter peer

    * ``PdcpSapUser::ReceiveRrcPdu``

        * The PDCP entity uses this primitive to send an RRC PDU to the upper RRC entity
          in the receiver peer

RLC Service Interface
---------------------

The RLC service interface is divided into two parts:

    * the ``RlcSapProvider`` part is provided by the RLC layer and used by the upper PDCP layer and
    * the ``RlcSapUser`` part is provided by the upper PDCP layer and used by the RLC layer.

Both the UM and the AM RLC entities provide the same RLC service interface to the upper PDCP layer.

RLC Service Primitives
^^^^^^^^^^^^^^^^^^^^^^

The following list specifies which service primitives are provided by the RLC service interfaces:

    * ``RlcSapProvider::TransmitPdcpPdu``

        * The PDCP entity uses this primitive to send a PDCP PDU to the lower RLC entity
          in the transmitter peer

    * ``RlcSapUser::ReceivePdcpPdu``

        * The RLC entity uses this primitive to send a PDCP PDU to the upper PDCP entity
          in the receiver peer

MAC Service Interface
---------------------

The MAC service interface is divided into two parts:

  * the ``MacSapProvider`` part is provided by the MAC layer and used by the upper RLC layer and
  * the ``MacSapUser``  part is provided by the upper RLC layer and used by the MAC layer.

MAC Service Primitives
^^^^^^^^^^^^^^^^^^^^^^

The following list specifies which service primitives are provided by the MAC service interfaces:

    * ``MacSapProvider::TransmitPdu``

        * The RLC entity uses this primitive to send a RLC PDU to the lower MAC entity
          in the transmitter peer

    * ``MacSapProvider::ReportBufferStatus``

        * The RLC entity uses this primitive to report the MAC entity the size of pending buffers
          in the transmitter peer

    * ``MacSapUser::NotifyTxOpportunity``

        * The MAC entity uses this primitive to nofify the RLC entity a transmission opportunity

    * ``MacSapUser::ReceivePdu``

        * The MAC entity uses this primitive to send an RLC PDU to the upper RLC entity
          in the receiver peer


Interactions between entities and services
++++++++++++++++++++++++++++++++++++++++++

Transmit operations in downlink
-------------------------------

The following sequence diagram shows the interactions between the
different entities (RRC, PDCP, AM RLC, MAC and MAC scheduler) of the
eNB in the downlink to perform data communications.

Figure :ref:`fig-lte-rlc-data-txon-dl` shows how the upper layers send
data PDUs and how the data flow is processed by the different
entities/services of the LTE protocol stack. We will explain in detail
only the processing related to the AM RLC entity, which is the most
complex. 

.. _fig-lte-rlc-data-txon-dl:
   
.. figure:: figures/lte-rlc-data-txon-dl.*
   :width: 800px

    Sequence diagram of data PDU transmission in downlink

The PDCP entity calls the ``Transmit_PDCP_PDU service primitive`` in
order to send a data PDU. The AM RLC entity processes this service
primitive according to the AM data transfer procedures defined in 
section 5.1.3 of [TS36322]_.

When the ``Transmit_PDCP_PDU`` service primitive is called, the AM RLC
entity performs the following operations:

    * Put the data SDU in the Transmission Buffer.
    * Compute the size of the buffers (how the size of buffers is
      computed will be explained afterwards).
    * Call the ``Report_Buffer_Status`` service primitive of the eNB
      MAC entity in order to notify to the eNB MAC 
      entity the sizes of the buffers of the AM RLC entity. Then, the
      eNB MAC entity updates the buffer status in the MAC scheduler
      using the SchedDlRlcBufferReq service primitive of the FF MAC
      Scheduler API. 

Afterwards, when the MAC scheduler decides that some data can be sent,
the MAC entity notifies it to the RLC entity, i.e. it calls the
``Notify_Tx_Opportunity`` service primitive, then the AM RLC entity
does the following:

    * Create a single data PDU by segmenting and/or concatenating the
      SDUs in the Transmission Buffer. 
    * Move the data PDU from the Transmission Buffer to the
      Transmitted PDUs Buffer. 
    * Update state variables according section 5.1.3.1.1 of
      [TS36322]_. 
    * Call the ``Transmit_PDU`` primitive in order to send the data
      PDU to the MAC entity. 

Retransmission in downlink
--------------------------

The sequence diagram of Figure :ref:`fig-lte-rlc-data-retx-dl` shows
the interactions between the different entities (AM RLC, MAC and MAC
scheduler) of the eNB in downlink when data PDUs must be retransmitted
by the AM RLC entity.  

.. _fig-lte-rlc-data-retx-dl:
   
.. figure:: figures/lte-rlc-data-retx-dl.*
   :width: 500px

    Sequence diagram of data PDU retransmission in downlink

The transmitting AM RLC entity can receive STATUS PDUs from the peer AM RLC entity. STATUS PDUs are
sent according section 5.3.2 of [TS36322]_ and the processing of reception is made according
section 5.2.1 of [TS36322]_.

When a data PDUs is retransmitted from the Transmitted PDUs Buffer, it is also moved to the
Retransmission Buffer.

Transmit operations in uplink
-----------------------------

The sequence diagram of Figure :ref:`fig-lte-rlc-data-txon-ul` shows
the interactions between the different entities of the UE (RRC, PDCP,
RLC and MAC) and the eNB (MAC and Scheduler) in uplink when data PDUs
are sent by the upper layers.

.. _fig-lte-rlc-data-txon-ul:
   
.. figure:: figures/lte-rlc-data-txon-ul.*
   :width: 800px

    Sequence diagram of data PDU transmission in uplink

It is similar to the sequence diagram in downlink; the main difference
is that in this case the Report_Buffer_Status is sent from the UE MAC
to the MAC Scheduler in the eNB over the air using the control
channel. 

Retransmission in uplink
------------------------

The sequence diagram of Figure :ref:`fig-lte-rlc-data-retx-ul` shows
the interactions between the different entities of the UE (AM RLC and
MAC) and the eNB (MAC) in uplink when data PDUs must be retransmitted
by the AM RLC entity. 

.. _fig-lte-rlc-data-retx-ul:

.. figure:: figures/lte-rlc-data-retx-ul.*
   :width: 500px

    Sequence diagram of data PDU retransmission in uplink


AM data transfer
++++++++++++++++

The processing of the data transfer in the AM RLC entity is explained in section 5.1.3 of [TS36322]_.
In this section we describe some details of the implementation of the RLC entity.

Management of buffers in transmit operations
--------------------------------------------

The AM RLC entity manages 3 buffers:

    * **Transmission Buffer**: it is the RLC SDU queue. 
      When the AM RLC entity receives a SDU in the TransmitPdcpPdu service primitive from the
      upper PDCP entity, it enqueues it in the Transmission Buffer. We
      put a limit on the RLC buffer size and just silently drop SDUs
      when the buffer is full. 

    * **Transmitted PDUs Buffer**: it is the queue of transmitted RLC PDUs for which an ACK/NACK has not
      been received yet. When the AM RLC entity sends a PDU to the MAC
      entity, it also puts a copy of the transmitted PDU in the Transmitted PDUs Buffer.

    * **Retransmission Buffer**: it is the queue of RLC PDUs which are considered for retransmission
      (i.e., they have been NACKed). The AM RLC entity moves this PDU to the Retransmission Buffer,
      when it retransmits a PDU from the Transmitted Buffer.


Calculation of the buffer size
------------------------------

The Transmission Buffer contains RLC SDUs. A RLC PDU is one or more SDU segments plus an RLC header.
The size of the RLC header of one RLC PDU depends on the number of SDU segments the PDU contains.

The 3GPP standard  (section 6.1.3.1 of [TS36321]_) says clearly that,
for the uplink, the RLC and MAC headers are not considered in the
buffer size that is to be report as part of the Buffer Status Report. 
For the downlink, the behavior is not specified. Neither [FFAPI]_ specifies
how to do it. Our RLC model works by assuming that the calculation of
the buffer size in the downlink is done exactly as in the uplink,
i.e., not considering the RLC and MAC header size.

We note that this choice affects the interoperation with the
MAC scheduler, since, in response to the
``Notify_Tx_Opportunity`` service primitive, the RLC is expected to
create a PDU of no more than the size requested by the MAC, including
RLC overhead. Hence, unneeded fragmentation can occur if (for example)
the MAC notifies a transmission exactly equal to the buffer size
previously reported by the RLC. We assume that it is left to the Scheduler
to implement smart strategies for the selection of the size of the
transmission opportunity, in order to eventually avoid the inefficiency
of unneeded fragmentation. 



Concatenation and Segmentation
------------------------------

The AM RLC entity generates and sends exactly one RLC PDU for each transmission opportunity even
if it is smaller than the size reported by the transmission opportunity. So for instance, if a
STATUS PDU is to be sent, then only this PDU will be sent in that transmission opportunity.

The segmentation and concatenation for the SDU queue of the AM RLC entity follows the same philosophy
as the same procedures of the UM RLC entity but there are new state variables (see section 7.1) only
present in the AM RLC entity.

It is noted that, according to the 3GPP specs, there is no concatenation for the Retransmission Buffer.

Re-segmentation
---------------

The current model of the AM RLC entity does not support the
re-segmentation of the retransmission buffer. Rather, the AM RLC
entity just expects to receive a big enough transmission
opportunity. An assertion fails if a too small transmission opportunity is 
received.


Unsupported features
--------------------

We do not support the following procedures of [TS36322]_ :

    * “Send an indication of successful delivery of RLC SDU” (See section 5.1.3.1.1)
    * “Indicate to upper layers that max retransmission has been reached” (See section 5.2.1)
    * “SDU discard procedures” (See section 5.3)
    * “Re-establishment procedure” (See section 5.4)

We do not support any of the additional primitives of RLC SAP for AM RLC entity. In particular:

    * no SDU discard notified by PDCP 
    * no notification of successful / failed delivery by AM RLC entity to PDCP entity

