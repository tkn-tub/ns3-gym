.. include:: replace.txt


+++++++++++++++++++++++++++++++++
 User Documentation
+++++++++++++++++++++++++++++++++


Background
**********



We assume the reader is already familiar with how to use the ns-3
simulator to run generic simulation programs. If this is not the case,
we strongly recommend the reader to consult [ns3tutorial]_.

  
Usage Overview
**************

The ns-3 LTE model is a software library that allows the simulation of
LTE networks.  The process of performing such simulations typically involves the following
steps:

 1. *Define the scenario* to be simulated
 2. *Write a simulation program* that recreates the desired scenario
    topology/architecture. This is done accessing the ns-3 LTE model
    libraryusing the ``ns3::LenaHelper`` API defined in ``src/lte/helper/lena-helper.h``. 
 3. *Specify configuration parameters* of the objects that are being
    used for the simulation. This can be done using input files (via the
    ``ns3::ConfigStore``) or directly within the simulation program.
 4. *Configure the desired output* to be produced by the simulator
 5. *Run* the simulation.

All these aspects will be explained in the following sections by means
of practical examples.



Basic simulation program
------------------------

Here is the minimal simulation program that is needed to do an LTE simulation.

.. highlight:: none

#. Initial boilerplate::
       
    #include "ns3/core-module.h"
    #include "ns3/network-module.h"
    #include "ns3/mobility-module.h"
    #include "ns3/lte-module.h"   

    using namespace ns3;    

    int main (int argc, char *argv[])
    {


#. Create a LenaHelper object::

      Ptr<LenaHelper> lena = CreateObject<LenaHelper> ();

   This will instantiate some common
   objects (e.g., the Channel object) and provide the methods to add
   eNBs and UEs and configure them.

#. Create Node objects for the eNB(s) and the UEs::

      NodeContainer enbNodes;
      enbNodes.Create (1);
      NodeContainer ueNodes;
      ueNodes.Create (2);
  
   Note that the above Node instances at this point still don't have
   an LTE protocol stack installed; they're just empty nodes. 

#. Configure the Mobility model for all the nodes::

      MobilityHelper mobility;
      mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
      mobility.Install (enbNodes);
      mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
      mobility.Install (ueNodes);

   The above will place all nodes at the coordinates (0,0,0). Please
   refer to the documentation of the ns-3 mobility model for how to
   set your own position or configure node movement.
  
#. Install an LTE protocol stack on the eNB(s)::

      NetDeviceContainer enbDevs;
      enbDevs = lena->InstallEnbDevice (enbNodes);

#. Install an LTE protocol stack on the UEs::

      NetDeviceContainer ueDevs;
      ueDevs = lena->InstallUeDevice (ueNodes);


#. Attach the UEs to an eNB. This will configure each UE according to
   the eNB configuration, and create an RRC connection between them::

      lena->Attach (ueDevs, enbDevs.Get (0));

#. Activate an EPS Bearer including the setup of the Radio Bearer between an eNB and its attached UE::

      enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
      EpsBearer bearer (q);
      lena->ActivateEpsBearer (ueDevs, bearer);

   In the current version of the ns-3 LTE model, the activation of an
   EPS Bearer will also activate two saturation traffic generators for
   that bearer, one in uplink and one in downlink. 

#. Set the stop time::

       Simulator::Stop (Seconds (0.005));
  
   This is needed otherwise the simulation will last forever, because
   (among others) the start-of-subframe event is scheduled repeatedly, and the
   ns-3 simulator scheduler will hence never run out of events.

#. Run the simulation::

       Simulator::Run ();

#. Cleanup and exit::


       Simulator::Destroy ();
       return 0;
       }

For how to compile and run simulation programs, please refer to [ns3tutorial]_.


Configuration of LTE model parameters 
--------------------------------------

All the relevant LTE model parameters are managed through the ns-3
attribute system. Please refer to the [ns3tutorial]_ and [ns3manual]_
for detailed information on all the possible methods to do it
(environmental variables, C++ API, GtkConfigStore...).

In the following, we just briefly summarize
how to do it using input files together with the ns-3 ConfigStore.
First of all, you need to put the following in your simulation
program, right after ``main ()`` starts::

      CommandLine cmd;
      cmd.Parse (argc, argv);
      ConfigStore inputConfig;
      inputConfig.ConfigureDefaults ();
      // parse again so you can override default values from the command line
      cmd.Parse (argc, argv);

for the above to work, make sure you also ``#include "ns3/config-store.h"``.
Now create a text file named (for example) ``input-defaults.txt``
specifying the new default values that you want to use for some attributes::

   default ns3::LenaHelper::Scheduler "ns3::PfFfMacScheduler"
   default ns3::LenaHelper::PropagationModel "ns3::FriisSpectrumPropagationLossModel"
   default ns3::LteEnbNetDevice::UlBandwidth "25"
   default ns3::LteEnbNetDevice::DlBandwidth "25"
   default ns3::LteEnbNetDevice::DlEarfcn "100"
   default ns3::LteEnbNetDevice::UlEarfcn "18100"
   default ns3::LteUePhy::TxPower "10"
   default ns3::LteUePhy::NoiseFigure "9"
   default ns3::LteEnbPhy::TxPower "30"
   default ns3::LteEnbPhy::NoiseFigure "5"

Supposing your simulation program is called
``src/lte/examples/lte-sim-with-input``, you can now pass these
settings to the simulation program in the following way::

   ./waf --command-template="%s --ns3::ConfigStore::Filename=input-defaults.txt --ns3::ConfigStore::Mode=Load --ns3::ConfigStore::FileFormat=RawText" --run src/lte/examples/lte-sim-with-input


Furthermore, you can generate a template input file with the following
command::

   ./waf --command-template="%s --ns3::ConfigStore::Filename=input-defaults.txt --ns3::ConfigStore::Mode=Save --ns3::ConfigStore::FileFormat=RawText" --run src/lte/examples/lte-sim-with-input

note that the above will put in the file ``input-defaults.txt`` *all*
the default values that are registered in your particular build of the
simulator, including lots of non-LTE attributes.


Simulation Output
-----------------

The ns-3 LTE model currently supports the output to file of both MAC and RLC
level Key Performance Indicators (KPIs). You can enable it in the following way::

      Ptr<LenaHelper> lena = CreateObject<LenaHelper> ();
      
      // configure all the simulation scenario here...
      
      lena->EnableMacTraces ();
      lena->EnableRlcTraces ();   

      Simulator::Run ();


RLC KPIs are calculated over a time interval and stored on two ASCII
files, one for uplink and one for downlink. The time interval duration
and the name of the files can be controlled using the attributes
``ns3::RlcStatsCalculator::EpochDuration``,
``ns3::RlcStatsCalculator::DlOutputFilename`` and
``ns3::RlcStatsCalculator::UlOutputFilename``.
The content of the columns of these files is the following (the same
for uplink and downlink):
 
  1. start time of measurement interval in seconds since the start of simulation
  2. end time of measurement interval in seconds since the start of simulation
  3. unique UE ID
  4. RNTI
  5. Logical Channel ID
  6. Number of transmitted PDUs
  7. Total bytes transmitted.
  8. Number of received PDUs
  9. Total bytes received
  10. Average PDU delay in seconds
  11. Standard deviation of the PDU delay
  12. Minimum value of the PDU delay
  13. Maximum value of the PDU delay
  14. Average PDU size, in bytes
  15. Standard deviation of the PDU size
  16. Minimum PDU size
  17. Maximum PDU size

MAC KPIs are basically a trace of the resource allocation reported by
the scheduler upon the start of every subframe. They are stored in
ASCII files. For downlink MAC KPIs the format is the following:

  1. Simulation time in seconds at which the allocation is indicated by the scheduler
  2. Cell Identifier
  3. Frame number
  4. Subframe number
  5. RNTI
  6. MCS of TB 1
  7. size of TB 1
  8. MCS of TB 2 (0 if not present)
  9. size of TB 2 (0 if not present)

while for uplink MAC KPIs the format is:

  1. Simulation time in seconds at which the allocation is indicated by the scheduler
  2. Frame number
  3. Subframe number
  4. RNTI
  5. MCS of TB
  6. size of TB

The names of the files used for MAC KPI output can be customized via
the ns-3 attributes ``ns3::MacStatsCalculator::DlOutputFilename`` and 
``ns3::MacStatsCalculator::UlOutputFilename``.


Further Reading
---------------

The directory ``src/lte/examples/`` contains some example simulation programs that
show how to simulate different LTE scenarios. 


Performance evaluation
**********************

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


Fading Trace Management
***********************

Fading Traces Generation
------------------------

Thanks to the matlab script provided with the code (``/lte/model/JakesTraces/fading-trace-generator.m``) it is possible to generate traces according to specific simulation scenarios. The script already includes the typical taps configurations for three 3GPP scenarios (i.e., pedestrian, vehicular and urban as defined in Annex B.2 of [TS36.104]_); however users can introduce their specific configurations. The list of the configurable parameters is provided in the following:

 * ``fc`` : the frequency in use (it affects the computation of the dopples speed).
 * ``v_km_h`` : the speed of the users
 * ``traceDuration`` : the duration in seconds of the total length of the trace.
 * ``numRBs`` : the number of the resource block to be evaluated. 
 * ``tag`` : the tag to be applied to the file generated.

The file generated is formatted in a matrix fashion by putting each RBs temporal fading trace samples in different rows.
It has to be noted that, the model is able to manage external fading trace (e.g., generated with dedicated simulators or obtained in experimental way) which respect the format used by the model (i.e., ASCII file with temporal fading trace of each RB distributed in rows).

Fading Traces Usage
-------------------

The proper set of the trace parameters in the simulation is of paramount importance for the correct interpretation of the trace itself within the simulator.
The list of the parameters to be configured are:

 * ``TraceFilename`` : the name of the trace to be loaded (absolute path o relative one according to the execution point of the script).
 * ``TraceLength`` : the trace duration in seconds.
 * ``SamplesNum`` : the number of samples.
 * ``WindowSize`` : the size of the fading sampling window in seconds.

It is important to highlight that the sampling interval of the fading trace has to me at most of 1 ms or greater and in the latter case it has to be an integer multiple of 1 ms in order to be correctly processed by the fading module.
The default configuration of the matlab script provides a trace 10 seconds long, made of 10,000 samples (i.e., 1 sample per TTI ~ 1ms) and used with a windows size of 0.5 seconds amplitude. These are also the default values of the parameters above used in the simulator; therefore their settage can be avoided in case the fading trace respects them.

In order to activate the fading module (by default not active) it has to be explicitly specified in the simulation script::

  Ptr<LenaHelper> lena = CreateObject<LenaHelper> ();
  lena->SetFadingModel("ns3::TraceFadingLossModel");

While, for setting the parameters::

  lena->SetFadingModelAttribute ("TraceFilename", StringValue ("src/lte/model/FadingTraces/fading_trace_EPA_3kmph.fad"));
  lena->SetFadingModelAttribute ("TraceLength", TimeValue (Seconds (10.0)));
  lena->SetFadingModelAttribute ("SamplesNum", UintegerValue (10000));
  lena->SetFadingModelAttribute ("WindowSize", TimeValue (Seconds (0.5)));
  lena->SetFadingModelAttribute ("RbNum", UintegerValue (100));

It has to be noted that, ``TraceFilename`` does not have a default value, therefore is has to be always set for using the fading module in the simulator. The simulator provide natively a fading trace generated according to the pedestrian behavior defined in in Annex B.2 of [TS36.104]_ (available at ``src/lte/model/FadingTraces/fading_trace_EPA_3kmph.fad``); a sample of such trace is reported in `fig-fadingPedestrianTrace`_.


.. _fig-fadingPedestrianTrace:

.. figure:: figures/fading_pedestrian.*                 
   :align: center

   Example of the fading trace included in the simulator.


