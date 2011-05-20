.. include:: replace.txt


+++++++++++++++++++++++++++
 LTE Testing Documentation
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

The test suite ``lte-downlink-sinr`` checks that the SINR calculation in downlink is performed correctly. The SINR in the downlink is calculated for each Resource Block assigned to data transmissions by dividing the power of the intended signal from the considered eNB by the sum of the noise power plus all the transmissions on the same RB coming from other eNBs (the interference signals).

.. math::

  \gamma = \frac{ P_\mathrm{signal} }{ P_\mathrm{noise} + \sum P_\mathrm{interference} }

In general, different signals can be active during different periods of time. We define a *chunk* as the time interval between any two events of type either start or end of a waveform. In other words, a chunk identifies a time interval during which the set of active waveforms does not change. Let :math:`i` be the generic chunk, :math:`T_i` its duration and :math:`\mathrm{SINR_i}` its SINR, calculated with the above equation. The calculation of the average SINR :math:`\overline{\gamma}` to be used for CQI feedback reporting uses the following formula:

.. math::

  \overline{\gamma} = \frac{ \sum_i {\gamma}_i  T_i }{ \sum_i T_{i} }

The test suite checks that the above calculation is performed correctly in the simulator. The test vectors are obtained offline by an Octave script that implements the above equation, and that recreates a number of random transmitted signals and interference signals that mimic a scenario where an UE is trying to decode a signal from an eNB while facing interference from other eNBs. The test passes if the calculated values are equal to the test vector within a tolerance of :math:`10^{-7}`. The tolerance is meant to account for the approximation errors typical of floating point arithmetic.



SINR calculation in the Uplink
------------------------------

The test suite ``lte-uplink-sinr`` checks that the SINR calculation in uplink is performed correctly. This test suite is identical to ``lte-downlink-sinr`` described in the previous section, with the difference than both the signal and the interference now refer to transmissions by the UEs, and reception is performed by the eNB.
This test suite recreates a number of random transmitted signals and interference signals to mimic a scenario where an eNB is trying to decode the signal from several UEs simultaneously (the ones in the cell of the eNB) while facing interference from other UEs (the ones belonging to other cells). 

The test vectors are obtained by a dedicated Octave script. The test passes if the calculated values are equal to the test vector within a tolerance of :math:`10^{-7}` which, as for the downlink SINR test, deals with floating point arithmetic approximation issues.


System Tests
~~~~~~~~~~~~

Adaptive Modulation and Coding
------------------------------

The test suite ``lte-link-adaptation`` provides system tests recreating a scenario with a single eNB and a single UE. Different test cases are created corresponding to different SINR values perceived by the UE. The aim of the test is to check that in each test case the chosen MCS corresponds to the values in a known test vector.

The test vector is obtained with the model described in [Piro2011]_ which cites [Seo2004]_. Here we described how this model works. Let :math:`i` denote the generic user, and let :math:`\gamma_i` be its SINR. We get the spectral efficiency :math:`\eta_i` of user :math:`i` using the following equations:

.. math::

  \mathrm{BER} = 0.00005

.. math::

  \Gamma = \frac{ -\ln{ (5 * \mathrm{BER}) } }{ 1.5 }

.. math::

  \eta_i = \log_2 { \left( 1 + \frac{ {\gamma}_i }{ \Gamma } \right) }

Then, 3GPP [R1-081483]_ document (its XLS sheet annexed file) is used to get the corresponding MCS scheme. The spectral efficiency is quantized based on the CQI (rounding to the lowest value) and is mapped to the corresponding MCS scheme (i.e. the MCS index that appears on the same line looking at the MCS table on the right). Note that the quantization of the CQI is coarser than the spectral efficiency reported in the CQI table.

Finally, note that there are some discrepancies between the MCS index in [R1-081483]_ and that indicated by the standard: [TS36.213]_ Table 7.1.7.1-1 says that the MCS index goes from 0 to 31, and 0 appears to be a valid MCS scheme (TB size is not 0) but in [R1-081483]_ the first useful MCS index is 1. Hence to get the value as intended by the standard we need to subtract 1 from the index reported in [R1-081483]_.

The test passes if both the following conditions are verified:

 #. the SINR calculated by the UE corresponds to the value intended for the given test case within a tolerance of :math:`10^{-7}`.  The tolerance is meant to account for the approximation errors typical of floating point arithmetic. This test condition acts as a system test of the SINR calculation. This is needed because the other test suites that deal with the SINR calculation are unit tests, and hence might not be able to detect system-level bugs in the SINR calculation.
 #. the MCS index chosen by the scheduler matches exactly with the MCS index in the test vector, determined using the above described procedure.

In the following figure, we see the modulation to be used depending on the MCS index. This mapping is defined in [R1-081483]_.

.. figure:: figures/lte-mcs-index.*
   :align: center

   Modulation depending on the SINR and MCS index



Round Robin scheduler performance
---------------------------------

The test suite ``lte-rr-ff-mac-scheduler`` creates different test cases with a single eNB and several UEs, all having the same Radio Bearer specification. In each test case, the UEs see the same SINR from the eNB; different test cases are implemented by using different distance among UEs and the eNB (i.e., therefore having different SINR values) and different numbers of UEs. The test consists on checking that the obtained throughput performance is equal among users and matches a reference throughput value obtained according to the SINR perceived within a given tolerance.
The test vector is obtained according to the values of transport block size reported in table 7.1.7.2.1-1 of [TS36.213]_, considering an equal distribution of the physical resource block among the users using Resource Allocation Type 0 as defined in Section 7.1.6.1 of [TS36.213]_.  Let :math:`\tau` be the TTI duration, :math:`N` be the number of UEs, :math:`B` the transmission bandwidth configuration in number of RBs, :math:`G` the RBG size, :math:`M` the modulation and coding scheme in use at the given SINR and :math:`S(M, B)` be the transport block size in bits as defined by 3GPP TS 36.213. We first calculate the number :math:`L` of RBGs allocated to each user as

.. math::

   L = \left\lfloor \frac{B}{NG} \right\rfloor 

The reference throughput (in bytes per second) :math:`T` in bps achieved by each UE is then calculated as

.. math::

   T =  \frac{S(M, L G)}{8 \; \tau}

The test passes if the measured throughput matches with the reference throughput within a relative tolerance of 0.1. This tolerance is needed to account for the transient behavior at the beginning of the simulation (e.g., CQI feedback is only available after a few subframes) as well as for the accuracy of the estimator of the average throughput performance over the chosen simulation time (0.04s). We note that a lower value of the tolerance can be achieved by making each test case running longer simulations; we chose not to do this in order to keep the total execution time of this test suite as low as possible in spite of the high number of test cases. 



Proportional Fair scheduler performance
---------------------------------------

The test suite ``lte-pf-ff-mac-scheduler`` creates different test cases with a single eNB, using the Proportional Fair (PF) scheduler, and several UEs, all having the same Radio Bearer specification. The test cases are grouped in two categories in order to evaluate both the performance in terms of adaptation to channel condition and from fairness perspective.

In the first category of test cases, the UEs are all placed at the same distance from the eNB, and hence all placed in order to have the same SINR. Different test cases are implemented by using a different SINR value and a different number of UEs. The test consists on checking that the obtained throughput performance matches with the known reference throughput up to a given tolerance. The expected behavior of the PF scheduler when all UEs have the same SNR is that each UE should get an equal fraction of the throughput obtainable by a single UE when using all the resources. We calculate the reference throughput value by dividing the throughput achievable by a single UE at the given SNR by the total number of UEs. Let :math:`\tau` be the TTI duration, :math:`B` the transmission bandwidth configuration in number of RBs, :math:`M` the modulation and coding scheme in use at the given SINR and :math:`S(M, B)` be the transport block size as defined in [TS36.213]_. The reference throughput :math:`T` achieved by each UE is calculated as

.. math::

   T = \frac{S(M,B)}{\tau N}

The second category of tests is aimed at verify the fairness behavior in presence of UEs with different SINRs (and therefore different estimated throughput from PFS). The test consists of checking whether the throughput obtained by each UE over the whole simulation matches with the steady-state throughput expected by the PF scheduler according to the theory (see for instance [Kushner2004]_).
Let :math:`Ri` the estimation done by PFS of the throughput of the :math:`i` UE for the next TTI according to the CQIs received and :math:`Ti` the throughput preceived by the :math:`i` UE . The test verifies that the ratio of the obtained throughput value respect to the global one (i.e. the sum of the ones of all users) is equal to the steady-state throughput of the PFS, that is

.. math::

   K = \frac{Ri}{\sum_{k=1}^N Ri} = \frac{Ti}{\sum_{k=1}^N Ti}

The test passes if the measured throughput matches with the reference throughput within a relative tolerance of 0.1. The choice of this tolerance has the same motivations already discussed for the Round Robin scheduler test suite.


