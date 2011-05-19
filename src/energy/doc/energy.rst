Energy Framework
----------------

Energy consumption is a key issue for wireless devices, and wireless network researchers often need to investigate the energy consumption at a node or in the overall network while running network simulations in ns-3. This requires ns-3 to support energy consumption modeling. Further, as concepts such as fuel cells and energy scavenging are becoming viable for low power wireless devices, incorporating the effect of these emerging technologies into simulations requires support for modeling diverse energy sources in ns-3. The ns-3 Energy Framework provides the basis for energy consumption and energy source modeling.


Model Description
=================

The source code for the Energy Framework is currently at: ``src/energy``.

Design
******

The ns-3 Energy Framework is composed of 2 parts: Energy Source and Device Energy Model. 
The framework will be implemented into the ``src/energy/models`` folder.

Energy Source
#############

The Energy Source represents the power supply on each node. A node can have one or more energy sources, and each energy source can be connected to multiple device energy models. Connecting an energy source to a device energy model implies that the corresponding device draws power from the source. The basic functionality of the Energy Source is to provide energy for devices on the node. When energy is completely drained from the Energy Source, it notifies the devices on node such that each device can react to this event. Further, each node can access the Energy Source Objects for information such as remaining energy or energy fraction (battery level). This enables the implementation of energy aware protocols in ns-3. 

In order to model a wide range of power supplies such as batteries, the Energy Source must be able to capture characteristics of these supplies. There are 2 important characteristics or effects related to practical batteries:

* Rate Capacity Effect: Decrease of battery lifetime when the current draw is higher than the rated value of the battery.
* Recovery Effect: Increase of battery lifetime when the battery is alternating between discharge and idle states.

In order to incorporate the Rate Capacity Effect, the Energy Source uses current draw from all devices on the same node to calculate energy consumption. The Energy Source polls all devices on the same node periodically to calculate the total current draw and hence the energy consumption. When a device changes state, its corresponding Device Energy Model will notify the Energy Source of this change and new total current draw will be calculated.

The Energy Source base class keeps a list of devices (Device Energy Model objects) using the particular Energy Source as power supply. When energy is completely drained, the Energy Source will notify all devices on this list. Each device can then handle this event independently, based on the desired behavior when power supply is drained.

Device Energy Model
###################

The Device Energy Model is the energy consumption model of a device on node. It is designed to be a state based model where each device is assumed to have a number of states, and each state is associated with a power consumption value. Whenever the state of the device changes, the corresponding Device Energy Model will notify the Energy Source of the new current draw of the device. The Energy Source will then calculate the new total current draw and update the remaining energy.

The Device Energy Model can also be used for devices that do not have finite number of states. For example, in an electric vehicle, the current draw of the motor is determined by its speed. Since the vehicle's speed can take continuous values within a certain range, it is infeasible to define a set of discrete states of operation. However, by converting the speed value into current directly, the same set of Device Energy Model APIs can still be used.

Future Work
***********

For Device Energy Models, we are planning to include support for other PHY layer models provided in ns-3 such as WiMAX. For Energy Sources, we are planning to included new types of Energy Sources such as energy scavenging.

References
**********

.. [1] ns-2 Energy model: http://www.cubinlab.ee.unimelb.edu.au/~jrid/Docs/Manuel-NS2/node204.html
.. [2] H. Wu, S. Nabar and R. Poovendran. An Energy Framework for the Network Simulator 3 (ns-3).
.. [3] M. Handy and D. Timmermann. Simulation of mobile wireless networks with accurate 
       modelling of non-linear battery effects. In Proc. of Applied simulation and Modeling 
       (ASM), 2003.
.. [4] D. N. Rakhmatov and S. B. Vrudhula. An analytical high-level battery model for use in energy 
       management of portable electronic systems. In Proc. of IEEE/ACM International Conference on 
       Computer Aided Design (ICCAD’01), pages 488–493, November 2001.
.. [5] D. N. Rakhmatov, S. B. Vrudhula, and D. A. Wallach. Battery lifetime prediction for 
       energy-aware computing. In Proc. of the 2002 International Symposium on Low Power 
       Electronics and Design (ISLPED’02), pages 154–159, 2002.

Usage
=====

The main way that ns-3 users will typically interact with the Energy Framework is through the helper API and through the publicly visible attributes of the framework. The helper API is defined in ``src/energy/helper/*.h``.

In order to use the energy framework, the user must install an Energy Source for the node of interest and the corresponding Device Energy Model for the network devices. Energy Source (objects) are aggregated onto each node by the Energy Source Helper. In order to allow multiple energy sources per node, we aggregate an Energy Source Container rather than directly aggregating a source object.

The Energy Source object also keeps a list of Device Energy Model objects using the source as power supply. Device Energy Model objects are installed onto the Energy Source by the Device Energy Model Helper. User can access the Device Energy Model objects through the Energy Source object to obtain energy consumption information of individual devices.


Examples
********

The example directories, ``src/examples/energy`` and
``examples/energy``, contain some basic code that shows how to set up
the framework.

Helpers
*******

Energy Source Helper
####################

Base helper class for Energy Source objects, this helper Aggregates Energy Source object onto a node. Child implementation of this class creates the actual Energy Source object.

Device Energy Model Helper
##########################

Base helper class for Device Energy Model objects, this helper attaches Device Energy Model objects onto Energy Source objects. Child implementation of this class creates the actual Device Energy Model object.

Attributes
**********

Attributes differ between Energy Sources and Devices Energy Models implementations, please look at the specific child class for details.

Basic Energy Source
###################

* ``BasicEnergySourceInitialEnergyJ``: Initial energy stored in basic energy source.
* ``BasicEnergySupplyVoltageV``: Initial supply voltage for basic energy source.
* ``PeriodicEnergyUpdateInterval``: Time between two consecutive periodic energy updates.

RV Battery Model
################

* ``RvBatteryModelPeriodicEnergyUpdateInterval``: RV battery model sampling interval.
* ``RvBatteryModelOpenCircuitVoltage``: RV battery model open circuit voltage.
* ``RvBatteryModelCutoffVoltage``: RV battery model cutoff voltage.
* ``RvBatteryModelAlphaValue``: RV battery model alpha value.
* ``RvBatteryModelBetaValue``: RV battery model beta value.
* ``RvBatteryModelNumOfTerms``: The number of terms of the infinite sum for estimating battery level.

WiFi Radio Energy Model
#######################

* ``IdleCurrentA``: The default radio Idle current in Ampere.
* ``CcaBusyCurrentA``: The default radio CCA Busy State current in Ampere.
* ``TxCurrentA``: The radio Tx current in Ampere.
* ``RxCurrentA``: The radio Rx current in Ampere.
* ``SwitchingCurrentA``: The default radio Channel Switch current in Ampere.

Tracing
*******

Traced values differ between Energy Sources and Devices Energy Models implementations, please look at the specific child class for details.

Basic Energy Source
###################

* ``RemainingEnergy``: Remaining energy at BasicEnergySource.

RV Battery Model
################

* ``RvBatteryModelBatteryLevel``: RV battery model battery level.
* ``RvBatteryModelBatteryLifetime``: RV battery model battery lifetime.

WiFi Radio Energy Model
#######################

* ``TotalEnergyConsumption``: Total energy consumption of the radio device.

Validation
**********

Comparison of the Energy Framework against actual devices have not been performed. Current implementation of the Energy Framework is checked numerically for computation errors. The RV battery model is validated by comparing results with what was presented in the original RV battery model paper.
