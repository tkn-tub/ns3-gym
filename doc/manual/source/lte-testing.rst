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

Proportional Fair scheduler performance
---------------------------------------
