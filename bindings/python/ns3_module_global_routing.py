from pybindgen import Module, FileCodeSink, write_preamble, param, retval

def register_types(module):
    root_module = module.get_root()
    
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
    ## global-router-interface.h: ns3::GlobalRouter [class]
    module.add_class('GlobalRouter', allow_subclassing=True, is_singleton=True, parent=root_module['ns3::Object'])
    
    ## Register a nested module for the namespace internal
    
    nested_module = module.add_cpp_namespace('internal')
    register_types_ns3_internal(nested_module)
    
    
    ## Register a nested module for the namespace TimeStepPrecision
    
    nested_module = module.add_cpp_namespace('TimeStepPrecision')
    register_types_ns3_TimeStepPrecision(nested_module)
    
    
    ## Register a nested module for the namespace Config
    
    nested_module = module.add_cpp_namespace('Config')
    register_types_ns3_Config(nested_module)
    
    
    ## Register a nested module for the namespace olsr
    
    nested_module = module.add_cpp_namespace('olsr')
    register_types_ns3_olsr(nested_module)
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_TimeStepPrecision(module):
    root_module = module.get_root()
    

def register_types_ns3_Config(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3GlobalRouteManager_methods(root_module, root_module['ns3::GlobalRouteManager'])
    register_Ns3GlobalRoutingLSA_methods(root_module, root_module['ns3::GlobalRoutingLSA'])
    register_Ns3GlobalRoutingLinkRecord_methods(root_module, root_module['ns3::GlobalRoutingLinkRecord'])
    register_Ns3GlobalRouter_methods(root_module, root_module['ns3::GlobalRouter'])
    return

def register_Ns3GlobalRouteManager_methods(root_module, cls):
    ## global-route-manager.h: static void ns3::GlobalRouteManager::PopulateRoutingTables() [member function]
    cls.add_method('PopulateRoutingTables', 'void', [], is_static=True)
    ## global-route-manager.h: static uint32_t ns3::GlobalRouteManager::AllocateRouterId() [member function]
    cls.add_method('AllocateRouterId', 'uint32_t', [], is_static=True)
    return

def register_Ns3GlobalRoutingLSA_methods(root_module, cls):
    ## global-router-interface.h: ns3::GlobalRoutingLSA::GlobalRoutingLSA() [constructor]
    cls.add_constructor([], visibility='public')
    ## global-router-interface.h: ns3::GlobalRoutingLSA::GlobalRoutingLSA(ns3::GlobalRoutingLSA::SPFStatus status, ns3::Ipv4Address linkStateId, ns3::Ipv4Address advertisingRtr) [constructor]
    cls.add_constructor([param('ns3::GlobalRoutingLSA::SPFStatus', 'status'), param('ns3::Ipv4Address', 'linkStateId'), param('ns3::Ipv4Address', 'advertisingRtr')], visibility='public')
    ## global-router-interface.h: ns3::GlobalRoutingLSA::GlobalRoutingLSA(ns3::GlobalRoutingLSA & lsa) [constructor]
    cls.add_constructor([param('ns3::GlobalRoutingLSA&', 'lsa')], visibility='public')
    ## global-router-interface.h: uint32_t ns3::GlobalRoutingLSA::AddAttachedRouter(ns3::Ipv4Address addr) [member function]
    cls.add_method('AddAttachedRouter', 'uint32_t', [param('ns3::Ipv4Address', 'addr')])
    ## global-router-interface.h: uint32_t ns3::GlobalRoutingLSA::AddLinkRecord(ns3::GlobalRoutingLinkRecord * lr) [member function]
    cls.add_method('AddLinkRecord', 'uint32_t', [param('ns3::GlobalRoutingLinkRecord *', 'lr')])
    ## global-router-interface.h: void ns3::GlobalRoutingLSA::ClearLinkRecords() [member function]
    cls.add_method('ClearLinkRecords', 'void', [])
    ## global-router-interface.h: void ns3::GlobalRoutingLSA::CopyLinkRecords(ns3::GlobalRoutingLSA const & lsa) [member function]
    cls.add_method('CopyLinkRecords', 'void', [param('ns3::GlobalRoutingLSA&', 'lsa', is_const=True)])
    ## global-router-interface.h: ns3::Ipv4Address ns3::GlobalRoutingLSA::GetAdvertisingRouter() const [member function]
    cls.add_method('GetAdvertisingRouter', 'ns3::Ipv4Address', [], is_const=True)
    ## global-router-interface.h: ns3::Ipv4Address ns3::GlobalRoutingLSA::GetAttachedRouter(uint32_t n) const [member function]
    cls.add_method('GetAttachedRouter', 'ns3::Ipv4Address', [param('uint32_t', 'n')], is_const=True)
    ## global-router-interface.h: ns3::GlobalRoutingLSA::LSType ns3::GlobalRoutingLSA::GetLSType() const [member function]
    cls.add_method('GetLSType', 'ns3::GlobalRoutingLSA::LSType', [], is_const=True)
    ## global-router-interface.h: ns3::GlobalRoutingLinkRecord * ns3::GlobalRoutingLSA::GetLinkRecord(uint32_t n) const [member function]
    cls.add_method('GetLinkRecord', 'ns3::GlobalRoutingLinkRecord *', [param('uint32_t', 'n')], is_const=True)
    ## global-router-interface.h: ns3::Ipv4Address ns3::GlobalRoutingLSA::GetLinkStateId() const [member function]
    cls.add_method('GetLinkStateId', 'ns3::Ipv4Address', [], is_const=True)
    ## global-router-interface.h: uint32_t ns3::GlobalRoutingLSA::GetNAttachedRouters() const [member function]
    cls.add_method('GetNAttachedRouters', 'uint32_t', [], is_const=True)
    ## global-router-interface.h: uint32_t ns3::GlobalRoutingLSA::GetNLinkRecords() const [member function]
    cls.add_method('GetNLinkRecords', 'uint32_t', [], is_const=True)
    ## global-router-interface.h: ns3::Ipv4Mask ns3::GlobalRoutingLSA::GetNetworkLSANetworkMask() const [member function]
    cls.add_method('GetNetworkLSANetworkMask', 'ns3::Ipv4Mask', [], is_const=True)
    ## global-router-interface.h: ns3::GlobalRoutingLSA::SPFStatus ns3::GlobalRoutingLSA::GetStatus() const [member function]
    cls.add_method('GetStatus', 'ns3::GlobalRoutingLSA::SPFStatus', [], is_const=True)
    ## global-router-interface.h: bool ns3::GlobalRoutingLSA::IsEmpty() const [member function]
    cls.add_method('IsEmpty', 'bool', [], is_const=True)
    ## global-router-interface.h: void ns3::GlobalRoutingLSA::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 'void', [param('std::ostream&', 'os')], is_const=True)
    ## global-router-interface.h: void ns3::GlobalRoutingLSA::SetAdvertisingRouter(ns3::Ipv4Address rtr) [member function]
    cls.add_method('SetAdvertisingRouter', 'void', [param('ns3::Ipv4Address', 'rtr')])
    ## global-router-interface.h: void ns3::GlobalRoutingLSA::SetLSType(ns3::GlobalRoutingLSA::LSType typ) [member function]
    cls.add_method('SetLSType', 'void', [param('ns3::GlobalRoutingLSA::LSType', 'typ')])
    ## global-router-interface.h: void ns3::GlobalRoutingLSA::SetLinkStateId(ns3::Ipv4Address addr) [member function]
    cls.add_method('SetLinkStateId', 'void', [param('ns3::Ipv4Address', 'addr')])
    ## global-router-interface.h: void ns3::GlobalRoutingLSA::SetNetworkLSANetworkMask(ns3::Ipv4Mask mask) [member function]
    cls.add_method('SetNetworkLSANetworkMask', 'void', [param('ns3::Ipv4Mask', 'mask')])
    ## global-router-interface.h: void ns3::GlobalRoutingLSA::SetStatus(ns3::GlobalRoutingLSA::SPFStatus status) [member function]
    cls.add_method('SetStatus', 'void', [param('ns3::GlobalRoutingLSA::SPFStatus', 'status')])
    cls.add_output_stream_operator()
    return

def register_Ns3GlobalRoutingLinkRecord_methods(root_module, cls):
    ## global-router-interface.h: ns3::GlobalRoutingLinkRecord::GlobalRoutingLinkRecord() [constructor]
    cls.add_constructor([], visibility='public')
    ## global-router-interface.h: ns3::GlobalRoutingLinkRecord::GlobalRoutingLinkRecord(ns3::GlobalRoutingLinkRecord::LinkType linkType, ns3::Ipv4Address linkId, ns3::Ipv4Address linkData, uint16_t metric) [constructor]
    cls.add_constructor([param('ns3::GlobalRoutingLinkRecord::LinkType', 'linkType'), param('ns3::Ipv4Address', 'linkId'), param('ns3::Ipv4Address', 'linkData'), param('uint16_t', 'metric')], visibility='public')
    ## global-router-interface.h: ns3::Ipv4Address ns3::GlobalRoutingLinkRecord::GetLinkId() const [member function]
    cls.add_method('GetLinkId', 'ns3::Ipv4Address', [], is_const=True)
    ## global-router-interface.h: void ns3::GlobalRoutingLinkRecord::SetLinkId(ns3::Ipv4Address addr) [member function]
    cls.add_method('SetLinkId', 'void', [param('ns3::Ipv4Address', 'addr')])
    ## global-router-interface.h: ns3::Ipv4Address ns3::GlobalRoutingLinkRecord::GetLinkData() const [member function]
    cls.add_method('GetLinkData', 'ns3::Ipv4Address', [], is_const=True)
    ## global-router-interface.h: void ns3::GlobalRoutingLinkRecord::SetLinkData(ns3::Ipv4Address addr) [member function]
    cls.add_method('SetLinkData', 'void', [param('ns3::Ipv4Address', 'addr')])
    ## global-router-interface.h: ns3::GlobalRoutingLinkRecord::LinkType ns3::GlobalRoutingLinkRecord::GetLinkType() const [member function]
    cls.add_method('GetLinkType', 'ns3::GlobalRoutingLinkRecord::LinkType', [], is_const=True)
    ## global-router-interface.h: void ns3::GlobalRoutingLinkRecord::SetLinkType(ns3::GlobalRoutingLinkRecord::LinkType linkType) [member function]
    cls.add_method('SetLinkType', 'void', [param('ns3::GlobalRoutingLinkRecord::LinkType', 'linkType')])
    ## global-router-interface.h: uint16_t ns3::GlobalRoutingLinkRecord::GetMetric() const [member function]
    cls.add_method('GetMetric', 'uint16_t', [], is_const=True)
    ## global-router-interface.h: void ns3::GlobalRoutingLinkRecord::SetMetric(uint16_t metric) [member function]
    cls.add_method('SetMetric', 'void', [param('uint16_t', 'metric')])
    return

def register_Ns3GlobalRouter_methods(root_module, cls):
    ## global-router-interface.h: static ns3::TypeId ns3::GlobalRouter::GetTypeId() [member function]
    cls.add_method('GetTypeId', 'ns3::TypeId', [], is_static=True)
    ## global-router-interface.h: ns3::GlobalRouter::GlobalRouter() [constructor]
    cls.add_constructor([], visibility='public')
    ## global-router-interface.h: ns3::Ipv4Address ns3::GlobalRouter::GetRouterId() const [member function]
    cls.add_method('GetRouterId', 'ns3::Ipv4Address', [], is_const=True)
    ## global-router-interface.h: uint32_t ns3::GlobalRouter::DiscoverLSAs() [member function]
    cls.add_method('DiscoverLSAs', 'uint32_t', [])
    ## global-router-interface.h: uint32_t ns3::GlobalRouter::GetNumLSAs() const [member function]
    cls.add_method('GetNumLSAs', 'uint32_t', [], is_const=True)
    ## global-router-interface.h: bool ns3::GlobalRouter::GetLSA(uint32_t n, ns3::GlobalRoutingLSA & lsa) const [member function]
    cls.add_method('GetLSA', 'bool', [param('uint32_t', 'n'), param('ns3::GlobalRoutingLSA&', 'lsa')], is_const=True)
    ## global-router-interface.h: void ns3::GlobalRouter::DoDispose() [member function]
    cls.add_method('DoDispose', 'void', [], visibility='private', is_virtual=True)
    return

def register_functions(root_module):
    module = root_module
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    register_functions_ns3_TimeStepPrecision(module.get_submodule('TimeStepPrecision'), root_module)
    register_functions_ns3_Config(module.get_submodule('Config'), root_module)
    register_functions_ns3_olsr(module.get_submodule('olsr'), root_module)
    return

def register_functions_ns3_internal(module, root_module):
    return

def register_functions_ns3_TimeStepPrecision(module, root_module):
    return

def register_functions_ns3_Config(module, root_module):
    return

def register_functions_ns3_olsr(module, root_module):
    return

