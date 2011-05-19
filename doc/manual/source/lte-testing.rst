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

  \mathrm{SINR} = \frac{ P_\mathrm{signal} }{ P_\mathrm{noise} + \sum P_\mathrm{interference} }

The unit test program includes two test cases with random transmited signals and random interference signals. The signals are active during different periods of time. A chunk of time is a period of time during which :math:`n` signals are active (:math:`T_i`). A different SINR is calculated (:math:`\mathrm{SINR_i}`) for each chunk. The expected :math:`\mathrm{SINR}` is

.. math::

  {\mathrm{SINR}}_{total} = \frac{ \displaystyle\sum_{\forall\mathrm{chunks}} {\mathrm{SINR}}_i * T_i }{ T_{total} }

The test vectors are obtained by an Octave script that applies the described algorithm. The unit test program checks that the SINR calculation in downlink is performed correctly in the :cpp:class:`ns3::LtePhy` interface. It implements this interface in the :cpp:class:`ns3::LteTestUePhy` test class.

The test passes if the calculated values are equal to the test vector within a tolerance of :math:`10^{-7}`. The tolerance is meant to account for the approximation errors typical of floating point arithmetic.




SINR calculation in the Uplink
------------------------------

The test suite ``lte-uplink-sinr`` checks that the SINR calculation in uplink is performed correctly. The SINR in the uplink is calculated for each Resource Block assigned to data transmissions by dividing the power of the intended signal from the considered UE by the sum of the noise power plus all the transmissions on the same RB coming from other UEs belonging to different cells (the interference signals).

.. math::

  \mathrm{SINR} = \frac{ P_\mathrm{signal} }{ P_\mathrm{noise} + \sum P_\mathrm{interference} }

The unit test program includes two test cases with random transmited signals and random interference signals. The signals are active during different periods of time. A chunk of time is a period of time during which :math:`n` signals are active (:math:`T_i`). A different SINR is calculated (:math:`\mathrm{SINR_i}`) for each chunk. The expected :math:`\mathrm{SINR}` is

.. math::

  {\mathrm{SINR}}_{total} = \frac{ \displaystyle\sum_{\forall\mathrm{chunks}} {\mathrm{SINR}}_i * T_i }{ T_{total} }

The test vectors are obtained by an Octave script that applies the described algorithm. The unit test program checks that the SINR calculation in uplink is performed correctly in the :cpp:class:`ns3::LtePhy` interface. It implements this interface in the :cpp:class:`ns3::LteTestUePhy` test class.

The test passes if the calculated values are equal to the test vector within a tolerance of :math:`10^{-7}`. The tolerance is meant to account for the approximation errors typical of floating point arithmetic.


System Tests
~~~~~~~~~~~~

Adaptive Modulation and Coding
------------------------------

The test suite ``lte-link-adaptation`` provides system tests recreating a scenario with a single eNB and a single UE. Different test cases are created corresponding to different SINR values perceived by the UE. The aim of the test is to check that in each test case the chosen MCS corresponds to the values in a known test vector.

The test vector is obtained with the model described in [Piro2011]_ which cites [Seo2004]_. Here we described how this model works. We get the spectral efficiency :math:`\eta_i` for each value of the SINR in dB using Shannon's theorem:

.. math::

  \mathrm{BER} = 0.00005

.. math::

  \Gamma = \frac{ -\ln{ (5 * \mathrm{BER}) } }{ 1.5 }

.. math::

  \eta_i = \log_2 { \left( 1 + \frac{ {\mathrm{SINR}}_i }{ \Gamma } \right) }

Then, 3GPP [R1-081483]_ document (its XLS sheet annexed file) is used to get the corresponding MCS scheme. The spectral efficiency is quantized based on the CQI (rounding to the lowest value) and is mapped to the corresponding MCS scheme (i.e. the MCS index that appears on the same line looking at the MCS table on the right). Note that the quantization of the CQI is coarser than the spectral efficiency reported in the CQI table.

Finally, note that there are some discrepancies between the MCS index in [R1-081483]_ and that indicated by the standard: [TS36.213]_ Table 7.1.7.1-1 says that the MCS index goes from 0 to 31, and 0 appears to be a valid MCS scheme (TB size is not 0) but in [R1-081483]_ the first useful MCS index is 1. Hence to get the value as intended by the standard we need to subtract 1 from the index reported in [R1-081483]_.

The test passes if both the following conditions are verified:

 #. the SINR calculated by the UE corresponds to the value intended for the given test case within a tolerance of :math:`10^{-7}`.  The tolerance is meant to account for the approximation errors typical of floating point arithmetic. This test condition acts as a system test of the SINR calculation. This is needed because the other test suites that deal with the SINR calculation are unit tests, and hence might not be able to detect system-level bugs in the SINR calculation.
 #. the MCS index chosen by the scheduler matches exactly with the MCS index in the test vector, determined using the above described procedure.



Round Robin scheduler performance
---------------------------------

This system test program creates different test cases with a single eNB and several UEs, all having the same Radio Bearer specification. In each test case, the UEs see the same SINR from the eNB; different test cases are implemented by using different distance among UEs and the eNB (i.e., therefore having different SINR values) and different numbers of UEs. The test consists on checking that the obtained throughput performance is equal among users and matches a reference throughput value obtained according to the SINR perceived within a given tolerance.
The test vector is obtained according to the values of transport block size reported in table 7.1.7.2.1-1 of [TS36.213]_, considering an equal distribution of the physical resource block among the users using Resource Allocation Type 0 as defined in Section 7.1.6.1 of [TS36.213]_.  Let :math:`\tau` be the TTI duration, :math:`N` be the number of UEs, :math:`B` the transmission bandwidth configuration in number of RBs, :math:`G` the RBG size, :math:`M` the modulation and coding scheme in use at the given SINR and :math:`S(M, B)` be the transport block size in bits as defined by 3GPP TS 36.213. We first calculate the number :math:`L` of RBGs allocated to each user as

.. math::

   L = \left\lfloor \frac{B}{NG} \right\rfloor 

The reference throughput (in bytes per second) :math:`T` in bps achieved by each UE is then calculated as

.. math::

   T =  \frac{S(M, L G)}{8 \; \tau}




Proportional Fair scheduler performance
---------------------------------------

This system test program creates different test cases with a single eNB, using the Proportional Fair (PF) scheduler, and several UEs, all having the same Radio Bearer specification. The test cases are grouped in two categories in order to evaluate both the performance in terms of adaptation to channel condition and from fairness perspective.

In the first category of test cases, the UEs are all placed at the same distance from the eNB, and hence all placed in order to have the same SINR. Different test cases are implemented by using a different SINR value and a different number of UEs. The test consists on checking that the obtained throughput performance matches with the known reference throughput up to a given tolerance. The expected behavior of the PF scheduler when all UEs have the same SNR is that each UE should get an equal fraction of the throughput obtainable by a single UE when using all the resources. We calculate the reference throughput value by dividing the throughput achievable by a single UE at the given SNR by the total number of UEs. Let :math:`\tau` be the TTI duration, :math:`B` the transmission bandwidth configuration in number of RBs, :math:`M` the modulation and coding scheme in use at the given SINR and :math:`S(M, B)` be the transport block size as defined in [TS36.213]_. The reference throughput :math:`T` achieved by each UE is calculated as

.. math::

   T = \frac{S(M,B)}{\tau N}

The second category of tests is aimed at verify the fairness behavior in presence of UEs with different SINRs (and therefore different estimated throughput from PFS). The test consists of checking whether the throughput obtained by each UE over the whole simulation matches with the steady-state throughput expected by the PF scheduler according to the theory (see for instance [Kushner2004]_).
Let :math:`Ri` the estimation done by PFS of the throughput of the :math:`i` UE for the next TTI according to the CQIs received and :math:`Ti` the throughput preceived by the :math:`i` UE . The test verifies that the ratio of the obtained throughput value respect to the global one (i.e. the sum of the ones of all users) is equal to the steady-state throughput of the PFS, that is

.. math::

   K = \frac{Ri}{\sum_{k=1}^N Ri} = \frac{Ti}{\sum_{k=1}^N Ti}



References
**********

.. [TS36.213] 3GPP TS 36.213 "LTE Physical layer procedures"

.. [Kushner2004] H.J. Kushner and P.A. Whiting, "Convergence of proportional-fair sharing algorithms under general conditions", IEEE Trans. on Wireless Communications, July 2004

.. [Piro2011] G. Piro, N. Baldo. M. Miozzo, "An LTE module for the ns-3 network simulator", Wns3 2011
              (in conjunction with SimuTOOLS 2011), March 2011, Barcelona (Spain)

.. [Seo2004] H. Seo, B. G. Lee. "A proportional-fair power allocation scheme for fair and efficient multiuser
             OFDM systems", In Proc. of IEEE GLOBECOM, December 2004. Dallas (USA)

.. [R1-081483] 3GPP R1-081483 "Conveying MCS and TB size via PDCCH"
