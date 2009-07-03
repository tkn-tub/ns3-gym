from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## data-output-interface.h: ns3::DataOutputCallback [class]
    module.add_class('DataOutputCallback', allow_subclassing=True)
    ## data-calculator.h: ns3::DataCalculator [class]
    module.add_class('DataCalculator', parent=root_module['ns3::Object'])
    ## data-collector.h: ns3::DataCollector [class]
    module.add_class('DataCollector', parent=root_module['ns3::Object'])
    ## data-output-interface.h: ns3::DataOutputInterface [class]
    module.add_class('DataOutputInterface', parent=root_module['ns3::Object'])
    ## basic-data-calculators.h: ns3::MinMaxAvgTotalCalculator<unsigned int> [class]
    module.add_class('MinMaxAvgTotalCalculator', template_parameters=['unsigned int'], parent=root_module['ns3::DataCalculator'])
    ## omnet-data-output.h: ns3::OmnetDataOutput [class]
    module.add_class('OmnetDataOutput', parent=root_module['ns3::DataOutputInterface'])
    ## packet-data-calculators.h: ns3::PacketSizeMinMaxAvgTotalCalculator [class]
    module.add_class('PacketSizeMinMaxAvgTotalCalculator', parent=root_module['ns3::MinMaxAvgTotalCalculator< unsigned int >'])
    ## sqlite-data-output.h: ns3::SqliteDataOutput [class]
    module.add_class('SqliteDataOutput', parent=root_module['ns3::DataOutputInterface'])
    ## time-data-calculators.h: ns3::TimeMinMaxAvgTotalCalculator [class]
    module.add_class('TimeMinMaxAvgTotalCalculator', parent=root_module['ns3::DataCalculator'])
    ## basic-data-calculators.h: ns3::CounterCalculator<unsigned int> [class]
    module.add_class('CounterCalculator', template_parameters=['unsigned int'], parent=root_module['ns3::DataCalculator'])
    ## packet-data-calculators.h: ns3::PacketCounterCalculator [class]
    module.add_class('PacketCounterCalculator', parent=root_module['ns3::CounterCalculator< unsigned int >'])
    typehandlers.add_type_alias('std::list< ns3::Ptr< ns3::DataCalculator >, std::allocator< ns3::Ptr< ns3::DataCalculator > > >', 'ns3::DataCalculatorList')
    typehandlers.add_type_alias('std::list< std::pair< std::string, std::string >, std::allocator< std::pair< std::string, std::string > > >', 'ns3::MetadataList')
    
    ## Register a nested module for the namespace Config
    
    nested_module = module.add_cpp_namespace('Config')
    register_types_ns3_Config(nested_module)
    
    
    ## Register a nested module for the namespace TimeStepPrecision
    
    nested_module = module.add_cpp_namespace('TimeStepPrecision')
    register_types_ns3_TimeStepPrecision(nested_module)
    
    
    ## Register a nested module for the namespace addressUtils
    
    nested_module = module.add_cpp_namespace('addressUtils')
    register_types_ns3_addressUtils(nested_module)
    
    
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
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3DataOutputCallback_methods(root_module, root_module['ns3::DataOutputCallback'])
    register_Ns3DataCalculator_methods(root_module, root_module['ns3::DataCalculator'])
    register_Ns3DataCollector_methods(root_module, root_module['ns3::DataCollector'])
    register_Ns3DataOutputInterface_methods(root_module, root_module['ns3::DataOutputInterface'])
    register_Ns3MinMaxAvgTotalCalculator__Unsigned_int_methods(root_module, root_module['ns3::MinMaxAvgTotalCalculator< unsigned int >'])
    register_Ns3OmnetDataOutput_methods(root_module, root_module['ns3::OmnetDataOutput'])
    register_Ns3PacketSizeMinMaxAvgTotalCalculator_methods(root_module, root_module['ns3::PacketSizeMinMaxAvgTotalCalculator'])
    register_Ns3SqliteDataOutput_methods(root_module, root_module['ns3::SqliteDataOutput'])
    register_Ns3TimeMinMaxAvgTotalCalculator_methods(root_module, root_module['ns3::TimeMinMaxAvgTotalCalculator'])
    register_Ns3CounterCalculator__Unsigned_int_methods(root_module, root_module['ns3::CounterCalculator< unsigned int >'])
    register_Ns3PacketCounterCalculator_methods(root_module, root_module['ns3::PacketCounterCalculator'])
    return

def register_Ns3DataOutputCallback_methods(root_module, cls):
    ## data-output-interface.h: ns3::DataOutputCallback::DataOutputCallback(ns3::DataOutputCallback const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DataOutputCallback const &', 'arg0')])
    ## data-output-interface.h: ns3::DataOutputCallback::DataOutputCallback() [constructor]
    cls.add_constructor([])
    ## data-output-interface.h: void ns3::DataOutputCallback::OutputSingleton(std::string key, std::string variable, int val) [member function]
    cls.add_method('OutputSingleton', 
                   'void', 
                   [param('std::string', 'key'), param('std::string', 'variable'), param('int', 'val')], 
                   is_pure_virtual=True, is_virtual=True)
    ## data-output-interface.h: void ns3::DataOutputCallback::OutputSingleton(std::string key, std::string variable, uint32_t val) [member function]
    cls.add_method('OutputSingleton', 
                   'void', 
                   [param('std::string', 'key'), param('std::string', 'variable'), param('uint32_t', 'val')], 
                   is_pure_virtual=True, is_virtual=True)
    ## data-output-interface.h: void ns3::DataOutputCallback::OutputSingleton(std::string key, std::string variable, double val) [member function]
    cls.add_method('OutputSingleton', 
                   'void', 
                   [param('std::string', 'key'), param('std::string', 'variable'), param('double', 'val')], 
                   is_pure_virtual=True, is_virtual=True)
    ## data-output-interface.h: void ns3::DataOutputCallback::OutputSingleton(std::string key, std::string variable, std::string val) [member function]
    cls.add_method('OutputSingleton', 
                   'void', 
                   [param('std::string', 'key'), param('std::string', 'variable'), param('std::string', 'val')], 
                   is_pure_virtual=True, is_virtual=True)
    ## data-output-interface.h: void ns3::DataOutputCallback::OutputSingleton(std::string key, std::string variable, ns3::Time val) [member function]
    cls.add_method('OutputSingleton', 
                   'void', 
                   [param('std::string', 'key'), param('std::string', 'variable'), param('ns3::Time', 'val')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3DataCalculator_methods(root_module, cls):
    ## data-calculator.h: ns3::DataCalculator::DataCalculator(ns3::DataCalculator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DataCalculator const &', 'arg0')])
    ## data-calculator.h: ns3::DataCalculator::DataCalculator() [constructor]
    cls.add_constructor([])
    ## data-calculator.h: bool ns3::DataCalculator::GetEnabled() const [member function]
    cls.add_method('GetEnabled', 
                   'bool', 
                   [], 
                   is_const=True)
    ## data-calculator.h: void ns3::DataCalculator::Enable() [member function]
    cls.add_method('Enable', 
                   'void', 
                   [])
    ## data-calculator.h: void ns3::DataCalculator::Disable() [member function]
    cls.add_method('Disable', 
                   'void', 
                   [])
    ## data-calculator.h: void ns3::DataCalculator::SetKey(std::string const key) [member function]
    cls.add_method('SetKey', 
                   'void', 
                   [param('std::string const', 'key')])
    ## data-calculator.h: std::string ns3::DataCalculator::GetKey() const [member function]
    cls.add_method('GetKey', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## data-calculator.h: void ns3::DataCalculator::Start(ns3::Time const & startTime) [member function]
    cls.add_method('Start', 
                   'void', 
                   [param('ns3::Time const &', 'startTime')], 
                   is_virtual=True)
    ## data-calculator.h: void ns3::DataCalculator::Stop(ns3::Time const & stopTime) [member function]
    cls.add_method('Stop', 
                   'void', 
                   [param('ns3::Time const &', 'stopTime')], 
                   is_virtual=True)
    ## data-calculator.h: void ns3::DataCalculator::Output(ns3::DataOutputCallback & callback) const [member function]
    cls.add_method('Output', 
                   'void', 
                   [param('ns3::DataOutputCallback &', 'callback')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## data-calculator.h: void ns3::DataCalculator::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3DataCollector_methods(root_module, cls):
    ## data-collector.h: ns3::DataCollector::DataCollector(ns3::DataCollector const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DataCollector const &', 'arg0')])
    ## data-collector.h: ns3::DataCollector::DataCollector() [constructor]
    cls.add_constructor([])
    ## data-collector.h: void ns3::DataCollector::DescribeRun(std::string experiment, std::string strategy, std::string input, std::string runID, std::string description="") [member function]
    cls.add_method('DescribeRun', 
                   'void', 
                   [param('std::string', 'experiment'), param('std::string', 'strategy'), param('std::string', 'input'), param('std::string', 'runID'), param('std::string', 'description', default_value='""')])
    ## data-collector.h: std::string ns3::DataCollector::GetExperimentLabel() const [member function]
    cls.add_method('GetExperimentLabel', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## data-collector.h: std::string ns3::DataCollector::GetStrategyLabel() const [member function]
    cls.add_method('GetStrategyLabel', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## data-collector.h: std::string ns3::DataCollector::GetInputLabel() const [member function]
    cls.add_method('GetInputLabel', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## data-collector.h: std::string ns3::DataCollector::GetRunLabel() const [member function]
    cls.add_method('GetRunLabel', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## data-collector.h: std::string ns3::DataCollector::GetDescription() const [member function]
    cls.add_method('GetDescription', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## data-collector.h: void ns3::DataCollector::AddMetadata(std::string key, std::string value) [member function]
    cls.add_method('AddMetadata', 
                   'void', 
                   [param('std::string', 'key'), param('std::string', 'value')])
    ## data-collector.h: void ns3::DataCollector::AddMetadata(std::string key, double value) [member function]
    cls.add_method('AddMetadata', 
                   'void', 
                   [param('std::string', 'key'), param('double', 'value')])
    ## data-collector.h: void ns3::DataCollector::AddMetadata(std::string key, uint32_t value) [member function]
    cls.add_method('AddMetadata', 
                   'void', 
                   [param('std::string', 'key'), param('uint32_t', 'value')])
    ## data-collector.h: std::_List_iterator<std::pair<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > ns3::DataCollector::MetadataBegin() [member function]
    cls.add_method('MetadataBegin', 
                   'std::_List_iterator< std::pair< std::basic_string< char, std::char_traits< char >, std::allocator< char > >, std::basic_string< char, std::char_traits< char >, std::allocator< char > > > >', 
                   [])
    ## data-collector.h: std::_List_iterator<std::pair<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > ns3::DataCollector::MetadataEnd() [member function]
    cls.add_method('MetadataEnd', 
                   'std::_List_iterator< std::pair< std::basic_string< char, std::char_traits< char >, std::allocator< char > >, std::basic_string< char, std::char_traits< char >, std::allocator< char > > > >', 
                   [])
    ## data-collector.h: void ns3::DataCollector::AddDataCalculator(ns3::Ptr<ns3::DataCalculator> datac) [member function]
    cls.add_method('AddDataCalculator', 
                   'void', 
                   [param('ns3::Ptr< ns3::DataCalculator >', 'datac')])
    ## data-collector.h: std::_List_iterator<ns3::Ptr<ns3::DataCalculator> > ns3::DataCollector::DataCalculatorBegin() [member function]
    cls.add_method('DataCalculatorBegin', 
                   'std::_List_iterator< ns3::Ptr< ns3::DataCalculator > >', 
                   [])
    ## data-collector.h: std::_List_iterator<ns3::Ptr<ns3::DataCalculator> > ns3::DataCollector::DataCalculatorEnd() [member function]
    cls.add_method('DataCalculatorEnd', 
                   'std::_List_iterator< ns3::Ptr< ns3::DataCalculator > >', 
                   [])
    ## data-collector.h: void ns3::DataCollector::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3DataOutputInterface_methods(root_module, cls):
    ## data-output-interface.h: ns3::DataOutputInterface::DataOutputInterface(ns3::DataOutputInterface const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DataOutputInterface const &', 'arg0')])
    ## data-output-interface.h: ns3::DataOutputInterface::DataOutputInterface() [constructor]
    cls.add_constructor([])
    ## data-output-interface.h: void ns3::DataOutputInterface::Output(ns3::DataCollector & dc) [member function]
    cls.add_method('Output', 
                   'void', 
                   [param('ns3::DataCollector &', 'dc')], 
                   is_pure_virtual=True, is_virtual=True)
    ## data-output-interface.h: void ns3::DataOutputInterface::SetFilePrefix(std::string const prefix) [member function]
    cls.add_method('SetFilePrefix', 
                   'void', 
                   [param('std::string const', 'prefix')])
    ## data-output-interface.h: std::string ns3::DataOutputInterface::GetFilePrefix() const [member function]
    cls.add_method('GetFilePrefix', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## data-output-interface.h: void ns3::DataOutputInterface::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3MinMaxAvgTotalCalculator__Unsigned_int_methods(root_module, cls):
    ## basic-data-calculators.h: ns3::MinMaxAvgTotalCalculator<unsigned int>::MinMaxAvgTotalCalculator(ns3::MinMaxAvgTotalCalculator<unsigned int> const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MinMaxAvgTotalCalculator< unsigned int > const &', 'arg0')])
    ## basic-data-calculators.h: ns3::MinMaxAvgTotalCalculator<unsigned int>::MinMaxAvgTotalCalculator() [constructor]
    cls.add_constructor([])
    ## basic-data-calculators.h: void ns3::MinMaxAvgTotalCalculator<unsigned int>::Update(unsigned int const i) [member function]
    cls.add_method('Update', 
                   'void', 
                   [param('unsigned int const', 'i')])
    ## basic-data-calculators.h: void ns3::MinMaxAvgTotalCalculator<unsigned int>::Output(ns3::DataOutputCallback & callback) const [member function]
    cls.add_method('Output', 
                   'void', 
                   [param('ns3::DataOutputCallback &', 'callback')], 
                   is_const=True, is_virtual=True)
    ## basic-data-calculators.h: void ns3::MinMaxAvgTotalCalculator<unsigned int>::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3OmnetDataOutput_methods(root_module, cls):
    ## omnet-data-output.h: ns3::OmnetDataOutput::OmnetDataOutput(ns3::OmnetDataOutput const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OmnetDataOutput const &', 'arg0')])
    ## omnet-data-output.h: ns3::OmnetDataOutput::OmnetDataOutput() [constructor]
    cls.add_constructor([])
    ## omnet-data-output.h: void ns3::OmnetDataOutput::Output(ns3::DataCollector & dc) [member function]
    cls.add_method('Output', 
                   'void', 
                   [param('ns3::DataCollector &', 'dc')], 
                   is_virtual=True)
    ## omnet-data-output.h: void ns3::OmnetDataOutput::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3PacketSizeMinMaxAvgTotalCalculator_methods(root_module, cls):
    ## packet-data-calculators.h: ns3::PacketSizeMinMaxAvgTotalCalculator::PacketSizeMinMaxAvgTotalCalculator(ns3::PacketSizeMinMaxAvgTotalCalculator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketSizeMinMaxAvgTotalCalculator const &', 'arg0')])
    ## packet-data-calculators.h: ns3::PacketSizeMinMaxAvgTotalCalculator::PacketSizeMinMaxAvgTotalCalculator() [constructor]
    cls.add_constructor([])
    ## packet-data-calculators.h: void ns3::PacketSizeMinMaxAvgTotalCalculator::PacketUpdate(std::string path, ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('PacketUpdate', 
                   'void', 
                   [param('std::string', 'path'), param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## packet-data-calculators.h: void ns3::PacketSizeMinMaxAvgTotalCalculator::FrameUpdate(std::string path, ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address realto) [member function]
    cls.add_method('FrameUpdate', 
                   'void', 
                   [param('std::string', 'path'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'realto')])
    ## packet-data-calculators.h: void ns3::PacketSizeMinMaxAvgTotalCalculator::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3SqliteDataOutput_methods(root_module, cls):
    ## sqlite-data-output.h: ns3::SqliteDataOutput::SqliteDataOutput(ns3::SqliteDataOutput const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SqliteDataOutput const &', 'arg0')])
    ## sqlite-data-output.h: ns3::SqliteDataOutput::SqliteDataOutput() [constructor]
    cls.add_constructor([])
    ## sqlite-data-output.h: void ns3::SqliteDataOutput::Output(ns3::DataCollector & dc) [member function]
    cls.add_method('Output', 
                   'void', 
                   [param('ns3::DataCollector &', 'dc')], 
                   is_virtual=True)
    ## sqlite-data-output.h: void ns3::SqliteDataOutput::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3TimeMinMaxAvgTotalCalculator_methods(root_module, cls):
    ## time-data-calculators.h: ns3::TimeMinMaxAvgTotalCalculator::TimeMinMaxAvgTotalCalculator(ns3::TimeMinMaxAvgTotalCalculator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TimeMinMaxAvgTotalCalculator const &', 'arg0')])
    ## time-data-calculators.h: ns3::TimeMinMaxAvgTotalCalculator::TimeMinMaxAvgTotalCalculator() [constructor]
    cls.add_constructor([])
    ## time-data-calculators.h: void ns3::TimeMinMaxAvgTotalCalculator::Update(ns3::Time const i) [member function]
    cls.add_method('Update', 
                   'void', 
                   [param('ns3::Time const', 'i')])
    ## time-data-calculators.h: void ns3::TimeMinMaxAvgTotalCalculator::Output(ns3::DataOutputCallback & callback) const [member function]
    cls.add_method('Output', 
                   'void', 
                   [param('ns3::DataOutputCallback &', 'callback')], 
                   is_const=True, is_virtual=True)
    ## time-data-calculators.h: void ns3::TimeMinMaxAvgTotalCalculator::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3CounterCalculator__Unsigned_int_methods(root_module, cls):
    ## basic-data-calculators.h: ns3::CounterCalculator<unsigned int>::CounterCalculator(ns3::CounterCalculator<unsigned int> const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CounterCalculator< unsigned int > const &', 'arg0')])
    ## basic-data-calculators.h: ns3::CounterCalculator<unsigned int>::CounterCalculator() [constructor]
    cls.add_constructor([])
    ## basic-data-calculators.h: void ns3::CounterCalculator<unsigned int>::Update() [member function]
    cls.add_method('Update', 
                   'void', 
                   [])
    ## basic-data-calculators.h: void ns3::CounterCalculator<unsigned int>::Update(unsigned int const i) [member function]
    cls.add_method('Update', 
                   'void', 
                   [param('unsigned int const', 'i')])
    ## basic-data-calculators.h: unsigned int ns3::CounterCalculator<unsigned int>::GetCount() const [member function]
    cls.add_method('GetCount', 
                   'unsigned int', 
                   [], 
                   is_const=True)
    ## basic-data-calculators.h: void ns3::CounterCalculator<unsigned int>::Output(ns3::DataOutputCallback & callback) const [member function]
    cls.add_method('Output', 
                   'void', 
                   [param('ns3::DataOutputCallback &', 'callback')], 
                   is_const=True, is_virtual=True)
    ## basic-data-calculators.h: void ns3::CounterCalculator<unsigned int>::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3PacketCounterCalculator_methods(root_module, cls):
    ## packet-data-calculators.h: ns3::PacketCounterCalculator::PacketCounterCalculator(ns3::PacketCounterCalculator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketCounterCalculator const &', 'arg0')])
    ## packet-data-calculators.h: ns3::PacketCounterCalculator::PacketCounterCalculator() [constructor]
    cls.add_constructor([])
    ## packet-data-calculators.h: void ns3::PacketCounterCalculator::PacketUpdate(std::string path, ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('PacketUpdate', 
                   'void', 
                   [param('std::string', 'path'), param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## packet-data-calculators.h: void ns3::PacketCounterCalculator::FrameUpdate(std::string path, ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address realto) [member function]
    cls.add_method('FrameUpdate', 
                   'void', 
                   [param('std::string', 'path'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'realto')])
    ## packet-data-calculators.h: void ns3::PacketCounterCalculator::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_functions(root_module):
    module = root_module
    register_functions_ns3_Config(module.get_submodule('Config'), root_module)
    register_functions_ns3_TimeStepPrecision(module.get_submodule('TimeStepPrecision'), root_module)
    register_functions_ns3_addressUtils(module.get_submodule('addressUtils'), root_module)
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    register_functions_ns3_olsr(module.get_submodule('olsr'), root_module)
    return

def register_functions_ns3_Config(module, root_module):
    return

def register_functions_ns3_TimeStepPrecision(module, root_module):
    return

def register_functions_ns3_addressUtils(module, root_module):
    return

def register_functions_ns3_internal(module, root_module):
    return

def register_functions_ns3_olsr(module, root_module):
    return

