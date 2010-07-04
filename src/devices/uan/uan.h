/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Leonard Tracy <lentracy@gmail.com>
 */

/**
 * \ingroup devices
 * \defgroup UAN UAN Models
 *
 * \section UanOverview UAN Module Overview
 *
 * The main goal of the UAN Model is to enable researchers to
 * model a variety of underwater network scenarios.  The UAN model
 * is broken into three main parts:  The channel, PHY,  and MAC models.
 *
 *
 * \section UanPropOverview UAN Propagation Models
 *
 * Modelling of the underwater acoustic channel has been an active
 * area of research for quite some time.  Given the complications involved,
 * surface and bottom interactions, varying speed of sound, etc..., the detailed
 * models in use for ocean acoustics research are much too complex
 * (in terms of runtime) for use in network level simulations.  We have
 * attempted to provide the often used models as well as make an attempt to bridge, in part, the gap between
 * complicated ocean acoustic models and network level simulation.  The three propagation
 * models included are the ideal channel model, the Thorp propagation model and
 * the Bellhop propagation model (Available as an addition).
 *
 * All of the Propagation Models follow the same simple interface in ns3::UanPropModel.
 * The propagation models provide a power delay profile (PDP) and pathloss
 * information.  The PDP is retrieved using the GetPdp method which returns type UanPdp.
 * ns3::UanPdp utilises a tapped delay line model for the acoustic channel.
 * The UanPdp class is a container class for Taps, each tap has a delay and amplitude
 * member corresponding to the time of arrival (relative to the first tap arrival time)
 * and amplitude.   The propagation model also provides pathloss between the source
 * and receiver in dB re 1uPa.  The PDP and pathloss can then be used to find the
 * received signal power over a duration of time (i.e. received signal power in
 * a symbol duration and ISI which interferes with neighbouring signals).  Both
 * UanPropModelIdeal and UanPropModelThorp return a single impulse for a PDP.
 *
 *  a) Ideal Channel Model ns3::UanPropModelIdeal
 *
 *  The ideal channel model assumes 0 pathloss inside a cylindrical area with bounds
 *  set by attribute.  The ideal channel model also assumes an impulse PDP.
 *
 *  b) Thorp Propagation Model ns3::UanPropModelThorp
 *
 *  The Thorp Propagation Model calculates pathloss using the well-known Thorp approximation.
 *  This model is similar to the underwater channel model implemented in ns2 as described here:
 *
 *  Harris, A. F. and Zorzi, M. 2007. Modeling the underwater acoustic channel in ns2. In Proceedings
 *  of the 2nd international Conference on Performance Evaluation Methodologies and Tools
 *  (Nantes, France, October 22 - 27, 2007). ValueTools, vol. 321. ICST (Institute for Computer
 *  Sciences Social-Informatics and Telecommunications Engineering), ICST, Brussels, Belgium, 1-8.
 *
 *  The frequency used in calculation however, is the center frequency of the modulation as found from
 *  ns3::UanTxMode.  The Thorp Propagation Model also assumes an impulse channel response.
 *
 *  c) Bellhop Propagation Model ns3::UanPropModelBh (Available as an addition)
 *
 *  The Bellhop propagation model reads propagation information from a database.  A configuration
 *  file describing the location, and resolution of the archived information must be supplied via
 *  attributes.  We have included a utility, create-dat, which can create these data files using the Bellhop
 *  Acoustic Ray Tracing software (http://oalib.hlsresearch.com/).
 *
 *  The create-dat utility requires a Bellhop installation to run.  Bellhop takes
 *  environment information about the channel, such as sound speed profile, surface height
 *  bottom type, water depth, and uses a gaussian ray tracing algorithm to determine
 *  propagation information.  Arrivals from Bellhop are grouped together into equal length
 *  taps (the arrivals in a tap duration are coherently summed).  The maximum taps are then
 *  aligned to take the same position in the PDP.  The create-dat utility averages together
 *  several runs and then normalizes the average such that the sum of all taps is 1.  The same
 *  configuration file used to create the data files using create-dat should be passed via
 *  attribute to the Bellhop Propagation Model.
 *
 *  The Bellhop propagation model is available as a patch.  The link address will be
 *  made available here when it is posted online.  Otherwise email lentracy@gmail.com
 *  for more information.
 *
 *
 * \section UanPhyOverview UAN PHY Model Overview
 *
 * The PHY has been designed to allow for relatively easy extension
 * to new networking scenarios.  We feel this is important as, to date,
 * there has been no commonly accepted network level simulation model
 * for underwater networks.  The lack of commonly accepted network simulation
 * tools has resulted in a wide array of simulators and models used to report
 * results in literature.  The lack of standardization makes comparing results
 * nearly impossible.
 *
 * The main component of the PHY Model is the generic
 * PHY class, ns3::UanPhyGen.  The PHY class's general responsibility
 * is to handle packet acquisition, error determination, and forwarding of successful
 * packets up to the MAC layer.  The Generic PHY uses two models for determination
 * of signal to noise ratio (SINR) and packet error rate (PER).  The
 * combination of the PER and SINR models determine successful reception
 * of packets.  The PHY model connects to the channel via a Transducer class.
 * The Transducer class is responsible for tracking all arriving packets and
 * departing packets over the duration of the events. How the PHY class and the PER and SINR models
 * respond to packets is based on the "Mode" of the transmission as described by the ns3::UanTxMode
 * class.
 *
 * When a MAC layer sends down a packet to the PHY for transmission it specifies a "mode number" to
 * be used for the transmission.  The PHY class accepts, as an attribute, a list of supported modes.  The
 * mode number corresponds to an index in the supported modes.  The UanTxMode contains simple modulation
 * information and a unique string id.  The generic PHY class will only acquire arriving packets which
 * use a mode which is in the supported modes list of the PHY.  The mode along with received signal power,
 * and other pertinent attributes (e.g. possibly interfering packets and their modes) are passed to the SINR
 * and PER models for calculation of SINR and probability of error.
 *
 * Several simple example PER and SINR models have been created.
 * a) The PER models
 *    - Default (simple) PER model (ns3::UanPhyPerGenDefault):  The Default PER model tests the packet against a threshold and
 *      assumes error (with prob. 1) if the SINR is below the threshold or success if the SINR is above
 *      the threshold
 *    - Micromodem FH-FSK PER (ns3::UanPhyPerUmodem).  The FH-FSK PER model calculates probability of error assuming a
 *      rate 1/2 convolutional code with constraint length 9 and a CRC check capable of correcting
 *      up to 1 bit error.  This is similar to what is used in the receiver of the WHOI Micromodem.
 *
 * b) SINR models
 * - Default Model (ns3::UanPhyCalcSinrDefault), The default SINR model assumes that all transmitted energy is captured at the receiver
 *   and that there is no ISI.  Any received signal power from interferes acts as additional
 *   ambient noise.
 * - FH-FSK SINR Model (ns3::UanPhyCalcSinrFhFsk), The WHOI Micromodem operating in FH-FSK mode uses a predetermined hopping
 *   pattern that is shared by all nodes in the network.  We model this by only including signal
 *   energy receiving within one symbol time (as given by ns3::UanTxMode) in calculating the
 *   received signal power.  A channel clearing time is given to the FH-FSK SINR model via attribute.
 *   Any signal energy arriving in adjacent signals (after a symbol time and the clearing time) is
 *   considered ISI and is treated as additional ambient noise.   Interfering signal arrivals inside
 *   a symbol time (any symbol time) is also counted as additional ambient noise
 * - Frequency filtered SINR (ns3::UanPhyCalcSinrDual).  This SINR model calculates SINR in the same manner
 *   as the default model.  This model however only considers interference if there is an overlap in frequency
 *   of the arriving packets as determined by UanTxMode.
 *
 *  In addition to the generic PHY a dual phy layer is also included (ns3::UanPhyDual).  This wraps two
 *  generic phy layers together to model a net device which includes two receivers.  This was primarily
 *  developed for UanMacRc, described in the next section.
 *
 *\section UanMAC  UAN MAC Model Overview
 *
 * Over the last several years there have been a myriad of underwater MAC proposals
 * in the literature.  We have included three MAC protocols with this distribution:
 * a) CW-MAC, a MAC protocol which uses a slotted contention window similar in nature to
 * the IEEE 802.11 DCF.  Nodes have a constant contention window measured in slot times (configured
 * via attribute).  If the channel is sensed busy, then nodes backoff by randomly (uniform distribution) choose
 * a slot to transmit in.  The slot time durations are also configured via attribute.  This MAC was described in
 *
 * Parrish N.; Tracy L.; Roy S. Arabshahi P.; and Fox, W.,  System Design Considerations for Undersea Networks:
 * Link and Multiple Access Protocols , IEEE Journal on Selected Areas in Communications (JSAC), Special
 * Issue on Underwater Wireless Communications and Networks, Dec. 2008.
 *
 * b) RC-MAC (ns3::UanMacRc ns3::UanMacRcGw) a reservation channel protocol which dynamically divides
 * the available bandwidth into a data channel and a control channel.  This MAC protocol
 * assumes there is a gateway node which all network traffic is destined for.  The current
 * implementation assumes a single gateway and a single network neighborhood (a single hop network).
 * RTS/CTS handshaking is used and time is divided into cycles.  Non-gateway nodes transmit RTS packets
 * on the control channel in parallel to data packet transmissions which were scheduled in the previous cycle
 * at the start of a new cycle, the gateway responds on the data channel with a CTS packet which includes
 * packet transmission times of data packets for received RTS packets in the previous cycle as well as bandwidth
 * allocation information.  At the end of a cycle ACK packets are transmitted for received data packets.
 *
 * When a publication is available it will be cited here.
 *
 * c) Simple ALOHA (ns3::UanMacAloha)  Nodes transmit at will.
 *
 *
 */
