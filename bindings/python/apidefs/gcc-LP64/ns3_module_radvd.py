from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::RadvdInterface, ns3::empty, ns3::DefaultDeleter<ns3::RadvdInterface> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::RadvdInterface', 'ns3::empty', 'ns3::DefaultDeleter<ns3::RadvdInterface>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::RadvdPrefix, ns3::empty, ns3::DefaultDeleter<ns3::RadvdPrefix> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::RadvdPrefix', 'ns3::empty', 'ns3::DefaultDeleter<ns3::RadvdPrefix>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## radvd.h: ns3::Radvd [class]
    module.add_class('Radvd', parent=root_module['ns3::Application'])
    ## radvd-interface.h: ns3::RadvdInterface [class]
    module.add_class('RadvdInterface', parent=root_module['ns3::SimpleRefCount< ns3::RadvdInterface, ns3::empty, ns3::DefaultDeleter<ns3::RadvdInterface> >'])
    ## radvd-prefix.h: ns3::RadvdPrefix [class]
    module.add_class('RadvdPrefix', parent=root_module['ns3::SimpleRefCount< ns3::RadvdPrefix, ns3::empty, ns3::DefaultDeleter<ns3::RadvdPrefix> >'])
    
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
    

def register_types_ns3_dot11s(module):
    root_module = module.get_root()
    

def register_types_ns3_flame(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3Radvd_methods(root_module, root_module['ns3::Radvd'])
    register_Ns3RadvdInterface_methods(root_module, root_module['ns3::RadvdInterface'])
    register_Ns3RadvdPrefix_methods(root_module, root_module['ns3::RadvdPrefix'])
    return

def register_Ns3Radvd_methods(root_module, cls):
    ## radvd.h: ns3::Radvd::Radvd(ns3::Radvd const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Radvd const &', 'arg0')])
    ## radvd.h: ns3::Radvd::Radvd() [constructor]
    cls.add_constructor([])
    ## radvd.h: void ns3::Radvd::AddConfiguration(ns3::Ptr<ns3::RadvdInterface> routerInterface) [member function]
    cls.add_method('AddConfiguration', 
                   'void', 
                   [param('ns3::Ptr< ns3::RadvdInterface >', 'routerInterface')])
    ## radvd.h: static ns3::TypeId ns3::Radvd::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## radvd.h: ns3::Radvd::MAX_RA_DELAY_TIME [variable]
    cls.add_static_attribute('MAX_RA_DELAY_TIME', 'uint32_t const', is_const=True)
    ## radvd.h: void ns3::Radvd::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## radvd.h: void ns3::Radvd::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## radvd.h: void ns3::Radvd::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3RadvdInterface_methods(root_module, cls):
    ## radvd-interface.h: ns3::RadvdInterface::RadvdInterface(ns3::RadvdInterface const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RadvdInterface const &', 'arg0')])
    ## radvd-interface.h: ns3::RadvdInterface::RadvdInterface(uint32_t interface) [constructor]
    cls.add_constructor([param('uint32_t', 'interface')])
    ## radvd-interface.h: ns3::RadvdInterface::RadvdInterface(uint32_t interface, uint32_t maxRtrAdvInterval, uint32_t minRtrAdvInterval) [constructor]
    cls.add_constructor([param('uint32_t', 'interface'), param('uint32_t', 'maxRtrAdvInterval'), param('uint32_t', 'minRtrAdvInterval')])
    ## radvd-interface.h: void ns3::RadvdInterface::AddPrefix(ns3::Ptr<ns3::RadvdPrefix> routerPrefix) [member function]
    cls.add_method('AddPrefix', 
                   'void', 
                   [param('ns3::Ptr< ns3::RadvdPrefix >', 'routerPrefix')])
    ## radvd-interface.h: uint8_t ns3::RadvdInterface::GetCurHopLimit() const [member function]
    cls.add_method('GetCurHopLimit', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetDefaultLifeTime() const [member function]
    cls.add_method('GetDefaultLifeTime', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint8_t ns3::RadvdInterface::GetDefaultPreference() const [member function]
    cls.add_method('GetDefaultPreference', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetHomeAgentLifeTime() const [member function]
    cls.add_method('GetHomeAgentLifeTime', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetHomeAgentPreference() const [member function]
    cls.add_method('GetHomeAgentPreference', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetInterface() const [member function]
    cls.add_method('GetInterface', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetLinkMtu() const [member function]
    cls.add_method('GetLinkMtu', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetMaxRtrAdvInterval() const [member function]
    cls.add_method('GetMaxRtrAdvInterval', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetMinDelayBetweenRAs() const [member function]
    cls.add_method('GetMinDelayBetweenRAs', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetMinRtrAdvInterval() const [member function]
    cls.add_method('GetMinRtrAdvInterval', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: std::list<ns3::Ptr<ns3::RadvdPrefix>, std::allocator<ns3::Ptr<ns3::RadvdPrefix> > > ns3::RadvdInterface::GetPrefixes() const [member function]
    cls.add_method('GetPrefixes', 
                   'std::list< ns3::Ptr< ns3::RadvdPrefix > >', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetReachableTime() const [member function]
    cls.add_method('GetReachableTime', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetRetransTimer() const [member function]
    cls.add_method('GetRetransTimer', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: bool ns3::RadvdInterface::IsHomeAgentFlag() const [member function]
    cls.add_method('IsHomeAgentFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: bool ns3::RadvdInterface::IsHomeAgentInfo() const [member function]
    cls.add_method('IsHomeAgentInfo', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: bool ns3::RadvdInterface::IsIntervalOpt() const [member function]
    cls.add_method('IsIntervalOpt', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: bool ns3::RadvdInterface::IsManagedFlag() const [member function]
    cls.add_method('IsManagedFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: bool ns3::RadvdInterface::IsMobRtrSupportFlag() const [member function]
    cls.add_method('IsMobRtrSupportFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: bool ns3::RadvdInterface::IsOtherConfigFlag() const [member function]
    cls.add_method('IsOtherConfigFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: bool ns3::RadvdInterface::IsSendAdvert() const [member function]
    cls.add_method('IsSendAdvert', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: bool ns3::RadvdInterface::IsSourceLLAddress() const [member function]
    cls.add_method('IsSourceLLAddress', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: void ns3::RadvdInterface::SetCurHopLimit(uint8_t curHopLimit) [member function]
    cls.add_method('SetCurHopLimit', 
                   'void', 
                   [param('uint8_t', 'curHopLimit')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetDefaultLifeTime(uint32_t defaultLifeTime) [member function]
    cls.add_method('SetDefaultLifeTime', 
                   'void', 
                   [param('uint32_t', 'defaultLifeTime')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetDefaultPreference(uint8_t defaultPreference) [member function]
    cls.add_method('SetDefaultPreference', 
                   'void', 
                   [param('uint8_t', 'defaultPreference')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetHomeAgentFlag(bool homeAgentFlag) [member function]
    cls.add_method('SetHomeAgentFlag', 
                   'void', 
                   [param('bool', 'homeAgentFlag')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetHomeAgentInfo(bool homeAgentFlag) [member function]
    cls.add_method('SetHomeAgentInfo', 
                   'void', 
                   [param('bool', 'homeAgentFlag')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetHomeAgentLifeTime(uint32_t homeAgentLifeTime) [member function]
    cls.add_method('SetHomeAgentLifeTime', 
                   'void', 
                   [param('uint32_t', 'homeAgentLifeTime')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetHomeAgentPreference(uint32_t homeAgentPreference) [member function]
    cls.add_method('SetHomeAgentPreference', 
                   'void', 
                   [param('uint32_t', 'homeAgentPreference')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetIntervalOpt(bool intervalOpt) [member function]
    cls.add_method('SetIntervalOpt', 
                   'void', 
                   [param('bool', 'intervalOpt')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetLinkMtu(uint32_t linkMtu) [member function]
    cls.add_method('SetLinkMtu', 
                   'void', 
                   [param('uint32_t', 'linkMtu')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetManagedFlag(bool managedFlag) [member function]
    cls.add_method('SetManagedFlag', 
                   'void', 
                   [param('bool', 'managedFlag')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetMaxRtrAdvInterval(uint32_t maxRtrAdvInterval) [member function]
    cls.add_method('SetMaxRtrAdvInterval', 
                   'void', 
                   [param('uint32_t', 'maxRtrAdvInterval')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetMinDelayBetweenRAs(uint32_t minDelayBetweenRAs) [member function]
    cls.add_method('SetMinDelayBetweenRAs', 
                   'void', 
                   [param('uint32_t', 'minDelayBetweenRAs')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetMinRtrAdvInterval(uint32_t minRtrAdvInterval) [member function]
    cls.add_method('SetMinRtrAdvInterval', 
                   'void', 
                   [param('uint32_t', 'minRtrAdvInterval')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetMobRtrSupportFlag(bool mobRtrSupportFlag) [member function]
    cls.add_method('SetMobRtrSupportFlag', 
                   'void', 
                   [param('bool', 'mobRtrSupportFlag')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetOtherConfigFlag(bool otherConfigFlag) [member function]
    cls.add_method('SetOtherConfigFlag', 
                   'void', 
                   [param('bool', 'otherConfigFlag')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetReachableTime(uint32_t reachableTime) [member function]
    cls.add_method('SetReachableTime', 
                   'void', 
                   [param('uint32_t', 'reachableTime')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetRetransTimer(uint32_t retransTimer) [member function]
    cls.add_method('SetRetransTimer', 
                   'void', 
                   [param('uint32_t', 'retransTimer')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetSendAdvert(bool sendAdvert) [member function]
    cls.add_method('SetSendAdvert', 
                   'void', 
                   [param('bool', 'sendAdvert')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetSourceLLAddress(bool sourceLLAddress) [member function]
    cls.add_method('SetSourceLLAddress', 
                   'void', 
                   [param('bool', 'sourceLLAddress')])
    return

def register_Ns3RadvdPrefix_methods(root_module, cls):
    ## radvd-prefix.h: ns3::RadvdPrefix::RadvdPrefix(ns3::RadvdPrefix const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RadvdPrefix const &', 'arg0')])
    ## radvd-prefix.h: ns3::RadvdPrefix::RadvdPrefix(ns3::Ipv6Address network, uint8_t prefixLength, uint32_t preferredLifeTime=604800, uint32_t validLifeTime=2592000, bool onLinkFlag=true, bool autonomousFlag=true, bool routerAddrFlag=false) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address', 'network'), param('uint8_t', 'prefixLength'), param('uint32_t', 'preferredLifeTime', default_value='604800'), param('uint32_t', 'validLifeTime', default_value='2592000'), param('bool', 'onLinkFlag', default_value='true'), param('bool', 'autonomousFlag', default_value='true'), param('bool', 'routerAddrFlag', default_value='false')])
    ## radvd-prefix.h: ns3::Ipv6Address ns3::RadvdPrefix::GetNetwork() const [member function]
    cls.add_method('GetNetwork', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## radvd-prefix.h: uint32_t ns3::RadvdPrefix::GetPreferredLifeTime() const [member function]
    cls.add_method('GetPreferredLifeTime', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-prefix.h: uint8_t ns3::RadvdPrefix::GetPrefixLength() const [member function]
    cls.add_method('GetPrefixLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## radvd-prefix.h: uint32_t ns3::RadvdPrefix::GetValidLifeTime() const [member function]
    cls.add_method('GetValidLifeTime', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-prefix.h: bool ns3::RadvdPrefix::IsAutonomousFlag() const [member function]
    cls.add_method('IsAutonomousFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-prefix.h: bool ns3::RadvdPrefix::IsOnLinkFlag() const [member function]
    cls.add_method('IsOnLinkFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-prefix.h: bool ns3::RadvdPrefix::IsRouterAddrFlag() const [member function]
    cls.add_method('IsRouterAddrFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-prefix.h: void ns3::RadvdPrefix::SetAutonomousFlag(bool autonomousFlag) [member function]
    cls.add_method('SetAutonomousFlag', 
                   'void', 
                   [param('bool', 'autonomousFlag')])
    ## radvd-prefix.h: void ns3::RadvdPrefix::SetNetwork(ns3::Ipv6Address network) [member function]
    cls.add_method('SetNetwork', 
                   'void', 
                   [param('ns3::Ipv6Address', 'network')])
    ## radvd-prefix.h: void ns3::RadvdPrefix::SetOnLinkFlag(bool onLinkFlag) [member function]
    cls.add_method('SetOnLinkFlag', 
                   'void', 
                   [param('bool', 'onLinkFlag')])
    ## radvd-prefix.h: void ns3::RadvdPrefix::SetPreferredLifeTime(uint32_t preferredLifeTime) [member function]
    cls.add_method('SetPreferredLifeTime', 
                   'void', 
                   [param('uint32_t', 'preferredLifeTime')])
    ## radvd-prefix.h: void ns3::RadvdPrefix::SetPrefixLength(uint8_t prefixLength) [member function]
    cls.add_method('SetPrefixLength', 
                   'void', 
                   [param('uint8_t', 'prefixLength')])
    ## radvd-prefix.h: void ns3::RadvdPrefix::SetRouterAddrFlag(bool routerAddrFlag) [member function]
    cls.add_method('SetRouterAddrFlag', 
                   'void', 
                   [param('bool', 'routerAddrFlag')])
    ## radvd-prefix.h: void ns3::RadvdPrefix::SetValidLifeTime(uint32_t validLifeTime) [member function]
    cls.add_method('SetValidLifeTime', 
                   'void', 
                   [param('uint32_t', 'validLifeTime')])
    return

def register_functions(root_module):
    module = root_module
    register_functions_ns3_Config(module.get_submodule('Config'), root_module)
    register_functions_ns3_FatalImpl(module.get_submodule('FatalImpl'), root_module)
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

