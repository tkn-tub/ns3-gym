from pybindgen import Module, FileCodeSink, param, retval, cppclass


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
import ns3_module_mobility
import ns3_module_common
import ns3_module_contrib
import ns3_module_node
import ns3_module_point_to_point
import ns3_module_stats
import ns3_module_internet_stack
import ns3_module_wifi
import ns3_module_csma
import ns3_module_emu
import ns3_module_bridge
import ns3_module_packet_sink
import ns3_module_v4ping
import ns3_module_global_routing
import ns3_module_onoff
import ns3_module_olsr
import ns3_module_udp_echo
import ns3_module_helper

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
    root_module.begin_section('ns3_module_mobility')
    ns3_module_mobility.register_types(module)
    
    try:
        import ns3_module_mobility__local
    except ImportError:
        pass
    else:
        ns3_module_mobility__local.register_types(module)
    
    root_module.end_section('ns3_module_mobility')
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
    root_module.begin_section('ns3_module_point_to_point')
    ns3_module_point_to_point.register_types(module)
    
    try:
        import ns3_module_point_to_point__local
    except ImportError:
        pass
    else:
        ns3_module_point_to_point__local.register_types(module)
    
    root_module.end_section('ns3_module_point_to_point')
    root_module.begin_section('ns3_module_stats')
    ns3_module_stats.register_types(module)
    
    try:
        import ns3_module_stats__local
    except ImportError:
        pass
    else:
        ns3_module_stats__local.register_types(module)
    
    root_module.end_section('ns3_module_stats')
    root_module.begin_section('ns3_module_internet_stack')
    ns3_module_internet_stack.register_types(module)
    
    try:
        import ns3_module_internet_stack__local
    except ImportError:
        pass
    else:
        ns3_module_internet_stack__local.register_types(module)
    
    root_module.end_section('ns3_module_internet_stack')
    root_module.begin_section('ns3_module_wifi')
    ns3_module_wifi.register_types(module)
    
    try:
        import ns3_module_wifi__local
    except ImportError:
        pass
    else:
        ns3_module_wifi__local.register_types(module)
    
    root_module.end_section('ns3_module_wifi')
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
    root_module.begin_section('ns3_module_bridge')
    ns3_module_bridge.register_types(module)
    
    try:
        import ns3_module_bridge__local
    except ImportError:
        pass
    else:
        ns3_module_bridge__local.register_types(module)
    
    root_module.end_section('ns3_module_bridge')
    root_module.begin_section('ns3_module_packet_sink')
    ns3_module_packet_sink.register_types(module)
    
    try:
        import ns3_module_packet_sink__local
    except ImportError:
        pass
    else:
        ns3_module_packet_sink__local.register_types(module)
    
    root_module.end_section('ns3_module_packet_sink')
    root_module.begin_section('ns3_module_v4ping')
    ns3_module_v4ping.register_types(module)
    
    try:
        import ns3_module_v4ping__local
    except ImportError:
        pass
    else:
        ns3_module_v4ping__local.register_types(module)
    
    root_module.end_section('ns3_module_v4ping')
    root_module.begin_section('ns3_module_global_routing')
    ns3_module_global_routing.register_types(module)
    
    try:
        import ns3_module_global_routing__local
    except ImportError:
        pass
    else:
        ns3_module_global_routing__local.register_types(module)
    
    root_module.end_section('ns3_module_global_routing')
    root_module.begin_section('ns3_module_onoff')
    ns3_module_onoff.register_types(module)
    
    try:
        import ns3_module_onoff__local
    except ImportError:
        pass
    else:
        ns3_module_onoff__local.register_types(module)
    
    root_module.end_section('ns3_module_onoff')
    root_module.begin_section('ns3_module_olsr')
    ns3_module_olsr.register_types(module)
    
    try:
        import ns3_module_olsr__local
    except ImportError:
        pass
    else:
        ns3_module_olsr__local.register_types(module)
    
    root_module.end_section('ns3_module_olsr')
    root_module.begin_section('ns3_module_udp_echo')
    ns3_module_udp_echo.register_types(module)
    
    try:
        import ns3_module_udp_echo__local
    except ImportError:
        pass
    else:
        ns3_module_udp_echo__local.register_types(module)
    
    root_module.end_section('ns3_module_udp_echo')
    root_module.begin_section('ns3_module_helper')
    ns3_module_helper.register_types(module)
    
    try:
        import ns3_module_helper__local
    except ImportError:
        pass
    else:
        ns3_module_helper__local.register_types(module)
    
    root_module.end_section('ns3_module_helper')
    module.add_container('std::vector< unsigned int >', 'unsigned int', container_type='vector')
    module.add_container('std::list< unsigned int >', 'unsigned int', container_type='list')
    
    ## Register a nested module for the namespace Config
    
    nested_module = module.add_cpp_namespace('Config')
    register_types_ns3_Config(nested_module)
    
    
    ## Register a nested module for the namespace TimeStepPrecision
    
    nested_module = module.add_cpp_namespace('TimeStepPrecision')
    register_types_ns3_TimeStepPrecision(nested_module)
    
    
    ## Register a nested module for the namespace internal
    
    nested_module = module.add_cpp_namespace('internal')
    register_types_ns3_internal(nested_module)
    
    
    ## Register a nested module for the namespace olsr
    
    nested_module = module.add_cpp_namespace('olsr')
    register_types_ns3_olsr(nested_module)
    

def register_types_ns3_Config(module):
    root_module = module.get_root()
    
    module.add_container('std::vector< std::string >', 'std::string', container_type='vector')

def register_types_ns3_TimeStepPrecision(module):
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
    root_module.begin_section('ns3_module_simulator')
    ns3_module_simulator.register_methods(root_module)
    
    try:
        import ns3_module_simulator__local
    except ImportError:
        pass
    else:
        ns3_module_simulator__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_simulator')
    root_module.begin_section('ns3_module_mobility')
    ns3_module_mobility.register_methods(root_module)
    
    try:
        import ns3_module_mobility__local
    except ImportError:
        pass
    else:
        ns3_module_mobility__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_mobility')
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
    root_module.begin_section('ns3_module_point_to_point')
    ns3_module_point_to_point.register_methods(root_module)
    
    try:
        import ns3_module_point_to_point__local
    except ImportError:
        pass
    else:
        ns3_module_point_to_point__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_point_to_point')
    root_module.begin_section('ns3_module_stats')
    ns3_module_stats.register_methods(root_module)
    
    try:
        import ns3_module_stats__local
    except ImportError:
        pass
    else:
        ns3_module_stats__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_stats')
    root_module.begin_section('ns3_module_internet_stack')
    ns3_module_internet_stack.register_methods(root_module)
    
    try:
        import ns3_module_internet_stack__local
    except ImportError:
        pass
    else:
        ns3_module_internet_stack__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_internet_stack')
    root_module.begin_section('ns3_module_wifi')
    ns3_module_wifi.register_methods(root_module)
    
    try:
        import ns3_module_wifi__local
    except ImportError:
        pass
    else:
        ns3_module_wifi__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_wifi')
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
    root_module.begin_section('ns3_module_bridge')
    ns3_module_bridge.register_methods(root_module)
    
    try:
        import ns3_module_bridge__local
    except ImportError:
        pass
    else:
        ns3_module_bridge__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_bridge')
    root_module.begin_section('ns3_module_packet_sink')
    ns3_module_packet_sink.register_methods(root_module)
    
    try:
        import ns3_module_packet_sink__local
    except ImportError:
        pass
    else:
        ns3_module_packet_sink__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_packet_sink')
    root_module.begin_section('ns3_module_v4ping')
    ns3_module_v4ping.register_methods(root_module)
    
    try:
        import ns3_module_v4ping__local
    except ImportError:
        pass
    else:
        ns3_module_v4ping__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_v4ping')
    root_module.begin_section('ns3_module_global_routing')
    ns3_module_global_routing.register_methods(root_module)
    
    try:
        import ns3_module_global_routing__local
    except ImportError:
        pass
    else:
        ns3_module_global_routing__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_global_routing')
    root_module.begin_section('ns3_module_onoff')
    ns3_module_onoff.register_methods(root_module)
    
    try:
        import ns3_module_onoff__local
    except ImportError:
        pass
    else:
        ns3_module_onoff__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_onoff')
    root_module.begin_section('ns3_module_olsr')
    ns3_module_olsr.register_methods(root_module)
    
    try:
        import ns3_module_olsr__local
    except ImportError:
        pass
    else:
        ns3_module_olsr__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_olsr')
    root_module.begin_section('ns3_module_udp_echo')
    ns3_module_udp_echo.register_methods(root_module)
    
    try:
        import ns3_module_udp_echo__local
    except ImportError:
        pass
    else:
        ns3_module_udp_echo__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_udp_echo')
    root_module.begin_section('ns3_module_helper')
    ns3_module_helper.register_methods(root_module)
    
    try:
        import ns3_module_helper__local
    except ImportError:
        pass
    else:
        ns3_module_helper__local.register_methods(root_module)
    
    root_module.end_section('ns3_module_helper')
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
    root_module.begin_section('ns3_module_mobility')
    ns3_module_mobility.register_functions(root_module)
    
    try:
        import ns3_module_mobility__local
    except ImportError:
        pass
    else:
        ns3_module_mobility__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_mobility')
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
    root_module.begin_section('ns3_module_point_to_point')
    ns3_module_point_to_point.register_functions(root_module)
    
    try:
        import ns3_module_point_to_point__local
    except ImportError:
        pass
    else:
        ns3_module_point_to_point__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_point_to_point')
    root_module.begin_section('ns3_module_stats')
    ns3_module_stats.register_functions(root_module)
    
    try:
        import ns3_module_stats__local
    except ImportError:
        pass
    else:
        ns3_module_stats__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_stats')
    root_module.begin_section('ns3_module_internet_stack')
    ns3_module_internet_stack.register_functions(root_module)
    
    try:
        import ns3_module_internet_stack__local
    except ImportError:
        pass
    else:
        ns3_module_internet_stack__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_internet_stack')
    root_module.begin_section('ns3_module_wifi')
    ns3_module_wifi.register_functions(root_module)
    
    try:
        import ns3_module_wifi__local
    except ImportError:
        pass
    else:
        ns3_module_wifi__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_wifi')
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
    root_module.begin_section('ns3_module_bridge')
    ns3_module_bridge.register_functions(root_module)
    
    try:
        import ns3_module_bridge__local
    except ImportError:
        pass
    else:
        ns3_module_bridge__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_bridge')
    root_module.begin_section('ns3_module_packet_sink')
    ns3_module_packet_sink.register_functions(root_module)
    
    try:
        import ns3_module_packet_sink__local
    except ImportError:
        pass
    else:
        ns3_module_packet_sink__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_packet_sink')
    root_module.begin_section('ns3_module_v4ping')
    ns3_module_v4ping.register_functions(root_module)
    
    try:
        import ns3_module_v4ping__local
    except ImportError:
        pass
    else:
        ns3_module_v4ping__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_v4ping')
    root_module.begin_section('ns3_module_global_routing')
    ns3_module_global_routing.register_functions(root_module)
    
    try:
        import ns3_module_global_routing__local
    except ImportError:
        pass
    else:
        ns3_module_global_routing__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_global_routing')
    root_module.begin_section('ns3_module_onoff')
    ns3_module_onoff.register_functions(root_module)
    
    try:
        import ns3_module_onoff__local
    except ImportError:
        pass
    else:
        ns3_module_onoff__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_onoff')
    root_module.begin_section('ns3_module_olsr')
    ns3_module_olsr.register_functions(root_module)
    
    try:
        import ns3_module_olsr__local
    except ImportError:
        pass
    else:
        ns3_module_olsr__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_olsr')
    root_module.begin_section('ns3_module_udp_echo')
    ns3_module_udp_echo.register_functions(root_module)
    
    try:
        import ns3_module_udp_echo__local
    except ImportError:
        pass
    else:
        ns3_module_udp_echo__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_udp_echo')
    root_module.begin_section('ns3_module_helper')
    ns3_module_helper.register_functions(root_module)
    
    try:
        import ns3_module_helper__local
    except ImportError:
        pass
    else:
        ns3_module_helper__local.register_functions(root_module)
    
    root_module.end_section('ns3_module_helper')
    register_functions_ns3_Config(module.get_submodule('Config'), root_module)
    register_functions_ns3_TimeStepPrecision(module.get_submodule('TimeStepPrecision'), root_module)
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    register_functions_ns3_olsr(module.get_submodule('olsr'), root_module)
    return

def register_functions_ns3_Config(module, root_module):
    return

def register_functions_ns3_TimeStepPrecision(module, root_module):
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

