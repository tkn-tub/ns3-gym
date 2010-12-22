from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers


import pybindgen.settings
import warnings

class ErrorHandler(pybindgen.settings.ErrorHandler):
    def handle_error(self, wrapper, exception, traceback_):
        warnings.warn("exception %r in wrapper %s" % (exception, wrapper))
        return True
pybindgen.settings.error_handler = ErrorHandler()


import sys
import ns3_module_core
import ns3_module_lte
import ns3_module_simulator
import ns3_module_test
import ns3_module_visualizer
import ns3_module_mobility
import ns3_module_mpi
import ns3_module_common
import ns3_module_contrib
import ns3_module_node
import ns3_module_bridge
import ns3_module_bulk_send
import ns3_module_csma
import ns3_module_emu
import ns3_module_energy
import ns3_module_global_routing
import ns3_module_internet_stack
import ns3_module_list_routing
import ns3_module_onoff
import ns3_module_packet_sink
import ns3_module_point_to_point
import ns3_module_spectrum
import ns3_module_static_routing
import ns3_module_stats
import ns3_module_tap_bridge
import ns3_module_topology_read
import ns3_module_v4ping
import ns3_module_virtual_net_device
import ns3_module_wifi
import ns3_module_wimax
import ns3_module_uan
import ns3_module_aodv
import ns3_module_flow_monitor
import ns3_module_nix_vector_routing
import ns3_module_olsr
import ns3_module_ping6
import ns3_module_radvd
import ns3_module_udp_client_server
import ns3_module_udp_echo
import ns3_module_dsdv
import ns3_module_mesh
import ns3_module_helper
import ns3_module_dot11s
import ns3_module_flame

def module_init():
    root_module = Module('ns3', cpp_namespace='::ns3')
    return root_module

def register_types(module):
    root_module = module.get_root()
    
    root_module.begin_section('ns3_module_core')
    ns3_module_core.register_types(module)
    
    try:
        import ns3_module_core__local
    except ImportError:
        pass
    else:
        ns3_module_core__local.register_types(module)
    
    root_module.end_section('ns3_module_core')
    root_module.begin_section('ns3_module_lte')
    ns3_module_lte.register_types(module)
    
    try:
        import ns3_module_lte__local
    except ImportError:
        pass
    else:
        ns3_module_lte__local.register_types(module)
    
    root_module.end_section('ns3_module_lte')
    root_module.begin_section('ns3_module_simulator')
    ns3_module_simulator.register_types(module)
    
    try:
        import ns3_module_simulator__local
    except ImportError:
        pass
    else:
        ns3_module_simulator__local.register_types(module)
    
    root_module.end_section('ns3_module_simulator')
    root_module.begin_section('ns3_module_test')
    ns3_module_test.register_types(module)
    
    try:
        import ns3_module_test__local
    except ImportError:
        pass
    else:
        ns3_module_test__local.register_types(module)
    
    root_module.end_section('ns3_module_test')
    root_module.begin_section('ns3_module_visualizer')
    ns3_module_visualizer.register_types(module)
    
    try:
        import ns3_module_visualizer__local
    except ImportError:
        pass
    else:
        ns3_module_visualizer__local.register_types(module)
    
    root_module.end_section('ns3_module_visualizer')
    root_module.begin_section('ns3_module_mobility')
    ns3_module_mobility.register_types(module)
    
    try:
        import ns3_module_mobility__local
    except ImportError:
        pass
    else:
        ns3_module_mobility__local.register_types(module)
    
    root_module.end_section('ns3_module_mobility')
    root_module.begin_section('ns3_module_mpi')
    ns3_module_mpi.register_types(module)
    
    try:
        import ns3_module_mpi__local
    except ImportError:
        pass
    else:
        ns3_module_mpi__local.register_types(module)
    
    root_module.end_section('ns3_module_mpi')
    root_module.begin_section('ns3_module_common')
    ns3_module_common.register_types(module)
    
    try:
        import ns3_module_common__local
    except ImportError:
        pass
    else:
        ns3_module_common__local.register_types(module)
    
    root_module.end_section('ns3_module_common')
    root_module.begin_section('ns3_module_contrib')
    ns3_module_contrib.register_types(module)
    
    try:
        import ns3_module_contrib__local
    except ImportError:
        pass
    else:
        ns3_module_contrib__local.register_types(module)
    
    root_module.end_section('ns3_module_contrib')
    root_module.begin_section('ns3_module_node')
    ns3_module_node.register_types(module)
    
    try:
        import ns3_module_node__local
    except ImportError:
        pass
    else:
        ns3_module_node__local.register_types(module)
    
    root_module.end_section('ns3_module_node')
    root_module.begin_section('ns3_module_bridge')
    ns3_module_bridge.register_types(module)
    
    try:
        import ns3_module_bridge__local
    except ImportError:
        pass
    else:
        ns3_module_bridge__local.register_types(module)
    
    root_module.end_section('ns3_module_bridge')
    root_module.begin_section('ns3_module_bulk_send')
    ns3_module_bulk_send.register_types(module)
    
    try:
        import ns3_module_bulk_send__local
    except ImportError:
        pass
    else:
        ns3_module_bulk_send__local.register_types(module)
    
    root_module.end_section('ns3_module_bulk_send')
    root_module.begin_section('ns3_module_csma')
    ns3_module_csma.register_types(module)
    
    try:
        import ns3_module_csma__local
    except ImportError:
        pass
    else:
        ns3_module_csma__local.register_types(module)
    
    root_module.end_section('ns3_module_csma')
    root_module.begin_section('ns3_module_emu')
    ns3_module_emu.register_types(module)
    
    try:
        import ns3_module_emu__local
    except ImportError:
        pass
    else:
        ns3_module_emu__local.register_types(module)
    
    root_module.end_section('ns3_module_emu')
    root_module.begin_section('ns3_module_energy')
    ns3_module_energy.register_types(module)
    
    try:
        import ns3_module_energy__local
    except ImportError:
        pass
    else:
        ns3_module_energy__local.register_types(module)
    
    root_module.end_section('ns3_module_energy')
    root_module.begin_section('ns3_module_global_routing')
    ns3_module_global_routing.register_types(module)
    
    try:
        import ns3_module_global_routing__local
    except ImportError:
        pass
    else:
        ns3_module_global_routing__local.register_types(module)
    
    root_module.end_section('ns3_module_global_routing')
    root_module.begin_section('ns3_module_internet_stack')
    ns3_module_internet_stack.register_types(module)
    
    try:
        import ns3_module_internet_stack__local
    except ImportError:
        pass
    else:
        ns3_module_internet_stack__local.register_types(module)
    
    root_module.end_section('ns3_module_internet_stack')
    root_module.begin_section('ns3_module_list_routing')
    ns3_module_list_routing.register_types(module)
    
    try:
        import ns3_module_list_routing__local
    except ImportError:
        pass
    else:
        ns3_module_list_routing__local.register_types(module)
    
    root_module.end_section('ns3_module_list_routing')
    root_module.begin_section('ns3_module_onoff')
    ns3_module_onoff.register_types(module)
    
    try:
        import ns3_module_onoff__local
    except ImportError:
        pass
    else:
        ns3_module_onoff__local.register_types(module)
    
    root_module.end_section('ns3_module_onoff')
    root_module.begin_section('ns3_module_packet_sink')
    ns3_module_packet_sink.register_types(module)
    
    try:
        import ns3_module_packet_sink__local
    except ImportError:
        pass
    else:
        ns3_module_packet_sink__local.register_types(module)
    
    root_module.end_section('ns3_module_packet_sink')
    root_module.begin_section('ns3_module_point_to_point')
    ns3_module_point_to_point.register_types(module)
    
    try:
        import ns3_module_point_to_point__local
    except ImportError:
        pass
    else:
        ns3_module_point_to_point__local.register_types(module)
    
    root_module.end_section('ns3_module_point_to_point')
    root_module.begin_section('ns3_module_spectrum')
    ns3_module_spectrum.register_types(module)
    
    try:
        import ns3_module_spectrum__local
    except ImportError:
        pass
    else:
        ns3_module_spectrum__local.register_types(module)
    
    root_module.end_section('ns3_module_spectrum')
    root_module.begin_section('ns3_module_static_routing')
    ns3_module_static_routing.register_types(module)
    
    try:
        import ns3_module_static_routing__local
    except ImportError:
        pass
    else:
        ns3_module_static_routing__local.register_types(module)
    
    root_module.end_section('ns3_module_static_routing')
    root_module.begin_section('ns3_module_stats')
    ns3_module_stats.register_types(module)
    
    try:
        import ns3_module_stats__local
    except ImportError:
        pass
    else:
        ns3_module_stats__local.register_types(module)
    
    root_module.end_section('ns3_module_stats')
    root_module.begin_section('ns3_module_tap_bridge')
    ns3_module_tap_bridge.register_types(module)
    
    try:
        import ns3_module_tap_bridge__local
    except ImportError:
        pass
    else:
        ns3_module_tap_bridge__local.register_types(module)
    
    root_module.end_section('ns3_module_tap_bridge')
    root_module.begin_section('ns3_module_topology_read')
    ns3_module_topology_read.register_types(module)
    
    try:
        import ns3_module_topology_read__local
    except ImportError:
        pass
    else:
        ns3_module_topology_read__local.register_types(module)
    
    root_module.end_section('ns3_module_topology_read')
    root_module.begin_section('ns3_module_v4ping')
    ns3_module_v4ping.register_types(module)
    
    try:
        import ns3_module_v4ping__local
    except ImportError:
        pass
    else:
        ns3_module_v4ping__local.register_types(module)
    
    root_module.end_section('ns3_module_v4ping')
    root_module.begin_section('ns3_module_virtual_net_device')
    ns3_module_virtual_net_device.register_types(module)
    
    try:
        import ns3_module_virtual_net_device__local
    except ImportError:
        pass
    else:
        ns3_module_virtual_net_device__local.register_types(module)
    
    root_module.end_section('ns3_module_virtual_net_device')
    root_module.begin_section('ns3_module_wifi')
    ns3_module_wifi.register_types(module)
    
    try:
        import ns3_module_wifi__local
    except ImportError:
        pass
    else:
        ns3_module_wifi__local.register_types(module)
    
    root_module.end_section('ns3_module_wifi')
    root_module.begin_section('ns3_module_wimax')
    ns3_module_wimax.register_types(module)
    
    try:
        import ns3_module_wimax__local
    except ImportError:
        pass
    else:
        ns3_module_wimax__local.register_types(module)
    
    root_module.end_section('ns3_module_wimax')
    root_module.begin_section('ns3_module_uan')
    ns3_module_uan.register_types(module)
    
    try:
        import ns3_module_uan__local
    except ImportError:
        pass
    else:
        ns3_module_uan__local.register_types(module)
    
    root_module.end_section('ns3_module_uan')
    root_module.begin_section('ns3_module_aodv')
    ns3_module_aodv.register_types(module)
    
    try:
        import ns3_module_aodv__local
    except ImportError:
        pass
    else:
        ns3_module_aodv__local.register_types(module)
    
    root_module.end_section('ns3_module_aodv')
    root_module.begin_section('ns3_module_flow_monitor')
    ns3_module_flow_monitor.register_types(module)
    
    try:
        import ns3_module_flow_monitor__local
    except ImportError:
        pass
    else:
        ns3_module_flow_monitor__local.register_types(module)
    
    root_module.end_section('ns3_module_flow_monitor')
    root_module.begin_section('ns3_module_nix_vector_routing')
    ns3_module_nix_vector_routing.register_types(module)
    
    try:
        import ns3_module_nix_vector_routing__local
    except ImportError:
        pass
    else:
        ns3_module_nix_vector_routing__local.register_types(module)
    
    root_module.end_section('ns3_module_nix_vector_routing')
    root_module.begin_section('ns3_module_olsr')
    ns3_module_olsr.register_types(module)
    
    try:
        import ns3_module_olsr__local
    except ImportError:
        pass
    else:
        ns3_module_olsr__local.register_types(module)
    
    root_module.end_section('ns3_module_olsr')
    root_module.begin_section('ns3_module_ping6')
    ns3_module_ping6.register_types(module)
    
    try:
        import ns3_module_ping6__local
    except ImportError:
        pass
    else:
        ns3_module_ping6__local.register_types(module)
    
    root_module.end_section('ns3_module_ping6')
    root_module.begin_section('ns3_module_radvd')
    ns3_module_radvd.register_types(module)
    
    try:
        import ns3_module_radvd__local
    except ImportError:
        pass
    else:
        ns3_module_radvd__local.register_types(module)
    
    root_module.end_section('ns3_module_radvd')
    root_module.begin_section('ns3_module_udp_client_server')
    ns3_module_udp_client_server.register_types(module)
    
    try:
        import ns3_module_udp_client_server__local
    except ImportError:
        pass
    else:
        ns3_module_udp_client_server__local.register_types(module)
    
    root_module.end_section('ns3_module_udp_client_server')
    root_module.begin_section('ns3_module_udp_echo')
    ns3_module_udp_echo.register_types(module)
    
    try:
        import ns3_module_udp_echo__local
    except ImportError:
        pass
    else:
        ns3_module_udp_echo__local.register_types(module)
    
    root_module.end_section('ns3_module_udp_echo')
    root_module.begin_section('ns3_module_dsdv')
    ns3_module_dsdv.register_types(module)
    
    try:
        import ns3_module_dsdv__local
    except ImportError:
        pass
    else:
        ns3_module_dsdv__local.register_types(module)
    
    root_module.end_section('ns3_module_dsdv')
    root_module.begin_section('ns3_module_mesh')
    ns3_module_mesh.register_types(module)
    
    try:
        import ns3_module_mesh__local
    except ImportError:
        pass
    else:
        ns3_module_mesh__local.register_types(module)
    
    root_module.end_section('ns3_module_mesh')
    root_module.begin_section('ns3_module_helper')
    ns3_module_helper.register_types(module)
    
    try:
        import ns3_module_helper__local
    except ImportError:
        pass
    else:
        ns3_module_helper__local.register_types(module)
    
    root_module.end_section('ns3_module_helper')
    root_module.begin_section('ns3_module_dot11s')
    ns3_module_dot11s.register_types(module)
    
    try:
        import ns3_module_dot11s__local
    except ImportError:
        pass
    else:
        ns3_module_dot11s__local.register_types(module)
    
    root_module.end_section('ns3_module_dot11s')
    root_module.begin_section('ns3_module_flame')
    ns3_module_flame.register_types(module)
    
    try:
        import ns3_module_flame__local
    except ImportError:
        pass
    else:
        ns3_module_flame__local.register_types(module)
    
    root_module.end_section('ns3_module_flame')
    ## bridge-helper.h: ns3::BridgeHelper [class]
    module.add_class('BridgeHelper')
    ## device-energy-model-container.h: ns3::DeviceEnergyModelContainer [class]
    module.add_class('DeviceEnergyModelContainer')
    ## energy-model-helper.h: ns3::DeviceEnergyModelHelper [class]
    module.add_class('DeviceEnergyModelHelper', allow_subclassing=True)
    ## energy-model-helper.h: ns3::EnergySourceHelper [class]
    module.add_class('EnergySourceHelper', allow_subclassing=True)
    ## flow-monitor-helper.h: ns3::FlowMonitorHelper [class]
    module.add_class('FlowMonitorHelper')
    ## global-route-manager.h: ns3::GlobalRouteManager [class]
    module.add_class('GlobalRouteManager')
    ## global-router-interface.h: ns3::GlobalRoutingLSA [class]
    module.add_class('GlobalRoutingLSA')
    ## global-router-interface.h: ns3::GlobalRoutingLSA::LSType [enumeration]
    module.add_enum('LSType', ['Unknown', 'RouterLSA', 'NetworkLSA', 'SummaryLSA', 'SummaryLSA_ASBR', 'ASExternalLSAs'], outer_class=root_module['ns3::GlobalRoutingLSA'])
    ## global-router-interface.h: ns3::GlobalRoutingLSA::SPFStatus [enumeration]
    module.add_enum('SPFStatus', ['LSA_SPF_NOT_EXPLORED', 'LSA_SPF_CANDIDATE', 'LSA_SPF_IN_SPFTREE'], outer_class=root_module['ns3::GlobalRoutingLSA'])
    ## global-router-interface.h: ns3::GlobalRoutingLinkRecord [class]
    module.add_class('GlobalRoutingLinkRecord')
    ## global-router-interface.h: ns3::GlobalRoutingLinkRecord::LinkType [enumeration]
    module.add_enum('LinkType', ['Unknown', 'PointToPoint', 'TransitNetwork', 'StubNetwork', 'VirtualLink'], outer_class=root_module['ns3::GlobalRoutingLinkRecord'])
    ## histogram.h: ns3::Histogram [class]
    module.add_class('Histogram')
    ## ipv4-routing-table-entry.h: ns3::Ipv4MulticastRoutingTableEntry [class]
    module.add_class('Ipv4MulticastRoutingTableEntry')
    ## ipv4-routing-table-entry.h: ns3::Ipv4RoutingTableEntry [class]
    module.add_class('Ipv4RoutingTableEntry')
    ## ipv4-static-routing-helper.h: ns3::Ipv4StaticRoutingHelper [class]
    module.add_class('Ipv4StaticRoutingHelper', parent=root_module['ns3::Ipv4RoutingHelper'])
    ## ipv6-routing-table-entry.h: ns3::Ipv6MulticastRoutingTableEntry [class]
    module.add_class('Ipv6MulticastRoutingTableEntry')
    ## ipv6-routing-table-entry.h: ns3::Ipv6RoutingTableEntry [class]
    module.add_class('Ipv6RoutingTableEntry')
    ## ipv6-static-routing-helper.h: ns3::Ipv6StaticRoutingHelper [class]
    module.add_class('Ipv6StaticRoutingHelper', parent=root_module['ns3::Ipv6RoutingHelper'])
    ## lte-helper.h: ns3::LteHelper [class]
    module.add_class('LteHelper')
    ## lte-helper.h: ns3::LteHelper::NetDeviceType [enumeration]
    module.add_enum('NetDeviceType', ['DEVICE_TYPE_USER_EQUIPMENT', 'DEVICE_TYPE_ENODEB'], outer_class=root_module['ns3::LteHelper'])
    ## lte-spectrum-value-helper.h: ns3::LteSpectrumValueHelper [class]
    module.add_class('LteSpectrumValueHelper')
    ## olsr-helper.h: ns3::OlsrHelper [class]
    module.add_class('OlsrHelper', parent=root_module['ns3::Ipv4RoutingHelper'])
    ## olsr-state.h: ns3::OlsrState [class]
    module.add_class('OlsrState')
    ## pyviz.h: ns3::PyViz [class]
    module.add_class('PyViz')
    ## pyviz.h: ns3::PyViz::PacketCaptureMode [enumeration]
    module.add_enum('PacketCaptureMode', ['PACKET_CAPTURE_DISABLED', 'PACKET_CAPTURE_FILTER_HEADERS_OR', 'PACKET_CAPTURE_FILTER_HEADERS_AND'], outer_class=root_module['ns3::PyViz'])
    ## pyviz.h: ns3::PyViz::LastPacketsSample [struct]
    module.add_class('LastPacketsSample', outer_class=root_module['ns3::PyViz'])
    ## pyviz.h: ns3::PyViz::NetDeviceStatistics [struct]
    module.add_class('NetDeviceStatistics', outer_class=root_module['ns3::PyViz'])
    ## pyviz.h: ns3::PyViz::NodeStatistics [struct]
    module.add_class('NodeStatistics', outer_class=root_module['ns3::PyViz'])
    ## pyviz.h: ns3::PyViz::PacketCaptureOptions [struct]
    module.add_class('PacketCaptureOptions', outer_class=root_module['ns3::PyViz'])
    ## pyviz.h: ns3::PyViz::PacketDropSample [struct]
    module.add_class('PacketDropSample', outer_class=root_module['ns3::PyViz'])
    ## pyviz.h: ns3::PyViz::PacketSample [struct]
    module.add_class('PacketSample', outer_class=root_module['ns3::PyViz'])
    ## pyviz.h: ns3::PyViz::RxPacketSample [struct]
    module.add_class('RxPacketSample', parent=root_module['ns3::PyViz::PacketSample'], outer_class=root_module['ns3::PyViz'])
    ## pyviz.h: ns3::PyViz::TransmissionSample [struct]
    module.add_class('TransmissionSample', outer_class=root_module['ns3::PyViz'])
    ## pyviz.h: ns3::PyViz::TxPacketSample [struct]
    module.add_class('TxPacketSample', parent=root_module['ns3::PyViz::PacketSample'], outer_class=root_module['ns3::PyViz'])
    ## uan-mac-rc.h: ns3::Reservation [class]
    module.add_class('Reservation')
    ## rv-battery-model-helper.h: ns3::RvBatteryModelHelper [class]
    module.add_class('RvBatteryModelHelper', parent=root_module['ns3::EnergySourceHelper'])
    ## uan-prop-model.h: ns3::Tap [class]
    module.add_class('Tap')
    ## uan-address.h: ns3::UanAddress [class]
    module.add_class('UanAddress')
    ## uan-address.h: ns3::UanAddress [class]
    root_module['ns3::UanAddress'].implicitly_converts_to(root_module['ns3::Address'])
    ## uan-helper.h: ns3::UanHelper [class]
    module.add_class('UanHelper')
    ## uan-tx-mode.h: ns3::UanModesList [class]
    module.add_class('UanModesList')
    ## uan-transducer.h: ns3::UanPacketArrival [class]
    module.add_class('UanPacketArrival')
    ## uan-prop-model.h: ns3::UanPdp [class]
    module.add_class('UanPdp')
    ## uan-phy.h: ns3::UanPhyListener [class]
    module.add_class('UanPhyListener', allow_subclassing=True)
    ## uan-tx-mode.h: ns3::UanTxMode [class]
    module.add_class('UanTxMode')
    ## uan-tx-mode.h: ns3::UanTxMode::ModulationType [enumeration]
    module.add_enum('ModulationType', ['PSK', 'QAM', 'FSK', 'OTHER'], outer_class=root_module['ns3::UanTxMode'])
    ## uan-tx-mode.h: ns3::UanTxModeFactory [class]
    module.add_class('UanTxModeFactory')
    ## wifi-radio-energy-model-helper.h: ns3::WifiRadioEnergyModelHelper [class]
    module.add_class('WifiRadioEnergyModelHelper', parent=root_module['ns3::DeviceEnergyModelHelper'])
    ## wifi-radio-energy-model.h: ns3::WifiRadioEnergyModelPhyListener [class]
    module.add_class('WifiRadioEnergyModelPhyListener', parent=root_module['ns3::WifiPhyListener'])
    ## acoustic-modem-energy-model-helper.h: ns3::AcousticModemEnergyModelHelper [class]
    module.add_class('AcousticModemEnergyModelHelper', parent=root_module['ns3::DeviceEnergyModelHelper'])
    ## basic-energy-source-helper.h: ns3::BasicEnergySourceHelper [class]
    module.add_class('BasicEnergySourceHelper', parent=root_module['ns3::EnergySourceHelper'])
    ## dsdv-helper.h: ns3::DsdvHelper [class]
    module.add_class('DsdvHelper', parent=root_module['ns3::Ipv4RoutingHelper'])
    ## ipv4-global-routing-helper.h: ns3::Ipv4GlobalRoutingHelper [class]
    module.add_class('Ipv4GlobalRoutingHelper', parent=root_module['ns3::Ipv4RoutingHelper'])
    ## ipv4-list-routing-helper.h: ns3::Ipv4ListRoutingHelper [class]
    module.add_class('Ipv4ListRoutingHelper', parent=root_module['ns3::Ipv4RoutingHelper'])
    ## ipv6-list-routing-helper.h: ns3::Ipv6ListRoutingHelper [class]
    module.add_class('Ipv6ListRoutingHelper', parent=root_module['ns3::Ipv6RoutingHelper'])
    ## lte-mac-header.h: ns3::LteMacHeader [class]
    module.add_class('LteMacHeader', parent=root_module['ns3::Header'])
    ## packet-scheduler.h: ns3::PacketScheduler [class]
    module.add_class('PacketScheduler', parent=root_module['ns3::Object'])
    ## radio-bearer-instance.h: ns3::RadioBearerInstance [class]
    module.add_class('RadioBearerInstance', parent=root_module['ns3::Object'])
    ## radio-bearer-instance.h: ns3::RadioBearerInstance::BearerDirection [enumeration]
    module.add_enum('BearerDirection', ['DIRECTION_TYPE_UL', 'DIRECTION_TYPE_DL'], outer_class=root_module['ns3::RadioBearerInstance'])
    ## radio-bearer-instance.h: ns3::RadioBearerInstance::BearerType [enumeration]
    module.add_enum('BearerType', ['BEARER_TYPE_SRB1', 'BEARER_TYPE_SRB2', 'BEARER_TYPE_DRB'], outer_class=root_module['ns3::RadioBearerInstance'])
    ## rlc-entity.h: ns3::RlcEntity [class]
    module.add_class('RlcEntity', parent=root_module['ns3::Object'])
    ## rrc-entity.h: ns3::RrcEntity [class]
    module.add_class('RrcEntity', parent=root_module['ns3::Object'])
    ## simple-packet-scheduler.h: ns3::SimplePacketScheduler [class]
    module.add_class('SimplePacketScheduler', parent=root_module['ns3::PacketScheduler'])
    ## uan-header-common.h: ns3::UanHeaderCommon [class]
    module.add_class('UanHeaderCommon', parent=root_module['ns3::Header'])
    ## uan-header-rc.h: ns3::UanHeaderRcAck [class]
    module.add_class('UanHeaderRcAck', parent=root_module['ns3::Header'])
    ## uan-header-rc.h: ns3::UanHeaderRcCts [class]
    module.add_class('UanHeaderRcCts', parent=root_module['ns3::Header'])
    ## uan-header-rc.h: ns3::UanHeaderRcCtsGlobal [class]
    module.add_class('UanHeaderRcCtsGlobal', parent=root_module['ns3::Header'])
    ## uan-header-rc.h: ns3::UanHeaderRcData [class]
    module.add_class('UanHeaderRcData', parent=root_module['ns3::Header'])
    ## uan-header-rc.h: ns3::UanHeaderRcRts [class]
    module.add_class('UanHeaderRcRts', parent=root_module['ns3::Header'])
    ## uan-mac.h: ns3::UanMac [class]
    module.add_class('UanMac', parent=root_module['ns3::Object'])
    ## uan-mac-aloha.h: ns3::UanMacAloha [class]
    module.add_class('UanMacAloha', parent=root_module['ns3::UanMac'])
    ## uan-mac-cw.h: ns3::UanMacCw [class]
    module.add_class('UanMacCw', parent=[root_module['ns3::UanMac'], root_module['ns3::UanPhyListener']])
    ## uan-mac-rc.h: ns3::UanMacRc [class]
    module.add_class('UanMacRc', parent=root_module['ns3::UanMac'])
    ## uan-mac-rc.h: ns3::UanMacRc [enumeration]
    module.add_enum('', ['TYPE_DATA', 'TYPE_GWPING', 'TYPE_RTS', 'TYPE_CTS', 'TYPE_ACK'], outer_class=root_module['ns3::UanMacRc'])
    ## uan-mac-rc-gw.h: ns3::UanMacRcGw [class]
    module.add_class('UanMacRcGw', parent=root_module['ns3::UanMac'])
    ## uan-noise-model.h: ns3::UanNoiseModel [class]
    module.add_class('UanNoiseModel', parent=root_module['ns3::Object'])
    ## uan-noise-model-default.h: ns3::UanNoiseModelDefault [class]
    module.add_class('UanNoiseModelDefault', parent=root_module['ns3::UanNoiseModel'])
    ## uan-phy.h: ns3::UanPhy [class]
    module.add_class('UanPhy', parent=root_module['ns3::Object'])
    ## uan-phy.h: ns3::UanPhy::State [enumeration]
    module.add_enum('State', ['IDLE', 'CCABUSY', 'RX', 'TX', 'SLEEP'], outer_class=root_module['ns3::UanPhy'])
    ## uan-phy.h: ns3::UanPhyCalcSinr [class]
    module.add_class('UanPhyCalcSinr', parent=root_module['ns3::Object'])
    ## uan-phy-gen.h: ns3::UanPhyCalcSinrDefault [class]
    module.add_class('UanPhyCalcSinrDefault', parent=root_module['ns3::UanPhyCalcSinr'])
    ## uan-phy-dual.h: ns3::UanPhyCalcSinrDual [class]
    module.add_class('UanPhyCalcSinrDual', parent=root_module['ns3::UanPhyCalcSinr'])
    ## uan-phy-gen.h: ns3::UanPhyCalcSinrFhFsk [class]
    module.add_class('UanPhyCalcSinrFhFsk', parent=root_module['ns3::UanPhyCalcSinr'])
    ## uan-phy-dual.h: ns3::UanPhyDual [class]
    module.add_class('UanPhyDual', parent=root_module['ns3::UanPhy'])
    ## uan-phy-gen.h: ns3::UanPhyGen [class]
    module.add_class('UanPhyGen', parent=root_module['ns3::UanPhy'])
    ## uan-phy.h: ns3::UanPhyPer [class]
    module.add_class('UanPhyPer', parent=root_module['ns3::Object'])
    ## uan-phy-gen.h: ns3::UanPhyPerGenDefault [class]
    module.add_class('UanPhyPerGenDefault', parent=root_module['ns3::UanPhyPer'])
    ## uan-phy-gen.h: ns3::UanPhyPerUmodem [class]
    module.add_class('UanPhyPerUmodem', parent=root_module['ns3::UanPhyPer'])
    ## uan-prop-model.h: ns3::UanPropModel [class]
    module.add_class('UanPropModel', parent=root_module['ns3::Object'])
    ## uan-prop-model-ideal.h: ns3::UanPropModelIdeal [class]
    module.add_class('UanPropModelIdeal', parent=root_module['ns3::UanPropModel'])
    ## uan-prop-model-thorp.h: ns3::UanPropModelThorp [class]
    module.add_class('UanPropModelThorp', parent=root_module['ns3::UanPropModel'])
    ## uan-transducer.h: ns3::UanTransducer [class]
    module.add_class('UanTransducer', parent=root_module['ns3::Object'])
    ## uan-transducer.h: ns3::UanTransducer::State [enumeration]
    module.add_enum('State', ['TX', 'RX'], outer_class=root_module['ns3::UanTransducer'])
    ## uan-transducer-hd.h: ns3::UanTransducerHd [class]
    module.add_class('UanTransducerHd', parent=root_module['ns3::UanTransducer'])
    ## ue-manager.h: ns3::UeManager [class]
    module.add_class('UeManager', parent=root_module['ns3::Object'])
    ## ue-record.h: ns3::UeRecord [class]
    module.add_class('UeRecord', parent=root_module['ns3::Object'])
    ## ue-record.h: ns3::UeRecord::CqiFeedback [struct]
    module.add_class('CqiFeedback', outer_class=root_module['ns3::UeRecord'])
    ## amc-module.h: ns3::AmcModule [class]
    module.add_class('AmcModule', parent=root_module['ns3::Object'])
    ## bearer-qos-parameters.h: ns3::BearerQosParameters [class]
    module.add_class('BearerQosParameters', parent=root_module['ns3::Object'])
    ## bearer-qos-parameters.h: ns3::BearerQosParameters::BearerQosType [enumeration]
    module.add_enum('BearerQosType', ['BEARER_TYPE_GBR', 'BEARER_TYPE_NGBR'], outer_class=root_module['ns3::BearerQosParameters'])
    ## channel-realization.h: ns3::ChannelRealization [class]
    module.add_class('ChannelRealization', parent=root_module['ns3::Object'])
    ## device-energy-model.h: ns3::DeviceEnergyModel [class]
    module.add_class('DeviceEnergyModel', parent=root_module['ns3::Object'])
    ## discrete-time-loss-model.h: ns3::DiscreteTimeLossModel [class]
    module.add_class('DiscreteTimeLossModel', parent=root_module['ns3::Object'])
    ## energy-source.h: ns3::EnergySource [class]
    module.add_class('EnergySource', parent=root_module['ns3::Object'])
    ## energy-source-container.h: ns3::EnergySourceContainer [class]
    module.add_class('EnergySourceContainer', parent=root_module['ns3::Object'])
    ## flow-classifier.h: ns3::FlowClassifier [class]
    module.add_class('FlowClassifier', parent=root_module['ns3::SimpleRefCount< ns3::FlowClassifier, ns3::empty, ns3::DefaultDeleter<ns3::FlowClassifier> >'])
    ## flow-monitor.h: ns3::FlowMonitor [class]
    module.add_class('FlowMonitor', parent=root_module['ns3::Object'])
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats [struct]
    module.add_class('FlowStats', outer_class=root_module['ns3::FlowMonitor'])
    ## flow-probe.h: ns3::FlowProbe [class]
    module.add_class('FlowProbe', parent=root_module['ns3::SimpleRefCount< ns3::FlowProbe, ns3::empty, ns3::DefaultDeleter<ns3::FlowProbe> >'])
    ## flow-probe.h: ns3::FlowProbe::FlowStats [struct]
    module.add_class('FlowStats', outer_class=root_module['ns3::FlowProbe'])
    ## global-router-interface.h: ns3::GlobalRouter [class]
    module.add_class('GlobalRouter', is_singleton=True, parent=root_module['ns3::Object'])
    ## ideal-control-messages.h: ns3::IdealControlMessage [class]
    module.add_class('IdealControlMessage', parent=root_module['ns3::SimpleRefCount< ns3::IdealControlMessage, ns3::empty, ns3::DefaultDeleter<ns3::IdealControlMessage> >'])
    ## ideal-control-messages.h: ns3::IdealControlMessage::MessageType [enumeration]
    module.add_enum('MessageType', ['CQI_FEEDBACKS', 'ALLOCATION_MAP'], outer_class=root_module['ns3::IdealControlMessage'])
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier [class]
    module.add_class('Ipv4FlowClassifier', parent=root_module['ns3::FlowClassifier'])
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::FiveTuple [struct]
    module.add_class('FiveTuple', outer_class=root_module['ns3::Ipv4FlowClassifier'])
    ## ipv4-flow-probe.h: ns3::Ipv4FlowProbe [class]
    module.add_class('Ipv4FlowProbe', parent=root_module['ns3::FlowProbe'])
    ## ipv4-flow-probe.h: ns3::Ipv4FlowProbe::DropReason [enumeration]
    module.add_enum('DropReason', ['DROP_NO_ROUTE', 'DROP_TTL_EXPIRE', 'DROP_BAD_CHECKSUM', 'DROP_QUEUE', 'DROP_INTERFACE_DOWN', 'DROP_ROUTE_ERROR', 'DROP_INVALID_REASON'], outer_class=root_module['ns3::Ipv4FlowProbe'])
    ## ipv4-static-routing.h: ns3::Ipv4StaticRouting [class]
    module.add_class('Ipv4StaticRouting', parent=root_module['ns3::Ipv4RoutingProtocol'])
    ## ipv6-static-routing.h: ns3::Ipv6StaticRouting [class]
    module.add_class('Ipv6StaticRouting', parent=root_module['ns3::Ipv6RoutingProtocol'])
    ## jakes-fading-loss-model.h: ns3::JakesFadingLossModel [class]
    module.add_class('JakesFadingLossModel', parent=root_module['ns3::DiscreteTimeLossModel'])
    ## li-ion-energy-source.h: ns3::LiIonEnergySource [class]
    module.add_class('LiIonEnergySource', parent=root_module['ns3::EnergySource'])
    ## lte-mac-queue.h: ns3::LteMacQueue [class]
    module.add_class('LteMacQueue', parent=root_module['ns3::Object'])
    ## lte-phy.h: ns3::LtePhy [class]
    module.add_class('LtePhy', parent=root_module['ns3::Object'])
    ## lte-propagation-loss-model.h: ns3::LtePropagationLossModel [class]
    module.add_class('LtePropagationLossModel', parent=root_module['ns3::SpectrumPropagationLossModel'])
    ## lte-spectrum-phy.h: ns3::LteSpectrumPhy [class]
    module.add_class('LteSpectrumPhy', parent=root_module['ns3::SpectrumPhy'])
    ## lte-spectrum-phy.h: ns3::LteSpectrumPhy::State [enumeration]
    module.add_enum('State', ['IDLE', 'TX', 'RX'], outer_class=root_module['ns3::LteSpectrumPhy'])
    ## mac-entity.h: ns3::MacEntity [class]
    module.add_class('MacEntity', parent=root_module['ns3::Object'])
    ## path-loss-model.h: ns3::PathLossModel [class]
    module.add_class('PathLossModel', parent=root_module['ns3::DiscreteTimeLossModel'])
    ## ideal-control-messages.h: ns3::PdcchMapIdealControlMessage [class]
    module.add_class('PdcchMapIdealControlMessage', parent=root_module['ns3::IdealControlMessage'])
    ## ideal-control-messages.h: ns3::PdcchMapIdealControlMessage::Direction [enumeration]
    module.add_enum('Direction', ['DOWNLINK', 'UPLINK'], outer_class=root_module['ns3::PdcchMapIdealControlMessage'])
    ## ideal-control-messages.h: ns3::PdcchMapIdealControlMessage::IdealPdcchRecord [struct]
    module.add_class('IdealPdcchRecord', outer_class=root_module['ns3::PdcchMapIdealControlMessage'])
    ## penetration-loss-model.h: ns3::PenetrationLossModel [class]
    module.add_class('PenetrationLossModel', parent=root_module['ns3::DiscreteTimeLossModel'])
    ## rv-battery-model.h: ns3::RvBatteryModel [class]
    module.add_class('RvBatteryModel', parent=root_module['ns3::EnergySource'])
    ## shadowing-loss-model.h: ns3::ShadowingLossModel [class]
    module.add_class('ShadowingLossModel', parent=root_module['ns3::DiscreteTimeLossModel'])
    ## simple-device-energy-model.h: ns3::SimpleDeviceEnergyModel [class]
    module.add_class('SimpleDeviceEnergyModel', parent=root_module['ns3::DeviceEnergyModel'])
    ## uan-channel.h: ns3::UanChannel [class]
    module.add_class('UanChannel', parent=root_module['ns3::Channel'])
    ## uan-tx-mode.h: ns3::UanModesListChecker [class]
    module.add_class('UanModesListChecker', parent=root_module['ns3::AttributeChecker'])
    ## uan-tx-mode.h: ns3::UanModesListValue [class]
    module.add_class('UanModesListValue', parent=root_module['ns3::AttributeValue'])
    ## uan-net-device.h: ns3::UanNetDevice [class]
    module.add_class('UanNetDevice', parent=root_module['ns3::NetDevice'])
    ## ue-phy.h: ns3::UeLtePhy [class]
    module.add_class('UeLtePhy', parent=root_module['ns3::LtePhy'])
    ## ue-lte-spectrum-phy.h: ns3::UeLteSpectrumPhy [class]
    module.add_class('UeLteSpectrumPhy', parent=root_module['ns3::LteSpectrumPhy'])
    ## ue-mac-entity.h: ns3::UeMacEntity [class]
    module.add_class('UeMacEntity', parent=root_module['ns3::MacEntity'])
    ## virtual-net-device.h: ns3::VirtualNetDevice [class]
    module.add_class('VirtualNetDevice', parent=root_module['ns3::NetDevice'])
    ## wifi-radio-energy-model.h: ns3::WifiRadioEnergyModel [class]
    module.add_class('WifiRadioEnergyModel', parent=root_module['ns3::DeviceEnergyModel'])
    ## acoustic-modem-energy-model.h: ns3::AcousticModemEnergyModel [class]
    module.add_class('AcousticModemEnergyModel', parent=root_module['ns3::DeviceEnergyModel'])
    ## basic-energy-source.h: ns3::BasicEnergySource [class]
    module.add_class('BasicEnergySource', parent=root_module['ns3::EnergySource'])
    ## bridge-channel.h: ns3::BridgeChannel [class]
    module.add_class('BridgeChannel', parent=root_module['ns3::Channel'])
    ## bridge-net-device.h: ns3::BridgeNetDevice [class]
    module.add_class('BridgeNetDevice', parent=root_module['ns3::NetDevice'])
    ## ideal-control-messages.h: ns3::CqiIdealControlMessage [class]
    module.add_class('CqiIdealControlMessage', parent=root_module['ns3::IdealControlMessage'])
    ## ideal-control-messages.h: ns3::CqiIdealControlMessage::CqiFeedback [struct]
    module.add_class('CqiFeedback', outer_class=root_module['ns3::CqiIdealControlMessage'])
    ## enb-phy.h: ns3::EnbLtePhy [class]
    module.add_class('EnbLtePhy', parent=root_module['ns3::LtePhy'])
    ## enb-lte-spectrum-phy.h: ns3::EnbLteSpectrumPhy [class]
    module.add_class('EnbLteSpectrumPhy', parent=root_module['ns3::LteSpectrumPhy'])
    ## enb-mac-entity.h: ns3::EnbMacEntity [class]
    module.add_class('EnbMacEntity', parent=root_module['ns3::MacEntity'])
    ## ipv4-global-routing.h: ns3::Ipv4GlobalRouting [class]
    module.add_class('Ipv4GlobalRouting', parent=root_module['ns3::Ipv4RoutingProtocol'])
    ## ipv4-list-routing.h: ns3::Ipv4ListRouting [class]
    module.add_class('Ipv4ListRouting', parent=root_module['ns3::Ipv4RoutingProtocol'])
    ## ipv6-list-routing.h: ns3::Ipv6ListRouting [class]
    module.add_class('Ipv6ListRouting', parent=root_module['ns3::Ipv6RoutingProtocol'])
    ## lte-net-device.h: ns3::LteNetDevice [class]
    module.add_class('LteNetDevice', parent=root_module['ns3::NetDevice'])
    ## ue-net-device.h: ns3::UeNetDevice [class]
    module.add_class('UeNetDevice', parent=root_module['ns3::LteNetDevice'])
    ## enb-net-device.h: ns3::EnbNetDevice [class]
    module.add_class('EnbNetDevice', parent=root_module['ns3::LteNetDevice'])
    module.add_container('std::vector< unsigned int >', 'unsigned int', container_type='vector')
    module.add_container('std::vector< bool >', 'bool', container_type='vector')
    module.add_container('std::vector< int >', 'int', container_type='vector')
    module.add_container('std::vector< ns3::olsr::MprSelectorTuple >', 'ns3::olsr::MprSelectorTuple', container_type='vector')
    module.add_container('std::vector< ns3::olsr::NeighborTuple >', 'ns3::olsr::NeighborTuple', container_type='vector')
    module.add_container('std::vector< ns3::olsr::TwoHopNeighborTuple >', 'ns3::olsr::TwoHopNeighborTuple', container_type='vector')
    module.add_container('std::vector< ns3::olsr::LinkTuple >', 'ns3::olsr::LinkTuple', container_type='vector')
    module.add_container('std::vector< ns3::olsr::TopologyTuple >', 'ns3::olsr::TopologyTuple', container_type='vector')
    module.add_container('std::vector< ns3::olsr::IfaceAssocTuple >', 'ns3::olsr::IfaceAssocTuple', container_type='vector')
    module.add_container('std::vector< ns3::olsr::AssociationTuple >', 'ns3::olsr::AssociationTuple', container_type='vector')
    module.add_container('std::vector< ns3::olsr::Association >', 'ns3::olsr::Association', container_type='vector')
    module.add_container('std::vector< ns3::PyViz::RxPacketSample >', 'ns3::PyViz::RxPacketSample', container_type='vector')
    module.add_container('std::vector< ns3::PyViz::TxPacketSample >', 'ns3::PyViz::TxPacketSample', container_type='vector')
    module.add_container('std::vector< ns3::PyViz::PacketSample >', 'ns3::PyViz::PacketSample', container_type='vector')
    module.add_container('std::vector< ns3::PyViz::TransmissionSample >', 'ns3::PyViz::TransmissionSample', container_type='vector')
    module.add_container('std::vector< ns3::PyViz::PacketDropSample >', 'ns3::PyViz::PacketDropSample', container_type='vector')
    module.add_container('std::vector< ns3::PyViz::NetDeviceStatistics >', 'ns3::PyViz::NetDeviceStatistics', container_type='vector')
    module.add_container('std::vector< std::string >', 'std::string', container_type='vector')
    module.add_container('std::set< unsigned int >', 'unsigned int', container_type='set')
    module.add_container('std::vector< ns3::PyViz::NodeStatistics >', 'ns3::PyViz::NodeStatistics', container_type='vector')
    module.add_container('std::list< std::pair< ns3::Ptr< ns3::Packet >, ns3::UanAddress > >', 'std::pair< ns3::Ptr< ns3::Packet >, ns3::UanAddress >', container_type='list')
    module.add_container('std::vector< ns3::ServiceFlow * >', 'ns3::ServiceFlow *', container_type='vector')
    module.add_container('std::vector< ns3::Tap >', 'ns3::Tap', container_type='vector')
    module.add_container('std::vector< std::complex< double > >', 'std::complex< double >', container_type='vector')
    module.add_container('std::vector< double >', 'double', container_type='vector')
    module.add_container('ns3::bvec', 'bool', container_type='vector')
    module.add_container('std::vector< ns3::SSRecord * >', 'ns3::SSRecord *', container_type='vector')
    module.add_container('std::map< std::string, std::string >', ('std::string', 'std::string'), container_type='map')
    module.add_container('std::set< unsigned char >', 'unsigned char', container_type='set')
    module.add_container('std::list< ns3::UanPacketArrival >', 'ns3::UanPacketArrival', container_type='list')
    module.add_container('std::vector< ns3::UeRecord::CqiFeedback >', 'ns3::UeRecord::CqiFeedback', container_type='vector')
    module.add_container('std::list< std::pair< ns3::OfdmDlMapIe *, ns3::Ptr< ns3::PacketBurst > > >', 'std::pair< ns3::OfdmDlMapIe *, ns3::Ptr< ns3::PacketBurst > >', container_type='list')
    module.add_container('std::vector< unsigned long >', 'long unsigned int', container_type='vector')
    module.add_container('std::map< unsigned int, ns3::FlowMonitor::FlowStats >', ('unsigned int', 'ns3::FlowMonitor::FlowStats'), container_type='map')
    module.add_container('std::map< unsigned int, ns3::FlowProbe::FlowStats >', ('unsigned int', 'ns3::FlowProbe::FlowStats'), container_type='map')
    module.add_container('std::vector< std::vector< double > >', 'std::vector< double >', container_type='vector')
    module.add_container('std::list< unsigned int >', 'unsigned int', container_type='list')
    module.add_container('std::deque< ns3::LteMacQueue::QueueElement >', 'ns3::LteMacQueue::QueueElement', container_type='dequeue')
    module.add_container('std::list< std::pair< ns3::Ptr< ns3::Packet >, ns3::AmsduSubframeHeader > >', 'std::pair< ns3::Ptr< ns3::Packet >, ns3::AmsduSubframeHeader >', container_type='list')
    module.add_container('std::list< ns3::PdcchMapIdealControlMessage::IdealPdcchRecord >', 'ns3::PdcchMapIdealControlMessage::IdealPdcchRecord', container_type='list')
    module.add_container('std::vector< std::pair< ns3::Ptr< ns3::UanNetDevice >, ns3::Ptr< ns3::UanTransducer > > >', 'std::pair< ns3::Ptr< ns3::UanNetDevice >, ns3::Ptr< ns3::UanTransducer > >', container_type='vector')
    module.add_container('std::list< ns3::CqiIdealControlMessage::CqiFeedback >', 'ns3::CqiIdealControlMessage::CqiFeedback', container_type='list')
    typehandlers.add_type_alias('uint32_t', 'ns3::FlowPacketId')
    typehandlers.add_type_alias('uint32_t*', 'ns3::FlowPacketId*')
    typehandlers.add_type_alias('uint32_t&', 'ns3::FlowPacketId&')
    typehandlers.add_type_alias('uint32_t', 'ns3::FlowId')
    typehandlers.add_type_alias('uint32_t*', 'ns3::FlowId*')
    typehandlers.add_type_alias('uint32_t&', 'ns3::FlowId&')
    
    ## Register a nested module for the namespace Config
    
    nested_module = module.add_cpp_namespace('Config')
    register_types_ns3_Config(nested_module)
    
    
    ## Register a nested module for the namespace FatalImpl
    
    nested_module = module.add_cpp_namespace('FatalImpl')
    register_types_ns3_FatalImpl(nested_module)
    
    
    ## Register a nested module for the namespace addressUtils
    
    nested_module = module.add_cpp_namespace('addressUtils')
    register_types_ns3_addressUtils(nested_module)
    
    
    ## Register a nested module for the namespace aodv
    
    nested_module = module.add_cpp_namespace('aodv')
    register_types_ns3_aodv(nested_module)
    
    
    ## Register a nested module for the namespace dot11s
    
    nested_module = module.add_cpp_namespace('dot11s')
    register_types_ns3_dot11s(nested_module)
    
    
    ## Register a nested module for the namespace dsdv
    
    nested_module = module.add_cpp_namespace('dsdv')
    register_types_ns3_dsdv(nested_module)
    
    
    ## Register a nested module for the namespace flame
    
    nested_module = module.add_cpp_namespace('flame')
    register_types_ns3_flame(nested_module)
    
    
    ## Register a nested module for the namespace internal
    
    nested_module = module.add_cpp_namespace('internal')
    register_types_ns3_internal(nested_module)
    
    
    ## Register a nested module for the namespace olsr
    
    nested_module = module.add_cpp_namespace('olsr')
    register_types_ns3_olsr(nested_module)
    

def register_types_ns3_Config(module):
    root_module = module.get_root()
    

def register_types_ns3_FatalImpl(module):
    root_module = module.get_root()
    

def register_types_ns3_addressUtils(module):
    root_module = module.get_root()
    

def register_types_ns3_aodv(module):
    root_module = module.get_root()
    
    module.add_container('std::map< ns3::Ipv4Address, unsigned int >', ('ns3::Ipv4Address', 'unsigned int'), container_type='map')

def register_types_ns3_dot11s(module):
    root_module = module.get_root()
    

def register_types_ns3_dsdv(module):
    root_module = module.get_root()
    
    ## dsdv-rtable.h: ns3::dsdv::RouteFlags [enumeration]
    module.add_enum('RouteFlags', ['VALID', 'INVALID'])
    ## dsdv-packet.h: ns3::dsdv::DsdvHeader [class]
    module.add_class('DsdvHeader', parent=root_module['ns3::Header'])
    ## dsdv-packet-queue.h: ns3::dsdv::PacketQueue [class]
    module.add_class('PacketQueue')
    ## dsdv-packet-queue.h: ns3::dsdv::QueueEntry [class]
    module.add_class('QueueEntry')
    ## dsdv-routing-protocol.h: ns3::dsdv::RoutingProtocol [class]
    module.add_class('RoutingProtocol', parent=root_module['ns3::Ipv4RoutingProtocol'])
    ## dsdv-rtable.h: ns3::dsdv::RoutingTable [class]
    module.add_class('RoutingTable')
    ## dsdv-rtable.h: ns3::dsdv::RoutingTableEntry [class]
    module.add_class('RoutingTableEntry')
    module.add_container('std::map< ns3::Ipv4Address, ns3::dsdv::RoutingTableEntry >', ('ns3::Ipv4Address', 'ns3::dsdv::RoutingTableEntry'), container_type='map')

def register_types_ns3_flame(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    
    ## olsr-repositories.h: ns3::olsr::Association [struct]
    module.add_class('Association')
    ## olsr-repositories.h: ns3::olsr::AssociationTuple [struct]
    module.add_class('AssociationTuple')
    ## olsr-repositories.h: ns3::olsr::DuplicateTuple [struct]
    module.add_class('DuplicateTuple')
    ## olsr-repositories.h: ns3::olsr::IfaceAssocTuple [struct]
    module.add_class('IfaceAssocTuple')
    ## olsr-repositories.h: ns3::olsr::LinkTuple [struct]
    module.add_class('LinkTuple')
    ## olsr-header.h: ns3::olsr::MessageHeader [class]
    module.add_class('MessageHeader', parent=root_module['ns3::Header'])
    ## olsr-header.h: ns3::olsr::MessageHeader::MessageType [enumeration]
    module.add_enum('MessageType', ['HELLO_MESSAGE', 'TC_MESSAGE', 'MID_MESSAGE', 'HNA_MESSAGE'], outer_class=root_module['ns3::olsr::MessageHeader'])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello [struct]
    module.add_class('Hello', outer_class=root_module['ns3::olsr::MessageHeader'])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello::LinkMessage [struct]
    module.add_class('LinkMessage', outer_class=root_module['ns3::olsr::MessageHeader::Hello'])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hna [struct]
    module.add_class('Hna', outer_class=root_module['ns3::olsr::MessageHeader'])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hna::Association [struct]
    module.add_class('Association', outer_class=root_module['ns3::olsr::MessageHeader::Hna'])
    ## olsr-header.h: ns3::olsr::MessageHeader::Mid [struct]
    module.add_class('Mid', outer_class=root_module['ns3::olsr::MessageHeader'])
    ## olsr-header.h: ns3::olsr::MessageHeader::Tc [struct]
    module.add_class('Tc', outer_class=root_module['ns3::olsr::MessageHeader'])
    ## olsr-repositories.h: ns3::olsr::MprSelectorTuple [struct]
    module.add_class('MprSelectorTuple')
    ## olsr-repositories.h: ns3::olsr::NeighborTuple [struct]
    module.add_class('NeighborTuple')
    ## olsr-repositories.h: ns3::olsr::NeighborTuple::Status [enumeration]
    module.add_enum('Status', ['STATUS_NOT_SYM', 'STATUS_SYM'], outer_class=root_module['ns3::olsr::NeighborTuple'])
    ## olsr-routing-protocol.h: ns3::olsr::OlsrMprTestCase [class]
    module.add_class('OlsrMprTestCase', parent=root_module['ns3::TestCase'])
    ## olsr-header.h: ns3::olsr::PacketHeader [class]
    module.add_class('PacketHeader', parent=root_module['ns3::Header'])
    ## olsr-routing-protocol.h: ns3::olsr::RoutingProtocol [class]
    module.add_class('RoutingProtocol', parent=root_module['ns3::Ipv4RoutingProtocol'])
    ## olsr-routing-protocol.h: ns3::olsr::RoutingTableEntry [struct]
    module.add_class('RoutingTableEntry')
    ## olsr-repositories.h: ns3::olsr::TopologyTuple [struct]
    module.add_class('TopologyTuple')
    ## olsr-repositories.h: ns3::olsr::TwoHopNeighborTuple [struct]
    module.add_class('TwoHopNeighborTuple')
    module.add_container('std::vector< ns3::olsr::MessageHeader::Hello::LinkMessage >', 'ns3::olsr::MessageHeader::Hello::LinkMessage', container_type='vector')
    module.add_container('std::vector< ns3::olsr::MessageHeader::Hna::Association >', 'ns3::olsr::MessageHeader::Hna::Association', container_type='vector')
    module.add_container('std::vector< ns3::olsr::RoutingTableEntry >', 'ns3::olsr::RoutingTableEntry', container_type='vector')
    typehandlers.add_type_alias('std::vector< ns3::olsr::TopologyTuple, std::allocator< ns3::olsr::TopologyTuple > >', 'ns3::olsr::TopologySet')
    typehandlers.add_type_alias('std::vector< ns3::olsr::TopologyTuple, std::allocator< ns3::olsr::TopologyTuple > >*', 'ns3::olsr::TopologySet*')
    typehandlers.add_type_alias('std::vector< ns3::olsr::TopologyTuple, std::allocator< ns3::olsr::TopologyTuple > >&', 'ns3::olsr::TopologySet&')
    typehandlers.add_type_alias('std::set< ns3::Ipv4Address, std::less< ns3::Ipv4Address >, std::allocator< ns3::Ipv4Address > >', 'ns3::olsr::MprSet')
    typehandlers.add_type_alias('std::set< ns3::Ipv4Address, std::less< ns3::Ipv4Address >, std::allocator< ns3::Ipv4Address > >*', 'ns3::olsr::MprSet*')
    typehandlers.add_type_alias('std::set< ns3::Ipv4Address, std::less< ns3::Ipv4Address >, std::allocator< ns3::Ipv4Address > >&', 'ns3::olsr::MprSet&')
    typehandlers.add_type_alias('std::vector< ns3::olsr::MprSelectorTuple, std::allocator< ns3::olsr::MprSelectorTuple > >', 'ns3::olsr::MprSelectorSet')
    typehandlers.add_type_alias('std::vector< ns3::olsr::MprSelectorTuple, std::allocator< ns3::olsr::MprSelectorTuple > >*', 'ns3::olsr::MprSelectorSet*')
    typehandlers.add_type_alias('std::vector< ns3::olsr::MprSelectorTuple, std::allocator< ns3::olsr::MprSelectorTuple > >&', 'ns3::olsr::MprSelectorSet&')
    typehandlers.add_type_alias('std::vector< ns3::olsr::Association, std::allocator< ns3::olsr::Association > >', 'ns3::olsr::Associations')
    typehandlers.add_type_alias('std::vector< ns3::olsr::Association, std::allocator< ns3::olsr::Association > >*', 'ns3::olsr::Associations*')
    typehandlers.add_type_alias('std::vector< ns3::olsr::Association, std::allocator< ns3::olsr::Association > >&', 'ns3::olsr::Associations&')
    typehandlers.add_type_alias('std::vector< ns3::olsr::IfaceAssocTuple, std::allocator< ns3::olsr::IfaceAssocTuple > >', 'ns3::olsr::IfaceAssocSet')
    typehandlers.add_type_alias('std::vector< ns3::olsr::IfaceAssocTuple, std::allocator< ns3::olsr::IfaceAssocTuple > >*', 'ns3::olsr::IfaceAssocSet*')
    typehandlers.add_type_alias('std::vector< ns3::olsr::IfaceAssocTuple, std::allocator< ns3::olsr::IfaceAssocTuple > >&', 'ns3::olsr::IfaceAssocSet&')
    typehandlers.add_type_alias('std::vector< ns3::olsr::TwoHopNeighborTuple, std::allocator< ns3::olsr::TwoHopNeighborTuple > >', 'ns3::olsr::TwoHopNeighborSet')
    typehandlers.add_type_alias('std::vector< ns3::olsr::TwoHopNeighborTuple, std::allocator< ns3::olsr::TwoHopNeighborTuple > >*', 'ns3::olsr::TwoHopNeighborSet*')
    typehandlers.add_type_alias('std::vector< ns3::olsr::TwoHopNeighborTuple, std::allocator< ns3::olsr::TwoHopNeighborTuple > >&', 'ns3::olsr::TwoHopNeighborSet&')
    typehandlers.add_type_alias('std::vector< ns3::olsr::DuplicateTuple, std::allocator< ns3::olsr::DuplicateTuple > >', 'ns3::olsr::DuplicateSet')
    typehandlers.add_type_alias('std::vector< ns3::olsr::DuplicateTuple, std::allocator< ns3::olsr::DuplicateTuple > >*', 'ns3::olsr::DuplicateSet*')
    typehandlers.add_type_alias('std::vector< ns3::olsr::DuplicateTuple, std::allocator< ns3::olsr::DuplicateTuple > >&', 'ns3::olsr::DuplicateSet&')
    typehandlers.add_type_alias('std::vector< ns3::olsr::MessageHeader, std::allocator< ns3::olsr::MessageHeader > >', 'ns3::olsr::MessageList')
    typehandlers.add_type_alias('std::vector< ns3::olsr::MessageHeader, std::allocator< ns3::olsr::MessageHeader > >*', 'ns3::olsr::MessageList*')
    typehandlers.add_type_alias('std::vector< ns3::olsr::MessageHeader, std::allocator< ns3::olsr::MessageHeader > >&', 'ns3::olsr::MessageList&')
    typehandlers.add_type_alias('std::vector< ns3::olsr::NeighborTuple, std::allocator< ns3::olsr::NeighborTuple > >', 'ns3::olsr::NeighborSet')
    typehandlers.add_type_alias('std::vector< ns3::olsr::NeighborTuple, std::allocator< ns3::olsr::NeighborTuple > >*', 'ns3::olsr::NeighborSet*')
    typehandlers.add_type_alias('std::vector< ns3::olsr::NeighborTuple, std::allocator< ns3::olsr::NeighborTuple > >&', 'ns3::olsr::NeighborSet&')
    typehandlers.add_type_alias('std::vector< ns3::olsr::AssociationTuple, std::allocator< ns3::olsr::AssociationTuple > >', 'ns3::olsr::AssociationSet')
    typehandlers.add_type_alias('std::vector< ns3::olsr::AssociationTuple, std::allocator< ns3::olsr::AssociationTuple > >*', 'ns3::olsr::AssociationSet*')
    typehandlers.add_type_alias('std::vector< ns3::olsr::AssociationTuple, std::allocator< ns3::olsr::AssociationTuple > >&', 'ns3::olsr::AssociationSet&')
    typehandlers.add_type_alias('std::vector< ns3::olsr::LinkTuple, std::allocator< ns3::olsr::LinkTuple > >', 'ns3::olsr::LinkSet')
    typehandlers.add_type_alias('std::vector< ns3::olsr::LinkTuple, std::allocator< ns3::olsr::LinkTuple > >*', 'ns3::olsr::LinkSet*')
    typehandlers.add_type_alias('std::vector< ns3::olsr::LinkTuple, std::allocator< ns3::olsr::LinkTuple > >&', 'ns3::olsr::LinkSet&')

def register_methods(root_module):
    register_Ns3BridgeHelper_methods(root_module, root_module['ns3::BridgeHelper'])
    register_Ns3DeviceEnergyModelContainer_methods(root_module, root_module['ns3::DeviceEnergyModelContainer'])
    register_Ns3DeviceEnergyModelHelper_methods(root_module, root_module['ns3::DeviceEnergyModelHelper'])
    register_Ns3EnergySourceHelper_methods(root_module, root_module['ns3::EnergySourceHelper'])
    register_Ns3FlowMonitorHelper_methods(root_module, root_module['ns3::FlowMonitorHelper'])
    register_Ns3GlobalRouteManager_methods(root_module, root_module['ns3::GlobalRouteManager'])
    register_Ns3GlobalRoutingLSA_methods(root_module, root_module['ns3::GlobalRoutingLSA'])
    register_Ns3GlobalRoutingLinkRecord_methods(root_module, root_module['ns3::GlobalRoutingLinkRecord'])
    register_Ns3Histogram_methods(root_module, root_module['ns3::Histogram'])
    register_Ns3Ipv4MulticastRoutingTableEntry_methods(root_module, root_module['ns3::Ipv4MulticastRoutingTableEntry'])
    register_Ns3Ipv4RoutingTableEntry_methods(root_module, root_module['ns3::Ipv4RoutingTableEntry'])
    register_Ns3Ipv4StaticRoutingHelper_methods(root_module, root_module['ns3::Ipv4StaticRoutingHelper'])
    register_Ns3Ipv6MulticastRoutingTableEntry_methods(root_module, root_module['ns3::Ipv6MulticastRoutingTableEntry'])
    register_Ns3Ipv6RoutingTableEntry_methods(root_module, root_module['ns3::Ipv6RoutingTableEntry'])
    register_Ns3Ipv6StaticRoutingHelper_methods(root_module, root_module['ns3::Ipv6StaticRoutingHelper'])
    register_Ns3LteHelper_methods(root_module, root_module['ns3::LteHelper'])
    register_Ns3LteSpectrumValueHelper_methods(root_module, root_module['ns3::LteSpectrumValueHelper'])
    register_Ns3OlsrHelper_methods(root_module, root_module['ns3::OlsrHelper'])
    register_Ns3OlsrState_methods(root_module, root_module['ns3::OlsrState'])
    register_Ns3PyViz_methods(root_module, root_module['ns3::PyViz'])
    register_Ns3PyVizLastPacketsSample_methods(root_module, root_module['ns3::PyViz::LastPacketsSample'])
    register_Ns3PyVizNetDeviceStatistics_methods(root_module, root_module['ns3::PyViz::NetDeviceStatistics'])
    register_Ns3PyVizNodeStatistics_methods(root_module, root_module['ns3::PyViz::NodeStatistics'])
    register_Ns3PyVizPacketCaptureOptions_methods(root_module, root_module['ns3::PyViz::PacketCaptureOptions'])
    register_Ns3PyVizPacketDropSample_methods(root_module, root_module['ns3::PyViz::PacketDropSample'])
    register_Ns3PyVizPacketSample_methods(root_module, root_module['ns3::PyViz::PacketSample'])
    register_Ns3PyVizRxPacketSample_methods(root_module, root_module['ns3::PyViz::RxPacketSample'])
    register_Ns3PyVizTransmissionSample_methods(root_module, root_module['ns3::PyViz::TransmissionSample'])
    register_Ns3PyVizTxPacketSample_methods(root_module, root_module['ns3::PyViz::TxPacketSample'])
    register_Ns3Reservation_methods(root_module, root_module['ns3::Reservation'])
    register_Ns3RvBatteryModelHelper_methods(root_module, root_module['ns3::RvBatteryModelHelper'])
    register_Ns3Tap_methods(root_module, root_module['ns3::Tap'])
    register_Ns3UanAddress_methods(root_module, root_module['ns3::UanAddress'])
    register_Ns3UanHelper_methods(root_module, root_module['ns3::UanHelper'])
    register_Ns3UanModesList_methods(root_module, root_module['ns3::UanModesList'])
    register_Ns3UanPacketArrival_methods(root_module, root_module['ns3::UanPacketArrival'])
    register_Ns3UanPdp_methods(root_module, root_module['ns3::UanPdp'])
    register_Ns3UanPhyListener_methods(root_module, root_module['ns3::UanPhyListener'])
    register_Ns3UanTxMode_methods(root_module, root_module['ns3::UanTxMode'])
    register_Ns3UanTxModeFactory_methods(root_module, root_module['ns3::UanTxModeFactory'])
    register_Ns3WifiRadioEnergyModelHelper_methods(root_module, root_module['ns3::WifiRadioEnergyModelHelper'])
    register_Ns3WifiRadioEnergyModelPhyListener_methods(root_module, root_module['ns3::WifiRadioEnergyModelPhyListener'])
    register_Ns3AcousticModemEnergyModelHelper_methods(root_module, root_module['ns3::AcousticModemEnergyModelHelper'])
    register_Ns3BasicEnergySourceHelper_methods(root_module, root_module['ns3::BasicEnergySourceHelper'])
    register_Ns3DsdvHelper_methods(root_module, root_module['ns3::DsdvHelper'])
    register_Ns3Ipv4GlobalRoutingHelper_methods(root_module, root_module['ns3::Ipv4GlobalRoutingHelper'])
    register_Ns3Ipv4ListRoutingHelper_methods(root_module, root_module['ns3::Ipv4ListRoutingHelper'])
    register_Ns3Ipv6ListRoutingHelper_methods(root_module, root_module['ns3::Ipv6ListRoutingHelper'])
    register_Ns3LteMacHeader_methods(root_module, root_module['ns3::LteMacHeader'])
    register_Ns3PacketScheduler_methods(root_module, root_module['ns3::PacketScheduler'])
    register_Ns3RadioBearerInstance_methods(root_module, root_module['ns3::RadioBearerInstance'])
    register_Ns3RlcEntity_methods(root_module, root_module['ns3::RlcEntity'])
    register_Ns3RrcEntity_methods(root_module, root_module['ns3::RrcEntity'])
    register_Ns3SimplePacketScheduler_methods(root_module, root_module['ns3::SimplePacketScheduler'])
    register_Ns3UanHeaderCommon_methods(root_module, root_module['ns3::UanHeaderCommon'])
    register_Ns3UanHeaderRcAck_methods(root_module, root_module['ns3::UanHeaderRcAck'])
    register_Ns3UanHeaderRcCts_methods(root_module, root_module['ns3::UanHeaderRcCts'])
    register_Ns3UanHeaderRcCtsGlobal_methods(root_module, root_module['ns3::UanHeaderRcCtsGlobal'])
    register_Ns3UanHeaderRcData_methods(root_module, root_module['ns3::UanHeaderRcData'])
    register_Ns3UanHeaderRcRts_methods(root_module, root_module['ns3::UanHeaderRcRts'])
    register_Ns3UanMac_methods(root_module, root_module['ns3::UanMac'])
    register_Ns3UanMacAloha_methods(root_module, root_module['ns3::UanMacAloha'])
    register_Ns3UanMacCw_methods(root_module, root_module['ns3::UanMacCw'])
    register_Ns3UanMacRc_methods(root_module, root_module['ns3::UanMacRc'])
    register_Ns3UanMacRcGw_methods(root_module, root_module['ns3::UanMacRcGw'])
    register_Ns3UanNoiseModel_methods(root_module, root_module['ns3::UanNoiseModel'])
    register_Ns3UanNoiseModelDefault_methods(root_module, root_module['ns3::UanNoiseModelDefault'])
    register_Ns3UanPhy_methods(root_module, root_module['ns3::UanPhy'])
    register_Ns3UanPhyCalcSinr_methods(root_module, root_module['ns3::UanPhyCalcSinr'])
    register_Ns3UanPhyCalcSinrDefault_methods(root_module, root_module['ns3::UanPhyCalcSinrDefault'])
    register_Ns3UanPhyCalcSinrDual_methods(root_module, root_module['ns3::UanPhyCalcSinrDual'])
    register_Ns3UanPhyCalcSinrFhFsk_methods(root_module, root_module['ns3::UanPhyCalcSinrFhFsk'])
    register_Ns3UanPhyDual_methods(root_module, root_module['ns3::UanPhyDual'])
    register_Ns3UanPhyGen_methods(root_module, root_module['ns3::UanPhyGen'])
    register_Ns3UanPhyPer_methods(root_module, root_module['ns3::UanPhyPer'])
    register_Ns3UanPhyPerGenDefault_methods(root_module, root_module['ns3::UanPhyPerGenDefault'])
    register_Ns3UanPhyPerUmodem_methods(root_module, root_module['ns3::UanPhyPerUmodem'])
    register_Ns3UanPropModel_methods(root_module, root_module['ns3::UanPropModel'])
    register_Ns3UanPropModelIdeal_methods(root_module, root_module['ns3::UanPropModelIdeal'])
    register_Ns3UanPropModelThorp_methods(root_module, root_module['ns3::UanPropModelThorp'])
    register_Ns3UanTransducer_methods(root_module, root_module['ns3::UanTransducer'])
    register_Ns3UanTransducerHd_methods(root_module, root_module['ns3::UanTransducerHd'])
    register_Ns3UeManager_methods(root_module, root_module['ns3::UeManager'])
    register_Ns3UeRecord_methods(root_module, root_module['ns3::UeRecord'])
    register_Ns3UeRecordCqiFeedback_methods(root_module, root_module['ns3::UeRecord::CqiFeedback'])
    register_Ns3AmcModule_methods(root_module, root_module['ns3::AmcModule'])
    register_Ns3BearerQosParameters_methods(root_module, root_module['ns3::BearerQosParameters'])
    register_Ns3ChannelRealization_methods(root_module, root_module['ns3::ChannelRealization'])
    register_Ns3DeviceEnergyModel_methods(root_module, root_module['ns3::DeviceEnergyModel'])
    register_Ns3DiscreteTimeLossModel_methods(root_module, root_module['ns3::DiscreteTimeLossModel'])
    register_Ns3EnergySource_methods(root_module, root_module['ns3::EnergySource'])
    register_Ns3EnergySourceContainer_methods(root_module, root_module['ns3::EnergySourceContainer'])
    register_Ns3FlowClassifier_methods(root_module, root_module['ns3::FlowClassifier'])
    register_Ns3FlowMonitor_methods(root_module, root_module['ns3::FlowMonitor'])
    register_Ns3FlowMonitorFlowStats_methods(root_module, root_module['ns3::FlowMonitor::FlowStats'])
    register_Ns3FlowProbe_methods(root_module, root_module['ns3::FlowProbe'])
    register_Ns3FlowProbeFlowStats_methods(root_module, root_module['ns3::FlowProbe::FlowStats'])
    register_Ns3GlobalRouter_methods(root_module, root_module['ns3::GlobalRouter'])
    register_Ns3IdealControlMessage_methods(root_module, root_module['ns3::IdealControlMessage'])
    register_Ns3Ipv4FlowClassifier_methods(root_module, root_module['ns3::Ipv4FlowClassifier'])
    register_Ns3Ipv4FlowClassifierFiveTuple_methods(root_module, root_module['ns3::Ipv4FlowClassifier::FiveTuple'])
    register_Ns3Ipv4FlowProbe_methods(root_module, root_module['ns3::Ipv4FlowProbe'])
    register_Ns3Ipv4StaticRouting_methods(root_module, root_module['ns3::Ipv4StaticRouting'])
    register_Ns3Ipv6StaticRouting_methods(root_module, root_module['ns3::Ipv6StaticRouting'])
    register_Ns3JakesFadingLossModel_methods(root_module, root_module['ns3::JakesFadingLossModel'])
    register_Ns3LiIonEnergySource_methods(root_module, root_module['ns3::LiIonEnergySource'])
    register_Ns3LteMacQueue_methods(root_module, root_module['ns3::LteMacQueue'])
    register_Ns3LtePhy_methods(root_module, root_module['ns3::LtePhy'])
    register_Ns3LtePropagationLossModel_methods(root_module, root_module['ns3::LtePropagationLossModel'])
    register_Ns3LteSpectrumPhy_methods(root_module, root_module['ns3::LteSpectrumPhy'])
    register_Ns3MacEntity_methods(root_module, root_module['ns3::MacEntity'])
    register_Ns3PathLossModel_methods(root_module, root_module['ns3::PathLossModel'])
    register_Ns3PdcchMapIdealControlMessage_methods(root_module, root_module['ns3::PdcchMapIdealControlMessage'])
    register_Ns3PdcchMapIdealControlMessageIdealPdcchRecord_methods(root_module, root_module['ns3::PdcchMapIdealControlMessage::IdealPdcchRecord'])
    register_Ns3PenetrationLossModel_methods(root_module, root_module['ns3::PenetrationLossModel'])
    register_Ns3RvBatteryModel_methods(root_module, root_module['ns3::RvBatteryModel'])
    register_Ns3ShadowingLossModel_methods(root_module, root_module['ns3::ShadowingLossModel'])
    register_Ns3SimpleDeviceEnergyModel_methods(root_module, root_module['ns3::SimpleDeviceEnergyModel'])
    register_Ns3UanChannel_methods(root_module, root_module['ns3::UanChannel'])
    register_Ns3UanModesListChecker_methods(root_module, root_module['ns3::UanModesListChecker'])
    register_Ns3UanModesListValue_methods(root_module, root_module['ns3::UanModesListValue'])
    register_Ns3UanNetDevice_methods(root_module, root_module['ns3::UanNetDevice'])
    register_Ns3UeLtePhy_methods(root_module, root_module['ns3::UeLtePhy'])
    register_Ns3UeLteSpectrumPhy_methods(root_module, root_module['ns3::UeLteSpectrumPhy'])
    register_Ns3UeMacEntity_methods(root_module, root_module['ns3::UeMacEntity'])
    register_Ns3VirtualNetDevice_methods(root_module, root_module['ns3::VirtualNetDevice'])
    register_Ns3WifiRadioEnergyModel_methods(root_module, root_module['ns3::WifiRadioEnergyModel'])
    register_Ns3AcousticModemEnergyModel_methods(root_module, root_module['ns3::AcousticModemEnergyModel'])
    register_Ns3BasicEnergySource_methods(root_module, root_module['ns3::BasicEnergySource'])
    register_Ns3BridgeChannel_methods(root_module, root_module['ns3::BridgeChannel'])
    register_Ns3BridgeNetDevice_methods(root_module, root_module['ns3::BridgeNetDevice'])
    register_Ns3CqiIdealControlMessage_methods(root_module, root_module['ns3::CqiIdealControlMessage'])
    register_Ns3CqiIdealControlMessageCqiFeedback_methods(root_module, root_module['ns3::CqiIdealControlMessage::CqiFeedback'])
    register_Ns3EnbLtePhy_methods(root_module, root_module['ns3::EnbLtePhy'])
    register_Ns3EnbLteSpectrumPhy_methods(root_module, root_module['ns3::EnbLteSpectrumPhy'])
    register_Ns3EnbMacEntity_methods(root_module, root_module['ns3::EnbMacEntity'])
    register_Ns3Ipv4GlobalRouting_methods(root_module, root_module['ns3::Ipv4GlobalRouting'])
    register_Ns3Ipv4ListRouting_methods(root_module, root_module['ns3::Ipv4ListRouting'])
    register_Ns3Ipv6ListRouting_methods(root_module, root_module['ns3::Ipv6ListRouting'])
    register_Ns3LteNetDevice_methods(root_module, root_module['ns3::LteNetDevice'])
    register_Ns3UeNetDevice_methods(root_module, root_module['ns3::UeNetDevice'])
    register_Ns3EnbNetDevice_methods(root_module, root_module['ns3::EnbNetDevice'])
    register_Ns3DsdvDsdvHeader_methods(root_module, root_module['ns3::dsdv::DsdvHeader'])
    register_Ns3DsdvPacketQueue_methods(root_module, root_module['ns3::dsdv::PacketQueue'])
    register_Ns3DsdvQueueEntry_methods(root_module, root_module['ns3::dsdv::QueueEntry'])
    register_Ns3DsdvRoutingProtocol_methods(root_module, root_module['ns3::dsdv::RoutingProtocol'])
    register_Ns3DsdvRoutingTable_methods(root_module, root_module['ns3::dsdv::RoutingTable'])
    register_Ns3DsdvRoutingTableEntry_methods(root_module, root_module['ns3::dsdv::RoutingTableEntry'])
    register_Ns3OlsrAssociation_methods(root_module, root_module['ns3::olsr::Association'])
    register_Ns3OlsrAssociationTuple_methods(root_module, root_module['ns3::olsr::AssociationTuple'])
    register_Ns3OlsrDuplicateTuple_methods(root_module, root_module['ns3::olsr::DuplicateTuple'])
    register_Ns3OlsrIfaceAssocTuple_methods(root_module, root_module['ns3::olsr::IfaceAssocTuple'])
    register_Ns3OlsrLinkTuple_methods(root_module, root_module['ns3::olsr::LinkTuple'])
    register_Ns3OlsrMessageHeader_methods(root_module, root_module['ns3::olsr::MessageHeader'])
    register_Ns3OlsrMessageHeaderHello_methods(root_module, root_module['ns3::olsr::MessageHeader::Hello'])
    register_Ns3OlsrMessageHeaderHelloLinkMessage_methods(root_module, root_module['ns3::olsr::MessageHeader::Hello::LinkMessage'])
    register_Ns3OlsrMessageHeaderHna_methods(root_module, root_module['ns3::olsr::MessageHeader::Hna'])
    register_Ns3OlsrMessageHeaderHnaAssociation_methods(root_module, root_module['ns3::olsr::MessageHeader::Hna::Association'])
    register_Ns3OlsrMessageHeaderMid_methods(root_module, root_module['ns3::olsr::MessageHeader::Mid'])
    register_Ns3OlsrMessageHeaderTc_methods(root_module, root_module['ns3::olsr::MessageHeader::Tc'])
    register_Ns3OlsrMprSelectorTuple_methods(root_module, root_module['ns3::olsr::MprSelectorTuple'])
    register_Ns3OlsrNeighborTuple_methods(root_module, root_module['ns3::olsr::NeighborTuple'])
    register_Ns3OlsrOlsrMprTestCase_methods(root_module, root_module['ns3::olsr::OlsrMprTestCase'])
    register_Ns3OlsrPacketHeader_methods(root_module, root_module['ns3::olsr::PacketHeader'])
    register_Ns3OlsrRoutingProtocol_methods(root_module, root_module['ns3::olsr::RoutingProtocol'])
    register_Ns3OlsrRoutingTableEntry_methods(root_module, root_module['ns3::olsr::RoutingTableEntry'])
    register_Ns3OlsrTopologyTuple_methods(root_module, root_module['ns3::olsr::TopologyTuple'])
    register_Ns3OlsrTwoHopNeighborTuple_methods(root_module, root_module['ns3::olsr::TwoHopNeighborTuple'])
    root_module.begin_section('ns3_module_core')
    ns3_module_core.register_methods(root_module)
    
    try:
        import ns3_module_core__local
    except ImportError:
        pass
    else:
        ns3_module_core__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_core')
    root_module.begin_section('ns3_module_lte')
    ns3_module_lte.register_methods(root_module)
    
    try:
        import ns3_module_lte__local
    except ImportError:
        pass
    else:
        ns3_module_lte__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_lte')
    root_module.begin_section('ns3_module_simulator')
    ns3_module_simulator.register_methods(root_module)
    
    try:
        import ns3_module_simulator__local
    except ImportError:
        pass
    else:
        ns3_module_simulator__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_simulator')
    root_module.begin_section('ns3_module_test')
    ns3_module_test.register_methods(root_module)
    
    try:
        import ns3_module_test__local
    except ImportError:
        pass
    else:
        ns3_module_test__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_test')
    root_module.begin_section('ns3_module_visualizer')
    ns3_module_visualizer.register_methods(root_module)
    
    try:
        import ns3_module_visualizer__local
    except ImportError:
        pass
    else:
        ns3_module_visualizer__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_visualizer')
    root_module.begin_section('ns3_module_mobility')
    ns3_module_mobility.register_methods(root_module)
    
    try:
        import ns3_module_mobility__local
    except ImportError:
        pass
    else:
        ns3_module_mobility__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_mobility')
    root_module.begin_section('ns3_module_mpi')
    ns3_module_mpi.register_methods(root_module)
    
    try:
        import ns3_module_mpi__local
    except ImportError:
        pass
    else:
        ns3_module_mpi__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_mpi')
    root_module.begin_section('ns3_module_common')
    ns3_module_common.register_methods(root_module)
    
    try:
        import ns3_module_common__local
    except ImportError:
        pass
    else:
        ns3_module_common__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_common')
    root_module.begin_section('ns3_module_contrib')
    ns3_module_contrib.register_methods(root_module)
    
    try:
        import ns3_module_contrib__local
    except ImportError:
        pass
    else:
        ns3_module_contrib__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_contrib')
    root_module.begin_section('ns3_module_node')
    ns3_module_node.register_methods(root_module)
    
    try:
        import ns3_module_node__local
    except ImportError:
        pass
    else:
        ns3_module_node__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_node')
    root_module.begin_section('ns3_module_bridge')
    ns3_module_bridge.register_methods(root_module)
    
    try:
        import ns3_module_bridge__local
    except ImportError:
        pass
    else:
        ns3_module_bridge__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_bridge')
    root_module.begin_section('ns3_module_bulk_send')
    ns3_module_bulk_send.register_methods(root_module)
    
    try:
        import ns3_module_bulk_send__local
    except ImportError:
        pass
    else:
        ns3_module_bulk_send__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_bulk_send')
    root_module.begin_section('ns3_module_csma')
    ns3_module_csma.register_methods(root_module)
    
    try:
        import ns3_module_csma__local
    except ImportError:
        pass
    else:
        ns3_module_csma__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_csma')
    root_module.begin_section('ns3_module_emu')
    ns3_module_emu.register_methods(root_module)
    
    try:
        import ns3_module_emu__local
    except ImportError:
        pass
    else:
        ns3_module_emu__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_emu')
    root_module.begin_section('ns3_module_energy')
    ns3_module_energy.register_methods(root_module)
    
    try:
        import ns3_module_energy__local
    except ImportError:
        pass
    else:
        ns3_module_energy__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_energy')
    root_module.begin_section('ns3_module_global_routing')
    ns3_module_global_routing.register_methods(root_module)
    
    try:
        import ns3_module_global_routing__local
    except ImportError:
        pass
    else:
        ns3_module_global_routing__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_global_routing')
    root_module.begin_section('ns3_module_internet_stack')
    ns3_module_internet_stack.register_methods(root_module)
    
    try:
        import ns3_module_internet_stack__local
    except ImportError:
        pass
    else:
        ns3_module_internet_stack__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_internet_stack')
    root_module.begin_section('ns3_module_list_routing')
    ns3_module_list_routing.register_methods(root_module)
    
    try:
        import ns3_module_list_routing__local
    except ImportError:
        pass
    else:
        ns3_module_list_routing__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_list_routing')
    root_module.begin_section('ns3_module_onoff')
    ns3_module_onoff.register_methods(root_module)
    
    try:
        import ns3_module_onoff__local
    except ImportError:
        pass
    else:
        ns3_module_onoff__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_onoff')
    root_module.begin_section('ns3_module_packet_sink')
    ns3_module_packet_sink.register_methods(root_module)
    
    try:
        import ns3_module_packet_sink__local
    except ImportError:
        pass
    else:
        ns3_module_packet_sink__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_packet_sink')
    root_module.begin_section('ns3_module_point_to_point')
    ns3_module_point_to_point.register_methods(root_module)
    
    try:
        import ns3_module_point_to_point__local
    except ImportError:
        pass
    else:
        ns3_module_point_to_point__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_point_to_point')
    root_module.begin_section('ns3_module_spectrum')
    ns3_module_spectrum.register_methods(root_module)
    
    try:
        import ns3_module_spectrum__local
    except ImportError:
        pass
    else:
        ns3_module_spectrum__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_spectrum')
    root_module.begin_section('ns3_module_static_routing')
    ns3_module_static_routing.register_methods(root_module)
    
    try:
        import ns3_module_static_routing__local
    except ImportError:
        pass
    else:
        ns3_module_static_routing__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_static_routing')
    root_module.begin_section('ns3_module_stats')
    ns3_module_stats.register_methods(root_module)
    
    try:
        import ns3_module_stats__local
    except ImportError:
        pass
    else:
        ns3_module_stats__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_stats')
    root_module.begin_section('ns3_module_tap_bridge')
    ns3_module_tap_bridge.register_methods(root_module)
    
    try:
        import ns3_module_tap_bridge__local
    except ImportError:
        pass
    else:
        ns3_module_tap_bridge__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_tap_bridge')
    root_module.begin_section('ns3_module_topology_read')
    ns3_module_topology_read.register_methods(root_module)
    
    try:
        import ns3_module_topology_read__local
    except ImportError:
        pass
    else:
        ns3_module_topology_read__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_topology_read')
    root_module.begin_section('ns3_module_v4ping')
    ns3_module_v4ping.register_methods(root_module)
    
    try:
        import ns3_module_v4ping__local
    except ImportError:
        pass
    else:
        ns3_module_v4ping__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_v4ping')
    root_module.begin_section('ns3_module_virtual_net_device')
    ns3_module_virtual_net_device.register_methods(root_module)
    
    try:
        import ns3_module_virtual_net_device__local
    except ImportError:
        pass
    else:
        ns3_module_virtual_net_device__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_virtual_net_device')
    root_module.begin_section('ns3_module_wifi')
    ns3_module_wifi.register_methods(root_module)
    
    try:
        import ns3_module_wifi__local
    except ImportError:
        pass
    else:
        ns3_module_wifi__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_wifi')
    root_module.begin_section('ns3_module_wimax')
    ns3_module_wimax.register_methods(root_module)
    
    try:
        import ns3_module_wimax__local
    except ImportError:
        pass
    else:
        ns3_module_wimax__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_wimax')
    root_module.begin_section('ns3_module_uan')
    ns3_module_uan.register_methods(root_module)
    
    try:
        import ns3_module_uan__local
    except ImportError:
        pass
    else:
        ns3_module_uan__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_uan')
    root_module.begin_section('ns3_module_aodv')
    ns3_module_aodv.register_methods(root_module)
    
    try:
        import ns3_module_aodv__local
    except ImportError:
        pass
    else:
        ns3_module_aodv__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_aodv')
    root_module.begin_section('ns3_module_flow_monitor')
    ns3_module_flow_monitor.register_methods(root_module)
    
    try:
        import ns3_module_flow_monitor__local
    except ImportError:
        pass
    else:
        ns3_module_flow_monitor__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_flow_monitor')
    root_module.begin_section('ns3_module_nix_vector_routing')
    ns3_module_nix_vector_routing.register_methods(root_module)
    
    try:
        import ns3_module_nix_vector_routing__local
    except ImportError:
        pass
    else:
        ns3_module_nix_vector_routing__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_nix_vector_routing')
    root_module.begin_section('ns3_module_olsr')
    ns3_module_olsr.register_methods(root_module)
    
    try:
        import ns3_module_olsr__local
    except ImportError:
        pass
    else:
        ns3_module_olsr__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_olsr')
    root_module.begin_section('ns3_module_ping6')
    ns3_module_ping6.register_methods(root_module)
    
    try:
        import ns3_module_ping6__local
    except ImportError:
        pass
    else:
        ns3_module_ping6__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_ping6')
    root_module.begin_section('ns3_module_radvd')
    ns3_module_radvd.register_methods(root_module)
    
    try:
        import ns3_module_radvd__local
    except ImportError:
        pass
    else:
        ns3_module_radvd__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_radvd')
    root_module.begin_section('ns3_module_udp_client_server')
    ns3_module_udp_client_server.register_methods(root_module)
    
    try:
        import ns3_module_udp_client_server__local
    except ImportError:
        pass
    else:
        ns3_module_udp_client_server__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_udp_client_server')
    root_module.begin_section('ns3_module_udp_echo')
    ns3_module_udp_echo.register_methods(root_module)
    
    try:
        import ns3_module_udp_echo__local
    except ImportError:
        pass
    else:
        ns3_module_udp_echo__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_udp_echo')
    root_module.begin_section('ns3_module_dsdv')
    ns3_module_dsdv.register_methods(root_module)
    
    try:
        import ns3_module_dsdv__local
    except ImportError:
        pass
    else:
        ns3_module_dsdv__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_dsdv')
    root_module.begin_section('ns3_module_mesh')
    ns3_module_mesh.register_methods(root_module)
    
    try:
        import ns3_module_mesh__local
    except ImportError:
        pass
    else:
        ns3_module_mesh__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_mesh')
    root_module.begin_section('ns3_module_helper')
    ns3_module_helper.register_methods(root_module)
    
    try:
        import ns3_module_helper__local
    except ImportError:
        pass
    else:
        ns3_module_helper__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_helper')
    root_module.begin_section('ns3_module_dot11s')
    ns3_module_dot11s.register_methods(root_module)
    
    try:
        import ns3_module_dot11s__local
    except ImportError:
        pass
    else:
        ns3_module_dot11s__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_dot11s')
    root_module.begin_section('ns3_module_flame')
    ns3_module_flame.register_methods(root_module)
    
    try:
        import ns3_module_flame__local
    except ImportError:
        pass
    else:
        ns3_module_flame__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_flame')
    return

def register_Ns3BridgeHelper_methods(root_module, cls):
    ## bridge-helper.h: ns3::BridgeHelper::BridgeHelper(ns3::BridgeHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BridgeHelper const &', 'arg0')])
    ## bridge-helper.h: ns3::BridgeHelper::BridgeHelper() [constructor]
    cls.add_constructor([])
    ## bridge-helper.h: ns3::NetDeviceContainer ns3::BridgeHelper::Install(ns3::Ptr<ns3::Node> node, ns3::NetDeviceContainer c) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::NetDeviceContainer', 'c')])
    ## bridge-helper.h: ns3::NetDeviceContainer ns3::BridgeHelper::Install(std::string nodeName, ns3::NetDeviceContainer c) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('std::string', 'nodeName'), param('ns3::NetDeviceContainer', 'c')])
    ## bridge-helper.h: void ns3::BridgeHelper::SetDeviceAttribute(std::string n1, ns3::AttributeValue const & v1) [member function]
    cls.add_method('SetDeviceAttribute', 
                   'void', 
                   [param('std::string', 'n1'), param('ns3::AttributeValue const &', 'v1')])
    return

def register_Ns3DeviceEnergyModelContainer_methods(root_module, cls):
    ## device-energy-model-container.h: ns3::DeviceEnergyModelContainer::DeviceEnergyModelContainer(ns3::DeviceEnergyModelContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DeviceEnergyModelContainer const &', 'arg0')])
    ## device-energy-model-container.h: ns3::DeviceEnergyModelContainer::DeviceEnergyModelContainer() [constructor]
    cls.add_constructor([])
    ## device-energy-model-container.h: ns3::DeviceEnergyModelContainer::DeviceEnergyModelContainer(ns3::Ptr<ns3::DeviceEnergyModel> model) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::DeviceEnergyModel >', 'model')])
    ## device-energy-model-container.h: ns3::DeviceEnergyModelContainer::DeviceEnergyModelContainer(std::string modelName) [constructor]
    cls.add_constructor([param('std::string', 'modelName')])
    ## device-energy-model-container.h: ns3::DeviceEnergyModelContainer::DeviceEnergyModelContainer(ns3::DeviceEnergyModelContainer const & a, ns3::DeviceEnergyModelContainer const & b) [constructor]
    cls.add_constructor([param('ns3::DeviceEnergyModelContainer const &', 'a'), param('ns3::DeviceEnergyModelContainer const &', 'b')])
    ## device-energy-model-container.h: void ns3::DeviceEnergyModelContainer::Add(ns3::DeviceEnergyModelContainer container) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::DeviceEnergyModelContainer', 'container')])
    ## device-energy-model-container.h: void ns3::DeviceEnergyModelContainer::Add(ns3::Ptr<ns3::DeviceEnergyModel> model) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::DeviceEnergyModel >', 'model')])
    ## device-energy-model-container.h: void ns3::DeviceEnergyModelContainer::Add(std::string modelName) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('std::string', 'modelName')])
    ## device-energy-model-container.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::DeviceEnergyModel>*,std::vector<ns3::Ptr<ns3::DeviceEnergyModel>, std::allocator<ns3::Ptr<ns3::DeviceEnergyModel> > > > ns3::DeviceEnergyModelContainer::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::DeviceEnergyModel > const, std::vector< ns3::Ptr< ns3::DeviceEnergyModel > > >', 
                   [], 
                   is_const=True)
    ## device-energy-model-container.h: void ns3::DeviceEnergyModelContainer::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## device-energy-model-container.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::DeviceEnergyModel>*,std::vector<ns3::Ptr<ns3::DeviceEnergyModel>, std::allocator<ns3::Ptr<ns3::DeviceEnergyModel> > > > ns3::DeviceEnergyModelContainer::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::DeviceEnergyModel > const, std::vector< ns3::Ptr< ns3::DeviceEnergyModel > > >', 
                   [], 
                   is_const=True)
    ## device-energy-model-container.h: ns3::Ptr<ns3::DeviceEnergyModel> ns3::DeviceEnergyModelContainer::Get(uint32_t i) const [member function]
    cls.add_method('Get', 
                   'ns3::Ptr< ns3::DeviceEnergyModel >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## device-energy-model-container.h: uint32_t ns3::DeviceEnergyModelContainer::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3DeviceEnergyModelHelper_methods(root_module, cls):
    ## energy-model-helper.h: ns3::DeviceEnergyModelHelper::DeviceEnergyModelHelper() [constructor]
    cls.add_constructor([])
    ## energy-model-helper.h: ns3::DeviceEnergyModelHelper::DeviceEnergyModelHelper(ns3::DeviceEnergyModelHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DeviceEnergyModelHelper const &', 'arg0')])
    ## energy-model-helper.h: ns3::DeviceEnergyModelContainer ns3::DeviceEnergyModelHelper::Install(ns3::Ptr<ns3::NetDevice> device, ns3::Ptr<ns3::EnergySource> source) const [member function]
    cls.add_method('Install', 
                   'ns3::DeviceEnergyModelContainer', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device'), param('ns3::Ptr< ns3::EnergySource >', 'source')], 
                   is_const=True)
    ## energy-model-helper.h: ns3::DeviceEnergyModelContainer ns3::DeviceEnergyModelHelper::Install(ns3::NetDeviceContainer deviceContainer, ns3::EnergySourceContainer sourceContainer) const [member function]
    cls.add_method('Install', 
                   'ns3::DeviceEnergyModelContainer', 
                   [param('ns3::NetDeviceContainer', 'deviceContainer'), param('ns3::EnergySourceContainer', 'sourceContainer')], 
                   is_const=True)
    ## energy-model-helper.h: void ns3::DeviceEnergyModelHelper::Set(std::string name, ns3::AttributeValue const & v) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'v')], 
                   is_pure_virtual=True, is_virtual=True)
    ## energy-model-helper.h: ns3::Ptr<ns3::DeviceEnergyModel> ns3::DeviceEnergyModelHelper::DoInstall(ns3::Ptr<ns3::NetDevice> device, ns3::Ptr<ns3::EnergySource> source) const [member function]
    cls.add_method('DoInstall', 
                   'ns3::Ptr< ns3::DeviceEnergyModel >', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device'), param('ns3::Ptr< ns3::EnergySource >', 'source')], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3EnergySourceHelper_methods(root_module, cls):
    ## energy-model-helper.h: ns3::EnergySourceHelper::EnergySourceHelper() [constructor]
    cls.add_constructor([])
    ## energy-model-helper.h: ns3::EnergySourceHelper::EnergySourceHelper(ns3::EnergySourceHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EnergySourceHelper const &', 'arg0')])
    ## energy-model-helper.h: ns3::EnergySourceContainer ns3::EnergySourceHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::EnergySourceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## energy-model-helper.h: ns3::EnergySourceContainer ns3::EnergySourceHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::EnergySourceContainer', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## energy-model-helper.h: ns3::EnergySourceContainer ns3::EnergySourceHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::EnergySourceContainer', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## energy-model-helper.h: ns3::EnergySourceContainer ns3::EnergySourceHelper::InstallAll() const [member function]
    cls.add_method('InstallAll', 
                   'ns3::EnergySourceContainer', 
                   [], 
                   is_const=True)
    ## energy-model-helper.h: void ns3::EnergySourceHelper::Set(std::string name, ns3::AttributeValue const & v) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'v')], 
                   is_pure_virtual=True, is_virtual=True)
    ## energy-model-helper.h: ns3::Ptr<ns3::EnergySource> ns3::EnergySourceHelper::DoInstall(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('DoInstall', 
                   'ns3::Ptr< ns3::EnergySource >', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3FlowMonitorHelper_methods(root_module, cls):
    ## flow-monitor-helper.h: ns3::FlowMonitorHelper::FlowMonitorHelper(ns3::FlowMonitorHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FlowMonitorHelper const &', 'arg0')])
    ## flow-monitor-helper.h: ns3::FlowMonitorHelper::FlowMonitorHelper() [constructor]
    cls.add_constructor([])
    ## flow-monitor-helper.h: ns3::Ptr<ns3::FlowClassifier> ns3::FlowMonitorHelper::GetClassifier() [member function]
    cls.add_method('GetClassifier', 
                   'ns3::Ptr< ns3::FlowClassifier >', 
                   [])
    ## flow-monitor-helper.h: ns3::Ptr<ns3::FlowMonitor> ns3::FlowMonitorHelper::GetMonitor() [member function]
    cls.add_method('GetMonitor', 
                   'ns3::Ptr< ns3::FlowMonitor >', 
                   [])
    ## flow-monitor-helper.h: ns3::Ptr<ns3::FlowMonitor> ns3::FlowMonitorHelper::Install(ns3::NodeContainer nodes) [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::FlowMonitor >', 
                   [param('ns3::NodeContainer', 'nodes')])
    ## flow-monitor-helper.h: ns3::Ptr<ns3::FlowMonitor> ns3::FlowMonitorHelper::Install(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::FlowMonitor >', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## flow-monitor-helper.h: ns3::Ptr<ns3::FlowMonitor> ns3::FlowMonitorHelper::InstallAll() [member function]
    cls.add_method('InstallAll', 
                   'ns3::Ptr< ns3::FlowMonitor >', 
                   [])
    ## flow-monitor-helper.h: void ns3::FlowMonitorHelper::SetMonitorAttribute(std::string n1, ns3::AttributeValue const & v1) [member function]
    cls.add_method('SetMonitorAttribute', 
                   'void', 
                   [param('std::string', 'n1'), param('ns3::AttributeValue const &', 'v1')])
    return

def register_Ns3GlobalRouteManager_methods(root_module, cls):
    ## global-route-manager.h: static uint32_t ns3::GlobalRouteManager::AllocateRouterId() [member function]
    cls.add_method('AllocateRouterId', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## global-route-manager.h: static void ns3::GlobalRouteManager::DeleteGlobalRoutes() [member function]
    cls.add_method('DeleteGlobalRoutes', 
                   'void', 
                   [], 
                   is_static=True)
    ## global-route-manager.h: static void ns3::GlobalRouteManager::BuildGlobalRoutingDatabase() [member function]
    cls.add_method('BuildGlobalRoutingDatabase', 
                   'void', 
                   [], 
                   is_static=True)
    ## global-route-manager.h: static void ns3::GlobalRouteManager::InitializeRoutes() [member function]
    cls.add_method('InitializeRoutes', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3GlobalRoutingLSA_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## global-router-interface.h: ns3::GlobalRoutingLSA::GlobalRoutingLSA() [constructor]
    cls.add_constructor([])
    ## global-router-interface.h: ns3::GlobalRoutingLSA::GlobalRoutingLSA(ns3::GlobalRoutingLSA::SPFStatus status, ns3::Ipv4Address linkStateId, ns3::Ipv4Address advertisingRtr) [constructor]
    cls.add_constructor([param('ns3::GlobalRoutingLSA::SPFStatus', 'status'), param('ns3::Ipv4Address', 'linkStateId'), param('ns3::Ipv4Address', 'advertisingRtr')])
    ## global-router-interface.h: ns3::GlobalRoutingLSA::GlobalRoutingLSA(ns3::GlobalRoutingLSA & lsa) [constructor]
    cls.add_constructor([param('ns3::GlobalRoutingLSA &', 'lsa')])
    ## global-router-interface.h: uint32_t ns3::GlobalRoutingLSA::AddAttachedRouter(ns3::Ipv4Address addr) [member function]
    cls.add_method('AddAttachedRouter', 
                   'uint32_t', 
                   [param('ns3::Ipv4Address', 'addr')])
    ## global-router-interface.h: uint32_t ns3::GlobalRoutingLSA::AddLinkRecord(ns3::GlobalRoutingLinkRecord * lr) [member function]
    cls.add_method('AddLinkRecord', 
                   'uint32_t', 
                   [param('ns3::GlobalRoutingLinkRecord *', 'lr')])
    ## global-router-interface.h: void ns3::GlobalRoutingLSA::ClearLinkRecords() [member function]
    cls.add_method('ClearLinkRecords', 
                   'void', 
                   [])
    ## global-router-interface.h: void ns3::GlobalRoutingLSA::CopyLinkRecords(ns3::GlobalRoutingLSA const & lsa) [member function]
    cls.add_method('CopyLinkRecords', 
                   'void', 
                   [param('ns3::GlobalRoutingLSA const &', 'lsa')])
    ## global-router-interface.h: ns3::Ipv4Address ns3::GlobalRoutingLSA::GetAdvertisingRouter() const [member function]
    cls.add_method('GetAdvertisingRouter', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## global-router-interface.h: ns3::Ipv4Address ns3::GlobalRoutingLSA::GetAttachedRouter(uint32_t n) const [member function]
    cls.add_method('GetAttachedRouter', 
                   'ns3::Ipv4Address', 
                   [param('uint32_t', 'n')], 
                   is_const=True)
    ## global-router-interface.h: ns3::GlobalRoutingLSA::LSType ns3::GlobalRoutingLSA::GetLSType() const [member function]
    cls.add_method('GetLSType', 
                   'ns3::GlobalRoutingLSA::LSType', 
                   [], 
                   is_const=True)
    ## global-router-interface.h: ns3::GlobalRoutingLinkRecord * ns3::GlobalRoutingLSA::GetLinkRecord(uint32_t n) const [member function]
    cls.add_method('GetLinkRecord', 
                   'ns3::GlobalRoutingLinkRecord *', 
                   [param('uint32_t', 'n')], 
                   is_const=True)
    ## global-router-interface.h: ns3::Ipv4Address ns3::GlobalRoutingLSA::GetLinkStateId() const [member function]
    cls.add_method('GetLinkStateId', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## global-router-interface.h: uint32_t ns3::GlobalRoutingLSA::GetNAttachedRouters() const [member function]
    cls.add_method('GetNAttachedRouters', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## global-router-interface.h: uint32_t ns3::GlobalRoutingLSA::GetNLinkRecords() const [member function]
    cls.add_method('GetNLinkRecords', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## global-router-interface.h: ns3::Ipv4Mask ns3::GlobalRoutingLSA::GetNetworkLSANetworkMask() const [member function]
    cls.add_method('GetNetworkLSANetworkMask', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_const=True)
    ## global-router-interface.h: ns3::Ptr<ns3::Node> ns3::GlobalRoutingLSA::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True)
    ## global-router-interface.h: ns3::GlobalRoutingLSA::SPFStatus ns3::GlobalRoutingLSA::GetStatus() const [member function]
    cls.add_method('GetStatus', 
                   'ns3::GlobalRoutingLSA::SPFStatus', 
                   [], 
                   is_const=True)
    ## global-router-interface.h: bool ns3::GlobalRoutingLSA::IsEmpty() const [member function]
    cls.add_method('IsEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## global-router-interface.h: void ns3::GlobalRoutingLSA::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## global-router-interface.h: void ns3::GlobalRoutingLSA::SetAdvertisingRouter(ns3::Ipv4Address rtr) [member function]
    cls.add_method('SetAdvertisingRouter', 
                   'void', 
                   [param('ns3::Ipv4Address', 'rtr')])
    ## global-router-interface.h: void ns3::GlobalRoutingLSA::SetLSType(ns3::GlobalRoutingLSA::LSType typ) [member function]
    cls.add_method('SetLSType', 
                   'void', 
                   [param('ns3::GlobalRoutingLSA::LSType', 'typ')])
    ## global-router-interface.h: void ns3::GlobalRoutingLSA::SetLinkStateId(ns3::Ipv4Address addr) [member function]
    cls.add_method('SetLinkStateId', 
                   'void', 
                   [param('ns3::Ipv4Address', 'addr')])
    ## global-router-interface.h: void ns3::GlobalRoutingLSA::SetNetworkLSANetworkMask(ns3::Ipv4Mask mask) [member function]
    cls.add_method('SetNetworkLSANetworkMask', 
                   'void', 
                   [param('ns3::Ipv4Mask', 'mask')])
    ## global-router-interface.h: void ns3::GlobalRoutingLSA::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## global-router-interface.h: void ns3::GlobalRoutingLSA::SetStatus(ns3::GlobalRoutingLSA::SPFStatus status) [member function]
    cls.add_method('SetStatus', 
                   'void', 
                   [param('ns3::GlobalRoutingLSA::SPFStatus', 'status')])
    return

def register_Ns3GlobalRoutingLinkRecord_methods(root_module, cls):
    ## global-router-interface.h: ns3::GlobalRoutingLinkRecord::GlobalRoutingLinkRecord(ns3::GlobalRoutingLinkRecord const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::GlobalRoutingLinkRecord const &', 'arg0')])
    ## global-router-interface.h: ns3::GlobalRoutingLinkRecord::GlobalRoutingLinkRecord() [constructor]
    cls.add_constructor([])
    ## global-router-interface.h: ns3::GlobalRoutingLinkRecord::GlobalRoutingLinkRecord(ns3::GlobalRoutingLinkRecord::LinkType linkType, ns3::Ipv4Address linkId, ns3::Ipv4Address linkData, uint16_t metric) [constructor]
    cls.add_constructor([param('ns3::GlobalRoutingLinkRecord::LinkType', 'linkType'), param('ns3::Ipv4Address', 'linkId'), param('ns3::Ipv4Address', 'linkData'), param('uint16_t', 'metric')])
    ## global-router-interface.h: ns3::Ipv4Address ns3::GlobalRoutingLinkRecord::GetLinkData() const [member function]
    cls.add_method('GetLinkData', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## global-router-interface.h: ns3::Ipv4Address ns3::GlobalRoutingLinkRecord::GetLinkId() const [member function]
    cls.add_method('GetLinkId', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## global-router-interface.h: ns3::GlobalRoutingLinkRecord::LinkType ns3::GlobalRoutingLinkRecord::GetLinkType() const [member function]
    cls.add_method('GetLinkType', 
                   'ns3::GlobalRoutingLinkRecord::LinkType', 
                   [], 
                   is_const=True)
    ## global-router-interface.h: uint16_t ns3::GlobalRoutingLinkRecord::GetMetric() const [member function]
    cls.add_method('GetMetric', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## global-router-interface.h: void ns3::GlobalRoutingLinkRecord::SetLinkData(ns3::Ipv4Address addr) [member function]
    cls.add_method('SetLinkData', 
                   'void', 
                   [param('ns3::Ipv4Address', 'addr')])
    ## global-router-interface.h: void ns3::GlobalRoutingLinkRecord::SetLinkId(ns3::Ipv4Address addr) [member function]
    cls.add_method('SetLinkId', 
                   'void', 
                   [param('ns3::Ipv4Address', 'addr')])
    ## global-router-interface.h: void ns3::GlobalRoutingLinkRecord::SetLinkType(ns3::GlobalRoutingLinkRecord::LinkType linkType) [member function]
    cls.add_method('SetLinkType', 
                   'void', 
                   [param('ns3::GlobalRoutingLinkRecord::LinkType', 'linkType')])
    ## global-router-interface.h: void ns3::GlobalRoutingLinkRecord::SetMetric(uint16_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint16_t', 'metric')])
    return

def register_Ns3Histogram_methods(root_module, cls):
    ## histogram.h: ns3::Histogram::Histogram(ns3::Histogram const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Histogram const &', 'arg0')])
    ## histogram.h: ns3::Histogram::Histogram(double binWidth) [constructor]
    cls.add_constructor([param('double', 'binWidth')])
    ## histogram.h: ns3::Histogram::Histogram() [constructor]
    cls.add_constructor([])
    ## histogram.h: void ns3::Histogram::AddValue(double value) [member function]
    cls.add_method('AddValue', 
                   'void', 
                   [param('double', 'value')])
    ## histogram.h: uint32_t ns3::Histogram::GetBinCount(uint32_t index) [member function]
    cls.add_method('GetBinCount', 
                   'uint32_t', 
                   [param('uint32_t', 'index')])
    ## histogram.h: double ns3::Histogram::GetBinEnd(uint32_t index) [member function]
    cls.add_method('GetBinEnd', 
                   'double', 
                   [param('uint32_t', 'index')])
    ## histogram.h: double ns3::Histogram::GetBinStart(uint32_t index) [member function]
    cls.add_method('GetBinStart', 
                   'double', 
                   [param('uint32_t', 'index')])
    ## histogram.h: double ns3::Histogram::GetBinWidth(uint32_t index) const [member function]
    cls.add_method('GetBinWidth', 
                   'double', 
                   [param('uint32_t', 'index')], 
                   is_const=True)
    ## histogram.h: uint32_t ns3::Histogram::GetNBins() const [member function]
    cls.add_method('GetNBins', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## histogram.h: void ns3::Histogram::SerializeToXmlStream(std::ostream & os, int indent, std::string elementName) const [member function]
    cls.add_method('SerializeToXmlStream', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'indent'), param('std::string', 'elementName')], 
                   is_const=True)
    ## histogram.h: void ns3::Histogram::SetDefaultBinWidth(double binWidth) [member function]
    cls.add_method('SetDefaultBinWidth', 
                   'void', 
                   [param('double', 'binWidth')])
    return

def register_Ns3Ipv4MulticastRoutingTableEntry_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ipv4-routing-table-entry.h: ns3::Ipv4MulticastRoutingTableEntry::Ipv4MulticastRoutingTableEntry() [constructor]
    cls.add_constructor([])
    ## ipv4-routing-table-entry.h: ns3::Ipv4MulticastRoutingTableEntry::Ipv4MulticastRoutingTableEntry(ns3::Ipv4MulticastRoutingTableEntry const & route) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4MulticastRoutingTableEntry const &', 'route')])
    ## ipv4-routing-table-entry.h: ns3::Ipv4MulticastRoutingTableEntry::Ipv4MulticastRoutingTableEntry(ns3::Ipv4MulticastRoutingTableEntry const * route) [constructor]
    cls.add_constructor([param('ns3::Ipv4MulticastRoutingTableEntry const *', 'route')])
    ## ipv4-routing-table-entry.h: static ns3::Ipv4MulticastRoutingTableEntry ns3::Ipv4MulticastRoutingTableEntry::CreateMulticastRoute(ns3::Ipv4Address origin, ns3::Ipv4Address group, uint32_t inputInterface, std::vector<unsigned int, std::allocator<unsigned int> > outputInterfaces) [member function]
    cls.add_method('CreateMulticastRoute', 
                   'ns3::Ipv4MulticastRoutingTableEntry', 
                   [param('ns3::Ipv4Address', 'origin'), param('ns3::Ipv4Address', 'group'), param('uint32_t', 'inputInterface'), param('std::vector< unsigned int >', 'outputInterfaces')], 
                   is_static=True)
    ## ipv4-routing-table-entry.h: ns3::Ipv4Address ns3::Ipv4MulticastRoutingTableEntry::GetGroup() const [member function]
    cls.add_method('GetGroup', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: uint32_t ns3::Ipv4MulticastRoutingTableEntry::GetInputInterface() const [member function]
    cls.add_method('GetInputInterface', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: uint32_t ns3::Ipv4MulticastRoutingTableEntry::GetNOutputInterfaces() const [member function]
    cls.add_method('GetNOutputInterfaces', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: ns3::Ipv4Address ns3::Ipv4MulticastRoutingTableEntry::GetOrigin() const [member function]
    cls.add_method('GetOrigin', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: uint32_t ns3::Ipv4MulticastRoutingTableEntry::GetOutputInterface(uint32_t n) const [member function]
    cls.add_method('GetOutputInterface', 
                   'uint32_t', 
                   [param('uint32_t', 'n')], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: std::vector<unsigned int, std::allocator<unsigned int> > ns3::Ipv4MulticastRoutingTableEntry::GetOutputInterfaces() const [member function]
    cls.add_method('GetOutputInterfaces', 
                   'std::vector< unsigned int >', 
                   [], 
                   is_const=True)
    return

def register_Ns3Ipv4RoutingTableEntry_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ipv4-routing-table-entry.h: ns3::Ipv4RoutingTableEntry::Ipv4RoutingTableEntry() [constructor]
    cls.add_constructor([])
    ## ipv4-routing-table-entry.h: ns3::Ipv4RoutingTableEntry::Ipv4RoutingTableEntry(ns3::Ipv4RoutingTableEntry const & route) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4RoutingTableEntry const &', 'route')])
    ## ipv4-routing-table-entry.h: ns3::Ipv4RoutingTableEntry::Ipv4RoutingTableEntry(ns3::Ipv4RoutingTableEntry const * route) [constructor]
    cls.add_constructor([param('ns3::Ipv4RoutingTableEntry const *', 'route')])
    ## ipv4-routing-table-entry.h: static ns3::Ipv4RoutingTableEntry ns3::Ipv4RoutingTableEntry::CreateDefaultRoute(ns3::Ipv4Address nextHop, uint32_t interface) [member function]
    cls.add_method('CreateDefaultRoute', 
                   'ns3::Ipv4RoutingTableEntry', 
                   [param('ns3::Ipv4Address', 'nextHop'), param('uint32_t', 'interface')], 
                   is_static=True)
    ## ipv4-routing-table-entry.h: static ns3::Ipv4RoutingTableEntry ns3::Ipv4RoutingTableEntry::CreateHostRouteTo(ns3::Ipv4Address dest, ns3::Ipv4Address nextHop, uint32_t interface) [member function]
    cls.add_method('CreateHostRouteTo', 
                   'ns3::Ipv4RoutingTableEntry', 
                   [param('ns3::Ipv4Address', 'dest'), param('ns3::Ipv4Address', 'nextHop'), param('uint32_t', 'interface')], 
                   is_static=True)
    ## ipv4-routing-table-entry.h: static ns3::Ipv4RoutingTableEntry ns3::Ipv4RoutingTableEntry::CreateHostRouteTo(ns3::Ipv4Address dest, uint32_t interface) [member function]
    cls.add_method('CreateHostRouteTo', 
                   'ns3::Ipv4RoutingTableEntry', 
                   [param('ns3::Ipv4Address', 'dest'), param('uint32_t', 'interface')], 
                   is_static=True)
    ## ipv4-routing-table-entry.h: static ns3::Ipv4RoutingTableEntry ns3::Ipv4RoutingTableEntry::CreateNetworkRouteTo(ns3::Ipv4Address network, ns3::Ipv4Mask networkMask, ns3::Ipv4Address nextHop, uint32_t interface) [member function]
    cls.add_method('CreateNetworkRouteTo', 
                   'ns3::Ipv4RoutingTableEntry', 
                   [param('ns3::Ipv4Address', 'network'), param('ns3::Ipv4Mask', 'networkMask'), param('ns3::Ipv4Address', 'nextHop'), param('uint32_t', 'interface')], 
                   is_static=True)
    ## ipv4-routing-table-entry.h: static ns3::Ipv4RoutingTableEntry ns3::Ipv4RoutingTableEntry::CreateNetworkRouteTo(ns3::Ipv4Address network, ns3::Ipv4Mask networkMask, uint32_t interface) [member function]
    cls.add_method('CreateNetworkRouteTo', 
                   'ns3::Ipv4RoutingTableEntry', 
                   [param('ns3::Ipv4Address', 'network'), param('ns3::Ipv4Mask', 'networkMask'), param('uint32_t', 'interface')], 
                   is_static=True)
    ## ipv4-routing-table-entry.h: ns3::Ipv4Address ns3::Ipv4RoutingTableEntry::GetDest() const [member function]
    cls.add_method('GetDest', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: ns3::Ipv4Address ns3::Ipv4RoutingTableEntry::GetDestNetwork() const [member function]
    cls.add_method('GetDestNetwork', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: ns3::Ipv4Mask ns3::Ipv4RoutingTableEntry::GetDestNetworkMask() const [member function]
    cls.add_method('GetDestNetworkMask', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: ns3::Ipv4Address ns3::Ipv4RoutingTableEntry::GetGateway() const [member function]
    cls.add_method('GetGateway', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: uint32_t ns3::Ipv4RoutingTableEntry::GetInterface() const [member function]
    cls.add_method('GetInterface', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: bool ns3::Ipv4RoutingTableEntry::IsDefault() const [member function]
    cls.add_method('IsDefault', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: bool ns3::Ipv4RoutingTableEntry::IsGateway() const [member function]
    cls.add_method('IsGateway', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: bool ns3::Ipv4RoutingTableEntry::IsHost() const [member function]
    cls.add_method('IsHost', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: bool ns3::Ipv4RoutingTableEntry::IsNetwork() const [member function]
    cls.add_method('IsNetwork', 
                   'bool', 
                   [], 
                   is_const=True)
    return

def register_Ns3Ipv4StaticRoutingHelper_methods(root_module, cls):
    ## ipv4-static-routing-helper.h: ns3::Ipv4StaticRoutingHelper::Ipv4StaticRoutingHelper() [constructor]
    cls.add_constructor([])
    ## ipv4-static-routing-helper.h: ns3::Ipv4StaticRoutingHelper::Ipv4StaticRoutingHelper(ns3::Ipv4StaticRoutingHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4StaticRoutingHelper const &', 'arg0')])
    ## ipv4-static-routing-helper.h: ns3::Ipv4StaticRoutingHelper * ns3::Ipv4StaticRoutingHelper::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ipv4StaticRoutingHelper *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-static-routing-helper.h: ns3::Ptr<ns3::Ipv4RoutingProtocol> ns3::Ipv4StaticRoutingHelper::Create(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::Ipv4RoutingProtocol >', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True, is_virtual=True)
    ## ipv4-static-routing-helper.h: ns3::Ptr<ns3::Ipv4StaticRouting> ns3::Ipv4StaticRoutingHelper::GetStaticRouting(ns3::Ptr<ns3::Ipv4> ipv4) const [member function]
    cls.add_method('GetStaticRouting', 
                   'ns3::Ptr< ns3::Ipv4StaticRouting >', 
                   [param('ns3::Ptr< ns3::Ipv4 >', 'ipv4')], 
                   is_const=True)
    ## ipv4-static-routing-helper.h: void ns3::Ipv4StaticRoutingHelper::AddMulticastRoute(ns3::Ptr<ns3::Node> n, ns3::Ipv4Address source, ns3::Ipv4Address group, ns3::Ptr<ns3::NetDevice> input, ns3::NetDeviceContainer output) [member function]
    cls.add_method('AddMulticastRoute', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'n'), param('ns3::Ipv4Address', 'source'), param('ns3::Ipv4Address', 'group'), param('ns3::Ptr< ns3::NetDevice >', 'input'), param('ns3::NetDeviceContainer', 'output')])
    ## ipv4-static-routing-helper.h: void ns3::Ipv4StaticRoutingHelper::AddMulticastRoute(std::string n, ns3::Ipv4Address source, ns3::Ipv4Address group, ns3::Ptr<ns3::NetDevice> input, ns3::NetDeviceContainer output) [member function]
    cls.add_method('AddMulticastRoute', 
                   'void', 
                   [param('std::string', 'n'), param('ns3::Ipv4Address', 'source'), param('ns3::Ipv4Address', 'group'), param('ns3::Ptr< ns3::NetDevice >', 'input'), param('ns3::NetDeviceContainer', 'output')])
    ## ipv4-static-routing-helper.h: void ns3::Ipv4StaticRoutingHelper::AddMulticastRoute(ns3::Ptr<ns3::Node> n, ns3::Ipv4Address source, ns3::Ipv4Address group, std::string inputName, ns3::NetDeviceContainer output) [member function]
    cls.add_method('AddMulticastRoute', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'n'), param('ns3::Ipv4Address', 'source'), param('ns3::Ipv4Address', 'group'), param('std::string', 'inputName'), param('ns3::NetDeviceContainer', 'output')])
    ## ipv4-static-routing-helper.h: void ns3::Ipv4StaticRoutingHelper::AddMulticastRoute(std::string nName, ns3::Ipv4Address source, ns3::Ipv4Address group, std::string inputName, ns3::NetDeviceContainer output) [member function]
    cls.add_method('AddMulticastRoute', 
                   'void', 
                   [param('std::string', 'nName'), param('ns3::Ipv4Address', 'source'), param('ns3::Ipv4Address', 'group'), param('std::string', 'inputName'), param('ns3::NetDeviceContainer', 'output')])
    ## ipv4-static-routing-helper.h: void ns3::Ipv4StaticRoutingHelper::SetDefaultMulticastRoute(ns3::Ptr<ns3::Node> n, ns3::Ptr<ns3::NetDevice> nd) [member function]
    cls.add_method('SetDefaultMulticastRoute', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'n'), param('ns3::Ptr< ns3::NetDevice >', 'nd')])
    ## ipv4-static-routing-helper.h: void ns3::Ipv4StaticRoutingHelper::SetDefaultMulticastRoute(ns3::Ptr<ns3::Node> n, std::string ndName) [member function]
    cls.add_method('SetDefaultMulticastRoute', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'n'), param('std::string', 'ndName')])
    ## ipv4-static-routing-helper.h: void ns3::Ipv4StaticRoutingHelper::SetDefaultMulticastRoute(std::string nName, ns3::Ptr<ns3::NetDevice> nd) [member function]
    cls.add_method('SetDefaultMulticastRoute', 
                   'void', 
                   [param('std::string', 'nName'), param('ns3::Ptr< ns3::NetDevice >', 'nd')])
    ## ipv4-static-routing-helper.h: void ns3::Ipv4StaticRoutingHelper::SetDefaultMulticastRoute(std::string nName, std::string ndName) [member function]
    cls.add_method('SetDefaultMulticastRoute', 
                   'void', 
                   [param('std::string', 'nName'), param('std::string', 'ndName')])
    return

def register_Ns3Ipv6MulticastRoutingTableEntry_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ipv6-routing-table-entry.h: ns3::Ipv6MulticastRoutingTableEntry::Ipv6MulticastRoutingTableEntry() [constructor]
    cls.add_constructor([])
    ## ipv6-routing-table-entry.h: ns3::Ipv6MulticastRoutingTableEntry::Ipv6MulticastRoutingTableEntry(ns3::Ipv6MulticastRoutingTableEntry const & route) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6MulticastRoutingTableEntry const &', 'route')])
    ## ipv6-routing-table-entry.h: ns3::Ipv6MulticastRoutingTableEntry::Ipv6MulticastRoutingTableEntry(ns3::Ipv6MulticastRoutingTableEntry const * route) [constructor]
    cls.add_constructor([param('ns3::Ipv6MulticastRoutingTableEntry const *', 'route')])
    ## ipv6-routing-table-entry.h: static ns3::Ipv6MulticastRoutingTableEntry ns3::Ipv6MulticastRoutingTableEntry::CreateMulticastRoute(ns3::Ipv6Address origin, ns3::Ipv6Address group, uint32_t inputInterface, std::vector<unsigned int, std::allocator<unsigned int> > outputInterfaces) [member function]
    cls.add_method('CreateMulticastRoute', 
                   'ns3::Ipv6MulticastRoutingTableEntry', 
                   [param('ns3::Ipv6Address', 'origin'), param('ns3::Ipv6Address', 'group'), param('uint32_t', 'inputInterface'), param('std::vector< unsigned int >', 'outputInterfaces')], 
                   is_static=True)
    ## ipv6-routing-table-entry.h: ns3::Ipv6Address ns3::Ipv6MulticastRoutingTableEntry::GetGroup() const [member function]
    cls.add_method('GetGroup', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-routing-table-entry.h: uint32_t ns3::Ipv6MulticastRoutingTableEntry::GetInputInterface() const [member function]
    cls.add_method('GetInputInterface', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv6-routing-table-entry.h: uint32_t ns3::Ipv6MulticastRoutingTableEntry::GetNOutputInterfaces() const [member function]
    cls.add_method('GetNOutputInterfaces', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv6-routing-table-entry.h: ns3::Ipv6Address ns3::Ipv6MulticastRoutingTableEntry::GetOrigin() const [member function]
    cls.add_method('GetOrigin', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-routing-table-entry.h: uint32_t ns3::Ipv6MulticastRoutingTableEntry::GetOutputInterface(uint32_t n) const [member function]
    cls.add_method('GetOutputInterface', 
                   'uint32_t', 
                   [param('uint32_t', 'n')], 
                   is_const=True)
    ## ipv6-routing-table-entry.h: std::vector<unsigned int, std::allocator<unsigned int> > ns3::Ipv6MulticastRoutingTableEntry::GetOutputInterfaces() const [member function]
    cls.add_method('GetOutputInterfaces', 
                   'std::vector< unsigned int >', 
                   [], 
                   is_const=True)
    return

def register_Ns3Ipv6RoutingTableEntry_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ipv6-routing-table-entry.h: ns3::Ipv6RoutingTableEntry::Ipv6RoutingTableEntry() [constructor]
    cls.add_constructor([])
    ## ipv6-routing-table-entry.h: ns3::Ipv6RoutingTableEntry::Ipv6RoutingTableEntry(ns3::Ipv6RoutingTableEntry const & route) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6RoutingTableEntry const &', 'route')])
    ## ipv6-routing-table-entry.h: ns3::Ipv6RoutingTableEntry::Ipv6RoutingTableEntry(ns3::Ipv6RoutingTableEntry const * route) [constructor]
    cls.add_constructor([param('ns3::Ipv6RoutingTableEntry const *', 'route')])
    ## ipv6-routing-table-entry.h: static ns3::Ipv6RoutingTableEntry ns3::Ipv6RoutingTableEntry::CreateDefaultRoute(ns3::Ipv6Address nextHop, uint32_t interface) [member function]
    cls.add_method('CreateDefaultRoute', 
                   'ns3::Ipv6RoutingTableEntry', 
                   [param('ns3::Ipv6Address', 'nextHop'), param('uint32_t', 'interface')], 
                   is_static=True)
    ## ipv6-routing-table-entry.h: static ns3::Ipv6RoutingTableEntry ns3::Ipv6RoutingTableEntry::CreateHostRouteTo(ns3::Ipv6Address dest, ns3::Ipv6Address nextHop, uint32_t interface, ns3::Ipv6Address prefixToUse=ns3::Ipv6Address()) [member function]
    cls.add_method('CreateHostRouteTo', 
                   'ns3::Ipv6RoutingTableEntry', 
                   [param('ns3::Ipv6Address', 'dest'), param('ns3::Ipv6Address', 'nextHop'), param('uint32_t', 'interface'), param('ns3::Ipv6Address', 'prefixToUse', default_value='ns3::Ipv6Address()')], 
                   is_static=True)
    ## ipv6-routing-table-entry.h: static ns3::Ipv6RoutingTableEntry ns3::Ipv6RoutingTableEntry::CreateHostRouteTo(ns3::Ipv6Address dest, uint32_t interface) [member function]
    cls.add_method('CreateHostRouteTo', 
                   'ns3::Ipv6RoutingTableEntry', 
                   [param('ns3::Ipv6Address', 'dest'), param('uint32_t', 'interface')], 
                   is_static=True)
    ## ipv6-routing-table-entry.h: static ns3::Ipv6RoutingTableEntry ns3::Ipv6RoutingTableEntry::CreateNetworkRouteTo(ns3::Ipv6Address network, ns3::Ipv6Prefix networkPrefix, ns3::Ipv6Address nextHop, uint32_t interface) [member function]
    cls.add_method('CreateNetworkRouteTo', 
                   'ns3::Ipv6RoutingTableEntry', 
                   [param('ns3::Ipv6Address', 'network'), param('ns3::Ipv6Prefix', 'networkPrefix'), param('ns3::Ipv6Address', 'nextHop'), param('uint32_t', 'interface')], 
                   is_static=True)
    ## ipv6-routing-table-entry.h: static ns3::Ipv6RoutingTableEntry ns3::Ipv6RoutingTableEntry::CreateNetworkRouteTo(ns3::Ipv6Address network, ns3::Ipv6Prefix networkPrefix, ns3::Ipv6Address nextHop, uint32_t interface, ns3::Ipv6Address prefixToUse) [member function]
    cls.add_method('CreateNetworkRouteTo', 
                   'ns3::Ipv6RoutingTableEntry', 
                   [param('ns3::Ipv6Address', 'network'), param('ns3::Ipv6Prefix', 'networkPrefix'), param('ns3::Ipv6Address', 'nextHop'), param('uint32_t', 'interface'), param('ns3::Ipv6Address', 'prefixToUse')], 
                   is_static=True)
    ## ipv6-routing-table-entry.h: static ns3::Ipv6RoutingTableEntry ns3::Ipv6RoutingTableEntry::CreateNetworkRouteTo(ns3::Ipv6Address network, ns3::Ipv6Prefix networkPrefix, uint32_t interface) [member function]
    cls.add_method('CreateNetworkRouteTo', 
                   'ns3::Ipv6RoutingTableEntry', 
                   [param('ns3::Ipv6Address', 'network'), param('ns3::Ipv6Prefix', 'networkPrefix'), param('uint32_t', 'interface')], 
                   is_static=True)
    ## ipv6-routing-table-entry.h: ns3::Ipv6Address ns3::Ipv6RoutingTableEntry::GetDest() const [member function]
    cls.add_method('GetDest', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-routing-table-entry.h: ns3::Ipv6Address ns3::Ipv6RoutingTableEntry::GetDestNetwork() const [member function]
    cls.add_method('GetDestNetwork', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-routing-table-entry.h: ns3::Ipv6Prefix ns3::Ipv6RoutingTableEntry::GetDestNetworkPrefix() const [member function]
    cls.add_method('GetDestNetworkPrefix', 
                   'ns3::Ipv6Prefix', 
                   [], 
                   is_const=True)
    ## ipv6-routing-table-entry.h: ns3::Ipv6Address ns3::Ipv6RoutingTableEntry::GetGateway() const [member function]
    cls.add_method('GetGateway', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-routing-table-entry.h: uint32_t ns3::Ipv6RoutingTableEntry::GetInterface() const [member function]
    cls.add_method('GetInterface', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv6-routing-table-entry.h: ns3::Ipv6Address ns3::Ipv6RoutingTableEntry::GetPrefixToUse() const [member function]
    cls.add_method('GetPrefixToUse', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-routing-table-entry.h: bool ns3::Ipv6RoutingTableEntry::IsDefault() const [member function]
    cls.add_method('IsDefault', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-routing-table-entry.h: bool ns3::Ipv6RoutingTableEntry::IsGateway() const [member function]
    cls.add_method('IsGateway', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-routing-table-entry.h: bool ns3::Ipv6RoutingTableEntry::IsHost() const [member function]
    cls.add_method('IsHost', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-routing-table-entry.h: bool ns3::Ipv6RoutingTableEntry::IsNetwork() const [member function]
    cls.add_method('IsNetwork', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-routing-table-entry.h: void ns3::Ipv6RoutingTableEntry::SetPrefixToUse(ns3::Ipv6Address prefix) [member function]
    cls.add_method('SetPrefixToUse', 
                   'void', 
                   [param('ns3::Ipv6Address', 'prefix')])
    return

def register_Ns3Ipv6StaticRoutingHelper_methods(root_module, cls):
    ## ipv6-static-routing-helper.h: ns3::Ipv6StaticRoutingHelper::Ipv6StaticRoutingHelper() [constructor]
    cls.add_constructor([])
    ## ipv6-static-routing-helper.h: ns3::Ipv6StaticRoutingHelper::Ipv6StaticRoutingHelper(ns3::Ipv6StaticRoutingHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6StaticRoutingHelper const &', 'arg0')])
    ## ipv6-static-routing-helper.h: ns3::Ipv6StaticRoutingHelper * ns3::Ipv6StaticRoutingHelper::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ipv6StaticRoutingHelper *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-static-routing-helper.h: ns3::Ptr<ns3::Ipv6RoutingProtocol> ns3::Ipv6StaticRoutingHelper::Create(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::Ipv6RoutingProtocol >', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True, is_virtual=True)
    ## ipv6-static-routing-helper.h: ns3::Ptr<ns3::Ipv6StaticRouting> ns3::Ipv6StaticRoutingHelper::GetStaticRouting(ns3::Ptr<ns3::Ipv6> ipv6) const [member function]
    cls.add_method('GetStaticRouting', 
                   'ns3::Ptr< ns3::Ipv6StaticRouting >', 
                   [param('ns3::Ptr< ns3::Ipv6 >', 'ipv6')], 
                   is_const=True)
    ## ipv6-static-routing-helper.h: void ns3::Ipv6StaticRoutingHelper::AddMulticastRoute(ns3::Ptr<ns3::Node> n, ns3::Ipv6Address source, ns3::Ipv6Address group, ns3::Ptr<ns3::NetDevice> input, ns3::NetDeviceContainer output) [member function]
    cls.add_method('AddMulticastRoute', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'n'), param('ns3::Ipv6Address', 'source'), param('ns3::Ipv6Address', 'group'), param('ns3::Ptr< ns3::NetDevice >', 'input'), param('ns3::NetDeviceContainer', 'output')])
    ## ipv6-static-routing-helper.h: void ns3::Ipv6StaticRoutingHelper::AddMulticastRoute(std::string n, ns3::Ipv6Address source, ns3::Ipv6Address group, ns3::Ptr<ns3::NetDevice> input, ns3::NetDeviceContainer output) [member function]
    cls.add_method('AddMulticastRoute', 
                   'void', 
                   [param('std::string', 'n'), param('ns3::Ipv6Address', 'source'), param('ns3::Ipv6Address', 'group'), param('ns3::Ptr< ns3::NetDevice >', 'input'), param('ns3::NetDeviceContainer', 'output')])
    ## ipv6-static-routing-helper.h: void ns3::Ipv6StaticRoutingHelper::AddMulticastRoute(ns3::Ptr<ns3::Node> n, ns3::Ipv6Address source, ns3::Ipv6Address group, std::string inputName, ns3::NetDeviceContainer output) [member function]
    cls.add_method('AddMulticastRoute', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'n'), param('ns3::Ipv6Address', 'source'), param('ns3::Ipv6Address', 'group'), param('std::string', 'inputName'), param('ns3::NetDeviceContainer', 'output')])
    ## ipv6-static-routing-helper.h: void ns3::Ipv6StaticRoutingHelper::AddMulticastRoute(std::string nName, ns3::Ipv6Address source, ns3::Ipv6Address group, std::string inputName, ns3::NetDeviceContainer output) [member function]
    cls.add_method('AddMulticastRoute', 
                   'void', 
                   [param('std::string', 'nName'), param('ns3::Ipv6Address', 'source'), param('ns3::Ipv6Address', 'group'), param('std::string', 'inputName'), param('ns3::NetDeviceContainer', 'output')])
    return

def register_Ns3LteHelper_methods(root_module, cls):
    ## lte-helper.h: ns3::LteHelper::LteHelper(ns3::LteHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteHelper const &', 'arg0')])
    ## lte-helper.h: ns3::LteHelper::LteHelper() [constructor]
    cls.add_constructor([])
    ## lte-helper.h: void ns3::LteHelper::AddDownlinkChannelRealization(ns3::Ptr<ns3::MobilityModel> enbMobility, ns3::Ptr<ns3::MobilityModel> ueMobility, ns3::Ptr<ns3::LtePhy> phy) [member function]
    cls.add_method('AddDownlinkChannelRealization', 
                   'void', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'enbMobility'), param('ns3::Ptr< ns3::MobilityModel >', 'ueMobility'), param('ns3::Ptr< ns3::LtePhy >', 'phy')])
    ## lte-helper.h: void ns3::LteHelper::AddMobility(ns3::Ptr<ns3::LtePhy> phy, ns3::Ptr<ns3::MobilityModel> m) [member function]
    cls.add_method('AddMobility', 
                   'void', 
                   [param('ns3::Ptr< ns3::LtePhy >', 'phy'), param('ns3::Ptr< ns3::MobilityModel >', 'm')])
    ## lte-helper.h: void ns3::LteHelper::EnableLogComponents() [member function]
    cls.add_method('EnableLogComponents', 
                   'void', 
                   [])
    ## lte-helper.h: ns3::NetDeviceContainer ns3::LteHelper::Install(ns3::NodeContainer c, ns3::LteHelper::NetDeviceType type) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c'), param('ns3::LteHelper::NetDeviceType', 'type')])
    ## lte-helper.h: void ns3::LteHelper::RegisterUeToTheEnb(ns3::Ptr<ns3::UeNetDevice> ue, ns3::Ptr<ns3::EnbNetDevice> enb) [member function]
    cls.add_method('RegisterUeToTheEnb', 
                   'void', 
                   [param('ns3::Ptr< ns3::UeNetDevice >', 'ue'), param('ns3::Ptr< ns3::EnbNetDevice >', 'enb')])
    return

def register_Ns3LteSpectrumValueHelper_methods(root_module, cls):
    ## lte-spectrum-value-helper.h: ns3::LteSpectrumValueHelper::LteSpectrumValueHelper() [constructor]
    cls.add_constructor([])
    ## lte-spectrum-value-helper.h: ns3::LteSpectrumValueHelper::LteSpectrumValueHelper(ns3::LteSpectrumValueHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteSpectrumValueHelper const &', 'arg0')])
    ## lte-spectrum-value-helper.h: ns3::Ptr<ns3::SpectrumValue> ns3::LteSpectrumValueHelper::CreateDownlinkNoisePowerSpectralDensity() [member function]
    cls.add_method('CreateDownlinkNoisePowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [])
    ## lte-spectrum-value-helper.h: ns3::Ptr<ns3::SpectrumValue> ns3::LteSpectrumValueHelper::CreateDownlinkTxPowerSpectralDensity(double powerTx, std::vector<int, std::allocator<int> > channels) [member function]
    cls.add_method('CreateDownlinkTxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('double', 'powerTx'), param('std::vector< int >', 'channels')])
    ## lte-spectrum-value-helper.h: ns3::Ptr<ns3::SpectrumValue> ns3::LteSpectrumValueHelper::CreateUplinkNoisePowerSpectralDensity() [member function]
    cls.add_method('CreateUplinkNoisePowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [])
    ## lte-spectrum-value-helper.h: ns3::Ptr<ns3::SpectrumValue> ns3::LteSpectrumValueHelper::CreateUplinkTxPowerSpectralDensity(double powerTx, std::vector<int, std::allocator<int> > channels) [member function]
    cls.add_method('CreateUplinkTxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('double', 'powerTx'), param('std::vector< int >', 'channels')])
    return

def register_Ns3OlsrHelper_methods(root_module, cls):
    ## olsr-helper.h: ns3::OlsrHelper::OlsrHelper() [constructor]
    cls.add_constructor([])
    ## olsr-helper.h: ns3::OlsrHelper::OlsrHelper(ns3::OlsrHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OlsrHelper const &', 'arg0')])
    ## olsr-helper.h: ns3::OlsrHelper * ns3::OlsrHelper::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::OlsrHelper *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## olsr-helper.h: void ns3::OlsrHelper::ExcludeInterface(ns3::Ptr<ns3::Node> node, uint32_t interface) [member function]
    cls.add_method('ExcludeInterface', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('uint32_t', 'interface')])
    ## olsr-helper.h: ns3::Ptr<ns3::Ipv4RoutingProtocol> ns3::OlsrHelper::Create(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::Ipv4RoutingProtocol >', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True, is_virtual=True)
    ## olsr-helper.h: void ns3::OlsrHelper::Set(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3OlsrState_methods(root_module, cls):
    ## olsr-state.h: ns3::OlsrState::OlsrState(ns3::OlsrState const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OlsrState const &', 'arg0')])
    ## olsr-state.h: ns3::OlsrState::OlsrState() [constructor]
    cls.add_constructor([])
    ## olsr-state.h: void ns3::OlsrState::EraseAssociation(ns3::olsr::Association const & tuple) [member function]
    cls.add_method('EraseAssociation', 
                   'void', 
                   [param('ns3::olsr::Association const &', 'tuple')])
    ## olsr-state.h: void ns3::OlsrState::EraseAssociationTuple(ns3::olsr::AssociationTuple const & tuple) [member function]
    cls.add_method('EraseAssociationTuple', 
                   'void', 
                   [param('ns3::olsr::AssociationTuple const &', 'tuple')])
    ## olsr-state.h: void ns3::OlsrState::EraseDuplicateTuple(ns3::olsr::DuplicateTuple const & tuple) [member function]
    cls.add_method('EraseDuplicateTuple', 
                   'void', 
                   [param('ns3::olsr::DuplicateTuple const &', 'tuple')])
    ## olsr-state.h: void ns3::OlsrState::EraseIfaceAssocTuple(ns3::olsr::IfaceAssocTuple const & tuple) [member function]
    cls.add_method('EraseIfaceAssocTuple', 
                   'void', 
                   [param('ns3::olsr::IfaceAssocTuple const &', 'tuple')])
    ## olsr-state.h: void ns3::OlsrState::EraseLinkTuple(ns3::olsr::LinkTuple const & tuple) [member function]
    cls.add_method('EraseLinkTuple', 
                   'void', 
                   [param('ns3::olsr::LinkTuple const &', 'tuple')])
    ## olsr-state.h: void ns3::OlsrState::EraseMprSelectorTuple(ns3::olsr::MprSelectorTuple const & tuple) [member function]
    cls.add_method('EraseMprSelectorTuple', 
                   'void', 
                   [param('ns3::olsr::MprSelectorTuple const &', 'tuple')])
    ## olsr-state.h: void ns3::OlsrState::EraseMprSelectorTuples(ns3::Ipv4Address const & mainAddr) [member function]
    cls.add_method('EraseMprSelectorTuples', 
                   'void', 
                   [param('ns3::Ipv4Address const &', 'mainAddr')])
    ## olsr-state.h: void ns3::OlsrState::EraseNeighborTuple(ns3::olsr::NeighborTuple const & neighborTuple) [member function]
    cls.add_method('EraseNeighborTuple', 
                   'void', 
                   [param('ns3::olsr::NeighborTuple const &', 'neighborTuple')])
    ## olsr-state.h: void ns3::OlsrState::EraseNeighborTuple(ns3::Ipv4Address const & mainAddr) [member function]
    cls.add_method('EraseNeighborTuple', 
                   'void', 
                   [param('ns3::Ipv4Address const &', 'mainAddr')])
    ## olsr-state.h: void ns3::OlsrState::EraseOlderTopologyTuples(ns3::Ipv4Address const & lastAddr, uint16_t ansn) [member function]
    cls.add_method('EraseOlderTopologyTuples', 
                   'void', 
                   [param('ns3::Ipv4Address const &', 'lastAddr'), param('uint16_t', 'ansn')])
    ## olsr-state.h: void ns3::OlsrState::EraseTopologyTuple(ns3::olsr::TopologyTuple const & tuple) [member function]
    cls.add_method('EraseTopologyTuple', 
                   'void', 
                   [param('ns3::olsr::TopologyTuple const &', 'tuple')])
    ## olsr-state.h: void ns3::OlsrState::EraseTwoHopNeighborTuple(ns3::olsr::TwoHopNeighborTuple const & tuple) [member function]
    cls.add_method('EraseTwoHopNeighborTuple', 
                   'void', 
                   [param('ns3::olsr::TwoHopNeighborTuple const &', 'tuple')])
    ## olsr-state.h: void ns3::OlsrState::EraseTwoHopNeighborTuples(ns3::Ipv4Address const & neighbor) [member function]
    cls.add_method('EraseTwoHopNeighborTuples', 
                   'void', 
                   [param('ns3::Ipv4Address const &', 'neighbor')])
    ## olsr-state.h: void ns3::OlsrState::EraseTwoHopNeighborTuples(ns3::Ipv4Address const & neighbor, ns3::Ipv4Address const & twoHopNeighbor) [member function]
    cls.add_method('EraseTwoHopNeighborTuples', 
                   'void', 
                   [param('ns3::Ipv4Address const &', 'neighbor'), param('ns3::Ipv4Address const &', 'twoHopNeighbor')])
    ## olsr-state.h: ns3::olsr::AssociationTuple * ns3::OlsrState::FindAssociationTuple(ns3::Ipv4Address const & gatewayAddr, ns3::Ipv4Address const & networkAddr, ns3::Ipv4Mask const & netmask) [member function]
    cls.add_method('FindAssociationTuple', 
                   'ns3::olsr::AssociationTuple *', 
                   [param('ns3::Ipv4Address const &', 'gatewayAddr'), param('ns3::Ipv4Address const &', 'networkAddr'), param('ns3::Ipv4Mask const &', 'netmask')])
    ## olsr-state.h: ns3::olsr::DuplicateTuple * ns3::OlsrState::FindDuplicateTuple(ns3::Ipv4Address const & address, uint16_t sequenceNumber) [member function]
    cls.add_method('FindDuplicateTuple', 
                   'ns3::olsr::DuplicateTuple *', 
                   [param('ns3::Ipv4Address const &', 'address'), param('uint16_t', 'sequenceNumber')])
    ## olsr-state.h: ns3::olsr::IfaceAssocTuple * ns3::OlsrState::FindIfaceAssocTuple(ns3::Ipv4Address const & ifaceAddr) [member function]
    cls.add_method('FindIfaceAssocTuple', 
                   'ns3::olsr::IfaceAssocTuple *', 
                   [param('ns3::Ipv4Address const &', 'ifaceAddr')])
    ## olsr-state.h: ns3::olsr::IfaceAssocTuple const * ns3::OlsrState::FindIfaceAssocTuple(ns3::Ipv4Address const & ifaceAddr) const [member function]
    cls.add_method('FindIfaceAssocTuple', 
                   'ns3::olsr::IfaceAssocTuple const *', 
                   [param('ns3::Ipv4Address const &', 'ifaceAddr')], 
                   is_const=True)
    ## olsr-state.h: ns3::olsr::LinkTuple * ns3::OlsrState::FindLinkTuple(ns3::Ipv4Address const & ifaceAddr) [member function]
    cls.add_method('FindLinkTuple', 
                   'ns3::olsr::LinkTuple *', 
                   [param('ns3::Ipv4Address const &', 'ifaceAddr')])
    ## olsr-state.h: bool ns3::OlsrState::FindMprAddress(ns3::Ipv4Address const & address) [member function]
    cls.add_method('FindMprAddress', 
                   'bool', 
                   [param('ns3::Ipv4Address const &', 'address')])
    ## olsr-state.h: ns3::olsr::MprSelectorTuple * ns3::OlsrState::FindMprSelectorTuple(ns3::Ipv4Address const & mainAddr) [member function]
    cls.add_method('FindMprSelectorTuple', 
                   'ns3::olsr::MprSelectorTuple *', 
                   [param('ns3::Ipv4Address const &', 'mainAddr')])
    ## olsr-state.h: std::vector<ns3::Ipv4Address, std::allocator<ns3::Ipv4Address> > ns3::OlsrState::FindNeighborInterfaces(ns3::Ipv4Address const & neighborMainAddr) const [member function]
    cls.add_method('FindNeighborInterfaces', 
                   'std::vector< ns3::Ipv4Address >', 
                   [param('ns3::Ipv4Address const &', 'neighborMainAddr')], 
                   is_const=True)
    ## olsr-state.h: ns3::olsr::NeighborTuple * ns3::OlsrState::FindNeighborTuple(ns3::Ipv4Address const & mainAddr) [member function]
    cls.add_method('FindNeighborTuple', 
                   'ns3::olsr::NeighborTuple *', 
                   [param('ns3::Ipv4Address const &', 'mainAddr')])
    ## olsr-state.h: ns3::olsr::NeighborTuple * ns3::OlsrState::FindNeighborTuple(ns3::Ipv4Address const & mainAddr, uint8_t willingness) [member function]
    cls.add_method('FindNeighborTuple', 
                   'ns3::olsr::NeighborTuple *', 
                   [param('ns3::Ipv4Address const &', 'mainAddr'), param('uint8_t', 'willingness')])
    ## olsr-state.h: ns3::olsr::TopologyTuple * ns3::OlsrState::FindNewerTopologyTuple(ns3::Ipv4Address const & lastAddr, uint16_t ansn) [member function]
    cls.add_method('FindNewerTopologyTuple', 
                   'ns3::olsr::TopologyTuple *', 
                   [param('ns3::Ipv4Address const &', 'lastAddr'), param('uint16_t', 'ansn')])
    ## olsr-state.h: ns3::olsr::LinkTuple * ns3::OlsrState::FindSymLinkTuple(ns3::Ipv4Address const & ifaceAddr, ns3::Time time) [member function]
    cls.add_method('FindSymLinkTuple', 
                   'ns3::olsr::LinkTuple *', 
                   [param('ns3::Ipv4Address const &', 'ifaceAddr'), param('ns3::Time', 'time')])
    ## olsr-state.h: ns3::olsr::NeighborTuple const * ns3::OlsrState::FindSymNeighborTuple(ns3::Ipv4Address const & mainAddr) const [member function]
    cls.add_method('FindSymNeighborTuple', 
                   'ns3::olsr::NeighborTuple const *', 
                   [param('ns3::Ipv4Address const &', 'mainAddr')], 
                   is_const=True)
    ## olsr-state.h: ns3::olsr::TopologyTuple * ns3::OlsrState::FindTopologyTuple(ns3::Ipv4Address const & destAddr, ns3::Ipv4Address const & lastAddr) [member function]
    cls.add_method('FindTopologyTuple', 
                   'ns3::olsr::TopologyTuple *', 
                   [param('ns3::Ipv4Address const &', 'destAddr'), param('ns3::Ipv4Address const &', 'lastAddr')])
    ## olsr-state.h: ns3::olsr::TwoHopNeighborTuple * ns3::OlsrState::FindTwoHopNeighborTuple(ns3::Ipv4Address const & neighbor, ns3::Ipv4Address const & twoHopNeighbor) [member function]
    cls.add_method('FindTwoHopNeighborTuple', 
                   'ns3::olsr::TwoHopNeighborTuple *', 
                   [param('ns3::Ipv4Address const &', 'neighbor'), param('ns3::Ipv4Address const &', 'twoHopNeighbor')])
    ## olsr-state.h: ns3::olsr::AssociationSet const & ns3::OlsrState::GetAssociationSet() const [member function]
    cls.add_method('GetAssociationSet', 
                   'ns3::olsr::AssociationSet const &', 
                   [], 
                   is_const=True)
    ## olsr-state.h: ns3::olsr::Associations const & ns3::OlsrState::GetAssociations() const [member function]
    cls.add_method('GetAssociations', 
                   'ns3::olsr::Associations const &', 
                   [], 
                   is_const=True)
    ## olsr-state.h: ns3::olsr::IfaceAssocSet const & ns3::OlsrState::GetIfaceAssocSet() const [member function]
    cls.add_method('GetIfaceAssocSet', 
                   'ns3::olsr::IfaceAssocSet const &', 
                   [], 
                   is_const=True)
    ## olsr-state.h: ns3::olsr::IfaceAssocSet & ns3::OlsrState::GetIfaceAssocSetMutable() [member function]
    cls.add_method('GetIfaceAssocSetMutable', 
                   'ns3::olsr::IfaceAssocSet &', 
                   [])
    ## olsr-state.h: ns3::olsr::LinkSet const & ns3::OlsrState::GetLinks() const [member function]
    cls.add_method('GetLinks', 
                   'ns3::olsr::LinkSet const &', 
                   [], 
                   is_const=True)
    ## olsr-state.h: ns3::olsr::MprSelectorSet const & ns3::OlsrState::GetMprSelectors() const [member function]
    cls.add_method('GetMprSelectors', 
                   'ns3::olsr::MprSelectorSet const &', 
                   [], 
                   is_const=True)
    ## olsr-state.h: ns3::olsr::MprSet ns3::OlsrState::GetMprSet() const [member function]
    cls.add_method('GetMprSet', 
                   'ns3::olsr::MprSet', 
                   [], 
                   is_const=True)
    ## olsr-state.h: ns3::olsr::NeighborSet const & ns3::OlsrState::GetNeighbors() const [member function]
    cls.add_method('GetNeighbors', 
                   'ns3::olsr::NeighborSet const &', 
                   [], 
                   is_const=True)
    ## olsr-state.h: ns3::olsr::NeighborSet & ns3::OlsrState::GetNeighbors() [member function]
    cls.add_method('GetNeighbors', 
                   'ns3::olsr::NeighborSet &', 
                   [])
    ## olsr-state.h: ns3::olsr::TopologySet const & ns3::OlsrState::GetTopologySet() const [member function]
    cls.add_method('GetTopologySet', 
                   'ns3::olsr::TopologySet const &', 
                   [], 
                   is_const=True)
    ## olsr-state.h: ns3::olsr::TwoHopNeighborSet const & ns3::OlsrState::GetTwoHopNeighbors() const [member function]
    cls.add_method('GetTwoHopNeighbors', 
                   'ns3::olsr::TwoHopNeighborSet const &', 
                   [], 
                   is_const=True)
    ## olsr-state.h: ns3::olsr::TwoHopNeighborSet & ns3::OlsrState::GetTwoHopNeighbors() [member function]
    cls.add_method('GetTwoHopNeighbors', 
                   'ns3::olsr::TwoHopNeighborSet &', 
                   [])
    ## olsr-state.h: void ns3::OlsrState::InsertAssociation(ns3::olsr::Association const & tuple) [member function]
    cls.add_method('InsertAssociation', 
                   'void', 
                   [param('ns3::olsr::Association const &', 'tuple')])
    ## olsr-state.h: void ns3::OlsrState::InsertAssociationTuple(ns3::olsr::AssociationTuple const & tuple) [member function]
    cls.add_method('InsertAssociationTuple', 
                   'void', 
                   [param('ns3::olsr::AssociationTuple const &', 'tuple')])
    ## olsr-state.h: void ns3::OlsrState::InsertDuplicateTuple(ns3::olsr::DuplicateTuple const & tuple) [member function]
    cls.add_method('InsertDuplicateTuple', 
                   'void', 
                   [param('ns3::olsr::DuplicateTuple const &', 'tuple')])
    ## olsr-state.h: void ns3::OlsrState::InsertIfaceAssocTuple(ns3::olsr::IfaceAssocTuple const & tuple) [member function]
    cls.add_method('InsertIfaceAssocTuple', 
                   'void', 
                   [param('ns3::olsr::IfaceAssocTuple const &', 'tuple')])
    ## olsr-state.h: ns3::olsr::LinkTuple & ns3::OlsrState::InsertLinkTuple(ns3::olsr::LinkTuple const & tuple) [member function]
    cls.add_method('InsertLinkTuple', 
                   'ns3::olsr::LinkTuple &', 
                   [param('ns3::olsr::LinkTuple const &', 'tuple')])
    ## olsr-state.h: void ns3::OlsrState::InsertMprSelectorTuple(ns3::olsr::MprSelectorTuple const & tuple) [member function]
    cls.add_method('InsertMprSelectorTuple', 
                   'void', 
                   [param('ns3::olsr::MprSelectorTuple const &', 'tuple')])
    ## olsr-state.h: void ns3::OlsrState::InsertNeighborTuple(ns3::olsr::NeighborTuple const & tuple) [member function]
    cls.add_method('InsertNeighborTuple', 
                   'void', 
                   [param('ns3::olsr::NeighborTuple const &', 'tuple')])
    ## olsr-state.h: void ns3::OlsrState::InsertTopologyTuple(ns3::olsr::TopologyTuple const & tuple) [member function]
    cls.add_method('InsertTopologyTuple', 
                   'void', 
                   [param('ns3::olsr::TopologyTuple const &', 'tuple')])
    ## olsr-state.h: void ns3::OlsrState::InsertTwoHopNeighborTuple(ns3::olsr::TwoHopNeighborTuple const & tuple) [member function]
    cls.add_method('InsertTwoHopNeighborTuple', 
                   'void', 
                   [param('ns3::olsr::TwoHopNeighborTuple const &', 'tuple')])
    ## olsr-state.h: std::string ns3::OlsrState::PrintMprSelectorSet() const [member function]
    cls.add_method('PrintMprSelectorSet', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## olsr-state.h: void ns3::OlsrState::SetMprSet(ns3::olsr::MprSet mprSet) [member function]
    cls.add_method('SetMprSet', 
                   'void', 
                   [param('ns3::olsr::MprSet', 'mprSet')])
    return

def register_Ns3PyViz_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::PyViz(ns3::PyViz const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::PyViz() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::LastPacketsSample ns3::PyViz::GetLastPackets(uint32_t nodeId) const [member function]
    cls.add_method('GetLastPackets', 
                   'ns3::PyViz::LastPacketsSample', 
                   [param('uint32_t', 'nodeId')], 
                   is_const=True)
    ## pyviz.h: std::vector<ns3::PyViz::NodeStatistics,std::allocator<ns3::PyViz::NodeStatistics> > ns3::PyViz::GetNodesStatistics() const [member function]
    cls.add_method('GetNodesStatistics', 
                   'std::vector< ns3::PyViz::NodeStatistics >', 
                   [], 
                   is_const=True)
    ## pyviz.h: std::vector<ns3::PyViz::PacketDropSample,std::allocator<ns3::PyViz::PacketDropSample> > ns3::PyViz::GetPacketDropSamples() const [member function]
    cls.add_method('GetPacketDropSamples', 
                   'std::vector< ns3::PyViz::PacketDropSample >', 
                   [], 
                   is_const=True)
    ## pyviz.h: std::vector<std::string, std::allocator<std::string> > ns3::PyViz::GetPauseMessages() const [member function]
    cls.add_method('GetPauseMessages', 
                   'std::vector< std::string >', 
                   [], 
                   is_const=True)
    ## pyviz.h: std::vector<ns3::PyViz::TransmissionSample,std::allocator<ns3::PyViz::TransmissionSample> > ns3::PyViz::GetTransmissionSamples() const [member function]
    cls.add_method('GetTransmissionSamples', 
                   'std::vector< ns3::PyViz::TransmissionSample >', 
                   [], 
                   is_const=True)
    ## pyviz.h: static void ns3::PyViz::LineClipping(double boundsX1, double boundsY1, double boundsX2, double boundsY2, double & lineX1, double & lineY1, double & lineX2, double & lineY2) [member function]
    cls.add_method('LineClipping', 
                   'void', 
                   [param('double', 'boundsX1'), param('double', 'boundsY1'), param('double', 'boundsX2'), param('double', 'boundsY2'), param('double &', 'lineX1', direction=3), param('double &', 'lineY1', direction=3), param('double &', 'lineX2', direction=3), param('double &', 'lineY2', direction=3)], 
                   is_static=True)
    ## pyviz.h: static void ns3::PyViz::Pause(std::string const & message) [member function]
    cls.add_method('Pause', 
                   'void', 
                   [param('std::string const &', 'message')], 
                   is_static=True)
    ## pyviz.h: void ns3::PyViz::RegisterCsmaLikeDevice(std::string const & deviceTypeName) [member function]
    cls.add_method('RegisterCsmaLikeDevice', 
                   'void', 
                   [param('std::string const &', 'deviceTypeName')])
    ## pyviz.h: void ns3::PyViz::RegisterDropTracePath(std::string const & tracePath) [member function]
    cls.add_method('RegisterDropTracePath', 
                   'void', 
                   [param('std::string const &', 'tracePath')])
    ## pyviz.h: void ns3::PyViz::RegisterPointToPointLikeDevice(std::string const & deviceTypeName) [member function]
    cls.add_method('RegisterPointToPointLikeDevice', 
                   'void', 
                   [param('std::string const &', 'deviceTypeName')])
    ## pyviz.h: void ns3::PyViz::RegisterWifiLikeDevice(std::string const & deviceTypeName) [member function]
    cls.add_method('RegisterWifiLikeDevice', 
                   'void', 
                   [param('std::string const &', 'deviceTypeName')])
    ## pyviz.h: void ns3::PyViz::SetNodesOfInterest(std::set<unsigned int, std::less<unsigned int>, std::allocator<unsigned int> > nodes) [member function]
    cls.add_method('SetNodesOfInterest', 
                   'void', 
                   [param('std::set< unsigned int >', 'nodes')])
    ## pyviz.h: void ns3::PyViz::SetPacketCaptureOptions(uint32_t nodeId, ns3::PyViz::PacketCaptureOptions options) [member function]
    cls.add_method('SetPacketCaptureOptions', 
                   'void', 
                   [param('uint32_t', 'nodeId'), param('ns3::PyViz::PacketCaptureOptions', 'options')])
    ## pyviz.h: void ns3::PyViz::SimulatorRunUntil(ns3::Time time) [member function]
    cls.add_method('SimulatorRunUntil', 
                   'void', 
                   [param('ns3::Time', 'time')])
    return

def register_Ns3PyVizLastPacketsSample_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::LastPacketsSample::LastPacketsSample() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::LastPacketsSample::LastPacketsSample(ns3::PyViz::LastPacketsSample const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz::LastPacketsSample const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::LastPacketsSample::lastDroppedPackets [variable]
    cls.add_instance_attribute('lastDroppedPackets', 'std::vector< ns3::PyViz::PacketSample >', is_const=False)
    ## pyviz.h: ns3::PyViz::LastPacketsSample::lastReceivedPackets [variable]
    cls.add_instance_attribute('lastReceivedPackets', 'std::vector< ns3::PyViz::RxPacketSample >', is_const=False)
    ## pyviz.h: ns3::PyViz::LastPacketsSample::lastTransmittedPackets [variable]
    cls.add_instance_attribute('lastTransmittedPackets', 'std::vector< ns3::PyViz::TxPacketSample >', is_const=False)
    return

def register_Ns3PyVizNetDeviceStatistics_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::NetDeviceStatistics::NetDeviceStatistics(ns3::PyViz::NetDeviceStatistics const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz::NetDeviceStatistics const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::NetDeviceStatistics::NetDeviceStatistics() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::NetDeviceStatistics::receivedBytes [variable]
    cls.add_instance_attribute('receivedBytes', 'uint64_t', is_const=False)
    ## pyviz.h: ns3::PyViz::NetDeviceStatistics::receivedPackets [variable]
    cls.add_instance_attribute('receivedPackets', 'uint32_t', is_const=False)
    ## pyviz.h: ns3::PyViz::NetDeviceStatistics::transmittedBytes [variable]
    cls.add_instance_attribute('transmittedBytes', 'uint64_t', is_const=False)
    ## pyviz.h: ns3::PyViz::NetDeviceStatistics::transmittedPackets [variable]
    cls.add_instance_attribute('transmittedPackets', 'uint32_t', is_const=False)
    return

def register_Ns3PyVizNodeStatistics_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::NodeStatistics::NodeStatistics() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::NodeStatistics::NodeStatistics(ns3::PyViz::NodeStatistics const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz::NodeStatistics const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::NodeStatistics::nodeId [variable]
    cls.add_instance_attribute('nodeId', 'uint32_t', is_const=False)
    ## pyviz.h: ns3::PyViz::NodeStatistics::statistics [variable]
    cls.add_instance_attribute('statistics', 'std::vector< ns3::PyViz::NetDeviceStatistics >', is_const=False)
    return

def register_Ns3PyVizPacketCaptureOptions_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::PacketCaptureOptions::PacketCaptureOptions() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::PacketCaptureOptions::PacketCaptureOptions(ns3::PyViz::PacketCaptureOptions const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz::PacketCaptureOptions const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::PacketCaptureOptions::headers [variable]
    cls.add_instance_attribute('headers', 'std::set< ns3::TypeId >', is_const=False)
    ## pyviz.h: ns3::PyViz::PacketCaptureOptions::mode [variable]
    cls.add_instance_attribute('mode', 'ns3::PyViz::PacketCaptureMode', is_const=False)
    ## pyviz.h: ns3::PyViz::PacketCaptureOptions::numLastPackets [variable]
    cls.add_instance_attribute('numLastPackets', 'uint32_t', is_const=False)
    return

def register_Ns3PyVizPacketDropSample_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::PacketDropSample::PacketDropSample() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::PacketDropSample::PacketDropSample(ns3::PyViz::PacketDropSample const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz::PacketDropSample const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::PacketDropSample::bytes [variable]
    cls.add_instance_attribute('bytes', 'uint32_t', is_const=False)
    ## pyviz.h: ns3::PyViz::PacketDropSample::transmitter [variable]
    cls.add_instance_attribute('transmitter', 'ns3::Ptr< ns3::Node >', is_const=False)
    return

def register_Ns3PyVizPacketSample_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::PacketSample::PacketSample() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::PacketSample::PacketSample(ns3::PyViz::PacketSample const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz::PacketSample const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::PacketSample::device [variable]
    cls.add_instance_attribute('device', 'ns3::Ptr< ns3::NetDevice >', is_const=False)
    ## pyviz.h: ns3::PyViz::PacketSample::packet [variable]
    cls.add_instance_attribute('packet', 'ns3::Ptr< ns3::Packet >', is_const=False)
    ## pyviz.h: ns3::PyViz::PacketSample::time [variable]
    cls.add_instance_attribute('time', 'ns3::Time', is_const=False)
    return

def register_Ns3PyVizRxPacketSample_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::RxPacketSample::RxPacketSample() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::RxPacketSample::RxPacketSample(ns3::PyViz::RxPacketSample const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz::RxPacketSample const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::RxPacketSample::from [variable]
    cls.add_instance_attribute('from', 'ns3::Mac48Address', is_const=False)
    return

def register_Ns3PyVizTransmissionSample_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::TransmissionSample::TransmissionSample() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::TransmissionSample::TransmissionSample(ns3::PyViz::TransmissionSample const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz::TransmissionSample const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::TransmissionSample::bytes [variable]
    cls.add_instance_attribute('bytes', 'uint32_t', is_const=False)
    ## pyviz.h: ns3::PyViz::TransmissionSample::channel [variable]
    cls.add_instance_attribute('channel', 'ns3::Ptr< ns3::Channel >', is_const=False)
    ## pyviz.h: ns3::PyViz::TransmissionSample::receiver [variable]
    cls.add_instance_attribute('receiver', 'ns3::Ptr< ns3::Node >', is_const=False)
    ## pyviz.h: ns3::PyViz::TransmissionSample::transmitter [variable]
    cls.add_instance_attribute('transmitter', 'ns3::Ptr< ns3::Node >', is_const=False)
    return

def register_Ns3PyVizTxPacketSample_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::TxPacketSample::TxPacketSample() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::TxPacketSample::TxPacketSample(ns3::PyViz::TxPacketSample const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz::TxPacketSample const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::TxPacketSample::to [variable]
    cls.add_instance_attribute('to', 'ns3::Mac48Address', is_const=False)
    return

def register_Ns3Reservation_methods(root_module, cls):
    ## uan-mac-rc.h: ns3::Reservation::Reservation(ns3::Reservation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Reservation const &', 'arg0')])
    ## uan-mac-rc.h: ns3::Reservation::Reservation() [constructor]
    cls.add_constructor([])
    ## uan-mac-rc.h: ns3::Reservation::Reservation(std::list<std::pair<ns3::Ptr<ns3::Packet>, ns3::UanAddress>, std::allocator<std::pair<ns3::Ptr<ns3::Packet>, ns3::UanAddress> > > & list, uint8_t frameNo, uint32_t maxPkts=0) [constructor]
    cls.add_constructor([param('std::list< std::pair< ns3::Ptr< ns3::Packet >, ns3::UanAddress > > &', 'list'), param('uint8_t', 'frameNo'), param('uint32_t', 'maxPkts', default_value='0')])
    ## uan-mac-rc.h: void ns3::Reservation::AddTimestamp(ns3::Time t) [member function]
    cls.add_method('AddTimestamp', 
                   'void', 
                   [param('ns3::Time', 't')])
    ## uan-mac-rc.h: uint8_t ns3::Reservation::GetFrameNo() const [member function]
    cls.add_method('GetFrameNo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-mac-rc.h: uint32_t ns3::Reservation::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## uan-mac-rc.h: uint32_t ns3::Reservation::GetNoFrames() const [member function]
    cls.add_method('GetNoFrames', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## uan-mac-rc.h: std::list<std::pair<ns3::Ptr<ns3::Packet>, ns3::UanAddress>, std::allocator<std::pair<ns3::Ptr<ns3::Packet>, ns3::UanAddress> > > const & ns3::Reservation::GetPktList() const [member function]
    cls.add_method('GetPktList', 
                   'std::list< std::pair< ns3::Ptr< ns3::Packet >, ns3::UanAddress > > const &', 
                   [], 
                   is_const=True)
    ## uan-mac-rc.h: uint8_t ns3::Reservation::GetRetryNo() const [member function]
    cls.add_method('GetRetryNo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-mac-rc.h: ns3::Time ns3::Reservation::GetTimestamp(uint8_t n) const [member function]
    cls.add_method('GetTimestamp', 
                   'ns3::Time', 
                   [param('uint8_t', 'n')], 
                   is_const=True)
    ## uan-mac-rc.h: void ns3::Reservation::IncrementRetry() [member function]
    cls.add_method('IncrementRetry', 
                   'void', 
                   [])
    ## uan-mac-rc.h: bool ns3::Reservation::IsTransmitted() const [member function]
    cls.add_method('IsTransmitted', 
                   'bool', 
                   [], 
                   is_const=True)
    ## uan-mac-rc.h: void ns3::Reservation::SetFrameNo(uint8_t fn) [member function]
    cls.add_method('SetFrameNo', 
                   'void', 
                   [param('uint8_t', 'fn')])
    ## uan-mac-rc.h: void ns3::Reservation::SetTransmitted(bool t=true) [member function]
    cls.add_method('SetTransmitted', 
                   'void', 
                   [param('bool', 't', default_value='true')])
    return

def register_Ns3RvBatteryModelHelper_methods(root_module, cls):
    ## rv-battery-model-helper.h: ns3::RvBatteryModelHelper::RvBatteryModelHelper(ns3::RvBatteryModelHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RvBatteryModelHelper const &', 'arg0')])
    ## rv-battery-model-helper.h: ns3::RvBatteryModelHelper::RvBatteryModelHelper() [constructor]
    cls.add_constructor([])
    ## rv-battery-model-helper.h: void ns3::RvBatteryModelHelper::Set(std::string name, ns3::AttributeValue const & v) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'v')], 
                   is_virtual=True)
    ## rv-battery-model-helper.h: ns3::Ptr<ns3::EnergySource> ns3::RvBatteryModelHelper::DoInstall(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('DoInstall', 
                   'ns3::Ptr< ns3::EnergySource >', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3Tap_methods(root_module, cls):
    ## uan-prop-model.h: ns3::Tap::Tap(ns3::Tap const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Tap const &', 'arg0')])
    ## uan-prop-model.h: ns3::Tap::Tap() [constructor]
    cls.add_constructor([])
    ## uan-prop-model.h: ns3::Tap::Tap(ns3::Time delay, std::complex<double> amp) [constructor]
    cls.add_constructor([param('ns3::Time', 'delay'), param('std::complex< double >', 'amp')])
    ## uan-prop-model.h: std::complex<double> ns3::Tap::GetAmp() const [member function]
    cls.add_method('GetAmp', 
                   'std::complex< double >', 
                   [], 
                   is_const=True)
    ## uan-prop-model.h: ns3::Time ns3::Tap::GetDelay() const [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    return

def register_Ns3UanAddress_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## uan-address.h: ns3::UanAddress::UanAddress(ns3::UanAddress const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanAddress const &', 'arg0')])
    ## uan-address.h: ns3::UanAddress::UanAddress() [constructor]
    cls.add_constructor([])
    ## uan-address.h: ns3::UanAddress::UanAddress(uint8_t addr) [constructor]
    cls.add_constructor([param('uint8_t', 'addr')])
    ## uan-address.h: static ns3::UanAddress ns3::UanAddress::Allocate() [member function]
    cls.add_method('Allocate', 
                   'ns3::UanAddress', 
                   [], 
                   is_static=True)
    ## uan-address.h: static ns3::UanAddress ns3::UanAddress::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::UanAddress', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## uan-address.h: void ns3::UanAddress::CopyFrom(uint8_t const * pBuffer) [member function]
    cls.add_method('CopyFrom', 
                   'void', 
                   [param('uint8_t const *', 'pBuffer')])
    ## uan-address.h: void ns3::UanAddress::CopyTo(uint8_t * pBuffer) [member function]
    cls.add_method('CopyTo', 
                   'void', 
                   [param('uint8_t *', 'pBuffer')])
    ## uan-address.h: uint8_t ns3::UanAddress::GetAsInt() const [member function]
    cls.add_method('GetAsInt', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-address.h: static ns3::UanAddress ns3::UanAddress::GetBroadcast() [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::UanAddress', 
                   [], 
                   is_static=True)
    ## uan-address.h: static bool ns3::UanAddress::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    return

def register_Ns3UanHelper_methods(root_module, cls):
    ## uan-helper.h: ns3::UanHelper::UanHelper(ns3::UanHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanHelper const &', 'arg0')])
    ## uan-helper.h: ns3::UanHelper::UanHelper() [constructor]
    cls.add_constructor([])
    ## uan-helper.h: static void ns3::UanHelper::EnableAscii(std::ostream & os, uint32_t nodeid, uint32_t deviceid) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid')], 
                   is_static=True)
    ## uan-helper.h: static void ns3::UanHelper::EnableAscii(std::ostream & os, ns3::NetDeviceContainer d) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('ns3::NetDeviceContainer', 'd')], 
                   is_static=True)
    ## uan-helper.h: static void ns3::UanHelper::EnableAscii(std::ostream & os, ns3::NodeContainer n) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('ns3::NodeContainer', 'n')], 
                   is_static=True)
    ## uan-helper.h: static void ns3::UanHelper::EnableAsciiAll(std::ostream & os) [member function]
    cls.add_method('EnableAsciiAll', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_static=True)
    ## uan-helper.h: ns3::NetDeviceContainer ns3::UanHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## uan-helper.h: ns3::NetDeviceContainer ns3::UanHelper::Install(ns3::NodeContainer c, ns3::Ptr<ns3::UanChannel> channel) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c'), param('ns3::Ptr< ns3::UanChannel >', 'channel')], 
                   is_const=True)
    ## uan-helper.h: ns3::Ptr<ns3::UanNetDevice> ns3::UanHelper::Install(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::UanChannel> channel) const [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::UanNetDevice >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::UanChannel >', 'channel')], 
                   is_const=True)
    ## uan-helper.h: void ns3::UanHelper::SetMac(std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetMac', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## uan-helper.h: void ns3::UanHelper::SetPhy(std::string phyType, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetPhy', 
                   'void', 
                   [param('std::string', 'phyType'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## uan-helper.h: void ns3::UanHelper::SetTransducer(std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetTransducer', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    return

def register_Ns3UanModesList_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## uan-tx-mode.h: ns3::UanModesList::UanModesList(ns3::UanModesList const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanModesList const &', 'arg0')])
    ## uan-tx-mode.h: ns3::UanModesList::UanModesList() [constructor]
    cls.add_constructor([])
    ## uan-tx-mode.h: void ns3::UanModesList::AppendMode(ns3::UanTxMode mode) [member function]
    cls.add_method('AppendMode', 
                   'void', 
                   [param('ns3::UanTxMode', 'mode')])
    ## uan-tx-mode.h: void ns3::UanModesList::DeleteMode(uint32_t num) [member function]
    cls.add_method('DeleteMode', 
                   'void', 
                   [param('uint32_t', 'num')])
    ## uan-tx-mode.h: uint32_t ns3::UanModesList::GetNModes() const [member function]
    cls.add_method('GetNModes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3UanPacketArrival_methods(root_module, cls):
    ## uan-transducer.h: ns3::UanPacketArrival::UanPacketArrival(ns3::UanPacketArrival const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPacketArrival const &', 'arg0')])
    ## uan-transducer.h: ns3::UanPacketArrival::UanPacketArrival() [constructor]
    cls.add_constructor([])
    ## uan-transducer.h: ns3::UanPacketArrival::UanPacketArrival(ns3::Ptr<ns3::Packet> packet, double rxPowerDb, ns3::UanTxMode txMode, ns3::UanPdp pdp, ns3::Time arrTime) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'rxPowerDb'), param('ns3::UanTxMode', 'txMode'), param('ns3::UanPdp', 'pdp'), param('ns3::Time', 'arrTime')])
    ## uan-transducer.h: ns3::Time ns3::UanPacketArrival::GetArrivalTime() const [member function]
    cls.add_method('GetArrivalTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## uan-transducer.h: ns3::Ptr<ns3::Packet> ns3::UanPacketArrival::GetPacket() const [member function]
    cls.add_method('GetPacket', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True)
    ## uan-transducer.h: ns3::UanPdp ns3::UanPacketArrival::GetPdp() const [member function]
    cls.add_method('GetPdp', 
                   'ns3::UanPdp', 
                   [], 
                   is_const=True)
    ## uan-transducer.h: double ns3::UanPacketArrival::GetRxPowerDb() const [member function]
    cls.add_method('GetRxPowerDb', 
                   'double', 
                   [], 
                   is_const=True)
    ## uan-transducer.h: ns3::UanTxMode const & ns3::UanPacketArrival::GetTxMode() const [member function]
    cls.add_method('GetTxMode', 
                   'ns3::UanTxMode const &', 
                   [], 
                   is_const=True)
    return

def register_Ns3UanPdp_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## uan-prop-model.h: ns3::UanPdp::UanPdp(ns3::UanPdp const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPdp const &', 'arg0')])
    ## uan-prop-model.h: ns3::UanPdp::UanPdp() [constructor]
    cls.add_constructor([])
    ## uan-prop-model.h: ns3::UanPdp::UanPdp(std::vector<ns3::Tap, std::allocator<ns3::Tap> > taps, ns3::Time resolution) [constructor]
    cls.add_constructor([param('std::vector< ns3::Tap >', 'taps'), param('ns3::Time', 'resolution')])
    ## uan-prop-model.h: ns3::UanPdp::UanPdp(std::vector<std::complex<double>,std::allocator<std::complex<double> > > arrivals, ns3::Time resolution) [constructor]
    cls.add_constructor([param('std::vector< std::complex< double > >', 'arrivals'), param('ns3::Time', 'resolution')])
    ## uan-prop-model.h: ns3::UanPdp::UanPdp(std::vector<double, std::allocator<double> > arrivals, ns3::Time resolution) [constructor]
    cls.add_constructor([param('std::vector< double >', 'arrivals'), param('ns3::Time', 'resolution')])
    ## uan-prop-model.h: static ns3::UanPdp ns3::UanPdp::CreateImpulsePdp() [member function]
    cls.add_method('CreateImpulsePdp', 
                   'ns3::UanPdp', 
                   [], 
                   is_static=True)
    ## uan-prop-model.h: __gnu_cxx::__normal_iterator<const ns3::Tap*,std::vector<ns3::Tap, std::allocator<ns3::Tap> > > ns3::UanPdp::GetBegin() const [member function]
    cls.add_method('GetBegin', 
                   '__gnu_cxx::__normal_iterator< ns3::Tap const *, std::vector< ns3::Tap > >', 
                   [], 
                   is_const=True)
    ## uan-prop-model.h: __gnu_cxx::__normal_iterator<const ns3::Tap*,std::vector<ns3::Tap, std::allocator<ns3::Tap> > > ns3::UanPdp::GetEnd() const [member function]
    cls.add_method('GetEnd', 
                   '__gnu_cxx::__normal_iterator< ns3::Tap const *, std::vector< ns3::Tap > >', 
                   [], 
                   is_const=True)
    ## uan-prop-model.h: uint32_t ns3::UanPdp::GetNTaps() const [member function]
    cls.add_method('GetNTaps', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## uan-prop-model.h: ns3::Time ns3::UanPdp::GetResolution() const [member function]
    cls.add_method('GetResolution', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## uan-prop-model.h: ns3::Tap const & ns3::UanPdp::GetTap(uint32_t i) const [member function]
    cls.add_method('GetTap', 
                   'ns3::Tap const &', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## uan-prop-model.h: void ns3::UanPdp::SetNTaps(uint32_t nTaps) [member function]
    cls.add_method('SetNTaps', 
                   'void', 
                   [param('uint32_t', 'nTaps')])
    ## uan-prop-model.h: void ns3::UanPdp::SetResolution(ns3::Time resolution) [member function]
    cls.add_method('SetResolution', 
                   'void', 
                   [param('ns3::Time', 'resolution')])
    ## uan-prop-model.h: void ns3::UanPdp::SetTap(std::complex<double> arrival, uint32_t index) [member function]
    cls.add_method('SetTap', 
                   'void', 
                   [param('std::complex< double >', 'arrival'), param('uint32_t', 'index')])
    ## uan-prop-model.h: std::complex<double> ns3::UanPdp::SumTapsC(ns3::Time begin, ns3::Time end) const [member function]
    cls.add_method('SumTapsC', 
                   'std::complex< double >', 
                   [param('ns3::Time', 'begin'), param('ns3::Time', 'end')], 
                   is_const=True)
    ## uan-prop-model.h: std::complex<double> ns3::UanPdp::SumTapsFromMaxC(ns3::Time delay, ns3::Time duration) const [member function]
    cls.add_method('SumTapsFromMaxC', 
                   'std::complex< double >', 
                   [param('ns3::Time', 'delay'), param('ns3::Time', 'duration')], 
                   is_const=True)
    ## uan-prop-model.h: double ns3::UanPdp::SumTapsFromMaxNc(ns3::Time delay, ns3::Time duration) const [member function]
    cls.add_method('SumTapsFromMaxNc', 
                   'double', 
                   [param('ns3::Time', 'delay'), param('ns3::Time', 'duration')], 
                   is_const=True)
    ## uan-prop-model.h: double ns3::UanPdp::SumTapsNc(ns3::Time begin, ns3::Time end) const [member function]
    cls.add_method('SumTapsNc', 
                   'double', 
                   [param('ns3::Time', 'begin'), param('ns3::Time', 'end')], 
                   is_const=True)
    return

def register_Ns3UanPhyListener_methods(root_module, cls):
    ## uan-phy.h: ns3::UanPhyListener::UanPhyListener() [constructor]
    cls.add_constructor([])
    ## uan-phy.h: ns3::UanPhyListener::UanPhyListener(ns3::UanPhyListener const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyListener const &', 'arg0')])
    ## uan-phy.h: void ns3::UanPhyListener::NotifyCcaEnd() [member function]
    cls.add_method('NotifyCcaEnd', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhyListener::NotifyCcaStart() [member function]
    cls.add_method('NotifyCcaStart', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhyListener::NotifyRxEndError() [member function]
    cls.add_method('NotifyRxEndError', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhyListener::NotifyRxEndOk() [member function]
    cls.add_method('NotifyRxEndOk', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhyListener::NotifyRxStart() [member function]
    cls.add_method('NotifyRxStart', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhyListener::NotifyTxStart(ns3::Time duration) [member function]
    cls.add_method('NotifyTxStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3UanTxMode_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## uan-tx-mode.h: ns3::UanTxMode::UanTxMode(ns3::UanTxMode const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanTxMode const &', 'arg0')])
    ## uan-tx-mode.h: ns3::UanTxMode::UanTxMode() [constructor]
    cls.add_constructor([])
    ## uan-tx-mode.h: uint32_t ns3::UanTxMode::GetBandwidthHz() const [member function]
    cls.add_method('GetBandwidthHz', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## uan-tx-mode.h: uint32_t ns3::UanTxMode::GetCenterFreqHz() const [member function]
    cls.add_method('GetCenterFreqHz', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## uan-tx-mode.h: uint32_t ns3::UanTxMode::GetConstellationSize() const [member function]
    cls.add_method('GetConstellationSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## uan-tx-mode.h: uint32_t ns3::UanTxMode::GetDataRateBps() const [member function]
    cls.add_method('GetDataRateBps', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## uan-tx-mode.h: ns3::UanTxMode::ModulationType ns3::UanTxMode::GetModType() const [member function]
    cls.add_method('GetModType', 
                   'ns3::UanTxMode::ModulationType', 
                   [], 
                   is_const=True)
    ## uan-tx-mode.h: std::string ns3::UanTxMode::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## uan-tx-mode.h: uint32_t ns3::UanTxMode::GetPhyRateSps() const [member function]
    cls.add_method('GetPhyRateSps', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## uan-tx-mode.h: uint32_t ns3::UanTxMode::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3UanTxModeFactory_methods(root_module, cls):
    ## uan-tx-mode.h: ns3::UanTxModeFactory::UanTxModeFactory(ns3::UanTxModeFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanTxModeFactory const &', 'arg0')])
    ## uan-tx-mode.h: ns3::UanTxModeFactory::UanTxModeFactory() [constructor]
    cls.add_constructor([])
    ## uan-tx-mode.h: static ns3::UanTxMode ns3::UanTxModeFactory::CreateMode(ns3::UanTxMode::ModulationType type, uint32_t dataRateBps, uint32_t phyRateSps, uint32_t cfHz, uint32_t bwHz, uint32_t constSize, std::string name) [member function]
    cls.add_method('CreateMode', 
                   'ns3::UanTxMode', 
                   [param('ns3::UanTxMode::ModulationType', 'type'), param('uint32_t', 'dataRateBps'), param('uint32_t', 'phyRateSps'), param('uint32_t', 'cfHz'), param('uint32_t', 'bwHz'), param('uint32_t', 'constSize'), param('std::string', 'name')], 
                   is_static=True)
    ## uan-tx-mode.h: static ns3::UanTxMode ns3::UanTxModeFactory::GetMode(std::string name) [member function]
    cls.add_method('GetMode', 
                   'ns3::UanTxMode', 
                   [param('std::string', 'name')], 
                   is_static=True)
    ## uan-tx-mode.h: static ns3::UanTxMode ns3::UanTxModeFactory::GetMode(uint32_t uid) [member function]
    cls.add_method('GetMode', 
                   'ns3::UanTxMode', 
                   [param('uint32_t', 'uid')], 
                   is_static=True)
    return

def register_Ns3WifiRadioEnergyModelHelper_methods(root_module, cls):
    ## wifi-radio-energy-model-helper.h: ns3::WifiRadioEnergyModelHelper::WifiRadioEnergyModelHelper(ns3::WifiRadioEnergyModelHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiRadioEnergyModelHelper const &', 'arg0')])
    ## wifi-radio-energy-model-helper.h: ns3::WifiRadioEnergyModelHelper::WifiRadioEnergyModelHelper() [constructor]
    cls.add_constructor([])
    ## wifi-radio-energy-model-helper.h: void ns3::WifiRadioEnergyModelHelper::Set(std::string name, ns3::AttributeValue const & v) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'v')], 
                   is_virtual=True)
    ## wifi-radio-energy-model-helper.h: void ns3::WifiRadioEnergyModelHelper::SetDepletionCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetDepletionCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## wifi-radio-energy-model-helper.h: ns3::Ptr<ns3::DeviceEnergyModel> ns3::WifiRadioEnergyModelHelper::DoInstall(ns3::Ptr<ns3::NetDevice> device, ns3::Ptr<ns3::EnergySource> source) const [member function]
    cls.add_method('DoInstall', 
                   'ns3::Ptr< ns3::DeviceEnergyModel >', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device'), param('ns3::Ptr< ns3::EnergySource >', 'source')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3WifiRadioEnergyModelPhyListener_methods(root_module, cls):
    ## wifi-radio-energy-model.h: ns3::WifiRadioEnergyModelPhyListener::WifiRadioEnergyModelPhyListener(ns3::WifiRadioEnergyModelPhyListener const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiRadioEnergyModelPhyListener const &', 'arg0')])
    ## wifi-radio-energy-model.h: ns3::WifiRadioEnergyModelPhyListener::WifiRadioEnergyModelPhyListener() [constructor]
    cls.add_constructor([])
    ## wifi-radio-energy-model.h: void ns3::WifiRadioEnergyModelPhyListener::NotifyMaybeCcaBusyStart(ns3::Time duration) [member function]
    cls.add_method('NotifyMaybeCcaBusyStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_virtual=True)
    ## wifi-radio-energy-model.h: void ns3::WifiRadioEnergyModelPhyListener::NotifyRxEndError() [member function]
    cls.add_method('NotifyRxEndError', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## wifi-radio-energy-model.h: void ns3::WifiRadioEnergyModelPhyListener::NotifyRxEndOk() [member function]
    cls.add_method('NotifyRxEndOk', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## wifi-radio-energy-model.h: void ns3::WifiRadioEnergyModelPhyListener::NotifyRxStart(ns3::Time duration) [member function]
    cls.add_method('NotifyRxStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_virtual=True)
    ## wifi-radio-energy-model.h: void ns3::WifiRadioEnergyModelPhyListener::NotifySwitchingStart(ns3::Time duration) [member function]
    cls.add_method('NotifySwitchingStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_virtual=True)
    ## wifi-radio-energy-model.h: void ns3::WifiRadioEnergyModelPhyListener::NotifyTxStart(ns3::Time duration) [member function]
    cls.add_method('NotifyTxStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_virtual=True)
    ## wifi-radio-energy-model.h: void ns3::WifiRadioEnergyModelPhyListener::SetChangeStateCallback(ns3::Callback<void, int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetChangeStateCallback', 
                   'void', 
                   [param('ns3::Callback< void, int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    return

def register_Ns3AcousticModemEnergyModelHelper_methods(root_module, cls):
    ## acoustic-modem-energy-model-helper.h: ns3::AcousticModemEnergyModelHelper::AcousticModemEnergyModelHelper(ns3::AcousticModemEnergyModelHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AcousticModemEnergyModelHelper const &', 'arg0')])
    ## acoustic-modem-energy-model-helper.h: ns3::AcousticModemEnergyModelHelper::AcousticModemEnergyModelHelper() [constructor]
    cls.add_constructor([])
    ## acoustic-modem-energy-model-helper.h: void ns3::AcousticModemEnergyModelHelper::Set(std::string name, ns3::AttributeValue const & v) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'v')], 
                   is_virtual=True)
    ## acoustic-modem-energy-model-helper.h: void ns3::AcousticModemEnergyModelHelper::SetDepletionCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetDepletionCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## acoustic-modem-energy-model-helper.h: ns3::Ptr<ns3::DeviceEnergyModel> ns3::AcousticModemEnergyModelHelper::DoInstall(ns3::Ptr<ns3::NetDevice> device, ns3::Ptr<ns3::EnergySource> source) const [member function]
    cls.add_method('DoInstall', 
                   'ns3::Ptr< ns3::DeviceEnergyModel >', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device'), param('ns3::Ptr< ns3::EnergySource >', 'source')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3BasicEnergySourceHelper_methods(root_module, cls):
    ## basic-energy-source-helper.h: ns3::BasicEnergySourceHelper::BasicEnergySourceHelper(ns3::BasicEnergySourceHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BasicEnergySourceHelper const &', 'arg0')])
    ## basic-energy-source-helper.h: ns3::BasicEnergySourceHelper::BasicEnergySourceHelper() [constructor]
    cls.add_constructor([])
    ## basic-energy-source-helper.h: void ns3::BasicEnergySourceHelper::Set(std::string name, ns3::AttributeValue const & v) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'v')], 
                   is_virtual=True)
    ## basic-energy-source-helper.h: ns3::Ptr<ns3::EnergySource> ns3::BasicEnergySourceHelper::DoInstall(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('DoInstall', 
                   'ns3::Ptr< ns3::EnergySource >', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3DsdvHelper_methods(root_module, cls):
    ## dsdv-helper.h: ns3::DsdvHelper::DsdvHelper(ns3::DsdvHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DsdvHelper const &', 'arg0')])
    ## dsdv-helper.h: ns3::DsdvHelper::DsdvHelper() [constructor]
    cls.add_constructor([])
    ## dsdv-helper.h: ns3::DsdvHelper * ns3::DsdvHelper::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::DsdvHelper *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dsdv-helper.h: ns3::Ptr<ns3::Ipv4RoutingProtocol> ns3::DsdvHelper::Create(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::Ipv4RoutingProtocol >', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True, is_virtual=True)
    ## dsdv-helper.h: void ns3::DsdvHelper::Set(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3Ipv4GlobalRoutingHelper_methods(root_module, cls):
    ## ipv4-global-routing-helper.h: ns3::Ipv4GlobalRoutingHelper::Ipv4GlobalRoutingHelper() [constructor]
    cls.add_constructor([])
    ## ipv4-global-routing-helper.h: ns3::Ipv4GlobalRoutingHelper::Ipv4GlobalRoutingHelper(ns3::Ipv4GlobalRoutingHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4GlobalRoutingHelper const &', 'arg0')])
    ## ipv4-global-routing-helper.h: ns3::Ipv4GlobalRoutingHelper * ns3::Ipv4GlobalRoutingHelper::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ipv4GlobalRoutingHelper *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-global-routing-helper.h: ns3::Ptr<ns3::Ipv4RoutingProtocol> ns3::Ipv4GlobalRoutingHelper::Create(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::Ipv4RoutingProtocol >', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True, is_virtual=True)
    ## ipv4-global-routing-helper.h: static void ns3::Ipv4GlobalRoutingHelper::PopulateRoutingTables() [member function]
    cls.add_method('PopulateRoutingTables', 
                   'void', 
                   [], 
                   is_static=True)
    ## ipv4-global-routing-helper.h: static void ns3::Ipv4GlobalRoutingHelper::RecomputeRoutingTables() [member function]
    cls.add_method('RecomputeRoutingTables', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3Ipv4ListRoutingHelper_methods(root_module, cls):
    ## ipv4-list-routing-helper.h: ns3::Ipv4ListRoutingHelper::Ipv4ListRoutingHelper() [constructor]
    cls.add_constructor([])
    ## ipv4-list-routing-helper.h: ns3::Ipv4ListRoutingHelper::Ipv4ListRoutingHelper(ns3::Ipv4ListRoutingHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4ListRoutingHelper const &', 'arg0')])
    ## ipv4-list-routing-helper.h: ns3::Ipv4ListRoutingHelper * ns3::Ipv4ListRoutingHelper::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ipv4ListRoutingHelper *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-list-routing-helper.h: void ns3::Ipv4ListRoutingHelper::Add(ns3::Ipv4RoutingHelper const & routing, int16_t priority) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ipv4RoutingHelper const &', 'routing'), param('int16_t', 'priority')])
    ## ipv4-list-routing-helper.h: ns3::Ptr<ns3::Ipv4RoutingProtocol> ns3::Ipv4ListRoutingHelper::Create(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::Ipv4RoutingProtocol >', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Ipv6ListRoutingHelper_methods(root_module, cls):
    ## ipv6-list-routing-helper.h: ns3::Ipv6ListRoutingHelper::Ipv6ListRoutingHelper() [constructor]
    cls.add_constructor([])
    ## ipv6-list-routing-helper.h: ns3::Ipv6ListRoutingHelper::Ipv6ListRoutingHelper(ns3::Ipv6ListRoutingHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6ListRoutingHelper const &', 'arg0')])
    ## ipv6-list-routing-helper.h: ns3::Ipv6ListRoutingHelper * ns3::Ipv6ListRoutingHelper::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ipv6ListRoutingHelper *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-list-routing-helper.h: void ns3::Ipv6ListRoutingHelper::Add(ns3::Ipv6RoutingHelper const & routing, int16_t priority) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ipv6RoutingHelper const &', 'routing'), param('int16_t', 'priority')])
    ## ipv6-list-routing-helper.h: ns3::Ptr<ns3::Ipv6RoutingProtocol> ns3::Ipv6ListRoutingHelper::Create(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::Ipv6RoutingProtocol >', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3LteMacHeader_methods(root_module, cls):
    ## lte-mac-header.h: ns3::LteMacHeader::LteMacHeader() [constructor]
    cls.add_constructor([])
    ## lte-mac-header.h: ns3::LteMacHeader::LteMacHeader(ns3::LteMacHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteMacHeader const &', 'arg0')])
    ## lte-mac-header.h: uint32_t ns3::LteMacHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## lte-mac-header.h: ns3::Mac48Address ns3::LteMacHeader::GetDestination() const [member function]
    cls.add_method('GetDestination', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## lte-mac-header.h: ns3::TypeId ns3::LteMacHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-mac-header.h: uint32_t ns3::LteMacHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-mac-header.h: ns3::Mac48Address ns3::LteMacHeader::GetSource() const [member function]
    cls.add_method('GetSource', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## lte-mac-header.h: static ns3::TypeId ns3::LteMacHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-mac-header.h: void ns3::LteMacHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## lte-mac-header.h: void ns3::LteMacHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## lte-mac-header.h: void ns3::LteMacHeader::SetDestination(ns3::Mac48Address destination) [member function]
    cls.add_method('SetDestination', 
                   'void', 
                   [param('ns3::Mac48Address', 'destination')])
    ## lte-mac-header.h: void ns3::LteMacHeader::SetSource(ns3::Mac48Address source) [member function]
    cls.add_method('SetSource', 
                   'void', 
                   [param('ns3::Mac48Address', 'source')])
    return

def register_Ns3PacketScheduler_methods(root_module, cls):
    ## packet-scheduler.h: ns3::PacketScheduler::PacketScheduler(ns3::PacketScheduler const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketScheduler const &', 'arg0')])
    ## packet-scheduler.h: ns3::PacketScheduler::PacketScheduler() [constructor]
    cls.add_constructor([])
    ## packet-scheduler.h: ns3::PacketScheduler::PacketScheduler(ns3::Ptr<ns3::EnbNetDevice> enb) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::EnbNetDevice >', 'enb')])
    ## packet-scheduler.h: void ns3::PacketScheduler::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## packet-scheduler.h: void ns3::PacketScheduler::DoRunPacketScheduler() [member function]
    cls.add_method('DoRunPacketScheduler', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## packet-scheduler.h: ns3::Ptr<ns3::EnbNetDevice> ns3::PacketScheduler::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::EnbNetDevice >', 
                   [])
    ## packet-scheduler.h: ns3::Ptr<ns3::MacEntity> ns3::PacketScheduler::GetMacEntity() [member function]
    cls.add_method('GetMacEntity', 
                   'ns3::Ptr< ns3::MacEntity >', 
                   [])
    ## packet-scheduler.h: static ns3::TypeId ns3::PacketScheduler::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## packet-scheduler.h: void ns3::PacketScheduler::RunPacketScheduler() [member function]
    cls.add_method('RunPacketScheduler', 
                   'void', 
                   [])
    ## packet-scheduler.h: void ns3::PacketScheduler::SetDevice(ns3::Ptr<ns3::EnbNetDevice> enb) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::EnbNetDevice >', 'enb')])
    ## packet-scheduler.h: void ns3::PacketScheduler::SetMacEntity(ns3::Ptr<ns3::MacEntity> mac) [member function]
    cls.add_method('SetMacEntity', 
                   'void', 
                   [param('ns3::Ptr< ns3::MacEntity >', 'mac')])
    return

def register_Ns3RadioBearerInstance_methods(root_module, cls):
    ## radio-bearer-instance.h: ns3::RadioBearerInstance::RadioBearerInstance(ns3::RadioBearerInstance const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RadioBearerInstance const &', 'arg0')])
    ## radio-bearer-instance.h: ns3::RadioBearerInstance::RadioBearerInstance() [constructor]
    cls.add_constructor([])
    ## radio-bearer-instance.h: ns3::Ptr<ns3::Packet> ns3::RadioBearerInstance::Dequeue() [member function]
    cls.add_method('Dequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [])
    ## radio-bearer-instance.h: ns3::Ptr<ns3::Packet> ns3::RadioBearerInstance::Dequeue(uint32_t availableByte) [member function]
    cls.add_method('Dequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'availableByte')])
    ## radio-bearer-instance.h: bool ns3::RadioBearerInstance::Enqueue(ns3::Ptr<ns3::Packet> packet) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet')])
    ## radio-bearer-instance.h: ns3::RadioBearerInstance::BearerDirection ns3::RadioBearerInstance::GetBearerDirection() const [member function]
    cls.add_method('GetBearerDirection', 
                   'ns3::RadioBearerInstance::BearerDirection', 
                   [], 
                   is_const=True)
    ## radio-bearer-instance.h: ns3::RadioBearerInstance::BearerType ns3::RadioBearerInstance::GetBearerType() const [member function]
    cls.add_method('GetBearerType', 
                   'ns3::RadioBearerInstance::BearerType', 
                   [], 
                   is_const=True)
    ## radio-bearer-instance.h: ns3::IpcsClassifierRecord * ns3::RadioBearerInstance::GetIpcsClassifierRecord() [member function]
    cls.add_method('GetIpcsClassifierRecord', 
                   'ns3::IpcsClassifierRecord *', 
                   [])
    ## radio-bearer-instance.h: ns3::Ptr<ns3::BearerQosParameters> ns3::RadioBearerInstance::GetQosParameters() [member function]
    cls.add_method('GetQosParameters', 
                   'ns3::Ptr< ns3::BearerQosParameters >', 
                   [])
    ## radio-bearer-instance.h: ns3::Ptr<ns3::LteMacQueue> ns3::RadioBearerInstance::GetQueue() const [member function]
    cls.add_method('GetQueue', 
                   'ns3::Ptr< ns3::LteMacQueue >', 
                   [], 
                   is_const=True)
    ## radio-bearer-instance.h: ns3::Ptr<ns3::RlcEntity> ns3::RadioBearerInstance::GetRlcEntity() [member function]
    cls.add_method('GetRlcEntity', 
                   'ns3::Ptr< ns3::RlcEntity >', 
                   [])
    ## radio-bearer-instance.h: static ns3::TypeId ns3::RadioBearerInstance::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## radio-bearer-instance.h: bool ns3::RadioBearerInstance::HasPackets() const [member function]
    cls.add_method('HasPackets', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radio-bearer-instance.h: void ns3::RadioBearerInstance::SetBearerDirection(ns3::RadioBearerInstance::BearerDirection direction) [member function]
    cls.add_method('SetBearerDirection', 
                   'void', 
                   [param('ns3::RadioBearerInstance::BearerDirection', 'direction')])
    ## radio-bearer-instance.h: void ns3::RadioBearerInstance::SetBearerType(ns3::RadioBearerInstance::BearerType type) [member function]
    cls.add_method('SetBearerType', 
                   'void', 
                   [param('ns3::RadioBearerInstance::BearerType', 'type')])
    ## radio-bearer-instance.h: void ns3::RadioBearerInstance::SetIpcsClassifierRecord(ns3::IpcsClassifierRecord * c) [member function]
    cls.add_method('SetIpcsClassifierRecord', 
                   'void', 
                   [param('ns3::IpcsClassifierRecord *', 'c')])
    ## radio-bearer-instance.h: void ns3::RadioBearerInstance::SetQosParameters(ns3::Ptr<ns3::BearerQosParameters> qosParameters) [member function]
    cls.add_method('SetQosParameters', 
                   'void', 
                   [param('ns3::Ptr< ns3::BearerQosParameters >', 'qosParameters')])
    ## radio-bearer-instance.h: void ns3::RadioBearerInstance::SetRlcEntity(ns3::Ptr<ns3::RlcEntity> rlc) [member function]
    cls.add_method('SetRlcEntity', 
                   'void', 
                   [param('ns3::Ptr< ns3::RlcEntity >', 'rlc')])
    ## radio-bearer-instance.h: void ns3::RadioBearerInstance::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3RlcEntity_methods(root_module, cls):
    ## rlc-entity.h: ns3::RlcEntity::RlcEntity(ns3::RlcEntity const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RlcEntity const &', 'arg0')])
    ## rlc-entity.h: ns3::RlcEntity::RlcEntity() [constructor]
    cls.add_constructor([])
    ## rlc-entity.h: ns3::RlcEntity::RlcEntity(ns3::Ptr<ns3::LteNetDevice> d) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::LteNetDevice >', 'd')])
    ## rlc-entity.h: ns3::Ptr<ns3::Packet> ns3::RlcEntity::Dequeue() [member function]
    cls.add_method('Dequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [])
    ## rlc-entity.h: void ns3::RlcEntity::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## rlc-entity.h: ns3::Ptr<ns3::LteNetDevice> ns3::RlcEntity::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::LteNetDevice >', 
                   [])
    ## rlc-entity.h: ns3::Ptr<ns3::RadioBearerInstance> ns3::RlcEntity::GetRadioBearer() [member function]
    cls.add_method('GetRadioBearer', 
                   'ns3::Ptr< ns3::RadioBearerInstance >', 
                   [])
    ## rlc-entity.h: static ns3::TypeId ns3::RlcEntity::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## rlc-entity.h: void ns3::RlcEntity::SetDevice(ns3::Ptr<ns3::LteNetDevice> d) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::LteNetDevice >', 'd')])
    ## rlc-entity.h: void ns3::RlcEntity::SetRadioBearer(ns3::Ptr<ns3::RadioBearerInstance> b) [member function]
    cls.add_method('SetRadioBearer', 
                   'void', 
                   [param('ns3::Ptr< ns3::RadioBearerInstance >', 'b')])
    return

def register_Ns3RrcEntity_methods(root_module, cls):
    ## rrc-entity.h: ns3::RrcEntity::RrcEntity(ns3::RrcEntity const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RrcEntity const &', 'arg0')])
    ## rrc-entity.h: ns3::RrcEntity::RrcEntity() [constructor]
    cls.add_constructor([])
    ## rrc-entity.h: void ns3::RrcEntity::AddDownlinkGbrBearer(ns3::Ptr<ns3::RadioBearerInstance> bearer) [member function]
    cls.add_method('AddDownlinkGbrBearer', 
                   'void', 
                   [param('ns3::Ptr< ns3::RadioBearerInstance >', 'bearer')])
    ## rrc-entity.h: void ns3::RrcEntity::AddDownlinkNgbrBearer(ns3::Ptr<ns3::RadioBearerInstance> bearer) [member function]
    cls.add_method('AddDownlinkNgbrBearer', 
                   'void', 
                   [param('ns3::Ptr< ns3::RadioBearerInstance >', 'bearer')])
    ## rrc-entity.h: void ns3::RrcEntity::AddUplinkGbrBearer(ns3::Ptr<ns3::RadioBearerInstance> bearer) [member function]
    cls.add_method('AddUplinkGbrBearer', 
                   'void', 
                   [param('ns3::Ptr< ns3::RadioBearerInstance >', 'bearer')])
    ## rrc-entity.h: void ns3::RrcEntity::AddUplinkNgbrBearer(ns3::Ptr<ns3::RadioBearerInstance> bearer) [member function]
    cls.add_method('AddUplinkNgbrBearer', 
                   'void', 
                   [param('ns3::Ptr< ns3::RadioBearerInstance >', 'bearer')])
    ## rrc-entity.h: ns3::Ptr<ns3::RadioBearerInstance> ns3::RrcEntity::Classify(ns3::Ptr<ns3::Packet> p) const [member function]
    cls.add_method('Classify', 
                   'ns3::Ptr< ns3::RadioBearerInstance >', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_const=True)
    ## rrc-entity.h: void ns3::RrcEntity::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## rrc-entity.h: ns3::Ptr<ns3::RadioBearerInstance> ns3::RrcEntity::GetDefaultBearer() [member function]
    cls.add_method('GetDefaultBearer', 
                   'ns3::Ptr< ns3::RadioBearerInstance >', 
                   [])
    ## rrc-entity.h: std::vector<ns3::Ptr<ns3::RadioBearerInstance>,std::allocator<ns3::Ptr<ns3::RadioBearerInstance> > > * ns3::RrcEntity::GetDownlinkGbrBearers() const [member function]
    cls.add_method('GetDownlinkGbrBearers', 
                   'std::vector< ns3::Ptr< ns3::RadioBearerInstance > > *', 
                   [], 
                   is_const=True)
    ## rrc-entity.h: std::vector<ns3::Ptr<ns3::RadioBearerInstance>,std::allocator<ns3::Ptr<ns3::RadioBearerInstance> > > * ns3::RrcEntity::GetDownlinkNgbrBearers() const [member function]
    cls.add_method('GetDownlinkNgbrBearers', 
                   'std::vector< ns3::Ptr< ns3::RadioBearerInstance > > *', 
                   [], 
                   is_const=True)
    ## rrc-entity.h: static ns3::TypeId ns3::RrcEntity::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## rrc-entity.h: std::vector<ns3::Ptr<ns3::RadioBearerInstance>,std::allocator<ns3::Ptr<ns3::RadioBearerInstance> > > * ns3::RrcEntity::GetUplinkGbrBearers() const [member function]
    cls.add_method('GetUplinkGbrBearers', 
                   'std::vector< ns3::Ptr< ns3::RadioBearerInstance > > *', 
                   [], 
                   is_const=True)
    ## rrc-entity.h: std::vector<ns3::Ptr<ns3::RadioBearerInstance>,std::allocator<ns3::Ptr<ns3::RadioBearerInstance> > > * ns3::RrcEntity::GetUplinkNgbrBearers() const [member function]
    cls.add_method('GetUplinkNgbrBearers', 
                   'std::vector< ns3::Ptr< ns3::RadioBearerInstance > > *', 
                   [], 
                   is_const=True)
    return

def register_Ns3SimplePacketScheduler_methods(root_module, cls):
    ## simple-packet-scheduler.h: ns3::SimplePacketScheduler::SimplePacketScheduler(ns3::SimplePacketScheduler const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SimplePacketScheduler const &', 'arg0')])
    ## simple-packet-scheduler.h: ns3::SimplePacketScheduler::SimplePacketScheduler() [constructor]
    cls.add_constructor([])
    ## simple-packet-scheduler.h: ns3::SimplePacketScheduler::SimplePacketScheduler(ns3::Ptr<ns3::EnbNetDevice> enb) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::EnbNetDevice >', 'enb')])
    ## simple-packet-scheduler.h: void ns3::SimplePacketScheduler::DoRunPacketScheduler() [member function]
    cls.add_method('DoRunPacketScheduler', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## simple-packet-scheduler.h: static ns3::TypeId ns3::SimplePacketScheduler::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UanHeaderCommon_methods(root_module, cls):
    ## uan-header-common.h: ns3::UanHeaderCommon::UanHeaderCommon(ns3::UanHeaderCommon const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanHeaderCommon const &', 'arg0')])
    ## uan-header-common.h: ns3::UanHeaderCommon::UanHeaderCommon() [constructor]
    cls.add_constructor([])
    ## uan-header-common.h: ns3::UanHeaderCommon::UanHeaderCommon(ns3::UanAddress const src, ns3::UanAddress const dest, uint8_t type) [constructor]
    cls.add_constructor([param('ns3::UanAddress const', 'src'), param('ns3::UanAddress const', 'dest'), param('uint8_t', 'type')])
    ## uan-header-common.h: uint32_t ns3::UanHeaderCommon::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## uan-header-common.h: ns3::UanAddress ns3::UanHeaderCommon::GetDest() const [member function]
    cls.add_method('GetDest', 
                   'ns3::UanAddress', 
                   [], 
                   is_const=True)
    ## uan-header-common.h: ns3::TypeId ns3::UanHeaderCommon::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-common.h: uint32_t ns3::UanHeaderCommon::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-common.h: ns3::UanAddress ns3::UanHeaderCommon::GetSrc() const [member function]
    cls.add_method('GetSrc', 
                   'ns3::UanAddress', 
                   [], 
                   is_const=True)
    ## uan-header-common.h: uint8_t ns3::UanHeaderCommon::GetType() const [member function]
    cls.add_method('GetType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-header-common.h: static ns3::TypeId ns3::UanHeaderCommon::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-header-common.h: void ns3::UanHeaderCommon::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## uan-header-common.h: void ns3::UanHeaderCommon::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## uan-header-common.h: void ns3::UanHeaderCommon::SetDest(ns3::UanAddress dest) [member function]
    cls.add_method('SetDest', 
                   'void', 
                   [param('ns3::UanAddress', 'dest')])
    ## uan-header-common.h: void ns3::UanHeaderCommon::SetSrc(ns3::UanAddress src) [member function]
    cls.add_method('SetSrc', 
                   'void', 
                   [param('ns3::UanAddress', 'src')])
    ## uan-header-common.h: void ns3::UanHeaderCommon::SetType(uint8_t type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('uint8_t', 'type')])
    return

def register_Ns3UanHeaderRcAck_methods(root_module, cls):
    ## uan-header-rc.h: ns3::UanHeaderRcAck::UanHeaderRcAck(ns3::UanHeaderRcAck const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanHeaderRcAck const &', 'arg0')])
    ## uan-header-rc.h: ns3::UanHeaderRcAck::UanHeaderRcAck() [constructor]
    cls.add_constructor([])
    ## uan-header-rc.h: void ns3::UanHeaderRcAck::AddNackedFrame(uint8_t frame) [member function]
    cls.add_method('AddNackedFrame', 
                   'void', 
                   [param('uint8_t', 'frame')])
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcAck::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## uan-header-rc.h: uint8_t ns3::UanHeaderRcAck::GetFrameNo() const [member function]
    cls.add_method('GetFrameNo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: ns3::TypeId ns3::UanHeaderRcAck::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: std::set<unsigned char, std::less<unsigned char>, std::allocator<unsigned char> > const & ns3::UanHeaderRcAck::GetNackedFrames() const [member function]
    cls.add_method('GetNackedFrames', 
                   'std::set< unsigned char > const &', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint8_t ns3::UanHeaderRcAck::GetNoNacks() const [member function]
    cls.add_method('GetNoNacks', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcAck::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: static ns3::TypeId ns3::UanHeaderRcAck::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcAck::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcAck::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcAck::SetFrameNo(uint8_t frameNo) [member function]
    cls.add_method('SetFrameNo', 
                   'void', 
                   [param('uint8_t', 'frameNo')])
    return

def register_Ns3UanHeaderRcCts_methods(root_module, cls):
    ## uan-header-rc.h: ns3::UanHeaderRcCts::UanHeaderRcCts(ns3::UanHeaderRcCts const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanHeaderRcCts const &', 'arg0')])
    ## uan-header-rc.h: ns3::UanHeaderRcCts::UanHeaderRcCts() [constructor]
    cls.add_constructor([])
    ## uan-header-rc.h: ns3::UanHeaderRcCts::UanHeaderRcCts(uint8_t frameNo, uint8_t retryNo, ns3::Time rtsTs, ns3::Time delay, ns3::UanAddress addr) [constructor]
    cls.add_constructor([param('uint8_t', 'frameNo'), param('uint8_t', 'retryNo'), param('ns3::Time', 'rtsTs'), param('ns3::Time', 'delay'), param('ns3::UanAddress', 'addr')])
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcCts::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## uan-header-rc.h: ns3::UanAddress ns3::UanHeaderRcCts::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::UanAddress', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: ns3::Time ns3::UanHeaderRcCts::GetDelayToTx() const [member function]
    cls.add_method('GetDelayToTx', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint8_t ns3::UanHeaderRcCts::GetFrameNo() const [member function]
    cls.add_method('GetFrameNo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: ns3::TypeId ns3::UanHeaderRcCts::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: uint8_t ns3::UanHeaderRcCts::GetRetryNo() const [member function]
    cls.add_method('GetRetryNo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: ns3::Time ns3::UanHeaderRcCts::GetRtsTimeStamp() const [member function]
    cls.add_method('GetRtsTimeStamp', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcCts::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: static ns3::TypeId ns3::UanHeaderRcCts::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcCts::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcCts::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcCts::SetAddress(ns3::UanAddress addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::UanAddress', 'addr')])
    ## uan-header-rc.h: void ns3::UanHeaderRcCts::SetDelayToTx(ns3::Time delay) [member function]
    cls.add_method('SetDelayToTx', 
                   'void', 
                   [param('ns3::Time', 'delay')])
    ## uan-header-rc.h: void ns3::UanHeaderRcCts::SetFrameNo(uint8_t frameNo) [member function]
    cls.add_method('SetFrameNo', 
                   'void', 
                   [param('uint8_t', 'frameNo')])
    ## uan-header-rc.h: void ns3::UanHeaderRcCts::SetRetryNo(uint8_t no) [member function]
    cls.add_method('SetRetryNo', 
                   'void', 
                   [param('uint8_t', 'no')])
    ## uan-header-rc.h: void ns3::UanHeaderRcCts::SetRtsTimeStamp(ns3::Time timeStamp) [member function]
    cls.add_method('SetRtsTimeStamp', 
                   'void', 
                   [param('ns3::Time', 'timeStamp')])
    return

def register_Ns3UanHeaderRcCtsGlobal_methods(root_module, cls):
    ## uan-header-rc.h: ns3::UanHeaderRcCtsGlobal::UanHeaderRcCtsGlobal(ns3::UanHeaderRcCtsGlobal const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanHeaderRcCtsGlobal const &', 'arg0')])
    ## uan-header-rc.h: ns3::UanHeaderRcCtsGlobal::UanHeaderRcCtsGlobal() [constructor]
    cls.add_constructor([])
    ## uan-header-rc.h: ns3::UanHeaderRcCtsGlobal::UanHeaderRcCtsGlobal(ns3::Time wt, ns3::Time ts, uint16_t rate, uint16_t retryRate) [constructor]
    cls.add_constructor([param('ns3::Time', 'wt'), param('ns3::Time', 'ts'), param('uint16_t', 'rate'), param('uint16_t', 'retryRate')])
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcCtsGlobal::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## uan-header-rc.h: ns3::TypeId ns3::UanHeaderRcCtsGlobal::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: uint16_t ns3::UanHeaderRcCtsGlobal::GetRateNum() const [member function]
    cls.add_method('GetRateNum', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint16_t ns3::UanHeaderRcCtsGlobal::GetRetryRate() const [member function]
    cls.add_method('GetRetryRate', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcCtsGlobal::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: ns3::Time ns3::UanHeaderRcCtsGlobal::GetTxTimeStamp() const [member function]
    cls.add_method('GetTxTimeStamp', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: static ns3::TypeId ns3::UanHeaderRcCtsGlobal::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-header-rc.h: ns3::Time ns3::UanHeaderRcCtsGlobal::GetWindowTime() const [member function]
    cls.add_method('GetWindowTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcCtsGlobal::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcCtsGlobal::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcCtsGlobal::SetRateNum(uint16_t rate) [member function]
    cls.add_method('SetRateNum', 
                   'void', 
                   [param('uint16_t', 'rate')])
    ## uan-header-rc.h: void ns3::UanHeaderRcCtsGlobal::SetRetryRate(uint16_t rate) [member function]
    cls.add_method('SetRetryRate', 
                   'void', 
                   [param('uint16_t', 'rate')])
    ## uan-header-rc.h: void ns3::UanHeaderRcCtsGlobal::SetTxTimeStamp(ns3::Time timeStamp) [member function]
    cls.add_method('SetTxTimeStamp', 
                   'void', 
                   [param('ns3::Time', 'timeStamp')])
    ## uan-header-rc.h: void ns3::UanHeaderRcCtsGlobal::SetWindowTime(ns3::Time t) [member function]
    cls.add_method('SetWindowTime', 
                   'void', 
                   [param('ns3::Time', 't')])
    return

def register_Ns3UanHeaderRcData_methods(root_module, cls):
    ## uan-header-rc.h: ns3::UanHeaderRcData::UanHeaderRcData(ns3::UanHeaderRcData const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanHeaderRcData const &', 'arg0')])
    ## uan-header-rc.h: ns3::UanHeaderRcData::UanHeaderRcData() [constructor]
    cls.add_constructor([])
    ## uan-header-rc.h: ns3::UanHeaderRcData::UanHeaderRcData(uint8_t frameNum, ns3::Time propDelay) [constructor]
    cls.add_constructor([param('uint8_t', 'frameNum'), param('ns3::Time', 'propDelay')])
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcData::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## uan-header-rc.h: uint8_t ns3::UanHeaderRcData::GetFrameNo() const [member function]
    cls.add_method('GetFrameNo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: ns3::TypeId ns3::UanHeaderRcData::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: ns3::Time ns3::UanHeaderRcData::GetPropDelay() const [member function]
    cls.add_method('GetPropDelay', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcData::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: static ns3::TypeId ns3::UanHeaderRcData::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcData::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcData::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcData::SetFrameNo(uint8_t frameNum) [member function]
    cls.add_method('SetFrameNo', 
                   'void', 
                   [param('uint8_t', 'frameNum')])
    ## uan-header-rc.h: void ns3::UanHeaderRcData::SetPropDelay(ns3::Time propDelay) [member function]
    cls.add_method('SetPropDelay', 
                   'void', 
                   [param('ns3::Time', 'propDelay')])
    return

def register_Ns3UanHeaderRcRts_methods(root_module, cls):
    ## uan-header-rc.h: ns3::UanHeaderRcRts::UanHeaderRcRts(ns3::UanHeaderRcRts const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanHeaderRcRts const &', 'arg0')])
    ## uan-header-rc.h: ns3::UanHeaderRcRts::UanHeaderRcRts() [constructor]
    cls.add_constructor([])
    ## uan-header-rc.h: ns3::UanHeaderRcRts::UanHeaderRcRts(uint8_t frameNo, uint8_t retryNo, uint8_t noFrames, uint16_t length, ns3::Time ts) [constructor]
    cls.add_constructor([param('uint8_t', 'frameNo'), param('uint8_t', 'retryNo'), param('uint8_t', 'noFrames'), param('uint16_t', 'length'), param('ns3::Time', 'ts')])
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcRts::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## uan-header-rc.h: uint8_t ns3::UanHeaderRcRts::GetFrameNo() const [member function]
    cls.add_method('GetFrameNo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: ns3::TypeId ns3::UanHeaderRcRts::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: uint16_t ns3::UanHeaderRcRts::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint8_t ns3::UanHeaderRcRts::GetNoFrames() const [member function]
    cls.add_method('GetNoFrames', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint8_t ns3::UanHeaderRcRts::GetRetryNo() const [member function]
    cls.add_method('GetRetryNo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcRts::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: ns3::Time ns3::UanHeaderRcRts::GetTimeStamp() const [member function]
    cls.add_method('GetTimeStamp', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: static ns3::TypeId ns3::UanHeaderRcRts::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcRts::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcRts::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcRts::SetFrameNo(uint8_t fno) [member function]
    cls.add_method('SetFrameNo', 
                   'void', 
                   [param('uint8_t', 'fno')])
    ## uan-header-rc.h: void ns3::UanHeaderRcRts::SetLength(uint16_t length) [member function]
    cls.add_method('SetLength', 
                   'void', 
                   [param('uint16_t', 'length')])
    ## uan-header-rc.h: void ns3::UanHeaderRcRts::SetNoFrames(uint8_t no) [member function]
    cls.add_method('SetNoFrames', 
                   'void', 
                   [param('uint8_t', 'no')])
    ## uan-header-rc.h: void ns3::UanHeaderRcRts::SetRetryNo(uint8_t no) [member function]
    cls.add_method('SetRetryNo', 
                   'void', 
                   [param('uint8_t', 'no')])
    ## uan-header-rc.h: void ns3::UanHeaderRcRts::SetTimeStamp(ns3::Time timeStamp) [member function]
    cls.add_method('SetTimeStamp', 
                   'void', 
                   [param('ns3::Time', 'timeStamp')])
    return

def register_Ns3UanMac_methods(root_module, cls):
    ## uan-mac.h: ns3::UanMac::UanMac() [constructor]
    cls.add_constructor([])
    ## uan-mac.h: ns3::UanMac::UanMac(ns3::UanMac const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanMac const &', 'arg0')])
    ## uan-mac.h: void ns3::UanMac::AttachPhy(ns3::Ptr<ns3::UanPhy> phy) [member function]
    cls.add_method('AttachPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'phy')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-mac.h: void ns3::UanMac::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-mac.h: bool ns3::UanMac::Enqueue(ns3::Ptr<ns3::Packet> pkt, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-mac.h: ns3::Address ns3::UanMac::GetAddress() [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-mac.h: ns3::Address ns3::UanMac::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-mac.h: void ns3::UanMac::SetAddress(ns3::UanAddress addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::UanAddress', 'addr')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-mac.h: void ns3::UanMac::SetForwardUpCb(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::UanAddress const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetForwardUpCb', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::UanAddress const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3UanMacAloha_methods(root_module, cls):
    ## uan-mac-aloha.h: ns3::UanMacAloha::UanMacAloha(ns3::UanMacAloha const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanMacAloha const &', 'arg0')])
    ## uan-mac-aloha.h: ns3::UanMacAloha::UanMacAloha() [constructor]
    cls.add_constructor([])
    ## uan-mac-aloha.h: void ns3::UanMacAloha::AttachPhy(ns3::Ptr<ns3::UanPhy> phy) [member function]
    cls.add_method('AttachPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'phy')], 
                   is_virtual=True)
    ## uan-mac-aloha.h: void ns3::UanMacAloha::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-mac-aloha.h: bool ns3::UanMacAloha::Enqueue(ns3::Ptr<ns3::Packet> pkt, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## uan-mac-aloha.h: ns3::Address ns3::UanMacAloha::GetAddress() [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_virtual=True)
    ## uan-mac-aloha.h: ns3::Address ns3::UanMacAloha::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-mac-aloha.h: static ns3::TypeId ns3::UanMacAloha::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-mac-aloha.h: void ns3::UanMacAloha::SetAddress(ns3::UanAddress addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::UanAddress', 'addr')], 
                   is_virtual=True)
    ## uan-mac-aloha.h: void ns3::UanMacAloha::SetForwardUpCb(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::UanAddress const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetForwardUpCb', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::UanAddress const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-mac-aloha.h: void ns3::UanMacAloha::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3UanMacCw_methods(root_module, cls):
    ## uan-mac-cw.h: ns3::UanMacCw::UanMacCw(ns3::UanMacCw const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanMacCw const &', 'arg0')])
    ## uan-mac-cw.h: ns3::UanMacCw::UanMacCw() [constructor]
    cls.add_constructor([])
    ## uan-mac-cw.h: void ns3::UanMacCw::AttachPhy(ns3::Ptr<ns3::UanPhy> phy) [member function]
    cls.add_method('AttachPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'phy')], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-mac-cw.h: bool ns3::UanMacCw::Enqueue(ns3::Ptr<ns3::Packet> pkt, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## uan-mac-cw.h: ns3::Address ns3::UanMacCw::GetAddress() [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_virtual=True)
    ## uan-mac-cw.h: ns3::Address ns3::UanMacCw::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-mac-cw.h: uint32_t ns3::UanMacCw::GetCw() [member function]
    cls.add_method('GetCw', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    ## uan-mac-cw.h: ns3::Time ns3::UanMacCw::GetSlotTime() [member function]
    cls.add_method('GetSlotTime', 
                   'ns3::Time', 
                   [], 
                   is_virtual=True)
    ## uan-mac-cw.h: static ns3::TypeId ns3::UanMacCw::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::NotifyCcaEnd() [member function]
    cls.add_method('NotifyCcaEnd', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::NotifyCcaStart() [member function]
    cls.add_method('NotifyCcaStart', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::NotifyRxEndError() [member function]
    cls.add_method('NotifyRxEndError', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::NotifyRxEndOk() [member function]
    cls.add_method('NotifyRxEndOk', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::NotifyRxStart() [member function]
    cls.add_method('NotifyRxStart', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::NotifyTxStart(ns3::Time duration) [member function]
    cls.add_method('NotifyTxStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::SetAddress(ns3::UanAddress addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::UanAddress', 'addr')], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::SetCw(uint32_t cw) [member function]
    cls.add_method('SetCw', 
                   'void', 
                   [param('uint32_t', 'cw')], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::SetForwardUpCb(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::UanAddress const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetForwardUpCb', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::UanAddress const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::SetSlotTime(ns3::Time duration) [member function]
    cls.add_method('SetSlotTime', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3UanMacRc_methods(root_module, cls):
    ## uan-mac-rc.h: ns3::UanMacRc::UanMacRc(ns3::UanMacRc const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanMacRc const &', 'arg0')])
    ## uan-mac-rc.h: ns3::UanMacRc::UanMacRc() [constructor]
    cls.add_constructor([])
    ## uan-mac-rc.h: void ns3::UanMacRc::AttachPhy(ns3::Ptr<ns3::UanPhy> phy) [member function]
    cls.add_method('AttachPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'phy')], 
                   is_virtual=True)
    ## uan-mac-rc.h: void ns3::UanMacRc::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-mac-rc.h: bool ns3::UanMacRc::Enqueue(ns3::Ptr<ns3::Packet> pkt, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## uan-mac-rc.h: ns3::Address ns3::UanMacRc::GetAddress() [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_virtual=True)
    ## uan-mac-rc.h: ns3::Address ns3::UanMacRc::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-mac-rc.h: static ns3::TypeId ns3::UanMacRc::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-mac-rc.h: void ns3::UanMacRc::SetAddress(ns3::UanAddress addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::UanAddress', 'addr')], 
                   is_virtual=True)
    ## uan-mac-rc.h: void ns3::UanMacRc::SetForwardUpCb(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::UanAddress const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetForwardUpCb', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::UanAddress const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-mac-rc.h: void ns3::UanMacRc::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3UanMacRcGw_methods(root_module, cls):
    ## uan-mac-rc-gw.h: ns3::UanMacRcGw::UanMacRcGw(ns3::UanMacRcGw const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanMacRcGw const &', 'arg0')])
    ## uan-mac-rc-gw.h: ns3::UanMacRcGw::UanMacRcGw() [constructor]
    cls.add_constructor([])
    ## uan-mac-rc-gw.h: void ns3::UanMacRcGw::AttachPhy(ns3::Ptr<ns3::UanPhy> phy) [member function]
    cls.add_method('AttachPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'phy')], 
                   is_virtual=True)
    ## uan-mac-rc-gw.h: void ns3::UanMacRcGw::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-mac-rc-gw.h: bool ns3::UanMacRcGw::Enqueue(ns3::Ptr<ns3::Packet> pkt, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## uan-mac-rc-gw.h: ns3::Address ns3::UanMacRcGw::GetAddress() [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_virtual=True)
    ## uan-mac-rc-gw.h: ns3::Address ns3::UanMacRcGw::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-mac-rc-gw.h: static ns3::TypeId ns3::UanMacRcGw::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-mac-rc-gw.h: void ns3::UanMacRcGw::SetAddress(ns3::UanAddress addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::UanAddress', 'addr')], 
                   is_virtual=True)
    ## uan-mac-rc-gw.h: void ns3::UanMacRcGw::SetForwardUpCb(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::UanAddress const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetForwardUpCb', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::UanAddress const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-mac-rc-gw.h: void ns3::UanMacRcGw::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3UanNoiseModel_methods(root_module, cls):
    ## uan-noise-model.h: ns3::UanNoiseModel::UanNoiseModel() [constructor]
    cls.add_constructor([])
    ## uan-noise-model.h: ns3::UanNoiseModel::UanNoiseModel(ns3::UanNoiseModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanNoiseModel const &', 'arg0')])
    ## uan-noise-model.h: void ns3::UanNoiseModel::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-noise-model.h: void ns3::UanNoiseModel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-noise-model.h: double ns3::UanNoiseModel::GetNoiseDbHz(double fKhz) const [member function]
    cls.add_method('GetNoiseDbHz', 
                   'double', 
                   [param('double', 'fKhz')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3UanNoiseModelDefault_methods(root_module, cls):
    ## uan-noise-model-default.h: ns3::UanNoiseModelDefault::UanNoiseModelDefault(ns3::UanNoiseModelDefault const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanNoiseModelDefault const &', 'arg0')])
    ## uan-noise-model-default.h: ns3::UanNoiseModelDefault::UanNoiseModelDefault() [constructor]
    cls.add_constructor([])
    ## uan-noise-model-default.h: double ns3::UanNoiseModelDefault::GetNoiseDbHz(double fKhz) const [member function]
    cls.add_method('GetNoiseDbHz', 
                   'double', 
                   [param('double', 'fKhz')], 
                   is_const=True, is_virtual=True)
    ## uan-noise-model-default.h: static ns3::TypeId ns3::UanNoiseModelDefault::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UanPhy_methods(root_module, cls):
    ## uan-phy.h: ns3::UanPhy::UanPhy() [constructor]
    cls.add_constructor([])
    ## uan-phy.h: ns3::UanPhy::UanPhy(ns3::UanPhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhy const &', 'arg0')])
    ## uan-phy.h: void ns3::UanPhy::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::EnergyDepletionHandler() [member function]
    cls.add_method('EnergyDepletionHandler', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: double ns3::UanPhy::GetCcaThresholdDb() [member function]
    cls.add_method('GetCcaThresholdDb', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: ns3::Ptr<ns3::UanChannel> ns3::UanPhy::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::UanChannel >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-phy.h: ns3::Ptr<ns3::UanNetDevice> ns3::UanPhy::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::UanNetDevice >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: ns3::UanTxMode ns3::UanPhy::GetMode(uint32_t n) [member function]
    cls.add_method('GetMode', 
                   'ns3::UanTxMode', 
                   [param('uint32_t', 'n')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: uint32_t ns3::UanPhy::GetNModes() [member function]
    cls.add_method('GetNModes', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: ns3::Ptr<ns3::Packet> ns3::UanPhy::GetPacketRx() const [member function]
    cls.add_method('GetPacketRx', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-phy.h: double ns3::UanPhy::GetRxGainDb() [member function]
    cls.add_method('GetRxGainDb', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: double ns3::UanPhy::GetRxThresholdDb() [member function]
    cls.add_method('GetRxThresholdDb', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: ns3::Ptr<ns3::UanTransducer> ns3::UanPhy::GetTransducer() [member function]
    cls.add_method('GetTransducer', 
                   'ns3::Ptr< ns3::UanTransducer >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: double ns3::UanPhy::GetTxPowerDb() [member function]
    cls.add_method('GetTxPowerDb', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: bool ns3::UanPhy::IsStateBusy() [member function]
    cls.add_method('IsStateBusy', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: bool ns3::UanPhy::IsStateCcaBusy() [member function]
    cls.add_method('IsStateCcaBusy', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: bool ns3::UanPhy::IsStateIdle() [member function]
    cls.add_method('IsStateIdle', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: bool ns3::UanPhy::IsStateRx() [member function]
    cls.add_method('IsStateRx', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: bool ns3::UanPhy::IsStateSleep() [member function]
    cls.add_method('IsStateSleep', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: bool ns3::UanPhy::IsStateTx() [member function]
    cls.add_method('IsStateTx', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::NotifyIntChange() [member function]
    cls.add_method('NotifyIntChange', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::NotifyTransStartTx(ns3::Ptr<ns3::Packet> packet, double txPowerDb, ns3::UanTxMode txMode) [member function]
    cls.add_method('NotifyTransStartTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'txPowerDb'), param('ns3::UanTxMode', 'txMode')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::RegisterListener(ns3::UanPhyListener * listener) [member function]
    cls.add_method('RegisterListener', 
                   'void', 
                   [param('ns3::UanPhyListener *', 'listener')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SendPacket(ns3::Ptr<ns3::Packet> pkt, uint32_t modeNum) [member function]
    cls.add_method('SendPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('uint32_t', 'modeNum')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetCcaThresholdDb(double thresh) [member function]
    cls.add_method('SetCcaThresholdDb', 
                   'void', 
                   [param('double', 'thresh')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetChannel(ns3::Ptr<ns3::UanChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanChannel >', 'channel')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetDevice(ns3::Ptr<ns3::UanNetDevice> device) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanNetDevice >', 'device')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetEnergyModelCallback(ns3::Callback<void, int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetEnergyModelCallback', 
                   'void', 
                   [param('ns3::Callback< void, int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetMac(ns3::Ptr<ns3::UanMac> mac) [member function]
    cls.add_method('SetMac', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanMac >', 'mac')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetReceiveErrorCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, double, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveErrorCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetReceiveOkCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, double, ns3::UanTxMode, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveOkCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, ns3::UanTxMode, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetRxGainDb(double gain) [member function]
    cls.add_method('SetRxGainDb', 
                   'void', 
                   [param('double', 'gain')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetRxThresholdDb(double thresh) [member function]
    cls.add_method('SetRxThresholdDb', 
                   'void', 
                   [param('double', 'thresh')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetTransducer(ns3::Ptr<ns3::UanTransducer> trans) [member function]
    cls.add_method('SetTransducer', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanTransducer >', 'trans')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetTxPowerDb(double txpwr) [member function]
    cls.add_method('SetTxPowerDb', 
                   'void', 
                   [param('double', 'txpwr')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::StartRxPacket(ns3::Ptr<ns3::Packet> pkt, double rxPowerDb, ns3::UanTxMode txMode, ns3::UanPdp pdp) [member function]
    cls.add_method('StartRxPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('double', 'rxPowerDb'), param('ns3::UanTxMode', 'txMode'), param('ns3::UanPdp', 'pdp')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3UanPhyCalcSinr_methods(root_module, cls):
    ## uan-phy.h: ns3::UanPhyCalcSinr::UanPhyCalcSinr() [constructor]
    cls.add_constructor([])
    ## uan-phy.h: ns3::UanPhyCalcSinr::UanPhyCalcSinr(ns3::UanPhyCalcSinr const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyCalcSinr const &', 'arg0')])
    ## uan-phy.h: double ns3::UanPhyCalcSinr::CalcSinrDb(ns3::Ptr<ns3::Packet> pkt, ns3::Time arrTime, double rxPowerDb, double ambNoiseDb, ns3::UanTxMode mode, ns3::UanPdp pdp, std::list<ns3::UanPacketArrival, std::allocator<ns3::UanPacketArrival> > const & arrivalList) const [member function]
    cls.add_method('CalcSinrDb', 
                   'double', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::Time', 'arrTime'), param('double', 'rxPowerDb'), param('double', 'ambNoiseDb'), param('ns3::UanTxMode', 'mode'), param('ns3::UanPdp', 'pdp'), param('std::list< ns3::UanPacketArrival > const &', 'arrivalList')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhyCalcSinr::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-phy.h: double ns3::UanPhyCalcSinr::DbToKp(double db) const [member function]
    cls.add_method('DbToKp', 
                   'double', 
                   [param('double', 'db')], 
                   is_const=True)
    ## uan-phy.h: void ns3::UanPhyCalcSinr::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-phy.h: double ns3::UanPhyCalcSinr::KpToDb(double kp) const [member function]
    cls.add_method('KpToDb', 
                   'double', 
                   [param('double', 'kp')], 
                   is_const=True)
    return

def register_Ns3UanPhyCalcSinrDefault_methods(root_module, cls):
    ## uan-phy-gen.h: ns3::UanPhyCalcSinrDefault::UanPhyCalcSinrDefault(ns3::UanPhyCalcSinrDefault const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyCalcSinrDefault const &', 'arg0')])
    ## uan-phy-gen.h: ns3::UanPhyCalcSinrDefault::UanPhyCalcSinrDefault() [constructor]
    cls.add_constructor([])
    ## uan-phy-gen.h: double ns3::UanPhyCalcSinrDefault::CalcSinrDb(ns3::Ptr<ns3::Packet> pkt, ns3::Time arrTime, double rxPowerDb, double ambNoiseDb, ns3::UanTxMode mode, ns3::UanPdp pdp, std::list<ns3::UanPacketArrival, std::allocator<ns3::UanPacketArrival> > const & arrivalList) const [member function]
    cls.add_method('CalcSinrDb', 
                   'double', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::Time', 'arrTime'), param('double', 'rxPowerDb'), param('double', 'ambNoiseDb'), param('ns3::UanTxMode', 'mode'), param('ns3::UanPdp', 'pdp'), param('std::list< ns3::UanPacketArrival > const &', 'arrivalList')], 
                   is_const=True, is_virtual=True)
    ## uan-phy-gen.h: static ns3::TypeId ns3::UanPhyCalcSinrDefault::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UanPhyCalcSinrDual_methods(root_module, cls):
    ## uan-phy-dual.h: ns3::UanPhyCalcSinrDual::UanPhyCalcSinrDual(ns3::UanPhyCalcSinrDual const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyCalcSinrDual const &', 'arg0')])
    ## uan-phy-dual.h: ns3::UanPhyCalcSinrDual::UanPhyCalcSinrDual() [constructor]
    cls.add_constructor([])
    ## uan-phy-dual.h: double ns3::UanPhyCalcSinrDual::CalcSinrDb(ns3::Ptr<ns3::Packet> pkt, ns3::Time arrTime, double rxPowerDb, double ambNoiseDb, ns3::UanTxMode mode, ns3::UanPdp pdp, std::list<ns3::UanPacketArrival, std::allocator<ns3::UanPacketArrival> > const & arrivalList) const [member function]
    cls.add_method('CalcSinrDb', 
                   'double', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::Time', 'arrTime'), param('double', 'rxPowerDb'), param('double', 'ambNoiseDb'), param('ns3::UanTxMode', 'mode'), param('ns3::UanPdp', 'pdp'), param('std::list< ns3::UanPacketArrival > const &', 'arrivalList')], 
                   is_const=True, is_virtual=True)
    ## uan-phy-dual.h: static ns3::TypeId ns3::UanPhyCalcSinrDual::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UanPhyCalcSinrFhFsk_methods(root_module, cls):
    ## uan-phy-gen.h: ns3::UanPhyCalcSinrFhFsk::UanPhyCalcSinrFhFsk(ns3::UanPhyCalcSinrFhFsk const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyCalcSinrFhFsk const &', 'arg0')])
    ## uan-phy-gen.h: ns3::UanPhyCalcSinrFhFsk::UanPhyCalcSinrFhFsk() [constructor]
    cls.add_constructor([])
    ## uan-phy-gen.h: double ns3::UanPhyCalcSinrFhFsk::CalcSinrDb(ns3::Ptr<ns3::Packet> pkt, ns3::Time arrTime, double rxPowerDb, double ambNoiseDb, ns3::UanTxMode mode, ns3::UanPdp pdp, std::list<ns3::UanPacketArrival, std::allocator<ns3::UanPacketArrival> > const & arrivalList) const [member function]
    cls.add_method('CalcSinrDb', 
                   'double', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::Time', 'arrTime'), param('double', 'rxPowerDb'), param('double', 'ambNoiseDb'), param('ns3::UanTxMode', 'mode'), param('ns3::UanPdp', 'pdp'), param('std::list< ns3::UanPacketArrival > const &', 'arrivalList')], 
                   is_const=True, is_virtual=True)
    ## uan-phy-gen.h: static ns3::TypeId ns3::UanPhyCalcSinrFhFsk::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UanPhyDual_methods(root_module, cls):
    ## uan-phy-dual.h: ns3::UanPhyDual::UanPhyDual(ns3::UanPhyDual const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyDual const &', 'arg0')])
    ## uan-phy-dual.h: ns3::UanPhyDual::UanPhyDual() [constructor]
    cls.add_constructor([])
    ## uan-phy-dual.h: void ns3::UanPhyDual::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::EnergyDepletionHandler() [member function]
    cls.add_method('EnergyDepletionHandler', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetCcaThresholdDb() [member function]
    cls.add_method('GetCcaThresholdDb', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetCcaThresholdPhy1() const [member function]
    cls.add_method('GetCcaThresholdPhy1', 
                   'double', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetCcaThresholdPhy2() const [member function]
    cls.add_method('GetCcaThresholdPhy2', 
                   'double', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::UanChannel> ns3::UanPhyDual::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::UanChannel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::UanNetDevice> ns3::UanPhyDual::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::UanNetDevice >', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: ns3::UanTxMode ns3::UanPhyDual::GetMode(uint32_t n) [member function]
    cls.add_method('GetMode', 
                   'ns3::UanTxMode', 
                   [param('uint32_t', 'n')], 
                   is_virtual=True)
    ## uan-phy-dual.h: ns3::UanModesList ns3::UanPhyDual::GetModesPhy1() const [member function]
    cls.add_method('GetModesPhy1', 
                   'ns3::UanModesList', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: ns3::UanModesList ns3::UanPhyDual::GetModesPhy2() const [member function]
    cls.add_method('GetModesPhy2', 
                   'ns3::UanModesList', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: uint32_t ns3::UanPhyDual::GetNModes() [member function]
    cls.add_method('GetNModes', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::Packet> ns3::UanPhyDual::GetPacketRx() const [member function]
    cls.add_method('GetPacketRx', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::UanPhyPer> ns3::UanPhyDual::GetPerModelPhy1() const [member function]
    cls.add_method('GetPerModelPhy1', 
                   'ns3::Ptr< ns3::UanPhyPer >', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::UanPhyPer> ns3::UanPhyDual::GetPerModelPhy2() const [member function]
    cls.add_method('GetPerModelPhy2', 
                   'ns3::Ptr< ns3::UanPhyPer >', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::Packet> ns3::UanPhyDual::GetPhy1PacketRx() const [member function]
    cls.add_method('GetPhy1PacketRx', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::Packet> ns3::UanPhyDual::GetPhy2PacketRx() const [member function]
    cls.add_method('GetPhy2PacketRx', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetRxGainDb() [member function]
    cls.add_method('GetRxGainDb', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetRxGainDbPhy1() const [member function]
    cls.add_method('GetRxGainDbPhy1', 
                   'double', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetRxGainDbPhy2() const [member function]
    cls.add_method('GetRxGainDbPhy2', 
                   'double', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetRxThresholdDb() [member function]
    cls.add_method('GetRxThresholdDb', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::UanPhyCalcSinr> ns3::UanPhyDual::GetSinrModelPhy1() const [member function]
    cls.add_method('GetSinrModelPhy1', 
                   'ns3::Ptr< ns3::UanPhyCalcSinr >', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::UanPhyCalcSinr> ns3::UanPhyDual::GetSinrModelPhy2() const [member function]
    cls.add_method('GetSinrModelPhy2', 
                   'ns3::Ptr< ns3::UanPhyCalcSinr >', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::UanTransducer> ns3::UanPhyDual::GetTransducer() [member function]
    cls.add_method('GetTransducer', 
                   'ns3::Ptr< ns3::UanTransducer >', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetTxPowerDb() [member function]
    cls.add_method('GetTxPowerDb', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetTxPowerDbPhy1() const [member function]
    cls.add_method('GetTxPowerDbPhy1', 
                   'double', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetTxPowerDbPhy2() const [member function]
    cls.add_method('GetTxPowerDbPhy2', 
                   'double', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: static ns3::TypeId ns3::UanPhyDual::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsPhy1Idle() [member function]
    cls.add_method('IsPhy1Idle', 
                   'bool', 
                   [])
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsPhy1Rx() [member function]
    cls.add_method('IsPhy1Rx', 
                   'bool', 
                   [])
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsPhy1Tx() [member function]
    cls.add_method('IsPhy1Tx', 
                   'bool', 
                   [])
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsPhy2Idle() [member function]
    cls.add_method('IsPhy2Idle', 
                   'bool', 
                   [])
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsPhy2Rx() [member function]
    cls.add_method('IsPhy2Rx', 
                   'bool', 
                   [])
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsPhy2Tx() [member function]
    cls.add_method('IsPhy2Tx', 
                   'bool', 
                   [])
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsStateBusy() [member function]
    cls.add_method('IsStateBusy', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsStateCcaBusy() [member function]
    cls.add_method('IsStateCcaBusy', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsStateIdle() [member function]
    cls.add_method('IsStateIdle', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsStateRx() [member function]
    cls.add_method('IsStateRx', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsStateSleep() [member function]
    cls.add_method('IsStateSleep', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsStateTx() [member function]
    cls.add_method('IsStateTx', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::NotifyIntChange() [member function]
    cls.add_method('NotifyIntChange', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::NotifyTransStartTx(ns3::Ptr<ns3::Packet> packet, double txPowerDb, ns3::UanTxMode txMode) [member function]
    cls.add_method('NotifyTransStartTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'txPowerDb'), param('ns3::UanTxMode', 'txMode')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::RegisterListener(ns3::UanPhyListener * listener) [member function]
    cls.add_method('RegisterListener', 
                   'void', 
                   [param('ns3::UanPhyListener *', 'listener')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SendPacket(ns3::Ptr<ns3::Packet> pkt, uint32_t modeNum) [member function]
    cls.add_method('SendPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('uint32_t', 'modeNum')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetCcaThresholdDb(double thresh) [member function]
    cls.add_method('SetCcaThresholdDb', 
                   'void', 
                   [param('double', 'thresh')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetCcaThresholdPhy1(double thresh) [member function]
    cls.add_method('SetCcaThresholdPhy1', 
                   'void', 
                   [param('double', 'thresh')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetCcaThresholdPhy2(double thresh) [member function]
    cls.add_method('SetCcaThresholdPhy2', 
                   'void', 
                   [param('double', 'thresh')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetChannel(ns3::Ptr<ns3::UanChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanChannel >', 'channel')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetDevice(ns3::Ptr<ns3::UanNetDevice> device) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanNetDevice >', 'device')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetEnergyModelCallback(ns3::Callback<void, int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetEnergyModelCallback', 
                   'void', 
                   [param('ns3::Callback< void, int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetMac(ns3::Ptr<ns3::UanMac> mac) [member function]
    cls.add_method('SetMac', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanMac >', 'mac')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetModesPhy1(ns3::UanModesList modes) [member function]
    cls.add_method('SetModesPhy1', 
                   'void', 
                   [param('ns3::UanModesList', 'modes')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetModesPhy2(ns3::UanModesList modes) [member function]
    cls.add_method('SetModesPhy2', 
                   'void', 
                   [param('ns3::UanModesList', 'modes')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetPerModelPhy1(ns3::Ptr<ns3::UanPhyPer> per) [member function]
    cls.add_method('SetPerModelPhy1', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhyPer >', 'per')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetPerModelPhy2(ns3::Ptr<ns3::UanPhyPer> per) [member function]
    cls.add_method('SetPerModelPhy2', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhyPer >', 'per')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetReceiveErrorCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, double, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveErrorCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetReceiveOkCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, double, ns3::UanTxMode, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveOkCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, ns3::UanTxMode, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetRxGainDb(double gain) [member function]
    cls.add_method('SetRxGainDb', 
                   'void', 
                   [param('double', 'gain')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetRxGainDbPhy1(double gain) [member function]
    cls.add_method('SetRxGainDbPhy1', 
                   'void', 
                   [param('double', 'gain')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetRxGainDbPhy2(double gain) [member function]
    cls.add_method('SetRxGainDbPhy2', 
                   'void', 
                   [param('double', 'gain')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetRxThresholdDb(double thresh) [member function]
    cls.add_method('SetRxThresholdDb', 
                   'void', 
                   [param('double', 'thresh')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetSinrModelPhy1(ns3::Ptr<ns3::UanPhyCalcSinr> calcSinr) [member function]
    cls.add_method('SetSinrModelPhy1', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhyCalcSinr >', 'calcSinr')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetSinrModelPhy2(ns3::Ptr<ns3::UanPhyCalcSinr> calcSinr) [member function]
    cls.add_method('SetSinrModelPhy2', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhyCalcSinr >', 'calcSinr')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetTransducer(ns3::Ptr<ns3::UanTransducer> trans) [member function]
    cls.add_method('SetTransducer', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanTransducer >', 'trans')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetTxPowerDb(double txpwr) [member function]
    cls.add_method('SetTxPowerDb', 
                   'void', 
                   [param('double', 'txpwr')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetTxPowerDbPhy1(double arg0) [member function]
    cls.add_method('SetTxPowerDbPhy1', 
                   'void', 
                   [param('double', 'arg0')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetTxPowerDbPhy2(double arg0) [member function]
    cls.add_method('SetTxPowerDbPhy2', 
                   'void', 
                   [param('double', 'arg0')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::StartRxPacket(ns3::Ptr<ns3::Packet> pkt, double rxPowerDb, ns3::UanTxMode txMode, ns3::UanPdp pdp) [member function]
    cls.add_method('StartRxPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('double', 'rxPowerDb'), param('ns3::UanTxMode', 'txMode'), param('ns3::UanPdp', 'pdp')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3UanPhyGen_methods(root_module, cls):
    ## uan-phy-gen.h: ns3::UanPhyGen::UanPhyGen(ns3::UanPhyGen const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyGen const &', 'arg0')])
    ## uan-phy-gen.h: ns3::UanPhyGen::UanPhyGen() [constructor]
    cls.add_constructor([])
    ## uan-phy-gen.h: void ns3::UanPhyGen::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::EnergyDepletionHandler() [member function]
    cls.add_method('EnergyDepletionHandler', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: double ns3::UanPhyGen::GetCcaThresholdDb() [member function]
    cls.add_method('GetCcaThresholdDb', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: ns3::Ptr<ns3::UanChannel> ns3::UanPhyGen::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::UanChannel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-phy-gen.h: static ns3::UanModesList ns3::UanPhyGen::GetDefaultModes() [member function]
    cls.add_method('GetDefaultModes', 
                   'ns3::UanModesList', 
                   [], 
                   is_static=True)
    ## uan-phy-gen.h: ns3::Ptr<ns3::UanNetDevice> ns3::UanPhyGen::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::UanNetDevice >', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: ns3::UanTxMode ns3::UanPhyGen::GetMode(uint32_t n) [member function]
    cls.add_method('GetMode', 
                   'ns3::UanTxMode', 
                   [param('uint32_t', 'n')], 
                   is_virtual=True)
    ## uan-phy-gen.h: uint32_t ns3::UanPhyGen::GetNModes() [member function]
    cls.add_method('GetNModes', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: ns3::Ptr<ns3::Packet> ns3::UanPhyGen::GetPacketRx() const [member function]
    cls.add_method('GetPacketRx', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-phy-gen.h: double ns3::UanPhyGen::GetRxGainDb() [member function]
    cls.add_method('GetRxGainDb', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: double ns3::UanPhyGen::GetRxThresholdDb() [member function]
    cls.add_method('GetRxThresholdDb', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: ns3::Ptr<ns3::UanTransducer> ns3::UanPhyGen::GetTransducer() [member function]
    cls.add_method('GetTransducer', 
                   'ns3::Ptr< ns3::UanTransducer >', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: double ns3::UanPhyGen::GetTxPowerDb() [member function]
    cls.add_method('GetTxPowerDb', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: static ns3::TypeId ns3::UanPhyGen::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-phy-gen.h: bool ns3::UanPhyGen::IsStateBusy() [member function]
    cls.add_method('IsStateBusy', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: bool ns3::UanPhyGen::IsStateCcaBusy() [member function]
    cls.add_method('IsStateCcaBusy', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: bool ns3::UanPhyGen::IsStateIdle() [member function]
    cls.add_method('IsStateIdle', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: bool ns3::UanPhyGen::IsStateRx() [member function]
    cls.add_method('IsStateRx', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: bool ns3::UanPhyGen::IsStateSleep() [member function]
    cls.add_method('IsStateSleep', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: bool ns3::UanPhyGen::IsStateTx() [member function]
    cls.add_method('IsStateTx', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::NotifyIntChange() [member function]
    cls.add_method('NotifyIntChange', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::NotifyTransStartTx(ns3::Ptr<ns3::Packet> packet, double txPowerDb, ns3::UanTxMode txMode) [member function]
    cls.add_method('NotifyTransStartTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'txPowerDb'), param('ns3::UanTxMode', 'txMode')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::RegisterListener(ns3::UanPhyListener * listener) [member function]
    cls.add_method('RegisterListener', 
                   'void', 
                   [param('ns3::UanPhyListener *', 'listener')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SendPacket(ns3::Ptr<ns3::Packet> pkt, uint32_t modeNum) [member function]
    cls.add_method('SendPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('uint32_t', 'modeNum')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetCcaThresholdDb(double thresh) [member function]
    cls.add_method('SetCcaThresholdDb', 
                   'void', 
                   [param('double', 'thresh')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetChannel(ns3::Ptr<ns3::UanChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanChannel >', 'channel')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetDevice(ns3::Ptr<ns3::UanNetDevice> device) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanNetDevice >', 'device')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetEnergyModelCallback(ns3::Callback<void, int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetEnergyModelCallback', 
                   'void', 
                   [param('ns3::Callback< void, int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetMac(ns3::Ptr<ns3::UanMac> mac) [member function]
    cls.add_method('SetMac', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanMac >', 'mac')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetReceiveErrorCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, double, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveErrorCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetReceiveOkCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, double, ns3::UanTxMode, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveOkCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, ns3::UanTxMode, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetRxGainDb(double gain) [member function]
    cls.add_method('SetRxGainDb', 
                   'void', 
                   [param('double', 'gain')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetRxThresholdDb(double thresh) [member function]
    cls.add_method('SetRxThresholdDb', 
                   'void', 
                   [param('double', 'thresh')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetTransducer(ns3::Ptr<ns3::UanTransducer> trans) [member function]
    cls.add_method('SetTransducer', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanTransducer >', 'trans')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetTxPowerDb(double txpwr) [member function]
    cls.add_method('SetTxPowerDb', 
                   'void', 
                   [param('double', 'txpwr')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::StartRxPacket(ns3::Ptr<ns3::Packet> pkt, double rxPowerDb, ns3::UanTxMode txMode, ns3::UanPdp pdp) [member function]
    cls.add_method('StartRxPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('double', 'rxPowerDb'), param('ns3::UanTxMode', 'txMode'), param('ns3::UanPdp', 'pdp')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3UanPhyPer_methods(root_module, cls):
    ## uan-phy.h: ns3::UanPhyPer::UanPhyPer() [constructor]
    cls.add_constructor([])
    ## uan-phy.h: ns3::UanPhyPer::UanPhyPer(ns3::UanPhyPer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyPer const &', 'arg0')])
    ## uan-phy.h: double ns3::UanPhyPer::CalcPer(ns3::Ptr<ns3::Packet> pkt, double sinrDb, ns3::UanTxMode mode) [member function]
    cls.add_method('CalcPer', 
                   'double', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('double', 'sinrDb'), param('ns3::UanTxMode', 'mode')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhyPer::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-phy.h: void ns3::UanPhyPer::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3UanPhyPerGenDefault_methods(root_module, cls):
    ## uan-phy-gen.h: ns3::UanPhyPerGenDefault::UanPhyPerGenDefault(ns3::UanPhyPerGenDefault const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyPerGenDefault const &', 'arg0')])
    ## uan-phy-gen.h: ns3::UanPhyPerGenDefault::UanPhyPerGenDefault() [constructor]
    cls.add_constructor([])
    ## uan-phy-gen.h: double ns3::UanPhyPerGenDefault::CalcPer(ns3::Ptr<ns3::Packet> pkt, double sinrDb, ns3::UanTxMode mode) [member function]
    cls.add_method('CalcPer', 
                   'double', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('double', 'sinrDb'), param('ns3::UanTxMode', 'mode')], 
                   is_virtual=True)
    ## uan-phy-gen.h: static ns3::TypeId ns3::UanPhyPerGenDefault::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UanPhyPerUmodem_methods(root_module, cls):
    ## uan-phy-gen.h: ns3::UanPhyPerUmodem::UanPhyPerUmodem(ns3::UanPhyPerUmodem const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyPerUmodem const &', 'arg0')])
    ## uan-phy-gen.h: ns3::UanPhyPerUmodem::UanPhyPerUmodem() [constructor]
    cls.add_constructor([])
    ## uan-phy-gen.h: double ns3::UanPhyPerUmodem::CalcPer(ns3::Ptr<ns3::Packet> pkt, double sinrDb, ns3::UanTxMode mode) [member function]
    cls.add_method('CalcPer', 
                   'double', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('double', 'sinrDb'), param('ns3::UanTxMode', 'mode')], 
                   is_virtual=True)
    ## uan-phy-gen.h: static ns3::TypeId ns3::UanPhyPerUmodem::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UanPropModel_methods(root_module, cls):
    ## uan-prop-model.h: ns3::UanPropModel::UanPropModel() [constructor]
    cls.add_constructor([])
    ## uan-prop-model.h: ns3::UanPropModel::UanPropModel(ns3::UanPropModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPropModel const &', 'arg0')])
    ## uan-prop-model.h: void ns3::UanPropModel::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-prop-model.h: void ns3::UanPropModel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-prop-model.h: ns3::Time ns3::UanPropModel::GetDelay(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b, ns3::UanTxMode mode) [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b'), param('ns3::UanTxMode', 'mode')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-prop-model.h: double ns3::UanPropModel::GetPathLossDb(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b, ns3::UanTxMode txMode) [member function]
    cls.add_method('GetPathLossDb', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b'), param('ns3::UanTxMode', 'txMode')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-prop-model.h: ns3::UanPdp ns3::UanPropModel::GetPdp(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b, ns3::UanTxMode mode) [member function]
    cls.add_method('GetPdp', 
                   'ns3::UanPdp', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b'), param('ns3::UanTxMode', 'mode')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3UanPropModelIdeal_methods(root_module, cls):
    ## uan-prop-model-ideal.h: ns3::UanPropModelIdeal::UanPropModelIdeal(ns3::UanPropModelIdeal const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPropModelIdeal const &', 'arg0')])
    ## uan-prop-model-ideal.h: ns3::UanPropModelIdeal::UanPropModelIdeal() [constructor]
    cls.add_constructor([])
    ## uan-prop-model-ideal.h: ns3::Time ns3::UanPropModelIdeal::GetDelay(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b, ns3::UanTxMode mode) [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b'), param('ns3::UanTxMode', 'mode')], 
                   is_virtual=True)
    ## uan-prop-model-ideal.h: double ns3::UanPropModelIdeal::GetPathLossDb(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b, ns3::UanTxMode mode) [member function]
    cls.add_method('GetPathLossDb', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b'), param('ns3::UanTxMode', 'mode')], 
                   is_virtual=True)
    ## uan-prop-model-ideal.h: ns3::UanPdp ns3::UanPropModelIdeal::GetPdp(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b, ns3::UanTxMode mode) [member function]
    cls.add_method('GetPdp', 
                   'ns3::UanPdp', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b'), param('ns3::UanTxMode', 'mode')], 
                   is_virtual=True)
    ## uan-prop-model-ideal.h: static ns3::TypeId ns3::UanPropModelIdeal::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UanPropModelThorp_methods(root_module, cls):
    ## uan-prop-model-thorp.h: ns3::UanPropModelThorp::UanPropModelThorp(ns3::UanPropModelThorp const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPropModelThorp const &', 'arg0')])
    ## uan-prop-model-thorp.h: ns3::UanPropModelThorp::UanPropModelThorp() [constructor]
    cls.add_constructor([])
    ## uan-prop-model-thorp.h: ns3::Time ns3::UanPropModelThorp::GetDelay(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b, ns3::UanTxMode mode) [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b'), param('ns3::UanTxMode', 'mode')], 
                   is_virtual=True)
    ## uan-prop-model-thorp.h: double ns3::UanPropModelThorp::GetPathLossDb(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b, ns3::UanTxMode mode) [member function]
    cls.add_method('GetPathLossDb', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b'), param('ns3::UanTxMode', 'mode')], 
                   is_virtual=True)
    ## uan-prop-model-thorp.h: ns3::UanPdp ns3::UanPropModelThorp::GetPdp(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b, ns3::UanTxMode mode) [member function]
    cls.add_method('GetPdp', 
                   'ns3::UanPdp', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b'), param('ns3::UanTxMode', 'mode')], 
                   is_virtual=True)
    ## uan-prop-model-thorp.h: static ns3::TypeId ns3::UanPropModelThorp::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UanTransducer_methods(root_module, cls):
    ## uan-transducer.h: ns3::UanTransducer::UanTransducer() [constructor]
    cls.add_constructor([])
    ## uan-transducer.h: ns3::UanTransducer::UanTransducer(ns3::UanTransducer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanTransducer const &', 'arg0')])
    ## uan-transducer.h: void ns3::UanTransducer::AddPhy(ns3::Ptr<ns3::UanPhy> phy) [member function]
    cls.add_method('AddPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'phy')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-transducer.h: void ns3::UanTransducer::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-transducer.h: std::list<ns3::UanPacketArrival, std::allocator<ns3::UanPacketArrival> > const & ns3::UanTransducer::GetArrivalList() const [member function]
    cls.add_method('GetArrivalList', 
                   'std::list< ns3::UanPacketArrival > const &', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-transducer.h: ns3::Ptr<ns3::UanChannel> ns3::UanTransducer::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::UanChannel >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-transducer.h: std::list<ns3::Ptr<ns3::UanPhy>, std::allocator<ns3::Ptr<ns3::UanPhy> > > const & ns3::UanTransducer::GetPhyList() const [member function]
    cls.add_method('GetPhyList', 
                   'std::list< ns3::Ptr< ns3::UanPhy > > const &', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-transducer.h: ns3::UanTransducer::State ns3::UanTransducer::GetState() const [member function]
    cls.add_method('GetState', 
                   'ns3::UanTransducer::State', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-transducer.h: bool ns3::UanTransducer::IsRx() const [member function]
    cls.add_method('IsRx', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-transducer.h: bool ns3::UanTransducer::IsTx() const [member function]
    cls.add_method('IsTx', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-transducer.h: void ns3::UanTransducer::Receive(ns3::Ptr<ns3::Packet> packet, double rxPowerDb, ns3::UanTxMode txMode, ns3::UanPdp pdp) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'rxPowerDb'), param('ns3::UanTxMode', 'txMode'), param('ns3::UanPdp', 'pdp')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-transducer.h: void ns3::UanTransducer::SetChannel(ns3::Ptr<ns3::UanChannel> chan) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanChannel >', 'chan')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-transducer.h: void ns3::UanTransducer::Transmit(ns3::Ptr<ns3::UanPhy> src, ns3::Ptr<ns3::Packet> packet, double txPowerDb, ns3::UanTxMode txMode) [member function]
    cls.add_method('Transmit', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'src'), param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'txPowerDb'), param('ns3::UanTxMode', 'txMode')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3UanTransducerHd_methods(root_module, cls):
    ## uan-transducer-hd.h: ns3::UanTransducerHd::UanTransducerHd(ns3::UanTransducerHd const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanTransducerHd const &', 'arg0')])
    ## uan-transducer-hd.h: ns3::UanTransducerHd::UanTransducerHd() [constructor]
    cls.add_constructor([])
    ## uan-transducer-hd.h: void ns3::UanTransducerHd::AddPhy(ns3::Ptr<ns3::UanPhy> arg0) [member function]
    cls.add_method('AddPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'arg0')], 
                   is_virtual=True)
    ## uan-transducer-hd.h: void ns3::UanTransducerHd::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-transducer-hd.h: std::list<ns3::UanPacketArrival, std::allocator<ns3::UanPacketArrival> > const & ns3::UanTransducerHd::GetArrivalList() const [member function]
    cls.add_method('GetArrivalList', 
                   'std::list< ns3::UanPacketArrival > const &', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-transducer-hd.h: ns3::Ptr<ns3::UanChannel> ns3::UanTransducerHd::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::UanChannel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-transducer-hd.h: std::list<ns3::Ptr<ns3::UanPhy>, std::allocator<ns3::Ptr<ns3::UanPhy> > > const & ns3::UanTransducerHd::GetPhyList() const [member function]
    cls.add_method('GetPhyList', 
                   'std::list< ns3::Ptr< ns3::UanPhy > > const &', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-transducer-hd.h: ns3::UanTransducer::State ns3::UanTransducerHd::GetState() const [member function]
    cls.add_method('GetState', 
                   'ns3::UanTransducer::State', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-transducer-hd.h: static ns3::TypeId ns3::UanTransducerHd::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-transducer-hd.h: bool ns3::UanTransducerHd::IsRx() const [member function]
    cls.add_method('IsRx', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-transducer-hd.h: bool ns3::UanTransducerHd::IsTx() const [member function]
    cls.add_method('IsTx', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-transducer-hd.h: void ns3::UanTransducerHd::Receive(ns3::Ptr<ns3::Packet> packet, double rxPowerDb, ns3::UanTxMode txMode, ns3::UanPdp pdp) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'rxPowerDb'), param('ns3::UanTxMode', 'txMode'), param('ns3::UanPdp', 'pdp')], 
                   is_virtual=True)
    ## uan-transducer-hd.h: void ns3::UanTransducerHd::SetChannel(ns3::Ptr<ns3::UanChannel> chan) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanChannel >', 'chan')], 
                   is_virtual=True)
    ## uan-transducer-hd.h: void ns3::UanTransducerHd::Transmit(ns3::Ptr<ns3::UanPhy> src, ns3::Ptr<ns3::Packet> packet, double txPowerDb, ns3::UanTxMode txMode) [member function]
    cls.add_method('Transmit', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'src'), param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'txPowerDb'), param('ns3::UanTxMode', 'txMode')], 
                   is_virtual=True)
    ## uan-transducer-hd.h: void ns3::UanTransducerHd::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3UeManager_methods(root_module, cls):
    ## ue-manager.h: ns3::UeManager::UeManager(ns3::UeManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UeManager const &', 'arg0')])
    ## ue-manager.h: ns3::UeManager::UeManager() [constructor]
    cls.add_constructor([])
    ## ue-manager.h: void ns3::UeManager::CreateUeRecord(ns3::Ptr<ns3::UeNetDevice> ue, ns3::Ptr<ns3::EnbNetDevice> enb) [member function]
    cls.add_method('CreateUeRecord', 
                   'void', 
                   [param('ns3::Ptr< ns3::UeNetDevice >', 'ue'), param('ns3::Ptr< ns3::EnbNetDevice >', 'enb')])
    ## ue-manager.h: void ns3::UeManager::DeleteUeRecord(ns3::Ptr<ns3::UeNetDevice> ue) [member function]
    cls.add_method('DeleteUeRecord', 
                   'void', 
                   [param('ns3::Ptr< ns3::UeNetDevice >', 'ue')])
    ## ue-manager.h: void ns3::UeManager::DeleteUeRecord(ns3::Mac48Address const & macAddress) [member function]
    cls.add_method('DeleteUeRecord', 
                   'void', 
                   [param('ns3::Mac48Address const &', 'macAddress')])
    ## ue-manager.h: uint32_t ns3::UeManager::GetNRegisteredUes() const [member function]
    cls.add_method('GetNRegisteredUes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ue-manager.h: ns3::Ptr<ns3::UeRecord> ns3::UeManager::GetUeRecord(ns3::Ptr<ns3::UeNetDevice> ue) [member function]
    cls.add_method('GetUeRecord', 
                   'ns3::Ptr< ns3::UeRecord >', 
                   [param('ns3::Ptr< ns3::UeNetDevice >', 'ue')])
    ## ue-manager.h: ns3::Ptr<ns3::UeRecord> ns3::UeManager::GetUeRecord(ns3::Mac48Address const macAddress) [member function]
    cls.add_method('GetUeRecord', 
                   'ns3::Ptr< ns3::UeRecord >', 
                   [param('ns3::Mac48Address const', 'macAddress')])
    ## ue-manager.h: std::vector<ns3::Ptr<ns3::UeRecord>,std::allocator<ns3::Ptr<ns3::UeRecord> > > * ns3::UeManager::GetUeRecords() [member function]
    cls.add_method('GetUeRecords', 
                   'std::vector< ns3::Ptr< ns3::UeRecord > > *', 
                   [])
    ## ue-manager.h: bool ns3::UeManager::IsRegistered(ns3::Ptr<ns3::UeNetDevice> ue) const [member function]
    cls.add_method('IsRegistered', 
                   'bool', 
                   [param('ns3::Ptr< ns3::UeNetDevice >', 'ue')], 
                   is_const=True)
    ## ue-manager.h: bool ns3::UeManager::IsRegistered(ns3::Mac48Address const & macAddress) const [member function]
    cls.add_method('IsRegistered', 
                   'bool', 
                   [param('ns3::Mac48Address const &', 'macAddress')], 
                   is_const=True)
    return

def register_Ns3UeRecord_methods(root_module, cls):
    ## ue-record.h: ns3::UeRecord::UeRecord(ns3::UeRecord const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UeRecord const &', 'arg0')])
    ## ue-record.h: ns3::UeRecord::UeRecord() [constructor]
    cls.add_constructor([])
    ## ue-record.h: ns3::UeRecord::UeRecord(ns3::Ptr<ns3::NetDevice> ue, ns3::Ptr<ns3::NetDevice> enb) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::NetDevice >', 'ue'), param('ns3::Ptr< ns3::NetDevice >', 'enb')])
    ## ue-record.h: std::vector<ns3::UeRecord::CqiFeedback, std::allocator<ns3::UeRecord::CqiFeedback> > ns3::UeRecord::GetCqiFeedbacks() [member function]
    cls.add_method('GetCqiFeedbacks', 
                   'std::vector< ns3::UeRecord::CqiFeedback >', 
                   [])
    ## ue-record.h: ns3::Ptr<ns3::NetDevice> ns3::UeRecord::GetEnb() [member function]
    cls.add_method('GetEnb', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [])
    ## ue-record.h: ns3::Ptr<ns3::NetDevice> ns3::UeRecord::GetUe() [member function]
    cls.add_method('GetUe', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [])
    ## ue-record.h: void ns3::UeRecord::SetCqiFeedbacks(std::vector<ns3::UeRecord::CqiFeedback, std::allocator<ns3::UeRecord::CqiFeedback> > cqiFeedbacks) [member function]
    cls.add_method('SetCqiFeedbacks', 
                   'void', 
                   [param('std::vector< ns3::UeRecord::CqiFeedback >', 'cqiFeedbacks')])
    ## ue-record.h: void ns3::UeRecord::SetEnb(ns3::Ptr<ns3::NetDevice> enb) [member function]
    cls.add_method('SetEnb', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'enb')])
    ## ue-record.h: void ns3::UeRecord::SetUe(ns3::Ptr<ns3::NetDevice> ue) [member function]
    cls.add_method('SetUe', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'ue')])
    return

def register_Ns3UeRecordCqiFeedback_methods(root_module, cls):
    ## ue-record.h: ns3::UeRecord::CqiFeedback::CqiFeedback() [constructor]
    cls.add_constructor([])
    ## ue-record.h: ns3::UeRecord::CqiFeedback::CqiFeedback(ns3::UeRecord::CqiFeedback const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UeRecord::CqiFeedback const &', 'arg0')])
    ## ue-record.h: ns3::UeRecord::CqiFeedback::m_cqi [variable]
    cls.add_instance_attribute('m_cqi', 'int', is_const=False)
    ## ue-record.h: ns3::UeRecord::CqiFeedback::m_subChannelId [variable]
    cls.add_instance_attribute('m_subChannelId', 'int', is_const=False)
    return

def register_Ns3AmcModule_methods(root_module, cls):
    ## amc-module.h: ns3::AmcModule::AmcModule(ns3::AmcModule const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AmcModule const &', 'arg0')])
    ## amc-module.h: ns3::AmcModule::AmcModule() [constructor]
    cls.add_constructor([])
    ## amc-module.h: std::vector<int, std::allocator<int> > ns3::AmcModule::CreateCqiFeedbacks(std::vector<double, std::allocator<double> > sinr) [member function]
    cls.add_method('CreateCqiFeedbacks', 
                   'std::vector< int >', 
                   [param('std::vector< double >', 'sinr')])
    ## amc-module.h: int ns3::AmcModule::GetMcsFromCqi(int cqi) [member function]
    cls.add_method('GetMcsFromCqi', 
                   'int', 
                   [param('int', 'cqi')])
    ## amc-module.h: double ns3::AmcModule::GetSpectralEfficiencyFromCqi(int cqi) [member function]
    cls.add_method('GetSpectralEfficiencyFromCqi', 
                   'double', 
                   [param('int', 'cqi')])
    ## amc-module.h: int ns3::AmcModule::GetTbSizeFromMcs(int mcs) [member function]
    cls.add_method('GetTbSizeFromMcs', 
                   'int', 
                   [param('int', 'mcs')])
    ## amc-module.h: static ns3::TypeId ns3::AmcModule::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## amc-module.h: void ns3::AmcModule::Initialize() [member function]
    cls.add_method('Initialize', 
                   'void', 
                   [])
    return

def register_Ns3BearerQosParameters_methods(root_module, cls):
    ## bearer-qos-parameters.h: ns3::BearerQosParameters::BearerQosParameters(ns3::BearerQosParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BearerQosParameters const &', 'arg0')])
    ## bearer-qos-parameters.h: ns3::BearerQosParameters::BearerQosParameters() [constructor]
    cls.add_constructor([])
    ## bearer-qos-parameters.h: ns3::BearerQosParameters::BearerQosParameters(int qci, double gbr, double mbr) [constructor]
    cls.add_constructor([param('int', 'qci'), param('double', 'gbr'), param('double', 'mbr')])
    ## bearer-qos-parameters.h: ns3::BearerQosParameters::BearerQosParameters(int qci, bool apec, bool apev, double gbr, double mbr) [constructor]
    cls.add_constructor([param('int', 'qci'), param('bool', 'apec'), param('bool', 'apev'), param('double', 'gbr'), param('double', 'mbr')])
    ## bearer-qos-parameters.h: bool ns3::BearerQosParameters::GetArpPreEmptionCapability() const [member function]
    cls.add_method('GetArpPreEmptionCapability', 
                   'bool', 
                   [], 
                   is_const=True)
    ## bearer-qos-parameters.h: bool ns3::BearerQosParameters::GetArpPreEmptionVulnerability() const [member function]
    cls.add_method('GetArpPreEmptionVulnerability', 
                   'bool', 
                   [], 
                   is_const=True)
    ## bearer-qos-parameters.h: ns3::BearerQosParameters::BearerQosType ns3::BearerQosParameters::GetBearerQosType() const [member function]
    cls.add_method('GetBearerQosType', 
                   'ns3::BearerQosParameters::BearerQosType', 
                   [], 
                   is_const=True)
    ## bearer-qos-parameters.h: double ns3::BearerQosParameters::GetGbr() const [member function]
    cls.add_method('GetGbr', 
                   'double', 
                   [], 
                   is_const=True)
    ## bearer-qos-parameters.h: double ns3::BearerQosParameters::GetMaxDelay() const [member function]
    cls.add_method('GetMaxDelay', 
                   'double', 
                   [], 
                   is_const=True)
    ## bearer-qos-parameters.h: double ns3::BearerQosParameters::GetMbr() const [member function]
    cls.add_method('GetMbr', 
                   'double', 
                   [], 
                   is_const=True)
    ## bearer-qos-parameters.h: int ns3::BearerQosParameters::GetQci() const [member function]
    cls.add_method('GetQci', 
                   'int', 
                   [], 
                   is_const=True)
    ## bearer-qos-parameters.h: void ns3::BearerQosParameters::SetArpPreEmptionCapability(bool apec) [member function]
    cls.add_method('SetArpPreEmptionCapability', 
                   'void', 
                   [param('bool', 'apec')])
    ## bearer-qos-parameters.h: void ns3::BearerQosParameters::SetArpPreEmptionVulnerability(bool apev) [member function]
    cls.add_method('SetArpPreEmptionVulnerability', 
                   'void', 
                   [param('bool', 'apev')])
    ## bearer-qos-parameters.h: void ns3::BearerQosParameters::SetBearerQosType(ns3::BearerQosParameters::BearerQosType QosType) [member function]
    cls.add_method('SetBearerQosType', 
                   'void', 
                   [param('ns3::BearerQosParameters::BearerQosType', 'QosType')])
    ## bearer-qos-parameters.h: void ns3::BearerQosParameters::SetGbr(double gbr) [member function]
    cls.add_method('SetGbr', 
                   'void', 
                   [param('double', 'gbr')])
    ## bearer-qos-parameters.h: void ns3::BearerQosParameters::SetMaxDelay(double targetDelay) [member function]
    cls.add_method('SetMaxDelay', 
                   'void', 
                   [param('double', 'targetDelay')])
    ## bearer-qos-parameters.h: void ns3::BearerQosParameters::SetMbr(double mbr) [member function]
    cls.add_method('SetMbr', 
                   'void', 
                   [param('double', 'mbr')])
    ## bearer-qos-parameters.h: void ns3::BearerQosParameters::SetQci(int qci) [member function]
    cls.add_method('SetQci', 
                   'void', 
                   [param('int', 'qci')])
    return

def register_Ns3ChannelRealization_methods(root_module, cls):
    ## channel-realization.h: ns3::ChannelRealization::ChannelRealization(ns3::ChannelRealization const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ChannelRealization const &', 'arg0')])
    ## channel-realization.h: ns3::ChannelRealization::ChannelRealization() [constructor]
    cls.add_constructor([])
    ## channel-realization.h: ns3::Ptr<ns3::JakesFadingLossModel> ns3::ChannelRealization::GetJakesFadingLossModel() [member function]
    cls.add_method('GetJakesFadingLossModel', 
                   'ns3::Ptr< ns3::JakesFadingLossModel >', 
                   [])
    ## channel-realization.h: ns3::Ptr<ns3::PathLossModel> ns3::ChannelRealization::GetPathLossModel() [member function]
    cls.add_method('GetPathLossModel', 
                   'ns3::Ptr< ns3::PathLossModel >', 
                   [])
    ## channel-realization.h: ns3::Ptr<ns3::PenetrationLossModel> ns3::ChannelRealization::GetPenetrationLossModel() [member function]
    cls.add_method('GetPenetrationLossModel', 
                   'ns3::Ptr< ns3::PenetrationLossModel >', 
                   [])
    ## channel-realization.h: ns3::Ptr<ns3::ShadowingLossModel> ns3::ChannelRealization::GetShadowingLossModel() [member function]
    cls.add_method('GetShadowingLossModel', 
                   'ns3::Ptr< ns3::ShadowingLossModel >', 
                   [])
    ## channel-realization.h: static ns3::TypeId ns3::ChannelRealization::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## channel-realization.h: void ns3::ChannelRealization::SetJakesFadingLossModel(ns3::Ptr<ns3::JakesFadingLossModel> l) [member function]
    cls.add_method('SetJakesFadingLossModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::JakesFadingLossModel >', 'l')])
    ## channel-realization.h: void ns3::ChannelRealization::SetPathLossModel(ns3::Ptr<ns3::PathLossModel> l) [member function]
    cls.add_method('SetPathLossModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::PathLossModel >', 'l')])
    ## channel-realization.h: void ns3::ChannelRealization::SetPenetrationLossModel(ns3::Ptr<ns3::PenetrationLossModel> l) [member function]
    cls.add_method('SetPenetrationLossModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::PenetrationLossModel >', 'l')])
    ## channel-realization.h: void ns3::ChannelRealization::SetShadowingLossModel(ns3::Ptr<ns3::ShadowingLossModel> l) [member function]
    cls.add_method('SetShadowingLossModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::ShadowingLossModel >', 'l')])
    return

def register_Ns3DeviceEnergyModel_methods(root_module, cls):
    ## device-energy-model.h: ns3::DeviceEnergyModel::DeviceEnergyModel(ns3::DeviceEnergyModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DeviceEnergyModel const &', 'arg0')])
    ## device-energy-model.h: ns3::DeviceEnergyModel::DeviceEnergyModel() [constructor]
    cls.add_constructor([])
    ## device-energy-model.h: void ns3::DeviceEnergyModel::ChangeState(int newState) [member function]
    cls.add_method('ChangeState', 
                   'void', 
                   [param('int', 'newState')], 
                   is_pure_virtual=True, is_virtual=True)
    ## device-energy-model.h: double ns3::DeviceEnergyModel::GetCurrentA() const [member function]
    cls.add_method('GetCurrentA', 
                   'double', 
                   [], 
                   is_const=True)
    ## device-energy-model.h: double ns3::DeviceEnergyModel::GetTotalEnergyConsumption() const [member function]
    cls.add_method('GetTotalEnergyConsumption', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## device-energy-model.h: static ns3::TypeId ns3::DeviceEnergyModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## device-energy-model.h: void ns3::DeviceEnergyModel::HandleEnergyDepletion() [member function]
    cls.add_method('HandleEnergyDepletion', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## device-energy-model.h: void ns3::DeviceEnergyModel::SetEnergySource(ns3::Ptr<ns3::EnergySource> source) [member function]
    cls.add_method('SetEnergySource', 
                   'void', 
                   [param('ns3::Ptr< ns3::EnergySource >', 'source')], 
                   is_pure_virtual=True, is_virtual=True)
    ## device-energy-model.h: double ns3::DeviceEnergyModel::DoGetCurrentA() const [member function]
    cls.add_method('DoGetCurrentA', 
                   'double', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3DiscreteTimeLossModel_methods(root_module, cls):
    ## discrete-time-loss-model.h: ns3::DiscreteTimeLossModel::DiscreteTimeLossModel(ns3::DiscreteTimeLossModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DiscreteTimeLossModel const &', 'arg0')])
    ## discrete-time-loss-model.h: ns3::DiscreteTimeLossModel::DiscreteTimeLossModel() [constructor]
    cls.add_constructor([])
    ## discrete-time-loss-model.h: ns3::Time ns3::DiscreteTimeLossModel::GetLastUpdate() [member function]
    cls.add_method('GetLastUpdate', 
                   'ns3::Time', 
                   [])
    ## discrete-time-loss-model.h: double ns3::DiscreteTimeLossModel::GetSamplingPeriod() [member function]
    cls.add_method('GetSamplingPeriod', 
                   'double', 
                   [])
    ## discrete-time-loss-model.h: static ns3::TypeId ns3::DiscreteTimeLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## discrete-time-loss-model.h: bool ns3::DiscreteTimeLossModel::NeedForUpdate() [member function]
    cls.add_method('NeedForUpdate', 
                   'bool', 
                   [])
    ## discrete-time-loss-model.h: void ns3::DiscreteTimeLossModel::SetLastUpdate() [member function]
    cls.add_method('SetLastUpdate', 
                   'void', 
                   [])
    ## discrete-time-loss-model.h: void ns3::DiscreteTimeLossModel::SetSamplingPeriod(double sp) [member function]
    cls.add_method('SetSamplingPeriod', 
                   'void', 
                   [param('double', 'sp')])
    return

def register_Ns3EnergySource_methods(root_module, cls):
    ## energy-source.h: ns3::EnergySource::EnergySource(ns3::EnergySource const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EnergySource const &', 'arg0')])
    ## energy-source.h: ns3::EnergySource::EnergySource() [constructor]
    cls.add_constructor([])
    ## energy-source.h: void ns3::EnergySource::AppendDeviceEnergyModel(ns3::Ptr<ns3::DeviceEnergyModel> deviceEnergyModelPtr) [member function]
    cls.add_method('AppendDeviceEnergyModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::DeviceEnergyModel >', 'deviceEnergyModelPtr')])
    ## energy-source.h: void ns3::EnergySource::DisposeDeviceModels() [member function]
    cls.add_method('DisposeDeviceModels', 
                   'void', 
                   [])
    ## energy-source.h: ns3::DeviceEnergyModelContainer ns3::EnergySource::FindDeviceEnergyModels(ns3::TypeId tid) [member function]
    cls.add_method('FindDeviceEnergyModels', 
                   'ns3::DeviceEnergyModelContainer', 
                   [param('ns3::TypeId', 'tid')])
    ## energy-source.h: ns3::DeviceEnergyModelContainer ns3::EnergySource::FindDeviceEnergyModels(std::string name) [member function]
    cls.add_method('FindDeviceEnergyModels', 
                   'ns3::DeviceEnergyModelContainer', 
                   [param('std::string', 'name')])
    ## energy-source.h: double ns3::EnergySource::GetEnergyFraction() [member function]
    cls.add_method('GetEnergyFraction', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## energy-source.h: double ns3::EnergySource::GetInitialEnergy() const [member function]
    cls.add_method('GetInitialEnergy', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## energy-source.h: ns3::Ptr<ns3::Node> ns3::EnergySource::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True)
    ## energy-source.h: double ns3::EnergySource::GetRemainingEnergy() [member function]
    cls.add_method('GetRemainingEnergy', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## energy-source.h: double ns3::EnergySource::GetSupplyVoltage() const [member function]
    cls.add_method('GetSupplyVoltage', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## energy-source.h: static ns3::TypeId ns3::EnergySource::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## energy-source.h: void ns3::EnergySource::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## energy-source.h: void ns3::EnergySource::StartDeviceModels() [member function]
    cls.add_method('StartDeviceModels', 
                   'void', 
                   [])
    ## energy-source.h: void ns3::EnergySource::UpdateEnergySource() [member function]
    cls.add_method('UpdateEnergySource', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## energy-source.h: void ns3::EnergySource::BreakDeviceEnergyModelRefCycle() [member function]
    cls.add_method('BreakDeviceEnergyModelRefCycle', 
                   'void', 
                   [], 
                   visibility='protected')
    ## energy-source.h: double ns3::EnergySource::CalculateTotalCurrent() [member function]
    cls.add_method('CalculateTotalCurrent', 
                   'double', 
                   [], 
                   visibility='protected')
    ## energy-source.h: void ns3::EnergySource::NotifyEnergyDrained() [member function]
    cls.add_method('NotifyEnergyDrained', 
                   'void', 
                   [], 
                   visibility='protected')
    ## energy-source.h: void ns3::EnergySource::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3EnergySourceContainer_methods(root_module, cls):
    ## energy-source-container.h: ns3::EnergySourceContainer::EnergySourceContainer(ns3::EnergySourceContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EnergySourceContainer const &', 'arg0')])
    ## energy-source-container.h: ns3::EnergySourceContainer::EnergySourceContainer() [constructor]
    cls.add_constructor([])
    ## energy-source-container.h: ns3::EnergySourceContainer::EnergySourceContainer(ns3::Ptr<ns3::EnergySource> source) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::EnergySource >', 'source')])
    ## energy-source-container.h: ns3::EnergySourceContainer::EnergySourceContainer(std::string sourceName) [constructor]
    cls.add_constructor([param('std::string', 'sourceName')])
    ## energy-source-container.h: ns3::EnergySourceContainer::EnergySourceContainer(ns3::EnergySourceContainer const & a, ns3::EnergySourceContainer const & b) [constructor]
    cls.add_constructor([param('ns3::EnergySourceContainer const &', 'a'), param('ns3::EnergySourceContainer const &', 'b')])
    ## energy-source-container.h: void ns3::EnergySourceContainer::Add(ns3::EnergySourceContainer container) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::EnergySourceContainer', 'container')])
    ## energy-source-container.h: void ns3::EnergySourceContainer::Add(ns3::Ptr<ns3::EnergySource> source) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::EnergySource >', 'source')])
    ## energy-source-container.h: void ns3::EnergySourceContainer::Add(std::string sourceName) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('std::string', 'sourceName')])
    ## energy-source-container.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::EnergySource>*,std::vector<ns3::Ptr<ns3::EnergySource>, std::allocator<ns3::Ptr<ns3::EnergySource> > > > ns3::EnergySourceContainer::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::EnergySource > const, std::vector< ns3::Ptr< ns3::EnergySource > > >', 
                   [], 
                   is_const=True)
    ## energy-source-container.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::EnergySource>*,std::vector<ns3::Ptr<ns3::EnergySource>, std::allocator<ns3::Ptr<ns3::EnergySource> > > > ns3::EnergySourceContainer::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::EnergySource > const, std::vector< ns3::Ptr< ns3::EnergySource > > >', 
                   [], 
                   is_const=True)
    ## energy-source-container.h: ns3::Ptr<ns3::EnergySource> ns3::EnergySourceContainer::Get(uint32_t i) const [member function]
    cls.add_method('Get', 
                   'ns3::Ptr< ns3::EnergySource >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## energy-source-container.h: uint32_t ns3::EnergySourceContainer::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## energy-source-container.h: static ns3::TypeId ns3::EnergySourceContainer::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## energy-source-container.h: void ns3::EnergySourceContainer::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## energy-source-container.h: void ns3::EnergySourceContainer::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3FlowClassifier_methods(root_module, cls):
    ## flow-classifier.h: ns3::FlowClassifier::FlowClassifier() [constructor]
    cls.add_constructor([])
    ## flow-classifier.h: void ns3::FlowClassifier::SerializeToXmlStream(std::ostream & os, int indent) const [member function]
    cls.add_method('SerializeToXmlStream', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'indent')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## flow-classifier.h: ns3::FlowId ns3::FlowClassifier::GetNewFlowId() [member function]
    cls.add_method('GetNewFlowId', 
                   'ns3::FlowId', 
                   [], 
                   visibility='protected')
    return

def register_Ns3FlowMonitor_methods(root_module, cls):
    ## flow-monitor.h: ns3::FlowMonitor::FlowMonitor(ns3::FlowMonitor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FlowMonitor const &', 'arg0')])
    ## flow-monitor.h: ns3::FlowMonitor::FlowMonitor() [constructor]
    cls.add_constructor([])
    ## flow-monitor.h: void ns3::FlowMonitor::AddProbe(ns3::Ptr<ns3::FlowProbe> probe) [member function]
    cls.add_method('AddProbe', 
                   'void', 
                   [param('ns3::Ptr< ns3::FlowProbe >', 'probe')])
    ## flow-monitor.h: void ns3::FlowMonitor::CheckForLostPackets() [member function]
    cls.add_method('CheckForLostPackets', 
                   'void', 
                   [])
    ## flow-monitor.h: void ns3::FlowMonitor::CheckForLostPackets(ns3::Time maxDelay) [member function]
    cls.add_method('CheckForLostPackets', 
                   'void', 
                   [param('ns3::Time', 'maxDelay')])
    ## flow-monitor.h: std::vector<ns3::Ptr<ns3::FlowProbe>, std::allocator<ns3::Ptr<ns3::FlowProbe> > > ns3::FlowMonitor::GetAllProbes() const [member function]
    cls.add_method('GetAllProbes', 
                   'std::vector< ns3::Ptr< ns3::FlowProbe > >', 
                   [], 
                   is_const=True)
    ## flow-monitor.h: std::map<unsigned int, ns3::FlowMonitor::FlowStats, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, ns3::FlowMonitor::FlowStats> > > ns3::FlowMonitor::GetFlowStats() const [member function]
    cls.add_method('GetFlowStats', 
                   'std::map< unsigned int, ns3::FlowMonitor::FlowStats >', 
                   [], 
                   is_const=True)
    ## flow-monitor.h: ns3::TypeId ns3::FlowMonitor::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## flow-monitor.h: static ns3::TypeId ns3::FlowMonitor::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## flow-monitor.h: void ns3::FlowMonitor::ReportDrop(ns3::Ptr<ns3::FlowProbe> probe, ns3::FlowId flowId, ns3::FlowPacketId packetId, uint32_t packetSize, uint32_t reasonCode) [member function]
    cls.add_method('ReportDrop', 
                   'void', 
                   [param('ns3::Ptr< ns3::FlowProbe >', 'probe'), param('ns3::FlowId', 'flowId'), param('ns3::FlowPacketId', 'packetId'), param('uint32_t', 'packetSize'), param('uint32_t', 'reasonCode')])
    ## flow-monitor.h: void ns3::FlowMonitor::ReportFirstTx(ns3::Ptr<ns3::FlowProbe> probe, ns3::FlowId flowId, ns3::FlowPacketId packetId, uint32_t packetSize) [member function]
    cls.add_method('ReportFirstTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::FlowProbe >', 'probe'), param('ns3::FlowId', 'flowId'), param('ns3::FlowPacketId', 'packetId'), param('uint32_t', 'packetSize')])
    ## flow-monitor.h: void ns3::FlowMonitor::ReportForwarding(ns3::Ptr<ns3::FlowProbe> probe, ns3::FlowId flowId, ns3::FlowPacketId packetId, uint32_t packetSize) [member function]
    cls.add_method('ReportForwarding', 
                   'void', 
                   [param('ns3::Ptr< ns3::FlowProbe >', 'probe'), param('ns3::FlowId', 'flowId'), param('ns3::FlowPacketId', 'packetId'), param('uint32_t', 'packetSize')])
    ## flow-monitor.h: void ns3::FlowMonitor::ReportLastRx(ns3::Ptr<ns3::FlowProbe> probe, ns3::FlowId flowId, ns3::FlowPacketId packetId, uint32_t packetSize) [member function]
    cls.add_method('ReportLastRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::FlowProbe >', 'probe'), param('ns3::FlowId', 'flowId'), param('ns3::FlowPacketId', 'packetId'), param('uint32_t', 'packetSize')])
    ## flow-monitor.h: void ns3::FlowMonitor::SerializeToXmlFile(std::string fileName, bool enableHistograms, bool enableProbes) [member function]
    cls.add_method('SerializeToXmlFile', 
                   'void', 
                   [param('std::string', 'fileName'), param('bool', 'enableHistograms'), param('bool', 'enableProbes')])
    ## flow-monitor.h: void ns3::FlowMonitor::SerializeToXmlStream(std::ostream & os, int indent, bool enableHistograms, bool enableProbes) [member function]
    cls.add_method('SerializeToXmlStream', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'indent'), param('bool', 'enableHistograms'), param('bool', 'enableProbes')])
    ## flow-monitor.h: std::string ns3::FlowMonitor::SerializeToXmlString(int indent, bool enableHistograms, bool enableProbes) [member function]
    cls.add_method('SerializeToXmlString', 
                   'std::string', 
                   [param('int', 'indent'), param('bool', 'enableHistograms'), param('bool', 'enableProbes')])
    ## flow-monitor.h: void ns3::FlowMonitor::SetFlowClassifier(ns3::Ptr<ns3::FlowClassifier> classifier) [member function]
    cls.add_method('SetFlowClassifier', 
                   'void', 
                   [param('ns3::Ptr< ns3::FlowClassifier >', 'classifier')])
    ## flow-monitor.h: void ns3::FlowMonitor::Start(ns3::Time const & time) [member function]
    cls.add_method('Start', 
                   'void', 
                   [param('ns3::Time const &', 'time')])
    ## flow-monitor.h: void ns3::FlowMonitor::StartRightNow() [member function]
    cls.add_method('StartRightNow', 
                   'void', 
                   [])
    ## flow-monitor.h: void ns3::FlowMonitor::Stop(ns3::Time const & time) [member function]
    cls.add_method('Stop', 
                   'void', 
                   [param('ns3::Time const &', 'time')])
    ## flow-monitor.h: void ns3::FlowMonitor::StopRightNow() [member function]
    cls.add_method('StopRightNow', 
                   'void', 
                   [])
    ## flow-monitor.h: void ns3::FlowMonitor::NotifyConstructionCompleted() [member function]
    cls.add_method('NotifyConstructionCompleted', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3FlowMonitorFlowStats_methods(root_module, cls):
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::FlowStats() [constructor]
    cls.add_constructor([])
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::FlowStats(ns3::FlowMonitor::FlowStats const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FlowMonitor::FlowStats const &', 'arg0')])
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::bytesDropped [variable]
    cls.add_instance_attribute('bytesDropped', 'std::vector< unsigned long >', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::delayHistogram [variable]
    cls.add_instance_attribute('delayHistogram', 'ns3::Histogram', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::delaySum [variable]
    cls.add_instance_attribute('delaySum', 'ns3::Time', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::flowInterruptionsHistogram [variable]
    cls.add_instance_attribute('flowInterruptionsHistogram', 'ns3::Histogram', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::jitterHistogram [variable]
    cls.add_instance_attribute('jitterHistogram', 'ns3::Histogram', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::jitterSum [variable]
    cls.add_instance_attribute('jitterSum', 'ns3::Time', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::lastDelay [variable]
    cls.add_instance_attribute('lastDelay', 'ns3::Time', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::lostPackets [variable]
    cls.add_instance_attribute('lostPackets', 'uint32_t', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::packetSizeHistogram [variable]
    cls.add_instance_attribute('packetSizeHistogram', 'ns3::Histogram', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::packetsDropped [variable]
    cls.add_instance_attribute('packetsDropped', 'std::vector< unsigned int >', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::rxBytes [variable]
    cls.add_instance_attribute('rxBytes', 'uint64_t', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::rxPackets [variable]
    cls.add_instance_attribute('rxPackets', 'uint32_t', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::timeFirstRxPacket [variable]
    cls.add_instance_attribute('timeFirstRxPacket', 'ns3::Time', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::timeFirstTxPacket [variable]
    cls.add_instance_attribute('timeFirstTxPacket', 'ns3::Time', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::timeLastRxPacket [variable]
    cls.add_instance_attribute('timeLastRxPacket', 'ns3::Time', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::timeLastTxPacket [variable]
    cls.add_instance_attribute('timeLastTxPacket', 'ns3::Time', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::timesForwarded [variable]
    cls.add_instance_attribute('timesForwarded', 'uint32_t', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::txBytes [variable]
    cls.add_instance_attribute('txBytes', 'uint64_t', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::txPackets [variable]
    cls.add_instance_attribute('txPackets', 'uint32_t', is_const=False)
    return

def register_Ns3FlowProbe_methods(root_module, cls):
    ## flow-probe.h: void ns3::FlowProbe::AddPacketDropStats(ns3::FlowId flowId, uint32_t packetSize, uint32_t reasonCode) [member function]
    cls.add_method('AddPacketDropStats', 
                   'void', 
                   [param('ns3::FlowId', 'flowId'), param('uint32_t', 'packetSize'), param('uint32_t', 'reasonCode')])
    ## flow-probe.h: void ns3::FlowProbe::AddPacketStats(ns3::FlowId flowId, uint32_t packetSize, ns3::Time delayFromFirstProbe) [member function]
    cls.add_method('AddPacketStats', 
                   'void', 
                   [param('ns3::FlowId', 'flowId'), param('uint32_t', 'packetSize'), param('ns3::Time', 'delayFromFirstProbe')])
    ## flow-probe.h: std::map<unsigned int, ns3::FlowProbe::FlowStats, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, ns3::FlowProbe::FlowStats> > > ns3::FlowProbe::GetStats() const [member function]
    cls.add_method('GetStats', 
                   'std::map< unsigned int, ns3::FlowProbe::FlowStats >', 
                   [], 
                   is_const=True)
    ## flow-probe.h: void ns3::FlowProbe::SerializeToXmlStream(std::ostream & os, int indent, uint32_t index) const [member function]
    cls.add_method('SerializeToXmlStream', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'indent'), param('uint32_t', 'index')], 
                   is_const=True)
    ## flow-probe.h: ns3::FlowProbe::FlowProbe(ns3::Ptr<ns3::FlowMonitor> flowMonitor) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::FlowMonitor >', 'flowMonitor')], 
                        visibility='protected')
    return

def register_Ns3FlowProbeFlowStats_methods(root_module, cls):
    ## flow-probe.h: ns3::FlowProbe::FlowStats::FlowStats(ns3::FlowProbe::FlowStats const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FlowProbe::FlowStats const &', 'arg0')])
    ## flow-probe.h: ns3::FlowProbe::FlowStats::FlowStats() [constructor]
    cls.add_constructor([])
    ## flow-probe.h: ns3::FlowProbe::FlowStats::bytes [variable]
    cls.add_instance_attribute('bytes', 'uint64_t', is_const=False)
    ## flow-probe.h: ns3::FlowProbe::FlowStats::bytesDropped [variable]
    cls.add_instance_attribute('bytesDropped', 'std::vector< unsigned long >', is_const=False)
    ## flow-probe.h: ns3::FlowProbe::FlowStats::delayFromFirstProbeSum [variable]
    cls.add_instance_attribute('delayFromFirstProbeSum', 'ns3::Time', is_const=False)
    ## flow-probe.h: ns3::FlowProbe::FlowStats::packets [variable]
    cls.add_instance_attribute('packets', 'uint32_t', is_const=False)
    ## flow-probe.h: ns3::FlowProbe::FlowStats::packetsDropped [variable]
    cls.add_instance_attribute('packetsDropped', 'std::vector< unsigned int >', is_const=False)
    return

def register_Ns3GlobalRouter_methods(root_module, cls):
    ## global-router-interface.h: static ns3::TypeId ns3::GlobalRouter::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## global-router-interface.h: ns3::GlobalRouter::GlobalRouter() [constructor]
    cls.add_constructor([])
    ## global-router-interface.h: void ns3::GlobalRouter::SetRoutingProtocol(ns3::Ptr<ns3::Ipv4GlobalRouting> routing) [member function]
    cls.add_method('SetRoutingProtocol', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4GlobalRouting >', 'routing')])
    ## global-router-interface.h: ns3::Ptr<ns3::Ipv4GlobalRouting> ns3::GlobalRouter::GetRoutingProtocol() [member function]
    cls.add_method('GetRoutingProtocol', 
                   'ns3::Ptr< ns3::Ipv4GlobalRouting >', 
                   [])
    ## global-router-interface.h: ns3::Ipv4Address ns3::GlobalRouter::GetRouterId() const [member function]
    cls.add_method('GetRouterId', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## global-router-interface.h: uint32_t ns3::GlobalRouter::DiscoverLSAs() [member function]
    cls.add_method('DiscoverLSAs', 
                   'uint32_t', 
                   [])
    ## global-router-interface.h: uint32_t ns3::GlobalRouter::GetNumLSAs() const [member function]
    cls.add_method('GetNumLSAs', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## global-router-interface.h: bool ns3::GlobalRouter::GetLSA(uint32_t n, ns3::GlobalRoutingLSA & lsa) const [member function]
    cls.add_method('GetLSA', 
                   'bool', 
                   [param('uint32_t', 'n'), param('ns3::GlobalRoutingLSA &', 'lsa')], 
                   is_const=True)
    ## global-router-interface.h: void ns3::GlobalRouter::InjectRoute(ns3::Ipv4Address network, ns3::Ipv4Mask networkMask) [member function]
    cls.add_method('InjectRoute', 
                   'void', 
                   [param('ns3::Ipv4Address', 'network'), param('ns3::Ipv4Mask', 'networkMask')])
    ## global-router-interface.h: uint32_t ns3::GlobalRouter::GetNInjectedRoutes() [member function]
    cls.add_method('GetNInjectedRoutes', 
                   'uint32_t', 
                   [])
    ## global-router-interface.h: ns3::Ipv4RoutingTableEntry * ns3::GlobalRouter::GetInjectedRoute(uint32_t i) [member function]
    cls.add_method('GetInjectedRoute', 
                   retval('ns3::Ipv4RoutingTableEntry *', caller_owns_return=False), 
                   [param('uint32_t', 'i')])
    ## global-router-interface.h: void ns3::GlobalRouter::RemoveInjectedRoute(uint32_t i) [member function]
    cls.add_method('RemoveInjectedRoute', 
                   'void', 
                   [param('uint32_t', 'i')])
    ## global-router-interface.h: bool ns3::GlobalRouter::WithdrawRoute(ns3::Ipv4Address network, ns3::Ipv4Mask networkMask) [member function]
    cls.add_method('WithdrawRoute', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'network'), param('ns3::Ipv4Mask', 'networkMask')])
    ## global-router-interface.h: void ns3::GlobalRouter::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3IdealControlMessage_methods(root_module, cls):
    ## ideal-control-messages.h: ns3::IdealControlMessage::IdealControlMessage(ns3::IdealControlMessage const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IdealControlMessage const &', 'arg0')])
    ## ideal-control-messages.h: ns3::IdealControlMessage::IdealControlMessage() [constructor]
    cls.add_constructor([])
    ## ideal-control-messages.h: ns3::Ptr<ns3::LteNetDevice> ns3::IdealControlMessage::GetDestinationDevice() [member function]
    cls.add_method('GetDestinationDevice', 
                   'ns3::Ptr< ns3::LteNetDevice >', 
                   [])
    ## ideal-control-messages.h: ns3::IdealControlMessage::MessageType ns3::IdealControlMessage::GetMessageType() [member function]
    cls.add_method('GetMessageType', 
                   'ns3::IdealControlMessage::MessageType', 
                   [])
    ## ideal-control-messages.h: ns3::Ptr<ns3::LteNetDevice> ns3::IdealControlMessage::GetSourceDevice() [member function]
    cls.add_method('GetSourceDevice', 
                   'ns3::Ptr< ns3::LteNetDevice >', 
                   [])
    ## ideal-control-messages.h: void ns3::IdealControlMessage::SetDestinationDevice(ns3::Ptr<ns3::LteNetDevice> dst) [member function]
    cls.add_method('SetDestinationDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::LteNetDevice >', 'dst')])
    ## ideal-control-messages.h: void ns3::IdealControlMessage::SetMessageType(ns3::IdealControlMessage::MessageType type) [member function]
    cls.add_method('SetMessageType', 
                   'void', 
                   [param('ns3::IdealControlMessage::MessageType', 'type')])
    ## ideal-control-messages.h: void ns3::IdealControlMessage::SetSourceDevice(ns3::Ptr<ns3::LteNetDevice> src) [member function]
    cls.add_method('SetSourceDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::LteNetDevice >', 'src')])
    return

def register_Ns3Ipv4FlowClassifier_methods(root_module, cls):
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::Ipv4FlowClassifier() [constructor]
    cls.add_constructor([])
    ## ipv4-flow-classifier.h: bool ns3::Ipv4FlowClassifier::Classify(ns3::Ipv4Header const & ipHeader, ns3::Ptr<ns3::Packet const> ipPayload, uint32_t * out_flowId, uint32_t * out_packetId) [member function]
    cls.add_method('Classify', 
                   'bool', 
                   [param('ns3::Ipv4Header const &', 'ipHeader'), param('ns3::Ptr< ns3::Packet const >', 'ipPayload'), param('uint32_t *', 'out_flowId'), param('uint32_t *', 'out_packetId')])
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::FiveTuple ns3::Ipv4FlowClassifier::FindFlow(ns3::FlowId flowId) const [member function]
    cls.add_method('FindFlow', 
                   'ns3::Ipv4FlowClassifier::FiveTuple', 
                   [param('ns3::FlowId', 'flowId')], 
                   is_const=True)
    ## ipv4-flow-classifier.h: void ns3::Ipv4FlowClassifier::SerializeToXmlStream(std::ostream & os, int indent) const [member function]
    cls.add_method('SerializeToXmlStream', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'indent')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Ipv4FlowClassifierFiveTuple_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::FiveTuple::FiveTuple() [constructor]
    cls.add_constructor([])
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::FiveTuple::FiveTuple(ns3::Ipv4FlowClassifier::FiveTuple const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4FlowClassifier::FiveTuple const &', 'arg0')])
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::FiveTuple::destinationAddress [variable]
    cls.add_instance_attribute('destinationAddress', 'ns3::Ipv4Address', is_const=False)
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::FiveTuple::destinationPort [variable]
    cls.add_instance_attribute('destinationPort', 'uint16_t', is_const=False)
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::FiveTuple::protocol [variable]
    cls.add_instance_attribute('protocol', 'uint8_t', is_const=False)
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::FiveTuple::sourceAddress [variable]
    cls.add_instance_attribute('sourceAddress', 'ns3::Ipv4Address', is_const=False)
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::FiveTuple::sourcePort [variable]
    cls.add_instance_attribute('sourcePort', 'uint16_t', is_const=False)
    return

def register_Ns3Ipv4FlowProbe_methods(root_module, cls):
    ## ipv4-flow-probe.h: ns3::Ipv4FlowProbe::Ipv4FlowProbe(ns3::Ptr<ns3::FlowMonitor> monitor, ns3::Ptr<ns3::Ipv4FlowClassifier> classifier, ns3::Ptr<ns3::Node> node) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::FlowMonitor >', 'monitor'), param('ns3::Ptr< ns3::Ipv4FlowClassifier >', 'classifier'), param('ns3::Ptr< ns3::Node >', 'node')])
    return

def register_Ns3Ipv4StaticRouting_methods(root_module, cls):
    ## ipv4-static-routing.h: ns3::Ipv4StaticRouting::Ipv4StaticRouting(ns3::Ipv4StaticRouting const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4StaticRouting const &', 'arg0')])
    ## ipv4-static-routing.h: ns3::Ipv4StaticRouting::Ipv4StaticRouting() [constructor]
    cls.add_constructor([])
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::AddHostRouteTo(ns3::Ipv4Address dest, ns3::Ipv4Address nextHop, uint32_t interface, uint32_t metric=0) [member function]
    cls.add_method('AddHostRouteTo', 
                   'void', 
                   [param('ns3::Ipv4Address', 'dest'), param('ns3::Ipv4Address', 'nextHop'), param('uint32_t', 'interface'), param('uint32_t', 'metric', default_value='0')])
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::AddHostRouteTo(ns3::Ipv4Address dest, uint32_t interface, uint32_t metric=0) [member function]
    cls.add_method('AddHostRouteTo', 
                   'void', 
                   [param('ns3::Ipv4Address', 'dest'), param('uint32_t', 'interface'), param('uint32_t', 'metric', default_value='0')])
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::AddMulticastRoute(ns3::Ipv4Address origin, ns3::Ipv4Address group, uint32_t inputInterface, std::vector<unsigned int, std::allocator<unsigned int> > outputInterfaces) [member function]
    cls.add_method('AddMulticastRoute', 
                   'void', 
                   [param('ns3::Ipv4Address', 'origin'), param('ns3::Ipv4Address', 'group'), param('uint32_t', 'inputInterface'), param('std::vector< unsigned int >', 'outputInterfaces')])
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::AddNetworkRouteTo(ns3::Ipv4Address network, ns3::Ipv4Mask networkMask, ns3::Ipv4Address nextHop, uint32_t interface, uint32_t metric=0) [member function]
    cls.add_method('AddNetworkRouteTo', 
                   'void', 
                   [param('ns3::Ipv4Address', 'network'), param('ns3::Ipv4Mask', 'networkMask'), param('ns3::Ipv4Address', 'nextHop'), param('uint32_t', 'interface'), param('uint32_t', 'metric', default_value='0')])
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::AddNetworkRouteTo(ns3::Ipv4Address network, ns3::Ipv4Mask networkMask, uint32_t interface, uint32_t metric=0) [member function]
    cls.add_method('AddNetworkRouteTo', 
                   'void', 
                   [param('ns3::Ipv4Address', 'network'), param('ns3::Ipv4Mask', 'networkMask'), param('uint32_t', 'interface'), param('uint32_t', 'metric', default_value='0')])
    ## ipv4-static-routing.h: ns3::Ipv4RoutingTableEntry ns3::Ipv4StaticRouting::GetDefaultRoute() [member function]
    cls.add_method('GetDefaultRoute', 
                   'ns3::Ipv4RoutingTableEntry', 
                   [])
    ## ipv4-static-routing.h: uint32_t ns3::Ipv4StaticRouting::GetMetric(uint32_t index) [member function]
    cls.add_method('GetMetric', 
                   'uint32_t', 
                   [param('uint32_t', 'index')])
    ## ipv4-static-routing.h: ns3::Ipv4MulticastRoutingTableEntry ns3::Ipv4StaticRouting::GetMulticastRoute(uint32_t i) const [member function]
    cls.add_method('GetMulticastRoute', 
                   'ns3::Ipv4MulticastRoutingTableEntry', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## ipv4-static-routing.h: uint32_t ns3::Ipv4StaticRouting::GetNMulticastRoutes() const [member function]
    cls.add_method('GetNMulticastRoutes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-static-routing.h: uint32_t ns3::Ipv4StaticRouting::GetNRoutes() const [member function]
    cls.add_method('GetNRoutes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-static-routing.h: ns3::Ipv4RoutingTableEntry ns3::Ipv4StaticRouting::GetRoute(uint32_t i) const [member function]
    cls.add_method('GetRoute', 
                   'ns3::Ipv4RoutingTableEntry', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## ipv4-static-routing.h: static ns3::TypeId ns3::Ipv4StaticRouting::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::NotifyAddAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyAddAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::NotifyInterfaceDown(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceDown', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::NotifyInterfaceUp(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceUp', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::NotifyRemoveAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyRemoveAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::PrintRoutingTable(ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('PrintRoutingTable', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_const=True, is_virtual=True)
    ## ipv4-static-routing.h: bool ns3::Ipv4StaticRouting::RemoveMulticastRoute(ns3::Ipv4Address origin, ns3::Ipv4Address group, uint32_t inputInterface) [member function]
    cls.add_method('RemoveMulticastRoute', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'origin'), param('ns3::Ipv4Address', 'group'), param('uint32_t', 'inputInterface')])
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::RemoveMulticastRoute(uint32_t index) [member function]
    cls.add_method('RemoveMulticastRoute', 
                   'void', 
                   [param('uint32_t', 'index')])
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::RemoveRoute(uint32_t i) [member function]
    cls.add_method('RemoveRoute', 
                   'void', 
                   [param('uint32_t', 'i')])
    ## ipv4-static-routing.h: bool ns3::Ipv4StaticRouting::RouteInput(ns3::Ptr<ns3::Packet const> p, ns3::Ipv4Header const & header, ns3::Ptr<const ns3::NetDevice> idev, ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ucb, ns3::Callback<void,ns3::Ptr<ns3::Ipv4MulticastRoute>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> mcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,unsigned int,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> lcb, ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ecb) [member function]
    cls.add_method('RouteInput', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice const >', 'idev'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4MulticastRoute >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'mcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'lcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb')], 
                   is_virtual=True)
    ## ipv4-static-routing.h: ns3::Ptr<ns3::Ipv4Route> ns3::Ipv4StaticRouting::RouteOutput(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Header const & header, ns3::Ptr<ns3::NetDevice> oif, ns3::Socket::SocketErrno & sockerr) [member function]
    cls.add_method('RouteOutput', 
                   'ns3::Ptr< ns3::Ipv4Route >', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice >', 'oif'), param('ns3::Socket::SocketErrno &', 'sockerr')], 
                   is_virtual=True)
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::SetDefaultMulticastRoute(uint32_t outputInterface) [member function]
    cls.add_method('SetDefaultMulticastRoute', 
                   'void', 
                   [param('uint32_t', 'outputInterface')])
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::SetDefaultRoute(ns3::Ipv4Address nextHop, uint32_t interface, uint32_t metric=0) [member function]
    cls.add_method('SetDefaultRoute', 
                   'void', 
                   [param('ns3::Ipv4Address', 'nextHop'), param('uint32_t', 'interface'), param('uint32_t', 'metric', default_value='0')])
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::SetIpv4(ns3::Ptr<ns3::Ipv4> ipv4) [member function]
    cls.add_method('SetIpv4', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4 >', 'ipv4')], 
                   is_virtual=True)
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3Ipv6StaticRouting_methods(root_module, cls):
    ## ipv6-static-routing.h: ns3::Ipv6StaticRouting::Ipv6StaticRouting(ns3::Ipv6StaticRouting const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6StaticRouting const &', 'arg0')])
    ## ipv6-static-routing.h: ns3::Ipv6StaticRouting::Ipv6StaticRouting() [constructor]
    cls.add_constructor([])
    ## ipv6-static-routing.h: void ns3::Ipv6StaticRouting::AddHostRouteTo(ns3::Ipv6Address dest, ns3::Ipv6Address nextHop, uint32_t interface, ns3::Ipv6Address prefixToUse=ns3::Ipv6Address(((const char*)"::")), uint32_t metric=0) [member function]
    cls.add_method('AddHostRouteTo', 
                   'void', 
                   [param('ns3::Ipv6Address', 'dest'), param('ns3::Ipv6Address', 'nextHop'), param('uint32_t', 'interface'), param('ns3::Ipv6Address', 'prefixToUse', default_value='ns3::Ipv6Address(((const char*)"::"))'), param('uint32_t', 'metric', default_value='0')])
    ## ipv6-static-routing.h: void ns3::Ipv6StaticRouting::AddHostRouteTo(ns3::Ipv6Address dest, uint32_t interface, uint32_t metric=0) [member function]
    cls.add_method('AddHostRouteTo', 
                   'void', 
                   [param('ns3::Ipv6Address', 'dest'), param('uint32_t', 'interface'), param('uint32_t', 'metric', default_value='0')])
    ## ipv6-static-routing.h: void ns3::Ipv6StaticRouting::AddMulticastRoute(ns3::Ipv6Address origin, ns3::Ipv6Address group, uint32_t inputInterface, std::vector<unsigned int, std::allocator<unsigned int> > outputInterfaces) [member function]
    cls.add_method('AddMulticastRoute', 
                   'void', 
                   [param('ns3::Ipv6Address', 'origin'), param('ns3::Ipv6Address', 'group'), param('uint32_t', 'inputInterface'), param('std::vector< unsigned int >', 'outputInterfaces')])
    ## ipv6-static-routing.h: void ns3::Ipv6StaticRouting::AddNetworkRouteTo(ns3::Ipv6Address network, ns3::Ipv6Prefix networkPrefix, ns3::Ipv6Address nextHop, uint32_t interface, uint32_t metric=0) [member function]
    cls.add_method('AddNetworkRouteTo', 
                   'void', 
                   [param('ns3::Ipv6Address', 'network'), param('ns3::Ipv6Prefix', 'networkPrefix'), param('ns3::Ipv6Address', 'nextHop'), param('uint32_t', 'interface'), param('uint32_t', 'metric', default_value='0')])
    ## ipv6-static-routing.h: void ns3::Ipv6StaticRouting::AddNetworkRouteTo(ns3::Ipv6Address network, ns3::Ipv6Prefix networkPrefix, ns3::Ipv6Address nextHop, uint32_t interface, ns3::Ipv6Address prefixToUse, uint32_t metric=0) [member function]
    cls.add_method('AddNetworkRouteTo', 
                   'void', 
                   [param('ns3::Ipv6Address', 'network'), param('ns3::Ipv6Prefix', 'networkPrefix'), param('ns3::Ipv6Address', 'nextHop'), param('uint32_t', 'interface'), param('ns3::Ipv6Address', 'prefixToUse'), param('uint32_t', 'metric', default_value='0')])
    ## ipv6-static-routing.h: void ns3::Ipv6StaticRouting::AddNetworkRouteTo(ns3::Ipv6Address network, ns3::Ipv6Prefix networkPrefix, uint32_t interface, uint32_t metric=0) [member function]
    cls.add_method('AddNetworkRouteTo', 
                   'void', 
                   [param('ns3::Ipv6Address', 'network'), param('ns3::Ipv6Prefix', 'networkPrefix'), param('uint32_t', 'interface'), param('uint32_t', 'metric', default_value='0')])
    ## ipv6-static-routing.h: ns3::Ipv6RoutingTableEntry ns3::Ipv6StaticRouting::GetDefaultRoute() [member function]
    cls.add_method('GetDefaultRoute', 
                   'ns3::Ipv6RoutingTableEntry', 
                   [])
    ## ipv6-static-routing.h: uint32_t ns3::Ipv6StaticRouting::GetMetric(uint32_t index) [member function]
    cls.add_method('GetMetric', 
                   'uint32_t', 
                   [param('uint32_t', 'index')])
    ## ipv6-static-routing.h: ns3::Ipv6MulticastRoutingTableEntry ns3::Ipv6StaticRouting::GetMulticastRoute(uint32_t i) const [member function]
    cls.add_method('GetMulticastRoute', 
                   'ns3::Ipv6MulticastRoutingTableEntry', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## ipv6-static-routing.h: uint32_t ns3::Ipv6StaticRouting::GetNMulticastRoutes() const [member function]
    cls.add_method('GetNMulticastRoutes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv6-static-routing.h: uint32_t ns3::Ipv6StaticRouting::GetNRoutes() [member function]
    cls.add_method('GetNRoutes', 
                   'uint32_t', 
                   [])
    ## ipv6-static-routing.h: ns3::Ipv6RoutingTableEntry ns3::Ipv6StaticRouting::GetRoute(uint32_t i) [member function]
    cls.add_method('GetRoute', 
                   'ns3::Ipv6RoutingTableEntry', 
                   [param('uint32_t', 'i')])
    ## ipv6-static-routing.h: static ns3::TypeId ns3::Ipv6StaticRouting::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-static-routing.h: bool ns3::Ipv6StaticRouting::HasNetworkDest(ns3::Ipv6Address dest, uint32_t interfaceIndex) [member function]
    cls.add_method('HasNetworkDest', 
                   'bool', 
                   [param('ns3::Ipv6Address', 'dest'), param('uint32_t', 'interfaceIndex')])
    ## ipv6-static-routing.h: void ns3::Ipv6StaticRouting::NotifyAddAddress(uint32_t interface, ns3::Ipv6InterfaceAddress address) [member function]
    cls.add_method('NotifyAddAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv6InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## ipv6-static-routing.h: void ns3::Ipv6StaticRouting::NotifyAddRoute(ns3::Ipv6Address dst, ns3::Ipv6Prefix mask, ns3::Ipv6Address nextHop, uint32_t interface, ns3::Ipv6Address prefixToUse=ns3::Ipv6Address::GetZero( )) [member function]
    cls.add_method('NotifyAddRoute', 
                   'void', 
                   [param('ns3::Ipv6Address', 'dst'), param('ns3::Ipv6Prefix', 'mask'), param('ns3::Ipv6Address', 'nextHop'), param('uint32_t', 'interface'), param('ns3::Ipv6Address', 'prefixToUse', default_value='ns3::Ipv6Address::GetZero( )')], 
                   is_virtual=True)
    ## ipv6-static-routing.h: void ns3::Ipv6StaticRouting::NotifyInterfaceDown(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceDown', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## ipv6-static-routing.h: void ns3::Ipv6StaticRouting::NotifyInterfaceUp(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceUp', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## ipv6-static-routing.h: void ns3::Ipv6StaticRouting::NotifyRemoveAddress(uint32_t interface, ns3::Ipv6InterfaceAddress address) [member function]
    cls.add_method('NotifyRemoveAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv6InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## ipv6-static-routing.h: void ns3::Ipv6StaticRouting::NotifyRemoveRoute(ns3::Ipv6Address dst, ns3::Ipv6Prefix mask, ns3::Ipv6Address nextHop, uint32_t interface, ns3::Ipv6Address prefixToUse=ns3::Ipv6Address::GetZero( )) [member function]
    cls.add_method('NotifyRemoveRoute', 
                   'void', 
                   [param('ns3::Ipv6Address', 'dst'), param('ns3::Ipv6Prefix', 'mask'), param('ns3::Ipv6Address', 'nextHop'), param('uint32_t', 'interface'), param('ns3::Ipv6Address', 'prefixToUse', default_value='ns3::Ipv6Address::GetZero( )')], 
                   is_virtual=True)
    ## ipv6-static-routing.h: bool ns3::Ipv6StaticRouting::RemoveMulticastRoute(ns3::Ipv6Address origin, ns3::Ipv6Address group, uint32_t inputInterface) [member function]
    cls.add_method('RemoveMulticastRoute', 
                   'bool', 
                   [param('ns3::Ipv6Address', 'origin'), param('ns3::Ipv6Address', 'group'), param('uint32_t', 'inputInterface')])
    ## ipv6-static-routing.h: void ns3::Ipv6StaticRouting::RemoveMulticastRoute(uint32_t i) [member function]
    cls.add_method('RemoveMulticastRoute', 
                   'void', 
                   [param('uint32_t', 'i')])
    ## ipv6-static-routing.h: void ns3::Ipv6StaticRouting::RemoveRoute(uint32_t i) [member function]
    cls.add_method('RemoveRoute', 
                   'void', 
                   [param('uint32_t', 'i')])
    ## ipv6-static-routing.h: void ns3::Ipv6StaticRouting::RemoveRoute(ns3::Ipv6Address network, ns3::Ipv6Prefix prefix, uint32_t ifIndex, ns3::Ipv6Address prefixToUse) [member function]
    cls.add_method('RemoveRoute', 
                   'void', 
                   [param('ns3::Ipv6Address', 'network'), param('ns3::Ipv6Prefix', 'prefix'), param('uint32_t', 'ifIndex'), param('ns3::Ipv6Address', 'prefixToUse')])
    ## ipv6-static-routing.h: bool ns3::Ipv6StaticRouting::RouteInput(ns3::Ptr<ns3::Packet const> p, ns3::Ipv6Header const & header, ns3::Ptr<const ns3::NetDevice> idev, ns3::Callback<void,ns3::Ptr<ns3::Ipv6Route>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv6Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> ucb, ns3::Callback<void,ns3::Ptr<ns3::Ipv6MulticastRoute>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv6Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> mcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv6Header&,unsigned int,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> lcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv6Header&,ns3::Socket::SocketErrno,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> ecb) [member function]
    cls.add_method('RouteInput', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ipv6Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice const >', 'idev'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv6Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv6Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv6MulticastRoute >, ns3::Ptr< ns3::Packet const >, ns3::Ipv6Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'mcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv6Header const &, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'lcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv6Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb')], 
                   is_virtual=True)
    ## ipv6-static-routing.h: ns3::Ptr<ns3::Ipv6Route> ns3::Ipv6StaticRouting::RouteOutput(ns3::Ptr<ns3::Packet> p, ns3::Ipv6Header const & header, ns3::Ptr<ns3::NetDevice> oif, ns3::Socket::SocketErrno & sockerr) [member function]
    cls.add_method('RouteOutput', 
                   'ns3::Ptr< ns3::Ipv6Route >', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv6Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice >', 'oif'), param('ns3::Socket::SocketErrno &', 'sockerr')], 
                   is_virtual=True)
    ## ipv6-static-routing.h: void ns3::Ipv6StaticRouting::SetDefaultMulticastRoute(uint32_t outputInterface) [member function]
    cls.add_method('SetDefaultMulticastRoute', 
                   'void', 
                   [param('uint32_t', 'outputInterface')])
    ## ipv6-static-routing.h: void ns3::Ipv6StaticRouting::SetDefaultRoute(ns3::Ipv6Address nextHop, uint32_t interface, ns3::Ipv6Address prefixToUse=ns3::Ipv6Address(((const char*)"::")), uint32_t metric=0) [member function]
    cls.add_method('SetDefaultRoute', 
                   'void', 
                   [param('ns3::Ipv6Address', 'nextHop'), param('uint32_t', 'interface'), param('ns3::Ipv6Address', 'prefixToUse', default_value='ns3::Ipv6Address(((const char*)"::"))'), param('uint32_t', 'metric', default_value='0')])
    ## ipv6-static-routing.h: void ns3::Ipv6StaticRouting::SetIpv6(ns3::Ptr<ns3::Ipv6> ipv6) [member function]
    cls.add_method('SetIpv6', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv6 >', 'ipv6')], 
                   is_virtual=True)
    ## ipv6-static-routing.h: void ns3::Ipv6StaticRouting::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3JakesFadingLossModel_methods(root_module, cls):
    ## jakes-fading-loss-model.h: ns3::JakesFadingLossModel::JakesFadingLossModel(ns3::JakesFadingLossModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::JakesFadingLossModel const &', 'arg0')])
    ## jakes-fading-loss-model.h: ns3::JakesFadingLossModel::JakesFadingLossModel() [constructor]
    cls.add_constructor([])
    ## jakes-fading-loss-model.h: ns3::Ptr<ns3::LtePhy> ns3::JakesFadingLossModel::GetPhy() [member function]
    cls.add_method('GetPhy', 
                   'ns3::Ptr< ns3::LtePhy >', 
                   [])
    ## jakes-fading-loss-model.h: static ns3::TypeId ns3::JakesFadingLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## jakes-fading-loss-model.h: double ns3::JakesFadingLossModel::GetValue(int subChannel) [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('int', 'subChannel')])
    ## jakes-fading-loss-model.h: void ns3::JakesFadingLossModel::SetPhy(ns3::Ptr<ns3::LtePhy> phy) [member function]
    cls.add_method('SetPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::LtePhy >', 'phy')])
    ## jakes-fading-loss-model.h: void ns3::JakesFadingLossModel::SetValue() [member function]
    cls.add_method('SetValue', 
                   'void', 
                   [])
    return

def register_Ns3LiIonEnergySource_methods(root_module, cls):
    ## li-ion-energy-source.h: ns3::LiIonEnergySource::LiIonEnergySource(ns3::LiIonEnergySource const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LiIonEnergySource const &', 'arg0')])
    ## li-ion-energy-source.h: ns3::LiIonEnergySource::LiIonEnergySource() [constructor]
    cls.add_constructor([])
    ## li-ion-energy-source.h: void ns3::LiIonEnergySource::DecreaseRemainingEnergy(double energyJ) [member function]
    cls.add_method('DecreaseRemainingEnergy', 
                   'void', 
                   [param('double', 'energyJ')], 
                   is_virtual=True)
    ## li-ion-energy-source.h: double ns3::LiIonEnergySource::GetEnergyFraction() [member function]
    cls.add_method('GetEnergyFraction', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## li-ion-energy-source.h: ns3::Time ns3::LiIonEnergySource::GetEnergyUpdateInterval() const [member function]
    cls.add_method('GetEnergyUpdateInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## li-ion-energy-source.h: double ns3::LiIonEnergySource::GetInitialEnergy() const [member function]
    cls.add_method('GetInitialEnergy', 
                   'double', 
                   [], 
                   is_const=True, is_virtual=True)
    ## li-ion-energy-source.h: double ns3::LiIonEnergySource::GetRemainingEnergy() [member function]
    cls.add_method('GetRemainingEnergy', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## li-ion-energy-source.h: double ns3::LiIonEnergySource::GetSupplyVoltage() const [member function]
    cls.add_method('GetSupplyVoltage', 
                   'double', 
                   [], 
                   is_const=True, is_virtual=True)
    ## li-ion-energy-source.h: static ns3::TypeId ns3::LiIonEnergySource::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## li-ion-energy-source.h: void ns3::LiIonEnergySource::IncreaseRemainingEnergy(double energyJ) [member function]
    cls.add_method('IncreaseRemainingEnergy', 
                   'void', 
                   [param('double', 'energyJ')], 
                   is_virtual=True)
    ## li-ion-energy-source.h: void ns3::LiIonEnergySource::SetEnergyUpdateInterval(ns3::Time interval) [member function]
    cls.add_method('SetEnergyUpdateInterval', 
                   'void', 
                   [param('ns3::Time', 'interval')])
    ## li-ion-energy-source.h: void ns3::LiIonEnergySource::SetInitialEnergy(double initialEnergyJ) [member function]
    cls.add_method('SetInitialEnergy', 
                   'void', 
                   [param('double', 'initialEnergyJ')])
    ## li-ion-energy-source.h: void ns3::LiIonEnergySource::SetInitialSupplyVoltage(double supplyVoltageV) [member function]
    cls.add_method('SetInitialSupplyVoltage', 
                   'void', 
                   [param('double', 'supplyVoltageV')])
    ## li-ion-energy-source.h: void ns3::LiIonEnergySource::UpdateEnergySource() [member function]
    cls.add_method('UpdateEnergySource', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## li-ion-energy-source.h: void ns3::LiIonEnergySource::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## li-ion-energy-source.h: void ns3::LiIonEnergySource::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3LteMacQueue_methods(root_module, cls):
    ## lte-mac-queue.h: ns3::LteMacQueue::LteMacQueue(ns3::LteMacQueue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteMacQueue const &', 'arg0')])
    ## lte-mac-queue.h: ns3::LteMacQueue::LteMacQueue() [constructor]
    cls.add_constructor([])
    ## lte-mac-queue.h: ns3::LteMacQueue::LteMacQueue(uint32_t maxSize) [constructor]
    cls.add_constructor([param('uint32_t', 'maxSize')])
    ## lte-mac-queue.h: ns3::Ptr<ns3::Packet> ns3::LteMacQueue::Dequeue() [member function]
    cls.add_method('Dequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [])
    ## lte-mac-queue.h: ns3::Ptr<ns3::Packet> ns3::LteMacQueue::Dequeue(uint32_t availableByte) [member function]
    cls.add_method('Dequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'availableByte')])
    ## lte-mac-queue.h: bool ns3::LteMacQueue::Enqueue(ns3::Ptr<ns3::Packet> packet) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet')])
    ## lte-mac-queue.h: uint32_t ns3::LteMacQueue::GetMaxSize() const [member function]
    cls.add_method('GetMaxSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## lte-mac-queue.h: uint32_t ns3::LteMacQueue::GetNBytes() const [member function]
    cls.add_method('GetNBytes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## lte-mac-queue.h: std::deque<ns3::LteMacQueue::QueueElement, std::allocator<ns3::LteMacQueue::QueueElement> > const & ns3::LteMacQueue::GetPacketQueue() const [member function]
    cls.add_method('GetPacketQueue', 
                   'std::deque< ns3::LteMacQueue::QueueElement > const &', 
                   [], 
                   is_const=True)
    ## lte-mac-queue.h: uint32_t ns3::LteMacQueue::GetQueueLengthWithMACOverhead() [member function]
    cls.add_method('GetQueueLengthWithMACOverhead', 
                   'uint32_t', 
                   [])
    ## lte-mac-queue.h: uint32_t ns3::LteMacQueue::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## lte-mac-queue.h: static ns3::TypeId ns3::LteMacQueue::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-mac-queue.h: bool ns3::LteMacQueue::IsEmpty() const [member function]
    cls.add_method('IsEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## lte-mac-queue.h: ns3::Ptr<ns3::Packet> ns3::LteMacQueue::Peek() const [member function]
    cls.add_method('Peek', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True)
    ## lte-mac-queue.h: void ns3::LteMacQueue::SetMaxSize(uint32_t maxSize) [member function]
    cls.add_method('SetMaxSize', 
                   'void', 
                   [param('uint32_t', 'maxSize')])
    return

def register_Ns3LtePhy_methods(root_module, cls):
    ## lte-phy.h: ns3::LtePhy::LtePhy(ns3::LtePhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LtePhy const &', 'arg0')])
    ## lte-phy.h: ns3::LtePhy::LtePhy() [constructor]
    cls.add_constructor([])
    ## lte-phy.h: ns3::Ptr<ns3::SpectrumValue> ns3::LtePhy::CreateTxPowerSpectralDensity() [member function]
    cls.add_method('CreateTxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-phy.h: void ns3::LtePhy::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-phy.h: void ns3::LtePhy::DoSetDownlinkSubChannels() [member function]
    cls.add_method('DoSetDownlinkSubChannels', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-phy.h: void ns3::LtePhy::DoSetUplinkSubChannels() [member function]
    cls.add_method('DoSetUplinkSubChannels', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-phy.h: ns3::Ptr<ns3::LteNetDevice> ns3::LtePhy::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::LteNetDevice >', 
                   [])
    ## lte-phy.h: ns3::Ptr<ns3::SpectrumChannel> ns3::LtePhy::GetDownlinkChannel() [member function]
    cls.add_method('GetDownlinkChannel', 
                   'ns3::Ptr< ns3::SpectrumChannel >', 
                   [])
    ## lte-phy.h: ns3::Ptr<ns3::LteSpectrumPhy> ns3::LtePhy::GetDownlinkSpectrumPhy() [member function]
    cls.add_method('GetDownlinkSpectrumPhy', 
                   'ns3::Ptr< ns3::LteSpectrumPhy >', 
                   [])
    ## lte-phy.h: std::vector<int, std::allocator<int> > ns3::LtePhy::GetDownlinkSubChannels() [member function]
    cls.add_method('GetDownlinkSubChannels', 
                   'std::vector< int >', 
                   [])
    ## lte-phy.h: uint32_t ns3::LtePhy::GetNrFrames() const [member function]
    cls.add_method('GetNrFrames', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## lte-phy.h: uint32_t ns3::LtePhy::GetNrSubFrames() const [member function]
    cls.add_method('GetNrSubFrames', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## lte-phy.h: double ns3::LtePhy::GetTti() const [member function]
    cls.add_method('GetTti', 
                   'double', 
                   [], 
                   is_const=True)
    ## lte-phy.h: double ns3::LtePhy::GetTxPower() [member function]
    cls.add_method('GetTxPower', 
                   'double', 
                   [])
    ## lte-phy.h: static ns3::TypeId ns3::LtePhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-phy.h: ns3::Ptr<ns3::SpectrumChannel> ns3::LtePhy::GetUplinkChannel() [member function]
    cls.add_method('GetUplinkChannel', 
                   'ns3::Ptr< ns3::SpectrumChannel >', 
                   [])
    ## lte-phy.h: ns3::Ptr<ns3::LteSpectrumPhy> ns3::LtePhy::GetUplinkSpectrumPhy() [member function]
    cls.add_method('GetUplinkSpectrumPhy', 
                   'ns3::Ptr< ns3::LteSpectrumPhy >', 
                   [])
    ## lte-phy.h: std::vector<int, std::allocator<int> > ns3::LtePhy::GetUplinkSubChannels() [member function]
    cls.add_method('GetUplinkSubChannels', 
                   'std::vector< int >', 
                   [])
    ## lte-phy.h: void ns3::LtePhy::ReceiveIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('ReceiveIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-phy.h: void ns3::LtePhy::SendIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('SendIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-phy.h: bool ns3::LtePhy::SendPacket(ns3::Ptr<ns3::PacketBurst> pb) [member function]
    cls.add_method('SendPacket', 
                   'bool', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'pb')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-phy.h: void ns3::LtePhy::SetDevice(ns3::Ptr<ns3::LteNetDevice> d) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::LteNetDevice >', 'd')])
    ## lte-phy.h: void ns3::LtePhy::SetDownlinkChannel(ns3::Ptr<ns3::SpectrumChannel> c) [member function]
    cls.add_method('SetDownlinkChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'c')])
    ## lte-phy.h: void ns3::LtePhy::SetDownlinkSpectrumPhy(ns3::Ptr<ns3::LteSpectrumPhy> s) [member function]
    cls.add_method('SetDownlinkSpectrumPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::LteSpectrumPhy >', 's')])
    ## lte-phy.h: void ns3::LtePhy::SetDownlinkSubChannels(std::vector<int, std::allocator<int> > mask) [member function]
    cls.add_method('SetDownlinkSubChannels', 
                   'void', 
                   [param('std::vector< int >', 'mask')])
    ## lte-phy.h: void ns3::LtePhy::SetNrFrames(uint32_t nrFrames) [member function]
    cls.add_method('SetNrFrames', 
                   'void', 
                   [param('uint32_t', 'nrFrames')])
    ## lte-phy.h: void ns3::LtePhy::SetNrSubFrames(uint32_t nrSubFrames) [member function]
    cls.add_method('SetNrSubFrames', 
                   'void', 
                   [param('uint32_t', 'nrSubFrames')])
    ## lte-phy.h: void ns3::LtePhy::SetTti(double tti) [member function]
    cls.add_method('SetTti', 
                   'void', 
                   [param('double', 'tti')])
    ## lte-phy.h: void ns3::LtePhy::SetTxPower(double pw) [member function]
    cls.add_method('SetTxPower', 
                   'void', 
                   [param('double', 'pw')])
    ## lte-phy.h: void ns3::LtePhy::SetUplinkChannel(ns3::Ptr<ns3::SpectrumChannel> c) [member function]
    cls.add_method('SetUplinkChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'c')])
    ## lte-phy.h: void ns3::LtePhy::SetUplinkSpectrumPhy(ns3::Ptr<ns3::LteSpectrumPhy> s) [member function]
    cls.add_method('SetUplinkSpectrumPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::LteSpectrumPhy >', 's')])
    ## lte-phy.h: void ns3::LtePhy::SetUplinkSubChannels(std::vector<int, std::allocator<int> > mask) [member function]
    cls.add_method('SetUplinkSubChannels', 
                   'void', 
                   [param('std::vector< int >', 'mask')])
    return

def register_Ns3LtePropagationLossModel_methods(root_module, cls):
    ## lte-propagation-loss-model.h: ns3::LtePropagationLossModel::LtePropagationLossModel(ns3::LtePropagationLossModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LtePropagationLossModel const &', 'arg0')])
    ## lte-propagation-loss-model.h: ns3::LtePropagationLossModel::LtePropagationLossModel() [constructor]
    cls.add_constructor([])
    ## lte-propagation-loss-model.h: void ns3::LtePropagationLossModel::CreateChannelRealization(ns3::Ptr<const ns3::MobilityModel> enbMobility, ns3::Ptr<const ns3::MobilityModel> ueMobility) [member function]
    cls.add_method('CreateChannelRealization', 
                   'void', 
                   [param('ns3::Ptr< ns3::MobilityModel const >', 'enbMobility'), param('ns3::Ptr< ns3::MobilityModel const >', 'ueMobility')])
    ## lte-propagation-loss-model.h: ns3::Ptr<ns3::ChannelRealization> ns3::LtePropagationLossModel::GetChannelRealization(ns3::Ptr<const ns3::MobilityModel> a, ns3::Ptr<const ns3::MobilityModel> b) const [member function]
    cls.add_method('GetChannelRealization', 
                   'ns3::Ptr< ns3::ChannelRealization >', 
                   [param('ns3::Ptr< ns3::MobilityModel const >', 'a'), param('ns3::Ptr< ns3::MobilityModel const >', 'b')], 
                   is_const=True)
    ## lte-propagation-loss-model.h: static ns3::TypeId ns3::LtePropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-propagation-loss-model.h: ns3::Ptr<ns3::SpectrumValue> ns3::LtePropagationLossModel::DoCalcRxPowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue const> txPsd, ns3::Ptr<const ns3::MobilityModel> a, ns3::Ptr<const ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'txPsd'), param('ns3::Ptr< ns3::MobilityModel const >', 'a'), param('ns3::Ptr< ns3::MobilityModel const >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3LteSpectrumPhy_methods(root_module, cls):
    ## lte-spectrum-phy.h: ns3::LteSpectrumPhy::LteSpectrumPhy(ns3::LteSpectrumPhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteSpectrumPhy const &', 'arg0')])
    ## lte-spectrum-phy.h: ns3::LteSpectrumPhy::LteSpectrumPhy() [constructor]
    cls.add_constructor([])
    ## lte-spectrum-phy.h: void ns3::LteSpectrumPhy::CalcSinrValues(ns3::Ptr<ns3::SpectrumValue const> rxPsd, ns3::Ptr<ns3::SpectrumValue const> noise) [member function]
    cls.add_method('CalcSinrValues', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'rxPsd'), param('ns3::Ptr< ns3::SpectrumValue const >', 'noise')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-spectrum-phy.h: ns3::Ptr<ns3::SpectrumChannel> ns3::LteSpectrumPhy::GetChannel() [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::SpectrumChannel >', 
                   [])
    ## lte-spectrum-phy.h: ns3::Ptr<ns3::Object> ns3::LteSpectrumPhy::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_virtual=True)
    ## lte-spectrum-phy.h: ns3::Ptr<ns3::Object> ns3::LteSpectrumPhy::GetMobility() [member function]
    cls.add_method('GetMobility', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_virtual=True)
    ## lte-spectrum-phy.h: ns3::Ptr<ns3::SpectrumValue const> ns3::LteSpectrumPhy::GetNoisePowerSpectralDensity() [member function]
    cls.add_method('GetNoisePowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue const >', 
                   [])
    ## lte-spectrum-phy.h: ns3::Ptr<ns3::SpectrumModel const> ns3::LteSpectrumPhy::GetRxSpectrumModel() const [member function]
    cls.add_method('GetRxSpectrumModel', 
                   'ns3::Ptr< ns3::SpectrumModel const >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-spectrum-phy.h: ns3::SpectrumType ns3::LteSpectrumPhy::GetSpectrumType() [member function]
    cls.add_method('GetSpectrumType', 
                   'ns3::SpectrumType', 
                   [])
    ## lte-spectrum-phy.h: static ns3::TypeId ns3::LteSpectrumPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-spectrum-phy.h: void ns3::LteSpectrumPhy::SetChannel(ns3::Ptr<ns3::SpectrumChannel> c) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'c')], 
                   is_virtual=True)
    ## lte-spectrum-phy.h: void ns3::LteSpectrumPhy::SetDevice(ns3::Ptr<ns3::Object> d) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'd')], 
                   is_virtual=True)
    ## lte-spectrum-phy.h: void ns3::LteSpectrumPhy::SetMobility(ns3::Ptr<ns3::Object> m) [member function]
    cls.add_method('SetMobility', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'm')], 
                   is_virtual=True)
    ## lte-spectrum-phy.h: void ns3::LteSpectrumPhy::SetNoisePowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue const> noisePsd) [member function]
    cls.add_method('SetNoisePowerSpectralDensity', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'noisePsd')])
    ## lte-spectrum-phy.h: void ns3::LteSpectrumPhy::SetPhyMacRxEndErrorCallback(ns3::PhyMacRxEndErrorCallback c) [member function]
    cls.add_method('SetPhyMacRxEndErrorCallback', 
                   'void', 
                   [param('ns3::PhyMacRxEndErrorCallback', 'c')])
    ## lte-spectrum-phy.h: void ns3::LteSpectrumPhy::SetPhyMacRxEndOkCallback(ns3::PhyMacRxEndOkCallback c) [member function]
    cls.add_method('SetPhyMacRxEndOkCallback', 
                   'void', 
                   [param('ns3::PhyMacRxEndOkCallback', 'c')])
    ## lte-spectrum-phy.h: void ns3::LteSpectrumPhy::SetPhyMacRxStartCallback(ns3::PhyMacRxStartCallback c) [member function]
    cls.add_method('SetPhyMacRxStartCallback', 
                   'void', 
                   [param('ns3::PhyMacRxStartCallback', 'c')])
    ## lte-spectrum-phy.h: void ns3::LteSpectrumPhy::SetPhyMacTxEndCallback(ns3::PhyMacTxEndCallback c) [member function]
    cls.add_method('SetPhyMacTxEndCallback', 
                   'void', 
                   [param('ns3::PhyMacTxEndCallback', 'c')])
    ## lte-spectrum-phy.h: void ns3::LteSpectrumPhy::SetState(ns3::LteSpectrumPhy::State newState) [member function]
    cls.add_method('SetState', 
                   'void', 
                   [param('ns3::LteSpectrumPhy::State', 'newState')])
    ## lte-spectrum-phy.h: void ns3::LteSpectrumPhy::SetTxPowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue> txPsd) [member function]
    cls.add_method('SetTxPowerSpectralDensity', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue >', 'txPsd')])
    ## lte-spectrum-phy.h: void ns3::LteSpectrumPhy::StartRx(ns3::Ptr<ns3::PacketBurst> pb, ns3::Ptr<ns3::SpectrumValue const> rxPsd, ns3::SpectrumType st, ns3::Time duration) [member function]
    cls.add_method('StartRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'pb'), param('ns3::Ptr< ns3::SpectrumValue const >', 'rxPsd'), param('ns3::SpectrumType', 'st'), param('ns3::Time', 'duration')], 
                   is_virtual=True)
    ## lte-spectrum-phy.h: bool ns3::LteSpectrumPhy::StartTx(ns3::Ptr<ns3::PacketBurst> pb) [member function]
    cls.add_method('StartTx', 
                   'bool', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'pb')])
    ## lte-spectrum-phy.h: void ns3::LteSpectrumPhy::EndRx() [member function]
    cls.add_method('EndRx', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3MacEntity_methods(root_module, cls):
    ## mac-entity.h: ns3::MacEntity::MacEntity(ns3::MacEntity const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MacEntity const &', 'arg0')])
    ## mac-entity.h: ns3::MacEntity::MacEntity() [constructor]
    cls.add_constructor([])
    ## mac-entity.h: void ns3::MacEntity::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## mac-entity.h: ns3::Ptr<ns3::AmcModule> ns3::MacEntity::GetAmcModule() const [member function]
    cls.add_method('GetAmcModule', 
                   'ns3::Ptr< ns3::AmcModule >', 
                   [], 
                   is_const=True)
    ## mac-entity.h: ns3::Ptr<ns3::LteNetDevice> ns3::MacEntity::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::LteNetDevice >', 
                   [])
    ## mac-entity.h: static ns3::TypeId ns3::MacEntity::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mac-entity.h: void ns3::MacEntity::SetAmcModule(ns3::Ptr<ns3::AmcModule> amcModule) [member function]
    cls.add_method('SetAmcModule', 
                   'void', 
                   [param('ns3::Ptr< ns3::AmcModule >', 'amcModule')])
    ## mac-entity.h: void ns3::MacEntity::SetDevice(ns3::Ptr<ns3::LteNetDevice> d) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::LteNetDevice >', 'd')])
    return

def register_Ns3PathLossModel_methods(root_module, cls):
    ## path-loss-model.h: ns3::PathLossModel::PathLossModel(ns3::PathLossModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PathLossModel const &', 'arg0')])
    ## path-loss-model.h: ns3::PathLossModel::PathLossModel() [constructor]
    cls.add_constructor([])
    ## path-loss-model.h: static ns3::TypeId ns3::PathLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## path-loss-model.h: double ns3::PathLossModel::GetValue(ns3::Ptr<const ns3::MobilityModel> a, ns3::Ptr<const ns3::MobilityModel> b) [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel const >', 'a'), param('ns3::Ptr< ns3::MobilityModel const >', 'b')])
    ## path-loss-model.h: void ns3::PathLossModel::SetValue(double pl) [member function]
    cls.add_method('SetValue', 
                   'void', 
                   [param('double', 'pl')])
    return

def register_Ns3PdcchMapIdealControlMessage_methods(root_module, cls):
    ## ideal-control-messages.h: ns3::PdcchMapIdealControlMessage::PdcchMapIdealControlMessage(ns3::PdcchMapIdealControlMessage const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PdcchMapIdealControlMessage const &', 'arg0')])
    ## ideal-control-messages.h: ns3::PdcchMapIdealControlMessage::PdcchMapIdealControlMessage() [constructor]
    cls.add_constructor([])
    ## ideal-control-messages.h: void ns3::PdcchMapIdealControlMessage::AddNewRecord(ns3::PdcchMapIdealControlMessage::Direction direction, int subChannel, ns3::Ptr<ns3::LteNetDevice> ue, double mcs) [member function]
    cls.add_method('AddNewRecord', 
                   'void', 
                   [param('ns3::PdcchMapIdealControlMessage::Direction', 'direction'), param('int', 'subChannel'), param('ns3::Ptr< ns3::LteNetDevice >', 'ue'), param('double', 'mcs')])
    ## ideal-control-messages.h: std::list<ns3::PdcchMapIdealControlMessage::IdealPdcchRecord,std::allocator<ns3::PdcchMapIdealControlMessage::IdealPdcchRecord> > * ns3::PdcchMapIdealControlMessage::GetMessage() [member function]
    cls.add_method('GetMessage', 
                   'std::list< ns3::PdcchMapIdealControlMessage::IdealPdcchRecord > *', 
                   [])
    return

def register_Ns3PdcchMapIdealControlMessageIdealPdcchRecord_methods(root_module, cls):
    ## ideal-control-messages.h: ns3::PdcchMapIdealControlMessage::IdealPdcchRecord::IdealPdcchRecord() [constructor]
    cls.add_constructor([])
    ## ideal-control-messages.h: ns3::PdcchMapIdealControlMessage::IdealPdcchRecord::IdealPdcchRecord(ns3::PdcchMapIdealControlMessage::IdealPdcchRecord const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PdcchMapIdealControlMessage::IdealPdcchRecord const &', 'arg0')])
    ## ideal-control-messages.h: ns3::PdcchMapIdealControlMessage::IdealPdcchRecord::m_direction [variable]
    cls.add_instance_attribute('m_direction', 'ns3::PdcchMapIdealControlMessage::Direction', is_const=False)
    ## ideal-control-messages.h: ns3::PdcchMapIdealControlMessage::IdealPdcchRecord::m_idSubChannel [variable]
    cls.add_instance_attribute('m_idSubChannel', 'int', is_const=False)
    ## ideal-control-messages.h: ns3::PdcchMapIdealControlMessage::IdealPdcchRecord::m_mcsIndex [variable]
    cls.add_instance_attribute('m_mcsIndex', 'double', is_const=False)
    ## ideal-control-messages.h: ns3::PdcchMapIdealControlMessage::IdealPdcchRecord::m_ue [variable]
    cls.add_instance_attribute('m_ue', 'ns3::Ptr< ns3::LteNetDevice >', is_const=False)
    return

def register_Ns3PenetrationLossModel_methods(root_module, cls):
    ## penetration-loss-model.h: ns3::PenetrationLossModel::PenetrationLossModel(ns3::PenetrationLossModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PenetrationLossModel const &', 'arg0')])
    ## penetration-loss-model.h: ns3::PenetrationLossModel::PenetrationLossModel() [constructor]
    cls.add_constructor([])
    ## penetration-loss-model.h: static ns3::TypeId ns3::PenetrationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## penetration-loss-model.h: double ns3::PenetrationLossModel::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [])
    ## penetration-loss-model.h: void ns3::PenetrationLossModel::SetValue(double pnl) [member function]
    cls.add_method('SetValue', 
                   'void', 
                   [param('double', 'pnl')])
    return

def register_Ns3RvBatteryModel_methods(root_module, cls):
    ## rv-battery-model.h: ns3::RvBatteryModel::RvBatteryModel(ns3::RvBatteryModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RvBatteryModel const &', 'arg0')])
    ## rv-battery-model.h: ns3::RvBatteryModel::RvBatteryModel() [constructor]
    cls.add_constructor([])
    ## rv-battery-model.h: double ns3::RvBatteryModel::GetAlpha() const [member function]
    cls.add_method('GetAlpha', 
                   'double', 
                   [], 
                   is_const=True)
    ## rv-battery-model.h: double ns3::RvBatteryModel::GetBatteryLevel() [member function]
    cls.add_method('GetBatteryLevel', 
                   'double', 
                   [])
    ## rv-battery-model.h: double ns3::RvBatteryModel::GetBeta() const [member function]
    cls.add_method('GetBeta', 
                   'double', 
                   [], 
                   is_const=True)
    ## rv-battery-model.h: double ns3::RvBatteryModel::GetCutoffVoltage() const [member function]
    cls.add_method('GetCutoffVoltage', 
                   'double', 
                   [], 
                   is_const=True)
    ## rv-battery-model.h: double ns3::RvBatteryModel::GetEnergyFraction() [member function]
    cls.add_method('GetEnergyFraction', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## rv-battery-model.h: double ns3::RvBatteryModel::GetInitialEnergy() const [member function]
    cls.add_method('GetInitialEnergy', 
                   'double', 
                   [], 
                   is_const=True, is_virtual=True)
    ## rv-battery-model.h: ns3::Time ns3::RvBatteryModel::GetLifetime() const [member function]
    cls.add_method('GetLifetime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## rv-battery-model.h: int ns3::RvBatteryModel::GetNumOfTerms() const [member function]
    cls.add_method('GetNumOfTerms', 
                   'int', 
                   [], 
                   is_const=True)
    ## rv-battery-model.h: double ns3::RvBatteryModel::GetOpenCircuitVoltage() const [member function]
    cls.add_method('GetOpenCircuitVoltage', 
                   'double', 
                   [], 
                   is_const=True)
    ## rv-battery-model.h: double ns3::RvBatteryModel::GetRemainingEnergy() [member function]
    cls.add_method('GetRemainingEnergy', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## rv-battery-model.h: ns3::Time ns3::RvBatteryModel::GetSamplingInterval() const [member function]
    cls.add_method('GetSamplingInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## rv-battery-model.h: double ns3::RvBatteryModel::GetSupplyVoltage() const [member function]
    cls.add_method('GetSupplyVoltage', 
                   'double', 
                   [], 
                   is_const=True, is_virtual=True)
    ## rv-battery-model.h: static ns3::TypeId ns3::RvBatteryModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## rv-battery-model.h: void ns3::RvBatteryModel::SetAlpha(double alpha) [member function]
    cls.add_method('SetAlpha', 
                   'void', 
                   [param('double', 'alpha')])
    ## rv-battery-model.h: void ns3::RvBatteryModel::SetBeta(double beta) [member function]
    cls.add_method('SetBeta', 
                   'void', 
                   [param('double', 'beta')])
    ## rv-battery-model.h: void ns3::RvBatteryModel::SetCutoffVoltage(double voltage) [member function]
    cls.add_method('SetCutoffVoltage', 
                   'void', 
                   [param('double', 'voltage')])
    ## rv-battery-model.h: void ns3::RvBatteryModel::SetNumOfTerms(int num) [member function]
    cls.add_method('SetNumOfTerms', 
                   'void', 
                   [param('int', 'num')])
    ## rv-battery-model.h: void ns3::RvBatteryModel::SetOpenCircuitVoltage(double voltage) [member function]
    cls.add_method('SetOpenCircuitVoltage', 
                   'void', 
                   [param('double', 'voltage')])
    ## rv-battery-model.h: void ns3::RvBatteryModel::SetSamplingInterval(ns3::Time interval) [member function]
    cls.add_method('SetSamplingInterval', 
                   'void', 
                   [param('ns3::Time', 'interval')])
    ## rv-battery-model.h: void ns3::RvBatteryModel::UpdateEnergySource() [member function]
    cls.add_method('UpdateEnergySource', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## rv-battery-model.h: void ns3::RvBatteryModel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## rv-battery-model.h: void ns3::RvBatteryModel::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3ShadowingLossModel_methods(root_module, cls):
    ## shadowing-loss-model.h: ns3::ShadowingLossModel::ShadowingLossModel(ns3::ShadowingLossModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ShadowingLossModel const &', 'arg0')])
    ## shadowing-loss-model.h: ns3::ShadowingLossModel::ShadowingLossModel() [constructor]
    cls.add_constructor([])
    ## shadowing-loss-model.h: ns3::ShadowingLossModel::ShadowingLossModel(double mu, double sigma, double samplingPeriod) [constructor]
    cls.add_constructor([param('double', 'mu'), param('double', 'sigma'), param('double', 'samplingPeriod')])
    ## shadowing-loss-model.h: static ns3::TypeId ns3::ShadowingLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## shadowing-loss-model.h: double ns3::ShadowingLossModel::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [])
    ## shadowing-loss-model.h: void ns3::ShadowingLossModel::SetValue(double sh) [member function]
    cls.add_method('SetValue', 
                   'void', 
                   [param('double', 'sh')])
    return

def register_Ns3SimpleDeviceEnergyModel_methods(root_module, cls):
    ## simple-device-energy-model.h: ns3::SimpleDeviceEnergyModel::SimpleDeviceEnergyModel(ns3::SimpleDeviceEnergyModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SimpleDeviceEnergyModel const &', 'arg0')])
    ## simple-device-energy-model.h: ns3::SimpleDeviceEnergyModel::SimpleDeviceEnergyModel() [constructor]
    cls.add_constructor([])
    ## simple-device-energy-model.h: void ns3::SimpleDeviceEnergyModel::ChangeState(int newState) [member function]
    cls.add_method('ChangeState', 
                   'void', 
                   [param('int', 'newState')], 
                   is_virtual=True)
    ## simple-device-energy-model.h: ns3::Ptr<ns3::Node> ns3::SimpleDeviceEnergyModel::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-device-energy-model.h: double ns3::SimpleDeviceEnergyModel::GetTotalEnergyConsumption() const [member function]
    cls.add_method('GetTotalEnergyConsumption', 
                   'double', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-device-energy-model.h: static ns3::TypeId ns3::SimpleDeviceEnergyModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## simple-device-energy-model.h: void ns3::SimpleDeviceEnergyModel::HandleEnergyDepletion() [member function]
    cls.add_method('HandleEnergyDepletion', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## simple-device-energy-model.h: void ns3::SimpleDeviceEnergyModel::SetCurrentA(double current) [member function]
    cls.add_method('SetCurrentA', 
                   'void', 
                   [param('double', 'current')])
    ## simple-device-energy-model.h: void ns3::SimpleDeviceEnergyModel::SetEnergySource(ns3::Ptr<ns3::EnergySource> source) [member function]
    cls.add_method('SetEnergySource', 
                   'void', 
                   [param('ns3::Ptr< ns3::EnergySource >', 'source')], 
                   is_virtual=True)
    ## simple-device-energy-model.h: void ns3::SimpleDeviceEnergyModel::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## simple-device-energy-model.h: void ns3::SimpleDeviceEnergyModel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## simple-device-energy-model.h: double ns3::SimpleDeviceEnergyModel::DoGetCurrentA() const [member function]
    cls.add_method('DoGetCurrentA', 
                   'double', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3UanChannel_methods(root_module, cls):
    ## uan-channel.h: ns3::UanChannel::UanChannel(ns3::UanChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanChannel const &', 'arg0')])
    ## uan-channel.h: ns3::UanChannel::UanChannel() [constructor]
    cls.add_constructor([])
    ## uan-channel.h: void ns3::UanChannel::AddDevice(ns3::Ptr<ns3::UanNetDevice> dev, ns3::Ptr<ns3::UanTransducer> trans) [member function]
    cls.add_method('AddDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanNetDevice >', 'dev'), param('ns3::Ptr< ns3::UanTransducer >', 'trans')])
    ## uan-channel.h: void ns3::UanChannel::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## uan-channel.h: ns3::Ptr<ns3::NetDevice> ns3::UanChannel::GetDevice(uint32_t i) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## uan-channel.h: uint32_t ns3::UanChannel::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-channel.h: double ns3::UanChannel::GetNoiseDbHz(double fKhz) [member function]
    cls.add_method('GetNoiseDbHz', 
                   'double', 
                   [param('double', 'fKhz')])
    ## uan-channel.h: static ns3::TypeId ns3::UanChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-channel.h: void ns3::UanChannel::SetNoiseModel(ns3::Ptr<ns3::UanNoiseModel> noise) [member function]
    cls.add_method('SetNoiseModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanNoiseModel >', 'noise')])
    ## uan-channel.h: void ns3::UanChannel::SetPropagationModel(ns3::Ptr<ns3::UanPropModel> prop) [member function]
    cls.add_method('SetPropagationModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPropModel >', 'prop')])
    ## uan-channel.h: void ns3::UanChannel::TxPacket(ns3::Ptr<ns3::UanTransducer> src, ns3::Ptr<ns3::Packet> packet, double txPowerDb, ns3::UanTxMode txmode) [member function]
    cls.add_method('TxPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanTransducer >', 'src'), param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'txPowerDb'), param('ns3::UanTxMode', 'txmode')])
    ## uan-channel.h: void ns3::UanChannel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3UanModesListChecker_methods(root_module, cls):
    ## uan-tx-mode.h: ns3::UanModesListChecker::UanModesListChecker() [constructor]
    cls.add_constructor([])
    ## uan-tx-mode.h: ns3::UanModesListChecker::UanModesListChecker(ns3::UanModesListChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanModesListChecker const &', 'arg0')])
    return

def register_Ns3UanModesListValue_methods(root_module, cls):
    ## uan-tx-mode.h: ns3::UanModesListValue::UanModesListValue() [constructor]
    cls.add_constructor([])
    ## uan-tx-mode.h: ns3::UanModesListValue::UanModesListValue(ns3::UanModesListValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanModesListValue const &', 'arg0')])
    ## uan-tx-mode.h: ns3::UanModesListValue::UanModesListValue(ns3::UanModesList const & value) [constructor]
    cls.add_constructor([param('ns3::UanModesList const &', 'value')])
    ## uan-tx-mode.h: ns3::Ptr<ns3::AttributeValue> ns3::UanModesListValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-tx-mode.h: bool ns3::UanModesListValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## uan-tx-mode.h: ns3::UanModesList ns3::UanModesListValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::UanModesList', 
                   [], 
                   is_const=True)
    ## uan-tx-mode.h: std::string ns3::UanModesListValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## uan-tx-mode.h: void ns3::UanModesListValue::Set(ns3::UanModesList const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::UanModesList const &', 'value')])
    return

def register_Ns3UanNetDevice_methods(root_module, cls):
    ## uan-net-device.h: ns3::UanNetDevice::UanNetDevice(ns3::UanNetDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanNetDevice const &', 'arg0')])
    ## uan-net-device.h: ns3::UanNetDevice::UanNetDevice() [constructor]
    cls.add_constructor([])
    ## uan-net-device.h: void ns3::UanNetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## uan-net-device.h: ns3::Address ns3::UanNetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: ns3::Address ns3::UanNetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: ns3::Ptr<ns3::Channel> ns3::UanNetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: uint32_t ns3::UanNetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: ns3::Ptr<ns3::UanMac> ns3::UanNetDevice::GetMac() const [member function]
    cls.add_method('GetMac', 
                   'ns3::Ptr< ns3::UanMac >', 
                   [], 
                   is_const=True)
    ## uan-net-device.h: uint16_t ns3::UanNetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: ns3::Address ns3::UanNetDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: ns3::Address ns3::UanNetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: ns3::Ptr<ns3::Node> ns3::UanNetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: ns3::Ptr<ns3::UanPhy> ns3::UanNetDevice::GetPhy() const [member function]
    cls.add_method('GetPhy', 
                   'ns3::Ptr< ns3::UanPhy >', 
                   [], 
                   is_const=True)
    ## uan-net-device.h: ns3::Ptr<ns3::UanTransducer> ns3::UanNetDevice::GetTransducer() const [member function]
    cls.add_method('GetTransducer', 
                   'ns3::Ptr< ns3::UanTransducer >', 
                   [], 
                   is_const=True)
    ## uan-net-device.h: static ns3::TypeId ns3::UanNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-net-device.h: bool ns3::UanNetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: bool ns3::UanNetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: bool ns3::UanNetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: bool ns3::UanNetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: bool ns3::UanNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: bool ns3::UanNetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: bool ns3::UanNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## uan-net-device.h: bool ns3::UanNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::SetChannel(ns3::Ptr<ns3::UanChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanChannel >', 'channel')])
    ## uan-net-device.h: void ns3::UanNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::SetMac(ns3::Ptr<ns3::UanMac> mac) [member function]
    cls.add_method('SetMac', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanMac >', 'mac')])
    ## uan-net-device.h: bool ns3::UanNetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::SetPhy(ns3::Ptr<ns3::UanPhy> phy) [member function]
    cls.add_method('SetPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'phy')])
    ## uan-net-device.h: void ns3::UanNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::SetTransducer(ns3::Ptr<ns3::UanTransducer> trans) [member function]
    cls.add_method('SetTransducer', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanTransducer >', 'trans')])
    ## uan-net-device.h: bool ns3::UanNetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::ForwardUp(ns3::Ptr<ns3::Packet> pkt, ns3::UanAddress const & src) [member function]
    cls.add_method('ForwardUp', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::UanAddress const &', 'src')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3UeLtePhy_methods(root_module, cls):
    ## ue-phy.h: ns3::UeLtePhy::UeLtePhy(ns3::UeLtePhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UeLtePhy const &', 'arg0')])
    ## ue-phy.h: ns3::UeLtePhy::UeLtePhy() [constructor]
    cls.add_constructor([])
    ## ue-phy.h: ns3::UeLtePhy::UeLtePhy(ns3::Ptr<ns3::LteNetDevice> d) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::LteNetDevice >', 'd')])
    ## ue-phy.h: void ns3::UeLtePhy::CreateCqiFeedbacks(std::vector<double, std::allocator<double> > sinr) [member function]
    cls.add_method('CreateCqiFeedbacks', 
                   'void', 
                   [param('std::vector< double >', 'sinr')])
    ## ue-phy.h: ns3::Ptr<ns3::SpectrumValue> ns3::UeLtePhy::CreateTxPowerSpectralDensity() [member function]
    cls.add_method('CreateTxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [], 
                   is_virtual=True)
    ## ue-phy.h: void ns3::UeLtePhy::DoSetUplinkSubChannels() [member function]
    cls.add_method('DoSetUplinkSubChannels', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ue-phy.h: std::vector<int, std::allocator<int> > ns3::UeLtePhy::GetSubChannelsForReception() [member function]
    cls.add_method('GetSubChannelsForReception', 
                   'std::vector< int >', 
                   [])
    ## ue-phy.h: std::vector<int, std::allocator<int> > ns3::UeLtePhy::GetSubChannelsForTransmission() [member function]
    cls.add_method('GetSubChannelsForTransmission', 
                   'std::vector< int >', 
                   [])
    ## ue-phy.h: static ns3::TypeId ns3::UeLtePhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ue-phy.h: void ns3::UeLtePhy::ReceiveIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('ReceiveIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')], 
                   is_virtual=True)
    ## ue-phy.h: void ns3::UeLtePhy::SendIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('SendIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')], 
                   is_virtual=True)
    ## ue-phy.h: bool ns3::UeLtePhy::SendPacket(ns3::Ptr<ns3::PacketBurst> pb) [member function]
    cls.add_method('SendPacket', 
                   'bool', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'pb')], 
                   is_virtual=True)
    ## ue-phy.h: void ns3::UeLtePhy::SetSubChannelsForReception(std::vector<int, std::allocator<int> > mask) [member function]
    cls.add_method('SetSubChannelsForReception', 
                   'void', 
                   [param('std::vector< int >', 'mask')])
    ## ue-phy.h: void ns3::UeLtePhy::SetSubChannelsForTransmission(std::vector<int, std::allocator<int> > mask) [member function]
    cls.add_method('SetSubChannelsForTransmission', 
                   'void', 
                   [param('std::vector< int >', 'mask')])
    return

def register_Ns3UeLteSpectrumPhy_methods(root_module, cls):
    ## ue-lte-spectrum-phy.h: ns3::UeLteSpectrumPhy::UeLteSpectrumPhy(ns3::UeLteSpectrumPhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UeLteSpectrumPhy const &', 'arg0')])
    ## ue-lte-spectrum-phy.h: ns3::UeLteSpectrumPhy::UeLteSpectrumPhy() [constructor]
    cls.add_constructor([])
    ## ue-lte-spectrum-phy.h: void ns3::UeLteSpectrumPhy::CalcSinrValues(ns3::Ptr<ns3::SpectrumValue const> rxPsd, ns3::Ptr<ns3::SpectrumValue const> noise) [member function]
    cls.add_method('CalcSinrValues', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'rxPsd'), param('ns3::Ptr< ns3::SpectrumValue const >', 'noise')], 
                   is_virtual=True)
    ## ue-lte-spectrum-phy.h: static ns3::TypeId ns3::UeLteSpectrumPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UeMacEntity_methods(root_module, cls):
    ## ue-mac-entity.h: ns3::UeMacEntity::UeMacEntity(ns3::UeMacEntity const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UeMacEntity const &', 'arg0')])
    ## ue-mac-entity.h: ns3::UeMacEntity::UeMacEntity() [constructor]
    cls.add_constructor([])
    ## ue-mac-entity.h: ns3::Ptr<ns3::CqiIdealControlMessage> ns3::UeMacEntity::CreateCqiFeedbacks(std::vector<double, std::allocator<double> > sinr) [member function]
    cls.add_method('CreateCqiFeedbacks', 
                   'ns3::Ptr< ns3::CqiIdealControlMessage >', 
                   [param('std::vector< double >', 'sinr')])
    ## ue-mac-entity.h: static ns3::TypeId ns3::UeMacEntity::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3VirtualNetDevice_methods(root_module, cls):
    ## virtual-net-device.h: ns3::VirtualNetDevice::VirtualNetDevice(ns3::VirtualNetDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::VirtualNetDevice const &', 'arg0')])
    ## virtual-net-device.h: ns3::VirtualNetDevice::VirtualNetDevice() [constructor]
    cls.add_constructor([])
    ## virtual-net-device.h: void ns3::VirtualNetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## virtual-net-device.h: ns3::Address ns3::VirtualNetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h: ns3::Address ns3::VirtualNetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h: ns3::Ptr<ns3::Channel> ns3::VirtualNetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h: uint32_t ns3::VirtualNetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h: uint16_t ns3::VirtualNetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h: ns3::Address ns3::VirtualNetDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h: ns3::Address ns3::VirtualNetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h: ns3::Ptr<ns3::Node> ns3::VirtualNetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h: static ns3::TypeId ns3::VirtualNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## virtual-net-device.h: bool ns3::VirtualNetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h: bool ns3::VirtualNetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h: bool ns3::VirtualNetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h: bool ns3::VirtualNetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h: bool ns3::VirtualNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h: bool ns3::VirtualNetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h: bool ns3::VirtualNetDevice::Receive(ns3::Ptr<ns3::Packet> packet, uint16_t protocol, ns3::Address const & source, ns3::Address const & destination, ns3::NetDevice::PacketType packetType) [member function]
    cls.add_method('Receive', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('uint16_t', 'protocol'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'destination'), param('ns3::NetDevice::PacketType', 'packetType')])
    ## virtual-net-device.h: bool ns3::VirtualNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## virtual-net-device.h: bool ns3::VirtualNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## virtual-net-device.h: void ns3::VirtualNetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_virtual=True)
    ## virtual-net-device.h: void ns3::VirtualNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## virtual-net-device.h: void ns3::VirtualNetDevice::SetIsPointToPoint(bool isPointToPoint) [member function]
    cls.add_method('SetIsPointToPoint', 
                   'void', 
                   [param('bool', 'isPointToPoint')])
    ## virtual-net-device.h: bool ns3::VirtualNetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## virtual-net-device.h: void ns3::VirtualNetDevice::SetNeedsArp(bool needsArp) [member function]
    cls.add_method('SetNeedsArp', 
                   'void', 
                   [param('bool', 'needsArp')])
    ## virtual-net-device.h: void ns3::VirtualNetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## virtual-net-device.h: void ns3::VirtualNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## virtual-net-device.h: void ns3::VirtualNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## virtual-net-device.h: void ns3::VirtualNetDevice::SetSendCallback(ns3::Callback<bool, ns3::Ptr<ns3::Packet>, ns3::Address const&, ns3::Address const&, unsigned short, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> transmitCb) [member function]
    cls.add_method('SetSendCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::Packet >, ns3::Address const &, ns3::Address const &, unsigned short, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'transmitCb')])
    ## virtual-net-device.h: void ns3::VirtualNetDevice::SetSupportsSendFrom(bool supportsSendFrom) [member function]
    cls.add_method('SetSupportsSendFrom', 
                   'void', 
                   [param('bool', 'supportsSendFrom')])
    ## virtual-net-device.h: bool ns3::VirtualNetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h: void ns3::VirtualNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3WifiRadioEnergyModel_methods(root_module, cls):
    ## wifi-radio-energy-model.h: ns3::WifiRadioEnergyModel::WifiRadioEnergyModel(ns3::WifiRadioEnergyModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiRadioEnergyModel const &', 'arg0')])
    ## wifi-radio-energy-model.h: ns3::WifiRadioEnergyModel::WifiRadioEnergyModel() [constructor]
    cls.add_constructor([])
    ## wifi-radio-energy-model.h: void ns3::WifiRadioEnergyModel::ChangeState(int newState) [member function]
    cls.add_method('ChangeState', 
                   'void', 
                   [param('int', 'newState')], 
                   is_virtual=True)
    ## wifi-radio-energy-model.h: double ns3::WifiRadioEnergyModel::GetCcaBusyCurrentA() const [member function]
    cls.add_method('GetCcaBusyCurrentA', 
                   'double', 
                   [], 
                   is_const=True)
    ## wifi-radio-energy-model.h: ns3::WifiPhy::State ns3::WifiRadioEnergyModel::GetCurrentState() const [member function]
    cls.add_method('GetCurrentState', 
                   'ns3::WifiPhy::State', 
                   [], 
                   is_const=True)
    ## wifi-radio-energy-model.h: double ns3::WifiRadioEnergyModel::GetIdleCurrentA() const [member function]
    cls.add_method('GetIdleCurrentA', 
                   'double', 
                   [], 
                   is_const=True)
    ## wifi-radio-energy-model.h: ns3::WifiRadioEnergyModelPhyListener * ns3::WifiRadioEnergyModel::GetPhyListener() [member function]
    cls.add_method('GetPhyListener', 
                   'ns3::WifiRadioEnergyModelPhyListener *', 
                   [])
    ## wifi-radio-energy-model.h: double ns3::WifiRadioEnergyModel::GetRxCurrentA() const [member function]
    cls.add_method('GetRxCurrentA', 
                   'double', 
                   [], 
                   is_const=True)
    ## wifi-radio-energy-model.h: double ns3::WifiRadioEnergyModel::GetSwitchingCurrentA() const [member function]
    cls.add_method('GetSwitchingCurrentA', 
                   'double', 
                   [], 
                   is_const=True)
    ## wifi-radio-energy-model.h: double ns3::WifiRadioEnergyModel::GetTotalEnergyConsumption() const [member function]
    cls.add_method('GetTotalEnergyConsumption', 
                   'double', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-radio-energy-model.h: double ns3::WifiRadioEnergyModel::GetTxCurrentA() const [member function]
    cls.add_method('GetTxCurrentA', 
                   'double', 
                   [], 
                   is_const=True)
    ## wifi-radio-energy-model.h: static ns3::TypeId ns3::WifiRadioEnergyModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wifi-radio-energy-model.h: void ns3::WifiRadioEnergyModel::HandleEnergyDepletion() [member function]
    cls.add_method('HandleEnergyDepletion', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## wifi-radio-energy-model.h: void ns3::WifiRadioEnergyModel::SetCcaBusyCurrentA(double ccaBusyCurrentA) [member function]
    cls.add_method('SetCcaBusyCurrentA', 
                   'void', 
                   [param('double', 'ccaBusyCurrentA')])
    ## wifi-radio-energy-model.h: void ns3::WifiRadioEnergyModel::SetEnergyDepletionCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetEnergyDepletionCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## wifi-radio-energy-model.h: void ns3::WifiRadioEnergyModel::SetEnergySource(ns3::Ptr<ns3::EnergySource> source) [member function]
    cls.add_method('SetEnergySource', 
                   'void', 
                   [param('ns3::Ptr< ns3::EnergySource >', 'source')], 
                   is_virtual=True)
    ## wifi-radio-energy-model.h: void ns3::WifiRadioEnergyModel::SetIdleCurrentA(double idleCurrentA) [member function]
    cls.add_method('SetIdleCurrentA', 
                   'void', 
                   [param('double', 'idleCurrentA')])
    ## wifi-radio-energy-model.h: void ns3::WifiRadioEnergyModel::SetRxCurrentA(double rxCurrentA) [member function]
    cls.add_method('SetRxCurrentA', 
                   'void', 
                   [param('double', 'rxCurrentA')])
    ## wifi-radio-energy-model.h: void ns3::WifiRadioEnergyModel::SetSwitchingCurrentA(double switchingCurrentA) [member function]
    cls.add_method('SetSwitchingCurrentA', 
                   'void', 
                   [param('double', 'switchingCurrentA')])
    ## wifi-radio-energy-model.h: void ns3::WifiRadioEnergyModel::SetTxCurrentA(double txCurrentA) [member function]
    cls.add_method('SetTxCurrentA', 
                   'void', 
                   [param('double', 'txCurrentA')])
    ## wifi-radio-energy-model.h: void ns3::WifiRadioEnergyModel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## wifi-radio-energy-model.h: double ns3::WifiRadioEnergyModel::DoGetCurrentA() const [member function]
    cls.add_method('DoGetCurrentA', 
                   'double', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3AcousticModemEnergyModel_methods(root_module, cls):
    ## acoustic-modem-energy-model.h: ns3::AcousticModemEnergyModel::AcousticModemEnergyModel(ns3::AcousticModemEnergyModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AcousticModemEnergyModel const &', 'arg0')])
    ## acoustic-modem-energy-model.h: ns3::AcousticModemEnergyModel::AcousticModemEnergyModel() [constructor]
    cls.add_constructor([])
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::ChangeState(int newState) [member function]
    cls.add_method('ChangeState', 
                   'void', 
                   [param('int', 'newState')], 
                   is_virtual=True)
    ## acoustic-modem-energy-model.h: int ns3::AcousticModemEnergyModel::GetCurrentState() const [member function]
    cls.add_method('GetCurrentState', 
                   'int', 
                   [], 
                   is_const=True)
    ## acoustic-modem-energy-model.h: double ns3::AcousticModemEnergyModel::GetIdlePowerW() const [member function]
    cls.add_method('GetIdlePowerW', 
                   'double', 
                   [], 
                   is_const=True)
    ## acoustic-modem-energy-model.h: ns3::Ptr<ns3::Node> ns3::AcousticModemEnergyModel::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## acoustic-modem-energy-model.h: double ns3::AcousticModemEnergyModel::GetRxPowerW() const [member function]
    cls.add_method('GetRxPowerW', 
                   'double', 
                   [], 
                   is_const=True)
    ## acoustic-modem-energy-model.h: double ns3::AcousticModemEnergyModel::GetSleepPowerW() const [member function]
    cls.add_method('GetSleepPowerW', 
                   'double', 
                   [], 
                   is_const=True)
    ## acoustic-modem-energy-model.h: double ns3::AcousticModemEnergyModel::GetTotalEnergyConsumption() const [member function]
    cls.add_method('GetTotalEnergyConsumption', 
                   'double', 
                   [], 
                   is_const=True, is_virtual=True)
    ## acoustic-modem-energy-model.h: double ns3::AcousticModemEnergyModel::GetTxPowerW() const [member function]
    cls.add_method('GetTxPowerW', 
                   'double', 
                   [], 
                   is_const=True)
    ## acoustic-modem-energy-model.h: static ns3::TypeId ns3::AcousticModemEnergyModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::HandleEnergyDepletion() [member function]
    cls.add_method('HandleEnergyDepletion', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::SetEnergyDepletionCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetEnergyDepletionCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::SetEnergySource(ns3::Ptr<ns3::EnergySource> source) [member function]
    cls.add_method('SetEnergySource', 
                   'void', 
                   [param('ns3::Ptr< ns3::EnergySource >', 'source')], 
                   is_virtual=True)
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::SetIdlePowerW(double idlePowerW) [member function]
    cls.add_method('SetIdlePowerW', 
                   'void', 
                   [param('double', 'idlePowerW')])
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::SetRxPowerW(double rxPowerW) [member function]
    cls.add_method('SetRxPowerW', 
                   'void', 
                   [param('double', 'rxPowerW')])
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::SetSleepPowerW(double sleepPowerW) [member function]
    cls.add_method('SetSleepPowerW', 
                   'void', 
                   [param('double', 'sleepPowerW')])
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::SetTxPowerW(double txPowerW) [member function]
    cls.add_method('SetTxPowerW', 
                   'void', 
                   [param('double', 'txPowerW')])
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## acoustic-modem-energy-model.h: double ns3::AcousticModemEnergyModel::DoGetCurrentA() const [member function]
    cls.add_method('DoGetCurrentA', 
                   'double', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3BasicEnergySource_methods(root_module, cls):
    ## basic-energy-source.h: ns3::BasicEnergySource::BasicEnergySource(ns3::BasicEnergySource const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BasicEnergySource const &', 'arg0')])
    ## basic-energy-source.h: ns3::BasicEnergySource::BasicEnergySource() [constructor]
    cls.add_constructor([])
    ## basic-energy-source.h: double ns3::BasicEnergySource::GetEnergyFraction() [member function]
    cls.add_method('GetEnergyFraction', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## basic-energy-source.h: ns3::Time ns3::BasicEnergySource::GetEnergyUpdateInterval() const [member function]
    cls.add_method('GetEnergyUpdateInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## basic-energy-source.h: double ns3::BasicEnergySource::GetInitialEnergy() const [member function]
    cls.add_method('GetInitialEnergy', 
                   'double', 
                   [], 
                   is_const=True, is_virtual=True)
    ## basic-energy-source.h: double ns3::BasicEnergySource::GetRemainingEnergy() [member function]
    cls.add_method('GetRemainingEnergy', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## basic-energy-source.h: double ns3::BasicEnergySource::GetSupplyVoltage() const [member function]
    cls.add_method('GetSupplyVoltage', 
                   'double', 
                   [], 
                   is_const=True, is_virtual=True)
    ## basic-energy-source.h: static ns3::TypeId ns3::BasicEnergySource::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## basic-energy-source.h: void ns3::BasicEnergySource::SetEnergyUpdateInterval(ns3::Time interval) [member function]
    cls.add_method('SetEnergyUpdateInterval', 
                   'void', 
                   [param('ns3::Time', 'interval')])
    ## basic-energy-source.h: void ns3::BasicEnergySource::SetInitialEnergy(double initialEnergyJ) [member function]
    cls.add_method('SetInitialEnergy', 
                   'void', 
                   [param('double', 'initialEnergyJ')])
    ## basic-energy-source.h: void ns3::BasicEnergySource::SetSupplyVoltage(double supplyVoltageV) [member function]
    cls.add_method('SetSupplyVoltage', 
                   'void', 
                   [param('double', 'supplyVoltageV')])
    ## basic-energy-source.h: void ns3::BasicEnergySource::UpdateEnergySource() [member function]
    cls.add_method('UpdateEnergySource', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## basic-energy-source.h: void ns3::BasicEnergySource::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## basic-energy-source.h: void ns3::BasicEnergySource::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3BridgeChannel_methods(root_module, cls):
    ## bridge-channel.h: ns3::BridgeChannel::BridgeChannel(ns3::BridgeChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BridgeChannel const &', 'arg0')])
    ## bridge-channel.h: ns3::BridgeChannel::BridgeChannel() [constructor]
    cls.add_constructor([])
    ## bridge-channel.h: void ns3::BridgeChannel::AddChannel(ns3::Ptr<ns3::Channel> bridgedChannel) [member function]
    cls.add_method('AddChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::Channel >', 'bridgedChannel')])
    ## bridge-channel.h: ns3::Ptr<ns3::NetDevice> ns3::BridgeChannel::GetDevice(uint32_t i) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## bridge-channel.h: uint32_t ns3::BridgeChannel::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bridge-channel.h: static ns3::TypeId ns3::BridgeChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3BridgeNetDevice_methods(root_module, cls):
    ## bridge-net-device.h: ns3::BridgeNetDevice::BridgeNetDevice(ns3::BridgeNetDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BridgeNetDevice const &', 'arg0')])
    ## bridge-net-device.h: ns3::BridgeNetDevice::BridgeNetDevice() [constructor]
    cls.add_constructor([])
    ## bridge-net-device.h: void ns3::BridgeNetDevice::AddBridgePort(ns3::Ptr<ns3::NetDevice> bridgePort) [member function]
    cls.add_method('AddBridgePort', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'bridgePort')])
    ## bridge-net-device.h: void ns3::BridgeNetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## bridge-net-device.h: ns3::Address ns3::BridgeNetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bridge-net-device.h: ns3::Ptr<ns3::NetDevice> ns3::BridgeNetDevice::GetBridgePort(uint32_t n) const [member function]
    cls.add_method('GetBridgePort', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'n')], 
                   is_const=True)
    ## bridge-net-device.h: ns3::Address ns3::BridgeNetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bridge-net-device.h: ns3::Ptr<ns3::Channel> ns3::BridgeNetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bridge-net-device.h: uint32_t ns3::BridgeNetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bridge-net-device.h: uint16_t ns3::BridgeNetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bridge-net-device.h: ns3::Address ns3::BridgeNetDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## bridge-net-device.h: ns3::Address ns3::BridgeNetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## bridge-net-device.h: uint32_t ns3::BridgeNetDevice::GetNBridgePorts() const [member function]
    cls.add_method('GetNBridgePorts', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## bridge-net-device.h: ns3::Ptr<ns3::Node> ns3::BridgeNetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bridge-net-device.h: static ns3::TypeId ns3::BridgeNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## bridge-net-device.h: bool ns3::BridgeNetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bridge-net-device.h: bool ns3::BridgeNetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bridge-net-device.h: bool ns3::BridgeNetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bridge-net-device.h: bool ns3::BridgeNetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bridge-net-device.h: bool ns3::BridgeNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bridge-net-device.h: bool ns3::BridgeNetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bridge-net-device.h: bool ns3::BridgeNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## bridge-net-device.h: bool ns3::BridgeNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## bridge-net-device.h: void ns3::BridgeNetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_virtual=True)
    ## bridge-net-device.h: void ns3::BridgeNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## bridge-net-device.h: bool ns3::BridgeNetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## bridge-net-device.h: void ns3::BridgeNetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## bridge-net-device.h: void ns3::BridgeNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## bridge-net-device.h: void ns3::BridgeNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## bridge-net-device.h: bool ns3::BridgeNetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bridge-net-device.h: void ns3::BridgeNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## bridge-net-device.h: void ns3::BridgeNetDevice::ForwardBroadcast(ns3::Ptr<ns3::NetDevice> incomingPort, ns3::Ptr<ns3::Packet const> packet, uint16_t protocol, ns3::Mac48Address src, ns3::Mac48Address dst) [member function]
    cls.add_method('ForwardBroadcast', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'incomingPort'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint16_t', 'protocol'), param('ns3::Mac48Address', 'src'), param('ns3::Mac48Address', 'dst')], 
                   visibility='protected')
    ## bridge-net-device.h: void ns3::BridgeNetDevice::ForwardUnicast(ns3::Ptr<ns3::NetDevice> incomingPort, ns3::Ptr<ns3::Packet const> packet, uint16_t protocol, ns3::Mac48Address src, ns3::Mac48Address dst) [member function]
    cls.add_method('ForwardUnicast', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'incomingPort'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint16_t', 'protocol'), param('ns3::Mac48Address', 'src'), param('ns3::Mac48Address', 'dst')], 
                   visibility='protected')
    ## bridge-net-device.h: ns3::Ptr<ns3::NetDevice> ns3::BridgeNetDevice::GetLearnedState(ns3::Mac48Address source) [member function]
    cls.add_method('GetLearnedState', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('ns3::Mac48Address', 'source')], 
                   visibility='protected')
    ## bridge-net-device.h: void ns3::BridgeNetDevice::Learn(ns3::Mac48Address source, ns3::Ptr<ns3::NetDevice> port) [member function]
    cls.add_method('Learn', 
                   'void', 
                   [param('ns3::Mac48Address', 'source'), param('ns3::Ptr< ns3::NetDevice >', 'port')], 
                   visibility='protected')
    ## bridge-net-device.h: void ns3::BridgeNetDevice::ReceiveFromDevice(ns3::Ptr<ns3::NetDevice> device, ns3::Ptr<ns3::Packet const> packet, uint16_t protocol, ns3::Address const & source, ns3::Address const & destination, ns3::NetDevice::PacketType packetType) [member function]
    cls.add_method('ReceiveFromDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint16_t', 'protocol'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'destination'), param('ns3::NetDevice::PacketType', 'packetType')], 
                   visibility='protected')
    return

def register_Ns3CqiIdealControlMessage_methods(root_module, cls):
    ## ideal-control-messages.h: ns3::CqiIdealControlMessage::CqiIdealControlMessage(ns3::CqiIdealControlMessage const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CqiIdealControlMessage const &', 'arg0')])
    ## ideal-control-messages.h: ns3::CqiIdealControlMessage::CqiIdealControlMessage() [constructor]
    cls.add_constructor([])
    ## ideal-control-messages.h: void ns3::CqiIdealControlMessage::AddNewRecord(int subChannel, double cqi) [member function]
    cls.add_method('AddNewRecord', 
                   'void', 
                   [param('int', 'subChannel'), param('double', 'cqi')])
    ## ideal-control-messages.h: std::list<ns3::CqiIdealControlMessage::CqiFeedback,std::allocator<ns3::CqiIdealControlMessage::CqiFeedback> > * ns3::CqiIdealControlMessage::GetMessage() [member function]
    cls.add_method('GetMessage', 
                   'std::list< ns3::CqiIdealControlMessage::CqiFeedback > *', 
                   [])
    return

def register_Ns3CqiIdealControlMessageCqiFeedback_methods(root_module, cls):
    ## ideal-control-messages.h: ns3::CqiIdealControlMessage::CqiFeedback::CqiFeedback() [constructor]
    cls.add_constructor([])
    ## ideal-control-messages.h: ns3::CqiIdealControlMessage::CqiFeedback::CqiFeedback(ns3::CqiIdealControlMessage::CqiFeedback const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CqiIdealControlMessage::CqiFeedback const &', 'arg0')])
    ## ideal-control-messages.h: ns3::CqiIdealControlMessage::CqiFeedback::m_cqi [variable]
    cls.add_instance_attribute('m_cqi', 'double', is_const=False)
    ## ideal-control-messages.h: ns3::CqiIdealControlMessage::CqiFeedback::m_idSubChannel [variable]
    cls.add_instance_attribute('m_idSubChannel', 'int', is_const=False)
    return

def register_Ns3EnbLtePhy_methods(root_module, cls):
    ## enb-phy.h: ns3::EnbLtePhy::EnbLtePhy(ns3::EnbLtePhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EnbLtePhy const &', 'arg0')])
    ## enb-phy.h: ns3::EnbLtePhy::EnbLtePhy() [constructor]
    cls.add_constructor([])
    ## enb-phy.h: ns3::EnbLtePhy::EnbLtePhy(ns3::Ptr<ns3::LteNetDevice> d) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::LteNetDevice >', 'd')])
    ## enb-phy.h: void ns3::EnbLtePhy::CalcChannelQualityForUe(std::vector<double, std::allocator<double> > sinr, ns3::Ptr<ns3::LteSpectrumPhy> ue) [member function]
    cls.add_method('CalcChannelQualityForUe', 
                   'void', 
                   [param('std::vector< double >', 'sinr'), param('ns3::Ptr< ns3::LteSpectrumPhy >', 'ue')])
    ## enb-phy.h: ns3::Ptr<ns3::SpectrumValue> ns3::EnbLtePhy::CreateTxPowerSpectralDensity() [member function]
    cls.add_method('CreateTxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [], 
                   is_virtual=True)
    ## enb-phy.h: void ns3::EnbLtePhy::DoSetDownlinkSubChannels() [member function]
    cls.add_method('DoSetDownlinkSubChannels', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## enb-phy.h: void ns3::EnbLtePhy::EndFrame() [member function]
    cls.add_method('EndFrame', 
                   'void', 
                   [])
    ## enb-phy.h: void ns3::EnbLtePhy::EndSubFrame() [member function]
    cls.add_method('EndSubFrame', 
                   'void', 
                   [])
    ## enb-phy.h: static ns3::TypeId ns3::EnbLtePhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## enb-phy.h: void ns3::EnbLtePhy::ReceiveIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('ReceiveIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')], 
                   is_virtual=True)
    ## enb-phy.h: void ns3::EnbLtePhy::SendIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('SendIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')], 
                   is_virtual=True)
    ## enb-phy.h: bool ns3::EnbLtePhy::SendPacket(ns3::Ptr<ns3::PacketBurst> pb) [member function]
    cls.add_method('SendPacket', 
                   'bool', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'pb')], 
                   is_virtual=True)
    ## enb-phy.h: void ns3::EnbLtePhy::StartFrame() [member function]
    cls.add_method('StartFrame', 
                   'void', 
                   [])
    ## enb-phy.h: void ns3::EnbLtePhy::StartSubFrame() [member function]
    cls.add_method('StartSubFrame', 
                   'void', 
                   [])
    return

def register_Ns3EnbLteSpectrumPhy_methods(root_module, cls):
    ## enb-lte-spectrum-phy.h: ns3::EnbLteSpectrumPhy::EnbLteSpectrumPhy(ns3::EnbLteSpectrumPhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EnbLteSpectrumPhy const &', 'arg0')])
    ## enb-lte-spectrum-phy.h: ns3::EnbLteSpectrumPhy::EnbLteSpectrumPhy() [constructor]
    cls.add_constructor([])
    ## enb-lte-spectrum-phy.h: void ns3::EnbLteSpectrumPhy::CalcSinrValues(ns3::Ptr<ns3::SpectrumValue const> rxPsd, ns3::Ptr<ns3::SpectrumValue const> noise) [member function]
    cls.add_method('CalcSinrValues', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'rxPsd'), param('ns3::Ptr< ns3::SpectrumValue const >', 'noise')], 
                   is_virtual=True)
    ## enb-lte-spectrum-phy.h: static ns3::TypeId ns3::EnbLteSpectrumPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3EnbMacEntity_methods(root_module, cls):
    ## enb-mac-entity.h: ns3::EnbMacEntity::EnbMacEntity(ns3::EnbMacEntity const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EnbMacEntity const &', 'arg0')])
    ## enb-mac-entity.h: ns3::EnbMacEntity::EnbMacEntity() [constructor]
    cls.add_constructor([])
    ## enb-mac-entity.h: void ns3::EnbMacEntity::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## enb-mac-entity.h: ns3::Ptr<ns3::PacketScheduler> ns3::EnbMacEntity::GetDownlinkPacketScheduler() [member function]
    cls.add_method('GetDownlinkPacketScheduler', 
                   'ns3::Ptr< ns3::PacketScheduler >', 
                   [])
    ## enb-mac-entity.h: static ns3::TypeId ns3::EnbMacEntity::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## enb-mac-entity.h: ns3::Ptr<ns3::PacketScheduler> ns3::EnbMacEntity::GetUplinkPacketScheduler() [member function]
    cls.add_method('GetUplinkPacketScheduler', 
                   'ns3::Ptr< ns3::PacketScheduler >', 
                   [])
    ## enb-mac-entity.h: void ns3::EnbMacEntity::ReceiveCqiIdealControlMessage(ns3::Ptr<ns3::CqiIdealControlMessage> msg) [member function]
    cls.add_method('ReceiveCqiIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::CqiIdealControlMessage >', 'msg')])
    ## enb-mac-entity.h: void ns3::EnbMacEntity::SendPdcchMapIdealControlMessage(ns3::Ptr<ns3::PdcchMapIdealControlMessage> msg) [member function]
    cls.add_method('SendPdcchMapIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::PdcchMapIdealControlMessage >', 'msg')])
    ## enb-mac-entity.h: void ns3::EnbMacEntity::SetDownlinkPacketScheduler(ns3::Ptr<ns3::PacketScheduler> s) [member function]
    cls.add_method('SetDownlinkPacketScheduler', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketScheduler >', 's')])
    ## enb-mac-entity.h: void ns3::EnbMacEntity::SetUplinkPacketScheduler(ns3::Ptr<ns3::PacketScheduler> s) [member function]
    cls.add_method('SetUplinkPacketScheduler', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketScheduler >', 's')])
    return

def register_Ns3Ipv4GlobalRouting_methods(root_module, cls):
    ## ipv4-global-routing.h: ns3::Ipv4GlobalRouting::Ipv4GlobalRouting(ns3::Ipv4GlobalRouting const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4GlobalRouting const &', 'arg0')])
    ## ipv4-global-routing.h: ns3::Ipv4GlobalRouting::Ipv4GlobalRouting() [constructor]
    cls.add_constructor([])
    ## ipv4-global-routing.h: void ns3::Ipv4GlobalRouting::AddASExternalRouteTo(ns3::Ipv4Address network, ns3::Ipv4Mask networkMask, ns3::Ipv4Address nextHop, uint32_t interface) [member function]
    cls.add_method('AddASExternalRouteTo', 
                   'void', 
                   [param('ns3::Ipv4Address', 'network'), param('ns3::Ipv4Mask', 'networkMask'), param('ns3::Ipv4Address', 'nextHop'), param('uint32_t', 'interface')])
    ## ipv4-global-routing.h: void ns3::Ipv4GlobalRouting::AddHostRouteTo(ns3::Ipv4Address dest, ns3::Ipv4Address nextHop, uint32_t interface) [member function]
    cls.add_method('AddHostRouteTo', 
                   'void', 
                   [param('ns3::Ipv4Address', 'dest'), param('ns3::Ipv4Address', 'nextHop'), param('uint32_t', 'interface')])
    ## ipv4-global-routing.h: void ns3::Ipv4GlobalRouting::AddHostRouteTo(ns3::Ipv4Address dest, uint32_t interface) [member function]
    cls.add_method('AddHostRouteTo', 
                   'void', 
                   [param('ns3::Ipv4Address', 'dest'), param('uint32_t', 'interface')])
    ## ipv4-global-routing.h: void ns3::Ipv4GlobalRouting::AddNetworkRouteTo(ns3::Ipv4Address network, ns3::Ipv4Mask networkMask, ns3::Ipv4Address nextHop, uint32_t interface) [member function]
    cls.add_method('AddNetworkRouteTo', 
                   'void', 
                   [param('ns3::Ipv4Address', 'network'), param('ns3::Ipv4Mask', 'networkMask'), param('ns3::Ipv4Address', 'nextHop'), param('uint32_t', 'interface')])
    ## ipv4-global-routing.h: void ns3::Ipv4GlobalRouting::AddNetworkRouteTo(ns3::Ipv4Address network, ns3::Ipv4Mask networkMask, uint32_t interface) [member function]
    cls.add_method('AddNetworkRouteTo', 
                   'void', 
                   [param('ns3::Ipv4Address', 'network'), param('ns3::Ipv4Mask', 'networkMask'), param('uint32_t', 'interface')])
    ## ipv4-global-routing.h: uint32_t ns3::Ipv4GlobalRouting::GetNRoutes() const [member function]
    cls.add_method('GetNRoutes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-global-routing.h: ns3::Ipv4RoutingTableEntry * ns3::Ipv4GlobalRouting::GetRoute(uint32_t i) const [member function]
    cls.add_method('GetRoute', 
                   'ns3::Ipv4RoutingTableEntry *', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## ipv4-global-routing.h: static ns3::TypeId ns3::Ipv4GlobalRouting::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv4-global-routing.h: void ns3::Ipv4GlobalRouting::NotifyAddAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyAddAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## ipv4-global-routing.h: void ns3::Ipv4GlobalRouting::NotifyInterfaceDown(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceDown', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## ipv4-global-routing.h: void ns3::Ipv4GlobalRouting::NotifyInterfaceUp(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceUp', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## ipv4-global-routing.h: void ns3::Ipv4GlobalRouting::NotifyRemoveAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyRemoveAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## ipv4-global-routing.h: void ns3::Ipv4GlobalRouting::PrintRoutingTable(ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('PrintRoutingTable', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_const=True, is_virtual=True)
    ## ipv4-global-routing.h: void ns3::Ipv4GlobalRouting::RemoveRoute(uint32_t i) [member function]
    cls.add_method('RemoveRoute', 
                   'void', 
                   [param('uint32_t', 'i')])
    ## ipv4-global-routing.h: bool ns3::Ipv4GlobalRouting::RouteInput(ns3::Ptr<ns3::Packet const> p, ns3::Ipv4Header const & header, ns3::Ptr<const ns3::NetDevice> idev, ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ucb, ns3::Callback<void,ns3::Ptr<ns3::Ipv4MulticastRoute>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> mcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,unsigned int,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> lcb, ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ecb) [member function]
    cls.add_method('RouteInput', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice const >', 'idev'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4MulticastRoute >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'mcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'lcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb')], 
                   is_virtual=True)
    ## ipv4-global-routing.h: ns3::Ptr<ns3::Ipv4Route> ns3::Ipv4GlobalRouting::RouteOutput(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Header const & header, ns3::Ptr<ns3::NetDevice> oif, ns3::Socket::SocketErrno & sockerr) [member function]
    cls.add_method('RouteOutput', 
                   'ns3::Ptr< ns3::Ipv4Route >', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice >', 'oif'), param('ns3::Socket::SocketErrno &', 'sockerr')], 
                   is_virtual=True)
    ## ipv4-global-routing.h: void ns3::Ipv4GlobalRouting::SetIpv4(ns3::Ptr<ns3::Ipv4> ipv4) [member function]
    cls.add_method('SetIpv4', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4 >', 'ipv4')], 
                   is_virtual=True)
    ## ipv4-global-routing.h: void ns3::Ipv4GlobalRouting::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3Ipv4ListRouting_methods(root_module, cls):
    ## ipv4-list-routing.h: ns3::Ipv4ListRouting::Ipv4ListRouting(ns3::Ipv4ListRouting const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4ListRouting const &', 'arg0')])
    ## ipv4-list-routing.h: ns3::Ipv4ListRouting::Ipv4ListRouting() [constructor]
    cls.add_constructor([])
    ## ipv4-list-routing.h: void ns3::Ipv4ListRouting::AddRoutingProtocol(ns3::Ptr<ns3::Ipv4RoutingProtocol> routingProtocol, int16_t priority) [member function]
    cls.add_method('AddRoutingProtocol', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4RoutingProtocol >', 'routingProtocol'), param('int16_t', 'priority')], 
                   is_virtual=True)
    ## ipv4-list-routing.h: uint32_t ns3::Ipv4ListRouting::GetNRoutingProtocols() const [member function]
    cls.add_method('GetNRoutingProtocols', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-list-routing.h: ns3::Ptr<ns3::Ipv4RoutingProtocol> ns3::Ipv4ListRouting::GetRoutingProtocol(uint32_t index, int16_t & priority) const [member function]
    cls.add_method('GetRoutingProtocol', 
                   'ns3::Ptr< ns3::Ipv4RoutingProtocol >', 
                   [param('uint32_t', 'index'), param('int16_t &', 'priority', direction=2)], 
                   is_const=True, is_virtual=True)
    ## ipv4-list-routing.h: static ns3::TypeId ns3::Ipv4ListRouting::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv4-list-routing.h: void ns3::Ipv4ListRouting::NotifyAddAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyAddAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## ipv4-list-routing.h: void ns3::Ipv4ListRouting::NotifyInterfaceDown(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceDown', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## ipv4-list-routing.h: void ns3::Ipv4ListRouting::NotifyInterfaceUp(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceUp', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## ipv4-list-routing.h: void ns3::Ipv4ListRouting::NotifyRemoveAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyRemoveAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## ipv4-list-routing.h: void ns3::Ipv4ListRouting::PrintRoutingTable(ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('PrintRoutingTable', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_const=True, is_virtual=True)
    ## ipv4-list-routing.h: bool ns3::Ipv4ListRouting::RouteInput(ns3::Ptr<ns3::Packet const> p, ns3::Ipv4Header const & header, ns3::Ptr<const ns3::NetDevice> idev, ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ucb, ns3::Callback<void,ns3::Ptr<ns3::Ipv4MulticastRoute>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> mcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,unsigned int,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> lcb, ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ecb) [member function]
    cls.add_method('RouteInput', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice const >', 'idev'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4MulticastRoute >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'mcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'lcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb')], 
                   is_virtual=True)
    ## ipv4-list-routing.h: ns3::Ptr<ns3::Ipv4Route> ns3::Ipv4ListRouting::RouteOutput(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Header const & header, ns3::Ptr<ns3::NetDevice> oif, ns3::Socket::SocketErrno & sockerr) [member function]
    cls.add_method('RouteOutput', 
                   'ns3::Ptr< ns3::Ipv4Route >', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice >', 'oif'), param('ns3::Socket::SocketErrno &', 'sockerr')], 
                   is_virtual=True)
    ## ipv4-list-routing.h: void ns3::Ipv4ListRouting::SetIpv4(ns3::Ptr<ns3::Ipv4> ipv4) [member function]
    cls.add_method('SetIpv4', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4 >', 'ipv4')], 
                   is_virtual=True)
    ## ipv4-list-routing.h: void ns3::Ipv4ListRouting::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ipv4-list-routing.h: void ns3::Ipv4ListRouting::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3Ipv6ListRouting_methods(root_module, cls):
    ## ipv6-list-routing.h: ns3::Ipv6ListRouting::Ipv6ListRouting(ns3::Ipv6ListRouting const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6ListRouting const &', 'arg0')])
    ## ipv6-list-routing.h: ns3::Ipv6ListRouting::Ipv6ListRouting() [constructor]
    cls.add_constructor([])
    ## ipv6-list-routing.h: void ns3::Ipv6ListRouting::AddRoutingProtocol(ns3::Ptr<ns3::Ipv6RoutingProtocol> routingProtocol, int16_t priority) [member function]
    cls.add_method('AddRoutingProtocol', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv6RoutingProtocol >', 'routingProtocol'), param('int16_t', 'priority')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: uint32_t ns3::Ipv6ListRouting::GetNRoutingProtocols() const [member function]
    cls.add_method('GetNRoutingProtocols', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-list-routing.h: ns3::Ptr<ns3::Ipv6RoutingProtocol> ns3::Ipv6ListRouting::GetRoutingProtocol(uint32_t index, int16_t & priority) const [member function]
    cls.add_method('GetRoutingProtocol', 
                   'ns3::Ptr< ns3::Ipv6RoutingProtocol >', 
                   [param('uint32_t', 'index'), param('int16_t &', 'priority')], 
                   is_const=True, is_virtual=True)
    ## ipv6-list-routing.h: static ns3::TypeId ns3::Ipv6ListRouting::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-list-routing.h: void ns3::Ipv6ListRouting::NotifyAddAddress(uint32_t interface, ns3::Ipv6InterfaceAddress address) [member function]
    cls.add_method('NotifyAddAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv6InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: void ns3::Ipv6ListRouting::NotifyAddRoute(ns3::Ipv6Address dst, ns3::Ipv6Prefix mask, ns3::Ipv6Address nextHop, uint32_t interface, ns3::Ipv6Address prefixToUse=ns3::Ipv6Address::GetZero( )) [member function]
    cls.add_method('NotifyAddRoute', 
                   'void', 
                   [param('ns3::Ipv6Address', 'dst'), param('ns3::Ipv6Prefix', 'mask'), param('ns3::Ipv6Address', 'nextHop'), param('uint32_t', 'interface'), param('ns3::Ipv6Address', 'prefixToUse', default_value='ns3::Ipv6Address::GetZero( )')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: void ns3::Ipv6ListRouting::NotifyInterfaceDown(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceDown', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: void ns3::Ipv6ListRouting::NotifyInterfaceUp(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceUp', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: void ns3::Ipv6ListRouting::NotifyRemoveAddress(uint32_t interface, ns3::Ipv6InterfaceAddress address) [member function]
    cls.add_method('NotifyRemoveAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv6InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: void ns3::Ipv6ListRouting::NotifyRemoveRoute(ns3::Ipv6Address dst, ns3::Ipv6Prefix mask, ns3::Ipv6Address nextHop, uint32_t interface, ns3::Ipv6Address prefixToUse=ns3::Ipv6Address::GetZero( )) [member function]
    cls.add_method('NotifyRemoveRoute', 
                   'void', 
                   [param('ns3::Ipv6Address', 'dst'), param('ns3::Ipv6Prefix', 'mask'), param('ns3::Ipv6Address', 'nextHop'), param('uint32_t', 'interface'), param('ns3::Ipv6Address', 'prefixToUse', default_value='ns3::Ipv6Address::GetZero( )')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: bool ns3::Ipv6ListRouting::RouteInput(ns3::Ptr<ns3::Packet const> p, ns3::Ipv6Header const & header, ns3::Ptr<const ns3::NetDevice> idev, ns3::Callback<void,ns3::Ptr<ns3::Ipv6Route>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv6Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> ucb, ns3::Callback<void,ns3::Ptr<ns3::Ipv6MulticastRoute>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv6Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> mcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv6Header&,unsigned int,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> lcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv6Header&,ns3::Socket::SocketErrno,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> ecb) [member function]
    cls.add_method('RouteInput', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ipv6Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice const >', 'idev'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv6Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv6Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv6MulticastRoute >, ns3::Ptr< ns3::Packet const >, ns3::Ipv6Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'mcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv6Header const &, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'lcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv6Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: ns3::Ptr<ns3::Ipv6Route> ns3::Ipv6ListRouting::RouteOutput(ns3::Ptr<ns3::Packet> p, ns3::Ipv6Header const & header, ns3::Ptr<ns3::NetDevice> oif, ns3::Socket::SocketErrno & sockerr) [member function]
    cls.add_method('RouteOutput', 
                   'ns3::Ptr< ns3::Ipv6Route >', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv6Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice >', 'oif'), param('ns3::Socket::SocketErrno &', 'sockerr')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: void ns3::Ipv6ListRouting::SetIpv6(ns3::Ptr<ns3::Ipv6> ipv6) [member function]
    cls.add_method('SetIpv6', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv6 >', 'ipv6')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: void ns3::Ipv6ListRouting::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3LteNetDevice_methods(root_module, cls):
    ## lte-net-device.h: static ns3::TypeId ns3::LteNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-net-device.h: ns3::LteNetDevice::LteNetDevice() [constructor]
    cls.add_constructor([])
    ## lte-net-device.h: void ns3::LteNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-net-device.h: void ns3::LteNetDevice::SetPhyMacTxStartCallback(ns3::PhyMacTxStartCallback c) [member function]
    cls.add_method('SetPhyMacTxStartCallback', 
                   'void', 
                   [param('ns3::PhyMacTxStartCallback', 'c')])
    ## lte-net-device.h: void ns3::LteNetDevice::SetPhy(ns3::Ptr<ns3::LtePhy> phy) [member function]
    cls.add_method('SetPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::LtePhy >', 'phy')])
    ## lte-net-device.h: ns3::Ptr<ns3::LtePhy> ns3::LteNetDevice::GetPhy() const [member function]
    cls.add_method('GetPhy', 
                   'ns3::Ptr< ns3::LtePhy >', 
                   [], 
                   is_const=True)
    ## lte-net-device.h: void ns3::LteNetDevice::SetRrcEntity(ns3::Ptr<ns3::RrcEntity> rrc) [member function]
    cls.add_method('SetRrcEntity', 
                   'void', 
                   [param('ns3::Ptr< ns3::RrcEntity >', 'rrc')])
    ## lte-net-device.h: ns3::Ptr<ns3::RrcEntity> ns3::LteNetDevice::GetRrcEntity() [member function]
    cls.add_method('GetRrcEntity', 
                   'ns3::Ptr< ns3::RrcEntity >', 
                   [])
    ## lte-net-device.h: void ns3::LteNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## lte-net-device.h: uint32_t ns3::LteNetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h: ns3::Ptr<ns3::Channel> ns3::LteNetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h: bool ns3::LteNetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## lte-net-device.h: uint16_t ns3::LteNetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h: void ns3::LteNetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_virtual=True)
    ## lte-net-device.h: ns3::Address ns3::LteNetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h: bool ns3::LteNetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h: void ns3::LteNetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## lte-net-device.h: bool ns3::LteNetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h: ns3::Address ns3::LteNetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h: bool ns3::LteNetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h: bool ns3::LteNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h: bool ns3::LteNetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h: ns3::Ptr<ns3::Node> ns3::LteNetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h: void ns3::LteNetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## lte-net-device.h: bool ns3::LteNetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h: void ns3::LteNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## lte-net-device.h: ns3::Address ns3::LteNetDevice::GetMulticast(ns3::Ipv4Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h: ns3::Address ns3::LteNetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h: void ns3::LteNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## lte-net-device.h: void ns3::LteNetDevice::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-net-device.h: void ns3::LteNetDevice::Stop() [member function]
    cls.add_method('Stop', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-net-device.h: bool ns3::LteNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## lte-net-device.h: bool ns3::LteNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## lte-net-device.h: bool ns3::LteNetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h: void ns3::LteNetDevice::Receive(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## lte-net-device.h: void ns3::LteNetDevice::ForwardUp(ns3::Ptr<ns3::Packet> packet, ns3::Mac48Address const & source, ns3::Mac48Address const & dest) [member function]
    cls.add_method('ForwardUp', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Mac48Address const &', 'source'), param('ns3::Mac48Address const &', 'dest')])
    ## lte-net-device.h: void ns3::LteNetDevice::ForwardUp(ns3::Ptr<ns3::Packet> packet) [member function]
    cls.add_method('ForwardUp', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet')])
    ## lte-net-device.h: void ns3::LteNetDevice::SetPacketToSend(ns3::Ptr<ns3::PacketBurst> p) [member function]
    cls.add_method('SetPacketToSend', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p')])
    ## lte-net-device.h: ns3::Ptr<ns3::PacketBurst> ns3::LteNetDevice::GetPacketToSend() [member function]
    cls.add_method('GetPacketToSend', 
                   'ns3::Ptr< ns3::PacketBurst >', 
                   [])
    ## lte-net-device.h: void ns3::LteNetDevice::StartTransmission() [member function]
    cls.add_method('StartTransmission', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-net-device.h: bool ns3::LteNetDevice::SendPacket(ns3::Ptr<ns3::PacketBurst> p) [member function]
    cls.add_method('SendPacket', 
                   'bool', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-net-device.h: bool ns3::LteNetDevice::DoSend(ns3::Ptr<ns3::Packet> packet, ns3::Mac48Address const & source, ns3::Mac48Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('DoSend', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Mac48Address const &', 'source'), param('ns3::Mac48Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## lte-net-device.h: void ns3::LteNetDevice::DoReceive(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoReceive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    return

def register_Ns3UeNetDevice_methods(root_module, cls):
    ## ue-net-device.h: static ns3::TypeId ns3::UeNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ue-net-device.h: ns3::UeNetDevice::UeNetDevice() [constructor]
    cls.add_constructor([])
    ## ue-net-device.h: ns3::UeNetDevice::UeNetDevice(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::LtePhy> phy) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::LtePhy >', 'phy')])
    ## ue-net-device.h: ns3::UeNetDevice::UeNetDevice(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::LtePhy> phy, ns3::Ptr<ns3::EnbNetDevice> targetEnb) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::LtePhy >', 'phy'), param('ns3::Ptr< ns3::EnbNetDevice >', 'targetEnb')])
    ## ue-net-device.h: void ns3::UeNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ue-net-device.h: void ns3::UeNetDevice::SetMacEntity(ns3::Ptr<ns3::UeMacEntity> m) [member function]
    cls.add_method('SetMacEntity', 
                   'void', 
                   [param('ns3::Ptr< ns3::UeMacEntity >', 'm')])
    ## ue-net-device.h: ns3::Ptr<ns3::UeMacEntity> ns3::UeNetDevice::GetMacEntity() [member function]
    cls.add_method('GetMacEntity', 
                   'ns3::Ptr< ns3::UeMacEntity >', 
                   [])
    ## ue-net-device.h: void ns3::UeNetDevice::InitUeNetDevice() [member function]
    cls.add_method('InitUeNetDevice', 
                   'void', 
                   [])
    ## ue-net-device.h: void ns3::UeNetDevice::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ue-net-device.h: void ns3::UeNetDevice::Stop() [member function]
    cls.add_method('Stop', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ue-net-device.h: void ns3::UeNetDevice::SetTargetEnb(ns3::Ptr<ns3::EnbNetDevice> enb) [member function]
    cls.add_method('SetTargetEnb', 
                   'void', 
                   [param('ns3::Ptr< ns3::EnbNetDevice >', 'enb')])
    ## ue-net-device.h: ns3::Ptr<ns3::EnbNetDevice> ns3::UeNetDevice::GetTargetEnb() [member function]
    cls.add_method('GetTargetEnb', 
                   'ns3::Ptr< ns3::EnbNetDevice >', 
                   [])
    ## ue-net-device.h: void ns3::UeNetDevice::StartTransmission() [member function]
    cls.add_method('StartTransmission', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ue-net-device.h: bool ns3::UeNetDevice::SendPacket(ns3::Ptr<ns3::PacketBurst> p) [member function]
    cls.add_method('SendPacket', 
                   'bool', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p')], 
                   is_virtual=True)
    ## ue-net-device.h: bool ns3::UeNetDevice::DoSend(ns3::Ptr<ns3::Packet> packet, ns3::Mac48Address const & source, ns3::Mac48Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('DoSend', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Mac48Address const &', 'source'), param('ns3::Mac48Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   visibility='private', is_virtual=True)
    ## ue-net-device.h: void ns3::UeNetDevice::DoReceive(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoReceive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3EnbNetDevice_methods(root_module, cls):
    ## enb-net-device.h: static ns3::TypeId ns3::EnbNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## enb-net-device.h: ns3::EnbNetDevice::EnbNetDevice() [constructor]
    cls.add_constructor([])
    ## enb-net-device.h: ns3::EnbNetDevice::EnbNetDevice(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::LtePhy> phy) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::LtePhy >', 'phy')])
    ## enb-net-device.h: void ns3::EnbNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## enb-net-device.h: void ns3::EnbNetDevice::InitEnbNetDevice() [member function]
    cls.add_method('InitEnbNetDevice', 
                   'void', 
                   [])
    ## enb-net-device.h: void ns3::EnbNetDevice::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## enb-net-device.h: void ns3::EnbNetDevice::Stop() [member function]
    cls.add_method('Stop', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## enb-net-device.h: void ns3::EnbNetDevice::SetUeManager(ns3::Ptr<ns3::UeManager> m) [member function]
    cls.add_method('SetUeManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::UeManager >', 'm')])
    ## enb-net-device.h: ns3::Ptr<ns3::UeManager> ns3::EnbNetDevice::GetUeManager() [member function]
    cls.add_method('GetUeManager', 
                   'ns3::Ptr< ns3::UeManager >', 
                   [])
    ## enb-net-device.h: void ns3::EnbNetDevice::SetMacEntity(ns3::Ptr<ns3::EnbMacEntity> m) [member function]
    cls.add_method('SetMacEntity', 
                   'void', 
                   [param('ns3::Ptr< ns3::EnbMacEntity >', 'm')])
    ## enb-net-device.h: ns3::Ptr<ns3::EnbMacEntity> ns3::EnbNetDevice::GetMacEntity() [member function]
    cls.add_method('GetMacEntity', 
                   'ns3::Ptr< ns3::EnbMacEntity >', 
                   [])
    ## enb-net-device.h: void ns3::EnbNetDevice::StartTransmission() [member function]
    cls.add_method('StartTransmission', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## enb-net-device.h: bool ns3::EnbNetDevice::SendPacket(ns3::Ptr<ns3::PacketBurst> p) [member function]
    cls.add_method('SendPacket', 
                   'bool', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p')], 
                   is_virtual=True)
    ## enb-net-device.h: void ns3::EnbNetDevice::SendIdealPdcchMessage() [member function]
    cls.add_method('SendIdealPdcchMessage', 
                   'void', 
                   [])
    ## enb-net-device.h: bool ns3::EnbNetDevice::DoSend(ns3::Ptr<ns3::Packet> packet, ns3::Mac48Address const & source, ns3::Mac48Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('DoSend', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Mac48Address const &', 'source'), param('ns3::Mac48Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   visibility='private', is_virtual=True)
    ## enb-net-device.h: void ns3::EnbNetDevice::DoReceive(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoReceive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3DsdvDsdvHeader_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## dsdv-packet.h: ns3::dsdv::DsdvHeader::DsdvHeader(ns3::dsdv::DsdvHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dsdv::DsdvHeader const &', 'arg0')])
    ## dsdv-packet.h: ns3::dsdv::DsdvHeader::DsdvHeader(ns3::Ipv4Address dst=ns3::Ipv4Address(), uint32_t hopcount=0, uint32_t dstSeqNo=0) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'dst', default_value='ns3::Ipv4Address()'), param('uint32_t', 'hopcount', default_value='0'), param('uint32_t', 'dstSeqNo', default_value='0')])
    ## dsdv-packet.h: uint32_t ns3::dsdv::DsdvHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## dsdv-packet.h: ns3::Ipv4Address ns3::dsdv::DsdvHeader::GetDst() const [member function]
    cls.add_method('GetDst', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## dsdv-packet.h: uint32_t ns3::dsdv::DsdvHeader::GetDstSeqno() const [member function]
    cls.add_method('GetDstSeqno', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dsdv-packet.h: uint32_t ns3::dsdv::DsdvHeader::GetHopCount() const [member function]
    cls.add_method('GetHopCount', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dsdv-packet.h: ns3::TypeId ns3::dsdv::DsdvHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dsdv-packet.h: uint32_t ns3::dsdv::DsdvHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dsdv-packet.h: static ns3::TypeId ns3::dsdv::DsdvHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## dsdv-packet.h: void ns3::dsdv::DsdvHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## dsdv-packet.h: void ns3::dsdv::DsdvHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## dsdv-packet.h: void ns3::dsdv::DsdvHeader::SetDst(ns3::Ipv4Address destination) [member function]
    cls.add_method('SetDst', 
                   'void', 
                   [param('ns3::Ipv4Address', 'destination')])
    ## dsdv-packet.h: void ns3::dsdv::DsdvHeader::SetDstSeqno(uint32_t sequenceNumber) [member function]
    cls.add_method('SetDstSeqno', 
                   'void', 
                   [param('uint32_t', 'sequenceNumber')])
    ## dsdv-packet.h: void ns3::dsdv::DsdvHeader::SetHopCount(uint32_t hopCount) [member function]
    cls.add_method('SetHopCount', 
                   'void', 
                   [param('uint32_t', 'hopCount')])
    return

def register_Ns3DsdvPacketQueue_methods(root_module, cls):
    ## dsdv-packet-queue.h: ns3::dsdv::PacketQueue::PacketQueue(ns3::dsdv::PacketQueue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dsdv::PacketQueue const &', 'arg0')])
    ## dsdv-packet-queue.h: ns3::dsdv::PacketQueue::PacketQueue() [constructor]
    cls.add_constructor([])
    ## dsdv-packet-queue.h: bool ns3::dsdv::PacketQueue::Dequeue(ns3::Ipv4Address dst, ns3::dsdv::QueueEntry & entry) [member function]
    cls.add_method('Dequeue', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'dst'), param('ns3::dsdv::QueueEntry &', 'entry')])
    ## dsdv-packet-queue.h: void ns3::dsdv::PacketQueue::DropPacketWithDst(ns3::Ipv4Address dst) [member function]
    cls.add_method('DropPacketWithDst', 
                   'void', 
                   [param('ns3::Ipv4Address', 'dst')])
    ## dsdv-packet-queue.h: bool ns3::dsdv::PacketQueue::Enqueue(ns3::dsdv::QueueEntry & entry) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::dsdv::QueueEntry &', 'entry')])
    ## dsdv-packet-queue.h: bool ns3::dsdv::PacketQueue::Find(ns3::Ipv4Address dst) [member function]
    cls.add_method('Find', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'dst')])
    ## dsdv-packet-queue.h: uint32_t ns3::dsdv::PacketQueue::GetCountForPacketsWithDst(ns3::Ipv4Address dst) [member function]
    cls.add_method('GetCountForPacketsWithDst', 
                   'uint32_t', 
                   [param('ns3::Ipv4Address', 'dst')])
    ## dsdv-packet-queue.h: uint32_t ns3::dsdv::PacketQueue::GetMaxPacketsPerDst() const [member function]
    cls.add_method('GetMaxPacketsPerDst', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dsdv-packet-queue.h: uint32_t ns3::dsdv::PacketQueue::GetMaxQueueLen() const [member function]
    cls.add_method('GetMaxQueueLen', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dsdv-packet-queue.h: ns3::Time ns3::dsdv::PacketQueue::GetQueueTimeout() const [member function]
    cls.add_method('GetQueueTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## dsdv-packet-queue.h: uint32_t ns3::dsdv::PacketQueue::GetSize() [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [])
    ## dsdv-packet-queue.h: void ns3::dsdv::PacketQueue::SetMaxPacketsPerDst(uint32_t len) [member function]
    cls.add_method('SetMaxPacketsPerDst', 
                   'void', 
                   [param('uint32_t', 'len')])
    ## dsdv-packet-queue.h: void ns3::dsdv::PacketQueue::SetMaxQueueLen(uint32_t len) [member function]
    cls.add_method('SetMaxQueueLen', 
                   'void', 
                   [param('uint32_t', 'len')])
    ## dsdv-packet-queue.h: void ns3::dsdv::PacketQueue::SetQueueTimeout(ns3::Time t) [member function]
    cls.add_method('SetQueueTimeout', 
                   'void', 
                   [param('ns3::Time', 't')])
    return

def register_Ns3DsdvQueueEntry_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## dsdv-packet-queue.h: ns3::dsdv::QueueEntry::QueueEntry(ns3::dsdv::QueueEntry const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dsdv::QueueEntry const &', 'arg0')])
    ## dsdv-packet-queue.h: ns3::dsdv::QueueEntry::QueueEntry(ns3::Ptr<ns3::Packet const> pa=0, ns3::Ipv4Header const & h=ns3::Ipv4Header(), ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ucb=ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<const ns3::Packet>, const ns3::Ipv4Header&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty>(), ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ecb=ns3::Callback<void, ns3::Ptr<const ns3::Packet>, const ns3::Ipv4Header&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty>()) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Packet const >', 'pa', default_value='0'), param('ns3::Ipv4Header const &', 'h', default_value='ns3::Ipv4Header()'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb', default_value='ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<const ns3::Packet>, const ns3::Ipv4Header&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty>()'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb', default_value='ns3::Callback<void, ns3::Ptr<const ns3::Packet>, const ns3::Ipv4Header&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty>()')])
    ## dsdv-packet-queue.h: ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ns3::dsdv::QueueEntry::GetErrorCallback() const [member function]
    cls.add_method('GetErrorCallback', 
                   'ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                   [], 
                   is_const=True)
    ## dsdv-packet-queue.h: ns3::Time ns3::dsdv::QueueEntry::GetExpireTime() const [member function]
    cls.add_method('GetExpireTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## dsdv-packet-queue.h: ns3::Ipv4Header ns3::dsdv::QueueEntry::GetIpv4Header() const [member function]
    cls.add_method('GetIpv4Header', 
                   'ns3::Ipv4Header', 
                   [], 
                   is_const=True)
    ## dsdv-packet-queue.h: ns3::Ptr<ns3::Packet const> ns3::dsdv::QueueEntry::GetPacket() const [member function]
    cls.add_method('GetPacket', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [], 
                   is_const=True)
    ## dsdv-packet-queue.h: ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ns3::dsdv::QueueEntry::GetUnicastForwardCallback() const [member function]
    cls.add_method('GetUnicastForwardCallback', 
                   'ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                   [], 
                   is_const=True)
    ## dsdv-packet-queue.h: void ns3::dsdv::QueueEntry::SetErrorCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ecb) [member function]
    cls.add_method('SetErrorCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb')])
    ## dsdv-packet-queue.h: void ns3::dsdv::QueueEntry::SetExpireTime(ns3::Time exp) [member function]
    cls.add_method('SetExpireTime', 
                   'void', 
                   [param('ns3::Time', 'exp')])
    ## dsdv-packet-queue.h: void ns3::dsdv::QueueEntry::SetIpv4Header(ns3::Ipv4Header h) [member function]
    cls.add_method('SetIpv4Header', 
                   'void', 
                   [param('ns3::Ipv4Header', 'h')])
    ## dsdv-packet-queue.h: void ns3::dsdv::QueueEntry::SetPacket(ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('SetPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## dsdv-packet-queue.h: void ns3::dsdv::QueueEntry::SetUnicastForwardCallback(ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ucb) [member function]
    cls.add_method('SetUnicastForwardCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb')])
    return

def register_Ns3DsdvRoutingProtocol_methods(root_module, cls):
    ## dsdv-routing-protocol.h: ns3::dsdv::RoutingProtocol::RoutingProtocol(ns3::dsdv::RoutingProtocol const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dsdv::RoutingProtocol const &', 'arg0')])
    ## dsdv-routing-protocol.h: ns3::dsdv::RoutingProtocol::RoutingProtocol() [constructor]
    cls.add_constructor([])
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## dsdv-routing-protocol.h: bool ns3::dsdv::RoutingProtocol::GetEnableBufferFlag() const [member function]
    cls.add_method('GetEnableBufferFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## dsdv-routing-protocol.h: bool ns3::dsdv::RoutingProtocol::GetEnableRAFlag() const [member function]
    cls.add_method('GetEnableRAFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## dsdv-routing-protocol.h: static ns3::TypeId ns3::dsdv::RoutingProtocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## dsdv-routing-protocol.h: bool ns3::dsdv::RoutingProtocol::GetWSTFlag() const [member function]
    cls.add_method('GetWSTFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::NotifyAddAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyAddAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::NotifyInterfaceDown(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceDown', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::NotifyInterfaceUp(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceUp', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::NotifyRemoveAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyRemoveAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::PrintRoutingTable(ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('PrintRoutingTable', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_const=True, is_virtual=True)
    ## dsdv-routing-protocol.h: bool ns3::dsdv::RoutingProtocol::RouteInput(ns3::Ptr<ns3::Packet const> p, ns3::Ipv4Header const & header, ns3::Ptr<const ns3::NetDevice> idev, ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ucb, ns3::Callback<void,ns3::Ptr<ns3::Ipv4MulticastRoute>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> mcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,unsigned int,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> lcb, ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ecb) [member function]
    cls.add_method('RouteInput', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice const >', 'idev'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4MulticastRoute >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'mcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'lcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb')], 
                   is_virtual=True)
    ## dsdv-routing-protocol.h: ns3::Ptr<ns3::Ipv4Route> ns3::dsdv::RoutingProtocol::RouteOutput(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Header const & header, ns3::Ptr<ns3::NetDevice> oif, ns3::Socket::SocketErrno & sockerr) [member function]
    cls.add_method('RouteOutput', 
                   'ns3::Ptr< ns3::Ipv4Route >', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice >', 'oif'), param('ns3::Socket::SocketErrno &', 'sockerr')], 
                   is_virtual=True)
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::SetEnableBufferFlag(bool f) [member function]
    cls.add_method('SetEnableBufferFlag', 
                   'void', 
                   [param('bool', 'f')])
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::SetEnableRAFlag(bool f) [member function]
    cls.add_method('SetEnableRAFlag', 
                   'void', 
                   [param('bool', 'f')])
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::SetIpv4(ns3::Ptr<ns3::Ipv4> ipv4) [member function]
    cls.add_method('SetIpv4', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4 >', 'ipv4')], 
                   is_virtual=True)
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::SetWSTFlag(bool f) [member function]
    cls.add_method('SetWSTFlag', 
                   'void', 
                   [param('bool', 'f')])
    ## dsdv-routing-protocol.h: ns3::dsdv::RoutingProtocol::DSDV_PORT [variable]
    cls.add_static_attribute('DSDV_PORT', 'uint32_t const', is_const=True)
    return

def register_Ns3DsdvRoutingTable_methods(root_module, cls):
    ## dsdv-rtable.h: ns3::dsdv::RoutingTable::RoutingTable(ns3::dsdv::RoutingTable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dsdv::RoutingTable const &', 'arg0')])
    ## dsdv-rtable.h: ns3::dsdv::RoutingTable::RoutingTable() [constructor]
    cls.add_constructor([])
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTable::AddIpv4Event(ns3::Ipv4Address arg0, ns3::EventId arg1) [member function]
    cls.add_method('AddIpv4Event', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'arg0'), param('ns3::EventId', 'arg1')])
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTable::AddRoute(ns3::dsdv::RoutingTableEntry & r) [member function]
    cls.add_method('AddRoute', 
                   'bool', 
                   [param('ns3::dsdv::RoutingTableEntry &', 'r')])
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTable::AnyRunningEvent(ns3::Ipv4Address address) [member function]
    cls.add_method('AnyRunningEvent', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'address')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTable::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTable::DeleteAllRoutesFromInterface(ns3::Ipv4InterfaceAddress iface) [member function]
    cls.add_method('DeleteAllRoutesFromInterface', 
                   'void', 
                   [param('ns3::Ipv4InterfaceAddress', 'iface')])
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTable::DeleteIpv4Event(ns3::Ipv4Address address) [member function]
    cls.add_method('DeleteIpv4Event', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'address')])
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTable::DeleteRoute(ns3::Ipv4Address dst) [member function]
    cls.add_method('DeleteRoute', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'dst')])
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTable::ForceDeleteIpv4Event(ns3::Ipv4Address address) [member function]
    cls.add_method('ForceDeleteIpv4Event', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'address')])
    ## dsdv-rtable.h: ns3::EventId ns3::dsdv::RoutingTable::GetEventId(ns3::Ipv4Address address) [member function]
    cls.add_method('GetEventId', 
                   'ns3::EventId', 
                   [param('ns3::Ipv4Address', 'address')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTable::GetListOfAllRoutes(std::map<ns3::Ipv4Address, ns3::dsdv::RoutingTableEntry, std::less<ns3::Ipv4Address>, std::allocator<std::pair<ns3::Ipv4Address const, ns3::dsdv::RoutingTableEntry> > > & allRoutes) [member function]
    cls.add_method('GetListOfAllRoutes', 
                   'void', 
                   [param('std::map< ns3::Ipv4Address, ns3::dsdv::RoutingTableEntry > &', 'allRoutes')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTable::GetListOfDestinationWithNextHop(ns3::Ipv4Address nxtHp, std::map<ns3::Ipv4Address, ns3::dsdv::RoutingTableEntry, std::less<ns3::Ipv4Address>, std::allocator<std::pair<ns3::Ipv4Address const, ns3::dsdv::RoutingTableEntry> > > & dstList) [member function]
    cls.add_method('GetListOfDestinationWithNextHop', 
                   'void', 
                   [param('ns3::Ipv4Address', 'nxtHp'), param('std::map< ns3::Ipv4Address, ns3::dsdv::RoutingTableEntry > &', 'dstList')])
    ## dsdv-rtable.h: ns3::Time ns3::dsdv::RoutingTable::Getholddowntime() const [member function]
    cls.add_method('Getholddowntime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTable::LookupRoute(ns3::Ipv4Address dst, ns3::dsdv::RoutingTableEntry & rt) [member function]
    cls.add_method('LookupRoute', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'dst'), param('ns3::dsdv::RoutingTableEntry &', 'rt')])
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTable::LookupRoute(ns3::Ipv4Address id, ns3::dsdv::RoutingTableEntry & rt, bool forRouteInput) [member function]
    cls.add_method('LookupRoute', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'id'), param('ns3::dsdv::RoutingTableEntry &', 'rt'), param('bool', 'forRouteInput')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTable::Print(ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_const=True)
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTable::Purge(std::map<ns3::Ipv4Address, ns3::dsdv::RoutingTableEntry, std::less<ns3::Ipv4Address>, std::allocator<std::pair<ns3::Ipv4Address const, ns3::dsdv::RoutingTableEntry> > > & removedAddresses) [member function]
    cls.add_method('Purge', 
                   'void', 
                   [param('std::map< ns3::Ipv4Address, ns3::dsdv::RoutingTableEntry > &', 'removedAddresses')])
    ## dsdv-rtable.h: uint32_t ns3::dsdv::RoutingTable::RoutingTableSize() [member function]
    cls.add_method('RoutingTableSize', 
                   'uint32_t', 
                   [])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTable::Setholddowntime(ns3::Time t) [member function]
    cls.add_method('Setholddowntime', 
                   'void', 
                   [param('ns3::Time', 't')])
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTable::Update(ns3::dsdv::RoutingTableEntry & rt) [member function]
    cls.add_method('Update', 
                   'bool', 
                   [param('ns3::dsdv::RoutingTableEntry &', 'rt')])
    return

def register_Ns3DsdvRoutingTableEntry_methods(root_module, cls):
    ## dsdv-rtable.h: ns3::dsdv::RoutingTableEntry::RoutingTableEntry(ns3::dsdv::RoutingTableEntry const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dsdv::RoutingTableEntry const &', 'arg0')])
    ## dsdv-rtable.h: ns3::dsdv::RoutingTableEntry::RoutingTableEntry(ns3::Ptr<ns3::NetDevice> dev=0, ns3::Ipv4Address dst=ns3::Ipv4Address(), u_int32_t m_seqNo=0, ns3::Ipv4InterfaceAddress iface=ns3::Ipv4InterfaceAddress(), u_int32_t hops=0, ns3::Ipv4Address nextHop=ns3::Ipv4Address(), ns3::Time lifetime=ns3::Simulator::Now( ), ns3::Time SettlingTime=ns3::Simulator::Now( ), bool changedEntries=false) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::NetDevice >', 'dev', default_value='0'), param('ns3::Ipv4Address', 'dst', default_value='ns3::Ipv4Address()'), param('u_int32_t', 'm_seqNo', default_value='0'), param('ns3::Ipv4InterfaceAddress', 'iface', default_value='ns3::Ipv4InterfaceAddress()'), param('u_int32_t', 'hops', default_value='0'), param('ns3::Ipv4Address', 'nextHop', default_value='ns3::Ipv4Address()'), param('ns3::Time', 'lifetime', default_value='ns3::Simulator::Now( )'), param('ns3::Time', 'SettlingTime', default_value='ns3::Simulator::Now( )'), param('bool', 'changedEntries', default_value='false')])
    ## dsdv-rtable.h: ns3::Ipv4Address ns3::dsdv::RoutingTableEntry::GetDestination() const [member function]
    cls.add_method('GetDestination', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTableEntry::GetEntriesChanged() const [member function]
    cls.add_method('GetEntriesChanged', 
                   'bool', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: ns3::dsdv::RouteFlags ns3::dsdv::RoutingTableEntry::GetFlag() const [member function]
    cls.add_method('GetFlag', 
                   'ns3::dsdv::RouteFlags', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: uint32_t ns3::dsdv::RoutingTableEntry::GetHop() const [member function]
    cls.add_method('GetHop', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: ns3::Ipv4InterfaceAddress ns3::dsdv::RoutingTableEntry::GetInterface() const [member function]
    cls.add_method('GetInterface', 
                   'ns3::Ipv4InterfaceAddress', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: ns3::Time ns3::dsdv::RoutingTableEntry::GetLifeTime() const [member function]
    cls.add_method('GetLifeTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: ns3::Ipv4Address ns3::dsdv::RoutingTableEntry::GetNextHop() const [member function]
    cls.add_method('GetNextHop', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: ns3::Ptr<ns3::NetDevice> ns3::dsdv::RoutingTableEntry::GetOutputDevice() const [member function]
    cls.add_method('GetOutputDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: ns3::Ptr<ns3::Ipv4Route> ns3::dsdv::RoutingTableEntry::GetRoute() const [member function]
    cls.add_method('GetRoute', 
                   'ns3::Ptr< ns3::Ipv4Route >', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: uint32_t ns3::dsdv::RoutingTableEntry::GetSeqNo() const [member function]
    cls.add_method('GetSeqNo', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: ns3::Time ns3::dsdv::RoutingTableEntry::GetSettlingTime() const [member function]
    cls.add_method('GetSettlingTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::Print(ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_const=True)
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetEntriesChanged(bool entriesChanged) [member function]
    cls.add_method('SetEntriesChanged', 
                   'void', 
                   [param('bool', 'entriesChanged')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetFlag(ns3::dsdv::RouteFlags flag) [member function]
    cls.add_method('SetFlag', 
                   'void', 
                   [param('ns3::dsdv::RouteFlags', 'flag')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetHop(uint32_t hopCount) [member function]
    cls.add_method('SetHop', 
                   'void', 
                   [param('uint32_t', 'hopCount')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetInterface(ns3::Ipv4InterfaceAddress iface) [member function]
    cls.add_method('SetInterface', 
                   'void', 
                   [param('ns3::Ipv4InterfaceAddress', 'iface')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetLifeTime(ns3::Time lifeTime) [member function]
    cls.add_method('SetLifeTime', 
                   'void', 
                   [param('ns3::Time', 'lifeTime')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetNextHop(ns3::Ipv4Address nextHop) [member function]
    cls.add_method('SetNextHop', 
                   'void', 
                   [param('ns3::Ipv4Address', 'nextHop')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetOutputDevice(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('SetOutputDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetRoute(ns3::Ptr<ns3::Ipv4Route> route) [member function]
    cls.add_method('SetRoute', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4Route >', 'route')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetSeqNo(uint32_t sequenceNumber) [member function]
    cls.add_method('SetSeqNo', 
                   'void', 
                   [param('uint32_t', 'sequenceNumber')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetSettlingTime(ns3::Time settlingTime) [member function]
    cls.add_method('SetSettlingTime', 
                   'void', 
                   [param('ns3::Time', 'settlingTime')])
    return

def register_Ns3OlsrAssociation_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## olsr-repositories.h: ns3::olsr::Association::Association() [constructor]
    cls.add_constructor([])
    ## olsr-repositories.h: ns3::olsr::Association::Association(ns3::olsr::Association const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::Association const &', 'arg0')])
    ## olsr-repositories.h: ns3::olsr::Association::netmask [variable]
    cls.add_instance_attribute('netmask', 'ns3::Ipv4Mask', is_const=False)
    ## olsr-repositories.h: ns3::olsr::Association::networkAddr [variable]
    cls.add_instance_attribute('networkAddr', 'ns3::Ipv4Address', is_const=False)
    return

def register_Ns3OlsrAssociationTuple_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## olsr-repositories.h: ns3::olsr::AssociationTuple::AssociationTuple() [constructor]
    cls.add_constructor([])
    ## olsr-repositories.h: ns3::olsr::AssociationTuple::AssociationTuple(ns3::olsr::AssociationTuple const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::AssociationTuple const &', 'arg0')])
    ## olsr-repositories.h: ns3::olsr::AssociationTuple::expirationTime [variable]
    cls.add_instance_attribute('expirationTime', 'ns3::Time', is_const=False)
    ## olsr-repositories.h: ns3::olsr::AssociationTuple::gatewayAddr [variable]
    cls.add_instance_attribute('gatewayAddr', 'ns3::Ipv4Address', is_const=False)
    ## olsr-repositories.h: ns3::olsr::AssociationTuple::netmask [variable]
    cls.add_instance_attribute('netmask', 'ns3::Ipv4Mask', is_const=False)
    ## olsr-repositories.h: ns3::olsr::AssociationTuple::networkAddr [variable]
    cls.add_instance_attribute('networkAddr', 'ns3::Ipv4Address', is_const=False)
    return

def register_Ns3OlsrDuplicateTuple_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## olsr-repositories.h: ns3::olsr::DuplicateTuple::DuplicateTuple() [constructor]
    cls.add_constructor([])
    ## olsr-repositories.h: ns3::olsr::DuplicateTuple::DuplicateTuple(ns3::olsr::DuplicateTuple const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::DuplicateTuple const &', 'arg0')])
    ## olsr-repositories.h: ns3::olsr::DuplicateTuple::address [variable]
    cls.add_instance_attribute('address', 'ns3::Ipv4Address', is_const=False)
    ## olsr-repositories.h: ns3::olsr::DuplicateTuple::expirationTime [variable]
    cls.add_instance_attribute('expirationTime', 'ns3::Time', is_const=False)
    ## olsr-repositories.h: ns3::olsr::DuplicateTuple::ifaceList [variable]
    cls.add_instance_attribute('ifaceList', 'std::vector< ns3::Ipv4Address >', is_const=False)
    ## olsr-repositories.h: ns3::olsr::DuplicateTuple::retransmitted [variable]
    cls.add_instance_attribute('retransmitted', 'bool', is_const=False)
    ## olsr-repositories.h: ns3::olsr::DuplicateTuple::sequenceNumber [variable]
    cls.add_instance_attribute('sequenceNumber', 'uint16_t', is_const=False)
    return

def register_Ns3OlsrIfaceAssocTuple_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## olsr-repositories.h: ns3::olsr::IfaceAssocTuple::IfaceAssocTuple() [constructor]
    cls.add_constructor([])
    ## olsr-repositories.h: ns3::olsr::IfaceAssocTuple::IfaceAssocTuple(ns3::olsr::IfaceAssocTuple const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::IfaceAssocTuple const &', 'arg0')])
    ## olsr-repositories.h: ns3::olsr::IfaceAssocTuple::ifaceAddr [variable]
    cls.add_instance_attribute('ifaceAddr', 'ns3::Ipv4Address', is_const=False)
    ## olsr-repositories.h: ns3::olsr::IfaceAssocTuple::mainAddr [variable]
    cls.add_instance_attribute('mainAddr', 'ns3::Ipv4Address', is_const=False)
    ## olsr-repositories.h: ns3::olsr::IfaceAssocTuple::time [variable]
    cls.add_instance_attribute('time', 'ns3::Time', is_const=False)
    return

def register_Ns3OlsrLinkTuple_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## olsr-repositories.h: ns3::olsr::LinkTuple::LinkTuple() [constructor]
    cls.add_constructor([])
    ## olsr-repositories.h: ns3::olsr::LinkTuple::LinkTuple(ns3::olsr::LinkTuple const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::LinkTuple const &', 'arg0')])
    ## olsr-repositories.h: ns3::olsr::LinkTuple::asymTime [variable]
    cls.add_instance_attribute('asymTime', 'ns3::Time', is_const=False)
    ## olsr-repositories.h: ns3::olsr::LinkTuple::localIfaceAddr [variable]
    cls.add_instance_attribute('localIfaceAddr', 'ns3::Ipv4Address', is_const=False)
    ## olsr-repositories.h: ns3::olsr::LinkTuple::neighborIfaceAddr [variable]
    cls.add_instance_attribute('neighborIfaceAddr', 'ns3::Ipv4Address', is_const=False)
    ## olsr-repositories.h: ns3::olsr::LinkTuple::symTime [variable]
    cls.add_instance_attribute('symTime', 'ns3::Time', is_const=False)
    ## olsr-repositories.h: ns3::olsr::LinkTuple::time [variable]
    cls.add_instance_attribute('time', 'ns3::Time', is_const=False)
    return

def register_Ns3OlsrMessageHeader_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## olsr-header.h: ns3::olsr::MessageHeader::MessageHeader(ns3::olsr::MessageHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::MessageHeader const &', 'arg0')])
    ## olsr-header.h: ns3::olsr::MessageHeader::MessageHeader() [constructor]
    cls.add_constructor([])
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello & ns3::olsr::MessageHeader::GetHello() [member function]
    cls.add_method('GetHello', 
                   'ns3::olsr::MessageHeader::Hello &', 
                   [])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello const & ns3::olsr::MessageHeader::GetHello() const [member function]
    cls.add_method('GetHello', 
                   'ns3::olsr::MessageHeader::Hello const &', 
                   [], 
                   is_const=True)
    ## olsr-header.h: ns3::olsr::MessageHeader::Hna & ns3::olsr::MessageHeader::GetHna() [member function]
    cls.add_method('GetHna', 
                   'ns3::olsr::MessageHeader::Hna &', 
                   [])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hna const & ns3::olsr::MessageHeader::GetHna() const [member function]
    cls.add_method('GetHna', 
                   'ns3::olsr::MessageHeader::Hna const &', 
                   [], 
                   is_const=True)
    ## olsr-header.h: uint8_t ns3::olsr::MessageHeader::GetHopCount() const [member function]
    cls.add_method('GetHopCount', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## olsr-header.h: ns3::TypeId ns3::olsr::MessageHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## olsr-header.h: uint16_t ns3::olsr::MessageHeader::GetMessageSequenceNumber() const [member function]
    cls.add_method('GetMessageSequenceNumber', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## olsr-header.h: ns3::olsr::MessageHeader::MessageType ns3::olsr::MessageHeader::GetMessageType() const [member function]
    cls.add_method('GetMessageType', 
                   'ns3::olsr::MessageHeader::MessageType', 
                   [], 
                   is_const=True)
    ## olsr-header.h: ns3::olsr::MessageHeader::Mid & ns3::olsr::MessageHeader::GetMid() [member function]
    cls.add_method('GetMid', 
                   'ns3::olsr::MessageHeader::Mid &', 
                   [])
    ## olsr-header.h: ns3::olsr::MessageHeader::Mid const & ns3::olsr::MessageHeader::GetMid() const [member function]
    cls.add_method('GetMid', 
                   'ns3::olsr::MessageHeader::Mid const &', 
                   [], 
                   is_const=True)
    ## olsr-header.h: ns3::Ipv4Address ns3::olsr::MessageHeader::GetOriginatorAddress() const [member function]
    cls.add_method('GetOriginatorAddress', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## olsr-header.h: ns3::olsr::MessageHeader::Tc & ns3::olsr::MessageHeader::GetTc() [member function]
    cls.add_method('GetTc', 
                   'ns3::olsr::MessageHeader::Tc &', 
                   [])
    ## olsr-header.h: ns3::olsr::MessageHeader::Tc const & ns3::olsr::MessageHeader::GetTc() const [member function]
    cls.add_method('GetTc', 
                   'ns3::olsr::MessageHeader::Tc const &', 
                   [], 
                   is_const=True)
    ## olsr-header.h: uint8_t ns3::olsr::MessageHeader::GetTimeToLive() const [member function]
    cls.add_method('GetTimeToLive', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## olsr-header.h: static ns3::TypeId ns3::olsr::MessageHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## olsr-header.h: ns3::Time ns3::olsr::MessageHeader::GetVTime() const [member function]
    cls.add_method('GetVTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::SetHopCount(uint8_t hopCount) [member function]
    cls.add_method('SetHopCount', 
                   'void', 
                   [param('uint8_t', 'hopCount')])
    ## olsr-header.h: void ns3::olsr::MessageHeader::SetMessageSequenceNumber(uint16_t messageSequenceNumber) [member function]
    cls.add_method('SetMessageSequenceNumber', 
                   'void', 
                   [param('uint16_t', 'messageSequenceNumber')])
    ## olsr-header.h: void ns3::olsr::MessageHeader::SetMessageType(ns3::olsr::MessageHeader::MessageType messageType) [member function]
    cls.add_method('SetMessageType', 
                   'void', 
                   [param('ns3::olsr::MessageHeader::MessageType', 'messageType')])
    ## olsr-header.h: void ns3::olsr::MessageHeader::SetOriginatorAddress(ns3::Ipv4Address originatorAddress) [member function]
    cls.add_method('SetOriginatorAddress', 
                   'void', 
                   [param('ns3::Ipv4Address', 'originatorAddress')])
    ## olsr-header.h: void ns3::olsr::MessageHeader::SetTimeToLive(uint8_t timeToLive) [member function]
    cls.add_method('SetTimeToLive', 
                   'void', 
                   [param('uint8_t', 'timeToLive')])
    ## olsr-header.h: void ns3::olsr::MessageHeader::SetVTime(ns3::Time time) [member function]
    cls.add_method('SetVTime', 
                   'void', 
                   [param('ns3::Time', 'time')])
    return

def register_Ns3OlsrMessageHeaderHello_methods(root_module, cls):
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello::Hello() [constructor]
    cls.add_constructor([])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello::Hello(ns3::olsr::MessageHeader::Hello const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::MessageHeader::Hello const &', 'arg0')])
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::Hello::Deserialize(ns3::Buffer::Iterator start, uint32_t messageSize) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint32_t', 'messageSize')])
    ## olsr-header.h: ns3::Time ns3::olsr::MessageHeader::Hello::GetHTime() const [member function]
    cls.add_method('GetHTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::Hello::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Hello::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Hello::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Hello::SetHTime(ns3::Time time) [member function]
    cls.add_method('SetHTime', 
                   'void', 
                   [param('ns3::Time', 'time')])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello::hTime [variable]
    cls.add_instance_attribute('hTime', 'uint8_t', is_const=False)
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello::linkMessages [variable]
    cls.add_instance_attribute('linkMessages', 'std::vector< ns3::olsr::MessageHeader::Hello::LinkMessage >', is_const=False)
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello::willingness [variable]
    cls.add_instance_attribute('willingness', 'uint8_t', is_const=False)
    return

def register_Ns3OlsrMessageHeaderHelloLinkMessage_methods(root_module, cls):
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello::LinkMessage::LinkMessage() [constructor]
    cls.add_constructor([])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello::LinkMessage::LinkMessage(ns3::olsr::MessageHeader::Hello::LinkMessage const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::MessageHeader::Hello::LinkMessage const &', 'arg0')])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello::LinkMessage::linkCode [variable]
    cls.add_instance_attribute('linkCode', 'uint8_t', is_const=False)
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello::LinkMessage::neighborInterfaceAddresses [variable]
    cls.add_instance_attribute('neighborInterfaceAddresses', 'std::vector< ns3::Ipv4Address >', is_const=False)
    return

def register_Ns3OlsrMessageHeaderHna_methods(root_module, cls):
    ## olsr-header.h: ns3::olsr::MessageHeader::Hna::Hna() [constructor]
    cls.add_constructor([])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hna::Hna(ns3::olsr::MessageHeader::Hna const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::MessageHeader::Hna const &', 'arg0')])
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::Hna::Deserialize(ns3::Buffer::Iterator start, uint32_t messageSize) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint32_t', 'messageSize')])
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::Hna::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Hna::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Hna::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## olsr-header.h: ns3::olsr::MessageHeader::Hna::associations [variable]
    cls.add_instance_attribute('associations', 'std::vector< ns3::olsr::MessageHeader::Hna::Association >', is_const=False)
    return

def register_Ns3OlsrMessageHeaderHnaAssociation_methods(root_module, cls):
    ## olsr-header.h: ns3::olsr::MessageHeader::Hna::Association::Association() [constructor]
    cls.add_constructor([])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hna::Association::Association(ns3::olsr::MessageHeader::Hna::Association const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::MessageHeader::Hna::Association const &', 'arg0')])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hna::Association::address [variable]
    cls.add_instance_attribute('address', 'ns3::Ipv4Address', is_const=False)
    ## olsr-header.h: ns3::olsr::MessageHeader::Hna::Association::mask [variable]
    cls.add_instance_attribute('mask', 'ns3::Ipv4Mask', is_const=False)
    return

def register_Ns3OlsrMessageHeaderMid_methods(root_module, cls):
    ## olsr-header.h: ns3::olsr::MessageHeader::Mid::Mid() [constructor]
    cls.add_constructor([])
    ## olsr-header.h: ns3::olsr::MessageHeader::Mid::Mid(ns3::olsr::MessageHeader::Mid const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::MessageHeader::Mid const &', 'arg0')])
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::Mid::Deserialize(ns3::Buffer::Iterator start, uint32_t messageSize) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint32_t', 'messageSize')])
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::Mid::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Mid::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Mid::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## olsr-header.h: ns3::olsr::MessageHeader::Mid::interfaceAddresses [variable]
    cls.add_instance_attribute('interfaceAddresses', 'std::vector< ns3::Ipv4Address >', is_const=False)
    return

def register_Ns3OlsrMessageHeaderTc_methods(root_module, cls):
    ## olsr-header.h: ns3::olsr::MessageHeader::Tc::Tc() [constructor]
    cls.add_constructor([])
    ## olsr-header.h: ns3::olsr::MessageHeader::Tc::Tc(ns3::olsr::MessageHeader::Tc const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::MessageHeader::Tc const &', 'arg0')])
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::Tc::Deserialize(ns3::Buffer::Iterator start, uint32_t messageSize) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint32_t', 'messageSize')])
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::Tc::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Tc::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Tc::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## olsr-header.h: ns3::olsr::MessageHeader::Tc::ansn [variable]
    cls.add_instance_attribute('ansn', 'uint16_t', is_const=False)
    ## olsr-header.h: ns3::olsr::MessageHeader::Tc::neighborAddresses [variable]
    cls.add_instance_attribute('neighborAddresses', 'std::vector< ns3::Ipv4Address >', is_const=False)
    return

def register_Ns3OlsrMprSelectorTuple_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## olsr-repositories.h: ns3::olsr::MprSelectorTuple::MprSelectorTuple() [constructor]
    cls.add_constructor([])
    ## olsr-repositories.h: ns3::olsr::MprSelectorTuple::MprSelectorTuple(ns3::olsr::MprSelectorTuple const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::MprSelectorTuple const &', 'arg0')])
    ## olsr-repositories.h: ns3::olsr::MprSelectorTuple::expirationTime [variable]
    cls.add_instance_attribute('expirationTime', 'ns3::Time', is_const=False)
    ## olsr-repositories.h: ns3::olsr::MprSelectorTuple::mainAddr [variable]
    cls.add_instance_attribute('mainAddr', 'ns3::Ipv4Address', is_const=False)
    return

def register_Ns3OlsrNeighborTuple_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## olsr-repositories.h: ns3::olsr::NeighborTuple::NeighborTuple() [constructor]
    cls.add_constructor([])
    ## olsr-repositories.h: ns3::olsr::NeighborTuple::NeighborTuple(ns3::olsr::NeighborTuple const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::NeighborTuple const &', 'arg0')])
    ## olsr-repositories.h: ns3::olsr::NeighborTuple::neighborMainAddr [variable]
    cls.add_instance_attribute('neighborMainAddr', 'ns3::Ipv4Address', is_const=False)
    ## olsr-repositories.h: ns3::olsr::NeighborTuple::status [variable]
    cls.add_instance_attribute('status', 'ns3::olsr::NeighborTuple::Status', is_const=False)
    ## olsr-repositories.h: ns3::olsr::NeighborTuple::willingness [variable]
    cls.add_instance_attribute('willingness', 'uint8_t', is_const=False)
    return

def register_Ns3OlsrOlsrMprTestCase_methods(root_module, cls):
    ## olsr-routing-protocol.h: ns3::olsr::OlsrMprTestCase::OlsrMprTestCase() [constructor]
    cls.add_constructor([])
    ## olsr-routing-protocol.h: bool ns3::olsr::OlsrMprTestCase::DoRun() [member function]
    cls.add_method('DoRun', 
                   'bool', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3OlsrPacketHeader_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## olsr-header.h: ns3::olsr::PacketHeader::PacketHeader(ns3::olsr::PacketHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::PacketHeader const &', 'arg0')])
    ## olsr-header.h: ns3::olsr::PacketHeader::PacketHeader() [constructor]
    cls.add_constructor([])
    ## olsr-header.h: uint32_t ns3::olsr::PacketHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## olsr-header.h: ns3::TypeId ns3::olsr::PacketHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## olsr-header.h: uint16_t ns3::olsr::PacketHeader::GetPacketLength() const [member function]
    cls.add_method('GetPacketLength', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## olsr-header.h: uint16_t ns3::olsr::PacketHeader::GetPacketSequenceNumber() const [member function]
    cls.add_method('GetPacketSequenceNumber', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## olsr-header.h: uint32_t ns3::olsr::PacketHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## olsr-header.h: static ns3::TypeId ns3::olsr::PacketHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## olsr-header.h: void ns3::olsr::PacketHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## olsr-header.h: void ns3::olsr::PacketHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## olsr-header.h: void ns3::olsr::PacketHeader::SetPacketLength(uint16_t length) [member function]
    cls.add_method('SetPacketLength', 
                   'void', 
                   [param('uint16_t', 'length')])
    ## olsr-header.h: void ns3::olsr::PacketHeader::SetPacketSequenceNumber(uint16_t seqnum) [member function]
    cls.add_method('SetPacketSequenceNumber', 
                   'void', 
                   [param('uint16_t', 'seqnum')])
    return

def register_Ns3OlsrRoutingProtocol_methods(root_module, cls):
    ## olsr-routing-protocol.h: ns3::olsr::RoutingProtocol::RoutingProtocol(ns3::olsr::RoutingProtocol const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::RoutingProtocol const &', 'arg0')])
    ## olsr-routing-protocol.h: ns3::olsr::RoutingProtocol::RoutingProtocol() [constructor]
    cls.add_constructor([])
    ## olsr-routing-protocol.h: void ns3::olsr::RoutingProtocol::AddHostNetworkAssociation(ns3::Ipv4Address networkAddr, ns3::Ipv4Mask netmask) [member function]
    cls.add_method('AddHostNetworkAssociation', 
                   'void', 
                   [param('ns3::Ipv4Address', 'networkAddr'), param('ns3::Ipv4Mask', 'netmask')])
    ## olsr-routing-protocol.h: void ns3::olsr::RoutingProtocol::Dump() [member function]
    cls.add_method('Dump', 
                   'void', 
                   [])
    ## olsr-routing-protocol.h: std::set<unsigned int, std::less<unsigned int>, std::allocator<unsigned int> > ns3::olsr::RoutingProtocol::GetInterfaceExclusions() const [member function]
    cls.add_method('GetInterfaceExclusions', 
                   'std::set< unsigned int >', 
                   [], 
                   is_const=True)
    ## olsr-routing-protocol.h: std::vector<ns3::olsr::RoutingTableEntry,std::allocator<ns3::olsr::RoutingTableEntry> > ns3::olsr::RoutingProtocol::GetRoutingTableEntries() const [member function]
    cls.add_method('GetRoutingTableEntries', 
                   'std::vector< ns3::olsr::RoutingTableEntry >', 
                   [], 
                   is_const=True)
    ## olsr-routing-protocol.h: static ns3::TypeId ns3::olsr::RoutingProtocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## olsr-routing-protocol.h: void ns3::olsr::RoutingProtocol::SetInterfaceExclusions(std::set<unsigned int, std::less<unsigned int>, std::allocator<unsigned int> > exceptions) [member function]
    cls.add_method('SetInterfaceExclusions', 
                   'void', 
                   [param('std::set< unsigned int >', 'exceptions')])
    ## olsr-routing-protocol.h: void ns3::olsr::RoutingProtocol::SetMainInterface(uint32_t interface) [member function]
    cls.add_method('SetMainInterface', 
                   'void', 
                   [param('uint32_t', 'interface')])
    ## olsr-routing-protocol.h: void ns3::olsr::RoutingProtocol::SetRoutingTableAssociation(ns3::Ptr<ns3::Ipv4StaticRouting> routingTable) [member function]
    cls.add_method('SetRoutingTableAssociation', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4StaticRouting >', 'routingTable')])
    ## olsr-routing-protocol.h: void ns3::olsr::RoutingProtocol::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## olsr-routing-protocol.h: void ns3::olsr::RoutingProtocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## olsr-routing-protocol.h: void ns3::olsr::RoutingProtocol::NotifyAddAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyAddAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   visibility='private', is_virtual=True)
    ## olsr-routing-protocol.h: void ns3::olsr::RoutingProtocol::NotifyInterfaceDown(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceDown', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   visibility='private', is_virtual=True)
    ## olsr-routing-protocol.h: void ns3::olsr::RoutingProtocol::NotifyInterfaceUp(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceUp', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   visibility='private', is_virtual=True)
    ## olsr-routing-protocol.h: void ns3::olsr::RoutingProtocol::NotifyRemoveAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyRemoveAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   visibility='private', is_virtual=True)
    ## olsr-routing-protocol.h: void ns3::olsr::RoutingProtocol::PrintRoutingTable(ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('PrintRoutingTable', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_const=True, visibility='private', is_virtual=True)
    ## olsr-routing-protocol.h: bool ns3::olsr::RoutingProtocol::RouteInput(ns3::Ptr<ns3::Packet const> p, ns3::Ipv4Header const & header, ns3::Ptr<const ns3::NetDevice> idev, ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ucb, ns3::Callback<void,ns3::Ptr<ns3::Ipv4MulticastRoute>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> mcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,unsigned int,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> lcb, ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ecb) [member function]
    cls.add_method('RouteInput', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice const >', 'idev'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4MulticastRoute >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'mcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'lcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb')], 
                   visibility='private', is_virtual=True)
    ## olsr-routing-protocol.h: ns3::Ptr<ns3::Ipv4Route> ns3::olsr::RoutingProtocol::RouteOutput(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Header const & header, ns3::Ptr<ns3::NetDevice> oif, ns3::Socket::SocketErrno & sockerr) [member function]
    cls.add_method('RouteOutput', 
                   'ns3::Ptr< ns3::Ipv4Route >', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice >', 'oif'), param('ns3::Socket::SocketErrno &', 'sockerr')], 
                   visibility='private', is_virtual=True)
    ## olsr-routing-protocol.h: void ns3::olsr::RoutingProtocol::SetIpv4(ns3::Ptr<ns3::Ipv4> ipv4) [member function]
    cls.add_method('SetIpv4', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4 >', 'ipv4')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3OlsrRoutingTableEntry_methods(root_module, cls):
    ## olsr-routing-protocol.h: ns3::olsr::RoutingTableEntry::RoutingTableEntry(ns3::olsr::RoutingTableEntry const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::RoutingTableEntry const &', 'arg0')])
    ## olsr-routing-protocol.h: ns3::olsr::RoutingTableEntry::RoutingTableEntry() [constructor]
    cls.add_constructor([])
    ## olsr-routing-protocol.h: ns3::olsr::RoutingTableEntry::destAddr [variable]
    cls.add_instance_attribute('destAddr', 'ns3::Ipv4Address', is_const=False)
    ## olsr-routing-protocol.h: ns3::olsr::RoutingTableEntry::distance [variable]
    cls.add_instance_attribute('distance', 'uint32_t', is_const=False)
    ## olsr-routing-protocol.h: ns3::olsr::RoutingTableEntry::interface [variable]
    cls.add_instance_attribute('interface', 'uint32_t', is_const=False)
    ## olsr-routing-protocol.h: ns3::olsr::RoutingTableEntry::nextAddr [variable]
    cls.add_instance_attribute('nextAddr', 'ns3::Ipv4Address', is_const=False)
    return

def register_Ns3OlsrTopologyTuple_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## olsr-repositories.h: ns3::olsr::TopologyTuple::TopologyTuple() [constructor]
    cls.add_constructor([])
    ## olsr-repositories.h: ns3::olsr::TopologyTuple::TopologyTuple(ns3::olsr::TopologyTuple const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::TopologyTuple const &', 'arg0')])
    ## olsr-repositories.h: ns3::olsr::TopologyTuple::destAddr [variable]
    cls.add_instance_attribute('destAddr', 'ns3::Ipv4Address', is_const=False)
    ## olsr-repositories.h: ns3::olsr::TopologyTuple::expirationTime [variable]
    cls.add_instance_attribute('expirationTime', 'ns3::Time', is_const=False)
    ## olsr-repositories.h: ns3::olsr::TopologyTuple::lastAddr [variable]
    cls.add_instance_attribute('lastAddr', 'ns3::Ipv4Address', is_const=False)
    ## olsr-repositories.h: ns3::olsr::TopologyTuple::sequenceNumber [variable]
    cls.add_instance_attribute('sequenceNumber', 'uint16_t', is_const=False)
    return

def register_Ns3OlsrTwoHopNeighborTuple_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## olsr-repositories.h: ns3::olsr::TwoHopNeighborTuple::TwoHopNeighborTuple() [constructor]
    cls.add_constructor([])
    ## olsr-repositories.h: ns3::olsr::TwoHopNeighborTuple::TwoHopNeighborTuple(ns3::olsr::TwoHopNeighborTuple const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::TwoHopNeighborTuple const &', 'arg0')])
    ## olsr-repositories.h: ns3::olsr::TwoHopNeighborTuple::expirationTime [variable]
    cls.add_instance_attribute('expirationTime', 'ns3::Time', is_const=False)
    ## olsr-repositories.h: ns3::olsr::TwoHopNeighborTuple::neighborMainAddr [variable]
    cls.add_instance_attribute('neighborMainAddr', 'ns3::Ipv4Address', is_const=False)
    ## olsr-repositories.h: ns3::olsr::TwoHopNeighborTuple::twoHopNeighborAddr [variable]
    cls.add_instance_attribute('twoHopNeighborAddr', 'ns3::Ipv4Address', is_const=False)
    return

def register_functions(root_module):
    module = root_module
    root_module.begin_section('ns3_module_core')
    ns3_module_core.register_functions(root_module)
    
    try:
        import ns3_module_core__local
    except ImportError:
        pass
    else:
        ns3_module_core__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_core')
    root_module.begin_section('ns3_module_lte')
    ns3_module_lte.register_functions(root_module)
    
    try:
        import ns3_module_lte__local
    except ImportError:
        pass
    else:
        ns3_module_lte__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_lte')
    root_module.begin_section('ns3_module_simulator')
    ns3_module_simulator.register_functions(root_module)
    
    try:
        import ns3_module_simulator__local
    except ImportError:
        pass
    else:
        ns3_module_simulator__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_simulator')
    root_module.begin_section('ns3_module_test')
    ns3_module_test.register_functions(root_module)
    
    try:
        import ns3_module_test__local
    except ImportError:
        pass
    else:
        ns3_module_test__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_test')
    root_module.begin_section('ns3_module_visualizer')
    ns3_module_visualizer.register_functions(root_module)
    
    try:
        import ns3_module_visualizer__local
    except ImportError:
        pass
    else:
        ns3_module_visualizer__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_visualizer')
    root_module.begin_section('ns3_module_mobility')
    ns3_module_mobility.register_functions(root_module)
    
    try:
        import ns3_module_mobility__local
    except ImportError:
        pass
    else:
        ns3_module_mobility__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_mobility')
    root_module.begin_section('ns3_module_mpi')
    ns3_module_mpi.register_functions(root_module)
    
    try:
        import ns3_module_mpi__local
    except ImportError:
        pass
    else:
        ns3_module_mpi__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_mpi')
    root_module.begin_section('ns3_module_common')
    ns3_module_common.register_functions(root_module)
    
    try:
        import ns3_module_common__local
    except ImportError:
        pass
    else:
        ns3_module_common__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_common')
    root_module.begin_section('ns3_module_contrib')
    ns3_module_contrib.register_functions(root_module)
    
    try:
        import ns3_module_contrib__local
    except ImportError:
        pass
    else:
        ns3_module_contrib__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_contrib')
    root_module.begin_section('ns3_module_node')
    ns3_module_node.register_functions(root_module)
    
    try:
        import ns3_module_node__local
    except ImportError:
        pass
    else:
        ns3_module_node__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_node')
    root_module.begin_section('ns3_module_bridge')
    ns3_module_bridge.register_functions(root_module)
    
    try:
        import ns3_module_bridge__local
    except ImportError:
        pass
    else:
        ns3_module_bridge__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_bridge')
    root_module.begin_section('ns3_module_bulk_send')
    ns3_module_bulk_send.register_functions(root_module)
    
    try:
        import ns3_module_bulk_send__local
    except ImportError:
        pass
    else:
        ns3_module_bulk_send__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_bulk_send')
    root_module.begin_section('ns3_module_csma')
    ns3_module_csma.register_functions(root_module)
    
    try:
        import ns3_module_csma__local
    except ImportError:
        pass
    else:
        ns3_module_csma__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_csma')
    root_module.begin_section('ns3_module_emu')
    ns3_module_emu.register_functions(root_module)
    
    try:
        import ns3_module_emu__local
    except ImportError:
        pass
    else:
        ns3_module_emu__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_emu')
    root_module.begin_section('ns3_module_energy')
    ns3_module_energy.register_functions(root_module)
    
    try:
        import ns3_module_energy__local
    except ImportError:
        pass
    else:
        ns3_module_energy__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_energy')
    root_module.begin_section('ns3_module_global_routing')
    ns3_module_global_routing.register_functions(root_module)
    
    try:
        import ns3_module_global_routing__local
    except ImportError:
        pass
    else:
        ns3_module_global_routing__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_global_routing')
    root_module.begin_section('ns3_module_internet_stack')
    ns3_module_internet_stack.register_functions(root_module)
    
    try:
        import ns3_module_internet_stack__local
    except ImportError:
        pass
    else:
        ns3_module_internet_stack__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_internet_stack')
    root_module.begin_section('ns3_module_list_routing')
    ns3_module_list_routing.register_functions(root_module)
    
    try:
        import ns3_module_list_routing__local
    except ImportError:
        pass
    else:
        ns3_module_list_routing__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_list_routing')
    root_module.begin_section('ns3_module_onoff')
    ns3_module_onoff.register_functions(root_module)
    
    try:
        import ns3_module_onoff__local
    except ImportError:
        pass
    else:
        ns3_module_onoff__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_onoff')
    root_module.begin_section('ns3_module_packet_sink')
    ns3_module_packet_sink.register_functions(root_module)
    
    try:
        import ns3_module_packet_sink__local
    except ImportError:
        pass
    else:
        ns3_module_packet_sink__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_packet_sink')
    root_module.begin_section('ns3_module_point_to_point')
    ns3_module_point_to_point.register_functions(root_module)
    
    try:
        import ns3_module_point_to_point__local
    except ImportError:
        pass
    else:
        ns3_module_point_to_point__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_point_to_point')
    root_module.begin_section('ns3_module_spectrum')
    ns3_module_spectrum.register_functions(root_module)
    
    try:
        import ns3_module_spectrum__local
    except ImportError:
        pass
    else:
        ns3_module_spectrum__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_spectrum')
    root_module.begin_section('ns3_module_static_routing')
    ns3_module_static_routing.register_functions(root_module)
    
    try:
        import ns3_module_static_routing__local
    except ImportError:
        pass
    else:
        ns3_module_static_routing__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_static_routing')
    root_module.begin_section('ns3_module_stats')
    ns3_module_stats.register_functions(root_module)
    
    try:
        import ns3_module_stats__local
    except ImportError:
        pass
    else:
        ns3_module_stats__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_stats')
    root_module.begin_section('ns3_module_tap_bridge')
    ns3_module_tap_bridge.register_functions(root_module)
    
    try:
        import ns3_module_tap_bridge__local
    except ImportError:
        pass
    else:
        ns3_module_tap_bridge__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_tap_bridge')
    root_module.begin_section('ns3_module_topology_read')
    ns3_module_topology_read.register_functions(root_module)
    
    try:
        import ns3_module_topology_read__local
    except ImportError:
        pass
    else:
        ns3_module_topology_read__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_topology_read')
    root_module.begin_section('ns3_module_v4ping')
    ns3_module_v4ping.register_functions(root_module)
    
    try:
        import ns3_module_v4ping__local
    except ImportError:
        pass
    else:
        ns3_module_v4ping__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_v4ping')
    root_module.begin_section('ns3_module_virtual_net_device')
    ns3_module_virtual_net_device.register_functions(root_module)
    
    try:
        import ns3_module_virtual_net_device__local
    except ImportError:
        pass
    else:
        ns3_module_virtual_net_device__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_virtual_net_device')
    root_module.begin_section('ns3_module_wifi')
    ns3_module_wifi.register_functions(root_module)
    
    try:
        import ns3_module_wifi__local
    except ImportError:
        pass
    else:
        ns3_module_wifi__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_wifi')
    root_module.begin_section('ns3_module_wimax')
    ns3_module_wimax.register_functions(root_module)
    
    try:
        import ns3_module_wimax__local
    except ImportError:
        pass
    else:
        ns3_module_wimax__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_wimax')
    root_module.begin_section('ns3_module_uan')
    ns3_module_uan.register_functions(root_module)
    
    try:
        import ns3_module_uan__local
    except ImportError:
        pass
    else:
        ns3_module_uan__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_uan')
    root_module.begin_section('ns3_module_aodv')
    ns3_module_aodv.register_functions(root_module)
    
    try:
        import ns3_module_aodv__local
    except ImportError:
        pass
    else:
        ns3_module_aodv__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_aodv')
    root_module.begin_section('ns3_module_flow_monitor')
    ns3_module_flow_monitor.register_functions(root_module)
    
    try:
        import ns3_module_flow_monitor__local
    except ImportError:
        pass
    else:
        ns3_module_flow_monitor__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_flow_monitor')
    root_module.begin_section('ns3_module_nix_vector_routing')
    ns3_module_nix_vector_routing.register_functions(root_module)
    
    try:
        import ns3_module_nix_vector_routing__local
    except ImportError:
        pass
    else:
        ns3_module_nix_vector_routing__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_nix_vector_routing')
    root_module.begin_section('ns3_module_olsr')
    ns3_module_olsr.register_functions(root_module)
    
    try:
        import ns3_module_olsr__local
    except ImportError:
        pass
    else:
        ns3_module_olsr__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_olsr')
    root_module.begin_section('ns3_module_ping6')
    ns3_module_ping6.register_functions(root_module)
    
    try:
        import ns3_module_ping6__local
    except ImportError:
        pass
    else:
        ns3_module_ping6__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_ping6')
    root_module.begin_section('ns3_module_radvd')
    ns3_module_radvd.register_functions(root_module)
    
    try:
        import ns3_module_radvd__local
    except ImportError:
        pass
    else:
        ns3_module_radvd__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_radvd')
    root_module.begin_section('ns3_module_udp_client_server')
    ns3_module_udp_client_server.register_functions(root_module)
    
    try:
        import ns3_module_udp_client_server__local
    except ImportError:
        pass
    else:
        ns3_module_udp_client_server__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_udp_client_server')
    root_module.begin_section('ns3_module_udp_echo')
    ns3_module_udp_echo.register_functions(root_module)
    
    try:
        import ns3_module_udp_echo__local
    except ImportError:
        pass
    else:
        ns3_module_udp_echo__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_udp_echo')
    root_module.begin_section('ns3_module_dsdv')
    ns3_module_dsdv.register_functions(root_module)
    
    try:
        import ns3_module_dsdv__local
    except ImportError:
        pass
    else:
        ns3_module_dsdv__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_dsdv')
    root_module.begin_section('ns3_module_mesh')
    ns3_module_mesh.register_functions(root_module)
    
    try:
        import ns3_module_mesh__local
    except ImportError:
        pass
    else:
        ns3_module_mesh__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_mesh')
    root_module.begin_section('ns3_module_helper')
    ns3_module_helper.register_functions(root_module)
    
    try:
        import ns3_module_helper__local
    except ImportError:
        pass
    else:
        ns3_module_helper__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_helper')
    root_module.begin_section('ns3_module_dot11s')
    ns3_module_dot11s.register_functions(root_module)
    
    try:
        import ns3_module_dot11s__local
    except ImportError:
        pass
    else:
        ns3_module_dot11s__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_dot11s')
    root_module.begin_section('ns3_module_flame')
    ns3_module_flame.register_functions(root_module)
    
    try:
        import ns3_module_flame__local
    except ImportError:
        pass
    else:
        ns3_module_flame__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_flame')
    ## uan-tx-mode.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeUanModesListChecker() [free function]
    module.add_function('MakeUanModesListChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    register_functions_ns3_Config(module.get_submodule('Config'), root_module)
    register_functions_ns3_FatalImpl(module.get_submodule('FatalImpl'), root_module)
    register_functions_ns3_addressUtils(module.get_submodule('addressUtils'), root_module)
    register_functions_ns3_aodv(module.get_submodule('aodv'), root_module)
    register_functions_ns3_dot11s(module.get_submodule('dot11s'), root_module)
    register_functions_ns3_dsdv(module.get_submodule('dsdv'), root_module)
    register_functions_ns3_flame(module.get_submodule('flame'), root_module)
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    register_functions_ns3_olsr(module.get_submodule('olsr'), root_module)
    return

def register_functions_ns3_Config(module, root_module):
    return

def register_functions_ns3_FatalImpl(module, root_module):
    return

def register_functions_ns3_addressUtils(module, root_module):
    return

def register_functions_ns3_aodv(module, root_module):
    return

def register_functions_ns3_dot11s(module, root_module):
    return

def register_functions_ns3_dsdv(module, root_module):
    return

def register_functions_ns3_flame(module, root_module):
    return

def register_functions_ns3_internal(module, root_module):
    return

def register_functions_ns3_olsr(module, root_module):
    ## olsr-header.h: extern double ns3::olsr::EmfToSeconds(uint8_t emf) [free function]
    module.add_function('EmfToSeconds', 
                        'double', 
                        [param('uint8_t', 'emf')])
    ## olsr-header.h: extern uint8_t ns3::olsr::SecondsToEmf(double seconds) [free function]
    module.add_function('SecondsToEmf', 
                        'uint8_t', 
                        [param('double', 'seconds')])
    return

def main():
    out = FileCodeSink(sys.stdout)
    root_module = module_init()
    register_types(root_module)
    register_methods(root_module)
    register_functions(root_module)
    root_module.generate(out)

if __name__ == '__main__':
    main()

