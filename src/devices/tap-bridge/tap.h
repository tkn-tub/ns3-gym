/**
 * \ingroup devices
 * \defgroup TapBridgeModel Tap Bridge Model
 *
 * \section TapBridgeModelOverview TapBridge Model Overview
 *
 * The Tap Bridge is designed to integrate "real" internet hosts (or more 
 * precisely, hosts that support Tun/Tap devices) into ns-3 simulations.  The 
 * goal is to make it appear to the host host node in question that it has an
 * ns-3 net device as a local device.  The concept of a "real host" is a bit
 * slippery the "real host" may actually be virtualized using readily avialable
 * technologies such as VMware or OpenVZ.
 *
 * Since we are, in essence, connecting the inputs and outputs of an ns-3 net 
 * device to the inputs and outputs of a Linux Tap net device, we call this 
 * arrangement a Tap Bridge.
 *
 * The TapBridge appears to the Linux host computer as a network device just
 * like any arbitrary "eth0" or "ath0" might appear.  The creation and
 * configuration of the device is done by the ns-3 simulation, however.  You 
 * should not expect to be able to configure a net device via, for example,
 * wlanconfig.  The IP addresses, MAC addresses, gateway, etc., for the given
 * Tap device are also set up by the ns-3 simulation.  If you change the
 * or manipulate the configuration manually, you will almost certainly break
 * the simulation.
 *
 * The TapBridge appears to an ns-3 simulation as a channel-less net device.
 * This device, however, must _not_ have an IP address associated with it.  
 * Be aware that this is the inverse situation of an ns-3 BridgeNetDevice
 * which demands that its bridge ports not have IP addresses, but allows the
 * bridge to have an IP address.  
 *
 * The host computer will appear in a simulation as a "ghost" node that contains
 * pairs of net devices and Tap bridges that represent the host devices.  From
 * the perspective of a simulation, the only difference between a ghost node and
 * another node will be the presence of the TapBridge devices that connect to
 * the hosts.  Configuration of address information and the ns-3 devices is
 * not changed in any way.  A TapBridge will pick up the addressing info from
 * the ns-3 net device to which it is connected (its "bridged" net device) and
 * use that information to configure the device on the real host.
 *
 * The end result of this is a situation where one can, for example, use the
 * standard ping utility on a real host to ping a simulated ns-3 net device.  If
 * correct routes are added to the internet host, the routing systems in ns-3
 * will enable correct routing of the packets across simulated ns-3 networks.
 * For an example of this, see the example program, tap-dumbbell.cc in the 
 * ns-3 distribution.
 *
 * \section TapBridgeChannelModel Tap Bridge Channel Model
 *
 * There is no channel model associated with the Tap Bridge.  In fact, the
 * intention is make it appear that the real internet host is connected to 
 * the channel of the bridged net device.
 *
 * \section TapBridgeTracingModel Tap Bridge Tracing Model
 *
 * Unlike most ns-3 devices, the TapBridge does not provide any standard trace
 * sources.  This is because the bridge is an intermediary that is essentially
 * one function call away from the bridged device.  We expect that the trace
 * hooks in the bridged device will be sufficient for most users,
 *
 * \section TapBridgeUsage Using the Tap Bridge
 *
 * We expect that most users will interact with the TapBridge device through
 * the TapBridgeHelper.  Users of other helper classes, such as CSMA or Wifi,
 * should be comfortable with the idioms used.
 */

