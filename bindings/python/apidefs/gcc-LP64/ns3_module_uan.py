from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## uan-mac-rc.h: ns3::Reservation [class]
    module.add_class('Reservation')
    ## uan-prop-model.h: ns3::Tap [class]
    module.add_class('Tap')
    ## uan-address.h: ns3::UanAddress [class]
    module.add_class('UanAddress')
    ## uan-address.h: ns3::UanAddress [class]
    root_module['ns3::UanAddress'].implicitly_converts_to(root_module['ns3::Address'])
    ## uan-helper.h: ns3::UanHelper [class]
    module.add_class('UanHelper')
    ## uan-tx-mode.h: ns3::UanModesList [class]
    module.add_class('UanModesList')
    ## uan-transducer.h: ns3::UanPacketArrival [class]
    module.add_class('UanPacketArrival')
    ## uan-prop-model.h: ns3::UanPdp [class]
    module.add_class('UanPdp')
    ## uan-phy.h: ns3::UanPhyListener [class]
    module.add_class('UanPhyListener', allow_subclassing=True)
    ## uan-tx-mode.h: ns3::UanTxMode [class]
    module.add_class('UanTxMode')
    ## uan-tx-mode.h: ns3::UanTxMode::ModulationType [enumeration]
    module.add_enum('ModulationType', ['PSK', 'QAM', 'FSK', 'OTHER'], outer_class=root_module['ns3::UanTxMode'])
    ## uan-tx-mode.h: ns3::UanTxModeFactory [class]
    module.add_class('UanTxModeFactory')
    ## acoustic-modem-energy-model-helper.h: ns3::AcousticModemEnergyModelHelper [class]
    module.add_class('AcousticModemEnergyModelHelper', parent=root_module['ns3::DeviceEnergyModelHelper'])
    ## uan-header-common.h: ns3::UanHeaderCommon [class]
    module.add_class('UanHeaderCommon', parent=root_module['ns3::Header'])
    ## uan-header-rc.h: ns3::UanHeaderRcAck [class]
    module.add_class('UanHeaderRcAck', parent=root_module['ns3::Header'])
    ## uan-header-rc.h: ns3::UanHeaderRcCts [class]
    module.add_class('UanHeaderRcCts', parent=root_module['ns3::Header'])
    ## uan-header-rc.h: ns3::UanHeaderRcCtsGlobal [class]
    module.add_class('UanHeaderRcCtsGlobal', parent=root_module['ns3::Header'])
    ## uan-header-rc.h: ns3::UanHeaderRcData [class]
    module.add_class('UanHeaderRcData', parent=root_module['ns3::Header'])
    ## uan-header-rc.h: ns3::UanHeaderRcRts [class]
    module.add_class('UanHeaderRcRts', parent=root_module['ns3::Header'])
    ## uan-mac.h: ns3::UanMac [class]
    module.add_class('UanMac', parent=root_module['ns3::Object'])
    ## uan-mac-aloha.h: ns3::UanMacAloha [class]
    module.add_class('UanMacAloha', parent=root_module['ns3::UanMac'])
    ## uan-mac-cw.h: ns3::UanMacCw [class]
    module.add_class('UanMacCw', parent=[root_module['ns3::UanMac'], root_module['ns3::UanPhyListener']])
    ## uan-mac-rc.h: ns3::UanMacRc [class]
    module.add_class('UanMacRc', parent=root_module['ns3::UanMac'])
    ## uan-mac-rc.h: ns3::UanMacRc [enumeration]
    module.add_enum('', ['TYPE_DATA', 'TYPE_GWPING', 'TYPE_RTS', 'TYPE_CTS', 'TYPE_ACK'], outer_class=root_module['ns3::UanMacRc'])
    ## uan-mac-rc-gw.h: ns3::UanMacRcGw [class]
    module.add_class('UanMacRcGw', parent=root_module['ns3::UanMac'])
    ## uan-noise-model.h: ns3::UanNoiseModel [class]
    module.add_class('UanNoiseModel', parent=root_module['ns3::Object'])
    ## uan-noise-model-default.h: ns3::UanNoiseModelDefault [class]
    module.add_class('UanNoiseModelDefault', parent=root_module['ns3::UanNoiseModel'])
    ## uan-phy.h: ns3::UanPhy [class]
    module.add_class('UanPhy', parent=root_module['ns3::Object'])
    ## uan-phy.h: ns3::UanPhy::State [enumeration]
    module.add_enum('State', ['IDLE', 'CCABUSY', 'RX', 'TX', 'SLEEP'], outer_class=root_module['ns3::UanPhy'])
    ## uan-phy.h: ns3::UanPhyCalcSinr [class]
    module.add_class('UanPhyCalcSinr', parent=root_module['ns3::Object'])
    ## uan-phy-gen.h: ns3::UanPhyCalcSinrDefault [class]
    module.add_class('UanPhyCalcSinrDefault', parent=root_module['ns3::UanPhyCalcSinr'])
    ## uan-phy-dual.h: ns3::UanPhyCalcSinrDual [class]
    module.add_class('UanPhyCalcSinrDual', parent=root_module['ns3::UanPhyCalcSinr'])
    ## uan-phy-gen.h: ns3::UanPhyCalcSinrFhFsk [class]
    module.add_class('UanPhyCalcSinrFhFsk', parent=root_module['ns3::UanPhyCalcSinr'])
    ## uan-phy-dual.h: ns3::UanPhyDual [class]
    module.add_class('UanPhyDual', parent=root_module['ns3::UanPhy'])
    ## uan-phy-gen.h: ns3::UanPhyGen [class]
    module.add_class('UanPhyGen', parent=root_module['ns3::UanPhy'])
    ## uan-phy.h: ns3::UanPhyPer [class]
    module.add_class('UanPhyPer', parent=root_module['ns3::Object'])
    ## uan-phy-gen.h: ns3::UanPhyPerGenDefault [class]
    module.add_class('UanPhyPerGenDefault', parent=root_module['ns3::UanPhyPer'])
    ## uan-phy-gen.h: ns3::UanPhyPerUmodem [class]
    module.add_class('UanPhyPerUmodem', parent=root_module['ns3::UanPhyPer'])
    ## uan-prop-model.h: ns3::UanPropModel [class]
    module.add_class('UanPropModel', parent=root_module['ns3::Object'])
    ## uan-prop-model-ideal.h: ns3::UanPropModelIdeal [class]
    module.add_class('UanPropModelIdeal', parent=root_module['ns3::UanPropModel'])
    ## uan-prop-model-thorp.h: ns3::UanPropModelThorp [class]
    module.add_class('UanPropModelThorp', parent=root_module['ns3::UanPropModel'])
    ## uan-transducer.h: ns3::UanTransducer [class]
    module.add_class('UanTransducer', parent=root_module['ns3::Object'])
    ## uan-transducer.h: ns3::UanTransducer::State [enumeration]
    module.add_enum('State', ['TX', 'RX'], outer_class=root_module['ns3::UanTransducer'])
    ## uan-transducer-hd.h: ns3::UanTransducerHd [class]
    module.add_class('UanTransducerHd', parent=root_module['ns3::UanTransducer'])
    ## uan-channel.h: ns3::UanChannel [class]
    module.add_class('UanChannel', parent=root_module['ns3::Channel'])
    ## uan-tx-mode.h: ns3::UanModesListChecker [class]
    module.add_class('UanModesListChecker', parent=root_module['ns3::AttributeChecker'])
    ## uan-tx-mode.h: ns3::UanModesListValue [class]
    module.add_class('UanModesListValue', parent=root_module['ns3::AttributeValue'])
    ## uan-net-device.h: ns3::UanNetDevice [class]
    module.add_class('UanNetDevice', parent=root_module['ns3::NetDevice'])
    ## acoustic-modem-energy-model.h: ns3::AcousticModemEnergyModel [class]
    module.add_class('AcousticModemEnergyModel', parent=root_module['ns3::DeviceEnergyModel'])
    module.add_container('std::vector< ns3::Tap >', 'ns3::Tap', container_type='vector')
    module.add_container('std::list< ns3::UanPacketArrival >', 'ns3::UanPacketArrival', container_type='list')
    
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
    register_Ns3Reservation_methods(root_module, root_module['ns3::Reservation'])
    register_Ns3Tap_methods(root_module, root_module['ns3::Tap'])
    register_Ns3UanAddress_methods(root_module, root_module['ns3::UanAddress'])
    register_Ns3UanHelper_methods(root_module, root_module['ns3::UanHelper'])
    register_Ns3UanModesList_methods(root_module, root_module['ns3::UanModesList'])
    register_Ns3UanPacketArrival_methods(root_module, root_module['ns3::UanPacketArrival'])
    register_Ns3UanPdp_methods(root_module, root_module['ns3::UanPdp'])
    register_Ns3UanPhyListener_methods(root_module, root_module['ns3::UanPhyListener'])
    register_Ns3UanTxMode_methods(root_module, root_module['ns3::UanTxMode'])
    register_Ns3UanTxModeFactory_methods(root_module, root_module['ns3::UanTxModeFactory'])
    register_Ns3AcousticModemEnergyModelHelper_methods(root_module, root_module['ns3::AcousticModemEnergyModelHelper'])
    register_Ns3UanHeaderCommon_methods(root_module, root_module['ns3::UanHeaderCommon'])
    register_Ns3UanHeaderRcAck_methods(root_module, root_module['ns3::UanHeaderRcAck'])
    register_Ns3UanHeaderRcCts_methods(root_module, root_module['ns3::UanHeaderRcCts'])
    register_Ns3UanHeaderRcCtsGlobal_methods(root_module, root_module['ns3::UanHeaderRcCtsGlobal'])
    register_Ns3UanHeaderRcData_methods(root_module, root_module['ns3::UanHeaderRcData'])
    register_Ns3UanHeaderRcRts_methods(root_module, root_module['ns3::UanHeaderRcRts'])
    register_Ns3UanMac_methods(root_module, root_module['ns3::UanMac'])
    register_Ns3UanMacAloha_methods(root_module, root_module['ns3::UanMacAloha'])
    register_Ns3UanMacCw_methods(root_module, root_module['ns3::UanMacCw'])
    register_Ns3UanMacRc_methods(root_module, root_module['ns3::UanMacRc'])
    register_Ns3UanMacRcGw_methods(root_module, root_module['ns3::UanMacRcGw'])
    register_Ns3UanNoiseModel_methods(root_module, root_module['ns3::UanNoiseModel'])
    register_Ns3UanNoiseModelDefault_methods(root_module, root_module['ns3::UanNoiseModelDefault'])
    register_Ns3UanPhy_methods(root_module, root_module['ns3::UanPhy'])
    register_Ns3UanPhyCalcSinr_methods(root_module, root_module['ns3::UanPhyCalcSinr'])
    register_Ns3UanPhyCalcSinrDefault_methods(root_module, root_module['ns3::UanPhyCalcSinrDefault'])
    register_Ns3UanPhyCalcSinrDual_methods(root_module, root_module['ns3::UanPhyCalcSinrDual'])
    register_Ns3UanPhyCalcSinrFhFsk_methods(root_module, root_module['ns3::UanPhyCalcSinrFhFsk'])
    register_Ns3UanPhyDual_methods(root_module, root_module['ns3::UanPhyDual'])
    register_Ns3UanPhyGen_methods(root_module, root_module['ns3::UanPhyGen'])
    register_Ns3UanPhyPer_methods(root_module, root_module['ns3::UanPhyPer'])
    register_Ns3UanPhyPerGenDefault_methods(root_module, root_module['ns3::UanPhyPerGenDefault'])
    register_Ns3UanPhyPerUmodem_methods(root_module, root_module['ns3::UanPhyPerUmodem'])
    register_Ns3UanPropModel_methods(root_module, root_module['ns3::UanPropModel'])
    register_Ns3UanPropModelIdeal_methods(root_module, root_module['ns3::UanPropModelIdeal'])
    register_Ns3UanPropModelThorp_methods(root_module, root_module['ns3::UanPropModelThorp'])
    register_Ns3UanTransducer_methods(root_module, root_module['ns3::UanTransducer'])
    register_Ns3UanTransducerHd_methods(root_module, root_module['ns3::UanTransducerHd'])
    register_Ns3UanChannel_methods(root_module, root_module['ns3::UanChannel'])
    register_Ns3UanModesListChecker_methods(root_module, root_module['ns3::UanModesListChecker'])
    register_Ns3UanModesListValue_methods(root_module, root_module['ns3::UanModesListValue'])
    register_Ns3UanNetDevice_methods(root_module, root_module['ns3::UanNetDevice'])
    register_Ns3AcousticModemEnergyModel_methods(root_module, root_module['ns3::AcousticModemEnergyModel'])
    return

def register_Ns3Reservation_methods(root_module, cls):
    ## uan-mac-rc.h: ns3::Reservation::Reservation(ns3::Reservation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Reservation const &', 'arg0')])
    ## uan-mac-rc.h: ns3::Reservation::Reservation() [constructor]
    cls.add_constructor([])
    ## uan-mac-rc.h: ns3::Reservation::Reservation(std::list<std::pair<ns3::Ptr<ns3::Packet>, ns3::UanAddress>, std::allocator<std::pair<ns3::Ptr<ns3::Packet>, ns3::UanAddress> > > & list, uint8_t frameNo, uint32_t maxPkts=0) [constructor]
    cls.add_constructor([param('std::list< std::pair< ns3::Ptr< ns3::Packet >, ns3::UanAddress > > &', 'list'), param('uint8_t', 'frameNo'), param('uint32_t', 'maxPkts', default_value='0')])
    ## uan-mac-rc.h: void ns3::Reservation::AddTimestamp(ns3::Time t) [member function]
    cls.add_method('AddTimestamp', 
                   'void', 
                   [param('ns3::Time', 't')])
    ## uan-mac-rc.h: uint8_t ns3::Reservation::GetFrameNo() const [member function]
    cls.add_method('GetFrameNo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-mac-rc.h: uint32_t ns3::Reservation::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## uan-mac-rc.h: uint32_t ns3::Reservation::GetNoFrames() const [member function]
    cls.add_method('GetNoFrames', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## uan-mac-rc.h: std::list<std::pair<ns3::Ptr<ns3::Packet>, ns3::UanAddress>, std::allocator<std::pair<ns3::Ptr<ns3::Packet>, ns3::UanAddress> > > const & ns3::Reservation::GetPktList() const [member function]
    cls.add_method('GetPktList', 
                   'std::list< std::pair< ns3::Ptr< ns3::Packet >, ns3::UanAddress > > const &', 
                   [], 
                   is_const=True)
    ## uan-mac-rc.h: uint8_t ns3::Reservation::GetRetryNo() const [member function]
    cls.add_method('GetRetryNo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-mac-rc.h: ns3::Time ns3::Reservation::GetTimestamp(uint8_t n) const [member function]
    cls.add_method('GetTimestamp', 
                   'ns3::Time', 
                   [param('uint8_t', 'n')], 
                   is_const=True)
    ## uan-mac-rc.h: void ns3::Reservation::IncrementRetry() [member function]
    cls.add_method('IncrementRetry', 
                   'void', 
                   [])
    ## uan-mac-rc.h: bool ns3::Reservation::IsTransmitted() const [member function]
    cls.add_method('IsTransmitted', 
                   'bool', 
                   [], 
                   is_const=True)
    ## uan-mac-rc.h: void ns3::Reservation::SetFrameNo(uint8_t fn) [member function]
    cls.add_method('SetFrameNo', 
                   'void', 
                   [param('uint8_t', 'fn')])
    ## uan-mac-rc.h: void ns3::Reservation::SetTransmitted(bool t=true) [member function]
    cls.add_method('SetTransmitted', 
                   'void', 
                   [param('bool', 't', default_value='true')])
    return

def register_Ns3Tap_methods(root_module, cls):
    ## uan-prop-model.h: ns3::Tap::Tap(ns3::Tap const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Tap const &', 'arg0')])
    ## uan-prop-model.h: ns3::Tap::Tap() [constructor]
    cls.add_constructor([])
    ## uan-prop-model.h: ns3::Tap::Tap(ns3::Time delay, std::complex<double> amp) [constructor]
    cls.add_constructor([param('ns3::Time', 'delay'), param('std::complex< double >', 'amp')])
    ## uan-prop-model.h: std::complex<double> ns3::Tap::GetAmp() const [member function]
    cls.add_method('GetAmp', 
                   'std::complex< double >', 
                   [], 
                   is_const=True)
    ## uan-prop-model.h: ns3::Time ns3::Tap::GetDelay() const [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    return

def register_Ns3UanAddress_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## uan-address.h: ns3::UanAddress::UanAddress(ns3::UanAddress const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanAddress const &', 'arg0')])
    ## uan-address.h: ns3::UanAddress::UanAddress() [constructor]
    cls.add_constructor([])
    ## uan-address.h: ns3::UanAddress::UanAddress(uint8_t addr) [constructor]
    cls.add_constructor([param('uint8_t', 'addr')])
    ## uan-address.h: static ns3::UanAddress ns3::UanAddress::Allocate() [member function]
    cls.add_method('Allocate', 
                   'ns3::UanAddress', 
                   [], 
                   is_static=True)
    ## uan-address.h: static ns3::UanAddress ns3::UanAddress::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::UanAddress', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## uan-address.h: void ns3::UanAddress::CopyFrom(uint8_t const * pBuffer) [member function]
    cls.add_method('CopyFrom', 
                   'void', 
                   [param('uint8_t const *', 'pBuffer')])
    ## uan-address.h: void ns3::UanAddress::CopyTo(uint8_t * pBuffer) [member function]
    cls.add_method('CopyTo', 
                   'void', 
                   [param('uint8_t *', 'pBuffer')])
    ## uan-address.h: uint8_t ns3::UanAddress::GetAsInt() const [member function]
    cls.add_method('GetAsInt', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-address.h: static ns3::UanAddress ns3::UanAddress::GetBroadcast() [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::UanAddress', 
                   [], 
                   is_static=True)
    ## uan-address.h: static bool ns3::UanAddress::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    return

def register_Ns3UanHelper_methods(root_module, cls):
    ## uan-helper.h: ns3::UanHelper::UanHelper(ns3::UanHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanHelper const &', 'arg0')])
    ## uan-helper.h: ns3::UanHelper::UanHelper() [constructor]
    cls.add_constructor([])
    ## uan-helper.h: static void ns3::UanHelper::EnableAscii(std::ostream & os, uint32_t nodeid, uint32_t deviceid) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid')], 
                   is_static=True)
    ## uan-helper.h: static void ns3::UanHelper::EnableAscii(std::ostream & os, ns3::NetDeviceContainer d) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('ns3::NetDeviceContainer', 'd')], 
                   is_static=True)
    ## uan-helper.h: static void ns3::UanHelper::EnableAscii(std::ostream & os, ns3::NodeContainer n) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('ns3::NodeContainer', 'n')], 
                   is_static=True)
    ## uan-helper.h: static void ns3::UanHelper::EnableAsciiAll(std::ostream & os) [member function]
    cls.add_method('EnableAsciiAll', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_static=True)
    ## uan-helper.h: ns3::NetDeviceContainer ns3::UanHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## uan-helper.h: ns3::NetDeviceContainer ns3::UanHelper::Install(ns3::NodeContainer c, ns3::Ptr<ns3::UanChannel> channel) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c'), param('ns3::Ptr< ns3::UanChannel >', 'channel')], 
                   is_const=True)
    ## uan-helper.h: ns3::Ptr<ns3::UanNetDevice> ns3::UanHelper::Install(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::UanChannel> channel) const [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::UanNetDevice >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::UanChannel >', 'channel')], 
                   is_const=True)
    ## uan-helper.h: void ns3::UanHelper::SetMac(std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetMac', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## uan-helper.h: void ns3::UanHelper::SetPhy(std::string phyType, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetPhy', 
                   'void', 
                   [param('std::string', 'phyType'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## uan-helper.h: void ns3::UanHelper::SetTransducer(std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetTransducer', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    return

def register_Ns3UanModesList_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## uan-tx-mode.h: ns3::UanModesList::UanModesList(ns3::UanModesList const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanModesList const &', 'arg0')])
    ## uan-tx-mode.h: ns3::UanModesList::UanModesList() [constructor]
    cls.add_constructor([])
    ## uan-tx-mode.h: void ns3::UanModesList::AppendMode(ns3::UanTxMode mode) [member function]
    cls.add_method('AppendMode', 
                   'void', 
                   [param('ns3::UanTxMode', 'mode')])
    ## uan-tx-mode.h: void ns3::UanModesList::DeleteMode(uint32_t num) [member function]
    cls.add_method('DeleteMode', 
                   'void', 
                   [param('uint32_t', 'num')])
    ## uan-tx-mode.h: uint32_t ns3::UanModesList::GetNModes() const [member function]
    cls.add_method('GetNModes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3UanPacketArrival_methods(root_module, cls):
    ## uan-transducer.h: ns3::UanPacketArrival::UanPacketArrival(ns3::UanPacketArrival const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPacketArrival const &', 'arg0')])
    ## uan-transducer.h: ns3::UanPacketArrival::UanPacketArrival() [constructor]
    cls.add_constructor([])
    ## uan-transducer.h: ns3::UanPacketArrival::UanPacketArrival(ns3::Ptr<ns3::Packet> packet, double rxPowerDb, ns3::UanTxMode txMode, ns3::UanPdp pdp, ns3::Time arrTime) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'rxPowerDb'), param('ns3::UanTxMode', 'txMode'), param('ns3::UanPdp', 'pdp'), param('ns3::Time', 'arrTime')])
    ## uan-transducer.h: ns3::Time ns3::UanPacketArrival::GetArrivalTime() const [member function]
    cls.add_method('GetArrivalTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## uan-transducer.h: ns3::Ptr<ns3::Packet> ns3::UanPacketArrival::GetPacket() const [member function]
    cls.add_method('GetPacket', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True)
    ## uan-transducer.h: ns3::UanPdp ns3::UanPacketArrival::GetPdp() const [member function]
    cls.add_method('GetPdp', 
                   'ns3::UanPdp', 
                   [], 
                   is_const=True)
    ## uan-transducer.h: double ns3::UanPacketArrival::GetRxPowerDb() const [member function]
    cls.add_method('GetRxPowerDb', 
                   'double', 
                   [], 
                   is_const=True)
    ## uan-transducer.h: ns3::UanTxMode const & ns3::UanPacketArrival::GetTxMode() const [member function]
    cls.add_method('GetTxMode', 
                   'ns3::UanTxMode const &', 
                   [], 
                   is_const=True)
    return

def register_Ns3UanPdp_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## uan-prop-model.h: ns3::UanPdp::UanPdp(ns3::UanPdp const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPdp const &', 'arg0')])
    ## uan-prop-model.h: ns3::UanPdp::UanPdp() [constructor]
    cls.add_constructor([])
    ## uan-prop-model.h: ns3::UanPdp::UanPdp(std::vector<ns3::Tap, std::allocator<ns3::Tap> > taps, ns3::Time resolution) [constructor]
    cls.add_constructor([param('std::vector< ns3::Tap >', 'taps'), param('ns3::Time', 'resolution')])
    ## uan-prop-model.h: ns3::UanPdp::UanPdp(std::vector<std::complex<double>,std::allocator<std::complex<double> > > arrivals, ns3::Time resolution) [constructor]
    cls.add_constructor([param('std::vector< std::complex< double > >', 'arrivals'), param('ns3::Time', 'resolution')])
    ## uan-prop-model.h: ns3::UanPdp::UanPdp(std::vector<double, std::allocator<double> > arrivals, ns3::Time resolution) [constructor]
    cls.add_constructor([param('std::vector< double >', 'arrivals'), param('ns3::Time', 'resolution')])
    ## uan-prop-model.h: static ns3::UanPdp ns3::UanPdp::CreateImpulsePdp() [member function]
    cls.add_method('CreateImpulsePdp', 
                   'ns3::UanPdp', 
                   [], 
                   is_static=True)
    ## uan-prop-model.h: __gnu_cxx::__normal_iterator<const ns3::Tap*,std::vector<ns3::Tap, std::allocator<ns3::Tap> > > ns3::UanPdp::GetBegin() const [member function]
    cls.add_method('GetBegin', 
                   '__gnu_cxx::__normal_iterator< ns3::Tap const *, std::vector< ns3::Tap > >', 
                   [], 
                   is_const=True)
    ## uan-prop-model.h: __gnu_cxx::__normal_iterator<const ns3::Tap*,std::vector<ns3::Tap, std::allocator<ns3::Tap> > > ns3::UanPdp::GetEnd() const [member function]
    cls.add_method('GetEnd', 
                   '__gnu_cxx::__normal_iterator< ns3::Tap const *, std::vector< ns3::Tap > >', 
                   [], 
                   is_const=True)
    ## uan-prop-model.h: uint32_t ns3::UanPdp::GetNTaps() const [member function]
    cls.add_method('GetNTaps', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## uan-prop-model.h: ns3::Time ns3::UanPdp::GetResolution() const [member function]
    cls.add_method('GetResolution', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## uan-prop-model.h: ns3::Tap const & ns3::UanPdp::GetTap(uint32_t i) const [member function]
    cls.add_method('GetTap', 
                   'ns3::Tap const &', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## uan-prop-model.h: void ns3::UanPdp::SetNTaps(uint32_t nTaps) [member function]
    cls.add_method('SetNTaps', 
                   'void', 
                   [param('uint32_t', 'nTaps')])
    ## uan-prop-model.h: void ns3::UanPdp::SetResolution(ns3::Time resolution) [member function]
    cls.add_method('SetResolution', 
                   'void', 
                   [param('ns3::Time', 'resolution')])
    ## uan-prop-model.h: void ns3::UanPdp::SetTap(std::complex<double> arrival, uint32_t index) [member function]
    cls.add_method('SetTap', 
                   'void', 
                   [param('std::complex< double >', 'arrival'), param('uint32_t', 'index')])
    ## uan-prop-model.h: std::complex<double> ns3::UanPdp::SumTapsC(ns3::Time begin, ns3::Time end) const [member function]
    cls.add_method('SumTapsC', 
                   'std::complex< double >', 
                   [param('ns3::Time', 'begin'), param('ns3::Time', 'end')], 
                   is_const=True)
    ## uan-prop-model.h: std::complex<double> ns3::UanPdp::SumTapsFromMaxC(ns3::Time delay, ns3::Time duration) const [member function]
    cls.add_method('SumTapsFromMaxC', 
                   'std::complex< double >', 
                   [param('ns3::Time', 'delay'), param('ns3::Time', 'duration')], 
                   is_const=True)
    ## uan-prop-model.h: double ns3::UanPdp::SumTapsFromMaxNc(ns3::Time delay, ns3::Time duration) const [member function]
    cls.add_method('SumTapsFromMaxNc', 
                   'double', 
                   [param('ns3::Time', 'delay'), param('ns3::Time', 'duration')], 
                   is_const=True)
    ## uan-prop-model.h: double ns3::UanPdp::SumTapsNc(ns3::Time begin, ns3::Time end) const [member function]
    cls.add_method('SumTapsNc', 
                   'double', 
                   [param('ns3::Time', 'begin'), param('ns3::Time', 'end')], 
                   is_const=True)
    return

def register_Ns3UanPhyListener_methods(root_module, cls):
    ## uan-phy.h: ns3::UanPhyListener::UanPhyListener() [constructor]
    cls.add_constructor([])
    ## uan-phy.h: ns3::UanPhyListener::UanPhyListener(ns3::UanPhyListener const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyListener const &', 'arg0')])
    ## uan-phy.h: void ns3::UanPhyListener::NotifyCcaEnd() [member function]
    cls.add_method('NotifyCcaEnd', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhyListener::NotifyCcaStart() [member function]
    cls.add_method('NotifyCcaStart', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhyListener::NotifyRxEndError() [member function]
    cls.add_method('NotifyRxEndError', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhyListener::NotifyRxEndOk() [member function]
    cls.add_method('NotifyRxEndOk', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhyListener::NotifyRxStart() [member function]
    cls.add_method('NotifyRxStart', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhyListener::NotifyTxStart(ns3::Time duration) [member function]
    cls.add_method('NotifyTxStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3UanTxMode_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## uan-tx-mode.h: ns3::UanTxMode::UanTxMode(ns3::UanTxMode const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanTxMode const &', 'arg0')])
    ## uan-tx-mode.h: ns3::UanTxMode::UanTxMode() [constructor]
    cls.add_constructor([])
    ## uan-tx-mode.h: uint32_t ns3::UanTxMode::GetBandwidthHz() const [member function]
    cls.add_method('GetBandwidthHz', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## uan-tx-mode.h: uint32_t ns3::UanTxMode::GetCenterFreqHz() const [member function]
    cls.add_method('GetCenterFreqHz', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## uan-tx-mode.h: uint32_t ns3::UanTxMode::GetConstellationSize() const [member function]
    cls.add_method('GetConstellationSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## uan-tx-mode.h: uint32_t ns3::UanTxMode::GetDataRateBps() const [member function]
    cls.add_method('GetDataRateBps', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## uan-tx-mode.h: ns3::UanTxMode::ModulationType ns3::UanTxMode::GetModType() const [member function]
    cls.add_method('GetModType', 
                   'ns3::UanTxMode::ModulationType', 
                   [], 
                   is_const=True)
    ## uan-tx-mode.h: std::string ns3::UanTxMode::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## uan-tx-mode.h: uint32_t ns3::UanTxMode::GetPhyRateSps() const [member function]
    cls.add_method('GetPhyRateSps', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## uan-tx-mode.h: uint32_t ns3::UanTxMode::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3UanTxModeFactory_methods(root_module, cls):
    ## uan-tx-mode.h: ns3::UanTxModeFactory::UanTxModeFactory(ns3::UanTxModeFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanTxModeFactory const &', 'arg0')])
    ## uan-tx-mode.h: ns3::UanTxModeFactory::UanTxModeFactory() [constructor]
    cls.add_constructor([])
    ## uan-tx-mode.h: static ns3::UanTxMode ns3::UanTxModeFactory::CreateMode(ns3::UanTxMode::ModulationType type, uint32_t dataRateBps, uint32_t phyRateSps, uint32_t cfHz, uint32_t bwHz, uint32_t constSize, std::string name) [member function]
    cls.add_method('CreateMode', 
                   'ns3::UanTxMode', 
                   [param('ns3::UanTxMode::ModulationType', 'type'), param('uint32_t', 'dataRateBps'), param('uint32_t', 'phyRateSps'), param('uint32_t', 'cfHz'), param('uint32_t', 'bwHz'), param('uint32_t', 'constSize'), param('std::string', 'name')], 
                   is_static=True)
    ## uan-tx-mode.h: static ns3::UanTxMode ns3::UanTxModeFactory::GetMode(std::string name) [member function]
    cls.add_method('GetMode', 
                   'ns3::UanTxMode', 
                   [param('std::string', 'name')], 
                   is_static=True)
    ## uan-tx-mode.h: static ns3::UanTxMode ns3::UanTxModeFactory::GetMode(uint32_t uid) [member function]
    cls.add_method('GetMode', 
                   'ns3::UanTxMode', 
                   [param('uint32_t', 'uid')], 
                   is_static=True)
    return

def register_Ns3AcousticModemEnergyModelHelper_methods(root_module, cls):
    ## acoustic-modem-energy-model-helper.h: ns3::AcousticModemEnergyModelHelper::AcousticModemEnergyModelHelper(ns3::AcousticModemEnergyModelHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AcousticModemEnergyModelHelper const &', 'arg0')])
    ## acoustic-modem-energy-model-helper.h: ns3::AcousticModemEnergyModelHelper::AcousticModemEnergyModelHelper() [constructor]
    cls.add_constructor([])
    ## acoustic-modem-energy-model-helper.h: void ns3::AcousticModemEnergyModelHelper::Set(std::string name, ns3::AttributeValue const & v) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'v')], 
                   is_virtual=True)
    ## acoustic-modem-energy-model-helper.h: void ns3::AcousticModemEnergyModelHelper::SetDepletionCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetDepletionCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## acoustic-modem-energy-model-helper.h: ns3::Ptr<ns3::DeviceEnergyModel> ns3::AcousticModemEnergyModelHelper::DoInstall(ns3::Ptr<ns3::NetDevice> device, ns3::Ptr<ns3::EnergySource> source) const [member function]
    cls.add_method('DoInstall', 
                   'ns3::Ptr< ns3::DeviceEnergyModel >', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device'), param('ns3::Ptr< ns3::EnergySource >', 'source')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3UanHeaderCommon_methods(root_module, cls):
    ## uan-header-common.h: ns3::UanHeaderCommon::UanHeaderCommon(ns3::UanHeaderCommon const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanHeaderCommon const &', 'arg0')])
    ## uan-header-common.h: ns3::UanHeaderCommon::UanHeaderCommon() [constructor]
    cls.add_constructor([])
    ## uan-header-common.h: ns3::UanHeaderCommon::UanHeaderCommon(ns3::UanAddress const src, ns3::UanAddress const dest, uint8_t type) [constructor]
    cls.add_constructor([param('ns3::UanAddress const', 'src'), param('ns3::UanAddress const', 'dest'), param('uint8_t', 'type')])
    ## uan-header-common.h: uint32_t ns3::UanHeaderCommon::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## uan-header-common.h: ns3::UanAddress ns3::UanHeaderCommon::GetDest() const [member function]
    cls.add_method('GetDest', 
                   'ns3::UanAddress', 
                   [], 
                   is_const=True)
    ## uan-header-common.h: ns3::TypeId ns3::UanHeaderCommon::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-common.h: uint32_t ns3::UanHeaderCommon::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-common.h: ns3::UanAddress ns3::UanHeaderCommon::GetSrc() const [member function]
    cls.add_method('GetSrc', 
                   'ns3::UanAddress', 
                   [], 
                   is_const=True)
    ## uan-header-common.h: uint8_t ns3::UanHeaderCommon::GetType() const [member function]
    cls.add_method('GetType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-header-common.h: static ns3::TypeId ns3::UanHeaderCommon::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-header-common.h: void ns3::UanHeaderCommon::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## uan-header-common.h: void ns3::UanHeaderCommon::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## uan-header-common.h: void ns3::UanHeaderCommon::SetDest(ns3::UanAddress dest) [member function]
    cls.add_method('SetDest', 
                   'void', 
                   [param('ns3::UanAddress', 'dest')])
    ## uan-header-common.h: void ns3::UanHeaderCommon::SetSrc(ns3::UanAddress src) [member function]
    cls.add_method('SetSrc', 
                   'void', 
                   [param('ns3::UanAddress', 'src')])
    ## uan-header-common.h: void ns3::UanHeaderCommon::SetType(uint8_t type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('uint8_t', 'type')])
    return

def register_Ns3UanHeaderRcAck_methods(root_module, cls):
    ## uan-header-rc.h: ns3::UanHeaderRcAck::UanHeaderRcAck(ns3::UanHeaderRcAck const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanHeaderRcAck const &', 'arg0')])
    ## uan-header-rc.h: ns3::UanHeaderRcAck::UanHeaderRcAck() [constructor]
    cls.add_constructor([])
    ## uan-header-rc.h: void ns3::UanHeaderRcAck::AddNackedFrame(uint8_t frame) [member function]
    cls.add_method('AddNackedFrame', 
                   'void', 
                   [param('uint8_t', 'frame')])
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcAck::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## uan-header-rc.h: uint8_t ns3::UanHeaderRcAck::GetFrameNo() const [member function]
    cls.add_method('GetFrameNo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: ns3::TypeId ns3::UanHeaderRcAck::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: std::set<unsigned char, std::less<unsigned char>, std::allocator<unsigned char> > const & ns3::UanHeaderRcAck::GetNackedFrames() const [member function]
    cls.add_method('GetNackedFrames', 
                   'std::set< unsigned char > const &', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint8_t ns3::UanHeaderRcAck::GetNoNacks() const [member function]
    cls.add_method('GetNoNacks', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcAck::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: static ns3::TypeId ns3::UanHeaderRcAck::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcAck::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcAck::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcAck::SetFrameNo(uint8_t frameNo) [member function]
    cls.add_method('SetFrameNo', 
                   'void', 
                   [param('uint8_t', 'frameNo')])
    return

def register_Ns3UanHeaderRcCts_methods(root_module, cls):
    ## uan-header-rc.h: ns3::UanHeaderRcCts::UanHeaderRcCts(ns3::UanHeaderRcCts const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanHeaderRcCts const &', 'arg0')])
    ## uan-header-rc.h: ns3::UanHeaderRcCts::UanHeaderRcCts() [constructor]
    cls.add_constructor([])
    ## uan-header-rc.h: ns3::UanHeaderRcCts::UanHeaderRcCts(uint8_t frameNo, uint8_t retryNo, ns3::Time rtsTs, ns3::Time delay, ns3::UanAddress addr) [constructor]
    cls.add_constructor([param('uint8_t', 'frameNo'), param('uint8_t', 'retryNo'), param('ns3::Time', 'rtsTs'), param('ns3::Time', 'delay'), param('ns3::UanAddress', 'addr')])
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcCts::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## uan-header-rc.h: ns3::UanAddress ns3::UanHeaderRcCts::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::UanAddress', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: ns3::Time ns3::UanHeaderRcCts::GetDelayToTx() const [member function]
    cls.add_method('GetDelayToTx', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint8_t ns3::UanHeaderRcCts::GetFrameNo() const [member function]
    cls.add_method('GetFrameNo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: ns3::TypeId ns3::UanHeaderRcCts::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: uint8_t ns3::UanHeaderRcCts::GetRetryNo() const [member function]
    cls.add_method('GetRetryNo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: ns3::Time ns3::UanHeaderRcCts::GetRtsTimeStamp() const [member function]
    cls.add_method('GetRtsTimeStamp', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcCts::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: static ns3::TypeId ns3::UanHeaderRcCts::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcCts::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcCts::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcCts::SetAddress(ns3::UanAddress addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::UanAddress', 'addr')])
    ## uan-header-rc.h: void ns3::UanHeaderRcCts::SetDelayToTx(ns3::Time delay) [member function]
    cls.add_method('SetDelayToTx', 
                   'void', 
                   [param('ns3::Time', 'delay')])
    ## uan-header-rc.h: void ns3::UanHeaderRcCts::SetFrameNo(uint8_t frameNo) [member function]
    cls.add_method('SetFrameNo', 
                   'void', 
                   [param('uint8_t', 'frameNo')])
    ## uan-header-rc.h: void ns3::UanHeaderRcCts::SetRetryNo(uint8_t no) [member function]
    cls.add_method('SetRetryNo', 
                   'void', 
                   [param('uint8_t', 'no')])
    ## uan-header-rc.h: void ns3::UanHeaderRcCts::SetRtsTimeStamp(ns3::Time timeStamp) [member function]
    cls.add_method('SetRtsTimeStamp', 
                   'void', 
                   [param('ns3::Time', 'timeStamp')])
    return

def register_Ns3UanHeaderRcCtsGlobal_methods(root_module, cls):
    ## uan-header-rc.h: ns3::UanHeaderRcCtsGlobal::UanHeaderRcCtsGlobal(ns3::UanHeaderRcCtsGlobal const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanHeaderRcCtsGlobal const &', 'arg0')])
    ## uan-header-rc.h: ns3::UanHeaderRcCtsGlobal::UanHeaderRcCtsGlobal() [constructor]
    cls.add_constructor([])
    ## uan-header-rc.h: ns3::UanHeaderRcCtsGlobal::UanHeaderRcCtsGlobal(ns3::Time wt, ns3::Time ts, uint16_t rate, uint16_t retryRate) [constructor]
    cls.add_constructor([param('ns3::Time', 'wt'), param('ns3::Time', 'ts'), param('uint16_t', 'rate'), param('uint16_t', 'retryRate')])
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcCtsGlobal::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## uan-header-rc.h: ns3::TypeId ns3::UanHeaderRcCtsGlobal::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: uint16_t ns3::UanHeaderRcCtsGlobal::GetRateNum() const [member function]
    cls.add_method('GetRateNum', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint16_t ns3::UanHeaderRcCtsGlobal::GetRetryRate() const [member function]
    cls.add_method('GetRetryRate', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcCtsGlobal::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: ns3::Time ns3::UanHeaderRcCtsGlobal::GetTxTimeStamp() const [member function]
    cls.add_method('GetTxTimeStamp', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: static ns3::TypeId ns3::UanHeaderRcCtsGlobal::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-header-rc.h: ns3::Time ns3::UanHeaderRcCtsGlobal::GetWindowTime() const [member function]
    cls.add_method('GetWindowTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcCtsGlobal::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcCtsGlobal::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcCtsGlobal::SetRateNum(uint16_t rate) [member function]
    cls.add_method('SetRateNum', 
                   'void', 
                   [param('uint16_t', 'rate')])
    ## uan-header-rc.h: void ns3::UanHeaderRcCtsGlobal::SetRetryRate(uint16_t rate) [member function]
    cls.add_method('SetRetryRate', 
                   'void', 
                   [param('uint16_t', 'rate')])
    ## uan-header-rc.h: void ns3::UanHeaderRcCtsGlobal::SetTxTimeStamp(ns3::Time timeStamp) [member function]
    cls.add_method('SetTxTimeStamp', 
                   'void', 
                   [param('ns3::Time', 'timeStamp')])
    ## uan-header-rc.h: void ns3::UanHeaderRcCtsGlobal::SetWindowTime(ns3::Time t) [member function]
    cls.add_method('SetWindowTime', 
                   'void', 
                   [param('ns3::Time', 't')])
    return

def register_Ns3UanHeaderRcData_methods(root_module, cls):
    ## uan-header-rc.h: ns3::UanHeaderRcData::UanHeaderRcData(ns3::UanHeaderRcData const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanHeaderRcData const &', 'arg0')])
    ## uan-header-rc.h: ns3::UanHeaderRcData::UanHeaderRcData() [constructor]
    cls.add_constructor([])
    ## uan-header-rc.h: ns3::UanHeaderRcData::UanHeaderRcData(uint8_t frameNum, ns3::Time propDelay) [constructor]
    cls.add_constructor([param('uint8_t', 'frameNum'), param('ns3::Time', 'propDelay')])
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcData::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## uan-header-rc.h: uint8_t ns3::UanHeaderRcData::GetFrameNo() const [member function]
    cls.add_method('GetFrameNo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: ns3::TypeId ns3::UanHeaderRcData::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: ns3::Time ns3::UanHeaderRcData::GetPropDelay() const [member function]
    cls.add_method('GetPropDelay', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcData::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: static ns3::TypeId ns3::UanHeaderRcData::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcData::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcData::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcData::SetFrameNo(uint8_t frameNum) [member function]
    cls.add_method('SetFrameNo', 
                   'void', 
                   [param('uint8_t', 'frameNum')])
    ## uan-header-rc.h: void ns3::UanHeaderRcData::SetPropDelay(ns3::Time propDelay) [member function]
    cls.add_method('SetPropDelay', 
                   'void', 
                   [param('ns3::Time', 'propDelay')])
    return

def register_Ns3UanHeaderRcRts_methods(root_module, cls):
    ## uan-header-rc.h: ns3::UanHeaderRcRts::UanHeaderRcRts(ns3::UanHeaderRcRts const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanHeaderRcRts const &', 'arg0')])
    ## uan-header-rc.h: ns3::UanHeaderRcRts::UanHeaderRcRts() [constructor]
    cls.add_constructor([])
    ## uan-header-rc.h: ns3::UanHeaderRcRts::UanHeaderRcRts(uint8_t frameNo, uint8_t retryNo, uint8_t noFrames, uint16_t length, ns3::Time ts) [constructor]
    cls.add_constructor([param('uint8_t', 'frameNo'), param('uint8_t', 'retryNo'), param('uint8_t', 'noFrames'), param('uint16_t', 'length'), param('ns3::Time', 'ts')])
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcRts::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## uan-header-rc.h: uint8_t ns3::UanHeaderRcRts::GetFrameNo() const [member function]
    cls.add_method('GetFrameNo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: ns3::TypeId ns3::UanHeaderRcRts::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: uint16_t ns3::UanHeaderRcRts::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint8_t ns3::UanHeaderRcRts::GetNoFrames() const [member function]
    cls.add_method('GetNoFrames', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint8_t ns3::UanHeaderRcRts::GetRetryNo() const [member function]
    cls.add_method('GetRetryNo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: uint32_t ns3::UanHeaderRcRts::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: ns3::Time ns3::UanHeaderRcRts::GetTimeStamp() const [member function]
    cls.add_method('GetTimeStamp', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## uan-header-rc.h: static ns3::TypeId ns3::UanHeaderRcRts::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcRts::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcRts::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## uan-header-rc.h: void ns3::UanHeaderRcRts::SetFrameNo(uint8_t fno) [member function]
    cls.add_method('SetFrameNo', 
                   'void', 
                   [param('uint8_t', 'fno')])
    ## uan-header-rc.h: void ns3::UanHeaderRcRts::SetLength(uint16_t length) [member function]
    cls.add_method('SetLength', 
                   'void', 
                   [param('uint16_t', 'length')])
    ## uan-header-rc.h: void ns3::UanHeaderRcRts::SetNoFrames(uint8_t no) [member function]
    cls.add_method('SetNoFrames', 
                   'void', 
                   [param('uint8_t', 'no')])
    ## uan-header-rc.h: void ns3::UanHeaderRcRts::SetRetryNo(uint8_t no) [member function]
    cls.add_method('SetRetryNo', 
                   'void', 
                   [param('uint8_t', 'no')])
    ## uan-header-rc.h: void ns3::UanHeaderRcRts::SetTimeStamp(ns3::Time timeStamp) [member function]
    cls.add_method('SetTimeStamp', 
                   'void', 
                   [param('ns3::Time', 'timeStamp')])
    return

def register_Ns3UanMac_methods(root_module, cls):
    ## uan-mac.h: ns3::UanMac::UanMac() [constructor]
    cls.add_constructor([])
    ## uan-mac.h: ns3::UanMac::UanMac(ns3::UanMac const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanMac const &', 'arg0')])
    ## uan-mac.h: void ns3::UanMac::AttachPhy(ns3::Ptr<ns3::UanPhy> phy) [member function]
    cls.add_method('AttachPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'phy')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-mac.h: void ns3::UanMac::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-mac.h: bool ns3::UanMac::Enqueue(ns3::Ptr<ns3::Packet> pkt, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-mac.h: ns3::Address ns3::UanMac::GetAddress() [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-mac.h: ns3::Address ns3::UanMac::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-mac.h: void ns3::UanMac::SetAddress(ns3::UanAddress addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::UanAddress', 'addr')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-mac.h: void ns3::UanMac::SetForwardUpCb(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::UanAddress const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetForwardUpCb', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::UanAddress const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3UanMacAloha_methods(root_module, cls):
    ## uan-mac-aloha.h: ns3::UanMacAloha::UanMacAloha(ns3::UanMacAloha const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanMacAloha const &', 'arg0')])
    ## uan-mac-aloha.h: ns3::UanMacAloha::UanMacAloha() [constructor]
    cls.add_constructor([])
    ## uan-mac-aloha.h: void ns3::UanMacAloha::AttachPhy(ns3::Ptr<ns3::UanPhy> phy) [member function]
    cls.add_method('AttachPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'phy')], 
                   is_virtual=True)
    ## uan-mac-aloha.h: void ns3::UanMacAloha::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-mac-aloha.h: bool ns3::UanMacAloha::Enqueue(ns3::Ptr<ns3::Packet> pkt, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## uan-mac-aloha.h: ns3::Address ns3::UanMacAloha::GetAddress() [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_virtual=True)
    ## uan-mac-aloha.h: ns3::Address ns3::UanMacAloha::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-mac-aloha.h: static ns3::TypeId ns3::UanMacAloha::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-mac-aloha.h: void ns3::UanMacAloha::SetAddress(ns3::UanAddress addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::UanAddress', 'addr')], 
                   is_virtual=True)
    ## uan-mac-aloha.h: void ns3::UanMacAloha::SetForwardUpCb(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::UanAddress const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetForwardUpCb', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::UanAddress const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-mac-aloha.h: void ns3::UanMacAloha::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3UanMacCw_methods(root_module, cls):
    ## uan-mac-cw.h: ns3::UanMacCw::UanMacCw(ns3::UanMacCw const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanMacCw const &', 'arg0')])
    ## uan-mac-cw.h: ns3::UanMacCw::UanMacCw() [constructor]
    cls.add_constructor([])
    ## uan-mac-cw.h: void ns3::UanMacCw::AttachPhy(ns3::Ptr<ns3::UanPhy> phy) [member function]
    cls.add_method('AttachPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'phy')], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-mac-cw.h: bool ns3::UanMacCw::Enqueue(ns3::Ptr<ns3::Packet> pkt, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## uan-mac-cw.h: ns3::Address ns3::UanMacCw::GetAddress() [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_virtual=True)
    ## uan-mac-cw.h: ns3::Address ns3::UanMacCw::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-mac-cw.h: uint32_t ns3::UanMacCw::GetCw() [member function]
    cls.add_method('GetCw', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    ## uan-mac-cw.h: ns3::Time ns3::UanMacCw::GetSlotTime() [member function]
    cls.add_method('GetSlotTime', 
                   'ns3::Time', 
                   [], 
                   is_virtual=True)
    ## uan-mac-cw.h: static ns3::TypeId ns3::UanMacCw::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::NotifyCcaEnd() [member function]
    cls.add_method('NotifyCcaEnd', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::NotifyCcaStart() [member function]
    cls.add_method('NotifyCcaStart', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::NotifyRxEndError() [member function]
    cls.add_method('NotifyRxEndError', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::NotifyRxEndOk() [member function]
    cls.add_method('NotifyRxEndOk', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::NotifyRxStart() [member function]
    cls.add_method('NotifyRxStart', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::NotifyTxStart(ns3::Time duration) [member function]
    cls.add_method('NotifyTxStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::SetAddress(ns3::UanAddress addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::UanAddress', 'addr')], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::SetCw(uint32_t cw) [member function]
    cls.add_method('SetCw', 
                   'void', 
                   [param('uint32_t', 'cw')], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::SetForwardUpCb(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::UanAddress const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetForwardUpCb', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::UanAddress const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::SetSlotTime(ns3::Time duration) [member function]
    cls.add_method('SetSlotTime', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_virtual=True)
    ## uan-mac-cw.h: void ns3::UanMacCw::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3UanMacRc_methods(root_module, cls):
    ## uan-mac-rc.h: ns3::UanMacRc::UanMacRc(ns3::UanMacRc const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanMacRc const &', 'arg0')])
    ## uan-mac-rc.h: ns3::UanMacRc::UanMacRc() [constructor]
    cls.add_constructor([])
    ## uan-mac-rc.h: void ns3::UanMacRc::AttachPhy(ns3::Ptr<ns3::UanPhy> phy) [member function]
    cls.add_method('AttachPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'phy')], 
                   is_virtual=True)
    ## uan-mac-rc.h: void ns3::UanMacRc::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-mac-rc.h: bool ns3::UanMacRc::Enqueue(ns3::Ptr<ns3::Packet> pkt, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## uan-mac-rc.h: ns3::Address ns3::UanMacRc::GetAddress() [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_virtual=True)
    ## uan-mac-rc.h: ns3::Address ns3::UanMacRc::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-mac-rc.h: static ns3::TypeId ns3::UanMacRc::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-mac-rc.h: void ns3::UanMacRc::SetAddress(ns3::UanAddress addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::UanAddress', 'addr')], 
                   is_virtual=True)
    ## uan-mac-rc.h: void ns3::UanMacRc::SetForwardUpCb(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::UanAddress const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetForwardUpCb', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::UanAddress const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-mac-rc.h: void ns3::UanMacRc::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3UanMacRcGw_methods(root_module, cls):
    ## uan-mac-rc-gw.h: ns3::UanMacRcGw::UanMacRcGw(ns3::UanMacRcGw const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanMacRcGw const &', 'arg0')])
    ## uan-mac-rc-gw.h: ns3::UanMacRcGw::UanMacRcGw() [constructor]
    cls.add_constructor([])
    ## uan-mac-rc-gw.h: void ns3::UanMacRcGw::AttachPhy(ns3::Ptr<ns3::UanPhy> phy) [member function]
    cls.add_method('AttachPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'phy')], 
                   is_virtual=True)
    ## uan-mac-rc-gw.h: void ns3::UanMacRcGw::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-mac-rc-gw.h: bool ns3::UanMacRcGw::Enqueue(ns3::Ptr<ns3::Packet> pkt, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## uan-mac-rc-gw.h: ns3::Address ns3::UanMacRcGw::GetAddress() [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_virtual=True)
    ## uan-mac-rc-gw.h: ns3::Address ns3::UanMacRcGw::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-mac-rc-gw.h: static ns3::TypeId ns3::UanMacRcGw::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-mac-rc-gw.h: void ns3::UanMacRcGw::SetAddress(ns3::UanAddress addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::UanAddress', 'addr')], 
                   is_virtual=True)
    ## uan-mac-rc-gw.h: void ns3::UanMacRcGw::SetForwardUpCb(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::UanAddress const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetForwardUpCb', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::UanAddress const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-mac-rc-gw.h: void ns3::UanMacRcGw::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3UanNoiseModel_methods(root_module, cls):
    ## uan-noise-model.h: ns3::UanNoiseModel::UanNoiseModel() [constructor]
    cls.add_constructor([])
    ## uan-noise-model.h: ns3::UanNoiseModel::UanNoiseModel(ns3::UanNoiseModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanNoiseModel const &', 'arg0')])
    ## uan-noise-model.h: void ns3::UanNoiseModel::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-noise-model.h: void ns3::UanNoiseModel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-noise-model.h: double ns3::UanNoiseModel::GetNoiseDbHz(double fKhz) const [member function]
    cls.add_method('GetNoiseDbHz', 
                   'double', 
                   [param('double', 'fKhz')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3UanNoiseModelDefault_methods(root_module, cls):
    ## uan-noise-model-default.h: ns3::UanNoiseModelDefault::UanNoiseModelDefault(ns3::UanNoiseModelDefault const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanNoiseModelDefault const &', 'arg0')])
    ## uan-noise-model-default.h: ns3::UanNoiseModelDefault::UanNoiseModelDefault() [constructor]
    cls.add_constructor([])
    ## uan-noise-model-default.h: double ns3::UanNoiseModelDefault::GetNoiseDbHz(double fKhz) const [member function]
    cls.add_method('GetNoiseDbHz', 
                   'double', 
                   [param('double', 'fKhz')], 
                   is_const=True, is_virtual=True)
    ## uan-noise-model-default.h: static ns3::TypeId ns3::UanNoiseModelDefault::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UanPhy_methods(root_module, cls):
    ## uan-phy.h: ns3::UanPhy::UanPhy() [constructor]
    cls.add_constructor([])
    ## uan-phy.h: ns3::UanPhy::UanPhy(ns3::UanPhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhy const &', 'arg0')])
    ## uan-phy.h: void ns3::UanPhy::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::EnergyDepletionHandler() [member function]
    cls.add_method('EnergyDepletionHandler', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: double ns3::UanPhy::GetCcaThresholdDb() [member function]
    cls.add_method('GetCcaThresholdDb', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: ns3::Ptr<ns3::UanChannel> ns3::UanPhy::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::UanChannel >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-phy.h: ns3::Ptr<ns3::UanNetDevice> ns3::UanPhy::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::UanNetDevice >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: ns3::UanTxMode ns3::UanPhy::GetMode(uint32_t n) [member function]
    cls.add_method('GetMode', 
                   'ns3::UanTxMode', 
                   [param('uint32_t', 'n')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: uint32_t ns3::UanPhy::GetNModes() [member function]
    cls.add_method('GetNModes', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: ns3::Ptr<ns3::Packet> ns3::UanPhy::GetPacketRx() const [member function]
    cls.add_method('GetPacketRx', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-phy.h: double ns3::UanPhy::GetRxGainDb() [member function]
    cls.add_method('GetRxGainDb', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: double ns3::UanPhy::GetRxThresholdDb() [member function]
    cls.add_method('GetRxThresholdDb', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: ns3::Ptr<ns3::UanTransducer> ns3::UanPhy::GetTransducer() [member function]
    cls.add_method('GetTransducer', 
                   'ns3::Ptr< ns3::UanTransducer >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: double ns3::UanPhy::GetTxPowerDb() [member function]
    cls.add_method('GetTxPowerDb', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: bool ns3::UanPhy::IsStateBusy() [member function]
    cls.add_method('IsStateBusy', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: bool ns3::UanPhy::IsStateCcaBusy() [member function]
    cls.add_method('IsStateCcaBusy', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: bool ns3::UanPhy::IsStateIdle() [member function]
    cls.add_method('IsStateIdle', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: bool ns3::UanPhy::IsStateRx() [member function]
    cls.add_method('IsStateRx', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: bool ns3::UanPhy::IsStateSleep() [member function]
    cls.add_method('IsStateSleep', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: bool ns3::UanPhy::IsStateTx() [member function]
    cls.add_method('IsStateTx', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::NotifyIntChange() [member function]
    cls.add_method('NotifyIntChange', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::NotifyTransStartTx(ns3::Ptr<ns3::Packet> packet, double txPowerDb, ns3::UanTxMode txMode) [member function]
    cls.add_method('NotifyTransStartTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'txPowerDb'), param('ns3::UanTxMode', 'txMode')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::RegisterListener(ns3::UanPhyListener * listener) [member function]
    cls.add_method('RegisterListener', 
                   'void', 
                   [param('ns3::UanPhyListener *', 'listener')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SendPacket(ns3::Ptr<ns3::Packet> pkt, uint32_t modeNum) [member function]
    cls.add_method('SendPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('uint32_t', 'modeNum')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetCcaThresholdDb(double thresh) [member function]
    cls.add_method('SetCcaThresholdDb', 
                   'void', 
                   [param('double', 'thresh')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetChannel(ns3::Ptr<ns3::UanChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanChannel >', 'channel')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetDevice(ns3::Ptr<ns3::UanNetDevice> device) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanNetDevice >', 'device')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetEnergyModelCallback(ns3::Callback<void, int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetEnergyModelCallback', 
                   'void', 
                   [param('ns3::Callback< void, int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetMac(ns3::Ptr<ns3::UanMac> mac) [member function]
    cls.add_method('SetMac', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanMac >', 'mac')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetReceiveErrorCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, double, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveErrorCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetReceiveOkCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, double, ns3::UanTxMode, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveOkCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, ns3::UanTxMode, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetRxGainDb(double gain) [member function]
    cls.add_method('SetRxGainDb', 
                   'void', 
                   [param('double', 'gain')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetRxThresholdDb(double thresh) [member function]
    cls.add_method('SetRxThresholdDb', 
                   'void', 
                   [param('double', 'thresh')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetTransducer(ns3::Ptr<ns3::UanTransducer> trans) [member function]
    cls.add_method('SetTransducer', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanTransducer >', 'trans')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::SetTxPowerDb(double txpwr) [member function]
    cls.add_method('SetTxPowerDb', 
                   'void', 
                   [param('double', 'txpwr')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhy::StartRxPacket(ns3::Ptr<ns3::Packet> pkt, double rxPowerDb, ns3::UanTxMode txMode, ns3::UanPdp pdp) [member function]
    cls.add_method('StartRxPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('double', 'rxPowerDb'), param('ns3::UanTxMode', 'txMode'), param('ns3::UanPdp', 'pdp')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3UanPhyCalcSinr_methods(root_module, cls):
    ## uan-phy.h: ns3::UanPhyCalcSinr::UanPhyCalcSinr() [constructor]
    cls.add_constructor([])
    ## uan-phy.h: ns3::UanPhyCalcSinr::UanPhyCalcSinr(ns3::UanPhyCalcSinr const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyCalcSinr const &', 'arg0')])
    ## uan-phy.h: double ns3::UanPhyCalcSinr::CalcSinrDb(ns3::Ptr<ns3::Packet> pkt, ns3::Time arrTime, double rxPowerDb, double ambNoiseDb, ns3::UanTxMode mode, ns3::UanPdp pdp, std::list<ns3::UanPacketArrival, std::allocator<ns3::UanPacketArrival> > const & arrivalList) const [member function]
    cls.add_method('CalcSinrDb', 
                   'double', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::Time', 'arrTime'), param('double', 'rxPowerDb'), param('double', 'ambNoiseDb'), param('ns3::UanTxMode', 'mode'), param('ns3::UanPdp', 'pdp'), param('std::list< ns3::UanPacketArrival > const &', 'arrivalList')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhyCalcSinr::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-phy.h: double ns3::UanPhyCalcSinr::DbToKp(double db) const [member function]
    cls.add_method('DbToKp', 
                   'double', 
                   [param('double', 'db')], 
                   is_const=True)
    ## uan-phy.h: void ns3::UanPhyCalcSinr::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-phy.h: double ns3::UanPhyCalcSinr::KpToDb(double kp) const [member function]
    cls.add_method('KpToDb', 
                   'double', 
                   [param('double', 'kp')], 
                   is_const=True)
    return

def register_Ns3UanPhyCalcSinrDefault_methods(root_module, cls):
    ## uan-phy-gen.h: ns3::UanPhyCalcSinrDefault::UanPhyCalcSinrDefault(ns3::UanPhyCalcSinrDefault const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyCalcSinrDefault const &', 'arg0')])
    ## uan-phy-gen.h: ns3::UanPhyCalcSinrDefault::UanPhyCalcSinrDefault() [constructor]
    cls.add_constructor([])
    ## uan-phy-gen.h: double ns3::UanPhyCalcSinrDefault::CalcSinrDb(ns3::Ptr<ns3::Packet> pkt, ns3::Time arrTime, double rxPowerDb, double ambNoiseDb, ns3::UanTxMode mode, ns3::UanPdp pdp, std::list<ns3::UanPacketArrival, std::allocator<ns3::UanPacketArrival> > const & arrivalList) const [member function]
    cls.add_method('CalcSinrDb', 
                   'double', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::Time', 'arrTime'), param('double', 'rxPowerDb'), param('double', 'ambNoiseDb'), param('ns3::UanTxMode', 'mode'), param('ns3::UanPdp', 'pdp'), param('std::list< ns3::UanPacketArrival > const &', 'arrivalList')], 
                   is_const=True, is_virtual=True)
    ## uan-phy-gen.h: static ns3::TypeId ns3::UanPhyCalcSinrDefault::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UanPhyCalcSinrDual_methods(root_module, cls):
    ## uan-phy-dual.h: ns3::UanPhyCalcSinrDual::UanPhyCalcSinrDual(ns3::UanPhyCalcSinrDual const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyCalcSinrDual const &', 'arg0')])
    ## uan-phy-dual.h: ns3::UanPhyCalcSinrDual::UanPhyCalcSinrDual() [constructor]
    cls.add_constructor([])
    ## uan-phy-dual.h: double ns3::UanPhyCalcSinrDual::CalcSinrDb(ns3::Ptr<ns3::Packet> pkt, ns3::Time arrTime, double rxPowerDb, double ambNoiseDb, ns3::UanTxMode mode, ns3::UanPdp pdp, std::list<ns3::UanPacketArrival, std::allocator<ns3::UanPacketArrival> > const & arrivalList) const [member function]
    cls.add_method('CalcSinrDb', 
                   'double', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::Time', 'arrTime'), param('double', 'rxPowerDb'), param('double', 'ambNoiseDb'), param('ns3::UanTxMode', 'mode'), param('ns3::UanPdp', 'pdp'), param('std::list< ns3::UanPacketArrival > const &', 'arrivalList')], 
                   is_const=True, is_virtual=True)
    ## uan-phy-dual.h: static ns3::TypeId ns3::UanPhyCalcSinrDual::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UanPhyCalcSinrFhFsk_methods(root_module, cls):
    ## uan-phy-gen.h: ns3::UanPhyCalcSinrFhFsk::UanPhyCalcSinrFhFsk(ns3::UanPhyCalcSinrFhFsk const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyCalcSinrFhFsk const &', 'arg0')])
    ## uan-phy-gen.h: ns3::UanPhyCalcSinrFhFsk::UanPhyCalcSinrFhFsk() [constructor]
    cls.add_constructor([])
    ## uan-phy-gen.h: double ns3::UanPhyCalcSinrFhFsk::CalcSinrDb(ns3::Ptr<ns3::Packet> pkt, ns3::Time arrTime, double rxPowerDb, double ambNoiseDb, ns3::UanTxMode mode, ns3::UanPdp pdp, std::list<ns3::UanPacketArrival, std::allocator<ns3::UanPacketArrival> > const & arrivalList) const [member function]
    cls.add_method('CalcSinrDb', 
                   'double', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::Time', 'arrTime'), param('double', 'rxPowerDb'), param('double', 'ambNoiseDb'), param('ns3::UanTxMode', 'mode'), param('ns3::UanPdp', 'pdp'), param('std::list< ns3::UanPacketArrival > const &', 'arrivalList')], 
                   is_const=True, is_virtual=True)
    ## uan-phy-gen.h: static ns3::TypeId ns3::UanPhyCalcSinrFhFsk::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UanPhyDual_methods(root_module, cls):
    ## uan-phy-dual.h: ns3::UanPhyDual::UanPhyDual(ns3::UanPhyDual const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyDual const &', 'arg0')])
    ## uan-phy-dual.h: ns3::UanPhyDual::UanPhyDual() [constructor]
    cls.add_constructor([])
    ## uan-phy-dual.h: void ns3::UanPhyDual::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::EnergyDepletionHandler() [member function]
    cls.add_method('EnergyDepletionHandler', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetCcaThresholdDb() [member function]
    cls.add_method('GetCcaThresholdDb', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetCcaThresholdPhy1() const [member function]
    cls.add_method('GetCcaThresholdPhy1', 
                   'double', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetCcaThresholdPhy2() const [member function]
    cls.add_method('GetCcaThresholdPhy2', 
                   'double', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::UanChannel> ns3::UanPhyDual::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::UanChannel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::UanNetDevice> ns3::UanPhyDual::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::UanNetDevice >', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: ns3::UanTxMode ns3::UanPhyDual::GetMode(uint32_t n) [member function]
    cls.add_method('GetMode', 
                   'ns3::UanTxMode', 
                   [param('uint32_t', 'n')], 
                   is_virtual=True)
    ## uan-phy-dual.h: ns3::UanModesList ns3::UanPhyDual::GetModesPhy1() const [member function]
    cls.add_method('GetModesPhy1', 
                   'ns3::UanModesList', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: ns3::UanModesList ns3::UanPhyDual::GetModesPhy2() const [member function]
    cls.add_method('GetModesPhy2', 
                   'ns3::UanModesList', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: uint32_t ns3::UanPhyDual::GetNModes() [member function]
    cls.add_method('GetNModes', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::Packet> ns3::UanPhyDual::GetPacketRx() const [member function]
    cls.add_method('GetPacketRx', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::UanPhyPer> ns3::UanPhyDual::GetPerModelPhy1() const [member function]
    cls.add_method('GetPerModelPhy1', 
                   'ns3::Ptr< ns3::UanPhyPer >', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::UanPhyPer> ns3::UanPhyDual::GetPerModelPhy2() const [member function]
    cls.add_method('GetPerModelPhy2', 
                   'ns3::Ptr< ns3::UanPhyPer >', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::Packet> ns3::UanPhyDual::GetPhy1PacketRx() const [member function]
    cls.add_method('GetPhy1PacketRx', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::Packet> ns3::UanPhyDual::GetPhy2PacketRx() const [member function]
    cls.add_method('GetPhy2PacketRx', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetRxGainDb() [member function]
    cls.add_method('GetRxGainDb', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetRxGainDbPhy1() const [member function]
    cls.add_method('GetRxGainDbPhy1', 
                   'double', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetRxGainDbPhy2() const [member function]
    cls.add_method('GetRxGainDbPhy2', 
                   'double', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetRxThresholdDb() [member function]
    cls.add_method('GetRxThresholdDb', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::UanPhyCalcSinr> ns3::UanPhyDual::GetSinrModelPhy1() const [member function]
    cls.add_method('GetSinrModelPhy1', 
                   'ns3::Ptr< ns3::UanPhyCalcSinr >', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::UanPhyCalcSinr> ns3::UanPhyDual::GetSinrModelPhy2() const [member function]
    cls.add_method('GetSinrModelPhy2', 
                   'ns3::Ptr< ns3::UanPhyCalcSinr >', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: ns3::Ptr<ns3::UanTransducer> ns3::UanPhyDual::GetTransducer() [member function]
    cls.add_method('GetTransducer', 
                   'ns3::Ptr< ns3::UanTransducer >', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetTxPowerDb() [member function]
    cls.add_method('GetTxPowerDb', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetTxPowerDbPhy1() const [member function]
    cls.add_method('GetTxPowerDbPhy1', 
                   'double', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: double ns3::UanPhyDual::GetTxPowerDbPhy2() const [member function]
    cls.add_method('GetTxPowerDbPhy2', 
                   'double', 
                   [], 
                   is_const=True)
    ## uan-phy-dual.h: static ns3::TypeId ns3::UanPhyDual::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsPhy1Idle() [member function]
    cls.add_method('IsPhy1Idle', 
                   'bool', 
                   [])
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsPhy1Rx() [member function]
    cls.add_method('IsPhy1Rx', 
                   'bool', 
                   [])
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsPhy1Tx() [member function]
    cls.add_method('IsPhy1Tx', 
                   'bool', 
                   [])
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsPhy2Idle() [member function]
    cls.add_method('IsPhy2Idle', 
                   'bool', 
                   [])
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsPhy2Rx() [member function]
    cls.add_method('IsPhy2Rx', 
                   'bool', 
                   [])
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsPhy2Tx() [member function]
    cls.add_method('IsPhy2Tx', 
                   'bool', 
                   [])
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsStateBusy() [member function]
    cls.add_method('IsStateBusy', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsStateCcaBusy() [member function]
    cls.add_method('IsStateCcaBusy', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsStateIdle() [member function]
    cls.add_method('IsStateIdle', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsStateRx() [member function]
    cls.add_method('IsStateRx', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsStateSleep() [member function]
    cls.add_method('IsStateSleep', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: bool ns3::UanPhyDual::IsStateTx() [member function]
    cls.add_method('IsStateTx', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::NotifyIntChange() [member function]
    cls.add_method('NotifyIntChange', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::NotifyTransStartTx(ns3::Ptr<ns3::Packet> packet, double txPowerDb, ns3::UanTxMode txMode) [member function]
    cls.add_method('NotifyTransStartTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'txPowerDb'), param('ns3::UanTxMode', 'txMode')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::RegisterListener(ns3::UanPhyListener * listener) [member function]
    cls.add_method('RegisterListener', 
                   'void', 
                   [param('ns3::UanPhyListener *', 'listener')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SendPacket(ns3::Ptr<ns3::Packet> pkt, uint32_t modeNum) [member function]
    cls.add_method('SendPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('uint32_t', 'modeNum')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetCcaThresholdDb(double thresh) [member function]
    cls.add_method('SetCcaThresholdDb', 
                   'void', 
                   [param('double', 'thresh')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetCcaThresholdPhy1(double thresh) [member function]
    cls.add_method('SetCcaThresholdPhy1', 
                   'void', 
                   [param('double', 'thresh')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetCcaThresholdPhy2(double thresh) [member function]
    cls.add_method('SetCcaThresholdPhy2', 
                   'void', 
                   [param('double', 'thresh')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetChannel(ns3::Ptr<ns3::UanChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanChannel >', 'channel')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetDevice(ns3::Ptr<ns3::UanNetDevice> device) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanNetDevice >', 'device')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetEnergyModelCallback(ns3::Callback<void, int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetEnergyModelCallback', 
                   'void', 
                   [param('ns3::Callback< void, int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetMac(ns3::Ptr<ns3::UanMac> mac) [member function]
    cls.add_method('SetMac', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanMac >', 'mac')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetModesPhy1(ns3::UanModesList modes) [member function]
    cls.add_method('SetModesPhy1', 
                   'void', 
                   [param('ns3::UanModesList', 'modes')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetModesPhy2(ns3::UanModesList modes) [member function]
    cls.add_method('SetModesPhy2', 
                   'void', 
                   [param('ns3::UanModesList', 'modes')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetPerModelPhy1(ns3::Ptr<ns3::UanPhyPer> per) [member function]
    cls.add_method('SetPerModelPhy1', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhyPer >', 'per')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetPerModelPhy2(ns3::Ptr<ns3::UanPhyPer> per) [member function]
    cls.add_method('SetPerModelPhy2', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhyPer >', 'per')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetReceiveErrorCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, double, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveErrorCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetReceiveOkCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, double, ns3::UanTxMode, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveOkCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, ns3::UanTxMode, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetRxGainDb(double gain) [member function]
    cls.add_method('SetRxGainDb', 
                   'void', 
                   [param('double', 'gain')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetRxGainDbPhy1(double gain) [member function]
    cls.add_method('SetRxGainDbPhy1', 
                   'void', 
                   [param('double', 'gain')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetRxGainDbPhy2(double gain) [member function]
    cls.add_method('SetRxGainDbPhy2', 
                   'void', 
                   [param('double', 'gain')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetRxThresholdDb(double thresh) [member function]
    cls.add_method('SetRxThresholdDb', 
                   'void', 
                   [param('double', 'thresh')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetSinrModelPhy1(ns3::Ptr<ns3::UanPhyCalcSinr> calcSinr) [member function]
    cls.add_method('SetSinrModelPhy1', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhyCalcSinr >', 'calcSinr')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetSinrModelPhy2(ns3::Ptr<ns3::UanPhyCalcSinr> calcSinr) [member function]
    cls.add_method('SetSinrModelPhy2', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhyCalcSinr >', 'calcSinr')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetTransducer(ns3::Ptr<ns3::UanTransducer> trans) [member function]
    cls.add_method('SetTransducer', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanTransducer >', 'trans')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetTxPowerDb(double txpwr) [member function]
    cls.add_method('SetTxPowerDb', 
                   'void', 
                   [param('double', 'txpwr')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetTxPowerDbPhy1(double arg0) [member function]
    cls.add_method('SetTxPowerDbPhy1', 
                   'void', 
                   [param('double', 'arg0')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::SetTxPowerDbPhy2(double arg0) [member function]
    cls.add_method('SetTxPowerDbPhy2', 
                   'void', 
                   [param('double', 'arg0')])
    ## uan-phy-dual.h: void ns3::UanPhyDual::StartRxPacket(ns3::Ptr<ns3::Packet> pkt, double rxPowerDb, ns3::UanTxMode txMode, ns3::UanPdp pdp) [member function]
    cls.add_method('StartRxPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('double', 'rxPowerDb'), param('ns3::UanTxMode', 'txMode'), param('ns3::UanPdp', 'pdp')], 
                   is_virtual=True)
    ## uan-phy-dual.h: void ns3::UanPhyDual::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3UanPhyGen_methods(root_module, cls):
    ## uan-phy-gen.h: ns3::UanPhyGen::UanPhyGen(ns3::UanPhyGen const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyGen const &', 'arg0')])
    ## uan-phy-gen.h: ns3::UanPhyGen::UanPhyGen() [constructor]
    cls.add_constructor([])
    ## uan-phy-gen.h: void ns3::UanPhyGen::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::EnergyDepletionHandler() [member function]
    cls.add_method('EnergyDepletionHandler', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: double ns3::UanPhyGen::GetCcaThresholdDb() [member function]
    cls.add_method('GetCcaThresholdDb', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: ns3::Ptr<ns3::UanChannel> ns3::UanPhyGen::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::UanChannel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-phy-gen.h: static ns3::UanModesList ns3::UanPhyGen::GetDefaultModes() [member function]
    cls.add_method('GetDefaultModes', 
                   'ns3::UanModesList', 
                   [], 
                   is_static=True)
    ## uan-phy-gen.h: ns3::Ptr<ns3::UanNetDevice> ns3::UanPhyGen::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::UanNetDevice >', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: ns3::UanTxMode ns3::UanPhyGen::GetMode(uint32_t n) [member function]
    cls.add_method('GetMode', 
                   'ns3::UanTxMode', 
                   [param('uint32_t', 'n')], 
                   is_virtual=True)
    ## uan-phy-gen.h: uint32_t ns3::UanPhyGen::GetNModes() [member function]
    cls.add_method('GetNModes', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: ns3::Ptr<ns3::Packet> ns3::UanPhyGen::GetPacketRx() const [member function]
    cls.add_method('GetPacketRx', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-phy-gen.h: double ns3::UanPhyGen::GetRxGainDb() [member function]
    cls.add_method('GetRxGainDb', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: double ns3::UanPhyGen::GetRxThresholdDb() [member function]
    cls.add_method('GetRxThresholdDb', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: ns3::Ptr<ns3::UanTransducer> ns3::UanPhyGen::GetTransducer() [member function]
    cls.add_method('GetTransducer', 
                   'ns3::Ptr< ns3::UanTransducer >', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: double ns3::UanPhyGen::GetTxPowerDb() [member function]
    cls.add_method('GetTxPowerDb', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: static ns3::TypeId ns3::UanPhyGen::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-phy-gen.h: bool ns3::UanPhyGen::IsStateBusy() [member function]
    cls.add_method('IsStateBusy', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: bool ns3::UanPhyGen::IsStateCcaBusy() [member function]
    cls.add_method('IsStateCcaBusy', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: bool ns3::UanPhyGen::IsStateIdle() [member function]
    cls.add_method('IsStateIdle', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: bool ns3::UanPhyGen::IsStateRx() [member function]
    cls.add_method('IsStateRx', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: bool ns3::UanPhyGen::IsStateSleep() [member function]
    cls.add_method('IsStateSleep', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: bool ns3::UanPhyGen::IsStateTx() [member function]
    cls.add_method('IsStateTx', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::NotifyIntChange() [member function]
    cls.add_method('NotifyIntChange', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::NotifyTransStartTx(ns3::Ptr<ns3::Packet> packet, double txPowerDb, ns3::UanTxMode txMode) [member function]
    cls.add_method('NotifyTransStartTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'txPowerDb'), param('ns3::UanTxMode', 'txMode')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::RegisterListener(ns3::UanPhyListener * listener) [member function]
    cls.add_method('RegisterListener', 
                   'void', 
                   [param('ns3::UanPhyListener *', 'listener')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SendPacket(ns3::Ptr<ns3::Packet> pkt, uint32_t modeNum) [member function]
    cls.add_method('SendPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('uint32_t', 'modeNum')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetCcaThresholdDb(double thresh) [member function]
    cls.add_method('SetCcaThresholdDb', 
                   'void', 
                   [param('double', 'thresh')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetChannel(ns3::Ptr<ns3::UanChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanChannel >', 'channel')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetDevice(ns3::Ptr<ns3::UanNetDevice> device) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanNetDevice >', 'device')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetEnergyModelCallback(ns3::Callback<void, int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetEnergyModelCallback', 
                   'void', 
                   [param('ns3::Callback< void, int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetMac(ns3::Ptr<ns3::UanMac> mac) [member function]
    cls.add_method('SetMac', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanMac >', 'mac')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetReceiveErrorCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, double, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveErrorCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetReceiveOkCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, double, ns3::UanTxMode, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveOkCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, ns3::UanTxMode, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetRxGainDb(double gain) [member function]
    cls.add_method('SetRxGainDb', 
                   'void', 
                   [param('double', 'gain')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetRxThresholdDb(double thresh) [member function]
    cls.add_method('SetRxThresholdDb', 
                   'void', 
                   [param('double', 'thresh')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetTransducer(ns3::Ptr<ns3::UanTransducer> trans) [member function]
    cls.add_method('SetTransducer', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanTransducer >', 'trans')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::SetTxPowerDb(double txpwr) [member function]
    cls.add_method('SetTxPowerDb', 
                   'void', 
                   [param('double', 'txpwr')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::StartRxPacket(ns3::Ptr<ns3::Packet> pkt, double rxPowerDb, ns3::UanTxMode txMode, ns3::UanPdp pdp) [member function]
    cls.add_method('StartRxPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('double', 'rxPowerDb'), param('ns3::UanTxMode', 'txMode'), param('ns3::UanPdp', 'pdp')], 
                   is_virtual=True)
    ## uan-phy-gen.h: void ns3::UanPhyGen::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3UanPhyPer_methods(root_module, cls):
    ## uan-phy.h: ns3::UanPhyPer::UanPhyPer() [constructor]
    cls.add_constructor([])
    ## uan-phy.h: ns3::UanPhyPer::UanPhyPer(ns3::UanPhyPer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyPer const &', 'arg0')])
    ## uan-phy.h: double ns3::UanPhyPer::CalcPer(ns3::Ptr<ns3::Packet> pkt, double sinrDb, ns3::UanTxMode mode) [member function]
    cls.add_method('CalcPer', 
                   'double', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('double', 'sinrDb'), param('ns3::UanTxMode', 'mode')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-phy.h: void ns3::UanPhyPer::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-phy.h: void ns3::UanPhyPer::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3UanPhyPerGenDefault_methods(root_module, cls):
    ## uan-phy-gen.h: ns3::UanPhyPerGenDefault::UanPhyPerGenDefault(ns3::UanPhyPerGenDefault const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyPerGenDefault const &', 'arg0')])
    ## uan-phy-gen.h: ns3::UanPhyPerGenDefault::UanPhyPerGenDefault() [constructor]
    cls.add_constructor([])
    ## uan-phy-gen.h: double ns3::UanPhyPerGenDefault::CalcPer(ns3::Ptr<ns3::Packet> pkt, double sinrDb, ns3::UanTxMode mode) [member function]
    cls.add_method('CalcPer', 
                   'double', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('double', 'sinrDb'), param('ns3::UanTxMode', 'mode')], 
                   is_virtual=True)
    ## uan-phy-gen.h: static ns3::TypeId ns3::UanPhyPerGenDefault::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UanPhyPerUmodem_methods(root_module, cls):
    ## uan-phy-gen.h: ns3::UanPhyPerUmodem::UanPhyPerUmodem(ns3::UanPhyPerUmodem const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPhyPerUmodem const &', 'arg0')])
    ## uan-phy-gen.h: ns3::UanPhyPerUmodem::UanPhyPerUmodem() [constructor]
    cls.add_constructor([])
    ## uan-phy-gen.h: double ns3::UanPhyPerUmodem::CalcPer(ns3::Ptr<ns3::Packet> pkt, double sinrDb, ns3::UanTxMode mode) [member function]
    cls.add_method('CalcPer', 
                   'double', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('double', 'sinrDb'), param('ns3::UanTxMode', 'mode')], 
                   is_virtual=True)
    ## uan-phy-gen.h: static ns3::TypeId ns3::UanPhyPerUmodem::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UanPropModel_methods(root_module, cls):
    ## uan-prop-model.h: ns3::UanPropModel::UanPropModel() [constructor]
    cls.add_constructor([])
    ## uan-prop-model.h: ns3::UanPropModel::UanPropModel(ns3::UanPropModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPropModel const &', 'arg0')])
    ## uan-prop-model.h: void ns3::UanPropModel::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-prop-model.h: void ns3::UanPropModel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-prop-model.h: ns3::Time ns3::UanPropModel::GetDelay(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b, ns3::UanTxMode mode) [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b'), param('ns3::UanTxMode', 'mode')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-prop-model.h: double ns3::UanPropModel::GetPathLossDb(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b, ns3::UanTxMode txMode) [member function]
    cls.add_method('GetPathLossDb', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b'), param('ns3::UanTxMode', 'txMode')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-prop-model.h: ns3::UanPdp ns3::UanPropModel::GetPdp(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b, ns3::UanTxMode mode) [member function]
    cls.add_method('GetPdp', 
                   'ns3::UanPdp', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b'), param('ns3::UanTxMode', 'mode')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3UanPropModelIdeal_methods(root_module, cls):
    ## uan-prop-model-ideal.h: ns3::UanPropModelIdeal::UanPropModelIdeal(ns3::UanPropModelIdeal const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPropModelIdeal const &', 'arg0')])
    ## uan-prop-model-ideal.h: ns3::UanPropModelIdeal::UanPropModelIdeal() [constructor]
    cls.add_constructor([])
    ## uan-prop-model-ideal.h: ns3::Time ns3::UanPropModelIdeal::GetDelay(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b, ns3::UanTxMode mode) [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b'), param('ns3::UanTxMode', 'mode')], 
                   is_virtual=True)
    ## uan-prop-model-ideal.h: double ns3::UanPropModelIdeal::GetPathLossDb(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b, ns3::UanTxMode mode) [member function]
    cls.add_method('GetPathLossDb', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b'), param('ns3::UanTxMode', 'mode')], 
                   is_virtual=True)
    ## uan-prop-model-ideal.h: ns3::UanPdp ns3::UanPropModelIdeal::GetPdp(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b, ns3::UanTxMode mode) [member function]
    cls.add_method('GetPdp', 
                   'ns3::UanPdp', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b'), param('ns3::UanTxMode', 'mode')], 
                   is_virtual=True)
    ## uan-prop-model-ideal.h: static ns3::TypeId ns3::UanPropModelIdeal::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UanPropModelThorp_methods(root_module, cls):
    ## uan-prop-model-thorp.h: ns3::UanPropModelThorp::UanPropModelThorp(ns3::UanPropModelThorp const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanPropModelThorp const &', 'arg0')])
    ## uan-prop-model-thorp.h: ns3::UanPropModelThorp::UanPropModelThorp() [constructor]
    cls.add_constructor([])
    ## uan-prop-model-thorp.h: ns3::Time ns3::UanPropModelThorp::GetDelay(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b, ns3::UanTxMode mode) [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b'), param('ns3::UanTxMode', 'mode')], 
                   is_virtual=True)
    ## uan-prop-model-thorp.h: double ns3::UanPropModelThorp::GetPathLossDb(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b, ns3::UanTxMode mode) [member function]
    cls.add_method('GetPathLossDb', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b'), param('ns3::UanTxMode', 'mode')], 
                   is_virtual=True)
    ## uan-prop-model-thorp.h: ns3::UanPdp ns3::UanPropModelThorp::GetPdp(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b, ns3::UanTxMode mode) [member function]
    cls.add_method('GetPdp', 
                   'ns3::UanPdp', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b'), param('ns3::UanTxMode', 'mode')], 
                   is_virtual=True)
    ## uan-prop-model-thorp.h: static ns3::TypeId ns3::UanPropModelThorp::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UanTransducer_methods(root_module, cls):
    ## uan-transducer.h: ns3::UanTransducer::UanTransducer() [constructor]
    cls.add_constructor([])
    ## uan-transducer.h: ns3::UanTransducer::UanTransducer(ns3::UanTransducer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanTransducer const &', 'arg0')])
    ## uan-transducer.h: void ns3::UanTransducer::AddPhy(ns3::Ptr<ns3::UanPhy> phy) [member function]
    cls.add_method('AddPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'phy')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-transducer.h: void ns3::UanTransducer::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-transducer.h: std::list<ns3::UanPacketArrival, std::allocator<ns3::UanPacketArrival> > const & ns3::UanTransducer::GetArrivalList() const [member function]
    cls.add_method('GetArrivalList', 
                   'std::list< ns3::UanPacketArrival > const &', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-transducer.h: ns3::Ptr<ns3::UanChannel> ns3::UanTransducer::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::UanChannel >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-transducer.h: std::list<ns3::Ptr<ns3::UanPhy>, std::allocator<ns3::Ptr<ns3::UanPhy> > > const & ns3::UanTransducer::GetPhyList() const [member function]
    cls.add_method('GetPhyList', 
                   'std::list< ns3::Ptr< ns3::UanPhy > > const &', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-transducer.h: ns3::UanTransducer::State ns3::UanTransducer::GetState() const [member function]
    cls.add_method('GetState', 
                   'ns3::UanTransducer::State', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-transducer.h: bool ns3::UanTransducer::IsRx() const [member function]
    cls.add_method('IsRx', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-transducer.h: bool ns3::UanTransducer::IsTx() const [member function]
    cls.add_method('IsTx', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## uan-transducer.h: void ns3::UanTransducer::Receive(ns3::Ptr<ns3::Packet> packet, double rxPowerDb, ns3::UanTxMode txMode, ns3::UanPdp pdp) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'rxPowerDb'), param('ns3::UanTxMode', 'txMode'), param('ns3::UanPdp', 'pdp')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-transducer.h: void ns3::UanTransducer::SetChannel(ns3::Ptr<ns3::UanChannel> chan) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanChannel >', 'chan')], 
                   is_pure_virtual=True, is_virtual=True)
    ## uan-transducer.h: void ns3::UanTransducer::Transmit(ns3::Ptr<ns3::UanPhy> src, ns3::Ptr<ns3::Packet> packet, double txPowerDb, ns3::UanTxMode txMode) [member function]
    cls.add_method('Transmit', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'src'), param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'txPowerDb'), param('ns3::UanTxMode', 'txMode')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3UanTransducerHd_methods(root_module, cls):
    ## uan-transducer-hd.h: ns3::UanTransducerHd::UanTransducerHd(ns3::UanTransducerHd const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanTransducerHd const &', 'arg0')])
    ## uan-transducer-hd.h: ns3::UanTransducerHd::UanTransducerHd() [constructor]
    cls.add_constructor([])
    ## uan-transducer-hd.h: void ns3::UanTransducerHd::AddPhy(ns3::Ptr<ns3::UanPhy> arg0) [member function]
    cls.add_method('AddPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'arg0')], 
                   is_virtual=True)
    ## uan-transducer-hd.h: void ns3::UanTransducerHd::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## uan-transducer-hd.h: std::list<ns3::UanPacketArrival, std::allocator<ns3::UanPacketArrival> > const & ns3::UanTransducerHd::GetArrivalList() const [member function]
    cls.add_method('GetArrivalList', 
                   'std::list< ns3::UanPacketArrival > const &', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-transducer-hd.h: ns3::Ptr<ns3::UanChannel> ns3::UanTransducerHd::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::UanChannel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-transducer-hd.h: std::list<ns3::Ptr<ns3::UanPhy>, std::allocator<ns3::Ptr<ns3::UanPhy> > > const & ns3::UanTransducerHd::GetPhyList() const [member function]
    cls.add_method('GetPhyList', 
                   'std::list< ns3::Ptr< ns3::UanPhy > > const &', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-transducer-hd.h: ns3::UanTransducer::State ns3::UanTransducerHd::GetState() const [member function]
    cls.add_method('GetState', 
                   'ns3::UanTransducer::State', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-transducer-hd.h: static ns3::TypeId ns3::UanTransducerHd::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-transducer-hd.h: bool ns3::UanTransducerHd::IsRx() const [member function]
    cls.add_method('IsRx', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-transducer-hd.h: bool ns3::UanTransducerHd::IsTx() const [member function]
    cls.add_method('IsTx', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-transducer-hd.h: void ns3::UanTransducerHd::Receive(ns3::Ptr<ns3::Packet> packet, double rxPowerDb, ns3::UanTxMode txMode, ns3::UanPdp pdp) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'rxPowerDb'), param('ns3::UanTxMode', 'txMode'), param('ns3::UanPdp', 'pdp')], 
                   is_virtual=True)
    ## uan-transducer-hd.h: void ns3::UanTransducerHd::SetChannel(ns3::Ptr<ns3::UanChannel> chan) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanChannel >', 'chan')], 
                   is_virtual=True)
    ## uan-transducer-hd.h: void ns3::UanTransducerHd::Transmit(ns3::Ptr<ns3::UanPhy> src, ns3::Ptr<ns3::Packet> packet, double txPowerDb, ns3::UanTxMode txMode) [member function]
    cls.add_method('Transmit', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'src'), param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'txPowerDb'), param('ns3::UanTxMode', 'txMode')], 
                   is_virtual=True)
    ## uan-transducer-hd.h: void ns3::UanTransducerHd::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3UanChannel_methods(root_module, cls):
    ## uan-channel.h: ns3::UanChannel::UanChannel(ns3::UanChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanChannel const &', 'arg0')])
    ## uan-channel.h: ns3::UanChannel::UanChannel() [constructor]
    cls.add_constructor([])
    ## uan-channel.h: void ns3::UanChannel::AddDevice(ns3::Ptr<ns3::UanNetDevice> dev, ns3::Ptr<ns3::UanTransducer> trans) [member function]
    cls.add_method('AddDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanNetDevice >', 'dev'), param('ns3::Ptr< ns3::UanTransducer >', 'trans')])
    ## uan-channel.h: void ns3::UanChannel::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## uan-channel.h: ns3::Ptr<ns3::NetDevice> ns3::UanChannel::GetDevice(uint32_t i) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## uan-channel.h: uint32_t ns3::UanChannel::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-channel.h: double ns3::UanChannel::GetNoiseDbHz(double fKhz) [member function]
    cls.add_method('GetNoiseDbHz', 
                   'double', 
                   [param('double', 'fKhz')])
    ## uan-channel.h: static ns3::TypeId ns3::UanChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-channel.h: void ns3::UanChannel::SetNoiseModel(ns3::Ptr<ns3::UanNoiseModel> noise) [member function]
    cls.add_method('SetNoiseModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanNoiseModel >', 'noise')])
    ## uan-channel.h: void ns3::UanChannel::SetPropagationModel(ns3::Ptr<ns3::UanPropModel> prop) [member function]
    cls.add_method('SetPropagationModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPropModel >', 'prop')])
    ## uan-channel.h: void ns3::UanChannel::TxPacket(ns3::Ptr<ns3::UanTransducer> src, ns3::Ptr<ns3::Packet> packet, double txPowerDb, ns3::UanTxMode txmode) [member function]
    cls.add_method('TxPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanTransducer >', 'src'), param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'txPowerDb'), param('ns3::UanTxMode', 'txmode')])
    ## uan-channel.h: void ns3::UanChannel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3UanModesListChecker_methods(root_module, cls):
    ## uan-tx-mode.h: ns3::UanModesListChecker::UanModesListChecker() [constructor]
    cls.add_constructor([])
    ## uan-tx-mode.h: ns3::UanModesListChecker::UanModesListChecker(ns3::UanModesListChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanModesListChecker const &', 'arg0')])
    return

def register_Ns3UanModesListValue_methods(root_module, cls):
    ## uan-tx-mode.h: ns3::UanModesListValue::UanModesListValue() [constructor]
    cls.add_constructor([])
    ## uan-tx-mode.h: ns3::UanModesListValue::UanModesListValue(ns3::UanModesListValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanModesListValue const &', 'arg0')])
    ## uan-tx-mode.h: ns3::UanModesListValue::UanModesListValue(ns3::UanModesList const & value) [constructor]
    cls.add_constructor([param('ns3::UanModesList const &', 'value')])
    ## uan-tx-mode.h: ns3::Ptr<ns3::AttributeValue> ns3::UanModesListValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-tx-mode.h: bool ns3::UanModesListValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## uan-tx-mode.h: ns3::UanModesList ns3::UanModesListValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::UanModesList', 
                   [], 
                   is_const=True)
    ## uan-tx-mode.h: std::string ns3::UanModesListValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## uan-tx-mode.h: void ns3::UanModesListValue::Set(ns3::UanModesList const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::UanModesList const &', 'value')])
    return

def register_Ns3UanNetDevice_methods(root_module, cls):
    ## uan-net-device.h: ns3::UanNetDevice::UanNetDevice(ns3::UanNetDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UanNetDevice const &', 'arg0')])
    ## uan-net-device.h: ns3::UanNetDevice::UanNetDevice() [constructor]
    cls.add_constructor([])
    ## uan-net-device.h: void ns3::UanNetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## uan-net-device.h: ns3::Address ns3::UanNetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: ns3::Address ns3::UanNetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: ns3::Ptr<ns3::Channel> ns3::UanNetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: uint32_t ns3::UanNetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: ns3::Ptr<ns3::UanMac> ns3::UanNetDevice::GetMac() const [member function]
    cls.add_method('GetMac', 
                   'ns3::Ptr< ns3::UanMac >', 
                   [], 
                   is_const=True)
    ## uan-net-device.h: uint16_t ns3::UanNetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: ns3::Address ns3::UanNetDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: ns3::Address ns3::UanNetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: ns3::Ptr<ns3::Node> ns3::UanNetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: ns3::Ptr<ns3::UanPhy> ns3::UanNetDevice::GetPhy() const [member function]
    cls.add_method('GetPhy', 
                   'ns3::Ptr< ns3::UanPhy >', 
                   [], 
                   is_const=True)
    ## uan-net-device.h: ns3::Ptr<ns3::UanTransducer> ns3::UanNetDevice::GetTransducer() const [member function]
    cls.add_method('GetTransducer', 
                   'ns3::Ptr< ns3::UanTransducer >', 
                   [], 
                   is_const=True)
    ## uan-net-device.h: static ns3::TypeId ns3::UanNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## uan-net-device.h: bool ns3::UanNetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: bool ns3::UanNetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: bool ns3::UanNetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: bool ns3::UanNetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: bool ns3::UanNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: bool ns3::UanNetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: bool ns3::UanNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## uan-net-device.h: bool ns3::UanNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::SetChannel(ns3::Ptr<ns3::UanChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanChannel >', 'channel')])
    ## uan-net-device.h: void ns3::UanNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::SetMac(ns3::Ptr<ns3::UanMac> mac) [member function]
    cls.add_method('SetMac', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanMac >', 'mac')])
    ## uan-net-device.h: bool ns3::UanNetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::SetPhy(ns3::Ptr<ns3::UanPhy> phy) [member function]
    cls.add_method('SetPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanPhy >', 'phy')])
    ## uan-net-device.h: void ns3::UanNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::SetTransducer(ns3::Ptr<ns3::UanTransducer> trans) [member function]
    cls.add_method('SetTransducer', 
                   'void', 
                   [param('ns3::Ptr< ns3::UanTransducer >', 'trans')])
    ## uan-net-device.h: bool ns3::UanNetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## uan-net-device.h: void ns3::UanNetDevice::ForwardUp(ns3::Ptr<ns3::Packet> pkt, ns3::UanAddress const & src) [member function]
    cls.add_method('ForwardUp', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt'), param('ns3::UanAddress const &', 'src')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3AcousticModemEnergyModel_methods(root_module, cls):
    ## acoustic-modem-energy-model.h: ns3::AcousticModemEnergyModel::AcousticModemEnergyModel(ns3::AcousticModemEnergyModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AcousticModemEnergyModel const &', 'arg0')])
    ## acoustic-modem-energy-model.h: ns3::AcousticModemEnergyModel::AcousticModemEnergyModel() [constructor]
    cls.add_constructor([])
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::ChangeState(int newState) [member function]
    cls.add_method('ChangeState', 
                   'void', 
                   [param('int', 'newState')], 
                   is_virtual=True)
    ## acoustic-modem-energy-model.h: int ns3::AcousticModemEnergyModel::GetCurrentState() const [member function]
    cls.add_method('GetCurrentState', 
                   'int', 
                   [], 
                   is_const=True)
    ## acoustic-modem-energy-model.h: double ns3::AcousticModemEnergyModel::GetIdlePowerW() const [member function]
    cls.add_method('GetIdlePowerW', 
                   'double', 
                   [], 
                   is_const=True)
    ## acoustic-modem-energy-model.h: ns3::Ptr<ns3::Node> ns3::AcousticModemEnergyModel::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## acoustic-modem-energy-model.h: double ns3::AcousticModemEnergyModel::GetRxPowerW() const [member function]
    cls.add_method('GetRxPowerW', 
                   'double', 
                   [], 
                   is_const=True)
    ## acoustic-modem-energy-model.h: double ns3::AcousticModemEnergyModel::GetSleepPowerW() const [member function]
    cls.add_method('GetSleepPowerW', 
                   'double', 
                   [], 
                   is_const=True)
    ## acoustic-modem-energy-model.h: double ns3::AcousticModemEnergyModel::GetTotalEnergyConsumption() const [member function]
    cls.add_method('GetTotalEnergyConsumption', 
                   'double', 
                   [], 
                   is_const=True, is_virtual=True)
    ## acoustic-modem-energy-model.h: double ns3::AcousticModemEnergyModel::GetTxPowerW() const [member function]
    cls.add_method('GetTxPowerW', 
                   'double', 
                   [], 
                   is_const=True)
    ## acoustic-modem-energy-model.h: static ns3::TypeId ns3::AcousticModemEnergyModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::HandleEnergyDepletion() [member function]
    cls.add_method('HandleEnergyDepletion', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::SetEnergyDepletionCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetEnergyDepletionCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::SetEnergySource(ns3::Ptr<ns3::EnergySource> source) [member function]
    cls.add_method('SetEnergySource', 
                   'void', 
                   [param('ns3::Ptr< ns3::EnergySource >', 'source')], 
                   is_virtual=True)
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::SetIdlePowerW(double idlePowerW) [member function]
    cls.add_method('SetIdlePowerW', 
                   'void', 
                   [param('double', 'idlePowerW')])
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::SetRxPowerW(double rxPowerW) [member function]
    cls.add_method('SetRxPowerW', 
                   'void', 
                   [param('double', 'rxPowerW')])
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::SetSleepPowerW(double sleepPowerW) [member function]
    cls.add_method('SetSleepPowerW', 
                   'void', 
                   [param('double', 'sleepPowerW')])
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::SetTxPowerW(double txPowerW) [member function]
    cls.add_method('SetTxPowerW', 
                   'void', 
                   [param('double', 'txPowerW')])
    ## acoustic-modem-energy-model.h: void ns3::AcousticModemEnergyModel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## acoustic-modem-energy-model.h: double ns3::AcousticModemEnergyModel::DoGetCurrentA() const [member function]
    cls.add_method('DoGetCurrentA', 
                   'double', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_functions(root_module):
    module = root_module
    ## uan-tx-mode.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeUanModesListChecker() [free function]
    module.add_function('MakeUanModesListChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
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

