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

SpectrumWiFiPhy
***************

The SpectrumWifiPhy implementation has been verified to produce equivalent
results to the legacy YansWifiPhy by using the saturation and packet
error rate programs (described below) and toggling the implementation 
between the two physical layers.

A basic unit test is provided using injection of hand-crafted packets to
a receiving Phy object, controlling the timing and receive power of 
each packet arrival and checking the reception results.  However, most of
the testing of this Phy implementation has been performed using example
programs described below, and during the course of a (separate) LTE/Wi-Fi
coexistence study not documented herein.

Saturation performance
======================

The program ``examples/wireless/wifi-spectrum-saturation-example.cc``
allows user to select either the `SpectrumWifiPhy` or `YansWifiPhy` for
saturation tests.  The wifiType can be toggled by the argument
``'--wifiType=ns3::YansWifiPhy'`` or ``--wifiType=ns3::SpectrumWifiPhy'``

There isn't any difference in the output, which is to be expected because
this test is more of a test of the DCF than the physical layer.

By default, the program will use the `SpectrumWifiPhy` and will run
for 10 seconds of saturating UDP data, with 802.11n features enabled.
It produces this output for the main 802.11n rates (with short and long guard
intervals):

::

  wifiType: ns3::SpectrumWifiPhy distance: 1m
  index   MCS   width Rate (Mb/s) Tput (Mb/s) Received 
    0     0      20       6.5     5.96219    5063
    1     1      20        13     11.9491   10147
    2     2      20      19.5     17.9172   15215
    3     3      20        26     23.9241   20316
    4     4      20        39     35.9427   30522
    5     5      20        52     47.9283   40700
    6     6      20      58.5     53.7445   45639
    7     7      20        65     59.4629   50495
   ...
   63    15      40       300     240.884  204555


The above output shows the first 8 (of 32) modes, and last mode, that will be
output from the program.  The first 8 modes correspond
to short guard interval disabled and channel bonding disabled.  The
subsequent 24 modes run by this program are variations with short guard
interval enabled (cases 9-16), and then with channel bonding enabled and
short guard first disabled then enabled (cases 17-32).  Cases 33-64 repeat
the same configurations but for two spatial streams (MIMO abstraction).

When run with the legacy YansWifiPhy, as in ``./waf --run "wifi-spectrum-saturation-example --wifiType=ns3::YansWifiPhy"``, the same output is observed:

::

  wifiType: ns3::YansWifiPhy distance: 1m
  index   MCS   width Rate (Mb/s) Tput (Mb/s) Received 
      0     0      20       6.5     5.96219    5063
      1     1      20        13     11.9491   10147
      2     2      20      19.5     17.9172   15215
      3     3      20        26     23.9241   20316
      4     4      20        39     35.9427   30522
      5     5      20        52     47.9283   40700
      6     6      20      58.5     53.7445   45639

This is to be expected since YansWifiPhy and SpectrumWifiPhy use the 
same error rate model in this case.

Packet error rate performance
=============================

The program ``examples/wireless/wifi-spectrum-per-example.cc`` allows users
to select either `SpectrumWifiPhy` or `YansWifiPhy`, as above, and select
the distance between the nodes, and to log the reception statistics and
received SNR (as observed by the MonitorRx trace source), using a
Friis propagation loss model.  The transmit power is lowered from the default
of 40 mW (16 dBm) to 1 dBm to lower the baseline SNR; the distance between
the nodes can be changed to further change the SNR.  By default, it steps 
through the same index values as in the saturation example (0 through 31) 
for a 50m distance, producing output such as:

::

  wifiType: ns3::SpectrumWifiPhy distance: 50m; sent: 1000 TxPower: 1 dBm (1.3 mW)
  index   MCS Rate (Mb/s) Tput (Mb/s) Received Signal (dBm) Noise (dBm)  SNR (dB)
      0     0       6.5      0.7776    1000    -77.6633    -100.966     23.3027
      1     1        13      0.7776    1000    -77.6633    -100.966     23.3027
      2     2      19.5      0.7776    1000    -77.6633    -100.966     23.3027
      3     3        26      0.7776    1000    -77.6633    -100.966     23.3027
      4     4        39      0.7776    1000    -77.6633    -100.966     23.3027
      5     5        52           0       0         N/A         N/A         N/A
      6     6      58.5           0       0         N/A         N/A         N/A
      7     7        65           0       0         N/A         N/A         N/A
  
As in the above saturation example, running this program with YansWifiPhy
will yield identical output.

Interference performance
========================

The program ``examples/wireless/wifi-spectrum-per-interference.cc`` is based
on the previous packet error rate example, but copies over the
WaveformGenerator from the unlicensed LTE interferer test, to allow
users to inject a non-Wi-Fi signal (using the ``--waveformPower`` argument)
from the command line.  Another difference with respect to the packet
error rate example program is that the transmit power is set back to the
default of 40 mW (16 dBm).  By default, the interference generator is off,
and the program should behave similarly to the other packet error rate example,
but by adding small
amounts of power (e.g. ``--waveformPower=0.001``), one will start to observe
SNR degradation and frame loss.

Some sample output with default arguments (no interference) is:

::

  ./waf --run "wifi-spectrum-per-interference"

  wifiType: ns3::SpectrumWifiPhy distance: 50m; sent: 1000 TxPower: 16 dBm (40 mW)
  index   MCS Rate (Mb/s) Tput (Mb/s) Received Signal (dBm)Noi+Inf(dBm)  SNR (dB)
      0     0       6.5      0.7776    1000    -62.6427    -100.966     38.3233
      1     1        13      0.7776    1000    -62.6427    -100.966     38.3233
      2     2      19.5      0.7776    1000    -62.6427    -100.966     38.3233
      3     3        26      0.7776    1000    -62.6427    -100.966     38.3233
      4     4        39      0.7776    1000    -62.6427    -100.966     38.3233
      5     5        52      0.7776    1000    -62.6427    -100.966     38.3233
      6     6      58.5      0.7776    1000    -62.6427    -100.966     38.3233
    ...

while a small amount of waveform power will cause frame losses to occur at
higher order modulations, due to lower SNR:

::

  ./waf --run "wifi-spectrum-per-interference --waveformPower=0.001"

  wifiType: ns3::SpectrumWifiPhy distance: 50m; sent: 1000 TxPower: 16 dBm (40 mW)
  index   MCS Rate (Mb/s) Tput (Mb/s) Received Signal (dBm)Noi+Inf(dBm)  SNR (dB)
      0     0       6.5      0.7776    1000    -62.6427    -86.1031     23.4604
      1     1        13      0.7776    1000    -62.6427    -86.1031     23.4604
      2     2      19.5      0.7776    1000    -62.6427    -86.1032     23.4605
      3     3        26      0.7776    1000    -62.6427    -86.1031     23.4604
      4     4        39      0.7776    1000    -62.6427    -86.1032     23.4605
      5     5        52           0       0         N/A         N/A         N/A
      6     6      58.5           0       0         N/A         N/A         N/A
    ...

If ns3::YansWifiPhy is selected as the wifiType, the waveform generator will
not be enabled because only transmitters of type YansWifiPhy may be connected
to a YansWifiChannel.

The interference signal as received by the sending node is typically below 
the default -62 dBm CCA Mode 1 threshold in this example.  If it raises
above, the sending node will suppress all transmissions.

