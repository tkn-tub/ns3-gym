.. include:: replace.txt


+++++++++++++++++++++++++
 LTE User Documentation
+++++++++++++++++++++++++

EUTRA stack diagram
*******************

  
Usage
*****

Users interact with the LTE model through the LENA helper API and through the publicly visible methods of the model. The helper API is defined in ``src/lte/helper/lena-helper.h``.

The ``src/lte/examples/`` directory contains some basic examples that shows how to set up the LTE model in order to simulate an EUTRA downlink transmission.

Example simulation program
--------------------------

``src/lte/examples/lena-first-sim.cc`` shows how to build a simple but complete simulation with one LTE eNB and one LTE UE. The detail explanation of the code follows:

#. Declare the helper. This helper keeps together all the LTE components::

    LenaHelper lena;


#. Enable the logs in the LTE components::

    lena.EnableLogComponents ();


#. Create eNB and UE. They are created in its own node container::

    NodeContainer enbNodes;
    NodeContainer ueNodes;
    enbNodes.Create (1);
    ueNodes.Create (1);


#. Install mobility models to eNB and UE. This has to be done before devices are created and installed::

    MobilityHelper mobility;
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.Install (enbNodes);
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.Install (ueNodes);


#. Create devices and install them to the eNB and UE::

    NetDeviceContainer enbDevs;
    NetDeviceContainer ueDevs;
    enbDevs = lena.InstallEnbDevice (enbNodes);
    ueDevs = lena.InstallUeDevice (ueNodes);


#. Attach a UE to a eNB. It will also create a RRC connection between them and configure the UE::

    lena.Attach (ueDevs, enbDevs.Get (0));


#. Activate an EPS Bearer including the setup of the Radio Bearer between an eNB and its attached UE::

    enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
    EpsBearer bearer (q);
    lena.ActivateEpsBearer (ueDevs, bearer);


Performance evaluation
**********************

Execution time and memory consumption
-------------------------------------

In order to provide an running time and memory consumption estimation a reference simulation program has been developed, ``src/lte/examples/profiling-reference``. This program simulates a scenario composed by a set of eNodeBs, and a set of UEs attached to each eNB. All eNodeBs have the same number of attached UEs. Communications are performed both in the dowlink and in the uplink using a saturation model (i.e., each RLC instance always has a PDU to transmit). The UEs are all in the same position than its eNodeB and the eNodeBs are distributed in a line, each one 140m away from the previous one. The total simulation time is set to 60s.

Using this simulation program, we ran a simulation campaign varying the number of eNBs as well as the number of UEs per eNB. For each simulation, we measured the execution time using the ``time`` shell command in linux, and the memory consumtpion by looking at the information in ``/proc/{pid}/statm``. The reference hardware platform is a Intel Core2 Duo E8400 3.00GHz with 512 MB of RAM memory running a Fedora Core 10 with kernel 2.6.27.5. The simulator build used in this experiment was configured with the options ``-d optimized --enable-static``. 

The results are reported in the figures below. We note that the memory usage primarily depends on the number of eNBs, and is in general quite low. The execution time depends significantly on both the number of eNBs and the number of UEs per eNB, and in the largest experiment that we ran (12 eNBs and 10 UEs per eNB) it corresponds to 30 times the simulation time.

