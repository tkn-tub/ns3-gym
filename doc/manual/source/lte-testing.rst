.. include:: replace.txt


+++++++++++++++++++++++++++
 LTE Testing Documentation
+++++++++++++++++++++++++++


Overview
********

The tests for the ns-3 LTE module are integrated with the ns-3 test framework.
To run them, you need to have configured the build of the simulator in this way::

    ./waf configure --enable-tests --enable-modules=lte --enable-examples
    ./test.py

The above will run not only the test suites belonging to the LTE module, but also those belonging to all the other ns-3 modules on which the LTE module depends. See the ns-3 manual for generic information on the testing framework.

You can get a more detailed report in HTML format in this way::

    ./test.py -w results.html

After the above command has run, you can view the detailed result for each test by opebning the file ``results.html`` with a web browser.




Description of the tests
************************

Unit Tests
~~~~~~~~~~

SINR calculation in the Downlink
--------------------------------

SINR calculation in the uplink
------------------------------


System Tests
~~~~~~~~~~~~

Adaptive Modulation and Coding
------------------------------

Round Robin scheduler performance
---------------------------------

This system test program creates different test cases with a single eNB and several UEs, all having the same Radio Bearer specification. In each test case, the UEs see the same SINR from the eNB; different test cases are implemented by using different distance among UEs and the eNB (i.e., therefore having different SINR values) and different numbers of UEs. The test consists on checking that the obtained throughput performance is equal among users and matches a reference throughput value obtained according to the SINR perceived within a given tolerance.
The test vector is obtained according to the values of transport block size reported in table 7.1.7.2.1-1 of [TS36.213]_, considering an equal distribution of the physical resource block among the users using Resource Allocation Type 0 as defined in Section 7.1.6.1 of [TS36.213]_.  Let :math:`\tau` be the TTI duration, :math:`N` be the number of UEs, :math:`B` the transmission bandwidth configuration in number of RBs, :math:`G` the RBG size, :math:`M` the modulation and coding scheme in use at the given SINR and :math:`S(M, B)` be the transport block size in bytes as defined by 3GPP TS 36.213. We first calculate the number :math:`L` of RBGs allocated to each user as

.. math::

   L = \left\lfloor \frac{B}{NG} \right\rfloor 

The reference throughput :math:`T` in bps achieved by each UE is then calculated as

.. math::

   T =  \frac{8 \; S(M, L G)}{\tau}




Proportional Fair scheduler performance
---------------------------------------

This system test program creates different test cases with a single eNB, using the Proportional Fair (PF) scheduler, and several UEs, all having the same Radio Bearer specification. The test cases are grouped in two categories in order to evaluate both the performance in terms of adaptation to channel condition and from fairness perspective.

In the first category of test cases, the UEs are all placed at the same distance from the eNB, and hence all placed in order to have the same SINR. Different test cases are implemented by using a different SINR value and a different number of UEs. The test consists on checking that the obtained throughput performance matches with the known reference throughput up to a given tolerance. The expected behavior of the PF scheduler when all UEs have the same SNR is that each UE should get an equal fraction of the throughput obtainable by a single UE when using all the resources. We calculate the reference throughput value by dividing the throughput achievable by a single UE at the given SNR by the total number of UEs. Let :math:`\tau` be the TTI duration, :math:`B` the transmission bandwidth configuration in number of RBs, :math:`M` the modulation and coding scheme in use at the given SINR and :math:`S(M, B)` be the transport block size as defined in [TS36.213]_. The reference throughput :math:`T` achieved by each UE is calculated as

.. math::

   T = \frac{S(M,B)}{\tau N}



The second category of tests is aimed at verify the fairness behavior in presence of UEs with different SINRs (and therefore different estimated throughput from PFS). The test consists of checking whether the throughput obtained by each UE over the whole simulation matches with the steady-state throughput expected by the PF scheduler according to the theory (see for instance [Kushner2004cpfsa]_). 
In detail, each TTI the PFS estimates the throughput of each UE for the next TTI according to the CQIs received; the ratio of this value respect to the global of all users represent the long term trend for the bandwidht allocated by the scheduler. In this specific case, this results in allocating proportionally the maximum bandwidht achievable (i.e., all PRB allocated to a single users) among the users in a time division fashion.



References
**********

.. [TS36.213] 3GPP TS 36.213 "LTE Physical layer procedures"

.. [Kushner2004cpfsa] H.J. Kushner and P.A. Whiting, "Convergence of proportional-fair sharing algorithms under general conditions", IEEE Trans. on Wireless Communications, July 2004 
