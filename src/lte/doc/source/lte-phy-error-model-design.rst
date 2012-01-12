


++++++++++++++++++++++++++
 Design Documentation
++++++++++++++++++++++++++


PHY Error Model
+++++++++++++++

The simulator includes an error model of the data plane (i.e., PDSCH) according to the standard link-to-system mapping (LSM) techniques. The choice is aligned with the standard system simulation methodology of OFDMA  radio transmission technology. Thanks to LSM we are able to maintain a good level of accuracy and at the same time limiting the computational complexity increase. It is based on the mapping of single link layer performance obtained by means of link level simulators to system (in our case network) simulators. In particular link the layer simulator is used for generating the performance of a single link from a PHY layer perspective, usually in terms of code block error rate (BLER), under specific static conditions. LSM allows the usage of these parameters in more complex scenarios, typical of system/network simulators, where we have more links, interference and "colored" channel propagation phenomena (e.g., frequency selective fading).

To do this the Vienna LTE Simulator [Vienna]_ has been used for what concerns the extraction of link layer performance and the Mutual Information Based Effective SINR (MIESM) as LSM mapping function using part of the work recently published by the Signet Group of University of Padua [PaduaPEM]_.


MIESM
-----

The specific LSM method adopted is the one based on the usage of a mutual information metric, commonly referred to as the mutual information per per coded bit (MIB or MMIB when a mean of multiples MIBs is involved). Another option would be represented by the Exponential ESM (EESM); however, recent studies demonstrate that MIESM outperforms EESM in terms of accuracy [LozanoCost]_.
Moreover, from an HARQ perspective, the MIESM has more flexibility in managing the combinations of the HARQ blocks. In fact, by working in the MI field, the formulas for evaluating both the chase combining (CC) and the incremental redundancy (IR) schemes work in the MI field as well, where there is no dependency respect to the MCS. On the contrary, the HARQ model of EESM works in the effective SINR field, which is MCS dependent, and does not allow the combination of HARQ blocks using different MCSs [wimaxEmd]_.

.. _fig-miesm-architecture:

.. figure:: figures/miesm_scheme.*
   :align: center

   MIESM computational procedure diagram

The mutual information (MI) is dependent on the constellation mapping and can be calculated per transport block (TB) basis, by evaluating the MI over the symbols and the subcarrier. However, this would be too complex for a network simulator. Hence, in our implementation a flat channel response within the RB has been considered; therefore the overall MI of a TB is calculated averaging the MI evaluated per each RB used in the TB. In detail, the implemented scheme is depicted in Figure :ref:`fig-miesm-architecture`, where we see that the model starts by evaluating the MI value for each RB, represented in the figure by the SINR samples. Then the equivalent MI is evaluated per TB basis by averaging the MI values. Finally, a further step has to be done since the link level simulator returns the performance of the link in terms of block error rate (BLER) in a addive white guassian noise  (AWGN) channel, where the blocks are the code blocks (CBs) independently encoded/decoded by the turbo encoder. On this matter the standard 3GPP segmentation scheme has been used for estimating the actual CB size (described in section 5.1.2 of [TS36.212]_). This scheme divides the the TB in :math:`N_{K_-}` blocks of size :math:`K_-` and :math:`N_{K+}` blocks of size :math:`K_+`. Therefore the overall TB BLER (TBLER) can be expressed as

.. math::

  TBLER = 1- \prod\limits_{i=1}^{C}(1-CBLER_i)

where the :math:`CBLER_i` is the BLER of the CB :math:`i` obtained according to the link level simulator CB BLER curves.
For estimating the :math:`CBLER_i`, the MI evaluation has been implemented according to its numerical approximation defined in [wimaxEmd]_. Moreover, for reducing the complexity of the computation, the approximation has been converted into lookup tables. In detail, Gaussian cumulative model has been used for approximating the AWGN BLER curves with three parameters which provides a close fit to the standard AWGN performances, in formula:

.. math::

  CBLER_i = \frac{1}{2}\left[1-erf\left(\frac{x-b_{ECR}}{\sqrt{2}c_{ECR}} \right) \right]

where :math:`x` is the MI of the TB, :math:`b_{ECR}` represents the "transition center" and :math:`c_{ECR}` is related to the "transition width" of the Gaussian cumulative distribution for each Effective Code Rate (ECR) which is the actual transmission rate according to the channel coding and MCS. For limiting the computational complexity of the model we considered only a subset of the possible ECRs in fact we would have potentially 5076 possible ECRs (i.e., 27 MCSs and 188 CB sizes). On this respect, we will limit the CB sizes to some representative values (i.e., 40, 140, 256, 512, 1024, 2048, 4032, 6144), while for the others the worst one approximating the real one will be used (i.e., the smaller CB size value available respect to the real one). This choice is aligned to the typical performance of turbo codes, where the CB size is not strongly impacting on the BLER. However, it is to be notes that for CB sizes lower than 1000 bits the effect might be relevant (i.e., till 2 dB); therefore, we adopt this unbalanced sampling interval for having more precision where it is necessary. This behaviour is confirmed by the figures presented in the Annes Section.


Link Layer Performance
---------------------

On this respect, we reused part of the curves obtained within [PaduaPEM]_. In detail, we introduced the CB size dependency to the CB BLER curves with the support of the developers of [PaduaPEM]_ and of the LTE Vienna Simulator. In fact, the module released provides the link layer performance only for what concerns the MCSs (i.e, with a given fixed ECR). In detail the new error rate curves for each has been evaluated with a simulation campaign with the link layer simulator for a single link with AWGN noise and for CB size of 104, 140, 256, 512, 1024, 2048, 4032 and 6144. These curves has been mapped with the Gaussian cumulative model formula presented above for obtaining the correspondents :math:`b_{ECR}` and :math:`c_{ECR}` parameters.



Simulator Design
----------------

The model implemented uses the curves for the LSM of the recently LTE PHY Error Model released in the ns3 community by the Signet Group [PaduaPEM]_ and the new ones generated for different CB sizes. The ``LteSpectrumPhy`` class is in charge of evaluating the TB BLER thanks to the methods provided by the ``LteMiErrorModel`` class, which is in charge of evaluating the TB BLER according to the vector of the perceived SINR per RB, the MCS and the size in order to proper model the segmentation of the TB in CBs. In order to obtain the vector of the perceived SINR two instances of ``LtePemSinrChunkProcessor`` (child of ``LteSinrChunkProcessor`` dedicated to evaluate the SINR for obtaining physical error performance) have been attached to UE downlink and eNB uplink ``LteSpectrumPhy`` modules for evaluating the error model distribution respectively of PDSCH (UE side) and ULSCH (eNB side).


Test Environment
----------------

The test suite ``lte-phy-error-model`` generates nine test cases with single eNB and a various number of UEs, all having the same Radio Bearer specification. Each test is designed for evaluating the error rate perceived by a specific TB size in order to verify that it corresponds to the expected values according to the BLER generated for CB size analog to the TB size. This means that, for instance, the test will check that the performance of a TB of :math:`N` bits is analogous to the one of a a CB size of :math:`N` bits by collecting the performance of a user which has been forced the generation of a such TB size according to the distance to eNB. In order to significantly test the BER at MAC level, we modified the Adaptive Modulation and Coding (AMC) module, the ``LteAmc`` class, for making it less robust to channel conditions by adding a configurable BER parameter (called ``Ber`` in the ns3 attribute system) which enable the selection of the desired BER at MAC level when choosing the MCS to be used. In detail, the AMC module has been forced to select the AMC considering a BER of 0.01 (instead of the standard value equal to 0.00005). We note that, these values do not reflect actual BER since they come from an analytical bound which do not consider all the transmission chain aspects; therefore the resulted BER might be different. 

The parameters of the nine test cases are reported in the following:

 #. 4 UEs placed 898 meters far from the eNB, which implies the use of MCS 2 (SINR of -2.21 dB) and a TB of 176 bits, that in turns produce a BER of 0.19 (see point A in figure :ref:`fig-mcs-2-test`).
 #. 3 UEs placed 900 meters far from the eNB, which implies the use of MCS 2 (SINR of -2.25 dB) and a TB of 328 bits, that in turns produce a BER of 0.09 (see point B in figure :ref:`fig-mcs-2-test`).
 #. 2 UEs placed 920 meters far from the eNB, which implies the use of MCS 2 (SINR of -2.61 dB) and a TB of 72 bits, that in turns produce a BER of 0.123 (see point D in figure :ref:`fig-mcs-2-test`).
 #. 1 UE placed 930 meters far from the eNB, which implies the use of MCS 2 (SINR of -2.79 dB) and a TB of 72 bits, that in turns produce a BER of 0.9 (see point D in figure :ref:`fig-mcs-2-test`).
 #. 1 UE placed 538 meters far from the eNB, which implies the use of MCS 12 (SINR of 4.19 dB) and a TB of 4776 bits, that in turns produce a BER of 0.017 (see point E in figure :ref:`fig-mcs-12-test`).
 #. 3 UEs placed 538 meters far from the eNB, which implies the use of MCS 12 (SINR of 4.19 dB) and a TB of 1608 bits, that in turns produce a BER of 0.23 (see point F in figure :ref:`fig-mcs-12-test`).
 #. 7 UEs placed 538 meters far from the eNB, which implies the use of MCS 12 (SINR of 4.19 dB) and a TB of 376 bits, that in turns produce a BER of 0.72 (see point G in figure :ref:`fig-mcs-12-test`).
 #. 1 UE placed 500 meters far from the eNB, which implies the use of MCS 14 (SINR of 5.53 dB) and a TB of 6248 bits (segmented in 2 CBs of 3136 bits each one), that in turns produce a BER of 0.18, since each CB has CBLER equal to 0.096 (see point H in figure :ref:`fig-mcs-14-test`).


.. _fig-mcs-2-test:

.. figure:: figures/MCS_2_test.*
   :align: center


   BLER for tests 1, 2, 3, 4.

.. _fig-mcs-12-test:

.. figure:: figures/MCS_12_test.*
   :align: center

   BLER for tests 5, 6, 7.

.. _fig-mcs-14-test:

.. figure:: figures/MCS_14_test.*
   :align: center

   BLER for test 8.

The test verifies that in each case the expected number of packets received correct corresponds to a Bernoulli distribution with a confidence interval of 95%, where the probability of success in each trail is :math:`1-BER` and :math:`n` is the total number of packet sent.


Annex
-----

In this section the figures of the BLER of all the MCS are reported (blue lines) together with their correspondent mapping to the Gaussian cumulative distribution (red dashed lines).

.. _fig-mcs-1-4-ber:

.. figure:: figures/MCS_1_4.*
   :width: 900px
   :align: center
   :height: 700px


   BLER for MCS 1, 2, 3 and 4.


.. _fig-mcs-5-8-ber:

.. figure:: figures/MCS_5_8.*
   :width: 900px
   :align: center
   :height: 700px


   BLER for MCS 5, 6, 7 and 8.

.. _fig-mcs-9-12-ber:

.. figure:: figures/MCS_9_12.*
   :width: 900px
   :align: center
   :height: 700px


   BLER for MCS 9, 10, 11 and 12.

.. _fig-mcs-13-16-ber:

.. figure:: figures/MCS_13_16.*
   :width: 900px
   :align: center
   :height: 700px


   BLER for MCS 13, 14, 15 and 16.


.. _fig-mcs-17-20-ber:

.. figure:: figures/MCS_17_20.*
   :width: 900px
   :align: center
   :height: 700px


   BLER for MCS 17, 17, 19 and 20.

.. _fig-mcs-21-24-ber:

.. figure:: figures/MCS_21_24.*
   :width: 900px
   :align: center
   :height: 700px


   BLER for MCS 21, 22, 23 and 24.

.. _fig-mcs-25-27-ber:

.. figure:: figures/MCS_25_27.*
   :width: 900px
   :align: center
   :height: 700px


   BLER for MCS 25, 26 and 27.








.. [PaduaPEM] http://mailman.isi.edu/pipermail/ns-developers/2011-November/009559.html

.. [Vienna] The Vienna LTE Simulators http://www.nt.tuwien.ac.at/about-us/staff/josep-colom-ikuno/lte-simulators/

.. [LozanoCost] Joan Olmos, Silvia Ruiz, Mario García-Lozano and David Martín-Sacristán, "Link Abstraction Models Based on Mutual Information for LTE Downlink", COST 2100 TD(10)11052 Report

.. [wimaxEmd] WiMAX Forum White Paper, WiMAX System Evaluation Methodology, July 2008.

.. [TS36.212] 3GPP TS 36.212 "E-UTRA Multiplexing and channel coding"
