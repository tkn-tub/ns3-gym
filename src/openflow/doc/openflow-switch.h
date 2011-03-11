/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Blake Hurd
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
 * You should have received a copy of the GNU General Public
 * License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307  USA
 *
 * Author: Blake Hurd  <naimorai@gmail.com>
 */

/**
 * \ingroup devices
 * \defgroup OpenFlowSwitchModel OpenFlow Switch Model
 *
 * \section OpenFlowSwitchModelOverview OpenFlow Switch Model Overview
 *
 * The ns-3 OpenFlowSwitch device models an OpenFlow-enabled switch. It is designed to
 * express basic use of the OpenFlow protocol, with the maintaining of a virtual
 * Flow Table and TCAM to provide OpenFlow-like results.
 *
 * The functionality comes down to the Controllers, which send messages to the
 * switch that configure its flows, producing different effects. Controllers can
 * be added by the user, under the ofi namespace extends ofi::Controller. To 
 * demonstrate this, a DropController, which creates flows for ignoring every single
 * packet, and LearningController, which effectively makes the switch a more complicated
 * BridgeNetDevice. A user versed in a standard OFSID, and/or OF protocol, can write
 * virtual controllers to create switches of all kinds of types.
 *
 * \section OpenFlowSwitchModel OpenFlow Switch Model
 *
 * The OpenFlowSwitch device behaves somewhat according to the diagram setup as a classical OFSID
 * switch, with a few modifications made for a proper simulation environment.
 *
 * Normal OF-enabled Switch
 * -----------------------------------
 * | Secure Channel                  | <--OF Protocol--> | Controller is external |
 * | Hardware or Software Flow Table |
 * -----------------------------------
 *
 * ns-3 OF-enabled Switch (module)
 * -------------------------------------
 * | m_controller->ReceiveFromSwitch() | <--OF Protocol--> | Controller is internal |
 * | Software Flow Table, virtual TCAM |
 * -------------------------------------
 *
 * In essence, there are two differences:
 * # No SSL, Embedded Controller: Instead of a secure channel and connecting to an
 * outside location for the Controller program/machine, we currently only allow a
 * Controller extended from ofi::Controller, an extension of an ns3::Object. This
 * means ns-3 programmers cannot model the SSL part of the interface or possibility
 * of network failure. The connection to the OpenFlowSwitch is local and there aren't any
 * reasons for the channel/connection to break down. <<This difference may be an
 * option in the future. Using EmuNetDevices, it should be possible to engage an
 * external Controller program/machine, and thus work with controllers designed
 * outside of the ns-3 environment, that simply use the proper OF protocol when
 * communicating messages to the switch through a tap device.>>
 *
 * # Virtual Flow Table, TCAM: Typical OF-enabled switches are implemented on a hardware
 * TCAM. The OFSID we turn into a library includes a modelled software TCAM, that produces
 * the same results as a hardware TCAM. We include an attribute FlowTableLookupDelay, which
 * allows a simple delay of using the TCAM to be modelled. We don't endeavor to make this
 * delay more complicated, based on the tasks we are running on the TCAM, that is a possible
 * future improvement.
 *
 * \section OpenFlowSwitchNetDeviceModel OpenFlow Switch Net Device Model
 *
 * The OpenFlowSwitch network device is aimed to model an OpenFlow switch, with a TCAM and a connection
 * to a controller program. With some tweaking, it can model every switch type, as per OpenFlow's
 * extensibility. It outsources the complexity of the switch ports to NetDevices of the user's choosing.
 * It should be noted that these NetDevices must behave like practical switch ports, i.e. a Mac Address
 * is assigned, and nothing more. It also must support a SendFrom function so
 * that the OpenFlowSwitch can forward across that port.
 *
 * The OpenFlowSwitchNetDevice provides following Attributes:
 *
 * - FlowTableLookUpDelay:      This time gets run off the clock when making a lookup in our Flow Table.
 * - Flags:        		OpenFlow specific configuration flags. They are defined in the ofp_config_flags enum. Choices include:
 *				OFPC_SEND_FLOW_EXP (OpenFlowSwitch notifies controller when a flow has expired),
 *				OFPC_FRAG_NORMAL (Match fragment against Flow table),
 *				OFPC_FRAG_DROP (Drop fragments),
 *				OFPC_FRAG_REASM (Reassemble only if OFPC_IP_REASM set, which is currently impossible,
 *				because switch implementation does not support IP reassembly)
 *				OFPC_FRAG_MASK (Mask Fragments)
 * - FlowTableMissSendLength:   When the packet doesn't match in our Flow Table, and we forward to the controller,
 *				this sets # of bytes forwarded (packet is not forwarded in its entirety, unless specified).
 *
 * \section OpenFlowSwitchModelSummary OpenFlow Switch Model Summary
 *
 * The ns-3 OpenFlowSwitch device models an OpenFlow-enabled switch. It is designed to
 * express basic use of the OpenFlow protocol, with the maintaining of a virtual
 * Flow Table and TCAM to provide OpenFlow-like results.
 *
 * The functionality comes down to the Controllers, which send messages to the
 * switch that configure its flows, producing different effects. Controllers can
 * be added by the user, under the ofi namespace extends ofi::Controller. To 
 * demonstrate this, a DropController, which creates flows for ignoring every single
 * packet, and LearningController, which effectively makes the switch a more complicated
 * BridgeNetDevice. A user versed in a standard OFSID, and/or OF protocol, can write
 * virtual controllers to create switches of all kinds of types.
 *
 * In order to use the OpenFlowSwitch module, you must create and link the
 * OFSID (OpenFlow Software Implementation Distribution) to ns-3.
 * To do this:
 *
 * #1 Obtain the OFSID code. An ns-3 specific OFSID branch is provided 
 *    to ensure operation with ns-3. The OFSID has several dependencies 
 *    include libxml2, libdl, and the boost libraries. Use mercurial to 
 *    download this branch and waf to build the library:
 *
 * $ hg clone http://code.nsnam.org/bhurd/openflow
 * $ cd openflow
 * $ ./waf configure
 * $ ./waf build
 *
 * #2 Your OFSID is now built into a libopenflow.a library! To
 * link to an ns-3 build with this switch module, run from the ns-3-dev
 * (or whatever you have named your distribution):
 *
 * $ ./waf configure --with-openflow=path/to/openflow
 *
 * #3 Under "---- Summary of optional NS-3 features:", you should see
 * "NS-3 OpenFlow Integration     : enabled", indicating the library
 * has been linked to ns-3. Run:
 *
 * $ ./waf build
 *
 * to build ns-3 and activate the OpenFlowSwitch module.
 *
 * Once set up, you have access to some tests:
 *	
 * For a test suite for the switch module, run:
 *
 * $ ./test.py --suite=openflow
 *
 * For an example demonstrating its use in a simple learning controller/switch, run:
 *
 * $ ./waf --run openflow-switch
 *
 * To see it in detailed logging, run:
 *
 * $ ./waf --run "openflow-switch -v"
 *
 * If you have any questions, read the wiki <http://www.nsnam.org/wiki/index.php/GSOC2010OpenFlow>
 * first, and consider posting to the ns-3 developers mailing list <http://www.isi.edu/nsnam/ns/ns-dev-list.html>,
 * but feel free to reach me at <naimorai@gmail.com>
 */
