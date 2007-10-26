/**
 * \section Wifi Models
 *
 * The set of 802.11 models provided in ns-3 attempts to provide
 * an accurate MAC-level implementation of the 802.11 specification
 * and to provide a not-so-slow PHY-level model of the 802.11a
 * specification.
 *
 * The current implementation provides roughly 3 levels of models:
 *   - the PHY layer models
 *   - the so-called MAC low models: they implement DCF
 *   - the so-called MAC high models: they implement the MAC-level
 *     beacon generation, probing, and association state machines.
 *
 * We have today 3 MAC high models:
 *   - a simple adhoc state machine which does not perform any
 *     kind of beacon generation, probing, or association. This
 *     state machine is implemented by the ns3::AdhocWifiNetDevice
 *     class.
 *   - an active probing and association state machine which handles
 *     automatic re-association whenever too many beacons are missed
 *     is implemented by the ns3::NqstaWifiNetDevice class.
 *   - an access point which generates periodic beacons, and which
 *     accepts every attempt to associate. This AP state machine
 *     is implemented by the ns3::NqapWifiNetDevice.
 *
 * The PHY layer implements the model described in 
 * "Yet Another Network Simulator", (http://cutebugs.net/files/wns2-yans.pdf)
 * The implementation is located in the ns3::WifiPhy class.
 *
 * The MAC low models attempt to implement the 802.11 DCF function
 * in an optimized way through the technique described in XXX.
 * This implementation is located in the ns3::Dcf class.
 *
 * \section Wifi Tutorial
 *
 *
 */
