.. include:: replace.txt
.. highlight:: bash

+++++++++++++++++++++
Testing Documentation
+++++++++++++++++++++

At present, most of the available documentation about testing and validation
exists in publications, some of which are referenced below.

Error model
***********

Validation results for the 802.11b error model are available in this
`technical report <http://www.nsnam.org/~pei/80211b.pdf>`__

Two clarifications on the results should be noted.  First, Figure 1-4
of the above reference
corresponds to the |ns3| NIST BER model.   In the program in the 
Appendix of the paper (80211b.c), there are two constants used to generate
the data.  The first, packet size, is set to 1024 bytes.  The second,
"noise", is set to a value of 7 dB; this was empirically picked to align
the curves the best with the reported data from the CMU testbed.  Although
a value of 1.55 dB would correspond to the reported -99 dBm noise floor
from the CMU paper, a noise figure of 7 dB results in the best fit with the
CMU experimental data.  This default of 7 dB is the RxNoiseFigure in the
``ns3::YansWifiPhy`` model.  Other values for noise figure will shift the 
curves leftward or rightward but not change the slope. 

The curves can be reproduced by running the ``wifi-clear-channel-cmu.cc`` 
example program in the ``examples/wireless`` directory, and the figure produced
(when GNU Scientific Library (GSL) is enabled) is reproduced below in 
Figure :ref:`fig-clear-channel-80211b`.

.. _fig-clear-channel-80211b:

.. figure:: figures/clear-channel.*
   :align: center

   Clear channel (AWGN) error model for 802.11b

Validation results for the 802.11 OFDM error model are available in this
`technical report <https://www.nsnam.org/~pei/80211ofdm.pdf>`__.  The curves
can be reproduced by running the ``ofdm-validation.cc`` example program
in the ``examples/wireless`` directory, and the figure is reproduced below
in Figure :ref:`fig-nist-frame-success-rate`.

.. _fig-nist-frame-success-rate:

.. figure:: figures/nist-frame-success-rate.*
   :align: center

   Frame error rate (NIST model) for OFDM Wi-Fi


MAC validation
**************
Validation of the MAC layer has been performed in [baldo2010]_.
