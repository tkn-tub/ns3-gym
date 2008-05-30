/**
 * \ingroup devices
 * \defgroup PointToPoint Point-to-Point Model
 *
 * \section Point-to-Point Model
 *
 * The ns-3 point-to-point model is of a very simple point to point data link
 * connecting exactly two ns3::PointToPointNetDevice devices over an
 * ns3::PointToPointChannel.  This can be viewed as equivalent to a full
 * duplex RS-232 or RS-422 link with null modem and no handshaking.
 *
 * Data is encapsulated in the Point-to-Point Protocol (PPP -- RFC 1661),
 * however the Link Control Protocol (LCP) and associated state machine is 
 * not implemented.  The PPP link is assumed to be established and 
 * authenticated at all times.
 *
 * Data is not framed, therefore Address and Control fields will not be found.
 * Since the data is not framed, there is no need to provide Flag Sequence and
 * Control Escape octets, nor is a Frame Check Sequence appended.  All that is
 * required to implement non-framed PPP is to prepend the PPP protocol number
 * for IP Version 4 which is the sixteen-bit number 0x21 (see
 * http://www.iana.org/assignments/ppp-numbers).
 *
 * The ns3::PointToPointNetDevice provides following Attributes:
 *
 * - Address:       The ns3::Mac48Address of the device (if desired);
 * - DataRate:      The data rate of the device;
 * - TxQueue:       The trasmit queue used by the device;
 * - InterframeGap: The optional time to wait between "frames";
 * - Rx:            A trace source for received packets;
 * - Drop:          A trace source for dropped packets.
 *
 * The ns3::PointToPointNetDevice supports the assignment of a "receive error 
 * model."  This is an ns3::ErrorModel object that is used to simulate data
 * corruption on the link.
 *
 * The point to point net devices are connected via an 
 * ns3::PointToPointChannel.  This channel models two wires transmitting bits
 * at the data rate specified by the source net device.  There is no overhead
 * beyond the eight bits per byte of the packet sent.  That is, we do not 
 * model Flag Sequences, Frame Check Sequences nor do we "escape" any data.
 *
 * The ns3::PointToPointChannel does model a speed-of-light or transmission
 * delay which can be set and get via the attribute "Delay."
 */
