/**
 * \ingroup devices
 * \defgroup Emulated Emulated Net Device Model
 *
 * \section Emulated Net Device Model
 *
 * The emulated net device allows a simulation node to talk to a real network
 * and send and receive packets that are processed by the corresponding ns-3
 * node.
 *
 * The emulated net device relies on a specified interface being in promiscuous
 * mode.  It opens a raw socket and binds to that interface.  We perform MAC
 * spoofing to separate simulation network traffic from other network traffic
 * that may be flowing to and from the host.
 *
 * Normally, the use case for emulated net devices is in collections of
 * small simulations that connect to the outside world through specific 
 * interfaces.  For example, one could construct a number of virtual
 * machines and connect them via a host-only network.  To use the emulated
 * net device, you would need to set all of the host-only interfaces in
 * promiscuous mode and provide an appropriate device name, "eth1" for example.
 *
 * One could also use the emulated net device in a testbed situation
 * where the host on which the simulation is running has a specific interface
 * of interest which drives the testbed hardware.  You would also need to set 
 * this specific interface into promiscuous mode and provide an appropriate 
 * device name to the ns-3 emulated net device.
 *
 * The emulated net device only works if the underlying interface is up in 
 * promiscuous mode.  We could just turn it on, but the situation is that we 
 * expect the other considerations listed above to have been dealt with.
 * To verify that these issues are dealt with, we just make sure that the end 
 * result of that process has taken place and that the specified interface is
 * in promiscuous mode.
 *
 * Packets will be sent out over the device, but we use MAC spoofing.  The
 * MAC addresses will be generated using the Organizationally Unique Identifier
 * (OUI) 00:00:00 as a base.  This vendor code is not assigned to any 
 * organization and so should not conflict with any real hardware.  

 * It is always up to you to determine that using these MAC addresses is
 * okay on your network and won't conflict with anything else (including another
 * simulation using emu devices) on your network.  If you are using the 
 * emulated net device in separate simulations you must consider global MAC 
 * address assignment issues and ensure that MAC addresses are unique across
 * all simulations.  The emulated net device respects the MAC address provided
 * in the SetAddress method so you can do this manually.  For larger simulations,
 * you may want to set the OUI in the MAC address allocation function.
 *
 * IP addresses corresponding to the emulated net devices are the addresses 
 * generated in the simulation, which are generated in the usual way via helper
 * functions.
 *
 * The emulated net device comes with a helper function as all ns-3 devices do.
 * One unique aspect is that there is no channel associated with the underlying
 * medium.  We really have no idea what this medium is, and so have not made an
 * effort to model it abstractly.  The primary thing to be aware of is the 
 * implication this has for static global routing.  The global router module
 * attempts to walk the channels looking for adjacent networks.  Since there 
 * is no channel, the global router will be unable to do this.
 */
