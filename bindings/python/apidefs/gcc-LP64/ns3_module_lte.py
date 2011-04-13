from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## lte-helper.h: ns3::LteHelper [class]
    module.add_class('LteHelper')
    ## lte-helper.h: ns3::LteHelper::NetDeviceType [enumeration]
    module.add_enum('NetDeviceType', ['DEVICE_TYPE_USER_EQUIPMENT', 'DEVICE_TYPE_ENODEB'], outer_class=root_module['ns3::LteHelper'])
    ## lte-spectrum-value-helper.h: ns3::LteSpectrumValueHelper [class]
    module.add_class('LteSpectrumValueHelper')
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
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::IdealControlMessage, ns3::empty, ns3::DefaultDeleter<ns3::IdealControlMessage> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::IdealControlMessage', 'ns3::empty', 'ns3::DefaultDeleter<ns3::IdealControlMessage>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
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
    ## discrete-time-loss-model.h: ns3::DiscreteTimeLossModel [class]
    module.add_class('DiscreteTimeLossModel', parent=root_module['ns3::Object'])
    ## ideal-control-messages.h: ns3::IdealControlMessage [class]
    module.add_class('IdealControlMessage', parent=root_module['ns3::SimpleRefCount< ns3::IdealControlMessage, ns3::empty, ns3::DefaultDeleter<ns3::IdealControlMessage> >'])
    ## ideal-control-messages.h: ns3::IdealControlMessage::MessageType [enumeration]
    module.add_enum('MessageType', ['CQI_FEEDBACKS', 'ALLOCATION_MAP'], outer_class=root_module['ns3::IdealControlMessage'])
    ## jakes-fading-loss-model.h: ns3::JakesFadingLossModel [class]
    module.add_class('JakesFadingLossModel', parent=root_module['ns3::DiscreteTimeLossModel'])
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
    ## shadowing-loss-model.h: ns3::ShadowingLossModel [class]
    module.add_class('ShadowingLossModel', parent=root_module['ns3::DiscreteTimeLossModel'])
    ## ue-phy.h: ns3::UeLtePhy [class]
    module.add_class('UeLtePhy', parent=root_module['ns3::LtePhy'])
    ## ue-lte-spectrum-phy.h: ns3::UeLteSpectrumPhy [class]
    module.add_class('UeLteSpectrumPhy', parent=root_module['ns3::LteSpectrumPhy'])
    ## ue-mac-entity.h: ns3::UeMacEntity [class]
    module.add_class('UeMacEntity', parent=root_module['ns3::MacEntity'])
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
    ## lte-net-device.h: ns3::LteNetDevice [class]
    module.add_class('LteNetDevice', parent=root_module['ns3::NetDevice'])
    ## ue-net-device.h: ns3::UeNetDevice [class]
    module.add_class('UeNetDevice', parent=root_module['ns3::LteNetDevice'])
    ## enb-net-device.h: ns3::EnbNetDevice [class]
    module.add_class('EnbNetDevice', parent=root_module['ns3::LteNetDevice'])
    module.add_container('std::vector< ns3::UeRecord::CqiFeedback >', 'ns3::UeRecord::CqiFeedback', container_type='vector')
    module.add_container('std::deque< ns3::LteMacQueue::QueueElement >', 'ns3::LteMacQueue::QueueElement', container_type='dequeue')
    module.add_container('std::list< ns3::PdcchMapIdealControlMessage::IdealPdcchRecord >', 'ns3::PdcchMapIdealControlMessage::IdealPdcchRecord', container_type='list')
    module.add_container('std::list< ns3::CqiIdealControlMessage::CqiFeedback >', 'ns3::CqiIdealControlMessage::CqiFeedback', container_type='list')
    
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
    register_Ns3LteHelper_methods(root_module, root_module['ns3::LteHelper'])
    register_Ns3LteSpectrumValueHelper_methods(root_module, root_module['ns3::LteSpectrumValueHelper'])
    register_Ns3LteMacHeader_methods(root_module, root_module['ns3::LteMacHeader'])
    register_Ns3PacketScheduler_methods(root_module, root_module['ns3::PacketScheduler'])
    register_Ns3RadioBearerInstance_methods(root_module, root_module['ns3::RadioBearerInstance'])
    register_Ns3RlcEntity_methods(root_module, root_module['ns3::RlcEntity'])
    register_Ns3RrcEntity_methods(root_module, root_module['ns3::RrcEntity'])
    register_Ns3SimplePacketScheduler_methods(root_module, root_module['ns3::SimplePacketScheduler'])
    register_Ns3UeManager_methods(root_module, root_module['ns3::UeManager'])
    register_Ns3UeRecord_methods(root_module, root_module['ns3::UeRecord'])
    register_Ns3UeRecordCqiFeedback_methods(root_module, root_module['ns3::UeRecord::CqiFeedback'])
    register_Ns3AmcModule_methods(root_module, root_module['ns3::AmcModule'])
    register_Ns3BearerQosParameters_methods(root_module, root_module['ns3::BearerQosParameters'])
    register_Ns3ChannelRealization_methods(root_module, root_module['ns3::ChannelRealization'])
    register_Ns3DiscreteTimeLossModel_methods(root_module, root_module['ns3::DiscreteTimeLossModel'])
    register_Ns3IdealControlMessage_methods(root_module, root_module['ns3::IdealControlMessage'])
    register_Ns3JakesFadingLossModel_methods(root_module, root_module['ns3::JakesFadingLossModel'])
    register_Ns3LteMacQueue_methods(root_module, root_module['ns3::LteMacQueue'])
    register_Ns3LtePhy_methods(root_module, root_module['ns3::LtePhy'])
    register_Ns3LtePropagationLossModel_methods(root_module, root_module['ns3::LtePropagationLossModel'])
    register_Ns3LteSpectrumPhy_methods(root_module, root_module['ns3::LteSpectrumPhy'])
    register_Ns3MacEntity_methods(root_module, root_module['ns3::MacEntity'])
    register_Ns3PathLossModel_methods(root_module, root_module['ns3::PathLossModel'])
    register_Ns3PdcchMapIdealControlMessage_methods(root_module, root_module['ns3::PdcchMapIdealControlMessage'])
    register_Ns3PdcchMapIdealControlMessageIdealPdcchRecord_methods(root_module, root_module['ns3::PdcchMapIdealControlMessage::IdealPdcchRecord'])
    register_Ns3PenetrationLossModel_methods(root_module, root_module['ns3::PenetrationLossModel'])
    register_Ns3ShadowingLossModel_methods(root_module, root_module['ns3::ShadowingLossModel'])
    register_Ns3UeLtePhy_methods(root_module, root_module['ns3::UeLtePhy'])
    register_Ns3UeLteSpectrumPhy_methods(root_module, root_module['ns3::UeLteSpectrumPhy'])
    register_Ns3UeMacEntity_methods(root_module, root_module['ns3::UeMacEntity'])
    register_Ns3CqiIdealControlMessage_methods(root_module, root_module['ns3::CqiIdealControlMessage'])
    register_Ns3CqiIdealControlMessageCqiFeedback_methods(root_module, root_module['ns3::CqiIdealControlMessage::CqiFeedback'])
    register_Ns3EnbLtePhy_methods(root_module, root_module['ns3::EnbLtePhy'])
    register_Ns3EnbLteSpectrumPhy_methods(root_module, root_module['ns3::EnbLteSpectrumPhy'])
    register_Ns3EnbMacEntity_methods(root_module, root_module['ns3::EnbMacEntity'])
    register_Ns3LteNetDevice_methods(root_module, root_module['ns3::LteNetDevice'])
    register_Ns3UeNetDevice_methods(root_module, root_module['ns3::UeNetDevice'])
    register_Ns3EnbNetDevice_methods(root_module, root_module['ns3::EnbNetDevice'])
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
    ## lte-spectrum-phy.h: void ns3::LteSpectrumPhy::SetGenericPhyRxEndErrorCallback(ns3::GenericPhyRxEndErrorCallback c) [member function]
    cls.add_method('SetGenericPhyRxEndErrorCallback', 
                   'void', 
                   [param('ns3::GenericPhyRxEndErrorCallback', 'c')])
    ## lte-spectrum-phy.h: void ns3::LteSpectrumPhy::SetGenericPhyRxEndOkCallback(ns3::GenericPhyRxEndOkCallback c) [member function]
    cls.add_method('SetGenericPhyRxEndOkCallback', 
                   'void', 
                   [param('ns3::GenericPhyRxEndOkCallback', 'c')])
    ## lte-spectrum-phy.h: void ns3::LteSpectrumPhy::SetGenericPhyRxStartCallback(ns3::GenericPhyRxStartCallback c) [member function]
    cls.add_method('SetGenericPhyRxStartCallback', 
                   'void', 
                   [param('ns3::GenericPhyRxStartCallback', 'c')])
    ## lte-spectrum-phy.h: void ns3::LteSpectrumPhy::SetGenericPhyTxEndCallback(ns3::GenericPhyTxEndCallback c) [member function]
    cls.add_method('SetGenericPhyTxEndCallback', 
                   'void', 
                   [param('ns3::GenericPhyTxEndCallback', 'c')])
    ## lte-spectrum-phy.h: void ns3::LteSpectrumPhy::SetMobility(ns3::Ptr<ns3::Object> m) [member function]
    cls.add_method('SetMobility', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'm')], 
                   is_virtual=True)
    ## lte-spectrum-phy.h: void ns3::LteSpectrumPhy::SetNoisePowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue const> noisePsd) [member function]
    cls.add_method('SetNoisePowerSpectralDensity', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'noisePsd')])
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
    ## lte-net-device.h: void ns3::LteNetDevice::SetGenericPhyTxStartCallback(ns3::GenericPhyTxStartCallback c) [member function]
    cls.add_method('SetGenericPhyTxStartCallback', 
                   'void', 
                   [param('ns3::GenericPhyTxStartCallback', 'c')])
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

