PageBreak

Low-Rate Wireless Personal Area Network (LR-WPAN)
-------------------------------------------------

This chapter describes the implementation of ns-3 models for the
low-rate, wireless personal area network (LR-WPAN) as specified by
IEEE standard 802.15.4 (2006).

Model Description
*****************

The source code for the lr-wpan module lives in the directory ``src/lr-wpan``.

Design
======

The model design closely follows the standard from an architectural standpoint.

.. _fig-lr-wpan-arch:

.. figure:: figures/lr-wpan-arch.*

    Architecture and scope of lr-wpan models

The grey areas in the figure (adapted from Fig 3. of IEEE Std. 802.15.4-2006)
show the scope of the model.

The Spectrum NetDevice from Nicola Baldo is the basis for the implementation.

The implementation also borrows from the ns-2 models developed by Zheng
and Lee, and certain aspects of those models have been ported over.

APIs
####

The APIs closely follow the standard, adapted for ns-3 naming conventions
and idioms.  The APIs are organized around the concept of service primitives
as shown in the following figure adapted from Figure 14 of 
IEEE Std. 802.15.4-2006.

.. _fig-lr-wpan-arch:

.. figure:: figures/lr-wpan-primitives.*

    Service primitives

The APIs are organized around four conceptual services and service access
points (SAP):

* MAC data service (MCPS)
* MAC management service  (MLME)
* PHY data service (PD)
* PHY management service (PLME)

In general, primitives are standardized as follows (e.g. Sec 7.1.1.1.1
of IEEE 802.15.4-2006):::

  MCPS-DATA.request      (
                          SrcAddrMode,
                          DstAddrMode,
                          DstPANId,
                          DstAddr,
                          msduLength,
                          msdu,
                          msduHandle,
                          TxOptions,
                          SecurityLevel,
                          KeyIdMode,
                          KeySource,
                          KeyIndex
                          )

This maps to ns-3 classes and methods such as:::

  struct McpsDataRequestParameters
  {
    uint8_t m_srcAddrMode;
    uint8_t m_dstAddrMode;
    ...
  };

  void
  LrWpanMac::McpsDataRequest (McpsDataRequestParameters params)
  {
  ...
  }

MAC
###

To be written.  Focuses on unslotted CSMA/CA.

PHY
###

The physical layer components consist of a Phy model, an error rate model, 
and a loss model.  The error rate model presently models the error rate 
for IEEE 802.15.4 2.4 GHz AWGN channel for OQPSK; the model description can 
be found in IEEE Std 802.15.4-2006, section E.4.1.7.   The Phy model is 
based on SpectrumPhy and it follows specification described in section 6 
of IEEE Std 802.15.4-2006. It models PHY service specifications, PPDU 
formats, PHY constants and PIB attributes. It currently only supports 
the transmit power spectral density mask specified in 2.4 GHz per section 
6.5.3.1. The noise power density assumes uniformly distributed thermal 
noise across the frequency bands. The loss model can fully utilize all 
existing simple (non-spectrum phy) loss models. The Phy model uses 
the existing single spectrum channel model.

Scope and Limitations
=====================

Future versions of this document will contain a PICS proforma similar to
Appendix D of IEEE 802.15.4-2006.  The current emphasis is on the 
unslotted mode of 802.15.4 operation for use in Zigbee, and the scope
is extremely limited, to the point of just getting some basic end-to-end
frames flowing.

References
==========

* Wireless Medium Access Control (MAC) and Physical Layer (PHY) 
Specifications for Low-Rate Wireless Personal Area Networks (WPANs), 
IEEE Computer Society, IEEE Std 802.15.4-2006, 8 September 2006.
* J. Zheng and Myung J. Lee, "A comprehensive performance study of IEEE 
802.15.4," Sensor Network Operations, IEEE Press, Wiley Interscience, 
Chapter 4, pp. 218-237, 2006.

Usage
*****

Enabling lr-wpan
================

Add ``lr-wpan`` to the list of modules built with ns-3.

Helper
======

TBD, this is similar to other device helpers for ns-3.

Examples
========

The following examples have been written, which can be found in ``src/lr-wpan/examples/``:

* ``lr-wpan-data.cc``:  A simple example showing end-to-end data transfer.
* ``lr-wpan-packet-print.cc``:  An example to print out the MAC header fields.
* ``lr-wpan-phy-test.cc``:  An example to test the phy.

In particular, the module enables a very simplified end-to-end data
transfer scenario, implemented in ``lr-wpan-data.cc``.  The figure
shows a sequence of events that are triggered when the MAC receives
a DataRequest from the higher layer.  It invokes a Clear Channel
Assessment (CCA) from the PHY, and if successful, sends the frame
down to the PHY where it is transmitted over the channel and results
in a DataIndication on the peer node.
  
.. _fig-lr-wpan-data:

.. figure:: figures/lr-wpan-data-example.*

    Data example for simple LR-WPAN data transfer end-to-end

Tests
=====

The following tests have been written, which can be found in ``src/lr-wpan/tests/``:

* ``lr-wpan-packet-test.cc``:  Test the 802.15.4 MAC header/trailer classes
* ``lr-wpan-pd-plme-sap-test.cc``:  Test the PLME and PD SAP per IEEE 802.15.4

Validation
**********

This model has been tested as follows (*to be completed*).
