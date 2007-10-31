/**
 * \section Wifi Models
 *
 * The set of 802.11 models provided in ns-3 attempts to provide
 * an accurate MAC-level implementation of the 802.11 specification
 * and to provide a not-so-slow PHY-level model of the 802.11a
 * specification.
 *
 * The current implementation provides roughly 4 levels of models:
 *   - the PHY layer models
 *   - the so-called MAC low models: they implement DCF
 *   - the so-called MAC high models: they implement the MAC-level
 *     beacon generation, probing, and association state machines.
 *   - a set of Rate control algorithms used by the MAC low models.
 * 
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
 * The MAC low layer is split in 3 components:
 *   - ns3::MacLow which takes care of RTS/CTS/DATA/ACK transactions.
 *   - ns3::Dcf which implements the DCF function.
 *   - ns3::DcaTxop which handles the packet queue, packet fragmentation,
 *     and packet retransmissions if they are needed.
 *
 * The PHY layer implements a single model in ns3::WifiPhy.
 *
 * It also provides a set of Rate control algorithms:
 *   - ns3::ArfMacStations (initialized from \valueref{WifiArfTimerThreshold}, and,
 *     \valueref{WifiArfSuccessThreshold})
 *   - ns3::AArfMacStations (initialized from \valueref{WifiAarfMinSuccessThreshold},
 *     \valueref{WifiAarfMinTimerThreshold}, \valueref{WifiAarfSuccessK}, 
 *     \valueref{WifiAarfMaxSuccessThreshold}, and, \valueref{WifiAarfTimerK}
 *   - ns3::IdealMacStations (initialized from \valueref{WifiIdealRateControlBerThreshold})
 *   - ns3::CrMacStations (initialized from \valueref{WifiConstantDataRate}, and,
 *     \valueref{WifiConstantCtlRate}).
 *
 * The type of rate control algorithm is controlled through \valueref{WifiRateControlAlgorithm}.
 *
 *
 * \section Wifi Tutorial
 *
 *
 */
