.. include:: replace.txt


+++++++++++++++++++++++++++++++++
 Profiling Documentation
+++++++++++++++++++++++++++++++++



Execution time and memory consumption
-------------------------------------

In order to provide an evaluation of the execution time and
memory consumption, a
reference simulation program (``examples/profiling-reference``) has been
developed. This program simulates a scenario
composed by a set of eNodeBs, and a set of UEs attached to each eNB. All eNodeBs
have the same number of attached UEs. Communications are performed both in the
dowlink and in the uplink using a saturation model (i.e., each RLC instance
always has a PDU to transmit). The UEs are all in the same position than its
eNodeB and the eNodeBs are distributed in a line, each one 140m away from the
previous one. The total simulation time is set to 60s. 

Using this simulation program, we ran a simulation campaign varying the number
of eNBs as well as the number of UEs per eNB. For each simulation, we measured
the execution time using the ``time`` shell command in linux, and the memory
consumption by looking at the information in ``/proc/\{pid\}/statm``. The
reference hardware platform is an Intel Core2 Duo E8400 3.00GHz with 512 MB of
RAM memory running a Fedora Core 10 distribution with kernel
2.6.27.5. The simulator build used in this 
experiment was configured with the options ``-d optimized
--enable-static``.   

The results are reported in `fig-simulationTime`_ and `fig-memoryUsage`_. 
We note that the memory usage, as expected,
primarily depends on the number of eNBs, however is in general quite low. The
execution time depends significantly on both the number of eNBs and the number
of UEs per eNB. For the case of 10 UEs per eNB, we also show that the
experimental data can be fitted quite accurately by a quadratic function. We
suggest that this behavior is due to the fact that the 
interference calculations have a computational complexity which is quadratic with
respect to the number of eNBs, and which is the dominant contribution in the overall
computational load.


.. _fig-simulationTime:

.. figure:: figures/simulationTime.*                 
   :align: center

   Execution time of the reference program for a simulation duration of 60s.


.. _fig-memoryUsage:

.. figure:: figures/memoryUsage.*                 
   :align: center

   Memory usage of the reference program.

