.. include:: replace.txt


+++++++++++++++++++++++++++
 Testing Documentation
+++++++++++++++++++++++++++


Overview
********

To test and validate the ns-3 LTE module, several test suites are provided which are integrated with the ns-3 test framework.
To run them, you need to have configured the build of the simulator in this way::

    ./waf configure --enable-tests --enable-modules=lte --enable-examples
    ./test.py

The above will run not only the test suites belonging to the LTE module, but also those belonging to all the other ns-3 modules on which the LTE module depends. See the ns-3 manual for generic information on the testing framework.

You can get a more detailed report in HTML format in this way::

    ./test.py -w results.html

After the above command has run, you can view the detailed result for each test by opening the file ``results.html`` with a web browser. 

You can run each test suite separately using this command::

    ./test.py -s test-suite-name

For more details about ``test.py`` and the ns-3 testing framework, please refer to the ns-3 manual.



Description of the test suites
******************************

Unit Tests
~~~~~~~~~~

SINR calculation in the Downlink
--------------------------------

The test suite ``lte-downlink-sinr`` 
checks that the SINR calculation in
downlink is performed correctly. The SINR in the downlink is calculated for each
RB assigned to data transmissions by dividing the power of the
intended signal from the considered eNB by the sum of the noise power plus all
the transmissions on the same RB coming from other eNBs (the interference
signals):

.. math::

  \gamma = \frac{ P_\mathrm{signal} }{ P_\mathrm{noise} + \sum P_\mathrm{interference} }

In general, different signals can be active during different periods
of time. We define a *chunk* as the time interval between any two
events of type either start or end of a waveform. In other words, a
chunk identifies a time interval during which the set of active
waveforms does not change. Let :math:`i` be the generic chunk,
:math:`T_i` its duration and :math:`\mathrm{SINR_i}` its SINR,
calculated with the above equation. The calculation of the average
SINR :math:`\overline{\gamma}` to be used for CQI feedback reporting
uses the following formula:  

.. math::

  \overline{\gamma} = \frac{ \sum_i {\gamma}_i  T_i }{ \sum_i T_{i} }

The test suite checks that the above calculation is performed
correctly in the simulator. The test vectors are obtained offline by
an Octave script that implements the above equation, and that
recreates a number of random transmitted signals and interference
signals that mimic a scenario where an UE is trying to decode a signal
from an eNB while facing interference from other eNBs. The test passes
if the calculated values are equal to the test vector within a
tolerance of :math:`10^{-7}`. The tolerance is meant to account for
the approximation errors typical of floating point arithmetic. 



SINR calculation in the Uplink
------------------------------

The test suite ``lte-uplink-sinr`` checks that the SINR calculation in
uplink is performed correctly. This test suite is identical to
``lte-downlink-sinr`` described in the previous section, with the
difference than both the signal and the interference now refer to
transmissions by the UEs, and reception is performed by the eNB. 
This test suite recreates a number of random transmitted signals and
interference signals to mimic a scenario where an eNB is trying to
decode the signal from several UEs simultaneously (the ones in the
cell of the eNB) while facing interference from other UEs (the ones
belonging to other cells).  

The test vectors are obtained by a dedicated Octave script. The test
passes if the calculated values are equal to the test vector within a
tolerance of :math:`10^{-7}` which, as for the downlink SINR test,
deals with floating point arithmetic approximation issues. 


System Tests
~~~~~~~~~~~~

.. _sec-lte-amc-tests:

Adaptive Modulation and Coding Tests
------------------------------------

The test suite ``lte-link-adaptation`` provides system tests recreating a
scenario with a single eNB and a single UE. Different test cases are created
corresponding to different SNR values perceived by the UE. The aim of the test
is to check that in each test case the chosen MCS corresponds to some known
reference values. These reference values are obtained by
re-implementing in Octave (see `src/lte/test/reference/lte_amc.m`) the
model described in Section :ref:`sec-lte-amc` for the calculation of the
spectral efficiency, and determining the corresponding MCS index
by manually looking up the tables in [R1-081483]_. The resulting test vector is
represented in Figure :ref:`fig-lte-mcs-index`.

The MCS which is used by the simulator is measured by
obtaining the tracing output produced by the scheduler after 4ms (this
is needed to account for the initial delay in CQI reporting). The SINR
which is calcualted by the simulator is also obtained using the
``LteSinrChunkProcessor`` interface. The test
passes if both the following conditions are satisfied:
 
 #. the SINR calculated by the simulator correspond to the SNR
    of the test vector within an absolute tolerance of :math:`10^{-7}`;
 #. the MCS index used by the simulator exactly corresponds to
    the one in the test vector.

.. _fig-lte-mcs-index:

.. figure:: figures/lte-mcs-index.*
   :align: center

   Test vector for Adaptive Modulation and Coding



Inter-cell Interference Tests
-----------------------------

The test suite `lte-interference`` provides system tests recreating an
inter-cell interference scenario with two eNBs, each having a single
UE attached to it and employing Adaptive Modulation and Coding both in
the downlink and in the uplink. The topology of the scenario
is depicted in Figure :ref:`fig-lte-interference-test-scenario`. The
:math:`d_1` parameter represents the distance of each UE to the eNB it
is attached to, whereas the :math:`d_2` parameter represent the
interferer distance. We note that the scenario topology is such that
the interferer distance is the same for uplink and downlink; still,
the actual interference power perceived will be different, because of
the different propagation loss in the uplink and downlink
bands. Different test cases are obtained by varying the :math:`d_1`
and :math:`d_2` parameters.


.. _fig-lte-interference-test-scenario:

.. figure:: figures/lte-interference-test-scenario.*
   :align: center

   Topology for the inter-cell interference test

The test vectors are obtained by use of a dedicated octave script
(available in
`src/lte/test/reference/lte_link_budget_interference.m`), which does
the link budget calculations (including interference) corresponding to the topology of each
test case, and outputs the resulting SINR and spectral efficiency. The
latter is then used to determine (using the same procedure adopted for 
:ref:`sec-lte-amc-tests`. We note that the test vector
contains separate values for uplink and downlink. 



Round Robin scheduler performance
---------------------------------

The test suite ``lte-rr-ff-mac-scheduler`` creates different test cases with
a single eNB and several UEs, all having the same Radio Bearer specification. In
each test case, the UEs see the same SINR from the eNB; different test cases are
implemented by using different distance among UEs and the eNB (i.e., therefore
having different SINR values) and different numbers of UEs. The test consists on
checking that the obtained throughput performance is equal among users and
matches a reference throughput value obtained according to the SINR perceived
within a given tolerance. 

The test vector is obtained according to the values of transport block
size reported in table 7.1.7.2.1-1 of [TS36.213]_, considering an
equal distribution of the physical resource block among the users
using Resource Allocation Type 0 as defined in Section 7.1.6.1 of
[TS36.213]_.  Let :math:`\tau` be the TTI duration, :math:`N` be the
number of UEs, :math:`B` the transmission bandwidth configuration in
number of RBs, :math:`G` the RBG size, :math:`M` the modulation and
coding scheme in use at the given SINR and :math:`S(M, B)` be the
transport block size in bits as defined by 3GPP TS 36.213. We first
calculate the number :math:`L` of RBGs allocated to each user as 

.. math::

   L = \left\lfloor \frac{B}{NG} \right\rfloor 

The reference throughput :math:`T` in bit/s achieved by each UE is then calculated as

.. math::

   T =  \frac{S(M, L G)}{8 \; \tau}

The test passes if the measured throughput matches with the reference throughput
within a relative tolerance of 0.1. This tolerance is needed to account for the
transient behavior at the beginning of the simulation (e.g., CQI feedback is
only available after a few subframes) as well as for the accuracy of the
estimator of the average throughput performance over the chosen simulation time
(0.4s). This choice of the simulation time is justified by the need to
follow the ns-3 guidelines of keeping the total execution time of the test
suite low, in spite of the high number of test cases. In any case, we note that
a lower value of the tolerance can be used when longer simulations are
run. 

In Figure `fig-lenaThrTestCase1`_, the curves labeled "RR" represent the test values
calculated for the RR scheduler test, as a function of the number of UEs and of
the MCS index being used in each test case. 

.. _fig-lenaThrTestCase1:

.. figure:: figures/lenaThrTestCase1.*                 
   :align: center

   Test vectors for the RR and PF Scheduler in the downlink in a
   scenario where all UEs use the same MCS.

Proportional Fair scheduler performance
---------------------------------------

The test suite ``lte-pf-ff-mac-scheduler`` creates different test cases with
a single eNB, using the Proportional Fair (PF) scheduler, and several UEs, all
having the same Radio Bearer specification. The test cases are grouped in two
categories in order to evaluate the performance both in terms of the adaptation
to the channel condition and from a fairness perspective. 

In the first category of test cases, the UEs are all placed at the
same distance from the eNB, and hence all placed in order to have the
same SINR. Different test cases are implemented by using a different
SINR value and a different number of UEs. The test consists on
checking that the obtained throughput performance matches with the
known reference throughput up to a given tolerance. The expected
behavior of the PF scheduler when all UEs have the same SNR is that
each UE should get an equal fraction of the throughput obtainable by a
single UE when using all the resources. We calculate the reference
throughput value by dividing the throughput achievable by a single UE
at the given SNR by the total number of UEs. 
Let :math:`\tau` be the TTI duration, :math:`B` the transmission
bandwidth configuration in number of RBs, :math:`M` the modulation and
coding scheme in use at the given SINR and :math:`S(M, B)` be the
transport block size as defined in [TS36.213]_. The reference
throughput :math:`T` in bit/s achieved by each UE is calculated as 

.. math::

   T = \frac{S(M,B)}{\tau N}

The curves labeled "PF" in Figure `fig-lenaThrTestCase1`_ represent the test values
calculated for the PF scheduler tests of the first category, that we just described.

The second category of tests aims at verifying the fairness of the PF
scheduler in a more realistic simulation scenario where the UEs have a
different SINR (constant for the whole simulation). In these conditions, the PF
scheduler will give to each user a share of the system bandwidth that is
proportional to the capacity achievable by a single user alone considered its
SINR. In detail, let :math:`M_i` be the modulation and coding scheme being used by
each UE (which is a deterministic function of the SINR of the UE, and is hence
known in this scenario). Based on the MCS, we determine the achievable
rate :math:`R_i` for each user :math:`i` using the
procedure described in Section~\ref{sec:pfs}. We then define the
achievable rate ratio :math:`\rho_{R,i}` of each user :math:`i` as

.. math::
   \rho_{R,i} = \frac{R_i}{\sum_{j=1}^N R_j}

Let now :math:`T_i` be the throughput actually achieved by the UE :math:`i` , which
is obtained as part of the simulation output. We define the obtained throughput
ratio :math:`\rho_{T,i}` of UE :math:`i` as

.. math::
   \rho_{T,i} = \frac{T_i}{\sum_{j=1}^N T_j}

The test consists of checking that the following condition is verified:

.. math::
   \rho_{R,i} = \rho_{T,i}

if so, it means that the throughput obtained by each UE over the whole
simulation matches with the steady-state throughput expected by the PF scheduler
according to the theory. This test can be derived from [Holtzman2000]_
as follows. From Section 3 of [Holtzman2000]_, we know that 

.. math::
   \frac{T_i}{R_i} = c, \, \forall i

where :math:`c` is a constant. By substituting the above into the
definition of :math:`\rho_{T,i}` given previously, we get

.. math::
    \frac{T_i}{\sum_{j=1}^N T_j} &=  \frac{c R_i}{\sum_{j=1}^N c R_j} \\
                                 &=  \frac{c R_i}{c \sum_{j=1}^N  R_j} \\
                                 &=  \frac{R_i}{\sum_{j=1}^N  R_j}

which is exactly the expression being used in the test.
              


Figure :ref:`fig-lenaThrTestCase2` presents the results obtained in a test case with
UEs :math:`i=1,\dots,5` that are located at a distance from the base
station such that they will use respectively the MCS index :math:`28, 24, 16, 12,
6`. From the figure, we note that, as expected, the obtained throughput is
proportional to the achievable rate. In other words, the PF scheduler assign
more resources to the users that use a higher MCS index.


.. _fig-lenaThrTestCase2:

.. figure:: figures/lenaThrTestCase2.*
   :align: center

   Throughput ratio evaluation for the PF scheduler in a scenario
   where the UEs have MCS index :math:`28, 24, 16, 12, 6`
