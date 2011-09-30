.. include:: replace.txt


++++++++++++++++++++++++++++++++++++++++++++++++++++++
 User Building Propagation & Mobility Model Documentation
++++++++++++++++++++++++++++++++++++++++++++++++++++++


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

In what following, a few guidelines for the usage of the ``BuildingMobilityModel`` and the ``BuildingPropagationModel`` classes.

.. highlight:: none

#. Inheritance::

    #include <ns3/buildings-mobility-model.h>
    #include <ns3/buildings-propagation-loss-model.h>
    #include <ns3/building.h>

#. Propagation model selection::

    Ptr<LenaHelper> lena = CreateObject<LenaHelper> ();
  
    lena->SetAttribute ("PropagationModel", StringValue ("ns3::BuildingsPropagationLossModel"));

#. EUTRA Band Selection
   
The selection of the working frequency of the propagation model has to be done with the standard ns-3 attribute system as described in the correspond section ("Configuration of LTE model parameters") by means of the DlEarfcn and UlEarfcn parameters, for instance::

   default ns3::LteEnbNetDevice::DlEarfcn "100"
   default ns3::LteEnbNetDevice::UlEarfcn "18100"

It is to be noted that any other configuration (i.e., with BuildingsPropagationLossModel attributes) might generates conflicts in the frequencies definition in the modules during the simulation.

#. Mobility model selection::

    MobilityHelper mobility;
    mobility.SetMobilityModel ("ns3::BuildingsMobilityModel");

#. Node creation and positioning::

    ueNodes.Create (1);
    mobility.Install (ueNodes);
    NetDeviceContainer ueDevs;
    ueDevs = lena->InstallUeDevice (ueNodes);
    Ptr<BuildingsMobilityModel> mm = enbNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
    double x_axis = 0.0;
    double y_axis = 0.0;
    double z_axis = 0.0;
    mm->SetPosition (Vector (x_axis, y_axis, z_axis));

#. Building creation::

    double x_min = 0.0;
    double x_max = 10.0;
    double y_min = 0.0;
    double y_max = 20.0;
    double z_min = 0.0;
    double z_max = 10.0;
    Ptr<Building> building = Create<Building> (x_min, x_max, y_min, y_max, z_min, z_max);
    building->SetBuildingType (Building::Residential);
    building->SetExtWallsType (Building::ConcreteWithWindows);
    building->SetFloorsNumber (3);
    building->SetNumberRoomX (3);
    building->SetNumberRoomY (2);

   This will instantiate a residential building with base of 10 x 20 meters and height of 10 meters with concrete with windows as external walls, three floors and a grid of rooms of 3 x 2.

#. Building and nodes interactions::

    mm->SetIndoor (building);
    mm->SetFloorNumber (2);
    mm->SetRoomNumberX (1);
    mm->SetRoomNumberY (1);

   This informs node's mobility model the fact that the node is inside the building at the second floor in the corner room of the 3 x 2 grid. 
   It has to be noted that the simulator does not check the consistence between the node's position and the building site, which is user's responsibility.










