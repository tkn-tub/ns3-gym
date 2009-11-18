from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    
    ## Register a nested module for the namespace Config
    
    nested_module = module.add_cpp_namespace('Config')
    register_types_ns3_Config(nested_module)
    
    
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
    
    
    ## Register a nested module for the namespace dpd
    
    nested_module = module.add_cpp_namespace('dpd')
    register_types_ns3_dpd(nested_module)
    
    
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
    

def register_types_ns3_TimeStepPrecision(module):
    root_module = module.get_root()
    

def register_types_ns3_addressUtils(module):
    root_module = module.get_root()
    

def register_types_ns3_aodv(module):
    root_module = module.get_root()
    

def register_types_ns3_dot11s(module):
    root_module = module.get_root()
    

def register_types_ns3_dpd(module):
    root_module = module.get_root()
    
    ## dpd.h: ns3::dpd::DuplicatePacketDetection [class]
    module.add_class('DuplicatePacketDetection')
    ## id-cache.h: ns3::dpd::IdCache [class]
    module.add_class('IdCache')

def register_types_ns3_flame(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3DpdDuplicatePacketDetection_methods(root_module, root_module['ns3::dpd::DuplicatePacketDetection'])
    register_Ns3DpdIdCache_methods(root_module, root_module['ns3::dpd::IdCache'])
    return

def register_Ns3DpdDuplicatePacketDetection_methods(root_module, cls):
    ## dpd.h: ns3::dpd::DuplicatePacketDetection::DuplicatePacketDetection(ns3::dpd::DuplicatePacketDetection const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dpd::DuplicatePacketDetection const &', 'arg0')])
    ## dpd.h: ns3::dpd::DuplicatePacketDetection::DuplicatePacketDetection(ns3::Time lifetime) [constructor]
    cls.add_constructor([param('ns3::Time', 'lifetime')])
    ## dpd.h: ns3::Time ns3::dpd::DuplicatePacketDetection::GetLifetime() const [member function]
    cls.add_method('GetLifetime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## dpd.h: bool ns3::dpd::DuplicatePacketDetection::IsDuplicate(ns3::Ptr<ns3::Packet const> p, ns3::Ipv4Header const & header) [member function]
    cls.add_method('IsDuplicate', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ipv4Header const &', 'header')])
    ## dpd.h: void ns3::dpd::DuplicatePacketDetection::SetLifetime(ns3::Time lifetime) [member function]
    cls.add_method('SetLifetime', 
                   'void', 
                   [param('ns3::Time', 'lifetime')])
    return

def register_Ns3DpdIdCache_methods(root_module, cls):
    ## id-cache.h: ns3::dpd::IdCache::IdCache(ns3::dpd::IdCache const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dpd::IdCache const &', 'arg0')])
    ## id-cache.h: ns3::dpd::IdCache::IdCache(ns3::Time lifetime) [constructor]
    cls.add_constructor([param('ns3::Time', 'lifetime')])
    ## id-cache.h: ns3::Time ns3::dpd::IdCache::GetLifeTime() const [member function]
    cls.add_method('GetLifeTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## id-cache.h: uint32_t ns3::dpd::IdCache::GetSize() [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [])
    ## id-cache.h: bool ns3::dpd::IdCache::IsDuplicate(ns3::Ipv4Address addr, uint32_t id) [member function]
    cls.add_method('IsDuplicate', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'addr'), param('uint32_t', 'id')])
    ## id-cache.h: void ns3::dpd::IdCache::Purge() [member function]
    cls.add_method('Purge', 
                   'void', 
                   [])
    ## id-cache.h: void ns3::dpd::IdCache::SetLifetime(ns3::Time lifetime) [member function]
    cls.add_method('SetLifetime', 
                   'void', 
                   [param('ns3::Time', 'lifetime')])
    return

def register_functions(root_module):
    module = root_module
    register_functions_ns3_Config(module.get_submodule('Config'), root_module)
    register_functions_ns3_TimeStepPrecision(module.get_submodule('TimeStepPrecision'), root_module)
    register_functions_ns3_addressUtils(module.get_submodule('addressUtils'), root_module)
    register_functions_ns3_aodv(module.get_submodule('aodv'), root_module)
    register_functions_ns3_dot11s(module.get_submodule('dot11s'), root_module)
    register_functions_ns3_dpd(module.get_submodule('dpd'), root_module)
    register_functions_ns3_flame(module.get_submodule('flame'), root_module)
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    register_functions_ns3_olsr(module.get_submodule('olsr'), root_module)
    return

def register_functions_ns3_Config(module, root_module):
    return

def register_functions_ns3_TimeStepPrecision(module, root_module):
    return

def register_functions_ns3_addressUtils(module, root_module):
    return

def register_functions_ns3_aodv(module, root_module):
    return

def register_functions_ns3_dot11s(module, root_module):
    return

def register_functions_ns3_dpd(module, root_module):
    return

def register_functions_ns3_flame(module, root_module):
    return

def register_functions_ns3_internal(module, root_module):
    return

def register_functions_ns3_olsr(module, root_module):
    return

