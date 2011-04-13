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
import ns3_module_network
import ns3_module_bridge
import ns3_module_config_store
import ns3_module_emu
import ns3_module_mobility
import ns3_module_mpi
import ns3_module_stats
import ns3_module_tools
import ns3_module_topology_read
import ns3_module_propagation
import ns3_module_internet
import ns3_module_point_to_point
import ns3_module_wifi
import ns3_module_applications
import ns3_module_click
import ns3_module_flow_monitor
import ns3_module_nix_vector_routing
import ns3_module_openflow
import ns3_module_tap_bridge
import ns3_module_virtual_net_device
import ns3_module_netanim
import ns3_module_energy
import ns3_module_visualizer
import ns3_module_mesh
import ns3_module_ns3wifi
import ns3_module_spectrum
import ns3_module_csma
import ns3_module_uan
import ns3_module_aodv
import ns3_module_dsdv
import ns3_module_wimax
import ns3_module_ns3tcp
import ns3_module_olsr
import ns3_module_lte

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
    root_module.begin_section('ns3_module_network')
    ns3_module_network.register_types(module)
    
    try:
        import ns3_module_network__local
    except ImportError:
        pass
    else:
        ns3_module_network__local.register_types(module)
    
    root_module.end_section('ns3_module_network')
    root_module.begin_section('ns3_module_bridge')
    ns3_module_bridge.register_types(module)
    
    try:
        import ns3_module_bridge__local
    except ImportError:
        pass
    else:
        ns3_module_bridge__local.register_types(module)
    
    root_module.end_section('ns3_module_bridge')
    root_module.begin_section('ns3_module_config_store')
    ns3_module_config_store.register_types(module)
    
    try:
        import ns3_module_config_store__local
    except ImportError:
        pass
    else:
        ns3_module_config_store__local.register_types(module)
    
    root_module.end_section('ns3_module_config_store')
    root_module.begin_section('ns3_module_emu')
    ns3_module_emu.register_types(module)
    
    try:
        import ns3_module_emu__local
    except ImportError:
        pass
    else:
        ns3_module_emu__local.register_types(module)
    
    root_module.end_section('ns3_module_emu')
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
    root_module.begin_section('ns3_module_stats')
    ns3_module_stats.register_types(module)
    
    try:
        import ns3_module_stats__local
    except ImportError:
        pass
    else:
        ns3_module_stats__local.register_types(module)
    
    root_module.end_section('ns3_module_stats')
    root_module.begin_section('ns3_module_tools')
    ns3_module_tools.register_types(module)
    
    try:
        import ns3_module_tools__local
    except ImportError:
        pass
    else:
        ns3_module_tools__local.register_types(module)
    
    root_module.end_section('ns3_module_tools')
    root_module.begin_section('ns3_module_topology_read')
    ns3_module_topology_read.register_types(module)
    
    try:
        import ns3_module_topology_read__local
    except ImportError:
        pass
    else:
        ns3_module_topology_read__local.register_types(module)
    
    root_module.end_section('ns3_module_topology_read')
    root_module.begin_section('ns3_module_propagation')
    ns3_module_propagation.register_types(module)
    
    try:
        import ns3_module_propagation__local
    except ImportError:
        pass
    else:
        ns3_module_propagation__local.register_types(module)
    
    root_module.end_section('ns3_module_propagation')
    root_module.begin_section('ns3_module_internet')
    ns3_module_internet.register_types(module)
    
    try:
        import ns3_module_internet__local
    except ImportError:
        pass
    else:
        ns3_module_internet__local.register_types(module)
    
    root_module.end_section('ns3_module_internet')
    root_module.begin_section('ns3_module_point_to_point')
    ns3_module_point_to_point.register_types(module)
    
    try:
        import ns3_module_point_to_point__local
    except ImportError:
        pass
    else:
        ns3_module_point_to_point__local.register_types(module)
    
    root_module.end_section('ns3_module_point_to_point')
    root_module.begin_section('ns3_module_wifi')
    ns3_module_wifi.register_types(module)
    
    try:
        import ns3_module_wifi__local
    except ImportError:
        pass
    else:
        ns3_module_wifi__local.register_types(module)
    
    root_module.end_section('ns3_module_wifi')
    root_module.begin_section('ns3_module_applications')
    ns3_module_applications.register_types(module)
    
    try:
        import ns3_module_applications__local
    except ImportError:
        pass
    else:
        ns3_module_applications__local.register_types(module)
    
    root_module.end_section('ns3_module_applications')
    root_module.begin_section('ns3_module_click')
    ns3_module_click.register_types(module)
    
    try:
        import ns3_module_click__local
    except ImportError:
        pass
    else:
        ns3_module_click__local.register_types(module)
    
    root_module.end_section('ns3_module_click')
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
    root_module.begin_section('ns3_module_openflow')
    ns3_module_openflow.register_types(module)
    
    try:
        import ns3_module_openflow__local
    except ImportError:
        pass
    else:
        ns3_module_openflow__local.register_types(module)
    
    root_module.end_section('ns3_module_openflow')
    root_module.begin_section('ns3_module_tap_bridge')
    ns3_module_tap_bridge.register_types(module)
    
    try:
        import ns3_module_tap_bridge__local
    except ImportError:
        pass
    else:
        ns3_module_tap_bridge__local.register_types(module)
    
    root_module.end_section('ns3_module_tap_bridge')
    root_module.begin_section('ns3_module_virtual_net_device')
    ns3_module_virtual_net_device.register_types(module)
    
    try:
        import ns3_module_virtual_net_device__local
    except ImportError:
        pass
    else:
        ns3_module_virtual_net_device__local.register_types(module)
    
    root_module.end_section('ns3_module_virtual_net_device')
    root_module.begin_section('ns3_module_netanim')
    ns3_module_netanim.register_types(module)
    
    try:
        import ns3_module_netanim__local
    except ImportError:
        pass
    else:
        ns3_module_netanim__local.register_types(module)
    
    root_module.end_section('ns3_module_netanim')
    root_module.begin_section('ns3_module_energy')
    ns3_module_energy.register_types(module)
    
    try:
        import ns3_module_energy__local
    except ImportError:
        pass
    else:
        ns3_module_energy__local.register_types(module)
    
    root_module.end_section('ns3_module_energy')
    root_module.begin_section('ns3_module_visualizer')
    ns3_module_visualizer.register_types(module)
    
    try:
        import ns3_module_visualizer__local
    except ImportError:
        pass
    else:
        ns3_module_visualizer__local.register_types(module)
    
    root_module.end_section('ns3_module_visualizer')
    root_module.begin_section('ns3_module_mesh')
    ns3_module_mesh.register_types(module)
    
    try:
        import ns3_module_mesh__local
    except ImportError:
        pass
    else:
        ns3_module_mesh__local.register_types(module)
    
    root_module.end_section('ns3_module_mesh')
    root_module.begin_section('ns3_module_ns3wifi')
    ns3_module_ns3wifi.register_types(module)
    
    try:
        import ns3_module_ns3wifi__local
    except ImportError:
        pass
    else:
        ns3_module_ns3wifi__local.register_types(module)
    
    root_module.end_section('ns3_module_ns3wifi')
    root_module.begin_section('ns3_module_spectrum')
    ns3_module_spectrum.register_types(module)
    
    try:
        import ns3_module_spectrum__local
    except ImportError:
        pass
    else:
        ns3_module_spectrum__local.register_types(module)
    
    root_module.end_section('ns3_module_spectrum')
    root_module.begin_section('ns3_module_csma')
    ns3_module_csma.register_types(module)
    
    try:
        import ns3_module_csma__local
    except ImportError:
        pass
    else:
        ns3_module_csma__local.register_types(module)
    
    root_module.end_section('ns3_module_csma')
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
    root_module.begin_section('ns3_module_dsdv')
    ns3_module_dsdv.register_types(module)
    
    try:
        import ns3_module_dsdv__local
    except ImportError:
        pass
    else:
        ns3_module_dsdv__local.register_types(module)
    
    root_module.end_section('ns3_module_dsdv')
    root_module.begin_section('ns3_module_wimax')
    ns3_module_wimax.register_types(module)
    
    try:
        import ns3_module_wimax__local
    except ImportError:
        pass
    else:
        ns3_module_wimax__local.register_types(module)
    
    root_module.end_section('ns3_module_wimax')
    root_module.begin_section('ns3_module_ns3tcp')
    ns3_module_ns3tcp.register_types(module)
    
    try:
        import ns3_module_ns3tcp__local
    except ImportError:
        pass
    else:
        ns3_module_ns3tcp__local.register_types(module)
    
    root_module.end_section('ns3_module_ns3tcp')
    root_module.begin_section('ns3_module_olsr')
    ns3_module_olsr.register_types(module)
    
    try:
        import ns3_module_olsr__local
    except ImportError:
        pass
    else:
        ns3_module_olsr__local.register_types(module)
    
    root_module.end_section('ns3_module_olsr')
    root_module.begin_section('ns3_module_lte')
    ns3_module_lte.register_types(module)
    
    try:
        import ns3_module_lte__local
    except ImportError:
        pass
    else:
        ns3_module_lte__local.register_types(module)
    
    root_module.end_section('ns3_module_lte')
    module.add_container('std::vector< unsigned int >', 'unsigned int', container_type='vector')
    module.add_container('std::vector< bool >', 'bool', container_type='vector')
    module.add_container('std::vector< int >', 'int', container_type='vector')
    module.add_container('std::vector< std::string >', 'std::string', container_type='vector')
    module.add_container('std::set< unsigned int >', 'unsigned int', container_type='set')
    module.add_container('std::list< std::pair< ns3::Ptr< ns3::Packet >, ns3::UanAddress > >', 'std::pair< ns3::Ptr< ns3::Packet >, ns3::UanAddress >', container_type='list')
    module.add_container('std::vector< ns3::ServiceFlow * >', 'ns3::ServiceFlow *', container_type='vector')
    module.add_container('std::vector< std::complex< double > >', 'std::complex< double >', container_type='vector')
    module.add_container('std::vector< double >', 'double', container_type='vector')
    module.add_container('ns3::bvec', 'bool', container_type='vector')
    module.add_container('std::vector< ns3::SSRecord * >', 'ns3::SSRecord *', container_type='vector')
    module.add_container('std::map< std::string, std::string >', ('std::string', 'std::string'), container_type='map')
    module.add_container('std::set< unsigned char >', 'unsigned char', container_type='set')
    module.add_container('std::list< std::pair< ns3::OfdmDlMapIe *, ns3::Ptr< ns3::PacketBurst > > >', 'std::pair< ns3::OfdmDlMapIe *, ns3::Ptr< ns3::PacketBurst > >', container_type='list')
    module.add_container('std::vector< unsigned long long >', 'long long unsigned int', container_type='vector')
    module.add_container('std::vector< std::vector< double > >', 'std::vector< double >', container_type='vector')
    module.add_container('std::list< unsigned int >', 'unsigned int', container_type='list')
    module.add_container('std::list< std::pair< ns3::Ptr< ns3::Packet >, ns3::AmsduSubframeHeader > >', 'std::pair< ns3::Ptr< ns3::Packet >, ns3::AmsduSubframeHeader >', container_type='list')
    module.add_container('std::vector< std::pair< ns3::Ptr< ns3::UanNetDevice >, ns3::Ptr< ns3::UanTransducer > > >', 'std::pair< ns3::Ptr< ns3::UanNetDevice >, ns3::Ptr< ns3::UanTransducer > >', container_type='vector')
    
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
    
    module.add_container('std::vector< std::pair< unsigned int, ns3::Mac48Address > >', 'std::pair< unsigned int, ns3::Mac48Address >', container_type='vector')

def register_types_ns3_dsdv(module):
    root_module = module.get_root()
    

def register_types_ns3_flame(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    root_module.begin_section('ns3_module_core')
    ns3_module_core.register_methods(root_module)
    
    try:
        import ns3_module_core__local
    except ImportError:
        pass
    else:
        ns3_module_core__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_core')
    root_module.begin_section('ns3_module_network')
    ns3_module_network.register_methods(root_module)
    
    try:
        import ns3_module_network__local
    except ImportError:
        pass
    else:
        ns3_module_network__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_network')
    root_module.begin_section('ns3_module_bridge')
    ns3_module_bridge.register_methods(root_module)
    
    try:
        import ns3_module_bridge__local
    except ImportError:
        pass
    else:
        ns3_module_bridge__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_bridge')
    root_module.begin_section('ns3_module_config_store')
    ns3_module_config_store.register_methods(root_module)
    
    try:
        import ns3_module_config_store__local
    except ImportError:
        pass
    else:
        ns3_module_config_store__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_config_store')
    root_module.begin_section('ns3_module_emu')
    ns3_module_emu.register_methods(root_module)
    
    try:
        import ns3_module_emu__local
    except ImportError:
        pass
    else:
        ns3_module_emu__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_emu')
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
    root_module.begin_section('ns3_module_stats')
    ns3_module_stats.register_methods(root_module)
    
    try:
        import ns3_module_stats__local
    except ImportError:
        pass
    else:
        ns3_module_stats__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_stats')
    root_module.begin_section('ns3_module_tools')
    ns3_module_tools.register_methods(root_module)
    
    try:
        import ns3_module_tools__local
    except ImportError:
        pass
    else:
        ns3_module_tools__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_tools')
    root_module.begin_section('ns3_module_topology_read')
    ns3_module_topology_read.register_methods(root_module)
    
    try:
        import ns3_module_topology_read__local
    except ImportError:
        pass
    else:
        ns3_module_topology_read__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_topology_read')
    root_module.begin_section('ns3_module_propagation')
    ns3_module_propagation.register_methods(root_module)
    
    try:
        import ns3_module_propagation__local
    except ImportError:
        pass
    else:
        ns3_module_propagation__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_propagation')
    root_module.begin_section('ns3_module_internet')
    ns3_module_internet.register_methods(root_module)
    
    try:
        import ns3_module_internet__local
    except ImportError:
        pass
    else:
        ns3_module_internet__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_internet')
    root_module.begin_section('ns3_module_point_to_point')
    ns3_module_point_to_point.register_methods(root_module)
    
    try:
        import ns3_module_point_to_point__local
    except ImportError:
        pass
    else:
        ns3_module_point_to_point__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_point_to_point')
    root_module.begin_section('ns3_module_wifi')
    ns3_module_wifi.register_methods(root_module)
    
    try:
        import ns3_module_wifi__local
    except ImportError:
        pass
    else:
        ns3_module_wifi__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_wifi')
    root_module.begin_section('ns3_module_applications')
    ns3_module_applications.register_methods(root_module)
    
    try:
        import ns3_module_applications__local
    except ImportError:
        pass
    else:
        ns3_module_applications__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_applications')
    root_module.begin_section('ns3_module_click')
    ns3_module_click.register_methods(root_module)
    
    try:
        import ns3_module_click__local
    except ImportError:
        pass
    else:
        ns3_module_click__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_click')
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
    root_module.begin_section('ns3_module_openflow')
    ns3_module_openflow.register_methods(root_module)
    
    try:
        import ns3_module_openflow__local
    except ImportError:
        pass
    else:
        ns3_module_openflow__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_openflow')
    root_module.begin_section('ns3_module_tap_bridge')
    ns3_module_tap_bridge.register_methods(root_module)
    
    try:
        import ns3_module_tap_bridge__local
    except ImportError:
        pass
    else:
        ns3_module_tap_bridge__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_tap_bridge')
    root_module.begin_section('ns3_module_virtual_net_device')
    ns3_module_virtual_net_device.register_methods(root_module)
    
    try:
        import ns3_module_virtual_net_device__local
    except ImportError:
        pass
    else:
        ns3_module_virtual_net_device__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_virtual_net_device')
    root_module.begin_section('ns3_module_netanim')
    ns3_module_netanim.register_methods(root_module)
    
    try:
        import ns3_module_netanim__local
    except ImportError:
        pass
    else:
        ns3_module_netanim__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_netanim')
    root_module.begin_section('ns3_module_energy')
    ns3_module_energy.register_methods(root_module)
    
    try:
        import ns3_module_energy__local
    except ImportError:
        pass
    else:
        ns3_module_energy__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_energy')
    root_module.begin_section('ns3_module_visualizer')
    ns3_module_visualizer.register_methods(root_module)
    
    try:
        import ns3_module_visualizer__local
    except ImportError:
        pass
    else:
        ns3_module_visualizer__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_visualizer')
    root_module.begin_section('ns3_module_mesh')
    ns3_module_mesh.register_methods(root_module)
    
    try:
        import ns3_module_mesh__local
    except ImportError:
        pass
    else:
        ns3_module_mesh__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_mesh')
    root_module.begin_section('ns3_module_ns3wifi')
    ns3_module_ns3wifi.register_methods(root_module)
    
    try:
        import ns3_module_ns3wifi__local
    except ImportError:
        pass
    else:
        ns3_module_ns3wifi__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_ns3wifi')
    root_module.begin_section('ns3_module_spectrum')
    ns3_module_spectrum.register_methods(root_module)
    
    try:
        import ns3_module_spectrum__local
    except ImportError:
        pass
    else:
        ns3_module_spectrum__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_spectrum')
    root_module.begin_section('ns3_module_csma')
    ns3_module_csma.register_methods(root_module)
    
    try:
        import ns3_module_csma__local
    except ImportError:
        pass
    else:
        ns3_module_csma__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_csma')
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
    root_module.begin_section('ns3_module_dsdv')
    ns3_module_dsdv.register_methods(root_module)
    
    try:
        import ns3_module_dsdv__local
    except ImportError:
        pass
    else:
        ns3_module_dsdv__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_dsdv')
    root_module.begin_section('ns3_module_wimax')
    ns3_module_wimax.register_methods(root_module)
    
    try:
        import ns3_module_wimax__local
    except ImportError:
        pass
    else:
        ns3_module_wimax__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_wimax')
    root_module.begin_section('ns3_module_ns3tcp')
    ns3_module_ns3tcp.register_methods(root_module)
    
    try:
        import ns3_module_ns3tcp__local
    except ImportError:
        pass
    else:
        ns3_module_ns3tcp__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_ns3tcp')
    root_module.begin_section('ns3_module_olsr')
    ns3_module_olsr.register_methods(root_module)
    
    try:
        import ns3_module_olsr__local
    except ImportError:
        pass
    else:
        ns3_module_olsr__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_olsr')
    root_module.begin_section('ns3_module_lte')
    ns3_module_lte.register_methods(root_module)
    
    try:
        import ns3_module_lte__local
    except ImportError:
        pass
    else:
        ns3_module_lte__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_lte')
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
    root_module.begin_section('ns3_module_network')
    ns3_module_network.register_functions(root_module)
    
    try:
        import ns3_module_network__local
    except ImportError:
        pass
    else:
        ns3_module_network__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_network')
    root_module.begin_section('ns3_module_bridge')
    ns3_module_bridge.register_functions(root_module)
    
    try:
        import ns3_module_bridge__local
    except ImportError:
        pass
    else:
        ns3_module_bridge__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_bridge')
    root_module.begin_section('ns3_module_config_store')
    ns3_module_config_store.register_functions(root_module)
    
    try:
        import ns3_module_config_store__local
    except ImportError:
        pass
    else:
        ns3_module_config_store__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_config_store')
    root_module.begin_section('ns3_module_emu')
    ns3_module_emu.register_functions(root_module)
    
    try:
        import ns3_module_emu__local
    except ImportError:
        pass
    else:
        ns3_module_emu__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_emu')
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
    root_module.begin_section('ns3_module_stats')
    ns3_module_stats.register_functions(root_module)
    
    try:
        import ns3_module_stats__local
    except ImportError:
        pass
    else:
        ns3_module_stats__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_stats')
    root_module.begin_section('ns3_module_tools')
    ns3_module_tools.register_functions(root_module)
    
    try:
        import ns3_module_tools__local
    except ImportError:
        pass
    else:
        ns3_module_tools__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_tools')
    root_module.begin_section('ns3_module_topology_read')
    ns3_module_topology_read.register_functions(root_module)
    
    try:
        import ns3_module_topology_read__local
    except ImportError:
        pass
    else:
        ns3_module_topology_read__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_topology_read')
    root_module.begin_section('ns3_module_propagation')
    ns3_module_propagation.register_functions(root_module)
    
    try:
        import ns3_module_propagation__local
    except ImportError:
        pass
    else:
        ns3_module_propagation__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_propagation')
    root_module.begin_section('ns3_module_internet')
    ns3_module_internet.register_functions(root_module)
    
    try:
        import ns3_module_internet__local
    except ImportError:
        pass
    else:
        ns3_module_internet__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_internet')
    root_module.begin_section('ns3_module_point_to_point')
    ns3_module_point_to_point.register_functions(root_module)
    
    try:
        import ns3_module_point_to_point__local
    except ImportError:
        pass
    else:
        ns3_module_point_to_point__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_point_to_point')
    root_module.begin_section('ns3_module_wifi')
    ns3_module_wifi.register_functions(root_module)
    
    try:
        import ns3_module_wifi__local
    except ImportError:
        pass
    else:
        ns3_module_wifi__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_wifi')
    root_module.begin_section('ns3_module_applications')
    ns3_module_applications.register_functions(root_module)
    
    try:
        import ns3_module_applications__local
    except ImportError:
        pass
    else:
        ns3_module_applications__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_applications')
    root_module.begin_section('ns3_module_click')
    ns3_module_click.register_functions(root_module)
    
    try:
        import ns3_module_click__local
    except ImportError:
        pass
    else:
        ns3_module_click__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_click')
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
    root_module.begin_section('ns3_module_openflow')
    ns3_module_openflow.register_functions(root_module)
    
    try:
        import ns3_module_openflow__local
    except ImportError:
        pass
    else:
        ns3_module_openflow__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_openflow')
    root_module.begin_section('ns3_module_tap_bridge')
    ns3_module_tap_bridge.register_functions(root_module)
    
    try:
        import ns3_module_tap_bridge__local
    except ImportError:
        pass
    else:
        ns3_module_tap_bridge__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_tap_bridge')
    root_module.begin_section('ns3_module_virtual_net_device')
    ns3_module_virtual_net_device.register_functions(root_module)
    
    try:
        import ns3_module_virtual_net_device__local
    except ImportError:
        pass
    else:
        ns3_module_virtual_net_device__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_virtual_net_device')
    root_module.begin_section('ns3_module_netanim')
    ns3_module_netanim.register_functions(root_module)
    
    try:
        import ns3_module_netanim__local
    except ImportError:
        pass
    else:
        ns3_module_netanim__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_netanim')
    root_module.begin_section('ns3_module_energy')
    ns3_module_energy.register_functions(root_module)
    
    try:
        import ns3_module_energy__local
    except ImportError:
        pass
    else:
        ns3_module_energy__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_energy')
    root_module.begin_section('ns3_module_visualizer')
    ns3_module_visualizer.register_functions(root_module)
    
    try:
        import ns3_module_visualizer__local
    except ImportError:
        pass
    else:
        ns3_module_visualizer__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_visualizer')
    root_module.begin_section('ns3_module_mesh')
    ns3_module_mesh.register_functions(root_module)
    
    try:
        import ns3_module_mesh__local
    except ImportError:
        pass
    else:
        ns3_module_mesh__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_mesh')
    root_module.begin_section('ns3_module_ns3wifi')
    ns3_module_ns3wifi.register_functions(root_module)
    
    try:
        import ns3_module_ns3wifi__local
    except ImportError:
        pass
    else:
        ns3_module_ns3wifi__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_ns3wifi')
    root_module.begin_section('ns3_module_spectrum')
    ns3_module_spectrum.register_functions(root_module)
    
    try:
        import ns3_module_spectrum__local
    except ImportError:
        pass
    else:
        ns3_module_spectrum__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_spectrum')
    root_module.begin_section('ns3_module_csma')
    ns3_module_csma.register_functions(root_module)
    
    try:
        import ns3_module_csma__local
    except ImportError:
        pass
    else:
        ns3_module_csma__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_csma')
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
    root_module.begin_section('ns3_module_dsdv')
    ns3_module_dsdv.register_functions(root_module)
    
    try:
        import ns3_module_dsdv__local
    except ImportError:
        pass
    else:
        ns3_module_dsdv__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_dsdv')
    root_module.begin_section('ns3_module_wimax')
    ns3_module_wimax.register_functions(root_module)
    
    try:
        import ns3_module_wimax__local
    except ImportError:
        pass
    else:
        ns3_module_wimax__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_wimax')
    root_module.begin_section('ns3_module_ns3tcp')
    ns3_module_ns3tcp.register_functions(root_module)
    
    try:
        import ns3_module_ns3tcp__local
    except ImportError:
        pass
    else:
        ns3_module_ns3tcp__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_ns3tcp')
    root_module.begin_section('ns3_module_olsr')
    ns3_module_olsr.register_functions(root_module)
    
    try:
        import ns3_module_olsr__local
    except ImportError:
        pass
    else:
        ns3_module_olsr__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_olsr')
    root_module.begin_section('ns3_module_lte')
    ns3_module_lte.register_functions(root_module)
    
    try:
        import ns3_module_lte__local
    except ImportError:
        pass
    else:
        ns3_module_lte__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_lte')
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

