.. include:: replace.txt


+++++++++++++++++++++++++++++++++
 Profiling Documentation
+++++++++++++++++++++++++++++++++


Overview and objectives
***********************

The main profiling objective is to assess the simulator performance on a broad set of scenarios. This evaluation provides reference values for simulation running times and memory consumption figures. It also helps to identify potential performance improvements and to check for scalability problems when increasing the number of eNodeB and the number of UEs attached to each eNodeB.

In the following sections, a detailed description of the general profiling framework employed to perform this profiling study is introduced. It also introduces the details of the main tests performed and its results are evaluated.

Framework description
*********************

Simulation script
~~~~~~~~~~~~~~~~

The simulation script used for all the results gathered in this documentation is located at ``src/lte/examples/lena-runtime-profiler.cc``. This script generated a squared grid topology, placing a eNodeB at the centre of the square. UEs attached to this node are scattered randomly across the square (using a random uniform distribution along X and Y axis). If *BuildingPropagationModel* is used, the squares are replaced by rooms.

.. TODO: Insert figure picturing the geometry of the simulation.

RLC and MAC traces are enabled for all UEs and all eNodeBs and those traces are written to disk directly. The MAC scheduler used is *propotional fair*.

Simulation input parameters
---------------------------

The *lena-runtime-profiler* simulation script accepts the following input parameters: 
* ``simTime``: time to simulate (in seconds)
* ``nUe``: number of UEs attached to each eNodeB 
* ``nEnb``: number of eNodeB composing the grid per floor
* ``nFloors``: number of floors, 0 for *Friis propagation model* (no walls), 1 for *Building propagation model*
* ``traceDirectory``: destination directory where simulation traces will be stored

Time measurement
~~~~~~~~~~~~~~~~

Running time is measured using default Linux shell command **time**. This command counts how much user time the execution of a program takes.

Perl script
~~~~~~~~~~~

To simplify the process of running the profiling script for a wide range of values and collecting its timing data, a simple Perl script to automate the complete process is provided. It is placed in ``src/lte/test/lte-test-run-time.pl``. It simply runs a batch of simulations with a range of parameters and stores the timing results in a CSV file called *times.csv*. The range of values each parameter takes can be easily modified editing the script.

Requirements
------------

The following Perl modules are required to use the provided script, all of them available from CPAN:
* IO::CaptureOutput 
* Statistics::Descriptive
* Cwd

Reference software and equipment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

All timing tests had been run in a Intel Pentium IV 3.00 GHz machine with 512 Mb of RAM memory running Fedora Core 10 with a 2.6.27.41-170.2.117 kernel, storing the traces directly to the hard disk.

Also, as a reference configuration, the build has been configured static and optimized. The exact ``waf`` command issued is:

``CXXFLAGS="-O3 -w" ./waf -d optimized configure --enable-static --enable-examples``


Results
*******

The following results and figures had been obtained with LENA **changeset 1823cde6f40b**.

Running time
~~~~~~~~~~~~

This scenario, evaluates the running time in for a fixed simulation time (10s) and Friis propagation mode increasing the number of UEs attached to each eNodeB and the number of planted eNodeBs in every scenario.

.. _fig-runTime:

.. figure:: figures/runningTime10s.*                 
   :align: center
   :width: 60%

   Running time



Propagation model
~~~~~~~~~~~~~~~~~

The objective of this scenario is to evaluate the impact of the more complex building propagation model, so a set of scenarios has been run with Friis propagation model and Building propagation model and its run time are compared in the following figure.

.. _fig-runTime:

.. figure:: figures/propagationModel.*                 
   :align: center
   :width: 60%

   Propagation Model



Simulation time
~~~~~~~~~~~~~~~

In this scenario, for a fixed set of UEs per eNodeB, different simulation times had been run. As the simulation time increases, running time should also increase linearly. The slope of this line is a function of the complexity of the scenario: the more eNodeB / UEs placed, the higher the slope of the line.

.. _fig-runTime:

.. figure:: figures/simulationTime.*                 
   :align: center
   :width: 60%

   Simulation time



Memory usage
~~~~~~~~~~~~

Massif tool to profile memory consumption

.. _fig-profilingMemory:

.. figure:: figures/profiling-memory.*
   :align: center
   :width: 60%

   Memory profile

