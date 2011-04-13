from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## animation-interface.h: ns3::AnimationInterface [class]
    module.add_class('AnimationInterface')
    ## point-to-point-dumbbell-helper.h: ns3::PointToPointDumbbellHelper [class]
    module.add_class('PointToPointDumbbellHelper')
    ## point-to-point-grid-helper.h: ns3::PointToPointGridHelper [class]
    module.add_class('PointToPointGridHelper')
    ## point-to-point-star-helper.h: ns3::PointToPointStarHelper [class]
    module.add_class('PointToPointStarHelper')
    ## canvas-location.h: ns3::CanvasLocation [class]
    module.add_class('CanvasLocation', parent=root_module['ns3::Object'])
    
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
    

def register_types_ns3_dot11s(module):
    root_module = module.get_root()
    

def register_types_ns3_dsdv(module):
    root_module = module.get_root()
    

def register_types_ns3_flame(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3AnimationInterface_methods(root_module, root_module['ns3::AnimationInterface'])
    register_Ns3PointToPointDumbbellHelper_methods(root_module, root_module['ns3::PointToPointDumbbellHelper'])
    register_Ns3PointToPointGridHelper_methods(root_module, root_module['ns3::PointToPointGridHelper'])
    register_Ns3PointToPointStarHelper_methods(root_module, root_module['ns3::PointToPointStarHelper'])
    register_Ns3CanvasLocation_methods(root_module, root_module['ns3::CanvasLocation'])
    return

def register_Ns3AnimationInterface_methods(root_module, cls):
    ## animation-interface.h: ns3::AnimationInterface::AnimationInterface(ns3::AnimationInterface const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AnimationInterface const &', 'arg0')])
    ## animation-interface.h: ns3::AnimationInterface::AnimationInterface() [constructor]
    cls.add_constructor([])
    ## animation-interface.h: bool ns3::AnimationInterface::SetOutputFile(std::string const & fn) [member function]
    cls.add_method('SetOutputFile', 
                   'bool', 
                   [param('std::string const &', 'fn')])
    ## animation-interface.h: bool ns3::AnimationInterface::SetServerPort(uint16_t port) [member function]
    cls.add_method('SetServerPort', 
                   'bool', 
                   [param('uint16_t', 'port')])
    ## animation-interface.h: void ns3::AnimationInterface::StartAnimation() [member function]
    cls.add_method('StartAnimation', 
                   'void', 
                   [])
    ## animation-interface.h: void ns3::AnimationInterface::StopAnimation() [member function]
    cls.add_method('StopAnimation', 
                   'void', 
                   [])
    return

def register_Ns3PointToPointDumbbellHelper_methods(root_module, cls):
    ## point-to-point-dumbbell-helper.h: ns3::PointToPointDumbbellHelper::PointToPointDumbbellHelper(ns3::PointToPointDumbbellHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointToPointDumbbellHelper const &', 'arg0')])
    ## point-to-point-dumbbell-helper.h: ns3::PointToPointDumbbellHelper::PointToPointDumbbellHelper(uint32_t nLeftLeaf, ns3::PointToPointHelper leftHelper, uint32_t nRightLeaf, ns3::PointToPointHelper rightHelper, ns3::PointToPointHelper bottleneckHelper) [constructor]
    cls.add_constructor([param('uint32_t', 'nLeftLeaf'), param('ns3::PointToPointHelper', 'leftHelper'), param('uint32_t', 'nRightLeaf'), param('ns3::PointToPointHelper', 'rightHelper'), param('ns3::PointToPointHelper', 'bottleneckHelper')])
    ## point-to-point-dumbbell-helper.h: void ns3::PointToPointDumbbellHelper::AssignIpv4Addresses(ns3::Ipv4AddressHelper leftIp, ns3::Ipv4AddressHelper rightIp, ns3::Ipv4AddressHelper routerIp) [member function]
    cls.add_method('AssignIpv4Addresses', 
                   'void', 
                   [param('ns3::Ipv4AddressHelper', 'leftIp'), param('ns3::Ipv4AddressHelper', 'rightIp'), param('ns3::Ipv4AddressHelper', 'routerIp')])
    ## point-to-point-dumbbell-helper.h: void ns3::PointToPointDumbbellHelper::BoundingBox(double ulx, double uly, double lrx, double lry) [member function]
    cls.add_method('BoundingBox', 
                   'void', 
                   [param('double', 'ulx'), param('double', 'uly'), param('double', 'lrx'), param('double', 'lry')])
    ## point-to-point-dumbbell-helper.h: ns3::Ptr<ns3::Node> ns3::PointToPointDumbbellHelper::GetLeft() const [member function]
    cls.add_method('GetLeft', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True)
    ## point-to-point-dumbbell-helper.h: ns3::Ptr<ns3::Node> ns3::PointToPointDumbbellHelper::GetLeft(uint32_t i) const [member function]
    cls.add_method('GetLeft', 
                   'ns3::Ptr< ns3::Node >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## point-to-point-dumbbell-helper.h: ns3::Ipv4Address ns3::PointToPointDumbbellHelper::GetLeftIpv4Address(uint32_t i) const [member function]
    cls.add_method('GetLeftIpv4Address', 
                   'ns3::Ipv4Address', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## point-to-point-dumbbell-helper.h: ns3::Ptr<ns3::Node> ns3::PointToPointDumbbellHelper::GetRight() const [member function]
    cls.add_method('GetRight', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True)
    ## point-to-point-dumbbell-helper.h: ns3::Ptr<ns3::Node> ns3::PointToPointDumbbellHelper::GetRight(uint32_t i) const [member function]
    cls.add_method('GetRight', 
                   'ns3::Ptr< ns3::Node >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## point-to-point-dumbbell-helper.h: ns3::Ipv4Address ns3::PointToPointDumbbellHelper::GetRightIpv4Address(uint32_t i) const [member function]
    cls.add_method('GetRightIpv4Address', 
                   'ns3::Ipv4Address', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## point-to-point-dumbbell-helper.h: void ns3::PointToPointDumbbellHelper::InstallStack(ns3::InternetStackHelper stack) [member function]
    cls.add_method('InstallStack', 
                   'void', 
                   [param('ns3::InternetStackHelper', 'stack')])
    ## point-to-point-dumbbell-helper.h: uint32_t ns3::PointToPointDumbbellHelper::LeftCount() const [member function]
    cls.add_method('LeftCount', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## point-to-point-dumbbell-helper.h: uint32_t ns3::PointToPointDumbbellHelper::RightCount() const [member function]
    cls.add_method('RightCount', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3PointToPointGridHelper_methods(root_module, cls):
    ## point-to-point-grid-helper.h: ns3::PointToPointGridHelper::PointToPointGridHelper(ns3::PointToPointGridHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointToPointGridHelper const &', 'arg0')])
    ## point-to-point-grid-helper.h: ns3::PointToPointGridHelper::PointToPointGridHelper(uint32_t nRows, uint32_t nCols, ns3::PointToPointHelper pointToPoint) [constructor]
    cls.add_constructor([param('uint32_t', 'nRows'), param('uint32_t', 'nCols'), param('ns3::PointToPointHelper', 'pointToPoint')])
    ## point-to-point-grid-helper.h: void ns3::PointToPointGridHelper::AssignIpv4Addresses(ns3::Ipv4AddressHelper rowIp, ns3::Ipv4AddressHelper colIp) [member function]
    cls.add_method('AssignIpv4Addresses', 
                   'void', 
                   [param('ns3::Ipv4AddressHelper', 'rowIp'), param('ns3::Ipv4AddressHelper', 'colIp')])
    ## point-to-point-grid-helper.h: void ns3::PointToPointGridHelper::BoundingBox(double ulx, double uly, double lrx, double lry) [member function]
    cls.add_method('BoundingBox', 
                   'void', 
                   [param('double', 'ulx'), param('double', 'uly'), param('double', 'lrx'), param('double', 'lry')])
    ## point-to-point-grid-helper.h: ns3::Ipv4Address ns3::PointToPointGridHelper::GetIpv4Address(uint32_t row, uint32_t col) [member function]
    cls.add_method('GetIpv4Address', 
                   'ns3::Ipv4Address', 
                   [param('uint32_t', 'row'), param('uint32_t', 'col')])
    ## point-to-point-grid-helper.h: ns3::Ptr<ns3::Node> ns3::PointToPointGridHelper::GetNode(uint32_t row, uint32_t col) [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [param('uint32_t', 'row'), param('uint32_t', 'col')])
    ## point-to-point-grid-helper.h: void ns3::PointToPointGridHelper::InstallStack(ns3::InternetStackHelper stack) [member function]
    cls.add_method('InstallStack', 
                   'void', 
                   [param('ns3::InternetStackHelper', 'stack')])
    return

def register_Ns3PointToPointStarHelper_methods(root_module, cls):
    ## point-to-point-star-helper.h: ns3::PointToPointStarHelper::PointToPointStarHelper(ns3::PointToPointStarHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointToPointStarHelper const &', 'arg0')])
    ## point-to-point-star-helper.h: ns3::PointToPointStarHelper::PointToPointStarHelper(uint32_t numSpokes, ns3::PointToPointHelper p2pHelper) [constructor]
    cls.add_constructor([param('uint32_t', 'numSpokes'), param('ns3::PointToPointHelper', 'p2pHelper')])
    ## point-to-point-star-helper.h: void ns3::PointToPointStarHelper::AssignIpv4Addresses(ns3::Ipv4AddressHelper address) [member function]
    cls.add_method('AssignIpv4Addresses', 
                   'void', 
                   [param('ns3::Ipv4AddressHelper', 'address')])
    ## point-to-point-star-helper.h: void ns3::PointToPointStarHelper::BoundingBox(double ulx, double uly, double lrx, double lry) [member function]
    cls.add_method('BoundingBox', 
                   'void', 
                   [param('double', 'ulx'), param('double', 'uly'), param('double', 'lrx'), param('double', 'lry')])
    ## point-to-point-star-helper.h: ns3::Ptr<ns3::Node> ns3::PointToPointStarHelper::GetHub() const [member function]
    cls.add_method('GetHub', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True)
    ## point-to-point-star-helper.h: ns3::Ipv4Address ns3::PointToPointStarHelper::GetHubIpv4Address(uint32_t i) const [member function]
    cls.add_method('GetHubIpv4Address', 
                   'ns3::Ipv4Address', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## point-to-point-star-helper.h: ns3::Ipv4Address ns3::PointToPointStarHelper::GetSpokeIpv4Address(uint32_t i) const [member function]
    cls.add_method('GetSpokeIpv4Address', 
                   'ns3::Ipv4Address', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## point-to-point-star-helper.h: ns3::Ptr<ns3::Node> ns3::PointToPointStarHelper::GetSpokeNode(uint32_t i) const [member function]
    cls.add_method('GetSpokeNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## point-to-point-star-helper.h: void ns3::PointToPointStarHelper::InstallStack(ns3::InternetStackHelper stack) [member function]
    cls.add_method('InstallStack', 
                   'void', 
                   [param('ns3::InternetStackHelper', 'stack')])
    ## point-to-point-star-helper.h: uint32_t ns3::PointToPointStarHelper::SpokeCount() const [member function]
    cls.add_method('SpokeCount', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3CanvasLocation_methods(root_module, cls):
    ## canvas-location.h: ns3::CanvasLocation::CanvasLocation(ns3::CanvasLocation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CanvasLocation const &', 'arg0')])
    ## canvas-location.h: ns3::CanvasLocation::CanvasLocation() [constructor]
    cls.add_constructor([])
    ## canvas-location.h: ns3::Vector ns3::CanvasLocation::GetLocation() const [member function]
    cls.add_method('GetLocation', 
                   'ns3::Vector', 
                   [], 
                   is_const=True)
    ## canvas-location.h: static ns3::TypeId ns3::CanvasLocation::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## canvas-location.h: void ns3::CanvasLocation::SetLocation(ns3::Vector const & location) [member function]
    cls.add_method('SetLocation', 
                   'void', 
                   [param('ns3::Vector const &', 'location')])
    return

def register_functions(root_module):
    module = root_module
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

