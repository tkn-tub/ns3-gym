from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## device-energy-model-container.h: ns3::DeviceEnergyModelContainer [class]
    module.add_class('DeviceEnergyModelContainer')
    ## energy-model-helper.h: ns3::DeviceEnergyModelHelper [class]
    module.add_class('DeviceEnergyModelHelper', allow_subclassing=True)
    ## energy-model-helper.h: ns3::EnergySourceHelper [class]
    module.add_class('EnergySourceHelper', allow_subclassing=True)
    ## rv-battery-model-helper.h: ns3::RvBatteryModelHelper [class]
    module.add_class('RvBatteryModelHelper', parent=root_module['ns3::EnergySourceHelper'])
    ## wifi-radio-energy-model-helper.h: ns3::WifiRadioEnergyModelHelper [class]
    module.add_class('WifiRadioEnergyModelHelper', parent=root_module['ns3::DeviceEnergyModelHelper'])
    ## wifi-radio-energy-model.h: ns3::WifiRadioEnergyModelPhyListener [class]
    module.add_class('WifiRadioEnergyModelPhyListener', parent=root_module['ns3::WifiPhyListener'])
    ## basic-energy-source-helper.h: ns3::BasicEnergySourceHelper [class]
    module.add_class('BasicEnergySourceHelper', parent=root_module['ns3::EnergySourceHelper'])
    ## device-energy-model.h: ns3::DeviceEnergyModel [class]
    module.add_class('DeviceEnergyModel', parent=root_module['ns3::Object'])
    ## energy-source.h: ns3::EnergySource [class]
    module.add_class('EnergySource', parent=root_module['ns3::Object'])
    ## energy-source-container.h: ns3::EnergySourceContainer [class]
    module.add_class('EnergySourceContainer', parent=root_module['ns3::Object'])
    ## li-ion-energy-source.h: ns3::LiIonEnergySource [class]
    module.add_class('LiIonEnergySource', parent=root_module['ns3::EnergySource'])
    ## rv-battery-model.h: ns3::RvBatteryModel [class]
    module.add_class('RvBatteryModel', parent=root_module['ns3::EnergySource'])
    ## simple-device-energy-model.h: ns3::SimpleDeviceEnergyModel [class]
    module.add_class('SimpleDeviceEnergyModel', parent=root_module['ns3::DeviceEnergyModel'])
    ## wifi-radio-energy-model.h: ns3::WifiRadioEnergyModel [class]
    module.add_class('WifiRadioEnergyModel', parent=root_module['ns3::DeviceEnergyModel'])
    ## basic-energy-source.h: ns3::BasicEnergySource [class]
    module.add_class('BasicEnergySource', parent=root_module['ns3::EnergySource'])
    
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
    register_Ns3DeviceEnergyModelContainer_methods(root_module, root_module['ns3::DeviceEnergyModelContainer'])
    register_Ns3DeviceEnergyModelHelper_methods(root_module, root_module['ns3::DeviceEnergyModelHelper'])
    register_Ns3EnergySourceHelper_methods(root_module, root_module['ns3::EnergySourceHelper'])
    register_Ns3RvBatteryModelHelper_methods(root_module, root_module['ns3::RvBatteryModelHelper'])
    register_Ns3WifiRadioEnergyModelHelper_methods(root_module, root_module['ns3::WifiRadioEnergyModelHelper'])
    register_Ns3WifiRadioEnergyModelPhyListener_methods(root_module, root_module['ns3::WifiRadioEnergyModelPhyListener'])
    register_Ns3BasicEnergySourceHelper_methods(root_module, root_module['ns3::BasicEnergySourceHelper'])
    register_Ns3DeviceEnergyModel_methods(root_module, root_module['ns3::DeviceEnergyModel'])
    register_Ns3EnergySource_methods(root_module, root_module['ns3::EnergySource'])
    register_Ns3EnergySourceContainer_methods(root_module, root_module['ns3::EnergySourceContainer'])
    register_Ns3LiIonEnergySource_methods(root_module, root_module['ns3::LiIonEnergySource'])
    register_Ns3RvBatteryModel_methods(root_module, root_module['ns3::RvBatteryModel'])
    register_Ns3SimpleDeviceEnergyModel_methods(root_module, root_module['ns3::SimpleDeviceEnergyModel'])
    register_Ns3WifiRadioEnergyModel_methods(root_module, root_module['ns3::WifiRadioEnergyModel'])
    register_Ns3BasicEnergySource_methods(root_module, root_module['ns3::BasicEnergySource'])
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

