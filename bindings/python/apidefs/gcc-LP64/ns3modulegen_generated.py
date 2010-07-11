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
import ns3_module_simulator
import ns3_module_test
import ns3_module_common
import ns3_module_mobility
import ns3_module_mpi
import ns3_module_contrib
import ns3_module_node
import ns3_module_bridge
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
import ns3_module_uan
import ns3_module_v4ping
import ns3_module_virtual_net_device
import ns3_module_wifi
import ns3_module_wimax
import ns3_module_aodv
import ns3_module_flow_monitor
import ns3_module_nix_vector_routing
import ns3_module_olsr
import ns3_module_ping6
import ns3_module_radvd
import ns3_module_udp_client_server
import ns3_module_udp_echo
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
    root_module.begin_section('ns3_module_common')
    ns3_module_common.register_types(module)
    
    try:
        import ns3_module_common__local
    except ImportError:
        pass
    else:
        ns3_module_common__local.register_types(module)
    
    root_module.end_section('ns3_module_common')
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
    root_module.begin_section('ns3_module_uan')
    ns3_module_uan.register_types(module)
    
    try:
        import ns3_module_uan__local
    except ImportError:
        pass
    else:
        ns3_module_uan__local.register_types(module)
    
    root_module.end_section('ns3_module_uan')
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
    ## energy-model-helper.h: ns3::DeviceEnergyModelHelper [class]
    module.add_class('DeviceEnergyModelHelper', allow_subclassing=True)
    ## energy-model-helper.h: ns3::EnergyModelHelper [class]
    module.add_class('EnergyModelHelper')
    ## energy-model-helper.h: ns3::EnergySourceHelper [class]
    module.add_class('EnergySourceHelper', allow_subclassing=True)
    ## basic-energy-source-helper.h: ns3::BasicEnergySourceHelper [class]
    module.add_class('BasicEnergySourceHelper', parent=root_module['ns3::EnergySourceHelper'])
    ## basic-radio-energy-model-helper.h: ns3::BasicRadioEnergyModelHelper [class]
    module.add_class('BasicRadioEnergyModelHelper', parent=root_module['ns3::DeviceEnergyModelHelper'])
    ## device-energy-model.h: ns3::DeviceEnergyModel [class]
    module.add_class('DeviceEnergyModel', parent=root_module['ns3::Object'])
    ## energy-source.h: ns3::EnergySource [class]
    module.add_class('EnergySource', parent=root_module['ns3::Object'])
    ## radio-energy-model.h: ns3::RadioEnergyModel [class]
    module.add_class('RadioEnergyModel', parent=root_module['ns3::DeviceEnergyModel'])
    ## radio-energy-model.h: ns3::RadioEnergyModel::RadioState [enumeration]
    module.add_enum('RadioState', ['TX', 'RX', 'IDLE', 'SLEEP'], outer_class=root_module['ns3::RadioEnergyModel'])
    ## basic-energy-source.h: ns3::BasicEnergySource [class]
    module.add_class('BasicEnergySource', parent=root_module['ns3::EnergySource'])
    ## basic-radio-energy-model.h: ns3::BasicRadioEnergyModel [class]
    module.add_class('BasicRadioEnergyModel', parent=root_module['ns3::RadioEnergyModel'])
    module.add_container('std::vector< unsigned int >', 'unsigned int', container_type='vector')
    module.add_container('std::vector< bool >', 'bool', container_type='vector')
    module.add_container('std::list< std::pair< ns3::Ptr< ns3::Packet >, ns3::UanAddress > >', 'std::pair< ns3::Ptr< ns3::Packet >, ns3::UanAddress >', container_type='list')
    module.add_container('std::vector< ns3::ServiceFlow * >', 'ns3::ServiceFlow *', container_type='vector')
    module.add_container('std::vector< std::complex< double > >', 'std::complex< double >', container_type='vector')
    module.add_container('std::vector< double >', 'double', container_type='vector')
    module.add_container('ns3::bvec', 'bool', container_type='vector')
    module.add_container('std::vector< ns3::SSRecord * >', 'ns3::SSRecord *', container_type='vector')
    module.add_container('std::map< std::string, std::string >', ('std::string', 'std::string'), container_type='map')
    module.add_container('std::set< unsigned char >', 'unsigned char', container_type='set')
    module.add_container('std::list< std::pair< ns3::OfdmDlMapIe *, ns3::Ptr< ns3::PacketBurst > > >', 'std::pair< ns3::OfdmDlMapIe *, ns3::Ptr< ns3::PacketBurst > >', container_type='list')
    module.add_container('std::vector< unsigned long >', 'long unsigned int', container_type='vector')
    module.add_container('std::list< unsigned int >', 'unsigned int', container_type='list')
    module.add_container('std::list< std::pair< ns3::Ptr< ns3::Packet >, ns3::AmsduSubframeHeader > >', 'std::pair< ns3::Ptr< ns3::Packet >, ns3::AmsduSubframeHeader >', container_type='list')
    module.add_container('std::vector< std::pair< ns3::Ptr< ns3::UanNetDevice >, ns3::Ptr< ns3::UanTransducer > > >', 'std::pair< ns3::Ptr< ns3::UanNetDevice >, ns3::Ptr< ns3::UanTransducer > >', container_type='vector')
    
    ## Register a nested module for the namespace Config
    
    nested_module = module.add_cpp_namespace('Config')
    register_types_ns3_Config(nested_module)
    
    
    ## Register a nested module for the namespace FatalImpl
    
    nested_module = module.add_cpp_namespace('FatalImpl')
    register_types_ns3_FatalImpl(nested_module)
    
    
    ## Register a nested module for the namespace TimeStepPrecision
    
    nested_module = module.add_cpp_namespace('TimeStepPrecision')
    register_types_ns3_TimeStepPrecision(nested_module)
    
    
    ## Register a nested module for the namespace addressUtils
    
    nested_module = module.add_cpp_namespace('addressUtils')
    register_types_ns3_addressUtils(nested_module)
    
    
    ## Register a nested module for the namespace aodv
    
    nested_module = module.add_cpp_namespace('aodv')
    register_types_ns3_aodv(nested_module)
    
    
    ## Register a nested module for the namespace dot11s
    
    nested_module = module.add_cpp_namespace('dot11s')
    register_types_ns3_dot11s(nested_module)
    
    
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
    
    module.add_container('std::vector< std::string >', 'std::string', container_type='vector')

def register_types_ns3_FatalImpl(module):
    root_module = module.get_root()
    

def register_types_ns3_TimeStepPrecision(module):
    root_module = module.get_root()
    

def register_types_ns3_addressUtils(module):
    root_module = module.get_root()
    

def register_types_ns3_aodv(module):
    root_module = module.get_root()
    
    module.add_container('std::map< ns3::Ipv4Address, unsigned int >', ('ns3::Ipv4Address', 'unsigned int'), container_type='map')

def register_types_ns3_dot11s(module):
    root_module = module.get_root()
    

def register_types_ns3_flame(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    
    module.add_container('std::set< unsigned int >', 'unsigned int', container_type='set')

def register_methods(root_module):
    register_Ns3DeviceEnergyModelHelper_methods(root_module, root_module['ns3::DeviceEnergyModelHelper'])
    register_Ns3EnergyModelHelper_methods(root_module, root_module['ns3::EnergyModelHelper'])
    register_Ns3EnergySourceHelper_methods(root_module, root_module['ns3::EnergySourceHelper'])
    register_Ns3BasicEnergySourceHelper_methods(root_module, root_module['ns3::BasicEnergySourceHelper'])
    register_Ns3BasicRadioEnergyModelHelper_methods(root_module, root_module['ns3::BasicRadioEnergyModelHelper'])
    register_Ns3DeviceEnergyModel_methods(root_module, root_module['ns3::DeviceEnergyModel'])
    register_Ns3EnergySource_methods(root_module, root_module['ns3::EnergySource'])
    register_Ns3RadioEnergyModel_methods(root_module, root_module['ns3::RadioEnergyModel'])
    register_Ns3BasicEnergySource_methods(root_module, root_module['ns3::BasicEnergySource'])
    register_Ns3BasicRadioEnergyModel_methods(root_module, root_module['ns3::BasicRadioEnergyModel'])
    root_module.begin_section('ns3_module_core')
    ns3_module_core.register_methods(root_module)
    
    try:
        import ns3_module_core__local
    except ImportError:
        pass
    else:
        ns3_module_core__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_core')
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
    root_module.begin_section('ns3_module_common')
    ns3_module_common.register_methods(root_module)
    
    try:
        import ns3_module_common__local
    except ImportError:
        pass
    else:
        ns3_module_common__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_common')
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
    root_module.begin_section('ns3_module_uan')
    ns3_module_uan.register_methods(root_module)
    
    try:
        import ns3_module_uan__local
    except ImportError:
        pass
    else:
        ns3_module_uan__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_uan')
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

def register_Ns3DeviceEnergyModelHelper_methods(root_module, cls):
    ## energy-model-helper.h: ns3::DeviceEnergyModelHelper::DeviceEnergyModelHelper() [constructor]
    cls.add_constructor([])
    ## energy-model-helper.h: ns3::DeviceEnergyModelHelper::DeviceEnergyModelHelper(ns3::DeviceEnergyModelHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DeviceEnergyModelHelper const &', 'arg0')])
    ## energy-model-helper.h: ns3::Ptr<ns3::DeviceEnergyModel> ns3::DeviceEnergyModelHelper::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::DeviceEnergyModel >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## energy-model-helper.h: void ns3::DeviceEnergyModelHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## energy-model-helper.h: void ns3::DeviceEnergyModelHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## energy-model-helper.h: void ns3::DeviceEnergyModelHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## energy-model-helper.h: void ns3::DeviceEnergyModelHelper::InstallAll() const [member function]
    cls.add_method('InstallAll', 
                   'void', 
                   [], 
                   is_const=True)
    ## energy-model-helper.h: void ns3::DeviceEnergyModelHelper::Set(std::string name, ns3::AttributeValue const & v) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'v')], 
                   is_pure_virtual=True, is_virtual=True)
    ## energy-model-helper.h: void ns3::DeviceEnergyModelHelper::DoInstall(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('DoInstall', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3EnergyModelHelper_methods(root_module, cls):
    ## energy-model-helper.h: ns3::EnergyModelHelper::EnergyModelHelper(ns3::EnergyModelHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EnergyModelHelper const &', 'arg0')])
    ## energy-model-helper.h: ns3::EnergyModelHelper::EnergyModelHelper() [constructor]
    cls.add_constructor([])
    ## energy-model-helper.h: void ns3::EnergyModelHelper::Install(ns3::EnergySourceHelper const & sourceHelper, ns3::DeviceEnergyModelHelper const & modelHelper, ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::EnergySourceHelper const &', 'sourceHelper'), param('ns3::DeviceEnergyModelHelper const &', 'modelHelper'), param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## energy-model-helper.h: void ns3::EnergyModelHelper::Install(ns3::EnergySourceHelper const & sourceHelper, ns3::DeviceEnergyModelHelper const & modelHelper, ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::EnergySourceHelper const &', 'sourceHelper'), param('ns3::DeviceEnergyModelHelper const &', 'modelHelper'), param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## energy-model-helper.h: void ns3::EnergyModelHelper::Install(ns3::EnergySourceHelper const & sourceHelper, ns3::DeviceEnergyModelHelper const & modelHelper, std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::EnergySourceHelper const &', 'sourceHelper'), param('ns3::DeviceEnergyModelHelper const &', 'modelHelper'), param('std::string', 'nodeName')], 
                   is_const=True)
    ## energy-model-helper.h: void ns3::EnergyModelHelper::InstallAll(ns3::EnergySourceHelper const & sourceHelper, ns3::DeviceEnergyModelHelper const & modelHelper) const [member function]
    cls.add_method('InstallAll', 
                   'void', 
                   [param('ns3::EnergySourceHelper const &', 'sourceHelper'), param('ns3::DeviceEnergyModelHelper const &', 'modelHelper')], 
                   is_const=True)
    return

def register_Ns3EnergySourceHelper_methods(root_module, cls):
    ## energy-model-helper.h: ns3::EnergySourceHelper::EnergySourceHelper() [constructor]
    cls.add_constructor([])
    ## energy-model-helper.h: ns3::EnergySourceHelper::EnergySourceHelper(ns3::EnergySourceHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EnergySourceHelper const &', 'arg0')])
    ## energy-model-helper.h: ns3::Ptr<ns3::EnergySource> ns3::EnergySourceHelper::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::EnergySource >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## energy-model-helper.h: void ns3::EnergySourceHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3BasicEnergySourceHelper_methods(root_module, cls):
    ## basic-energy-source-helper.h: ns3::BasicEnergySourceHelper::BasicEnergySourceHelper(ns3::BasicEnergySourceHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BasicEnergySourceHelper const &', 'arg0')])
    ## basic-energy-source-helper.h: ns3::BasicEnergySourceHelper::BasicEnergySourceHelper() [constructor]
    cls.add_constructor([])
    ## basic-energy-source-helper.h: ns3::Ptr<ns3::EnergySource> ns3::BasicEnergySourceHelper::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::EnergySource >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## basic-energy-source-helper.h: void ns3::BasicEnergySourceHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True, is_virtual=True)
    ## basic-energy-source-helper.h: void ns3::BasicEnergySourceHelper::Set(std::string name, ns3::AttributeValue const & v) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'v')])
    return

def register_Ns3BasicRadioEnergyModelHelper_methods(root_module, cls):
    ## basic-radio-energy-model-helper.h: ns3::BasicRadioEnergyModelHelper::BasicRadioEnergyModelHelper(ns3::BasicRadioEnergyModelHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BasicRadioEnergyModelHelper const &', 'arg0')])
    ## basic-radio-energy-model-helper.h: ns3::BasicRadioEnergyModelHelper::BasicRadioEnergyModelHelper() [constructor]
    cls.add_constructor([])
    ## basic-radio-energy-model-helper.h: ns3::Ptr<ns3::DeviceEnergyModel> ns3::BasicRadioEnergyModelHelper::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::DeviceEnergyModel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## basic-radio-energy-model-helper.h: void ns3::BasicRadioEnergyModelHelper::Set(std::string name, ns3::AttributeValue const & v) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'v')], 
                   is_virtual=True)
    ## basic-radio-energy-model-helper.h: void ns3::BasicRadioEnergyModelHelper::SetDepletionCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetDepletionCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## basic-radio-energy-model-helper.h: void ns3::BasicRadioEnergyModelHelper::DoInstall(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('DoInstall', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3DeviceEnergyModel_methods(root_module, cls):
    ## device-energy-model.h: ns3::DeviceEnergyModel::DeviceEnergyModel(ns3::DeviceEnergyModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DeviceEnergyModel const &', 'arg0')])
    ## device-energy-model.h: ns3::DeviceEnergyModel::DeviceEnergyModel() [constructor]
    cls.add_constructor([])
    ## device-energy-model.h: static ns3::TypeId ns3::DeviceEnergyModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## device-energy-model.h: void ns3::DeviceEnergyModel::HandleEnergyDepletion() [member function]
    cls.add_method('HandleEnergyDepletion', 
                   'void', 
                   [])
    ## device-energy-model.h: void ns3::DeviceEnergyModel::SetEnergySource(ns3::Ptr<ns3::EnergySource> source) [member function]
    cls.add_method('SetEnergySource', 
                   'void', 
                   [param('ns3::Ptr< ns3::EnergySource >', 'source')])
    ## device-energy-model.h: void ns3::DeviceEnergyModel::BreakSourceRefCycle() [member function]
    cls.add_method('BreakSourceRefCycle', 
                   'void', 
                   [], 
                   visibility='protected')
    ## device-energy-model.h: void ns3::DeviceEnergyModel::DecreaseRemainingEnergy(double energyJ) [member function]
    cls.add_method('DecreaseRemainingEnergy', 
                   'void', 
                   [param('double', 'energyJ')], 
                   visibility='protected')
    ## device-energy-model.h: void ns3::DeviceEnergyModel::IncreaseRemainingEnergy(double energyJ) [member function]
    cls.add_method('IncreaseRemainingEnergy', 
                   'void', 
                   [param('double', 'energyJ')], 
                   visibility='protected')
    ## device-energy-model.h: void ns3::DeviceEnergyModel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## device-energy-model.h: void ns3::DeviceEnergyModel::DoHandleEnergyDepletion() [member function]
    cls.add_method('DoHandleEnergyDepletion', 
                   'void', 
                   [], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
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
    ## energy-source.h: void ns3::EnergySource::DecreaseRemainingEnergy(double energyJ) [member function]
    cls.add_method('DecreaseRemainingEnergy', 
                   'void', 
                   [param('double', 'energyJ')])
    ## energy-source.h: std::vector<ns3::Ptr<ns3::DeviceEnergyModel>, std::allocator<ns3::Ptr<ns3::DeviceEnergyModel> > > ns3::EnergySource::FindDeviceEnergyModels(ns3::TypeId tid) [member function]
    cls.add_method('FindDeviceEnergyModels', 
                   'std::vector< ns3::Ptr< ns3::DeviceEnergyModel > >', 
                   [param('ns3::TypeId', 'tid')])
    ## energy-source.h: std::vector<ns3::Ptr<ns3::DeviceEnergyModel>, std::allocator<ns3::Ptr<ns3::DeviceEnergyModel> > > ns3::EnergySource::FindDeviceEnergyModels(std::string name) [member function]
    cls.add_method('FindDeviceEnergyModels', 
                   'std::vector< ns3::Ptr< ns3::DeviceEnergyModel > >', 
                   [param('std::string', 'name')])
    ## energy-source.h: double ns3::EnergySource::GetEnergyFraction() const [member function]
    cls.add_method('GetEnergyFraction', 
                   'double', 
                   [], 
                   is_const=True)
    ## energy-source.h: double ns3::EnergySource::GetInitialEnergy() const [member function]
    cls.add_method('GetInitialEnergy', 
                   'double', 
                   [], 
                   is_const=True)
    ## energy-source.h: double ns3::EnergySource::GetRemainingEnergy() const [member function]
    cls.add_method('GetRemainingEnergy', 
                   'double', 
                   [], 
                   is_const=True)
    ## energy-source.h: static ns3::TypeId ns3::EnergySource::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## energy-source.h: void ns3::EnergySource::IncreaseRemainingEnergy(double energyJ) [member function]
    cls.add_method('IncreaseRemainingEnergy', 
                   'void', 
                   [param('double', 'energyJ')])
    ## energy-source.h: void ns3::EnergySource::BreakDeviceEnergyModelRefCycle() [member function]
    cls.add_method('BreakDeviceEnergyModelRefCycle', 
                   'void', 
                   [], 
                   visibility='protected')
    ## energy-source.h: void ns3::EnergySource::NotifyEnergyDrained() [member function]
    cls.add_method('NotifyEnergyDrained', 
                   'void', 
                   [], 
                   visibility='protected')
    ## energy-source.h: void ns3::EnergySource::DoDecreaseRemainingEnergy(double energyJ) [member function]
    cls.add_method('DoDecreaseRemainingEnergy', 
                   'void', 
                   [param('double', 'energyJ')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## energy-source.h: void ns3::EnergySource::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## energy-source.h: double ns3::EnergySource::DoGetEnergyFraction() const [member function]
    cls.add_method('DoGetEnergyFraction', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## energy-source.h: double ns3::EnergySource::DoGetInitialEnergy() const [member function]
    cls.add_method('DoGetInitialEnergy', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## energy-source.h: double ns3::EnergySource::DoGetRemainingEnergy() const [member function]
    cls.add_method('DoGetRemainingEnergy', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## energy-source.h: void ns3::EnergySource::DoIncreaseRemainingEnergy(double energyJ) [member function]
    cls.add_method('DoIncreaseRemainingEnergy', 
                   'void', 
                   [param('double', 'energyJ')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    return

def register_Ns3RadioEnergyModel_methods(root_module, cls):
    ## radio-energy-model.h: ns3::RadioEnergyModel::RadioEnergyModel(ns3::RadioEnergyModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RadioEnergyModel const &', 'arg0')])
    ## radio-energy-model.h: ns3::RadioEnergyModel::RadioEnergyModel() [constructor]
    cls.add_constructor([])
    ## radio-energy-model.h: double ns3::RadioEnergyModel::GetIdlePowerW() const [member function]
    cls.add_method('GetIdlePowerW', 
                   'double', 
                   [], 
                   is_const=True)
    ## radio-energy-model.h: double ns3::RadioEnergyModel::GetRxPowerW() const [member function]
    cls.add_method('GetRxPowerW', 
                   'double', 
                   [], 
                   is_const=True)
    ## radio-energy-model.h: double ns3::RadioEnergyModel::GetSleepPowerW() const [member function]
    cls.add_method('GetSleepPowerW', 
                   'double', 
                   [], 
                   is_const=True)
    ## radio-energy-model.h: double ns3::RadioEnergyModel::GetTxPowerW() const [member function]
    cls.add_method('GetTxPowerW', 
                   'double', 
                   [], 
                   is_const=True)
    ## radio-energy-model.h: static ns3::TypeId ns3::RadioEnergyModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## radio-energy-model.h: void ns3::RadioEnergyModel::SetIdlePowerW(double idlePowerW) [member function]
    cls.add_method('SetIdlePowerW', 
                   'void', 
                   [param('double', 'idlePowerW')])
    ## radio-energy-model.h: void ns3::RadioEnergyModel::SetRxPowerW(double rxPowerW) [member function]
    cls.add_method('SetRxPowerW', 
                   'void', 
                   [param('double', 'rxPowerW')])
    ## radio-energy-model.h: void ns3::RadioEnergyModel::SetSleepPowerW(double sleepPowerW) [member function]
    cls.add_method('SetSleepPowerW', 
                   'void', 
                   [param('double', 'sleepPowerW')])
    ## radio-energy-model.h: void ns3::RadioEnergyModel::SetTxPowerW(double txPowerW) [member function]
    cls.add_method('SetTxPowerW', 
                   'void', 
                   [param('double', 'txPowerW')])
    ## radio-energy-model.h: void ns3::RadioEnergyModel::UpdateRemainingEnergy(ns3::RadioEnergyModel::RadioState const destState) [member function]
    cls.add_method('UpdateRemainingEnergy', 
                   'void', 
                   [param('ns3::RadioEnergyModel::RadioState const', 'destState')])
    ## radio-energy-model.h: double ns3::RadioEnergyModel::DoGetIdlePowerW() const [member function]
    cls.add_method('DoGetIdlePowerW', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## radio-energy-model.h: double ns3::RadioEnergyModel::DoGetRxPowerW() const [member function]
    cls.add_method('DoGetRxPowerW', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## radio-energy-model.h: double ns3::RadioEnergyModel::DoGetSleepPowerW() const [member function]
    cls.add_method('DoGetSleepPowerW', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## radio-energy-model.h: double ns3::RadioEnergyModel::DoGetTxPowerW() const [member function]
    cls.add_method('DoGetTxPowerW', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## radio-energy-model.h: void ns3::RadioEnergyModel::DoSetIdlePowerW(double idlePowerW) [member function]
    cls.add_method('DoSetIdlePowerW', 
                   'void', 
                   [param('double', 'idlePowerW')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## radio-energy-model.h: void ns3::RadioEnergyModel::DoSetRxPowerW(double rxPowerW) [member function]
    cls.add_method('DoSetRxPowerW', 
                   'void', 
                   [param('double', 'rxPowerW')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## radio-energy-model.h: void ns3::RadioEnergyModel::DoSetSleepPowerW(double sleepPowerW) [member function]
    cls.add_method('DoSetSleepPowerW', 
                   'void', 
                   [param('double', 'sleepPowerW')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## radio-energy-model.h: void ns3::RadioEnergyModel::DoSetTxPowerW(double txPowerW) [member function]
    cls.add_method('DoSetTxPowerW', 
                   'void', 
                   [param('double', 'txPowerW')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## radio-energy-model.h: void ns3::RadioEnergyModel::DoUpdateRemainingEnergy(ns3::RadioEnergyModel::RadioState const destState) [member function]
    cls.add_method('DoUpdateRemainingEnergy', 
                   'void', 
                   [param('ns3::RadioEnergyModel::RadioState const', 'destState')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    return

def register_Ns3BasicEnergySource_methods(root_module, cls):
    ## basic-energy-source.h: ns3::BasicEnergySource::BasicEnergySource(ns3::BasicEnergySource const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BasicEnergySource const &', 'arg0')])
    ## basic-energy-source.h: ns3::BasicEnergySource::BasicEnergySource() [constructor]
    cls.add_constructor([])
    ## basic-energy-source.h: static ns3::TypeId ns3::BasicEnergySource::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## basic-energy-source.h: void ns3::BasicEnergySource::SetInitialEnergy(double initialEnergyJ) [member function]
    cls.add_method('SetInitialEnergy', 
                   'void', 
                   [param('double', 'initialEnergyJ')])
    ## basic-energy-source.h: void ns3::BasicEnergySource::DoDecreaseRemainingEnergy(double energyJ) [member function]
    cls.add_method('DoDecreaseRemainingEnergy', 
                   'void', 
                   [param('double', 'energyJ')], 
                   visibility='private', is_virtual=True)
    ## basic-energy-source.h: void ns3::BasicEnergySource::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## basic-energy-source.h: double ns3::BasicEnergySource::DoGetEnergyFraction() const [member function]
    cls.add_method('DoGetEnergyFraction', 
                   'double', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## basic-energy-source.h: double ns3::BasicEnergySource::DoGetInitialEnergy() const [member function]
    cls.add_method('DoGetInitialEnergy', 
                   'double', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## basic-energy-source.h: double ns3::BasicEnergySource::DoGetRemainingEnergy() const [member function]
    cls.add_method('DoGetRemainingEnergy', 
                   'double', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## basic-energy-source.h: void ns3::BasicEnergySource::DoIncreaseRemainingEnergy(double energyJ) [member function]
    cls.add_method('DoIncreaseRemainingEnergy', 
                   'void', 
                   [param('double', 'energyJ')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3BasicRadioEnergyModel_methods(root_module, cls):
    ## basic-radio-energy-model.h: ns3::BasicRadioEnergyModel::BasicRadioEnergyModel(ns3::BasicRadioEnergyModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BasicRadioEnergyModel const &', 'arg0')])
    ## basic-radio-energy-model.h: ns3::BasicRadioEnergyModel::BasicRadioEnergyModel() [constructor]
    cls.add_constructor([])
    ## basic-radio-energy-model.h: ns3::RadioEnergyModel::RadioState ns3::BasicRadioEnergyModel::GetCurrentState() const [member function]
    cls.add_method('GetCurrentState', 
                   'ns3::RadioEnergyModel::RadioState', 
                   [], 
                   is_const=True)
    ## basic-radio-energy-model.h: ns3::Time ns3::BasicRadioEnergyModel::GetEnergyUpdateInterval() const [member function]
    cls.add_method('GetEnergyUpdateInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## basic-radio-energy-model.h: static ns3::TypeId ns3::BasicRadioEnergyModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## basic-radio-energy-model.h: bool ns3::BasicRadioEnergyModel::IsStateTransitionValid(ns3::RadioEnergyModel::RadioState const destState) [member function]
    cls.add_method('IsStateTransitionValid', 
                   'bool', 
                   [param('ns3::RadioEnergyModel::RadioState const', 'destState')])
    ## basic-radio-energy-model.h: void ns3::BasicRadioEnergyModel::SetEnergyDepletionCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetEnergyDepletionCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## basic-radio-energy-model.h: void ns3::BasicRadioEnergyModel::SetEnergyUpdateInterval(ns3::Time const interval) [member function]
    cls.add_method('SetEnergyUpdateInterval', 
                   'void', 
                   [param('ns3::Time const', 'interval')])
    ## basic-radio-energy-model.h: void ns3::BasicRadioEnergyModel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## basic-radio-energy-model.h: double ns3::BasicRadioEnergyModel::DoGetIdlePowerW() const [member function]
    cls.add_method('DoGetIdlePowerW', 
                   'double', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## basic-radio-energy-model.h: double ns3::BasicRadioEnergyModel::DoGetRxPowerW() const [member function]
    cls.add_method('DoGetRxPowerW', 
                   'double', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## basic-radio-energy-model.h: double ns3::BasicRadioEnergyModel::DoGetSleepPowerW() const [member function]
    cls.add_method('DoGetSleepPowerW', 
                   'double', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## basic-radio-energy-model.h: double ns3::BasicRadioEnergyModel::DoGetTxPowerW() const [member function]
    cls.add_method('DoGetTxPowerW', 
                   'double', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## basic-radio-energy-model.h: void ns3::BasicRadioEnergyModel::DoHandleEnergyDepletion() [member function]
    cls.add_method('DoHandleEnergyDepletion', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## basic-radio-energy-model.h: void ns3::BasicRadioEnergyModel::DoSetIdlePowerW(double idlePowerW) [member function]
    cls.add_method('DoSetIdlePowerW', 
                   'void', 
                   [param('double', 'idlePowerW')], 
                   visibility='private', is_virtual=True)
    ## basic-radio-energy-model.h: void ns3::BasicRadioEnergyModel::DoSetRxPowerW(double rxPowerW) [member function]
    cls.add_method('DoSetRxPowerW', 
                   'void', 
                   [param('double', 'rxPowerW')], 
                   visibility='private', is_virtual=True)
    ## basic-radio-energy-model.h: void ns3::BasicRadioEnergyModel::DoSetSleepPowerW(double sleepPowerW) [member function]
    cls.add_method('DoSetSleepPowerW', 
                   'void', 
                   [param('double', 'sleepPowerW')], 
                   visibility='private', is_virtual=True)
    ## basic-radio-energy-model.h: void ns3::BasicRadioEnergyModel::DoSetTxPowerW(double txPowerW) [member function]
    cls.add_method('DoSetTxPowerW', 
                   'void', 
                   [param('double', 'txPowerW')], 
                   visibility='private', is_virtual=True)
    ## basic-radio-energy-model.h: void ns3::BasicRadioEnergyModel::DoUpdateRemainingEnergy(ns3::RadioEnergyModel::RadioState const destState) [member function]
    cls.add_method('DoUpdateRemainingEnergy', 
                   'void', 
                   [param('ns3::RadioEnergyModel::RadioState const', 'destState')], 
                   visibility='private', is_virtual=True)
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
    root_module.begin_section('ns3_module_common')
    ns3_module_common.register_functions(root_module)
    
    try:
        import ns3_module_common__local
    except ImportError:
        pass
    else:
        ns3_module_common__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_common')
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
    root_module.begin_section('ns3_module_uan')
    ns3_module_uan.register_functions(root_module)
    
    try:
        import ns3_module_uan__local
    except ImportError:
        pass
    else:
        ns3_module_uan__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_uan')
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
    register_functions_ns3_Config(module.get_submodule('Config'), root_module)
    register_functions_ns3_FatalImpl(module.get_submodule('FatalImpl'), root_module)
    register_functions_ns3_TimeStepPrecision(module.get_submodule('TimeStepPrecision'), root_module)
    register_functions_ns3_addressUtils(module.get_submodule('addressUtils'), root_module)
    register_functions_ns3_aodv(module.get_submodule('aodv'), root_module)
    register_functions_ns3_dot11s(module.get_submodule('dot11s'), root_module)
    register_functions_ns3_flame(module.get_submodule('flame'), root_module)
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    register_functions_ns3_olsr(module.get_submodule('olsr'), root_module)
    return

def register_functions_ns3_Config(module, root_module):
    return

def register_functions_ns3_FatalImpl(module, root_module):
    return

def register_functions_ns3_TimeStepPrecision(module, root_module):
    return

def register_functions_ns3_addressUtils(module, root_module):
    return

def register_functions_ns3_aodv(module, root_module):
    return

def register_functions_ns3_dot11s(module, root_module):
    return

def register_functions_ns3_flame(module, root_module):
    return

def register_functions_ns3_internal(module, root_module):
    return

def register_functions_ns3_olsr(module, root_module):
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

