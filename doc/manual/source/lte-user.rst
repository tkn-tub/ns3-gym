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


``src/lte/examples/lena-first-sim.cc`` shows how to build a simple but complete simulation with one LTE eNB and one LTE UE. The detail explanation of the code follows:
