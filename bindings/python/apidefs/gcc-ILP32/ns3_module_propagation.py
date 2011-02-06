from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## propagation-delay-model.h: ns3::PropagationDelayModel [class]
    module.add_class('PropagationDelayModel', parent=root_module['ns3::Object'])
    ## propagation-loss-model.h: ns3::PropagationLossModel [class]
    module.add_class('PropagationLossModel', parent=root_module['ns3::Object'])
    ## propagation-delay-model.h: ns3::RandomPropagationDelayModel [class]
    module.add_class('RandomPropagationDelayModel', parent=root_module['ns3::PropagationDelayModel'])
    ## propagation-loss-model.h: ns3::RandomPropagationLossModel [class]
    module.add_class('RandomPropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## propagation-loss-model.h: ns3::RangePropagationLossModel [class]
    module.add_class('RangePropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## propagation-loss-model.h: ns3::ThreeLogDistancePropagationLossModel [class]
    module.add_class('ThreeLogDistancePropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## propagation-loss-model.h: ns3::TwoRayGroundPropagationLossModel [class]
    module.add_class('TwoRayGroundPropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## propagation-delay-model.h: ns3::ConstantSpeedPropagationDelayModel [class]
    module.add_class('ConstantSpeedPropagationDelayModel', parent=root_module['ns3::PropagationDelayModel'])
    ## cost231-propagation-loss-model.h: ns3::Cost231PropagationLossModel [class]
    module.add_class('Cost231PropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## cost231-propagation-loss-model.h: ns3::Cost231PropagationLossModel::Environment [enumeration]
    module.add_enum('Environment', ['SubUrban', 'MediumCity', 'Metropolitan'], outer_class=root_module['ns3::Cost231PropagationLossModel'])
    ## propagation-loss-model.h: ns3::FixedRssLossModel [class]
    module.add_class('FixedRssLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## propagation-loss-model.h: ns3::FriisPropagationLossModel [class]
    module.add_class('FriisPropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## jakes-propagation-loss-model.h: ns3::JakesPropagationLossModel [class]
    module.add_class('JakesPropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## propagation-loss-model.h: ns3::LogDistancePropagationLossModel [class]
    module.add_class('LogDistancePropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## propagation-loss-model.h: ns3::MatrixPropagationLossModel [class]
    module.add_class('MatrixPropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## propagation-loss-model.h: ns3::NakagamiPropagationLossModel [class]
    module.add_class('NakagamiPropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    
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
    register_Ns3PropagationDelayModel_methods(root_module, root_module['ns3::PropagationDelayModel'])
    register_Ns3PropagationLossModel_methods(root_module, root_module['ns3::PropagationLossModel'])
    register_Ns3RandomPropagationDelayModel_methods(root_module, root_module['ns3::RandomPropagationDelayModel'])
    register_Ns3RandomPropagationLossModel_methods(root_module, root_module['ns3::RandomPropagationLossModel'])
    register_Ns3RangePropagationLossModel_methods(root_module, root_module['ns3::RangePropagationLossModel'])
    register_Ns3ThreeLogDistancePropagationLossModel_methods(root_module, root_module['ns3::ThreeLogDistancePropagationLossModel'])
    register_Ns3TwoRayGroundPropagationLossModel_methods(root_module, root_module['ns3::TwoRayGroundPropagationLossModel'])
    register_Ns3ConstantSpeedPropagationDelayModel_methods(root_module, root_module['ns3::ConstantSpeedPropagationDelayModel'])
    register_Ns3Cost231PropagationLossModel_methods(root_module, root_module['ns3::Cost231PropagationLossModel'])
    register_Ns3FixedRssLossModel_methods(root_module, root_module['ns3::FixedRssLossModel'])
    register_Ns3FriisPropagationLossModel_methods(root_module, root_module['ns3::FriisPropagationLossModel'])
    register_Ns3JakesPropagationLossModel_methods(root_module, root_module['ns3::JakesPropagationLossModel'])
    register_Ns3LogDistancePropagationLossModel_methods(root_module, root_module['ns3::LogDistancePropagationLossModel'])
    register_Ns3MatrixPropagationLossModel_methods(root_module, root_module['ns3::MatrixPropagationLossModel'])
    register_Ns3NakagamiPropagationLossModel_methods(root_module, root_module['ns3::NakagamiPropagationLossModel'])
    return

def register_Ns3PropagationDelayModel_methods(root_module, cls):
    ## propagation-delay-model.h: ns3::PropagationDelayModel::PropagationDelayModel() [constructor]
    cls.add_constructor([])
    ## propagation-delay-model.h: ns3::PropagationDelayModel::PropagationDelayModel(ns3::PropagationDelayModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PropagationDelayModel const &', 'arg0')])
    ## propagation-delay-model.h: ns3::Time ns3::PropagationDelayModel::GetDelay(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## propagation-delay-model.h: static ns3::TypeId ns3::PropagationDelayModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3PropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::PropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::PropagationLossModel::PropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: void ns3::PropagationLossModel::SetNext(ns3::Ptr<ns3::PropagationLossModel> next) [member function]
    cls.add_method('SetNext', 
                   'void', 
                   [param('ns3::Ptr< ns3::PropagationLossModel >', 'next')])
    ## propagation-loss-model.h: double ns3::PropagationLossModel::CalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('CalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True)
    ## propagation-loss-model.h: double ns3::PropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3RandomPropagationDelayModel_methods(root_module, cls):
    ## propagation-delay-model.h: ns3::RandomPropagationDelayModel::RandomPropagationDelayModel(ns3::RandomPropagationDelayModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RandomPropagationDelayModel const &', 'arg0')])
    ## propagation-delay-model.h: ns3::RandomPropagationDelayModel::RandomPropagationDelayModel() [constructor]
    cls.add_constructor([])
    ## propagation-delay-model.h: ns3::Time ns3::RandomPropagationDelayModel::GetDelay(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, is_virtual=True)
    ## propagation-delay-model.h: static ns3::TypeId ns3::RandomPropagationDelayModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3RandomPropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::RandomPropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::RandomPropagationLossModel::RandomPropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: double ns3::RandomPropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3RangePropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::RangePropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::RangePropagationLossModel::RangePropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: double ns3::RangePropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3ThreeLogDistancePropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::ThreeLogDistancePropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::ThreeLogDistancePropagationLossModel::ThreeLogDistancePropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: double ns3::ThreeLogDistancePropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3TwoRayGroundPropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::TwoRayGroundPropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::TwoRayGroundPropagationLossModel::TwoRayGroundPropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: void ns3::TwoRayGroundPropagationLossModel::SetLambda(double frequency, double speed) [member function]
    cls.add_method('SetLambda', 
                   'void', 
                   [param('double', 'frequency'), param('double', 'speed')])
    ## propagation-loss-model.h: void ns3::TwoRayGroundPropagationLossModel::SetLambda(double lambda) [member function]
    cls.add_method('SetLambda', 
                   'void', 
                   [param('double', 'lambda')])
    ## propagation-loss-model.h: void ns3::TwoRayGroundPropagationLossModel::SetSystemLoss(double systemLoss) [member function]
    cls.add_method('SetSystemLoss', 
                   'void', 
                   [param('double', 'systemLoss')])
    ## propagation-loss-model.h: void ns3::TwoRayGroundPropagationLossModel::SetMinDistance(double minDistance) [member function]
    cls.add_method('SetMinDistance', 
                   'void', 
                   [param('double', 'minDistance')])
    ## propagation-loss-model.h: double ns3::TwoRayGroundPropagationLossModel::GetMinDistance() const [member function]
    cls.add_method('GetMinDistance', 
                   'double', 
                   [], 
                   is_const=True)
    ## propagation-loss-model.h: double ns3::TwoRayGroundPropagationLossModel::GetLambda() const [member function]
    cls.add_method('GetLambda', 
                   'double', 
                   [], 
                   is_const=True)
    ## propagation-loss-model.h: double ns3::TwoRayGroundPropagationLossModel::GetSystemLoss() const [member function]
    cls.add_method('GetSystemLoss', 
                   'double', 
                   [], 
                   is_const=True)
    ## propagation-loss-model.h: void ns3::TwoRayGroundPropagationLossModel::SetHeightAboveZ(double heightAboveZ) [member function]
    cls.add_method('SetHeightAboveZ', 
                   'void', 
                   [param('double', 'heightAboveZ')])
    ## propagation-loss-model.h: double ns3::TwoRayGroundPropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3ConstantSpeedPropagationDelayModel_methods(root_module, cls):
    ## propagation-delay-model.h: ns3::ConstantSpeedPropagationDelayModel::ConstantSpeedPropagationDelayModel(ns3::ConstantSpeedPropagationDelayModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ConstantSpeedPropagationDelayModel const &', 'arg0')])
    ## propagation-delay-model.h: ns3::ConstantSpeedPropagationDelayModel::ConstantSpeedPropagationDelayModel() [constructor]
    cls.add_constructor([])
    ## propagation-delay-model.h: ns3::Time ns3::ConstantSpeedPropagationDelayModel::GetDelay(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, is_virtual=True)
    ## propagation-delay-model.h: double ns3::ConstantSpeedPropagationDelayModel::GetSpeed() const [member function]
    cls.add_method('GetSpeed', 
                   'double', 
                   [], 
                   is_const=True)
    ## propagation-delay-model.h: static ns3::TypeId ns3::ConstantSpeedPropagationDelayModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-delay-model.h: void ns3::ConstantSpeedPropagationDelayModel::SetSpeed(double speed) [member function]
    cls.add_method('SetSpeed', 
                   'void', 
                   [param('double', 'speed')])
    return

def register_Ns3Cost231PropagationLossModel_methods(root_module, cls):
    ## cost231-propagation-loss-model.h: static ns3::TypeId ns3::Cost231PropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## cost231-propagation-loss-model.h: ns3::Cost231PropagationLossModel::Cost231PropagationLossModel() [constructor]
    cls.add_constructor([])
    ## cost231-propagation-loss-model.h: double ns3::Cost231PropagationLossModel::GetLoss(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetLoss', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True)
    ## cost231-propagation-loss-model.h: void ns3::Cost231PropagationLossModel::SetBSAntennaHeight(double height) [member function]
    cls.add_method('SetBSAntennaHeight', 
                   'void', 
                   [param('double', 'height')])
    ## cost231-propagation-loss-model.h: void ns3::Cost231PropagationLossModel::SetSSAntennaHeight(double height) [member function]
    cls.add_method('SetSSAntennaHeight', 
                   'void', 
                   [param('double', 'height')])
    ## cost231-propagation-loss-model.h: void ns3::Cost231PropagationLossModel::SetEnvironment(ns3::Cost231PropagationLossModel::Environment env) [member function]
    cls.add_method('SetEnvironment', 
                   'void', 
                   [param('ns3::Cost231PropagationLossModel::Environment', 'env')])
    ## cost231-propagation-loss-model.h: void ns3::Cost231PropagationLossModel::SetLambda(double lambda) [member function]
    cls.add_method('SetLambda', 
                   'void', 
                   [param('double', 'lambda')])
    ## cost231-propagation-loss-model.h: void ns3::Cost231PropagationLossModel::SetMinDistance(double minDistance) [member function]
    cls.add_method('SetMinDistance', 
                   'void', 
                   [param('double', 'minDistance')])
    ## cost231-propagation-loss-model.h: double ns3::Cost231PropagationLossModel::GetBSAntennaHeight() const [member function]
    cls.add_method('GetBSAntennaHeight', 
                   'double', 
                   [], 
                   is_const=True)
    ## cost231-propagation-loss-model.h: double ns3::Cost231PropagationLossModel::GetSSAntennaHeight() const [member function]
    cls.add_method('GetSSAntennaHeight', 
                   'double', 
                   [], 
                   is_const=True)
    ## cost231-propagation-loss-model.h: ns3::Cost231PropagationLossModel::Environment ns3::Cost231PropagationLossModel::GetEnvironment() const [member function]
    cls.add_method('GetEnvironment', 
                   'ns3::Cost231PropagationLossModel::Environment', 
                   [], 
                   is_const=True)
    ## cost231-propagation-loss-model.h: double ns3::Cost231PropagationLossModel::GetMinDistance() const [member function]
    cls.add_method('GetMinDistance', 
                   'double', 
                   [], 
                   is_const=True)
    ## cost231-propagation-loss-model.h: double ns3::Cost231PropagationLossModel::GetLambda() const [member function]
    cls.add_method('GetLambda', 
                   'double', 
                   [], 
                   is_const=True)
    ## cost231-propagation-loss-model.h: void ns3::Cost231PropagationLossModel::SetLambda(double frequency, double speed) [member function]
    cls.add_method('SetLambda', 
                   'void', 
                   [param('double', 'frequency'), param('double', 'speed')])
    ## cost231-propagation-loss-model.h: double ns3::Cost231PropagationLossModel::GetShadowing() [member function]
    cls.add_method('GetShadowing', 
                   'double', 
                   [])
    ## cost231-propagation-loss-model.h: void ns3::Cost231PropagationLossModel::SetShadowing(double shadowing) [member function]
    cls.add_method('SetShadowing', 
                   'void', 
                   [param('double', 'shadowing')])
    ## cost231-propagation-loss-model.h: double ns3::Cost231PropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3FixedRssLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::FixedRssLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::FixedRssLossModel::FixedRssLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: void ns3::FixedRssLossModel::SetRss(double rss) [member function]
    cls.add_method('SetRss', 
                   'void', 
                   [param('double', 'rss')])
    ## propagation-loss-model.h: double ns3::FixedRssLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3FriisPropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::FriisPropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::FriisPropagationLossModel::FriisPropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: void ns3::FriisPropagationLossModel::SetLambda(double frequency, double speed) [member function]
    cls.add_method('SetLambda', 
                   'void', 
                   [param('double', 'frequency'), param('double', 'speed')])
    ## propagation-loss-model.h: void ns3::FriisPropagationLossModel::SetLambda(double lambda) [member function]
    cls.add_method('SetLambda', 
                   'void', 
                   [param('double', 'lambda')])
    ## propagation-loss-model.h: void ns3::FriisPropagationLossModel::SetSystemLoss(double systemLoss) [member function]
    cls.add_method('SetSystemLoss', 
                   'void', 
                   [param('double', 'systemLoss')])
    ## propagation-loss-model.h: void ns3::FriisPropagationLossModel::SetMinDistance(double minDistance) [member function]
    cls.add_method('SetMinDistance', 
                   'void', 
                   [param('double', 'minDistance')])
    ## propagation-loss-model.h: double ns3::FriisPropagationLossModel::GetMinDistance() const [member function]
    cls.add_method('GetMinDistance', 
                   'double', 
                   [], 
                   is_const=True)
    ## propagation-loss-model.h: double ns3::FriisPropagationLossModel::GetLambda() const [member function]
    cls.add_method('GetLambda', 
                   'double', 
                   [], 
                   is_const=True)
    ## propagation-loss-model.h: double ns3::FriisPropagationLossModel::GetSystemLoss() const [member function]
    cls.add_method('GetSystemLoss', 
                   'double', 
                   [], 
                   is_const=True)
    ## propagation-loss-model.h: double ns3::FriisPropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3JakesPropagationLossModel_methods(root_module, cls):
    ## jakes-propagation-loss-model.h: ns3::JakesPropagationLossModel::JakesPropagationLossModel() [constructor]
    cls.add_constructor([])
    ## jakes-propagation-loss-model.h: uint8_t ns3::JakesPropagationLossModel::GetNOscillators() const [member function]
    cls.add_method('GetNOscillators', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## jakes-propagation-loss-model.h: uint8_t ns3::JakesPropagationLossModel::GetNRays() const [member function]
    cls.add_method('GetNRays', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## jakes-propagation-loss-model.h: static ns3::TypeId ns3::JakesPropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## jakes-propagation-loss-model.h: void ns3::JakesPropagationLossModel::SetNOscillators(uint8_t nOscillators) [member function]
    cls.add_method('SetNOscillators', 
                   'void', 
                   [param('uint8_t', 'nOscillators')])
    ## jakes-propagation-loss-model.h: void ns3::JakesPropagationLossModel::SetNRays(uint8_t nRays) [member function]
    cls.add_method('SetNRays', 
                   'void', 
                   [param('uint8_t', 'nRays')])
    ## jakes-propagation-loss-model.h: double ns3::JakesPropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3LogDistancePropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::LogDistancePropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::LogDistancePropagationLossModel::LogDistancePropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: void ns3::LogDistancePropagationLossModel::SetPathLossExponent(double n) [member function]
    cls.add_method('SetPathLossExponent', 
                   'void', 
                   [param('double', 'n')])
    ## propagation-loss-model.h: double ns3::LogDistancePropagationLossModel::GetPathLossExponent() const [member function]
    cls.add_method('GetPathLossExponent', 
                   'double', 
                   [], 
                   is_const=True)
    ## propagation-loss-model.h: void ns3::LogDistancePropagationLossModel::SetReference(double referenceDistance, double referenceLoss) [member function]
    cls.add_method('SetReference', 
                   'void', 
                   [param('double', 'referenceDistance'), param('double', 'referenceLoss')])
    ## propagation-loss-model.h: double ns3::LogDistancePropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3MatrixPropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::MatrixPropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::MatrixPropagationLossModel::MatrixPropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: void ns3::MatrixPropagationLossModel::SetLoss(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b, double loss, bool symmetric=true) [member function]
    cls.add_method('SetLoss', 
                   'void', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b'), param('double', 'loss'), param('bool', 'symmetric', default_value='true')])
    ## propagation-loss-model.h: void ns3::MatrixPropagationLossModel::SetDefaultLoss(double arg0) [member function]
    cls.add_method('SetDefaultLoss', 
                   'void', 
                   [param('double', 'arg0')])
    ## propagation-loss-model.h: double ns3::MatrixPropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3NakagamiPropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::NakagamiPropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::NakagamiPropagationLossModel::NakagamiPropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: double ns3::NakagamiPropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
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

