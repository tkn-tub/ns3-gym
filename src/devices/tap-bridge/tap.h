/**
 * \ingroup devices
 * \defgroup TapBridgeModel Tap Bridge Model
 *
 * \section TapBridgeModelOverview TapBridge Model Overview
 *
 * The Tap Bridge is designed to integrate "real" internet hosts (or more 
 * precisely, hosts that support Tun/Tap devices) into ns-3 simulations.  The 
 * goal is to make it appear to a "real" host node in that it has an ns-3 net
 * device as a local device.  The concept of a "real host" is a bit slippery
 * since the "real host" may actually be virtualized using readily avialable
 * technologies such as VMware or OpenVZ.
 *
 * Since we are, in essence, connecting the inputs and outputs of an ns-3 net 
 * device to the inputs and outputs of a Linux Tap net device, we call this 
 * arrangement a Tap Bridge.
 *
 * There are two basic operating modes of this device available to users.  
 * Basic functionality is essentially identical, but the two modes are 
 * different in details regarding how the arrangement is configured.  In the
 * first mode, the configuration is ns-3 configuration-centric.  Configuration
 * information is taken from the ns-3 simulation and a tap device matching
 * the ns-3 attributes is created for you.  In this mode, which we call 
 * LocalDevice mode, an ns-3 net device is made to appear to be directly 
 * connected to a real host.  
 * 
 * This is illustrated below
 *
 * \verbatim
 *     +--------+
 *     |  Linux |
 *     |  host  |                    +----------+
 *     | ------ |                    |   ghost  |
 *     |  apps  |                    |   node   |
 *     | ------ |                    | -------- |
 *     |  stack |                    |    IP    |     +----------+
 *     | ------ |                    |   stack  |     |   node   |
 *     |  TAP   |                    |==========|     | -------- |
 *     | device | <-- IPC Bridge --> |   tap    |     |    IP    |
 *     +--------+                    |  bridge  |     |   stack  |
 *                                   | -------- |     | -------- |
 *                                   |   ns-3   |     |   ns-3   |
 *                                   |   net    |     |   net    |
 *                                   |  device  |     |  device  |
 *                                   +----------+     +----------+
 *                                        ||               ||
 *                                   +---------------------------+
 *                                   |        ns-3 channel       |
 *                                   +---------------------------+
 *\endverbatim
 *
 * In this case, the ns-3 net device in the ghost node appears as if it were 
 * actually replacing the TAP device in the Linux host.  The ns-3 process 
 * creates the TAP device configures the IP address and MAC address of the 
 * TAP device to match the values assigned to the ns-3 net device.  The IPC 
 * link is via the network tap mechanism in the underlying OS and acts as a 
 * conventional bridge; but a bridge between devices that happen to have the 
 * same shared MAC address.
 *
 * The LocalDevice mode is the default operating mode of the Tap Bridge.
 *
 * The second mode, BridgedDevice mode, is more oriented toward allowing existing
 * host configurations.  This allows ns-3 net devices to appear as part of a host
 * operating system bridge (in Linux, we make the ns-3 device part of a "brctl"
 * bridge.  This mode is especially  useful in the case of virtualization where 
 * the configuration of the virtual hosts may be dictated by another system and 
 * not be changable to suit ns-3.  For example, a particular VM scheme may create
 * virtual "vethx" or "vmnetx" devices that appear local to virtual hosts.  In 
 * order to connect to such systems, one would need to manually create TAP devices
 * on the host system and brigde these TAP devices to the existing (VM) virtual 
 * devices.  The job of the Tap Bridge in this case is to extend the bridge to
 * join the ns-3 net device.
 *
 * This is illustrated below:
 *
 * \verbatim
 *     +---------+
 *     |  Linux  |
 *     |   VM    |                             +----------+
 *     | ------- |                             |   ghost  |
 *     |  apps   |                             |   node   |
 *     | ------- |                             | -------- |
 *     |  stack  |                             |    IP    |     +----------+
 *     | ------- | +--------+                  |   stack  |     |   node   |
 *     | Virtual | |  TAP   |                  |==========|     | -------- |
 *     | Device  | | Device | <-- IPC Bridge-> |   tap    |     |    IP    |
 *     +---------+ +--------+                  |  bridge  |     |   stack  |
 *         ||          ||                      | -------- |     | -------- |
 *     +--------------------+                  |   ns-3   |     |   ns-3   |
 *     | OS (brctl) Bridge  |                  |   net    |     |   net    |
 *     +--------------------+                  |  device  |     |  device  |
 *                                             +----------+     +----------+
 *                                                  ||               ||
 *                                             +---------------------------+
 *                                             |        ns-3 channel       |
 *                                             +---------------------------+
 *\endverbatim
 *
 * In this case, a collection of virtual machines with associated Virtual
 * Devices is created in the virtualization environment (for exampe, OpenVZ
 * or VMware).  A TAP device (with a specific name) is then manually created 
 * for each Virtual Device that will be bridged into the ns-3 simulation.
 * These created TAP devices are then bridged together with the Virtual Devices
 * using a native OS bridge mechanism shown as "OS (brctl) Bridge" in the 
 * illustration above.
 *
 * In the ns-3 simulation, a Tap Bridge is created to match each TAP Device.
 * The name of the TAP Device is assigned to the Tap Bridge using the 
 * "DeviceName" attribute.  The Tap Bridge then opens a network tap to the TAP 
 * Device and logically extends the bridge to encompass the ns-3 net device.  
 * This makes it appear as if an ns-3 simulated net device is a member of the 
 * "OS (brctl) Bridge" and allows the Virtual Machines to communicate with the 
 * ns-3 simulation..
 *
 * \subsection TapBridgeLocalDeviceMode TapBridge LocalDevice Mode
 * 
 * In LocalDevice mode, the TapBridge and therefore its associated ns-3 net
 * device appears to the Linux host computer as a network device just like any
 * arbitrary "eth0" or "ath0" might appear.  The creation and configuration 
 * of the TAP device is done by the ns-3 simulation and no manual configuration
 * is required by the user.  The IP addresses, MAC addresses, gateways, etc., 
 * for created TAP devices are extracted from the simulation itself by querying
 * the configuration of the ns-3 device and the TapBridge Attributes.
 *
 * The TapBridge appears to an ns-3 simulation as a channel-less net device.
 * This device must not have an IP address associated with it, but the bridged
 * (ns-3) net device must have an IP adress.  Be aware that this is the inverse
 * of an ns-3 BridgeNetDevice (or a conventional bridge in general) which 
 * demands that its bridge ports not have IP addresses, but allows the bridge 
 * device itself to have an IP address.  
 *
 * The host computer will appear in a simulation as a "ghost" node that contains
 * one TapBridge for each NetDevice that is being bridged.  From the perspective
 * of a simulation, the only difference between a ghost node and any other node 
 * will be the presence of the TapBridge devices.  Note however, that the 
 * presence of the TapBridge does affect the connectivity of the net device to 
 * the IP stack of the ghost node.
 *
 * Configuration of address information and the ns-3 devices is not changed in 
 * any way if a TapBridge is present.  A TapBridge will pick up the addressing
 * information from the ns-3 net device to which it is connected (its "bridged"
 * net device) and use that information to create and configure the TAP device
 * on the real host.
 *
 * The end result of this is a situation where one can, for example, use the
 * standard ping utility on a real host to ping a simulated ns-3 node.  If
 * correct routes are added to the internet host (this is expected to be done
 * automatically in future ns-3 releases), the routing systems in ns-3 will
 * enable correct routing of the packets across simulated ns-3 networks.
 * For an example of this, see the example program, tap-wifi-dumbbell.cc in 
 * the ns-3 distribution.
 *
 * \subsection TapBridgeLocalDeviceModeOperation TapBridge LocalDevice Mode Operation
 *
 * The Tap Bridge lives in a kind of a gray world somewhere between a Linux host
 * and an ns-3 bridge device.  From the Linux perspective, this code appears as
 * the user mode handler for a TAP net device.  In LocalDevice mode, this TAP 
 * device is automatically created by the ns-3 simulation.  When the Linux host 
 * writes to one of these automatically created /dev/tap devices, the write is 
 * redirected into the TapBridge that lives in the ns-3 world; and from this 
 * perspective, the packet write on Linux becomes a packet read in the Tap Bridge.
 * In other words, a Linux process writes a packet to a tap device and this packet
 * is redirected by the network tap mechanism toan ns-3 process where it is 
 * received by the TapBridge as a result of a read operation there.  The TapBridge
 * then writes the packet to the ns-3 net device to which it is bridged; and 
 * therefore it appears as if the Linux host sent a packet directly through an 
 * ns-3 net device onto an ns-3 network.
 *
 * In the other direction, a packet received by the ns-3 net device connected to
 * the Tap Bridge is sent via promiscuous callback to the TapBridge.  The 
 * TapBridge then takes that packet and writes it back to the host using the 
 * network tap mechanism.  This write to the device will appear to the Linux 
 * host as if a packet has arrived on its device; and therefore as if a packet
 * received by the ns-3 net device has appeared on a Linux net device.
 * 
 * The upshot is that the Tap Bridge appears to bridge a tap device on a
 * Linux host in the "real world" to an ns-3 net device in the simulation.
 * Because the TAP device and the bridged ns-3 net device have the same MAC
 * address and the network tap IPC link is not exernalized, this particular
 * kind of bridge makes ti appear that a ns-3 net device is actually installed
 * in the Linux host.
 *
 * In order to implement this on the ns-3 side, we need a "ghost node" in the
 * simulation to hold the bridged ns-3 net device and the TapBridge.  This node
 * should not actually do anything else in the simulation since its job is 
 * simply to make the net device appear in Linux.  This is not just arbitrary 
 * policy, it is because:
 *
 * - Bits sent to the Tap Bridge from higher layers in the ghost node (using
 *   the TapBridge Send method) are completely ignored.  The Tap Bridge is 
 *   not, itself, connected to any network, neither in Linux nor in ns-3.  You
 *   can never send nor receive data over a Tap Bridge from the ghost node.
 *
 * - The bridged ns-3 net device has its receive callback disconnected
 *   from the ns-3 node and reconnected to the Tap Bridge.  All data received 
 *   by a bridged device will then be sent to the Linux host and will not be 
 *   received by the node.  From the perspective of the ghost node, you can 
 *   send over this device but you cannot ever receive.
 *
 * Of course, if you understand all of the issues you can take control of
 * your own destiny and do whatever you want -- we do not actively
 * prevent you from using the ghost node for anything you decide.  You
 * will be able to perform typical ns-3 operations on the ghost node if
 * you so desire.  The internet stack, for example, must be there and
 * functional on that node in order to participate in IP address
 * assignment and global routing.  However, as mentioned above,
 * interfaces talking any Tap Bridge or associated bridged net devices
 * will not work completely.  If you understand exactly what you are
 * doing, you can set up other interfaces and devices on the ghost node
 * and use them; or take advantage of the operational send side of the
 * bridged devices to create traffic generators.  We generally recommend
 * that you treat this node as a ghost of the Linux host and leave it to
 * itself, though.
 *
 * \subsection TapBridgeBridgedDeviceMode TapBridge BridgedDevice Mode
 * 
 * In BridgedDevice mode, the TapBridge and its associated ns-3 net device are
 * arranged in a fundamentally similar was as in LocalDevice mode.  The TAP
 * device is bridged to the ns-3 net device in the same way.  The description
 * of LocalDevice mode applies except as noted below.
 *
 * The most user-visible difference in modes is how the creation and 
 * configuration of the underlying TAP device is done.  In LocalDevice mode,
 * both creation and configuration of the underlying TAP device are handled 
 * completely by ns-3.  In BridgedDevice mode, creation and configuration is
 * delegated (due to requirements) to the user.  No configuration is done in
 * ns-3 other than settting the operating mode of the TapBridge to 
 * "BridgedDevice" and specifying the name of a pre-configured TAP device
 * using ns-3 Attributes of the TapBridge.
 *
 * The primary conceptual difference between modes is due to the fact that in 
 * BridgedDevice mode the MAC addresses of the user-created TAPs will be pre-
 * configured and will therefore be different than those in the bridged device.
 * As in LocalDevice mode, the Tap Bridge functions as IPC bridge between the
 * TAP device and the ns-3 net device, but in BridgedDevice configurations the
 * two devices will have different MAC addresses and the bridging functionality
 * will be fundamentally the same as in any bridge.  Since this implies MAC
 * address spoofing, the only ns-3 devices which may paritcipate in a bridge
 * in BridgedDevice mode must support SendFrom (i.e., a call to the method
 * SupportsSendFrom in the bridged net device must return true).
 *
 * \subsection TapBridgeBridgedDeviceModeOperation TapBridge BridgedDevice Mode Operation
 *
 * As described in the LocalDevice mode section, when the Linux host writes to 
 * one of the /dev/tap devices, the write is redirected into the TapBridge
 * that lives in the ns-3 world.  In the case of the BridgedDevice mode, these
 * packets will need to be sent out on the ns-3 network as if they were sent on
 * the Linux network.  This means calling the SendFrom method on the bridged
 * device and providing the source and destination MAC addresses found in the
 * packet.
 *
 * In the other direction, a packet received by an ns-3 net device is hooked
 * via callback to the TapBridge.  This must be done in promiscuous mode since
 * the goal is to bridge the ns-3 net device onto the OS (brctl) bridge of 
 * which the TAP device is a part.
 *
 * There is no functional difference between modes at this level, even though
 * the configuration and conceptual models regarding what is going on are quite
 * different -- the Tap Bridge is just acting like a bridge.  In the LocalDevice
 * mode, the bridge is between devices having the same MAC address and in the 
 * BridgedDevice model the bridge is between devices having different MAC 
 * addresses.
 *
 * \subsection TapBridgeSingleSourceModeOperation TapBridge SingleSource Mode Operation
 *
 * As described in above, the Tap Bridge acts like a bridge.  Just like every
 * other bridge, there is a requirement that participating devices must have
 * the ability to receive promiscuously and to spoof the source MAC addresses
 * of packets.
 *
 * We do, however, have a specific requirement to be able to bridge Virtual 
 * Machines onto wireless STA nodes.  Unfortunately, the 802.11 spec doesn't
 * provide a good way to implement SendFrom.  So we have to work around this.
 * 
 * To this end, we provice the SingleSource mode of the Tap Bridge.  This
 * mode allows you to create a bridge as described in BridgedDevice mode, but
 * only allows one source of packets on the Linux side of the bridge.  The
 * address on the Linux side is remembered in the Tap Bridge, and all packets
 * coming from the Linux side are repeated out the ns-3 side using the ns-3 device
 * MAC source address.  All packets coming in from the ns-3 side are repeated
 * out the Linux side using the remembered MAC address.  This allows us to use
 * SendFrom on the ns-3 device side which is available on all ns-3 net devices.
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

