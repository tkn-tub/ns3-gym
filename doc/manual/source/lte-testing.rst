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
The test vector is obtained according to the values of transport block size reported in table 7.1.7.2.1-1 of 36.213, considering an equal distribution of the physical resource block among the users. The tolerance has been introduced in order to overcome the process of initialization and quantization of the resource allocation.


Proportional Fair scheduler performance
---------------------------------------

This system test program creates different test cases with a single eNB and several UEs, all having the same Radio Bearer specification. The tests are grouped in two categories in order to evaluate both the performance in terms of adaptation to channel condition and from fairness perspective.
Regarding the former, the UEs are placed in order to have the same SINR from the eNB in each specific test. Different test cases are implemented by using different SINR values and different numbers of UEs. The test consists on checking that the obtained throughput performance is equal among users is consistent with the definition of proportional fair scheduling, which implies that the bandwidht assigned is proportional to the ratio of the estimated throughput per TTI of each user. In detail, each TTI the PFS estimates the throughput of each UE for the next TTI according to the CQIs received; the ratio of this value respect to the global of all users represent the long term trend for the bandwidht allocated by the scheduler. In this specific case, this results in allocating proportionally the maximum bandwidht achievable (i.e., all PRB allocated to a single users) among the users in a time division fashion.
The second category of tests is aimed at verify the fairness behavior in presence of UEs with different SINRs (and therefore different estimated throughput from PFS). The test consists of checking whether the throughput obtained is fairly distributed among the users, that is the ratio of the estimated throughput per TTI (function of the SINR).
